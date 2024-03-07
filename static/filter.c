#include "filter.h"

#define ALPHA 0.0198f

float lpf(filter_data_t *data, float in)
{
    data->out = in + (1 - ALPHA) * data->out;
    return data->out;
}