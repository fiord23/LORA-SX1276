#ifndef __LORA_SX1276_H
#define __LORA_SX1276_H
#include <stdbool.h>

#define Lora_reset_low()   (GPIOA->BSRR |=  GPIO_BSRR_BR_1) /* PA1 = 0  RESET LORA*/ 
#define Lora_reset_high() (GPIOA->BSRR |=  GPIO_BSRR_BS_1) /* PA1 = 1 SET LORA*/

#define REG_FIFO                 0x00
#define REG_OP_MODE              0x01
#define REG_FRF_MSB              0x06
#define REG_FRF_MID              0x07
#define REG_FRF_LSB              0x08
#define REG_PA_CONFIG            0x09
#define REG_LR_PARAMP            0x0A
#define REG_OCP                  0x0b
#define REG_LNA                  0x0c
#define REG_FIFO_ADDR_PTR        0x0d
#define REG_FIFO_TX_BASE_ADDR    0x0e
#define REG_FIFO_RX_BASE_ADDR    0x0f
#define REG_FIFO_RX_CURRENT_ADDR 0x10
#define REG_LR_IRQFLAGSMASK      0x11
#define REG_IRQ_FLAGS            0x12
#define REG_RX_NB_BYTES          0x13
#define REG_PKT_SNR_VALUE        0x19
#define REG_PKT_RSSI_VALUE       0x1a
#define REG_MODEM_CONFIG_1       0x1d
#define REG_MODEM_CONFIG_2       0x1e
#define REG_LR_SYMBTIMEOUTLSB    0x1F
#define REG_PREAMBLE_MSB         0x20
#define REG_PREAMBLE_LSB         0x21
#define REG_PAYLOAD_LENGTH       0x22
#define REG_LR_PAYLOADMAXLENGTH  0x23
#define REG_LR_HOPPERIOD         0x24
#define REG_MODEM_CONFIG_3       0x26
#define REG_FREQ_ERROR_MSB       0x28
#define REG_FREQ_ERROR_MID       0x29
#define REG_FREQ_ERROR_LSB       0x2a
#define REG_RSSI_WIDEBAND        0x2c
#define REG_DETECTION_OPTIMIZE   0x31
#define REG_INVERTIQ             0x33
#define REG_DETECTION_THRESHOLD  0x37
#define REG_SYNC_WORD            0x39
#define REG_INVERTIQ2            0x3b
#define REG_DIO_MAPPING_1        0x40
#define REG_LR_DIOMAPPING2       0x41
#define REG_VERSION              0x42
#define REG_PA_DAC               0x4d

// modes
#define MODE_LONG_RANGE_MODE     0x80
#define MODE_SLEEP               0x00
#define MODE_STDBY               0x01
#define MODE_TX                  0x03
#define MODE_RX_CONTINUOUS       0x05
#define MODE_RX_SINGLE           0x06


// PA config
#define PA_BOOST                 0x80

#define IRQ_TX_DONE_MASK           0x08
#define IRQ_PAYLOAD_CRC_ERROR_MASK 0x20
#define IRQ_RX_DONE_MASK           0x40
#define IRQ_VALID_HEADER           0x10

#define FXOSC 32000000
#define F868  868000000

#define RFLR_DIOMAPPING1_DIO0_00                    0x00  // Default
#define RFLR_DIOMAPPING1_DIO0_01                    0x40

//void Lora_reset (void);
void Lora_init (void);
void Lora_transmit (uint8_t *strdata, uint8_t number_of_data);
void Lora_reset (void);
void Lora_recieve(uint8_t *str_r, uint8_t *num_of_bytes);
void Show_RSSI (void);
void Show_SNR (void);

#endif

// Set the device to Standby and wait for oscillator startup
// Set the device to FSRx mode
// Set TempMonitorOff = 0 (enables the sensor). It is not required to wait for the PLL Lock indication
// Wait for 140 microseconds
// Set TempMonitorOff = 1
// Set device back to Sleep of Standby mode
// Access temperature value in RegTemp
