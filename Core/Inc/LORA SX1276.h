#ifndef __LORA_SX1276_H
#define __LORA_SX1276_H
#include <stdbool.h>

#define Lora_reset_low()   (GPIOA->BSRR |=  GPIO_BSRR_BR_1) /* PA1 = 0  RESET LORA*/ 
#define Lora_reset_high() (GPIOA->BSRR |=  GPIO_BSRR_BS_1) /* PA1 = 1 SET LORA*/

#define REG_FIFO                 0x00
#define REG_OP_MODE              0x01
    // modes
    #define MODE_LONG_RANGE_MODE     0x80
    #define MODE_SLEEP               0x00
    #define MODE_STDBY               0x01
    #define MODE_TX                  0x03
    #define MODE_RX_CONTINUOUS       0x05
    #define MODE_RX_SINGLE           0x06
#define REG_FRF_MSB              0x06
    //FREQUENCY 868
    #define FREQUENCY_868_MSB    0xD9
#define REG_FRF_MID              0x07
    //FREQUENCY 868
    #define FREQUENCY_868_MID    0x00
#define REG_FRF_LSB              0x08
    //FREQUENCY 868
    #define FREQUENCY_868_LSB    0x00

#define REG_PA_CONFIG            0x09
    //PA SELECT
    #define RFO_TO_14dBm         0x00
    #define PA_BOOST_TO_20dBm    0x80
    //Max output Power
    #define POWER_15dBm          0x70
    //Output Power
    #define POUT_PA_17dBm        0x0F
    #define POUT_RFO_15dBm       0x0F
#define REG_LR_PARAMP            0x0A
#define REG_OCP                  0x0b
    //OCP ON/OFF
    #define OCP_DISABLED         0x00
    #define OCP_ENABLED          0x20
    //OCP TRIM
    #define OCP_100mA            0x0b
    #define OCP_240mA            0x1b

#define REG_LNA                  0x0c
    //LNA Gain
    #define LNA_GAIN_NOT_USED        0x00
    #define LNA_GAIN_G1              0x20
    #define LNA_GAIN_G2              0x40
    #define LNA_GAIN_G3              0x60
    #define LNA_GAIN_G4              0x80
    #define LNA_GAIN_G5              0xA0
    #define LNA_GAIN_G6              0xC0
    //LNA Boost LF
    #define LNA_BOOST_LF             0x00
    //LNA Boost HF
    #define DEFAULT_LNA_CURRENT           0x00
    #define BOOST_ON_150_LNA_CURRENT      0x03

#define REG_FIFO_ADDR_PTR        0x0d
#define REG_FIFO_TX_BASE_ADDR    0x0e
#define REG_FIFO_RX_BASE_ADDR    0x0f
#define REG_FIFO_RX_CURRENT_ADDR 0x10
#define REG_LR_IRQFLAGSMASK      0x11
#define REG_IRQ_FLAGS            0x12
    //IRQs
    #define IRQ_VALID_HEADER           0x10
    #define IRQ_PAYLOAD_CRC_ERROR_MASK 0x20
    #define IRQ_RX_DONE_MASK           0x40
    #define IRQ_TX_DONE_MASK           0x08

#define REG_RX_NB_BYTES          0x13
#define REG_PKT_SNR_VALUE        0x19
#define REG_PKT_RSSI_VALUE       0x1A

#define REG_MODEM_CONFIG_1       0x1D   
    //Bandwidth
    #define BANDWIDTH_7_8_kHz          0x00
    #define BANDWIDTH_10_4_kHz         0x10
    #define BANDWIDTH_15_6_kHz         0x20
    #define BANDWIDTH_20_8_kHz         0x30
    #define BANDWIDTH_31_25_kHz        0x40
    #define BANDWIDTH_41_7_kHz         0x50
    #define BANDWIDTH_62_5_kHz         0x60
    #define BANDWIDTH_125_kHz          0x70
    #define BANDWIDTH_250_kHz          0x80
    #define BANDWIDTH_500_kHz          0x90
    //CodingRate
    #define CODING_RATE_4_5            0x02
    #define CODING_RATE_4_6            0x04
    #define CODING_RATE_4_7            0x06
    #define CODING_RATE_4_8            0x08
    //Header Mode
    #define EXPLICIT_HEADER_MODE       0x00
    #define IMPLICIT_HEADER_MODE       0x01

#define REG_MODEM_CONFIG_2       0x1E
    //SPREADING FACTOR
    #define SPREADING_FACTOR_6         0x60
    #define SPREADING_FACTOR_7         0x70
    #define SPREADING_FACTOR_8         0x80
    #define SPREADING_FACTOR_9         0x90
    #define SPREADING_FACTOR_10        0xA0
    #define SPREADING_FACTOR_11        0xB0
    #define SPREADING_FACTOR_12        0xC0
    //Tx Continious mode
    #define TX_NORMAL_SINGLE_MODE      0x00
    #define TX_CONTINIOUS_MODE         0x08
    //Rx Payload Crc
    #define CRC_DISABLE                0x00
    #define CRC_ENABLE                 0x04    

#define REG_LR_SYMBTIMEOUTLSB    0x1F
#define REG_PREAMBLE_MSB         0x20
#define REG_PREAMBLE_LSB         0x21
#define REG_PAYLOAD_LENGTH       0x22
#define REG_LR_PAYLOADMAXLENGTH  0x23
#define REG_LR_HOPPERIOD         0x24
#define REG_MODEM_CONFIG_3       0x26
    //Low data rate optimize
    #define DATA_RATE_OPTIMIZE_DISABLED     0x00
    #define DATA_RATE_OPTIMIZE_ENABLED      0x08    
    //AGC Auto on
    #define GAIN_LNA_GAIN                   0x00
    #define GAIN_INTERANL_AGC_LOOP          0x04
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
    //Interrupt
    #define RFLR_DIOMAPPING1_RX_READY   0x00  // Default
    #define RFLR_DIOMAPPING1_DIO0_01    0x40
#define REG_LR_DIOMAPPING2       0x41
#define REG_VERSION              0x42
#define REG_PA_DAC               0x4d
    //Power
    #define PA_HIGH_POWER        0x87
    #define PA_HF_LF_OR_17_dBm   0x84

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
