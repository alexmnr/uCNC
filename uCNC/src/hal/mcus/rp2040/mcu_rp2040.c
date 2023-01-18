/*
	Name: mcu_rpi_pico.c
	Description: Implements the µCNC HAL for ESP8266.

	Copyright: Copyright (c) João Martins
	Author: João Martins
	Date: 05-02-2022

	µCNC is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version. Please see <http://www.gnu.org/licenses/>

	µCNC is distributed WITHOUT ANY WARRANTY;
	Also without the implied warranty of	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	See the	GNU General Public License for more details.
*/

#include "../../../cnc.h"

#if (MCU == MCU_RP2040)
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "rp2040_arduino.h"

static volatile bool rp2040_global_isr_enabled;
static volatile uint32_t mcu_runtime_ms;

#ifdef MCU_HAS_ONESHOT_TIMER
static uint32_t rp2040_oneshot_counter;
static uint32_t rp2040_oneshot_reload;
static void mcu_gen_oneshot(void)
{
	if (rp2040_oneshot_counter)
	{
		rp2040_oneshot_counter--;
		if (!rp2040_oneshot_counter)
		{
			if (mcu_timeout_cb)
			{
				mcu_timeout_cb();
			}
		}
	}
}
#endif

uint8_t rp2040_pwm[16];
static void mcu_gen_pwm(void)
{
	static uint8_t pwm_counter = 0;
	// software PWM
	if (++pwm_counter < 127)
	{
#if ASSERT_PIN(PWM0)
		if (pwm_counter > rp2040_pwm[0])
		{
			mcu_clear_output(PWM0);
		}
#endif
#if ASSERT_PIN(PWM1)
		if (pwm_counter > rp2040_pwm[1])
		{
			mcu_clear_output(PWM1);
		}
#endif
#if ASSERT_PIN(PWM2)
		if (pwm_counter > rp2040_pwm[2])
		{
			mcu_clear_output(PWM2);
		}
#endif
#if ASSERT_PIN(PWM3)
		if (pwm_counter > rp2040_pwm[3])
		{
			mcu_clear_output(PWM3);
		}
#endif
#if ASSERT_PIN(PWM4)
		if (pwm_counter > rp2040_pwm[4])
		{
			mcu_clear_output(PWM4);
		}
#endif
#if ASSERT_PIN(PWM5)
		if (pwm_counter > rp2040_pwm[5])
		{
			mcu_clear_output(PWM5);
		}
#endif
#if ASSERT_PIN(PWM6)
		if (pwm_counter > rp2040_pwm[6])
		{
			mcu_clear_output(PWM6);
		}
#endif
#if ASSERT_PIN(PWM7)
		if (pwm_counter > rp2040_pwm[7])
		{
			mcu_clear_output(PWM7);
		}
#endif
#if ASSERT_PIN(PWM8)
		if (pwm_counter > rp2040_pwm[8])
		{
			mcu_clear_output(PWM8);
		}
#endif
#if ASSERT_PIN(PWM9)
		if (pwm_counter > rp2040_pwm[9])
		{
			mcu_clear_output(PWM9);
		}
#endif
#if ASSERT_PIN(PWM10)
		if (pwm_counter > rp2040_pwm[10])
		{
			mcu_clear_output(PWM10);
		}
#endif
#if ASSERT_PIN(PWM11)
		if (pwm_counter > rp2040_pwm[11])
		{
			mcu_clear_output(PWM11);
		}
#endif
#if ASSERT_PIN(PWM12)
		if (pwm_counter > rp2040_pwm[12])
		{
			mcu_clear_output(PWM12);
		}
#endif
#if ASSERT_PIN(PWM13)
		if (pwm_counter > rp2040_pwm[13])
		{
			mcu_clear_output(PWM13);
		}
#endif
#if ASSERT_PIN(PWM14)
		if (pwm_counter > rp2040_pwm[14])
		{
			mcu_clear_output(PWM14);
		}
#endif
#if ASSERT_PIN(PWM15)
		if (pwm_counter > rp2040_pwm[15])
		{
			mcu_clear_output(PWM15);
		}
#endif
	}
	else
	{
		pwm_counter = 0;
#if ASSERT_PIN(PWM0)
		if (rp2040_pwm[0])
		{
			mcu_set_output(PWM0);
		}
#endif
#if ASSERT_PIN(PWM1)
		if (rp2040_pwm[1])
		{
			mcu_set_output(PWM1);
		}
#endif
#if ASSERT_PIN(PWM2)
		if (rp2040_pwm[2])
		{
			mcu_set_output(PWM2);
		}
#endif
#if ASSERT_PIN(PWM3)
		if (rp2040_pwm[3])
		{
			mcu_set_output(PWM3);
		}
#endif
#if ASSERT_PIN(PWM4)
		if (rp2040_pwm[4])
		{
			mcu_set_output(PWM4);
		}
#endif
#if ASSERT_PIN(PWM5)
		if (rp2040_pwm[5])
		{
			mcu_set_output(PWM5);
		}
#endif
#if ASSERT_PIN(PWM6)
		if (rp2040_pwm[6])
		{
			mcu_set_output(PWM6);
		}
#endif
#if ASSERT_PIN(PWM7)
		if (rp2040_pwm[7])
		{
			mcu_set_output(PWM7);
		}
#endif
#if ASSERT_PIN(PWM8)
		if (rp2040_pwm[8])
		{
			mcu_set_output(PWM8);
		}
#endif
#if ASSERT_PIN(PWM9)
		if (rp2040_pwm[9])
		{
			mcu_set_output(PWM9);
		}
#endif
#if ASSERT_PIN(PWM10)
		if (rp2040_pwm[10])
		{
			mcu_set_output(PWM10);
		}
#endif
#if ASSERT_PIN(PWM11)
		if (rp2040_pwm[11])
		{
			mcu_set_output(PWM11);
		}
#endif
#if ASSERT_PIN(PWM12)
		if (rp2040_pwm[12])
		{
			mcu_set_output(PWM12);
		}
#endif
#if ASSERT_PIN(PWM13)
		if (rp2040_pwm[13])
		{
			mcu_set_output(PWM13);
		}
#endif
#if ASSERT_PIN(PWM14)
		if (rp2040_pwm[14])
		{
			mcu_set_output(PWM14);
		}
#endif
#if ASSERT_PIN(PWM15)
		if (rp2040_pwm[15])
		{
			mcu_set_output(PWM15);
		}
#endif
	}
}

