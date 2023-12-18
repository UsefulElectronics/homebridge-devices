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
#define RGB_LED_NUMBER		24
#define RGB_COLOR_COUNT		3
/* ENUMORATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/
/**
 * @brief 	This function is responsible for initializing RMT peripheral to encode WS8212
 * 			LED strip data packets and copy the peripheral parameters to rmt_handler_t hRmt
 *
 */
void rmt_config(uint8_t gpio_pin);

void rmt_channel_send(uint8_t* data_buffer, uint16_t data_size);
#endif /* MAIN_RGB_LED_RMT_CONFIG_H_ */

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
