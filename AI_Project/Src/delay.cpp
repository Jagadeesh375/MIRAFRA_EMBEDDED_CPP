/*
 * delay.cpp
 *
 *  Created on: Apr 4, 2026
 *      Author: Jeena Sijo
 */

#include "delay.h"

void Delay::ms(int t)
{
    for(int i=0;i<t*5000;i++)
        __asm("NOP");
}


