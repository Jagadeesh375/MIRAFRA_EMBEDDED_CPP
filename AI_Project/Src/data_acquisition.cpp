/*
 * data_aquisition.cpp
 *
 *  Created on: Apr 4, 2026
 *      Author: Jeena Sijo
 */
#include "data_acquisition.h"
#include "ir_sensor.h"
#include "delay.h"
#include "config.h"

void DataAcquisition::collect(uint8_t *buffer)
{
    for(int i=0;i<SAMPLE_SIZE;i++)
    {
        buffer[i] = IRSensor::read();
        Delay::ms(10);
    }
}



