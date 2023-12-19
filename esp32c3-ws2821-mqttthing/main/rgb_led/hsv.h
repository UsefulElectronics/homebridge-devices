/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  hsv.h
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Dec 17, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/

#ifndef MAIN_RGB_LED_HSV_H_
#define MAIN_RGB_LED_HSV_H_


/* INCLUDES ------------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* ENUMORATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/
/**
 * @brief 	Convert Hue, saturation and brightness values to 8 bit RGB value
 *
 * @param 	h	:	Used hue value which is the color angle between 0 and 360 that is used for conversion .
 *
 * @param 	s	:	Used saturation to determine how colorful the color is. this parameter is used for conversion.
 *
 * @param 	v	:	Used color brightness value to determine how bright the LED is. this parameter is used for conversion.
 *
 * @param 	r	:	Red color 8 bit value. Target pointer is passed to this function.
 *
 * @param 	g	:	Green color 8 bit value. Target pointer is passed to this function.
 *
 * @param 	b	:	Blue color 8 bit value. Target pointer is passed to this function.
 */
void led_strip_hsv2rgb(uint32_t h, uint32_t s, uint32_t v, uint32_t *r, uint32_t *g, uint32_t *b);



#endif /* MAIN_RGB_LED_HSV_H_ */

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
