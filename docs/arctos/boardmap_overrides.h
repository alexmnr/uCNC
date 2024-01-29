#ifndef BOADMAP_OVERRIDES_H
#define BOADMAP_OVERRIDES_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "boardmap_reset.h"
#define MCU MCU_AVR
#define KINEMATIC KINEMATIC_CARTESIAN
#define AXIS_COUNT 6
#define TOOL_COUNT 1
#define BAUDRATE 115200
#define S_CURVE_ACCELERATION_LEVEL 0
#define BOARD BOARD_RAMPS14
#define BOARD_NAME "RAMPS 1.4"
#define UART_PORT 0
#define UART2_PORT 0
#define ITP_TIMER 1
#define RTC_TIMER 0
#define ONESHOT_TIMER 4
#define STEP0_BIT 0
#define STEP0_PORT F
#define STEP1_BIT 6
#define STEP1_PORT F
#define STEP2_BIT 3
#define STEP2_PORT L
#define STEP3_BIT 4
#define STEP3_PORT A
#define STEP4_BIT 1
#define STEP4_PORT C
#define DIR0_BIT 1
#define DIR0_PORT F
#define DIR1_BIT 7
#define DIR1_PORT F
#define DIR2_BIT 1
#define DIR2_PORT L
#define DIR3_BIT 6
#define DIR3_PORT A
#define DIR4_BIT 3
#define DIR4_PORT C
#define STEP0_EN_BIT 7
#define STEP0_EN_PORT D
#define STEP1_EN_BIT 2
#define STEP1_EN_PORT F
#define STEP2_EN_BIT 0
#define STEP2_EN_PORT K
#define STEP3_EN_BIT 2
#define STEP3_EN_PORT A
#define STEP4_EN_BIT 7
#define STEP4_EN_PORT C
#define PWM0_BIT 5
#define PWM0_PORT H
#define PWM0_CHANNEL C
#define PWM0_TIMER 4
#define PWM1_BIT 6
#define PWM1_PORT H
#define PWM1_CHANNEL B
#define PWM1_TIMER 2
#define PWM2_BIT 4
#define PWM2_PORT B
#define PWM2_CHANNEL A
#define PWM2_TIMER 2
#define SERVO0_BIT 5
#define SERVO0_PORT B
#define SERVO1_BIT 3
#define SERVO1_PORT H
#define SERVO2_BIT 3
#define SERVO2_PORT E
#define SERVO3_BIT 5
#define SERVO3_PORT G
#define DOUT4_BIT 1
#define DOUT4_PORT A
#define DOUT5_BIT 0
#define DOUT5_PORT H
#define DOUT6_BIT 1
#define DOUT6_PORT H
#define DOUT7_BIT 0
#define DOUT7_PORT C
#define DOUT20_BIT 1
#define DOUT20_PORT G
#define DOUT21_BIT 5
#define DOUT21_PORT F
#define DOUT22_BIT 7
#define DOUT22_PORT L
#define DOUT23_BIT 5
#define DOUT23_PORT L
#define DOUT31_BIT 7
#define DOUT31_PORT B
#define LIMIT_X_BIT 5
#define LIMIT_X_PORT E
#define LIMIT_X_PULLUP
#define LIMIT_X_ISR -6
#define LIMIT_Y_BIT 4
#define LIMIT_Y_PORT E
#define LIMIT_Y_PULLUP
#define LIMIT_Y_ISR -6
#define LIMIT_Z_BIT 1
#define LIMIT_Z_PORT J
#define LIMIT_Z_PULLUP
#define LIMIT_Z_ISR -4
#define LIMIT_A_BIT 0
#define LIMIT_A_PORT J
#define LIMIT_A_PULLUP
#define LIMIT_A_ISR -4
#define LIMIT_B_BIT 2
#define LIMIT_B_PORT D
#define LIMIT_B_PULLUP
#define LIMIT_B_ISR 1
#define LIMIT_C_BIT 3
#define LIMIT_C_PORT D
#define LIMIT_C_PULLUP
#define LIMIT_C_ISR 1
#define PROBE_PULLUP
#define ESTOP_PULLUP
#define SAFETY_DOOR_PULLUP
#define FHOLD_PULLUP
#define CS_RES_PULLUP
#define DIN16_BIT 2
#define DIN16_PORT C
#define DIN16_PULLUP
#define DIN17_BIT 4
#define DIN17_PORT C
#define DIN17_PULLUP
#define DIN18_BIT 6
#define DIN18_PORT C
#define DIN18_PULLUP
#define DIN19_BIT 0
#define DIN19_PORT L
#define DIN19_PULLUP
#define DIN20_BIT 1
#define DIN20_PORT K
#define DIN20_PULLUP
#define DIN21_BIT 2
#define DIN21_PORT K
#define DIN21_PULLUP
#define DIN22_BIT 3
#define DIN22_PORT K
#define DIN22_PULLUP
#define DIN23_BIT 4
#define DIN23_PORT K
#define DIN23_PULLUP
#define TX_BIT 1
#define TX_PORT E
#define RX_BIT 0
#define RX_PORT E
#define SPI_CLK_BIT 1
#define SPI_CLK_PORT B
#define SPI_SDI_BIT 3
#define SPI_SDI_PORT B
#define SPI_SDO_BIT 2
#define SPI_SDO_PORT B
#define SPI_CS_BIT 0
#define SPI_CS_PORT B

#ifdef __cplusplus
}
#endif
#endif
