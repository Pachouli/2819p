/*
 mic閫氶亾娴嬭瘯
 */

#include "ap_test_head.h"
#include "ap_autotest_main_entry.h"
#include "bank_common/ap_autotest_common_entry.h"
#include "bank_include/ap_autotest_char_convert.h"
#include "hal_audio_extern.h"
#include "ap_autotest_channel_test.h"
#include "ap_autotest_channel_test_SNR.h"

#define ADC_CHAN_DMA_CHAN  DMA_CHAN_DMA0
#define DAC_CHAN_DMA_CHAN  DMA_CHAN_DMA2

//DAC数据缓存buffer，共1k的数据量 
#define DAC_BUFFER            0x9fc33f00       //     (0x9fc22400) 
#define DAC_BUFFER_LEN        (0x400)

//数据采集buffer
#define SOUND_DATA_ADDR      (DAC_BUFFER + DAC_BUFFER_LEN) 
//每次采集共4k的数据量
#define SOUND_DATA_LEN              (0x1000)

//16个点
const short pcm_table[] =
    {
        0x6ba, 0x371f, 0x5f22, 0x78a6, 0x7fce, 0x7328, 0x55c9, 0x2ab2,
        0xf945, 0xc8df, 0xa0de, 0x8757, 0x802f, 0x8c7f, 0xaa63, 0xd550
    };

void init_linein_dac_buffer(void)
{
    uint8 i, j;

    short *p_dac_buffer = (short *) DAC_BUFFER;

    for (j = 0; j < 16; j++)
    {
        for (i = 0; i < 16; i++)
        {
            //左右声道使用相同的数据
            *p_dac_buffer = pcm_table[i]; //
            p_dac_buffer++;
            *p_dac_buffer = pcm_table[i];
            p_dac_buffer++;
        }
    }

    return;
}

//使用1K的缓存 16*16*2*2=1k, 共256个采样对
void init_dac_buffer(void)
{
    uint8 i, j;

    short *p_dac_buffer = (short *) DAC_BUFFER;

    for (j = 0; j < 16; j++)
    {
        for (i = 0; i < 16; i++)
        {
            //左右声道使用相同的数据
            *p_dac_buffer = pcm_table[i] / 20;
            p_dac_buffer++;
            *p_dac_buffer = pcm_table[i] / 20;
            p_dac_buffer++;
        }
    }

    return;
}

void clear_cache()
{
    libc_memset((uint8*) SOUND_DATA_ADDR, 0, 0x1000);
}

void act_test_report_channel_result(uint32 test_mode, uint16 test_id, int32 ret_val, void *arg_buffer,
        uint8 * log_buffer, uint32 * cur_total_len)
{
    return_result_t *return_data;
    uint16 trans_bytes = 0;
    channel_test_arg_t *channel_test_arg = (channel_test_arg_t *) arg_buffer;

    if (test_mode != TEST_MODE_CARD)
    {
        uint8 * stub_rw_buffer = sys_malloc(STUB_ATT_RW_TEMP_BUFFER_LEN);
        if (NULL == stub_rw_buffer)
        {
            libc_printf("report_channel_result: stub_rw_buffer null\n");
            while (1)
            {
            }
        }
        return_data = (return_result_t *) (stub_rw_buffer);

        return_data->test_id = test_id;

        return_data->test_result = ret_val;

        int32_to_unicode(channel_test_arg->left_ch_power_threadshold, &(return_data->return_arg[trans_bytes]),
                &trans_bytes, 10);

        return_data->return_arg[trans_bytes++] = 0x002c;

        int32_to_unicode(channel_test_arg->right_ch_power_threadshold, &(return_data->return_arg[trans_bytes]),
                &trans_bytes, 10);

        return_data->return_arg[trans_bytes++] = 0x002c;

        int32_to_unicode(channel_test_arg->left_ch_SNR_threadshold, &(return_data->return_arg[trans_bytes]),
                &trans_bytes, 10);

        return_data->return_arg[trans_bytes++] = 0x002c;

        int32_to_unicode(channel_test_arg->right_ch_SNR_threadshold, &(return_data->return_arg[trans_bytes]),
                &trans_bytes, 10);

        return_data->return_arg[trans_bytes++] = 0x002c;

        //如果参数未四字节对齐，要四字节对齐处理
        if ((trans_bytes % 2) != 0)
        {
            return_data->return_arg[trans_bytes++] = 0x0000;
        }

        //act_test_report_result(return_data, trans_bytes * 2 + 4);
        STUB_REPORT_RESULT((uint8*) return_data, trans_bytes * 2 + 4);

        sys_free(stub_rw_buffer);
    }
    else
    {
        //act_test_report_test_log(ret_val, test_id);
        if (TESTID_LINEIN_CH_TEST == test_id)
        {
            ATT_TEST_ADD_RESULT_TO_SD_LOG("linein channel test:", ret_val, log_buffer, cur_total_len);
        }
        else if (TESTID_LINEIN_CH_TEST == test_id)
        {
            ATT_TEST_ADD_RESULT_TO_SD_LOG("mic channel test:", ret_val, log_buffer, cur_total_len);
        }

    }
}

