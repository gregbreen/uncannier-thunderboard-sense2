#!/bin/sh

###-----------------------------------------------------------------------------
###
### Post-build step to produce OTA DFU GBL files and a consolidated Flash image file
###
### Copyright (c) Uncannier Software 2018
###
###-----------------------------------------------------------------------------

# Extract S-record files for the apploader and application individually (so they can both be signed) 
arm-none-eabi-objcopy -O srec -j .text_apploader* "${1}/${2}.axf" "${1}/apploader.srec" >/dev/null
arm-none-eabi-objcopy -O srec -j .text_application* "${1}/${2}.axf" "${1}/application.srec" >/dev/null

# Names of the  key files
GBL_SIGNING_KEY_FILE="key-signing"
GBL_ENCRYPT_KEY_FILE="key-encryption-tokens.txt"

# Sign the apploader and application (so the bootloader can run them)
commander convert "${1}/apploader.srec" --secureboot --keyfile ${GBL_SIGNING_KEY_FILE} -o "${1}/apploader-signed.srec" >/dev/null
commander convert "${1}/application.srec" --secureboot --keyfile ${GBL_SIGNING_KEY_FILE} -o "${1}/application-signed.srec" >/dev/null

# Create a combined image HEX file - bootloader (first and second stage), signed apploader and signed application - this is flashable
commander convert ../uts-bootloader/Release/uts-bootloader-combined.s37 ./$1/apploader-signed.srec ./$1/application-signed.srec --outfile ./$1/uts-image.hex

# Create signed and encrypted GBL files for OTA updates
commander gbl create "${1}/uts-apploader.gbl" --app "${1}/apploader-signed.srec" --encrypt ${GBL_ENCRYPT_KEY_FILE} --sign ${GBL_SIGNING_KEY_FILE} >/dev/null
commander gbl create "${1}/uts-application.gbl" --app "${1}/application-signed.srec" --encrypt ${GBL_ENCRYPT_KEY_FILE} --sign ${GBL_SIGNING_KEY_FILE} >/dev/null

# Clean up the intermediate files
rm ${1}/*.srec
