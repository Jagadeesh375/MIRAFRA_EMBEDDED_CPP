#include "model.h"

const uint8_t train_data[TRAIN_SAMPLES][SAMPLE_SIZE] =
{
    // No object (all HIGH)
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},

    // Object appears briefly
    {1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1},

    // Object continuously present
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},

    // Object flickering (movement)
    {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0}
};

const uint8_t labels[TRAIN_SAMPLES] =
{
    0,  // No object
    1,  // Object detected briefly
    2,  // Object continuously present
    3   // Object moving/flickering
};
