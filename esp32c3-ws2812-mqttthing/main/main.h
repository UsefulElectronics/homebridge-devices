/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  main.h
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Jan 30, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/

#ifndef MAIN_MAIN_H_
#define MAIN_MAIN_H_


/* INCLUDES ------------------------------------------------------------------*/
#include "wireless/wifi_connect.h"
#include "wireless/mqtt.h"
#include "gpio/gpio_config.h"
#include <stdio.h>
#include <ctype.h>
/* MACROS --------------------------------------------------------------------*/
#define RMT_RGB_PIN		4		//2
#define TOUCH_PAD_PIN	7		//8

#define MQTT_DEVICE_ID	"1"
/* ENUMORATIONS --------------------------------------------------------------*/
typedef enum
{
	noti_idle	= 0,
	noti_button,
	noti_rotary_left,
	noti_rotary_right,
}notification_e;
/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/
extern TaskHandle_t hMain_uiTask;
/* FUNCTIONS DECLARATION -----------------------------------------------------*/



#endif /* MAIN_MAIN_H_ */

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
