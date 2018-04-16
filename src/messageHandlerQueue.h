/* 
 * File:   messageHandlerQueue.h
 * Author: Mary Hailu
 *
 * Created on April 1, 2018, 5:06 PM
 */

#ifndef MESSAGEHANDLERQUEUE_H
#define	MESSAGEHANDLERQUEUE_H


#include "FreeRTOS.h"
#include "queue.h"
#include "message.h"


void initMsgHandQueue();

void sendMessegeToMsgHandQueueFromISR(FULL_POINT_MSG msg, BaseType_t *xHigherPriorityTaskWoken);

BaseType_t sendMessageToMsgHandQueueFromAdcTask(FULL_MSG msg); 
BaseType_t sendMessageToMsgHandQueueFromPathTask(FULL_POINT_MSG msg); 

FULL_MSG receiveMessageFromMsgadcHandQueue();
FULL_POINT_MSG receiveMessageFromMsgpathHandQueue();
FULL_POINT_MSG receiveMessageFromMsgHandQueue();

BaseType_t sendMessageToMsgHandQueueFromPathTask(FULL_POINT_MSG msg);


#endif	/* MESSAGEHANDLERQUEUE_H */

