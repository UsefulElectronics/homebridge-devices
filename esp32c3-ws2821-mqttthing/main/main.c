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

    uint8_t led_strip_buffer[RGB_LED_NUMBER];

}ws2811_handler_t;

ws2811_handler_t hWs2811 = {0};
/* VARIABLES -----------------------------------------------------------------*/
static const char *TAG = "main";

TaskHandle_t hMain_uiTask 				= NULL;
/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/

static void wirless_init_task(void* param);
static void mqtt_msg_pars_task(void *param);

static void system_led_buffer_load(uint32_t red, uint32_t green, uint32_t blue);


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

	xTaskCreatePinnedToCore(mqtt_msg_pars_task, "Real time Handler", 10000, NULL, 4, NULL, 0);

}


/**
 * @brief Initialize WiFi and connect to The configured WiFi network. and then connect to the MQTT broker
 *
 */
static void wirless_init_task(void* param)
{
	wifi_connect();

	mqtt_app_start();

	system_led_buffer_load(0, 0, 0);

	vTaskDelete(NULL);
}


static void mqtt_msg_pars_task(void *param)
{

    static mqtt_buffer_t mqttSubscribeBuffer = {0};
    //Let the lamp turned open once it is powered up
    hWs2811.bright = 100;

	while (1)
	{
		if(xQueueReceive(mqttSubscribe_queue, (void *)&mqttSubscribeBuffer, portMAX_DELAY))
		{
			ESP_LOGI(TAG, "data %s", mqttSubscribeBuffer.data);
			ESP_LOGI(TAG, "topicString %s", mqttSubscribeBuffer.topicString);
			 if(0 == strcmp(mqttSubscribeBuffer.topicString, MQTT_RGBLED_SET_HSV))
			 {
				 sscanf(mqttSubscribeBuffer.data, "%d, %d, %d",(int*) &hWs2811.hue, (int*) &hWs2811.sat, (int*) &hWs2811.bright);

				 ESP_LOGI(TAG, "mqtt set hsv %s", mqttSubscribeBuffer.data);

				 led_strip_hsv2rgb(hWs2811.hue, hWs2811.sat, hWs2811.bright, &hWs2811.red, &hWs2811.green, &hWs2811.blue);

				 system_led_buffer_load(hWs2811.red, hWs2811.green, hWs2811.blue);

				 rmt_channel_send(hWs2811.led_strip_buffer, RGB_COLOR_COUNT * RGB_LED_NUMBER);


			 }
			 else if(0 == strcmp(mqttSubscribeBuffer.topicString, MQTT_RGBLED_SET_ON))
			 {
				 bool led_Status = atoi(mqttSubscribeBuffer.data);

				 ESP_LOGI(TAG, "mqtt set status %s", mqttSubscribeBuffer.data);

				 if(led_Status)
				 {
					 system_led_buffer_load(hWs2811.red, hWs2811.green, hWs2811.blue);
				 }
				 else
				 {
					 system_led_buffer_load(0, 0, 0);
				 }
				 rmt_channel_send(hWs2811.led_strip_buffer, RGB_COLOR_COUNT * RGB_LED_NUMBER);

				 memset(&mqttSubscribeBuffer, 0, sizeof(mqtt_buffer_t));
			 }

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
			hWs2811.led_strip_buffer[j * RGB_COLOR_COUNT + 0] = green;
			hWs2811.led_strip_buffer[j * RGB_COLOR_COUNT + 1] = red;
			hWs2811.led_strip_buffer[j * RGB_COLOR_COUNT + 2] = blue;
		}
	}
}
/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
