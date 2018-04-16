#include "path_queue.h"

static QueueHandle_t pathQueue = NULL;

void initpathQueue(){
    pathQueue = xQueueCreate(8, sizeof(PATH_MSG));
}

BaseType_t sendMessageToPathQueue(PATH_MSG data){
    BaseType_t success;
	success = xQueueSendToBack(pathQueue, &data, portMAX_DELAY);
	return success;	
}

PATH_MSG receiveMessageFromPathQueue(){
    PATH_MSG data;
    xQueueReceive(pathQueue, &data, portMAX_DELAY);
    return data;
}

unsigned int isLocationQueueEmpty(){
    unsigned int test = 0;
    if(xQueueIsQueueEmptyFromISR(pathQueue) != pdFALSE){
        test = 1;
    } else {
        test = 0;
    }
    return test;
}