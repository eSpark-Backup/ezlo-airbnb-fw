#include <main.h>
#include <arm_math.h>
#include <stdbool.h>

arm_biquad_cascade_df2T_instance_f32 iir_filter;
#define IIRSTAGES 3
static uint32_t iir_stages = IIRSTAGES;
static float32_t coeffs[5 * IIRSTAGES] = {
	0.28147504346,  -0.56291022628,   0.28143518563,   0.04318103380,  -0.00046615042,
	1.00000000000,  -2.00014161345,   1.00014162348,   1.99353908705,  -0.99354952290,
	1.00000000000,   2.00000000000,   1.00000000000,   1.87367745134,  -0.87551448640
};
float32_t iir_state[2 * IIRSTAGES];
static const uint32_t aop = 120;            /* 120dB SPL */
static const uint32_t snr = 61;             /* 61dB */
static const int32_t sensitivity = -26;     /* -26dbFS */
static uint32_t averaging_time_point = 0;

float32_t alpha;
float32_t output;
float32_t time_constant;

float32_t spl;
float32_t spl_data_period;

void a_weighting_a_weighting()
{
	arm_biquad_cascade_df2T_init_f32(&iir_filter, iir_stages, coeffs, iir_state);
}

bool a_weighting_process(float32_t* input, float* output, uint32_t size)
{
    arm_biquad_cascade_df2T_f32(&iir_filter, input, output, size);
    return true;
}

void averaging_filter_averaging_filter(float32_t l_time_constant, float32_t l_time_delta, float32_t l_inital_value)
{
    time_constant = l_time_constant;
    alpha = 1 - expf(-l_time_delta / time_constant);
    output = l_inital_value;
}

float32_t averaging_filter_process(float32_t input)
{
    return output = (1 - alpha) * output + alpha * input;
}

void meterSetup(void)
{
    spl = 0;
    spl_data_period = 16.f / 16000;

    a_weighting_a_weighting();
    averaging_filter_averaging_filter(0.100f, spl_data_period, spl);
}

float32_t meter_process(float32_t* input, uint32_t size)
{
	float32_t aux_dsp_buffer[16];
	float32_t *current_dsp_buffer;
    /* Apply weighting filter */
	a_weighting_process(input, aux_dsp_buffer, size);

    current_dsp_buffer = aux_dsp_buffer;

    /* Delete offset */
    float32_t mean;
    arm_mean_f32(current_dsp_buffer, size, &mean);
    arm_offset_f32(current_dsp_buffer, -mean, current_dsp_buffer, size);

    /* Calculate RMS value */
    float32_t rms;
    arm_rms_f32(current_dsp_buffer, size, &rms);

    /* Normalize RMS value */
    rms /= INT16_MAX;

    /* Calculate dB SPL */
    float32_t db_spl_raw = 94 - sensitivity + 20.0f * log10f(rms);

    /* Apply averaging */
    float32_t db_spl = averaging_filter_process(db_spl_raw);

    const uint32_t averaging_period = time_constant * 1000;

    if ((HAL_GetTick() - averaging_time_point) > averaging_period)
    {
    	averaging_time_point = HAL_GetTick();

//        this->spl_data.spl_max = std::max(db_spl, this->spl_data.spl_max);
//        this->spl_data.spl_min = std::min(db_spl, this->spl_data.spl_min);
    	spl = db_spl;
    	extern UART_HandleTypeDef huart2;
    	HAL_UART_Transmit(&huart2, &spl, sizeof(spl), 1000);

//        if (this->new_spl_data_cb != nullptr)
//            this->new_spl_data_cb(this->spl_data);
    }


    return spl;
}
