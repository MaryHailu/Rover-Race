/* 
 * File:   serializer_queue.h
 * Author: Mary Hailu
 *
 * Created on April 14, 2018, 2:12 PM
 */

#ifndef SERIALIZER_QUEUE_H
#define	SERIALIZER_QUEUE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "queue.h"
#include "message.h"

void initSerialQueue();
BaseType_t sendMessegeToSerialQueue(unsigned char c, BaseType_t *xHigherPriorityTaskWoken);
unsigned char receiveMessageFromSerialQueue();
//USED FOR DEBUGGING ONLY
BaseType_t sendMessegeToSerialQueueDEBUG(unsigned char c);

#ifdef	__cplusplus
}
#endif

#endif	/* SERIALIZER_QUEUE_H */

