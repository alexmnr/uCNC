/*
	Name: mcu_esp32.c
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

#if (MCU == MCU_ESP32)
#include "esp_timer.h"
#include "esp_task_wdt.h"
#include "esp_ipc.h"
#include "driver/uart.h"
#include "driver/timer.h"
#include "soc/i2s_struct.h"
#ifdef MCU_HAS_I2C
#include "driver/i2c.h"
#endif
#ifdef MCU_HAS_SPI
#include "hal/spi_types.h"
#include "driver/spi_master.h"
#endif
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

static volatile bool esp32_global_isr_enabled;
static volatile uint32_t mcu_runtime_ms;

void esp32_wifi_bt_init(void);
void esp32_wifi_bt_flush(char *buffer);
void esp32_wifi_bt_process(void);

#ifndef RAM_ONLY_SETTINGS
#include <nvs.h>
#include <esp_partition.h>
// Non volatile memory
#ifndef FLASH_EEPROM_SIZE
#define FLASH_EEPROM_SIZE 1024
#endif
typedef struct
{
	nvs_handle_t nvs_handle;
	size_t size;
	bool dirty;
	uint8_t data[FLASH_EEPROM_SIZE];
} flash_eeprom_t;

static flash_eeprom_t mcu_eeprom;
#endif

#ifdef IC74HC595_CUSTOM_SHIFT_IO
#if IC74HC595_COUNT != 4
#error "IC74HC595_COUNT must be 4 to use ESP32 I2S mode for IO shifting"
#endif
#include "driver/i2s.h"
#endif

hw_timer_t *esp32_step_timer;

#ifdef IC74HC595_CUSTOM_SHIFT_IO
extern uint8_t ic74hc595_io_pins[IC74HC595_COUNT];
static volatile uint8_t ic74hc595_update_lock;
void ic74hc595_shift_io_pins(void)
{
	if (!ic74hc595_update_lock++)
	{
		do
		{
			uint32_t data = *((uint32_t *)&ic74hc595_io_pins[0]);
			I2SREG.conf_single_data = data;
		} while (--ic74hc595_update_lock);
	}
}
#endif
// pwm channels
uint8_t esp32_pwm[16];
uint16_t esp32_pwm_mask;
// pwm resolution
uint8_t esp32_pwm_res;
IRAM_ATTR void mcu_pwm_isr(void *arg)
{
	static uint8_t pwm_counter = 0;
	uint8_t resolution = esp32_pwm_res;
#ifdef IC74HC595_HAS_PWMS
	static uint16_t pwm_mask_last = 0;
	uint16_t pwm_mask = esp32_pwm_mask;
#endif
	// software PWM
	if ((++pwm_counter) >> resolution)
	{
		uint8_t pwm_ref = pwm_counter << resolution;
#if !(PWM0 < 0)
		if (pwm_ref > esp32_pwm[0])
		{
			mcu_clear_output(PWM0);
		}
#endif
#if !(PWM1 < 0)
		if (pwm_ref > esp32_pwm[1])
		{
			mcu_clear_output(PWM1);
		}
#endif
#if !(PWM2 < 0)
		if (pwm_ref > esp32_pwm[2])
		{
			mcu_clear_output(PWM2);
		}
#endif
#if !(PWM3 < 0)
		if (pwm_ref > esp32_pwm[3])
		{
			mcu_clear_output(PWM3);
		}
#endif
#if !(PWM4 < 0)
		if (pwm_ref > esp32_pwm[4])
		{
			mcu_clear_output(PWM4);
		}
#endif
#if !(PWM5 < 0)
		if (pwm_ref > esp32_pwm[5])
		{
			mcu_clear_output(PWM5);
		}
#endif
#if !(PWM6 < 0)
		if (pwm_ref > esp32_pwm[6])
		{
			mcu_clear_output(PWM6);
		}
#endif
#if !(PWM7 < 0)
		if (pwm_ref > esp32_pwm[7])
		{
			mcu_clear_output(PWM7);
		}
#endif
#if !(PWM8 < 0)
		if (pwm_ref > esp32_pwm[8])
		{
			mcu_clear_output(PWM8);
		}
#endif
#if !(PWM9 < 0)
		if (pwm_ref > esp32_pwm[9])
		{
			mcu_clear_output(PWM9);
		}
#endif
#if !(PWM10 < 0)
		if (pwm_ref > esp32_pwm[10])
		{
			mcu_clear_output(PWM10);
		}
#endif
#if !(PWM11 < 0)
		if (pwm_ref > esp32_pwm[11])
		{
			mcu_clear_output(PWM11);
		}
#endif
#if !(PWM12 < 0)
		if (pwm_ref > esp32_pwm[12])
		{
			mcu_clear_output(PWM12);
		}
#endif
#if !(PWM13 < 0)
		if (pwm_ref > esp32_pwm[13])
		{
			mcu_clear_output(PWM13);
		}
#endif
#if !(PWM14 < 0)
		if (pwm_ref > esp32_pwm[14])
		{
			mcu_clear_output(PWM14);
		}
#endif
#if !(PWM15 < 0)
		if (pwm_ref > esp32_pwm[15])
		{
			mcu_clear_output(PWM15);
		}
#endif

#ifdef IC74HC595_HAS_PWMS
#if !(PWM0_IO_OFFSET < 0)
		if (pwm_ref > esp32_pwm[0])
		{
			pwm_mask &= ~(1 << 0);
		}
#endif
#if !(PWM1_IO_OFFSET < 0)
		if (pwm_ref > esp32_pwm[1])
		{
			pwm_mask &= ~(1 << 1);
		}
#endif
#if !(PWM2_IO_OFFSET < 0)
		if (pwm_ref > esp32_pwm[2])
		{
			pwm_mask &= ~(1 << 2);
		}
#endif
#if !(PWM3_IO_OFFSET < 0)
		if (pwm_ref > esp32_pwm[3])
		{
			pwm_mask &= ~(1 << 3);
		}
#endif
#if !(PWM4_IO_OFFSET < 0)
		if (pwm_ref > esp32_pwm[4])
		{
			pwm_mask &= ~(1 << 4);
		}
#endif
#if !(PWM5_IO_OFFSET < 0)
		if (pwm_ref > esp32_pwm[5])
		{
			pwm_mask &= ~(1 << 5);
		}
#endif
#if !(PWM6_IO_OFFSET < 0)
		if (pwm_ref > esp32_pwm[6])
		{
			pwm_mask &= ~(1 << 6);
		}
#endif
#if !(PWM7_IO_OFFSET < 0)
		if (pwm_ref > esp32_pwm[7])
		{
			pwm_mask &= ~(1 << 7);
		}
#endif
#if !(PWM8_IO_OFFSET < 0)
		if (pwm_ref > esp32_pwm[8])
		{
			pwm_mask &= ~(1 << 8);
		}
#endif
#if !(PWM9_IO_OFFSET < 0)
		if (pwm_ref > esp32_pwm[9])
		{
			pwm_mask &= ~(1 << 9);
		}
#endif
#if !(PWM10_IO_OFFSET < 0)
		if (pwm_ref > esp32_pwm[10])
		{
			pwm_mask &= ~(1 << 10);
		}
#endif
#if !(PWM11_IO_OFFSET < 0)
		if (pwm_ref > esp32_pwm[11])
		{
			pwm_mask &= ~(1 << 11);
		}
#endif
#if !(PWM12_IO_OFFSET < 0)
		if (pwm_ref > esp32_pwm[12])
		{
			pwm_mask &= ~(1 << 12);
		}
#endif
#if !(PWM13_IO_OFFSET < 0)
		if (pwm_ref > esp32_pwm[13])
		{
			pwm_mask &= ~(1 << 13);
		}
#endif
#if !(PWM14_IO_OFFSET < 0)
		if (pwm_ref > esp32_pwm[14])
		{
			pwm_mask &= ~(1 << 14);
		}
#endif
#if !(PWM15_IO_OFFSET < 0)
		if (pwm_ref > esp32_pwm[15])
		{
			pwm_mask &= ~(1 << 15);
		}
#endif
#endif
	}
	else
	{
		pwm_counter = 0;
#if !(PWM0 < 0)
		if (esp32_pwm[0])
		{
			mcu_set_output(PWM0);
		}
#endif
#if !(PWM1 < 0)
		if (esp32_pwm[1])
		{
			mcu_set_output(PWM1);
		}
#endif
#if !(PWM2 < 0)
		if (esp32_pwm[2])
		{
			mcu_set_output(PWM2);
		}
#endif
#if !(PWM3 < 0)
		if (esp32_pwm[3])
		{
			mcu_set_output(PWM3);
		}
#endif
#if !(PWM4 < 0)
		if (esp32_pwm[4])
		{
			mcu_set_output(PWM4);
		}
#endif
#if !(PWM5 < 0)
		if (esp32_pwm[5])
		{
			mcu_set_output(PWM5);
		}
#endif
#if !(PWM6 < 0)
		if (esp32_pwm[6])
		{
			mcu_set_output(PWM6);
		}
#endif
#if !(PWM7 < 0)
		if (esp32_pwm[7])
		{
			mcu_set_output(PWM7);
		}
#endif
#if !(PWM8 < 0)
		if (esp32_pwm[8])
		{
			mcu_set_output(PWM8);
		}
#endif
#if !(PWM9 < 0)
		if (esp32_pwm[9])
		{
			mcu_set_output(PWM9);
		}
#endif
#if !(PWM10 < 0)
		if (esp32_pwm[10])
		{
			mcu_set_output(PWM10);
		}
#endif
#if !(PWM11 < 0)
		if (esp32_pwm[11])
		{
			mcu_set_output(PWM11);
		}
#endif
#if !(PWM12 < 0)
		if (esp32_pwm[12])
		{
			mcu_set_output(PWM12);
		}
#endif
#if !(PWM13 < 0)
		if (esp32_pwm[13])
		{
			mcu_set_output(PWM13);
		}
#endif
#if !(PWM14 < 0)
		if (esp32_pwm[14])
		{
			mcu_set_output(PWM14);
		}
#endif
#if !(PWM15 < 0)
		if (esp32_pwm[15])
		{
			mcu_set_output(PWM15);
		}
#endif
	}

#ifdef IC74HC595_HAS_PWMS
	if (pwm_mask_last != pwm_mask)
	{
		ic74hc595_set_pwms(pwm_mask);
		pwm_mask_last = pwm_mask;
	}
#endif

	timer_group_clr_intr_status_in_isr(PWM_TIMER_TG, PWM_TIMER_IDX);
	/* After the alarm has been triggered
	  we need enable it again, so it is triggered the next time */
	timer_group_enable_alarm_in_isr(PWM_TIMER_TG, PWM_TIMER_IDX);
}

