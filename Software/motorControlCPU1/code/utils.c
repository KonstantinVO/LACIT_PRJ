#include "stdint.h"
#include "string.h"
#include "stdbool.h"
#include "stdlib.h"

#include "adcConfig.h"
#include "utils.h"

#include "driverlib.h"
#include "device.h"

struct kalman_filter k_f[KF_NUM_CH];

//specify the byte number in the field payload
float glueFloat(union conv4byte *p, struct uart_receive *uart, uint16_t start_pos)
{
    p->byte.a = uart->work_buffer[payload + start_pos];
    p->byte.b = uart->work_buffer[payload + start_pos + 1];
    p->byte.c = uart->work_buffer[payload + start_pos + 2];
    p->byte.d = uart->work_buffer[payload + start_pos + 3];

    return p->all;
}

int glueInt(union conv4byte *p, struct uart_receive *uart, uint16_t start_pos)
{
    p->byte.a = uart->work_buffer[payload + start_pos];
    p->byte.b = uart->work_buffer[payload + start_pos + 1];
    p->byte.c = uart->work_buffer[payload + start_pos + 2];
    p->byte.d = uart->work_buffer[payload + start_pos + 3];

    return p->all;
}


int32_t filterAverage(struct filter_run_average *f, int32_t val)
{
  f->sum =  f->sum + (val - f->data[f->count]);
  f->data[f->count] = val;
  f->count = (f->count + 1) % AVG_FILTER_SIZE;
  return f->sum / AVG_FILTER_SIZE;
}

#define SPEED_MAX_LIMIT   _IQ(1.0)
#define H_MAX_DELTA_SPEED _IQ(0.008)



_iq filterCapSpeed(_iq speed)
{
   static _iq speed_temp = 0;
   static _iq delta_sp = 0;

   delta_sp = labs(speed_temp - speed);

   if(delta_sp >= H_MAX_DELTA_SPEED) speed = speed_temp;
   else speed_temp = speed;

    return (speed+speed_temp)>>1;
}


void initKalmanFilter(struct kalman_filter *f)
{
    f->kalman_adc_old = 0;
    f->Q = 0.0003;
    f->R = 0.1;
    f->Kg = 0;
    f->P = 1;
}

void initStrKalmanFilter(void)
{
    for(uint16_t i = 0; i <KF_NUM_CH; i++)
    {
        initKalmanFilter(&k_f[i]);
    }
}

uint64_t kalmanFilter(struct kalman_filter *f, uint16_t ADC_Value)
{
    f->NowData = ADC_Value;
    f->LastData = f->kalman_adc_old;
    f->P = f->P1 + f->Q;
    f->Kg = f->P / ( f->P + f->R );
    f->kalman_adc = f->LastData + f->Kg * ( f->NowData - f->kalman_adc_old );
    f->P1 = ( 1 - f->Kg ) * f->P;
    f->P = f->P1;
    f->kalman_adc_old = f->kalman_adc;
    return (uint64_t)( f->kalman_adc );
}






