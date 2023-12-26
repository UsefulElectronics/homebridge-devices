/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  main.c
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Jan 30, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "main.h"

#include "rgb_led/rmt_config.h"
#include "rgb_led/hsv.h"
#include "stdbool.h"


/* MACROS --------------------------------------------------------------------*/
#define SYSTEM_BUFFER_SIZE		4
/* DEFINITIONS ---------------------------------------------------------------*/
#define SYS_TICK()				(xTaskGetTickCount() * portTICK_PERIOD_MS)
/* PRIVATE STRUCTRES ---------------------------------------------------------*/
typedef struct

{
    uint32_t hue;
    uint32_t sat;
    uint32_t bright;

    uint32_t red;
    uint32_t green;
    uint32_t blue;

    uint8_t led_strip_buffer[RGB_LED_NUMBER * RGB_LED_NUMBER];

    bool led_strip_status;

}ws2812_handler_t;

ws2812_handler_t hWs2812 = {0};
/* VARIABLES -----------------------------------------------------------------*/
static const char *TAG = "main";

TaskHandle_t hMain_uiTask 				= NULL;
/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/

static void wirless_init_task(void* param);
static void mqtt_msg_pars_task(void *param);

static void system_led_buffer_load(uint32_t red, uint32_t green, uint32_t blue);
static void mqtt_publish_task(void *param);

/* FUNCTION PROTOTYPES -------------------------------------------------------*/
/**
 * @brief 	Initialize system peripherals and create FreeRTOS tasks
 *
 */
void app_main(void)
{

	rmt_config(RMT_RGB_PIN);

	xTaskCreatePinnedToCore(wirless_init_task, "WiFi init", 10000, NULL, 4, NULL, 0);

	//Wait for WiFi and MQTT broker connection to be established.
	vTaskDelay(pdMS_TO_TICKS(15000));

	xTaskCreatePinnedToCore(mqtt_msg_pars_task, "mqtt parser Handler", 10000, NULL, 4, NULL, 0);

//	xTaskCreatePinnedToCore(mqtt_publish_task, "Real time Handler", 10000, NULL, 4, NULL, 0);

}


/**
 * @brief Initialize WiFi and connect to The configured WiFi network. and then connect to the MQTT broker
 *
 */
static void wirless_init_task(void* param)
{
	wifi_connect();

	mqtt_app_start();

	vTaskDelete(NULL);
}


static void mqtt_msg_pars_task(void *param)
{

    static mqtt_buffer_t mqttSubscribeBuffer = {0};

	while (1)
	{
		if(xQueueReceive(mqttSubscribe_queue, (void *)&mqttSubscribeBuffer, portMAX_DELAY))
		{
			ESP_LOGI(TAG, "data %s", mqttSubscribeBuffer.data);
			ESP_LOGI(TAG, "topicString %s", mqttSubscribeBuffer.topicString);
			 if(0 == strcmp(mqttSubscribeBuffer.topicString, MQTT_RGBLED_SET_HSV))
			 {
				 sscanf(mqttSubscribeBuffer.data, "%d, %d, %d",(int*) &hWs2812.hue, (int*) &hWs2812.sat, (int*) &hWs2812.bright);

				 ESP_LOGI(TAG, "mqtt set hsv %s", mqttSubscribeBuffer.data);

				 hWs2812.led_strip_status = true;

				 led_strip_hsv2rgb(hWs2812.hue, hWs2812.sat, hWs2812.bright, &hWs2812.red, &hWs2812.green, &hWs2812.blue);

				 system_led_buffer_load(hWs2812.red, hWs2812.green, hWs2812.blue);

				 rmt_channel_send(hWs2812.led_strip_buffer, RGB_COLOR_COUNT * RGB_LED_NUMBER);


			 }
			 else if(0 == strcmp(mqttSubscribeBuffer.topicString, MQTT_RGBLED_SET_ON))
			 {
				 bool led_Status = atoi(mqttSubscribeBuffer.data);

				 ESP_LOGI(TAG, "mqtt set status %s", mqttSubscribeBuffer.data);

				 if(led_Status)
				 {
					 system_led_buffer_load(hWs2812.red, hWs2812.green, hWs2812.blue);

					 hWs2812.led_strip_status = true;
				 }
				 else
				 {
					 system_led_buffer_load(0, 0, 0);

					 hWs2812.led_strip_status = false;
				 }
				 rmt_channel_send(hWs2812.led_strip_buffer, RGB_COLOR_COUNT * RGB_LED_NUMBER);

				 memset(&mqttSubscribeBuffer, 0, sizeof(mqtt_buffer_t));
			 }

		}

	}

}

static void mqtt_publish_task(void *param)
{
	TickType_t xLastWakeTime = xTaskGetTickCount();

	char temp_topic_string[20] = {0};

	char temp_publish_string[20] = {0};

	bool topic_toggle = false;

	while(1)
	{
		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(5000) );

		topic_toggle ^= 1;

		switch (topic_toggle)
		{
			case 1:
				printf(temp_publish_string, "%d",hWs2812.led_strip_status);

				mqtt_publish(MQTT_RGBLED_GET_ON, &temp_publish_string, 1);
				break;

			case 0:
				sprintf(temp_publish_string, "%d, %d, %d",(int) hWs2812.hue, (int) hWs2812.sat, (int) hWs2812.bright);

				mqtt_publish(temp_topic_string, &temp_publish_string, strlen(temp_publish_string));

				break;
			default:
				break;
		}

	}
}


static uint32_t main_get_systick(void)
{
	return SYS_TICK();
}

static void system_led_buffer_load(uint32_t red, uint32_t green, uint32_t blue)
{
	for (int i = 0; i < RGB_COLOR_COUNT; i++)
	{
		for (int j = i; j < RGB_LED_NUMBER ; j += 3)
		{
			// Build RGB pixels
			hWs2812.led_strip_buffer[j * RGB_COLOR_COUNT + 0] = green;
			hWs2812.led_strip_buffer[j * RGB_COLOR_COUNT + 1] = red;
			hWs2812.led_strip_buffer[j * RGB_COLOR_COUNT + 2] = blue;
		}
	}
}
/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