IRAM_ATTR void mcu_gpio_isr(void *type)
{
	// read the address and not the pointer value because we are passing a literal integer
	// reading the pointer value would try to read an invalid memory address
	switch ((int)type)
	{
	case 0:
		mcu_controls_changed_cb();
		break;
	case 1:
		mcu_limits_changed_cb();
		break;
	case 2:
		mcu_probe_changed_cb();
		break;
	case 3:
		mcu_inputs_changed_cb();
		break;
	default:
		break;
	}
}

void mcu_pwm_freq_config(uint16_t freq)
{
	// keeps 8 bit resolution up to 1KHz
	// reduces bit resolution for higher frequncies

	// determines the bit resolution (8 - esp32_pwm_res);
	uint8_t res = (uint8_t)MAX((int8_t)ceilf(log2(freq * 0.001f)), 0);
	freq >>= res;
	esp32_pwm_res = res;
	// timer base frequency is APB clock/2
	// it's then divided by 256
	timer_set_alarm_value(PWM_TIMER_TG, PWM_TIMER_IDX, (uint64_t)roundf((float)(getApbFrequency() >> 9) / (float)freq));
}

void mcu_core0_tasks_init(void *arg)
{
	// register PWM isr
	timer_isr_register(PWM_TIMER_TG, PWM_TIMER_IDX, mcu_pwm_isr, NULL, 0, NULL);
	// install UART driver handler
	uart_driver_install(COM_PORT, RX_BUFFER_CAPACITY * 2, 0, 0, NULL, 0);
}

