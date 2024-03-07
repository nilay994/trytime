#include <stdio.h>
#include "foo1.h"
#include "filter.h"

static filter_data_t filt;

void foo1()
{
    printf("foo1 called\n");
    float out = lpf(&filt, 1.0f);
    printf("out (foo1): %f\n", out);
}