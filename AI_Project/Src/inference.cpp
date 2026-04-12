/*
 * inference.cpp
 *
 *  Created on: Apr 4, 2026
 *      Author: Jeena Sijo
 */

#include "inference.h"
#include "model.h"
#include "config.h"

static int abs_val(int x)
{
    return (x < 0) ? -x : x;
}

int Inference::predict(uint8_t *input)
{
    int min_dist = 10000;
    int best_label = 0;

    for(int i=0;i<TRAIN_SAMPLES;i++)
    {
        int dist = 0;

        for(int j=0;j<SAMPLE_SIZE;j++)
        {
            dist += abs_val(input[j] - train_data[i][j]);
        }

        if(dist < min_dist)
        {
            min_dist = dist;
            best_label = labels[i];
        }
    }

    return best_label;
}


