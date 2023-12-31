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


#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_log.h"
#include "esp_check.h"

#include "rmt_config.h"
#include "driver/rmt_tx.h"
#include "driver/rmt_encoder.h"
#include "soc/clk_tree_defs.h"
/* PRIVATE STRUCTRES ---------------------------------------------------------*/
typedef struct
{
	rmt_channel_handle_t 	channel;
	rmt_encoder_handle_t 	encoder;
    rmt_transmit_config_t 	transmitter;

}rmt_handler_t;

typedef struct
{
    uint32_t resolution; /*!< Encoder resolution, in Hz */
} led_strip_encoder_config_t;

typedef struct {
    rmt_encoder_t base;
    rmt_encoder_t *bytes_encoder;
    rmt_encoder_t *copy_encoder;
    int state;
    rmt_symbol_word_t reset_code;
} rmt_led_strip_encoder_t;
/* VARIABLES -----------------------------------------------------------------*/
static rmt_handler_t	hRmt = {0};

static const char *TAG = "rmt";

static uint8_t led_strip_pixels[RGB_LED_NUMBER * 3];
/* DEFINITIONS ---------------------------------------------------------------*/
#define RMT_LED_STRIP_RESOLUTION_HZ 10000000 // 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)
/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
/**
 * @brief Deletes and releases resources associated with an LED strip encoder in the RMT (Remote Control) module.
 *
 * This function deletes an LED strip encoder in the RMT module and releases associated resources.
 *
 * @param encoder Pointer to the LED strip encoder to be deleted.
 *
 * @return esp_err_t Returns ESP_OK on success, otherwise an error code.
 *
 * @note This function deallocates memory and resources used by the LED strip encoder.
 *
 * @details The function internally accesses the LED strip encoder structure from the provided generic RMT encoder pointer.
 *          It deletes the byte encoder and copy encoder associated with the LED strip encoder.
 *          Finally, it frees the memory allocated for the LED strip encoder structure.
 *
 */
static esp_err_t rmt_del_led_strip_encoder(rmt_encoder_t *encoder);

/**
 * @brief Encodes data for an LED strip using the RMT (Remote Control) module.
 *
 * This function encodes data for an LED strip using the RMT module's LED strip encoder.
 *
 * @param encoder Pointer to the LED strip encoder structure.
 * @param channel RMT channel handle used for transmission.
 * @param primary_data Pointer to the data to be encoded.
 * @param data_size Size of the data to be encoded in bytes.
 * @param ret_state Pointer to the state of the encoding process.
 *
 * @return size_t The number of symbols encoded.
 *
 * @note This function encodes data for the LED strip, including RGB data and reset code.
 *
 * @details The function switches between two states to encode RGB data and a reset code for the LED strip.
 *          It utilizes the byte encoder and copy encoder within the LED strip encoder structure for encoding.
 *          The encoding process continues until completion or until memory becomes full for encoding artifacts.
 */
static size_t rmt_encode_led_strip(rmt_encoder_t *encoder, rmt_channel_handle_t channel, const void *primary_data, size_t data_size, rmt_encode_state_t *ret_state);

/**
 * @brief Resets an LED strip encoder in the RMT (Remote Control) module.
 *
 * This function resets an LED strip encoder in the RMT module, clearing its internal state.
 *
 * @param encoder Pointer to the LED strip encoder to be reset.
 *
 * @return esp_err_t Returns ESP_OK on success, otherwise an error code.
 *
 * @note This function resets the byte encoder, copy encoder, and state of the LED strip encoder.
 *
 * @details The function internally accesses the LED strip encoder structure from the provided generic RMT encoder pointer.
 *          It performs a reset operation on the byte encoder, copy encoder, and resets the state of the LED strip encoder to its initial state.
 */
static esp_err_t rmt_led_strip_encoder_reset(rmt_encoder_t *encoder);

static esp_err_t rmt_new_led_strip_encoder(const led_strip_encoder_config_t *config, rmt_encoder_handle_t *ret_encoder);
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

static esp_err_t rmt_del_led_strip_encoder(rmt_encoder_t *encoder)
{
    rmt_led_strip_encoder_t *led_encoder = __containerof(encoder, rmt_led_strip_encoder_t, base);
    rmt_del_encoder(led_encoder->bytes_encoder);
    rmt_del_encoder(led_encoder->copy_encoder);
    free(led_encoder);
    return ESP_OK;
}

