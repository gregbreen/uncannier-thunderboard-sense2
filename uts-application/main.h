#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#ifndef RGBLED_INIT_DEFAULT
#define RGBLED_INIT_DEFAULT                 \
  {                                         \
    TIMER0,           /* Timer */           \
    cmuClock_TIMER0,  /* Timer CMU clock */ \
    gpioPortD,        /* Red GPIO port   */ \
    11,               /* Red GPIO pin    */ \
    19,               /* TIM0_CC0 #19    */ \
    gpioPortD,        /* Green GPIO port */ \
    12,               /* Green GPIO pin  */ \
    19,               /* TIM0_CC1 #19    */ \
    gpioPortD,        /* Blue GPIO port  */ \
    13,               /* Blue GPIO pin   */ \
    19                /* TIM0_CC2 #19    */ \
  }
#endif

#define BALL_PACKET_TYPE_DIM_NODE       0x01
#define BALL_PACKET_TYPE_FREEFALL       0x02
#define BALL_PACKET_TYPE_MOVEMENT       0x03
#define BALL_PACKET_TYPE_SENSOR_READING 0x04

#define NODE_ROLE_REGULAR            0
#define NODE_ROLE_MASTER             1
#define NODE_ROLE_RADIOBOARD         2
#define NODE_ROLE_TELEMETRY_SENDER   3
#define NODE_ROLE_TELEMETRY_RECEIVER 4

typedef struct __BallPacket {
  uint8_t packetType;
  uint8_t payload[15];
} BallPacket;

void MAIN_initSensors(void);
void MAIN_deInitSensors(void);

#endif
