/**
  ******************************************************************************
  *		(c) Copyright 2012-2023; USEFUL ELECTRONICS
  *
  *	All rights reserved.  Protected by international copyright laws.
  *	Knowledge of the source code may not be used to write a similar
  *	product.  This file may only be used in accordance with a license
  *	and should not be redistributed in any way.
  *
  *****************************************************************************/

/**
 ******************************************************************************
 * @Channel Link    :  https://www.youtube.com/user/wardzx1
 * @file    		:  mqtt.h
 * @author  		:  Ward Almasarani
 * @version 		:  v.1.0
 * @date    		:  Jan 2, 2023
 * @brief   		:
 *
 ******************************************************************************/

#ifndef MAIN_MQTT_H_
#define MAIN_MQTT_H_


/* INCLUDES ------------------------------------------------------------------*/
#include "string.h"
#include "esp_log.h"
#include "mqtt_client.h"
/* PRIVATE STRUCTRES ---------------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/
extern QueueHandle_t mqttSubscribe_queue;
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/
#define MQTT_MAX_TOPIC_LENGTH	30
#define MQTT_MAX_DATA_LENGTH	30

#define MQTT_MOTION_DETECT_TOPIC 		"motion/detect"
//#define MQTT_MOTION_DETECT_TOPIC 		"motion/status"


/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
typedef enum
{
	MQTT_BROKER_CONNECT = 0,
	MQTT_BROKER_DISCONNECT,
	MQTT_TOPIC_PUBLISHED,
	MQTT_TOPIC_DATA_RX
}mqtt_event_t;
typedef struct
{
	char 		 topicString[MQTT_MAX_TOPIC_LENGTH];
	uint16_t 	 msgLength;
	char		 data[MQTT_MAX_DATA_LENGTH];
	mqtt_event_t eventType;
}mqtt_buffer_t;
/* FUNCTION PROTOTYPES -------------------------------------------------------*/
/**
 * @brief Start MQTT broker connection and register MQTT related events callback
 *
 */
void mqtt_app_start(void);

void mqtt_publish(const char *topic, const char *data, int len);


#endif /* MAIN_MQTT_H_ */


/**************************  Useful Electronics  ****************END OF FILE***/