void mcu_rtc_task(void *arg)
{
	portTickType xLastWakeTimeUpload = xTaskGetTickCount();
	for (;;)
	{
		mcu_runtime_ms++;
		mcu_rtc_cb(mcu_runtime_ms);
		vTaskDelayUntil(&xLastWakeTimeUpload, (1 / portTICK_RATE_MS));
	}
}

IRAM_ATTR void mcu_itp_isr(void *arg)
{
	static bool resetstep = false;

	if (!resetstep)
		mcu_step_cb();
	else
		mcu_step_reset_cb();
	resetstep = !resetstep;

	timer_group_clr_intr_status_in_isr(ITP_TIMER_TG, ITP_TIMER_IDX);
	/* After the alarm has been triggered
	  we need enable it again, so it is triggered the next time */
	timer_group_enable_alarm_in_isr(ITP_TIMER_TG, ITP_TIMER_IDX);
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
#if (defined(LIMIT_X_ISR) || defined(LIMIT_Y_ISR) || defined(LIMIT_Z_ISR) || defined(LIMIT_X2_ISR) || defined(LIMIT_Y2_ISR) || defined(LIMIT_Z2_ISR) || defined(LIMIT_A_ISR) || defined(LIMIT_B_ISR) || defined(LIMIT_C_ISR) || defined(PROBE_ISR) || defined(ESTOP_ISR) || defined(SAFETY_DOOR_ISR) || defined(FHOLD_ISR) || defined(CS_RES_ISR) || defined(DIN0_ISR) || defined(DIN1_ISR) || defined(DIN2_ISR) || defined(DIN3_ISR) || defined(DIN4_ISR) || defined(DIN5_ISR) || defined(DIN6_ISR) || defined(DIN7_ISR))
	gpio_install_isr_service(0);
#endif

	mcu_io_init();

	// starts EEPROM before UART to enable WiFi and BT settings
#ifndef RAM_ONLY_SETTINGS
	// esp32_eeprom_init(FLASH_EEPROM_SIZE); // 1K Emulated EEPROM

	// starts nvs
	mcu_eeprom.size = 0;
	memset(mcu_eeprom.data, 0, FLASH_EEPROM_SIZE);
	if (nvs_open("eeprom", NVS_READWRITE, &mcu_eeprom.nvs_handle) == ESP_OK)
	{
		// determines the maximum sector size of NVS that can be read/write
		nvs_get_blob(mcu_eeprom.nvs_handle, "eeprom", NULL, &mcu_eeprom.size);
		if (FLASH_EEPROM_SIZE > mcu_eeprom.size)
		{
			log_e("eeprom does not have enough space");
			mcu_eeprom.size = 0;
		}

		nvs_get_blob(mcu_eeprom.nvs_handle, "eeprom", mcu_eeprom.data, &mcu_eeprom.size);
	}
	else
	{
		log_e("eeprom failed to open");
	}
#endif

	// initialize UART
	const uart_config_t uartconfig = {
		.baud_rate = BAUDRATE,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
		.source_clk = UART_SCLK_APB};
	// We won't use a buffer for sending data.
	uart_param_config(COM_PORT, &uartconfig);
	uart_set_pin(COM_PORT, TX_BIT, RX_BIT, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

	// initialize PWM timer
	/* Select and initialize basic parameters of the timer */
	timer_config_t pwmconfig = {
		.divider = 2,
		.counter_dir = TIMER_COUNT_UP,
		.counter_en = TIMER_PAUSE,
		.alarm_en = TIMER_ALARM_EN,
		.auto_reload = true,
	}; // default clock source is APB
	timer_init(PWM_TIMER_TG, PWM_TIMER_IDX, &pwmconfig);

	/* Timer's counter will initially start from value below.
	   Also, if auto_reload is set, this value will be automatically reload on alarm */
	timer_set_counter_value(PWM_TIMER_TG, PWM_TIMER_IDX, 0x00000000ULL);
	/* Configure the alarm value and the interrupt on alarm. */
	timer_set_alarm_value(PWM_TIMER_TG, PWM_TIMER_IDX, (uint64_t)157);
	timer_enable_intr(PWM_TIMER_TG, PWM_TIMER_IDX);

	// inititialize ITP timer
	timer_config_t itpconfig = {
		.divider = 80,
		.counter_dir = TIMER_COUNT_UP,
		.counter_en = TIMER_PAUSE,
		.alarm_en = TIMER_ALARM_EN,
		.auto_reload = true,
	}; // default clock source is APB
	timer_init(ITP_TIMER_TG, ITP_TIMER_IDX, &itpconfig);

	// initialize rtc timer (currently on core 1)
	xTaskCreatePinnedToCore(mcu_rtc_task, "rtcTask", 1024, NULL, 7, NULL, CONFIG_ARDUINO_RUNNING_CORE);

	// launches isr tasks that will run on core 0
	// currently it's running PWM and UART on core 0
	esp_ipc_call_blocking(0, mcu_core0_tasks_init, NULL);
	// mcu_core0_tasks_init(NULL);

#ifdef MCU_HAS_SPI
	spi_bus_config_t spiconf = {
		.miso_io_num = SPI_SDI_BIT,
		.mosi_io_num = SPI_SDO_BIT,
		.sclk_io_num = SPI_CLK_BIT,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1,
		.data4_io_num = -1,
		.data5_io_num = -1,
		.data6_io_num = -1,
		.data7_io_num = -1,
		.max_transfer_sz = SOC_SPI_MAXIMUM_BUFFER_SIZE,
		.flags = 0,
		.intr_flags = 0};
	// Initialize the SPI bus
	spi_bus_initialize(SPI_PORT, &spiconf, SPI_DMA_DISABLED);
	mcu_spi_config(SPI_MODE, SPI_FREQ);
#endif

#ifdef MCU_HAS_I2C
	i2c_config_t i2cconf = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = I2C_SDA_BIT, // select GPIO specific to your project
		.sda_pullup_en = GPIO_PULLUP_ENABLE,
		.scl_io_num = I2C_SCL_BIT, // select GPIO specific to your project
		.scl_pullup_en = GPIO_PULLUP_ENABLE,
		.master.clk_speed = I2C_FREQ, // select frequency specific to your project
		.clk_flags = 0,				  // you can use I2C_SCLK_SRC_FLAG_* flags to choose i2c source clock here
	};
	i2c_param_config((i2c_port_t)I2C_PORT, &i2cconf);
	i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0);