int32 act_test_mic_channel_test_ext(void *arg_buffer, uint32 test_mode, uint8* log_buffer, uint32* cur_log_len)
{
    channel_test_arg_t *channel_test_arg = (channel_test_arg_t *) arg_buffer;
    int8 result_L = TRUE;
    int8 result_R = TRUE;
    int8 result_Final;

    com_reset_sound_volume (SET_VOL_BY_SYSTEM);
//1、初始化数字信号
    {
        init_dac_buffer();
    }
//2、对数字信号dac（dma不停地将数字信号送到dac的fifo）
    {
        //enable dac
        dac_setting_t dac_sett;
        dma_chan_setting_t dma_sett;
        clear_cache();

        libc_memset(&dac_sett, 0x00, sizeof(dac_setting_t));
        dac_sett.sample_rate = ADC_SAMPLE_RATE;
        dac_sett.aps = 4; /*AUD_APS_FAST*/

        enable_dac(&dac_sett);

        //enable dma
        sys_reset_dma_chan (DAC_CHAN_DMA_CHAN);

        libc_memset(&dma_sett, 0x00, sizeof(dma_chan_setting_t));
        dma_sett.src_mem_addr = (uint8 *) DAC_BUFFER;
        dma_sett.frame_len = DAC_BUFFER_LEN / sizeof(short); //DAC_BUFFER_LEN
        dma_sett.src_type = DMA_SRC_MEM;
        dma_sett.dst_type = DMA_DST_DAC_I2STX;
        dma_sett.reload = TRUE;
        dma_sett.start = TRUE;
        sys_set_dma_chan(DAC_CHAN_DMA_CHAN, &dma_sett);

    }

//mic0
//3、对模拟信号adc（并存储到mem）
    {

        ain_setting_t ain_sett;
        adc_setting_t adc_sett;
        dma_chan_setting_t dma_sett;
        clear_cache();

        //使能audio in
        ain_sett.op_gain.mic_op_gain = MIC_OP_26DB;
        ain_sett.ain_pa_mode = FALSE;
        enable_ain(AIN_SOURCE_MIC1, &ain_sett);

        //adc开始采样
        adc_sett.sample_rate = ADC_SAMPLE_RATE;
        adc_sett.gain = 0;
        enable_adc(AIN_SOURCE_MIC1, &adc_sett);

        sys_reset_dma_chan (ADC_CHAN_DMA_CHAN);
        libc_memset(&dma_sett, 0x00, sizeof(dma_chan_setting_t));
        dma_sett.dst_mem_addr = (uint8 *) SOUND_DATA_ADDR;
        dma_sett.frame_len = (0x1000) / sizeof(short);
        dma_sett.irq_cbk = NULL;
        dma_sett.src_type = DMA_SRC_ADC; //from adc fifo
        dma_sett.dst_type = DMA_DST_MEM;
        dma_sett.hf_ie = FALSE;
        dma_sett.tc_ie = FALSE;
        dma_sett.reload = TRUE;
        dma_sett.start = TRUE;
        sys_set_dma_chan(ADC_CHAN_DMA_CHAN, &dma_sett); //dma start

        sys_os_time_dly(30);

        sys_reset_dma_chan(ADC_CHAN_DMA_CHAN);

        dma_sett.reload = FALSE;
        sys_set_dma_chan(ADC_CHAN_DMA_CHAN, &dma_sett); //dma start
        sys_wait_dma_chan_tc(ADC_CHAN_DMA_CHAN, WAIT_DMA_TC_ALWAYS);

        sys_reset_dma_chan(ADC_CHAN_DMA_CHAN);
        disable_adc (AIN_SOURCE_MIC1);
        disable_ain(AIN_SOURCE_MIC1);
        //disable_dac();

        //4、比对数据
        result_L = thd_test((void*) SOUND_DATA_ADDR, channel_test_arg->left_ch_SNR_threadshold,
                channel_test_arg->left_ch_max_sig_point);

    }

    //mic1
    //3、对模拟信号adc（并存储到mem）
    {

        ain_setting_t ain_sett;
        adc_setting_t adc_sett;
        dma_chan_setting_t dma_sett;
        clear_cache();

        //使能audio in
        ain_sett.op_gain.mic_op_gain = MIC_OP_26DB;
        ain_sett.ain_pa_mode = FALSE;
        enable_ain(AIN_SOURCE_MIC2, &ain_sett);

        //adc开始采样
        adc_sett.sample_rate = ADC_SAMPLE_RATE;
        adc_sett.gain = 0;
        enable_adc(AIN_SOURCE_MIC2, &adc_sett);

        sys_reset_dma_chan (ADC_CHAN_DMA_CHAN);
        libc_memset(&dma_sett, 0x00, sizeof(dma_chan_setting_t));
        dma_sett.dst_mem_addr = (uint8 *) SOUND_DATA_ADDR;
        dma_sett.frame_len = (0x1000) / sizeof(short);
        dma_sett.irq_cbk = NULL;
        dma_sett.src_type = DMA_SRC_ADC; //from adc fifo
        dma_sett.dst_type = DMA_DST_MEM;
        dma_sett.hf_ie = FALSE;
        dma_sett.tc_ie = FALSE;
        dma_sett.reload = TRUE;
        dma_sett.start = TRUE;
        sys_set_dma_chan(ADC_CHAN_DMA_CHAN, &dma_sett); //dma start

        sys_os_time_dly(30);

        sys_reset_dma_chan(ADC_CHAN_DMA_CHAN);

        dma_sett.reload = FALSE;
        sys_set_dma_chan(ADC_CHAN_DMA_CHAN, &dma_sett); //dma start
        sys_wait_dma_chan_tc(ADC_CHAN_DMA_CHAN, WAIT_DMA_TC_ALWAYS);

        sys_reset_dma_chan(ADC_CHAN_DMA_CHAN);
        disable_adc (AIN_SOURCE_MIC2);
        disable_ain(AIN_SOURCE_MIC2);
        //disable_dac();

        //4、比对数据
        result_R = thd_test((void*) SOUND_DATA_ADDR, channel_test_arg->left_ch_SNR_threadshold,
                channel_test_arg->left_ch_max_sig_point);

    }

    result_Final = FALSE;
    if (result_L && result_R)
    {
        result_Final = TRUE;
    }

    act_test_report_channel_result(test_mode, TESTID_MIC_CH_TEST, result_Final, arg_buffer, log_buffer, cur_log_len);

    return result_Final;

}

