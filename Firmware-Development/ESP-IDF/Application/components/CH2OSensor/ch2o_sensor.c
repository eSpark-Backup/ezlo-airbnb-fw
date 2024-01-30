#include "ch2o_sensor.h"
#include "string.h"

static uint8_t ch2o_sensor_checksum(uint8_t array[],uint8_t length);
static void ezlopi_ch2o_upcall(uint8_t* buffer, s_ezlopi_uart_object_handle_t uart_object_handle, void* user_args);
static void ch2o_parse(uint8_t* buffer, ch2o_sensor_data_t* data);

static uint8_t ch2o_sensor_checksum(uint8_t array[],uint8_t length)	
{
    uint8_t sum = 0;

    for(int i = 1; i < length-1; i++)
    {
		  sum += array[i];
    }

    sum = (~sum) + 1;
    return sum;
}

static void ezlopi_ch2o_upcall(uint8_t* buffer, s_ezlopi_uart_object_handle_t uart_object_handle, void* user_args)
{
    ch2o_sensor_data_t* data = (ch2o_sensor_data_t*) user_args;
    for (size_t i = 0; i < MAXLENGTH; i++)
    {
        printf("0x%X ", buffer[i]);
    }
    printf("\n");
    ch2o_parse(buffer, data);
}

bool ch2o_sensor_init(ch2o_sensor_data_t* data)
{
    ezlopi_uart_init(CH2O_UART_BUAD_RATE, CH2O_TX_PIN, CH2O_RX_PIN, ezlopi_ch2o_upcall, data);
    return true;
}

static void ch2o_parse(uint8_t* buffer, ch2o_sensor_data_t* data)		//new data was recevied
{
    uint8_t sumNum = ch2o_sensor_checksum(buffer, MAXLENGTH);
    if( (buffer[0] == 0xFF) && (buffer[1] == 0x17) && (buffer[2] == 0x04) && (buffer[MAXLENGTH-1] == sumNum) )	//head bit and sum are all right
    {
      float ppb = (unsigned int)buffer[4]<<8 | buffer[5];		// bit 4: ppm high 8-bit; bit 5: ppm low 8-bit
      data->ppm = ppb / 1000.0;	//1ppb = 1000ppm
      data->available = true;
    }
}