static uint32_t mcu_step_counter;
static uint32_t mcu_step_reload;
static void mcu_gen_step(void)
{
	if (mcu_step_reload)
	{
		if (!--mcu_step_counter)
		{
			static bool resetstep = false;
			if (!resetstep)
				mcu_step_cb();
			else
				mcu_step_reset_cb();
			resetstep = !resetstep;
			mcu_step_counter = mcu_step_reload;
		}
	}
}

void mcu_din_isr(void)
{
	mcu_inputs_changed_cb();
}

void mcu_probe_isr(void)
{
	mcu_probe_changed_cb();
}

void mcu_limits_isr(void)
{
	mcu_limits_changed_cb();
}

void mcu_controls_isr(void)
{
	mcu_controls_changed_cb();
}

void mcu_rtc_isr(void *arg)
{
	mcu_runtime_ms++;
	mcu_rtc_cb(mcu_runtime_ms);
}

void mcu_itp_isr(void)
{
	// mcu_disable_global_isr();
	// static bool resetstep = false;
	// if (!resetstep)
	// 	mcu_step_cb();
	// else
	// 	mcu_step_reset_cb();
	// resetstep = !resetstep;
	// mcu_enable_global_isr();
	mcu_gen_step();
	mcu_gen_pwm();
	mcu_gen_oneshot();
}

// static void mcu_uart_isr(void *arg)
// {
// 	/*ATTENTION:*/
// 	/*IN NON-OS VERSION SDK, DO NOT USE "ICACHE_FLASH_ATTR" FUNCTIONS IN THE WHOLE HANDLER PROCESS*/
// 	/*ALL THE FUNCTIONS CALLED IN INTERRUPT HANDLER MUST BE DECLARED IN RAM */
// 	/*IF NOT , POST AN EVENT AND PROCESS IN SYSTEM TASK */
// 	if ((READ_PERI_REG(UART_INT_ST(0)) & UART_FRM_ERR_INT_ST))
// 	{
// 		WRITE_PERI_REG(UART_INT_CLR(0), UART_FRM_ERR_INT_CLR);
// 	}
// 	else if ((READ_PERI_REG(UART_INT_ST(0)) & (UART_RXFIFO_FULL_INT_ST | UART_RXFIFO_TOUT_INT_ST)))
// 	{
// 		// disable ISR
// 		CLEAR_PERI_REG_MASK(UART_INT_ENA(0), UART_RXFIFO_FULL_INT_ENA | UART_RXFIFO_TOUT_INT_ENA);
// 		WRITE_PERI_REG(UART_INT_CLR(0), (READ_PERI_REG(UART_INT_ST(0)) & (UART_RXFIFO_FULL_INT_ST | UART_RXFIFO_TOUT_INT_ST)));
// 		uint8_t fifo_len = (READ_PERI_REG(UART_STATUS(0)) >> UART_RXFIFO_CNT_S) & UART_RXFIFO_CNT;
// 		unsigned char c = 0;

