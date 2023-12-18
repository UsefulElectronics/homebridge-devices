/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  rmt_config.c
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Dec 17, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "rmt_config.h"

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "driver/rmt_tx.h"
/* PRIVATE STRUCTRES ---------------------------------------------------------*/
typedef struct
{
	rmt_channel_handle_t 	channel;
	rmt_encoder_handle_t 	encoder;
    rmt_transmit_config_t 	transmitter;

}rmt_handler_t;
/* VARIABLES -----------------------------------------------------------------*/
static rmt_handler_t	hRmt = {0};

static uint8_t led_strip_pixels[RGB_LED_NUMBER * 3];
/* DEFINITIONS ---------------------------------------------------------------*/
#define RMT_LED_STRIP_RESOLUTION_HZ 10000000 // 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)
/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
static void rmt_config(void);
/* FUNCTION PROTOTYPES -------------------------------------------------------*/

void rmt_config(uint8_t gpio_pin)
{
    ESP_LOGI(TAG, "Create RMT TX channel");
    rmt_channel_handle_t led_chan = NULL;
    rmt_tx_channel_config_t tx_chan_config =
    {
        .clk_src = RMT_CLK_SRC_DEFAULT, // select source clock
        .gpio_num = gpio_pin,
        .mem_block_symbols = 64, 		// increase the block size can make the LED less flickering
        .resolution_hz = RMT_LED_STRIP_RESOLUTION_HZ,
        .trans_queue_depth = 4, 		// set the number of transactions that can be pending in the background
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &led_chan));

    ESP_LOGI(TAG, "Install led strip encoder");
    rmt_encoder_handle_t led_encoder = NULL;
    led_strip_encoder_config_t encoder_config =
    {
        .resolution = RMT_LED_STRIP_RESOLUTION_HZ,
    };
    ESP_ERROR_CHECK(rmt_new_led_strip_encoder(&encoder_config, &led_encoder));

    ESP_LOGI(TAG, "Enable RMT TX channel");
    ESP_ERROR_CHECK(rmt_enable(led_chan));

    ESP_LOGI(TAG, "Start LED rainbow chase");
    rmt_transmit_config_t tx_config =
    {
        .loop_count = 0, // no transfer loop
    };

    //Copy RMT parameter to the global handler to manage them from a different task
    memcpy(&hRmt.channel, 		&led_chan,		sizeof(rmt_channel_handle_t));
    memcpy(&hRmt.encoder, 		&led_encoder,	sizeof(rmt_encoder_handle_t));
    memcpy(&hRmt.transmitter, 	&tx_config,		sizeof(rmt_transmit_config_t));
}

void rmt_channel_send(uint8_t* data_buffer, uint16_t data_size)
{
	rmt_transmit(hRmt.channel, hRmt.encoder, data_buffer, data_size, &hRmt.transmitter);
}


/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
