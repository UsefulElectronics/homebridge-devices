/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  rmt_config.h
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Dec 17, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/

#ifndef MAIN_RGB_LED_RMT_CONFIG_H_
#define MAIN_RGB_LED_RMT_CONFIG_H_


/* INCLUDES ------------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/
#define RGB_LED_NUMBER		120//52	//69
#define RGB_COLOR_COUNT		3
/* ENUMORATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/
/**
 * @brief Configures and initializes the RMT (Remote Control) transmitter for LED control.
 *
 * This function configures and initializes the RMT transmitter for LED control on the specified GPIO pin.
 *
 * @param gpio_pin The GPIO pin number to which the RMT transmitter is connected.
 *
 * @note This function sets up the RMT transmitter channel and LED strip encoder.
 *
 * @details The function creates an RMT TX channel and an LED strip encoder with the specified configurations.
 *          It configures parameters for the RMT channel such as clock source, GPIO pin, memory block size,
 *          resolution, and transaction queue depth for transmitting LED control signals.
 *          Additionally, it enables the RMT TX channel, initializes the LED strip encoder, and starts transmitting LED data.
 *
 */
void rmt_config(uint8_t gpio_pin);
/**
 * @brief Sends data through the configured RMT (Remote Control) transmitter channel.
 *
 * This function sends data through the configured RMT transmitter channel using the provided data buffer and size.
 *
 * @param data_buffer A pointer to the buffer containing the data to be transmitted.
 * @param data_size The size of the data to be transmitted in bytes.
 *
 * @note This function uses the configured RMT channel and encoder to transmit data through the transmitter.
 *
 * @details The function uses the RMT transmitter channel, encoder, and transmitter configuration stored in the global handler 'hRmt'.
 *          It transmits the data from the specified buffer with the given size using the RMT transmitter.
 *
 */
void rmt_channel_send(uint8_t* data_buffer, uint16_t data_size);
#endif /* MAIN_RGB_LED_RMT_CONFIG_H_ */

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
