// refer: https://www.youtube.com/watch?v=f4ekifyijIg

#include <stdio.h>

void main()
{
    float y;
    int i = (1 << 24) - 1;

    y = i;
    for (int j = 0; j < 5; j++) {
        printf("%d %f\n", i, y);
        y = y + 1.0f;
        i = i + 1;
    }

}