#endif

#ifdef IC74HC595_CUSTOM_SHIFT_IO
	i2s_config_t i2s_config = {
		.mode = I2S_MODE_MASTER | I2S_MODE_TX, // Only TX
		.sample_rate = 156250UL,			   // 312500KHz * 32bit * 2 channels = 20MHz
		.bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
		.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT, // 1-channels
		.communication_format = I2S_COMM_FORMAT_STAND_I2S | I2S_COMM_FORMAT_STAND_MSB,
		.dma_buf_count = 2,
		.dma_buf_len = 8,
		.use_apll = false,
		.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1 // Interrupt level 1
	};

	i2s_pin_config_t pin_config = {
		.bck_io_num = IC74HC595_I2S_CLK,
		.ws_io_num = IC74HC595_I2S_WS,
		.data_out_num = IC74HC595_I2S_DATA,
		.data_in_num = -1 // Not used
	};

	i2s_driver_install(IC74HC595_I2S_PORT, &i2s_config, 0, NULL);
	i2s_set_pin(IC74HC595_I2S_PORT, &pin_config);

	I2SREG.clkm_conf.clka_en = 0;	   // Use PLL/2 as reference
	I2SREG.clkm_conf.clkm_div_num = 4; // reset value of 4
	I2SREG.clkm_conf.clkm_div_a = 1;   // 0 at reset, what about divide by 0?
	I2SREG.clkm_conf.clkm_div_b = 0;   // 0 at reset

	//
	I2SREG.fifo_conf.tx_fifo_mod = 3; // 32 bits single channel data
	I2SREG.conf_chan.tx_chan_mod = 3; //
	I2SREG.sample_rate_conf.tx_bits_mod = 32;

	I2SREG.conf_single_data = 0;

	// Use normal clock format, (WS is aligned with the last bit)
	I2SREG.conf.tx_msb_shift = 0;
	I2SREG.conf.rx_msb_shift = 0;

	// Disable TX interrupts
	I2SREG.int_ena.out_eof = 0;
	I2SREG.int_ena.out_dscr_err = 0;