int32 act_test_linein_channel_test_ext(void *arg_buffer, uint32 test_mode, uint8* log_buffer, uint32* cur_log_len)
{

    int8 result_0_L = TRUE, result_0_R = TRUE, result_1_L = TRUE, result_1_R = TRUE, result_Final;

    channel_test_arg_t *channel_test_arg = (channel_test_arg_t*) arg_buffer;
    dma_chan_setting_t dma_sett;
    dac_setting_t dac_sett;

    com_reset_sound_volume (SET_VOL_BY_SYSTEM);

//1、初始化数字信号	
    {
        init_linein_dac_buffer();
    }
//2、对数字信号dac
    {
        //enable dac
        dac_setting_t dac_sett;
        dma_chan_setting_t dma_sett;

        libc_memset(&dac_sett, 0x00, sizeof(dac_setting_t));
        dac_sett.sample_rate = 16;
        dac_sett.aps = 4; /*AUD_APS_FAST;*/

        //enable dac
        enable_dac(&dac_sett);
        sys_reset_dma_chan (DAC_CHAN_DMA_CHAN);
        libc_memset(&dma_sett, 0x00, sizeof(dma_chan_setting_t));
        dma_sett.src_mem_addr = (uint8 *) DAC_BUFFER;
        dma_sett.frame_len = DAC_BUFFER_LEN / sizeof(short); //DAC_BUFFER_LEN
        dma_sett.src_type = DMA_SRC_MEM;
        dma_sett.dst_type = DMA_DST_DAC_I2STX;
        dma_sett.reload = TRUE; //  这里重复产生1k正弦波
        dma_sett.start = TRUE;
        sys_set_dma_chan(DAC_CHAN_DMA_CHAN, &dma_sett);
    }

    //对模拟信号数字化
    //AUDIO OUT  LEFT ------- AUX LEFT 0
    if (channel_test_arg->test_left_ch && channel_test_arg->test_left_ch_SNR)
    {
        dma_chan_setting_t dma_sett;
        ain_setting_t ain_sett;
        adc_setting_t adc_sett;
        clear_cache();

        ain_sett.op_gain.ain_op_gain = AIN_OP_0DB;
        ain_sett.ain_pa_mode = FALSE;
        ain_sett.ain_input_mode = AIN_INPUT_ONLY_L0; /*AIN_INPUT_ONLY_L;*/
        enable_ain(AIN_SOURCE_AUX, &ain_sett); //使能audio in

        adc_sett.sample_rate = 16;
        adc_sett.gain = 0; /*ADC_GAIN_0DB;*/
        enable_adc(AIN_SOURCE_AUX, &adc_sett);

        sys_reset_dma_chan (ADC_CHAN_DMA_CHAN);

        libc_memset(&dma_sett, 0x00, sizeof(dma_chan_setting_t));
        dma_sett.dst_mem_addr = (uint8 *) SOUND_DATA_ADDR;
        dma_sett.frame_len = (0x1000) / sizeof(short);
        dma_sett.irq_cbk = NULL;
        dma_sett.src_type = DMA_SRC_ADC;
        dma_sett.dst_type = DMA_DST_MEM;
        dma_sett.hf_ie = FALSE;
        dma_sett.tc_ie = FALSE;
        dma_sett.reload = TRUE;
        dma_sett.start = TRUE;
        sys_set_dma_chan(ADC_CHAN_DMA_CHAN, &dma_sett); //dma start

        sys_os_time_dly(30);

        sys_reset_dma_chan(ADC_CHAN_DMA_CHAN);

        dma_sett.reload = FALSE;
        sys_set_dma_chan(ADC_CHAN_DMA_CHAN, &dma_sett); //dma start
        sys_wait_dma_chan_tc(ADC_CHAN_DMA_CHAN, WAIT_DMA_TC_ALWAYS);

        sys_reset_dma_chan(ADC_CHAN_DMA_CHAN);
        disable_adc (AIN_SOURCE_AUX);
        disable_ain(AIN_SOURCE_AUX);

        result_0_L = thd_test((void*) SOUND_DATA_ADDR, channel_test_arg->left_ch_SNR_threadshold,
                channel_test_arg->left_ch_max_sig_point);

    }

    //AUDIO OUT RIGHT ------- AUX 0 RIGHT
    if (channel_test_arg->test_right_ch && channel_test_arg->test_right_ch_SNR)
    {
        dma_chan_setting_t dma_sett;
        ain_setting_t ain_sett;
        adc_setting_t adc_sett;

        clear_cache();
        ain_sett.op_gain.ain_op_gain = AIN_OP_0DB;
        ain_sett.ain_pa_mode = FALSE;
        ain_sett.ain_input_mode = AIN_INPUT_ONLY_R0; /*AIN_INPUT_ONLY_R;// right*/
        enable_ain(AIN_SOURCE_AUX, &ain_sett);

        adc_sett.sample_rate = 16;
        adc_sett.gain = 0; /*ADC_GAIN_0DB;*/
        enable_adc(AIN_SOURCE_AUX, &adc_sett);

        sys_reset_dma_chan (ADC_CHAN_DMA_CHAN);

        libc_memset(&dma_sett, 0x00, sizeof(dma_chan_setting_t));
        dma_sett.dst_mem_addr = (uint8 *) SOUND_DATA_ADDR;
        dma_sett.frame_len = (0x1000) / sizeof(short);
        dma_sett.irq_cbk = NULL;
        dma_sett.src_type = DMA_SRC_ADC;
        dma_sett.dst_type = DMA_DST_MEM;
        dma_sett.hf_ie = FALSE;
        dma_sett.tc_ie = FALSE;
        dma_sett.reload = TRUE;
        dma_sett.start = TRUE;
        sys_set_dma_chan(ADC_CHAN_DMA_CHAN, &dma_sett); //dma start

        sys_os_time_dly(30);

        sys_reset_dma_chan(ADC_CHAN_DMA_CHAN);

        dma_sett.reload = FALSE;
        sys_set_dma_chan(ADC_CHAN_DMA_CHAN, &dma_sett); //dma start
        sys_wait_dma_chan_tc(ADC_CHAN_DMA_CHAN, WAIT_DMA_TC_ALWAYS);

        sys_reset_dma_chan(ADC_CHAN_DMA_CHAN);
        disable_adc (AIN_SOURCE_AUX);
        disable_ain(AIN_SOURCE_AUX);

        result_0_R = thd_test((void*) SOUND_DATA_ADDR, channel_test_arg->right_ch_SNR_threadshold,
                channel_test_arg->right_ch_max_sig_point);

    }

    //AUDIO  OUT LEFT ------- AUX LEFT 1
    if (channel_test_arg->test_right_ch && channel_test_arg->test_right_ch_SNR)
    {
        dma_chan_setting_t dma_sett;
        ain_setting_t ain_sett;
        adc_setting_t adc_sett;

        clear_cache();
        ain_sett.op_gain.ain_op_gain = AIN_OP_0DB;
        ain_sett.ain_pa_mode = FALSE;
        ain_sett.ain_input_mode = AIN_INPUT_ONLY_L1; /*AIN_INPUT_ONLY_R;// right*/
        enable_ain(AIN_SOURCE_AUX, &ain_sett);

        adc_sett.sample_rate = 16;
        adc_sett.gain = 0; /*ADC_GAIN_0DB;*/
        enable_adc(AIN_SOURCE_AUX, &adc_sett);

        sys_reset_dma_chan (ADC_CHAN_DMA_CHAN);

        libc_memset(&dma_sett, 0x00, sizeof(dma_chan_setting_t));
        dma_sett.dst_mem_addr = (uint8 *) SOUND_DATA_ADDR;
        dma_sett.frame_len = (0x1000) / sizeof(short);
        dma_sett.irq_cbk = NULL;
        dma_sett.src_type = DMA_SRC_ADC;
        dma_sett.dst_type = DMA_DST_MEM;
        dma_sett.hf_ie = FALSE;
        dma_sett.tc_ie = FALSE;
        dma_sett.reload = TRUE;
        dma_sett.start = TRUE;
        sys_set_dma_chan(ADC_CHAN_DMA_CHAN, &dma_sett); //dma start

        sys_os_time_dly(30);

        sys_reset_dma_chan(ADC_CHAN_DMA_CHAN);

        dma_sett.reload = FALSE;
        sys_set_dma_chan(ADC_CHAN_DMA_CHAN, &dma_sett); //dma start
        sys_wait_dma_chan_tc(ADC_CHAN_DMA_CHAN, WAIT_DMA_TC_ALWAYS);

        sys_reset_dma_chan(ADC_CHAN_DMA_CHAN);
        disable_adc (AIN_SOURCE_AUX);
        disable_ain(AIN_SOURCE_AUX);

        result_1_L = thd_test((void*) SOUND_DATA_ADDR, channel_test_arg->left_ch_SNR_threadshold,
                channel_test_arg->left_ch_max_sig_point);

    }

    //AUDIO OUT RIGHT ------- AUX RIGHT 1
    if (channel_test_arg->test_right_ch && channel_test_arg->test_right_ch_SNR)
    {
        dma_chan_setting_t dma_sett;
        ain_setting_t ain_sett;
        adc_setting_t adc_sett;

        clear_cache();
        ain_sett.op_gain.ain_op_gain = AIN_OP_0DB;
        ain_sett.ain_pa_mode = FALSE;
        ain_sett.ain_input_mode = AIN_INPUT_ONLY_R1; /*AIN_INPUT_ONLY_R;// right*/
        enable_ain(AIN_SOURCE_AUX, &ain_sett);

        adc_sett.sample_rate = 16;
        adc_sett.gain = 0; /*ADC_GAIN_0DB;*/
        enable_adc(AIN_SOURCE_AUX, &adc_sett);

        sys_reset_dma_chan (ADC_CHAN_DMA_CHAN);

        libc_memset(&dma_sett, 0x00, sizeof(dma_chan_setting_t));
        dma_sett.dst_mem_addr = (uint8 *) SOUND_DATA_ADDR;
        dma_sett.frame_len = (0x1000) / sizeof(short);
        dma_sett.irq_cbk = NULL;
        dma_sett.src_type = DMA_SRC_ADC;
        dma_sett.dst_type = DMA_DST_MEM;
        dma_sett.hf_ie = FALSE;
        dma_sett.tc_ie = FALSE;
        dma_sett.reload = TRUE;
        dma_sett.start = TRUE;
        sys_set_dma_chan(ADC_CHAN_DMA_CHAN, &dma_sett); //dma start

        sys_os_time_dly(30);

        sys_reset_dma_chan(ADC_CHAN_DMA_CHAN);

        dma_sett.reload = FALSE;
        sys_set_dma_chan(ADC_CHAN_DMA_CHAN, &dma_sett); //dma start
        sys_wait_dma_chan_tc(ADC_CHAN_DMA_CHAN, WAIT_DMA_TC_ALWAYS);

        sys_reset_dma_chan(ADC_CHAN_DMA_CHAN);
        disable_adc (AIN_SOURCE_AUX);
        disable_ain(AIN_SOURCE_AUX);

        result_1_R = thd_test((void*) SOUND_DATA_ADDR, channel_test_arg->right_ch_SNR_threadshold,
                channel_test_arg->right_ch_max_sig_point);

    }

    libc_printf_info("RESULE:\nleft0 :%d right0 :%d left1 :%d right1 :%d\n", result_0_L, result_0_R, result_1_L,
            result_1_R);

    if (result_0_L && result_0_R && result_1_L && result_1_R)
    {
        result_Final = TRUE;
    }
    else
    {
        result_Final = FALSE;
    }
    act_test_report_channel_result(test_mode, TESTID_LINEIN_CH_TEST, result_Final, arg_buffer, log_buffer, cur_log_len);

    return result_Final;
}

