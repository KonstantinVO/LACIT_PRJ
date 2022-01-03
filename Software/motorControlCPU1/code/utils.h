

#ifndef CODE_UTILS_H_
#define CODE_UTILS_H_

#include "stdint.h"
#include "mavlink.h"
#include "crcControl.h"
#include "sciConfig.h"
#include "IQmathLib.h"


#define AVG_FILTER_SIZE  5
#define KF_NUM_CH        4

struct filter_run_average
{
    int32_t count;
    int32_t data[AVG_FILTER_SIZE];
    int64_t sum;
};

union conv4byte
{
    float all;
    struct
    {
        uint16_t a : 8;
        uint16_t b : 8;
        uint16_t c : 8;
        uint16_t d : 8;
    }byte;
};

struct kalman_filter
{
    float LastData;
    float NowData;
    double kalman_adc;
    float kalman_adc_old;
    float P1;
    float Q;
    float R;
    float Kg;
    float P;
};

float glueFloat(union conv4byte *p, struct uart_receive *uart, uint16_t start_pos);
int glueInt(union conv4byte * p, struct uart_receive *uart, uint16_t start_pos);
int32_t filterAverage(struct filter_run_average *f, int32_t val);
uint64_t kalmanFilter(struct kalman_filter *f, uint16_t ADC_Value);
void initStrKalmanFilter(void);
_iq filterCapSpeed(_iq speed);

#endif /* CODE_UTILS_H_ */