#endif

	timer_start(PWM_TIMER_TG, PWM_TIMER_IDX);

	esp32_wifi_bt_init();
	mcu_enable_global_isr();
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

/*UART*/
static char mcu_tx_buffer[TX_BUFFER_SIZE + 2];
static uint8_t mcu_tx_buffer_counter;

/**
 * checks if the serial hardware of the MCU is ready do send the next char
 * */
#ifndef mcu_tx_ready
bool mcu_tx_ready(void)
{
	return (mcu_tx_buffer_counter < TX_BUFFER_SIZE);
}
#endif

/**
 * sends a char either via uart (hardware, software or USB virtual COM port)
 * can be defined either as a function or a macro call
 * */
#ifndef mcu_putc

void mcu_putc(char c)
{
	mcu_tx_buffer[mcu_tx_buffer_counter++] = c;

	// autoflush if full
	if ((mcu_tx_buffer_counter >= TX_BUFFER_SIZE) || (c == '\n'))
	{
		mcu_tx_buffer[mcu_tx_buffer_counter] = 0;
		uart_write_bytes(COM_PORT, mcu_tx_buffer, mcu_tx_buffer_counter);
		esp32_wifi_bt_flush(mcu_tx_buffer);
		mcu_tx_buffer_counter = 0;
	}
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
	esp32_global_isr_enabled = true;
}
#endif

