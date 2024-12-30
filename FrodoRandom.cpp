#include "FrodoRandom.h"
#include "parameters.h"

int frodo_sample(uint16_t r)
{
    int t = r >> 1;
    int e = 0;
    for (int z = 0; z < support_s; z++)
    {
        if (t > T_x[z])
        {
            e++;
        }
    }
    if (r & 1)
    {
        e = -e;
    }
    return e;
}

void frodo_sampleMatrix(int16_t *E, uint16_t *seed_E, int n1, int n2, int c)
{

}
