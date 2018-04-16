#include "serializer_queue.h"

static QueueHandle_t serialQueue = NULL; 

void initSerialQueue() {
    // Create Queue able to hold MAX 16 items of size ()
    serialQueue = xQueueCreate(8, sizeof(unsigned char));
}
/**
BaseType_t sendMessegeToSerialQueueDEBUG(unsigned char c){
    
    BaseType_t xHigherPriorityTaskWoken;
       /* Task is not woken at start of the ISR *
    xHigherPriorityTaskWoken = pdFALSE; 

	xHigherPriorityTaskWoken = xQueueSendToBack(serialQueue, &c, portMAX_DELAY);

    return xHigherPriorityTaskWoken; 
}*/

BaseType_t sendMessegeToSerialQueue(unsigned char c, BaseType_t *xHigherPriorityTaskWoken)
{   
    return xQueueSendToBackFromISR(serialQueue, &c, xHigherPriorityTaskWoken );  
}

unsigned char receiveMessageFromSerialQueue(){
    unsigned char c;
    if (serialQueue != 0)
    {
        xQueueReceive( serialQueue, &c, portMAX_DELAY);
     }
    return c; 
}
