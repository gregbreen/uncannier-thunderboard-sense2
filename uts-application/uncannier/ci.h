///-----------------------------------------------------------------------------
///
/// @file ci.h
///
/// @brief Defines the software version elements to be created by the CI server
///
/// @copyright Copyright (c) Uncannier Software 2019
///
///-----------------------------------------------------------------------------

#ifndef CI_H_
#define CI_H_

// The default version number - overridden by CI builds
#define CI_VERSION          "0.0.0"

// The default build number - overridden by CI builds
#define CI_BUILD            0

// The default Git commit hash (as an integer) - overridden by CI builds
#define CI_COMMIT           0x0000000

#endif // CI_H_
