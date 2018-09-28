/*QuangNT*/
#include "nrf_saadc.h"
#include "nrf_drv_saadc.h"
void mysaadc_init(void)
{
    nrf_saadc_resolution_t resolution;
    nrf_saadc_oversample_t oversample;
    resolution = NRF_SAADC_RESOLUTION_12BIT;
    oversample = SAADC_OVERSAMPLE_OVERSAMPLE_Bypass;

    nrf_saadc_channel_config_t channel_0_config =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN4);
    channel_0_config.gain = NRF_SAADC_GAIN1_4;
    channel_0_config.reference = NRF_SAADC_REFERENCE_VDD4;

    nrf_saadc_channel_config_t channel_1_config =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN5);
    channel_1_config.gain = NRF_SAADC_GAIN1_4;
    channel_1_config.reference = NRF_SAADC_REFERENCE_VDD4;

    nrf_saadc_channel_config_t channel_2_config =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN6);
    channel_2_config.gain = NRF_SAADC_GAIN1_4;
    channel_2_config.reference = NRF_SAADC_REFERENCE_VDD4;

    nrf_saadc_channel_config_t channel_3_config =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN7);
    channel_3_config.gain = NRF_SAADC_GAIN1_4;
    channel_3_config.reference = NRF_SAADC_REFERENCE_VDD4;

    nrf_saadc_resolution_set(resolution);
    nrf_saadc_oversample_set(oversample);

    nrf_saadc_channel_init(0, &channel_0_config);
    //nrf_saadc_channel_input_set(0, channel_0_config->pin_p, channel_0_config->pin_n);
}