/**
 * disables global interrupts on the MCU
 * can be defined either as a function or a macro call
 * */
#ifndef mcu_disable_global_isr
void mcu_disable_global_isr(void)
{
	esp32_global_isr_enabled = false;
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
	return esp32_global_isr_enabled;
}
#endif

// Step interpolator
/**
 * convert step rate to clock cycles
 * */
void mcu_freq_to_clocks(float frequency, uint16_t *ticks, uint16_t *prescaller)
{
	// up and down counter (generates half the step rate at each event)
	uint32_t totalticks = (uint32_t)(500000.0f / frequency);
	*prescaller = 1;
	while (totalticks > 0xFFFF)
	{
		(*prescaller) <<= 1;
		totalticks >>= 1;
	}

	*ticks = (uint16_t)totalticks;
}

/**
 * starts the timer interrupt that generates the step pulses for the interpolator
 * */
static volatile bool mcu_itp_timer_running;
void mcu_start_itp_isr(uint16_t ticks, uint16_t prescaller)
{
	if (!mcu_itp_timer_running)
	{
		/* Timer's counter will initially start from value below.
		   Also, if auto_reload is set, this value will be automatically reload on alarm */
		timer_set_counter_value(ITP_TIMER_TG, ITP_TIMER_IDX, 0x00000000ULL);

		/* Configure the alarm value and the interrupt on alarm. */
		timer_set_alarm_value(ITP_TIMER_TG, ITP_TIMER_IDX, (uint64_t)ticks * prescaller);
		timer_enable_intr(ITP_TIMER_TG, ITP_TIMER_IDX);
		timer_isr_register(ITP_TIMER_TG, ITP_TIMER_IDX, mcu_itp_isr, NULL, 0, NULL);

		timer_start(ITP_TIMER_TG, ITP_TIMER_IDX);
		mcu_itp_timer_running = true;
	}
	else
	{
		mcu_change_itp_isr(ticks, prescaller);
	}
}

/**
 * changes the step rate of the timer interrupt that generates the step pulses for the interpolator
 * */
void mcu_change_itp_isr(uint16_t ticks, uint16_t prescaller)
{
	if (mcu_itp_timer_running)
	{
		timer_pause(ITP_TIMER_TG, ITP_TIMER_IDX);
		timer_set_alarm_value(ITP_TIMER_TG, ITP_TIMER_IDX, (uint64_t)ticks * prescaller);
		timer_start(ITP_TIMER_TG, ITP_TIMER_IDX);
	}
	else
	{
		mcu_start_itp_isr(ticks, prescaller);
	}
}

/**
 * stops the timer interrupt that generates the step pulses for the interpolator
 * */
