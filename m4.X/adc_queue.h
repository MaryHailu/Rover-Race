/* 
 * File:   adc_queue.h
 * Author: Mary Hailu
 *
 * Created on April 4, 2018, 9:07 AM
 */

#ifndef ADC_QUEUE_H
#define	ADC_QUEUE_H

 
#include "FreeRTOS.h"
#include "queue.h"
#include "message.h"
    

void adcTransmitQueue();
BaseType_t sendMessegeToQueueFromADCISR(unsigned int UD, unsigned int LR);
unsigned int receiveMessageFromQueue();
JDATA receiveMessageFromADCQueue();


#endif	/* ADC_QUEUE_H */

