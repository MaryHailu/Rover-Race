#include "msgqueue.h"

static QueueHandle_t transmitQueue = NULL;


void initTransmitQueue() {
	transmitQueue = xQueueCreate(16, sizeof(unsigned char));
}

BaseType_t sendSingleCharMessageToTrasmitQueue(unsigned char a){
    
	BaseType_t success;
	unsigned char x = a;
	success = xQueueSendToBack(transmitQueue, &x, portMAX_DELAY);
	return success;	
}

unsigned char receiveCharMessageFromTransmitQueueFromISR(BaseType_t *xHigherPriorityTaskWoken){
    
	unsigned char c;
	xQueueReceiveFromISR(transmitQueue, &c, xHigherPriorityTaskWoken);
	return c;
}

unsigned int isTransmitQueueEmpty(){
    
    unsigned int test = 0;
    if(xQueueIsQueueEmptyFromISR(transmitQueue) != pdFALSE){
        test = 1;
    } else {
        test = 0;
    }
    return test; 
}

//USED FOR DEBUGGING ONLY
/**
BaseType_t sendMessegeToSerialQueueDEBUG(unsigned char c)
{
    BaseType_t xHigherPriorityTaskWoken;
       /* Task is not woken at start of the ISR *
    xHigherPriorityTaskWoken = pdFALSE; 

	xHigherPriorityTaskWoken = xQueueSendToBack(serialQueue, &c, portMAX_DELAY);

    return xHigherPriorityTaskWoken;
}*/
/**
BaseType_t sendMessegeToSerialQueue(unsigned char c)
{
    BaseType_t xHigherPriorityTaskWoken;
       /* Task is not woken at start of the ISR *
    xHigherPriorityTaskWoken = pdFALSE; 
    
    xQueueSendToBackFromISR(serialQueue, &c, &xHigherPriorityTaskWoken );

    return xHigherPriorityTaskWoken;
}*/

/**
unsigned char receiveMessageFromSerialQueue(){
    
    unsigned char c;
    if (serialQueue != 0)
    {
        xQueueReceive( serialQueue, &c, portMAX_DELAY);
    }
    return c; 
}*/

