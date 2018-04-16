/* 
 * File:   msgqueue.h
 * Author: Mary Hailu
 *
 * Created on April 1, 2018, 3:17 PM
 */

#ifndef MSGQUEUE_H
#define	MSGQUEUE_H
  
#include "FreeRTOS.h"
#include "queue.h"
    

void initTransmitQueue();
BaseType_t sendSingleCharMessageToTrasmitQueue(unsigned char a);
unsigned char receiveCharMessageFromTransmitQueueFromISR(BaseType_t *xHigherPriorityTaskWoken);
unsigned int isTransmitQueueEmpty();
//BaseType_t sendMessegeToSerialQueueDEBUG(unsigned char c);
//BaseType_t sendMessegeToSerialQueue(unsigned char c);
//unsigned char receiveMessageFromSerialQueue();



#endif	/* MSGQUEUE_H */

