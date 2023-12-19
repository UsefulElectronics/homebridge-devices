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
static void radar_handle_task(void* param);




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

	xTaskCreatePinnedToCore(radar_handle_task, "Real time Handler", 10000, NULL, 4, NULL, 0);

}


/**
 * @brief Initialize WiFi and connect to The configured WiFi network. and then connect to the MQTT broker
 *
 */
static void wirless_init_task(void* param)
{
	wifi_connect();

	vTaskDelay(pdMS_TO_TICKS(10000));

	mqtt_app_start();

	vTaskDelete(NULL);
}


static void mqtt_msg_pars_task(void *param)
{

	char temp_publish_packet_buffer[20] = {0};

    mqtt_buffer_t mqttSubscribeBuffer;
    //Let the lamp turned open once it is powered up
    hWs2811.bright = 100;

	while (1)
	{
		if(xQueueReceive(mqttSubscribe_queue, (void *)&mqttSubscribeBuffer, portMAX_DELAY))
		{
			 if(0 == memcmp(mqttSubscribeBuffer.topicString, MQTT_RGBLED_SET_HSV, sizeof(MQTT_RGBLED_SET_HSV)))
			 {
				 sscanf(mqttSubscribeBuffer.data, "%d, %d, %d",(int*) &hWs2811.hue, (int*) &hWs2811.sat, (int*) &hWs2811.bright);

				 led_strip_hsv2rgb(hWs2811.hue, hWs2811.sat, hWs2811.bright, &hWs2811.red, &hWs2811.green, &hWs2811.blue);

					for (int i = 0; i < RGB_COLOR_COUNT; i++)
					{
						for (int j = i; j < RGB_LED_NUMBER ; j += 3)
						{
							// Build RGB pixels
							hWs2811.led_strip_buffer[j * RGB_COLOR_COUNT + 0] = hWs2811.green;
							hWs2811.led_strip_buffer[j * RGB_COLOR_COUNT + 1] = hWs2811.red;
							hWs2811.led_strip_buffer[j * RGB_COLOR_COUNT + 2] = hWs2811.blue;
						}
					}

				 rmt_channel_send(hWs2811.led_strip_buffer, RGB_COLOR_COUNT * RGB_LED_NUMBER);

				 sprintf(temp_publish_packet_buffer, "%d, %d, %d", (int) hWs2811.hue, (int) hWs2811.sat, (int) hWs2811.bright);

				 mqtt_publish(MQTT_RGBLED_GET_HSV, &temp_publish_packet_buffer, strlen(temp_publish_packet_buffer));
			 }
			 else if(0 == memcmp(mqttSubscribeBuffer.topicString, MQTT_RGBLED_GET_HSV, sizeof(MQTT_RGBLED_GET_HSV)))
			 {
				 sprintf(temp_publish_packet_buffer, "%d, %d, %d", (int) hWs2811.hue, (int) hWs2811.sat, (int) hWs2811.bright);

				 mqtt_publish(MQTT_RGBLED_GET_HSV, &temp_publish_packet_buffer, strlen(temp_publish_packet_buffer));
			 }

		}

	}

}


static uint32_t main_get_systick(void)
{
	return SYS_TICK();
}

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