// 		for (uint8_t i = 0; i < fifo_len; i++)
// 		{
// 			c = READ_PERI_REG(UART_FIFO(0)) & 0xFF;
// 			mcu_com_rx_cb(c);
// 		}

// 		WRITE_PERI_REG(UART_INT_CLR(0), UART_RXFIFO_FULL_INT_CLR | UART_RXFIFO_TOUT_INT_CLR);
// 		// reenable ISR
// 		SET_PERI_REG_MASK(UART_INT_ENA(0), UART_RXFIFO_FULL_INT_ENA | UART_RXFIFO_TOUT_INT_ENA);
// 	}
// 	else if (UART_TXFIFO_EMPTY_INT_ST == (READ_PERI_REG(UART_INT_ST(0)) & UART_TXFIFO_EMPTY_INT_ST))
// 	{
// 		/* to output uart data from uart buffer directly in empty interrupt handler*/
// 		/*instead of processing in system event, in order not to wait for current task/function to quit */
// 		/*ATTENTION:*/
// 		/*IN NON-OS VERSION SDK, DO NOT USE "ICACHE_FLASH_ATTR" FUNCTIONS IN THE WHOLE HANDLER PROCESS*/
// 		/*ALL THE FUNCTIONS CALLED IN INTERRUPT HANDLER MUST BE DECLARED IN RAM */
// 		CLEAR_PERI_REG_MASK(UART_INT_ENA(0), UART_TXFIFO_EMPTY_INT_ENA);
// 		mcu_com_tx_cb();
// 		// system_os_post(uart_recvTaskPrio, 1, 0);
// 		WRITE_PERI_REG(UART_INT_CLR(0), UART_TXFIFO_EMPTY_INT_CLR);
// 	}
// 	else if (UART_RXFIFO_OVF_INT_ST == (READ_PERI_REG(UART_INT_ST(0)) & UART_RXFIFO_OVF_INT_ST))
// 	{
// 		WRITE_PERI_REG(UART_INT_CLR(0), UART_RXFIFO_OVF_INT_CLR);
// 	}
// }

static void mcu_usart_init(void)
{
	rp2040_uart_init(BAUDRATE);
}

/**
 * 
 * Initializes the systick timer that is used as an RTC
 * and defines the systick handler for the ISR callback
 * 
 * **/
void mcu_rtc_init()
{
	SysTick->CTRL = 0;
	SysTick->LOAD = ((F_CPU / 1000) - 1);
	SysTick->VAL = 0;
	NVIC_SetPriority(SysTick_IRQn, 10);
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void)
{
	mcu_disable_global_isr();
	// counts to 20 and reloads
#if SERVOS_MASK > 0
	static uint8_t ms_servo_counter = 0;
	uint8_t servo_counter = ms_servo_counter;

	switch (servo_counter)
	{
#if ASSERT_PIN(SERVO0)
	case SERVO0_FRAME:
		servo_start_timeout(mcu_servos[0]);
		mcu_set_output(SERVO0);
		break;
#endif
#if ASSERT_PIN(SERVO1)
	case SERVO1_FRAME:
		mcu_set_output(SERVO1);
		servo_start_timeout(mcu_servos[1]);
		break;
#endif
#if ASSERT_PIN(SERVO2)
	case SERVO2_FRAME:
		mcu_set_output(SERVO2);
		servo_start_timeout(mcu_servos[2]);
		break;
#endif
#if ASSERT_PIN(SERVO3)
	case SERVO3_FRAME:
		mcu_set_output(SERVO3);
		servo_start_timeout(mcu_servos[3]);
		break;
#endif
#if ASSERT_PIN(SERVO4)
	case SERVO4_FRAME:
		mcu_set_output(SERVO4);
		servo_start_timeout(mcu_servos[4]);
		break;
#endif
#if ASSERT_PIN(SERVO5)
	case SERVO5_FRAME:
		mcu_set_output(SERVO5);
		servo_start_timeout(mcu_servos[5]);
		break;
#endif
	}

	servo_counter++;
	ms_servo_counter = (servo_counter != 20) ? servo_counter : 0;

#endif
	uint32_t millis = mcu_runtime_ms;
	millis++;
	mcu_runtime_ms = millis;
	mcu_rtc_cb(millis);
	mcu_enable_global_isr();
}

