#include <stdio.h>
#include "foo2.h"
#include "filter.h"

static filter_data_t filt;

void foo2()
{
    printf("foo2 called\n");
    float out = lpf(&filt, 2.0f);
    printf("out (foo2): %f\n", out);
}