static size_t rmt_encode_led_strip(rmt_encoder_t *encoder, rmt_channel_handle_t channel, const void *primary_data, size_t data_size, rmt_encode_state_t *ret_state)
{
    rmt_led_strip_encoder_t *led_encoder = __containerof(encoder, rmt_led_strip_encoder_t, base);
    rmt_encoder_handle_t bytes_encoder = led_encoder->bytes_encoder;
    rmt_encoder_handle_t copy_encoder = led_encoder->copy_encoder;
    rmt_encode_state_t session_state = 0;
    rmt_encode_state_t state = 0;
    size_t encoded_symbols = 0;
    switch (led_encoder->state)
    {
    case 0: // send RGB data
        encoded_symbols += bytes_encoder->encode(bytes_encoder, channel, primary_data, data_size, &session_state);
        if (session_state & RMT_ENCODING_COMPLETE)
        {
            led_encoder->state = 1; // switch to next state when current encoding session finished
        }
        if (session_state & RMT_ENCODING_MEM_FULL)
        {
            state |= RMT_ENCODING_MEM_FULL;
            goto out; // yield if there's no free space for encoding artifacts
        }
    // fall-through
    case 1: // send reset code
        encoded_symbols += copy_encoder->encode(copy_encoder, channel, &led_encoder->reset_code,
                                                sizeof(led_encoder->reset_code), &session_state);
        if (session_state & RMT_ENCODING_COMPLETE)
        {
            led_encoder->state = 0; // back to the initial encoding session
            state |= RMT_ENCODING_COMPLETE;
        }
        if (session_state & RMT_ENCODING_MEM_FULL)
        {
            state |= RMT_ENCODING_MEM_FULL;
            goto out; // yield if there's no free space for encoding artifacts
        }
    }
out:
    *ret_state = state;
    return encoded_symbols;
}

static esp_err_t rmt_led_strip_encoder_reset(rmt_encoder_t *encoder)
{
    rmt_led_strip_encoder_t *led_encoder = __containerof(encoder, rmt_led_strip_encoder_t, base);
    rmt_encoder_reset(led_encoder->bytes_encoder);
    rmt_encoder_reset(led_encoder->copy_encoder);
    led_encoder->state = 0;
    return ESP_OK;
}

static esp_err_t rmt_new_led_strip_encoder(const led_strip_encoder_config_t *config, rmt_encoder_handle_t *ret_encoder)
{
    esp_err_t ret = ESP_OK;
    rmt_led_strip_encoder_t *led_encoder = NULL;
    ESP_GOTO_ON_FALSE(config && ret_encoder, ESP_ERR_INVALID_ARG, err, TAG, "invalid argument");
    led_encoder = calloc(1, sizeof(rmt_led_strip_encoder_t));
    ESP_GOTO_ON_FALSE(led_encoder, ESP_ERR_NO_MEM, err, TAG, "no mem for led strip encoder");

    led_encoder->base.encode 	= rmt_encode_led_strip;
    led_encoder->base.del 		= rmt_del_led_strip_encoder;
    led_encoder->base.reset 	= rmt_led_strip_encoder_reset;
    // different led strip might have its own timing requirements, following parameter is for WS2812
    rmt_bytes_encoder_config_t bytes_encoder_config =
    {
        .bit0 =
        {
            .level0 = 1,
            .duration0 = 0.3 * config->resolution / 1000000, // T0H=0.3us
            .level1 = 0,
            .duration1 = 0.9 * config->resolution / 1000000, // T0L=0.9us
        },
        .bit1 =
        {
            .level0 = 1,
            .duration0 = 0.9 * config->resolution / 1000000, // T1H=0.9us
            .level1 = 0,
            .duration1 = 0.3 * config->resolution / 1000000, // T1L=0.3us
        },
        .flags.msb_first = 1 // WS2812 transfer bit order: G7...G0R7...R0B7...B0
    };
    ESP_GOTO_ON_ERROR(rmt_new_bytes_encoder(&bytes_encoder_config, &led_encoder->bytes_encoder), err, TAG, "create bytes encoder failed");
    rmt_copy_encoder_config_t copy_encoder_config = {};
    ESP_GOTO_ON_ERROR(rmt_new_copy_encoder(&copy_encoder_config, &led_encoder->copy_encoder), err, TAG, "create copy encoder failed");

    uint32_t reset_ticks = config->resolution / 1000000 * 50 / 2; // reset code duration defaults to 50us
    led_encoder->reset_code = (rmt_symbol_word_t) {
        .level0 = 0,
        .duration0 = reset_ticks,
        .level1 = 0,
        .duration1 = reset_ticks,
    };
    *ret_encoder = &led_encoder->base;
    return ESP_OK;
err:
    if (led_encoder)
    {
        if (led_encoder->bytes_encoder)
        {
            rmt_del_encoder(led_encoder->bytes_encoder);
        }
        if (led_encoder->copy_encoder)
        {
            rmt_del_encoder(led_encoder->copy_encoder);
        }
        free(led_encoder);
    }
    return ret;
}
/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
