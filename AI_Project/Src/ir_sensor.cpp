/*
 * ir_sensor.cpp
 *
 *  Created on: Apr 4, 2026
 *      Author: Jeena Sijo
 */

#include "ir_sensor.h"
#include "gpio_reg.h"

uint8_t IRSensor::read()
{
    return GPIO::readPA0();
}