void mcu_stop_itp_isr(void)
{
	if (mcu_itp_timer_running)
	{
		// timerAlarmDisable(esp32_step_timer);
		timer_pause(ITP_TIMER_TG, ITP_TIMER_IDX);
		timer_disable_intr(ITP_TIMER_TG, ITP_TIMER_IDX);
		mcu_itp_timer_running = false;
	}
}

/**
 * gets the MCU running time in milliseconds.
 * the time counting is controled by the internal RTC
 * */
uint32_t mcu_millis()
{
	return mcu_runtime_ms;
}

extern int64_t esp_system_get_time(void);
void esp32_delay_us(uint16_t delay)
{
	int64_t time = esp_system_get_time() + delay - 1;
	while (time > esp_system_get_time())
		;
}

/**
 * runs all internal tasks of the MCU.
 * for the moment these are:
 *   - if USB is enabled and MCU uses tinyUSB framework run tinyUSB tud_task
 * */
void mcu_dotasks(void)
{
	// reset WDT
	esp_task_wdt_reset();

	// loop through received data
	char rxdata[RX_BUFFER_SIZE];
	int rxlen = uart_read_bytes(COM_PORT, rxdata, RX_BUFFER_CAPACITY, 0);
	for (int i = 0; i < rxlen; i++)
	{
		mcu_com_rx_cb((unsigned char)rxdata[i]);
	}

	esp32_wifi_bt_process();
}

// Non volatile memory
/**
 * gets a byte at the given EEPROM (or other non volatile memory) address of the MCU.
 * */
uint8_t mcu_eeprom_getc(uint16_t address)
{
#ifndef RAM_ONLY_SETTINGS
	// return esp32_eeprom_read(address);
	size_t size = mcu_eeprom.size;
	if (size)
	{
		return mcu_eeprom.data[address];
	}
#endif
	return 0;
}

/**
 * sets a byte at the given EEPROM (or other non volatile memory) address of the MCU.
 * */
void mcu_eeprom_putc(uint16_t address, uint8_t value)
{
#ifndef RAM_ONLY_SETTINGS
	// esp32_eeprom_write(address, value);
	size_t size = mcu_eeprom.size;
	if (size)
	{
		mcu_eeprom.dirty |= (mcu_eeprom.data[address] != value);
		mcu_eeprom.data[address] = value;
	}
#endif
}

/**
 * flushes all recorded registers into the eeprom.
 * */
void mcu_eeprom_flush(void)
{
#ifndef RAM_ONLY_SETTINGS
	// esp32_eeprom_flush();
	// esp32_eeprom_write(address, value);
	if (mcu_eeprom.size && mcu_eeprom.dirty)
	{
		nvs_set_blob(mcu_eeprom.nvs_handle, "eeprom", mcu_eeprom.data, mcu_eeprom.size);
		nvs_commit(mcu_eeprom.nvs_handle);
	}
#endif
}

#ifdef MCU_HAS_I2C
#ifndef mcu_i2c_write
i2c_cmd_handle_t esp32_i2c_cmd;
uint8_t esp32_i2c_cmd_buff[I2C_LINK_RECOMMENDED_SIZE(1)];
uint8_t mcu_i2c_write(uint8_t data, bool send_start, bool send_stop)
{
	esp_err_t ret = ESP_FAIL;

	if (send_start)
	{
		// init
		if (esp32_i2c_cmd != NULL)
		{
			i2c_cmd_link_delete_static(esp32_i2c_cmd);
		}
		memset(esp32_i2c_cmd_buff, 0, I2C_LINK_RECOMMENDED_SIZE(1));
		esp32_i2c_cmd = i2c_cmd_link_create_static(esp32_i2c_cmd_buff, I2C_LINK_RECOMMENDED_SIZE(1));
		ret = i2c_master_start(esp32_i2c_cmd);
		if (ret != ESP_OK)
		{
			return 0;
		}
	}

	ret = i2c_master_write_byte(esp32_i2c_cmd, data, true);
	if (ret != ESP_OK)
	{
		return 0;
	}

	if (send_stop)
	{
		ret = i2c_master_stop(esp32_i2c_cmd);
		if (ret != ESP_OK)
		{
			return 0;
		}

		ret = i2c_master_cmd_begin(I2C_PORT, esp32_i2c_cmd, 100);
		if (ret != ESP_OK)
		{
			return 0;
		}
	}

	return 1;
}
#endif