/**
 * initializes the mcu
 * this function needs to:
 *   - configure all IO pins (digital IO, PWM, Analog, etc...)
 *   - configure all interrupts
 *   - configure uart or usb
 *   - start the internal RTC
 * */
void mcu_init(void)
{
	mcu_io_init();
	mcu_usart_init();

	// init rtc
	mcu_rtc_init();

	// // init timer1
	// timer1_isr_init();
	// timer1_attachInterrupt(mcu_itp_isr);
	// timer1_enable(TIM_DIV1, TIM_EDGE, TIM_LOOP);
	// timer1_write(625);

#ifndef RAM_ONLY_SETTINGS
	rp2040_eeprom_init(1024); // 1K Emulated EEPROM
#endif
#ifdef MCU_HAS_SPI
	rp2040_spi_init(SPI_FREQ, SPI_MODE);
#endif

#ifdef MCU_HAS_I2C
	i2c_master_gpio_init();
	i2c_master_init();
#endif
}

/**
 * enables the pin probe mcu isr on change
 * can be defined either as a function or a macro call
 * */
#ifndef mcu_enable_probe_isr
void mcu_enable_probe_isr(void)
{
}
#endif

/**
 * disables the pin probe mcu isr on change
 * can be defined either as a function or a macro call
 * */
#ifndef mcu_disable_probe_isr
void mcu_disable_probe_isr(void)
{
}
#endif

/**
 * gets the voltage value of a built-in ADC pin
 * can be defined either as a function or a macro call
 * */
#ifndef mcu_get_analog
uint8_t mcu_get_analog(uint8_t channel)
{
	return 0;
}
#endif

/**
 * sets the pwm value of a built-in pwm pin
 * can be defined either as a function or a macro call
 * */
#ifndef mcu_set_pwm
void mcu_set_pwm(uint8_t pwm, uint8_t value)
{
}
#endif

/**
 * gets the configured pwm value of a built-in pwm pin
 * can be defined either as a function or a macro call
 * */
#ifndef mcu_get_pwm
uint8_t mcu_get_pwm(uint8_t pwm)
{
	return 0;
}
#endif

/**
 * checks if the serial hardware of the MCU is ready do send the next char
 * */
#ifndef mcu_tx_ready
bool mcu_tx_ready(void)
{
	return rp2040_uart_tx_ready();
}
#endif

/**
 * sends a char either via uart (hardware, software or USB virtual COM port)
 * can be defined either as a function or a macro call
 * */
#ifndef mcu_putc

void mcu_putc(char c)
{
#ifdef ENABLE_SYNC_TX
	while (!mcu_tx_ready())
		;
#endif

	rp2040_uart_write(c);
}
#endif

// ISR
/**
 * enables global interrupts on the MCU
 * can be defined either as a function or a macro call
 * */
#ifndef mcu_enable_global_isr
void mcu_enable_global_isr(void)
{
	// ets_intr_unlock();
	rp2040_global_isr_enabled = true;
}
#endif

/**
 * disables global interrupts on the MCU
 * can be defined either as a function or a macro call
 * */
#ifndef mcu_disable_global_isr
void mcu_disable_global_isr(void)
{
	rp2040_global_isr_enabled = false;
	// ets_intr_lock();
}
#endif

/**
 * gets global interrupts state on the MCU
 * can be defined either as a function or a macro call
 * */
#ifndef mcu_get_global_isr
bool mcu_get_global_isr(void)
{
	return rp2040_global_isr_enabled;
}
#endif

// Step interpolator
/**
 * convert step rate to clock cycles
 * */
void mcu_freq_to_clocks(float frequency, uint16_t *ticks, uint16_t *prescaller)
{
	// up and down counter (generates half the step rate at each event)
	uint32_t totalticks = (uint32_t)((float)(128000UL >> 1) / frequency);
	*prescaller = 0;
	while (totalticks > 0xFFFF)
	{
		(*prescaller) += 1;
		totalticks >>= 1;
	}

	*ticks = (uint16_t)totalticks;
}

