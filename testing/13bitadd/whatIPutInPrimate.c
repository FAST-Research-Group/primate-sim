#include "primate-hardware.hpp"

struct output_t
{
    int a;
    int b;
};

typedef struct
{
    int value1 : 13;
    int value2 : 13;
} elements_input_t;

#pragma primate blue thirteenbitadd thirteenbitadd 1 1
int processthirteenbitadd(elements_input_t test)
{
    return (test.value1 + test.value2) & ((1 << 13) - 1);
}

void primate_main()
{
    elements_input_t experiment = PRIMATE::input<elements_input_t>();
    int result = processthirteenbitadd(experiment);
    PRIMATE::output<output_t>({result, 0});
}

/* Is this valid primate code? */