#ifndef mcu_i2c_read
uint8_t mcu_i2c_read(bool with_ack, bool send_stop)
{
	uint8_t c = 0;
	esp_err_t ret = ESP_FAIL;

	ret = i2c_master_read_byte(esp32_i2c_cmd, &c, (!with_ack) ? I2C_MASTER_ACK : I2C_MASTER_NACK);
	if (ret != ESP_OK)
	{
		return 0;
	}

	if (send_stop)
	{
		ret = i2c_master_stop(esp32_i2c_cmd);
		if (ret != ESP_OK)
		{
			return 0;
		}

		ret = i2c_master_cmd_begin(I2C_PORT, esp32_i2c_cmd, 100);
		if (ret != ESP_OK)
		{
			return 0;
		}
	}

	return c;
}
#endif
#endif

#ifdef MCU_HAS_ONESHOT_TIMER

IRAM_ATTR void mcu_oneshot_isr(void *arg)
{
	timer_pause(ONESHOT_TIMER_TG, ONESHOT_TIMER_IDX);
	timer_group_clr_intr_status_in_isr(ONESHOT_TIMER_TG, ONESHOT_TIMER_IDX);

	if (mcu_timeout_cb)
	{
		mcu_timeout_cb();
	}
}

/**
 * configures a single shot timeout in us
 * */
#ifndef mcu_config_timeout
void mcu_config_timeout(mcu_timeout_delgate fp, uint32_t timeout)
{
	mcu_timeout_cb = fp;
	timer_config_t config = {
		.divider = 80,
		.counter_dir = TIMER_COUNT_UP,
		.counter_en = TIMER_PAUSE,
		.alarm_en = TIMER_ALARM_EN,
		.auto_reload = true,
	}; // default clock source is APB
	timer_init(ONESHOT_TIMER_TG, ONESHOT_TIMER_IDX, &config);

	/* Timer's counter will initially start from value below.
	   Also, if auto_reload is set, this value will be automatically reload on alarm */
	timer_set_counter_value(ONESHOT_TIMER_TG, ONESHOT_TIMER_IDX, 0x00000000ULL);

	/* Configure the alarm value and the interrupt on alarm. */
	timer_set_alarm_value(ONESHOT_TIMER_TG, ONESHOT_TIMER_IDX, (uint64_t)(1000000UL / timeout));
	timer_enable_intr(ONESHOT_TIMER_TG, ONESHOT_TIMER_IDX);
	timer_isr_register(ONESHOT_TIMER_TG, ONESHOT_TIMER_IDX, mcu_oneshot_isr, NULL, 0, NULL);
}
#endif

/**
 * starts the timeout. Once hit the the respective callback is called
 * */
#ifndef mcu_start_timeout
void mcu_start_timeout()
{
	timer_start(ONESHOT_TIMER_TG, ONESHOT_TIMER_IDX);
}
#endif
#endif

#ifdef MCU_HAS_SPI

static spi_device_handle_t mcu_spi_handle;

#ifndef mcu_spi_xmit
uint8_t mcu_spi_xmit(uint8_t data)
{
	uint8_t rxdata = 0xFF;
	spi_transaction_t spi_trans = {0};
	spi_trans.length = 8; // Number of bits NOT number of bytes.
	spi_trans.tx_buffer = &data;
	spi_trans.rx_buffer = &rxdata;

	spi_device_transmit(mcu_spi_handle, &spi_trans);

	return rxdata;
}
#endif

#ifndef mcu_spi_config
void mcu_spi_config(uint8_t mode, uint32_t frequency)
{
	spi_bus_remove_device(mcu_spi_handle);
	spi_device_interface_config_t mcu_spi_conf = {0};
	mcu_spi_conf.clock_speed_hz = frequency;
	mcu_spi_conf.spics_io_num = -1;
	mcu_spi_conf.queue_size = 1;

	spi_bus_add_device(SPI_PORT, &mcu_spi_conf, &mcu_spi_handle);
}
#endif
#endif

#endif
