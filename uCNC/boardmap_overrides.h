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
#define STEP0_BIT 1
#define STEP0_PORT F
#define STEP1_BIT 4
#define STEP1_PORT F
#define STEP2_BIT 2
#define STEP2_PORT L
#define STEP3_BIT 3
#define STEP3_PORT C
#define STEP4_BIT 6
#define STEP4_PORT A
#define STEP5_BIT 3
#define STEP5_PORT A
#define DIR0_BIT 2
#define DIR0_PORT F
#define DIR1_BIT 5
#define DIR1_PORT F
#define DIR2_BIT 4
#define DIR2_PORT L
#define DIR3_BIT 5
#define DIR3_PORT C
#define DIR4_BIT 4
#define DIR4_PORT A
#define DIR5_BIT 5
#define DIR5_PORT A
#define STEP0_EN_BIT 0
#define STEP0_EN_PORT F
#define STEP1_EN_BIT 3
#define STEP1_EN_PORT F
#define STEP2_EN_BIT 0
#define STEP2_EN_PORT L
#define STEP3_EN_BIT 1
#define STEP3_EN_PORT C
#define STEP4_EN_BIT 3
#define STEP4_EN_PORT C
#define STEP5_EN_BIT 1
#define STEP5_EN_PORT A
#define PWM0_BIT 6
#define PWM0_PORT B
#define PWM0_CHANNEL C
#define PWM0_TIMER 4
#define PWM1_BIT 5
#define PWM1_PORT B
#define PWM1_CHANNEL B
#define PWM1_TIMER 2
#define PWM2_BIT 4
#define PWM2_PORT B
#define PWM2_CHANNEL A
#define PWM2_TIMER 2
#define DOUT0_BIT 0
#define DOUT0_PORT A
#define LIMIT_X_BIT 0
#define LIMIT_X_PORT K
#define LIMIT_X_PULLUP
#define LIMIT_X_ISR 2
#define LIMIT_Y_BIT 1
#define LIMIT_Y_PORT K
#define LIMIT_Y_PULLUP
#define LIMIT_Y_ISR 2
#define LIMIT_Z_BIT 2
#define LIMIT_Z_PORT K
#define LIMIT_Z_PULLUP
#define LIMIT_Z_ISR 2
#define LIMIT_A_BIT 3
#define LIMIT_A_PORT K
#define LIMIT_A_PULLUP
#define LIMIT_A_ISR 2
#define LIMIT_B_BIT 4
#define LIMIT_B_PORT K
#define LIMIT_B_PULLUP
#define LIMIT_B_ISR 2
#define LIMIT_C_BIT 5
#define LIMIT_C_PORT K
#define LIMIT_C_PULLUP
#define LIMIT_C_ISR 2
#define PROBE_PULLUP
#define ESTOP_PULLUP
#define SAFETY_DOOR_PULLUP
#define FHOLD_PULLUP
#define CS_RES_PULLUP
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
