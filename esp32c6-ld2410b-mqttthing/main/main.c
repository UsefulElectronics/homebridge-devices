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




/* MACROS --------------------------------------------------------------------*/
#define SYSTEM_BUFFER_SIZE		4
/* DEFINITIONS ---------------------------------------------------------------*/
#define SYS_TICK()				(xTaskGetTickCount() * portTICK_PERIOD_MS)
/* PRIVATE STRUCTRES ---------------------------------------------------------*/
typedef struct
{
	uint8_t packet_size;
	uint16_t data[SYSTEM_BUFFER_SIZE];
}system_packet;

bool prev_radar_status = false;
/* VARIABLES -----------------------------------------------------------------*/
static const char *main = "main";

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

	gpio_config_input(RADAR_PIN);

	 xTaskCreatePinnedToCore(wirless_init_task, "WiFi init", 10000, NULL, 4, NULL, 0);

     //Wait for WiFi and MQTT broker connection to be established.
     vTaskDelay(pdMS_TO_TICKS(15000));

     xTaskCreatePinnedToCore(radar_handle_task, "Real time Handler", 10000, NULL, 4, NULL, 1);

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



static void radar_handle_task(void* param)
{

	char temp_topic_string[20] = {0};

	sprintf(temp_topic_string, "%s/%s",MQTT_DEVICE_ID, MQTT_MOTION_DETECT_TOPIC);

	TickType_t xLastWakeTime = xTaskGetTickCount();

	static uint32_t tempUpdateCounter = 0;

	char publishRequest = 0;
	while(1)
	{
		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(5000) );

		++tempUpdateCounter;

		if(publishRequest != gpio_get_level(RADAR_PIN))
		{
			publishRequest = gpio_get_level(RADAR_PIN);

			mqtt_publish(temp_topic_string, &publishRequest, 1);

		}

	}
}

static uint32_t main_get_systick(void)
{
	return SYS_TICK();
}

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
