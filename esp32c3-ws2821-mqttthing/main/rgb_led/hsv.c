/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  hsv.c
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Dec 17, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "hsv.h"

/* PRIVATE STRUCTRES ---------------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/
static rmt_handler_t	hRmt = {0};
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/

/* FUNCTION PROTOTYPES -------------------------------------------------------*/
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

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