float mcu_clocks_to_freq(uint16_t ticks, uint16_t prescaller)
{
	return ((float)(128000UL >> 1) / (float)(((uint32_t)ticks) << prescaller));
}

/**
 * starts the timer interrupt that generates the step pulses for the interpolator
 * */
void mcu_start_itp_isr(uint16_t ticks, uint16_t prescaller)
{
	mcu_step_reload = (((uint32_t)ticks) << prescaller);
	mcu_step_counter = mcu_step_reload;
}

/**
 * changes the step rate of the timer interrupt that generates the step pulses for the interpolator
 * */
void mcu_change_itp_isr(uint16_t ticks, uint16_t prescaller)
{
	mcu_step_reload = (((uint32_t)ticks) << prescaller);
	mcu_step_counter = mcu_step_reload;
}

/**
 * stops the timer interrupt that generates the step pulses for the interpolator
 * */
void mcu_stop_itp_isr(void)
{
	mcu_step_reload = 0;
}

/**
 * gets the MCU running time in milliseconds.
 * the time counting is controled by the internal RTC
 * */
uint32_t mcu_millis()
{
	return millis();
}

/**
 * provides a delay in us (micro seconds)
 * the maximum allowed delay is 255 us
 * */
uint32_t mcu_micros()
{
	return micros();
}

/**
 * runs all internal tasks of the MCU.
 * for the moment these are:
 *   - if USB is enabled and MCU uses tinyUSB framework run tinyUSB tud_task
 * */
void mcu_dotasks(void)
{
	rp2040_uart_process();
}

// Non volatile memory
/**
 * gets a byte at the given EEPROM (or other non volatile memory) address of the MCU.
 * */
uint8_t mcu_eeprom_getc(uint16_t address)
{
#ifndef RAM_ONLY_SETTINGS
	return rp2040_eeprom_read(address);
#else
	return 0;
#endif
}

/**
 * sets a byte at the given EEPROM (or other non volatile memory) address of the MCU.
 * */
void mcu_eeprom_putc(uint16_t address, uint8_t value)
{
#ifndef RAM_ONLY_SETTINGS
	rp2040_eeprom_write(address, value);
#endif
}

/**
 * flushes all recorded registers into the eeprom.
 * */
void mcu_eeprom_flush(void)
{
#ifndef RAM_ONLY_SETTINGS
	rp2040_eeprom_flush();
#endif
}

// #ifdef MCU_HAS_I2C
// #ifndef mcu_i2c_write
// uint8_t mcu_i2c_write(uint8_t data, bool send_start, bool send_stop)
// {
// 	if (send_start)
// 	{
// 		// init
// 		i2c_master_start();
// 		if (!i2c_master_checkAck())
// 		{
// 			i2c_master_stop();
// 			return 0;
// 		}
// 	}

// 	i2c_master_writeByte(data);

// 	if (!i2c_master_checkAck())
// 	{
// 		i2c_master_stop();
// 		return 0;
// 	}

// 	if (send_stop)
// 	{
// 		i2c_master_stop();
// 	}

// 	return 1;
// }
// #endif

// #ifndef mcu_i2c_read
// uint8_t mcu_i2c_read(bool with_ack, bool send_stop)
// {
// 	uint8_t c = 0;

// 	if (with_ack)
// 	{
// 		i2c_master_send_ack();
// 	}
// 	else
// 	{
// 		i2c_master_send_nack();
// 	}
// 	c = i2c_master_readByte();

// 	if (send_stop)
// 	{
// 		i2c_master_stop();
// 	}

// 	return c;
// }
// #endif
// #endif

#ifdef MCU_HAS_ONESHOT_TIMER
/**
 * configures a single shot timeout in us
 * */

#ifndef mcu_config_timeout
void mcu_config_timeout(mcu_timeout_delgate fp, uint32_t timeout)
{
	mcu_timeout_cb = fp;
	rp2040_oneshot_reload = (128000UL / timeout);
}
#endif

/**
 * starts the timeout. Once hit the the respective callback is called
 * */
#ifndef mcu_start_timeout
void mcu_start_timeout()
{
	rp2040_oneshot_counter = rp2040_oneshot_reload;
}
#endif
#endif

#endif
