#include "messageHandlerQueue.h"

static QueueHandle_t msgadcHandQueue = NULL;
//static QueueHandle_t msgpathHandQueue;
static QueueHandle_t msgHandQueue = NULL;

void initMsgHandQueue() {
    
    msgadcHandQueue = xQueueCreate(8, sizeof(FULL_MSG));
   // msgpathHandQueue = xQueueCreate(8, sizeof(FULL_POINT_MSG));
    msgHandQueue = xQueueCreate(16, sizeof(FULL_POINT_MSG));
}

void sendMessegeToMsgHandQueueFromISR(FULL_POINT_MSG msg, BaseType_t *xHigherPriorityTaskWoken)
{ 
    xQueueSendToBackFromISR(msgHandQueue, &msg, xHigherPriorityTaskWoken );
}

BaseType_t sendMessageToMsgHandQueueFromPathTask(FULL_POINT_MSG msg){
    BaseType_t success;
    FULL_POINT_MSG message = msg;

    success = xQueueSendToBack( msgHandQueue, &message, portMAX_DELAY);
    return success;
}


FULL_POINT_MSG receiveMessageFromMsgHandQueue(){
    FULL_POINT_MSG message;
    if (msgHandQueue != 0) 
    {
        xQueueReceive( msgHandQueue, &message, portMAX_DELAY);
    }
    return message; 
}

BaseType_t sendMessageToMsgHandQueueFromAdcTask(FULL_MSG msg){
    BaseType_t success;
    FULL_MSG message = msg;

    success = xQueueSendToBack( msgadcHandQueue, &message, portMAX_DELAY);
    return success;
}
/**
BaseType_t sendMessageToMsgHandQueueFromPathTask(FULL_POINT_MSG msg){
    BaseType_t success;
    FULL_POINT_MSG message = msg;

    success = xQueueSendToBack( msgpathHandQueue, &message, portMAX_DELAY);
    return success;
}*/
 
FULL_MSG receiveMessageFromMsgadcHandQueue(){
    FULL_MSG message;
    xQueueReceive( msgadcHandQueue, &message, portMAX_DELAY);
    return message; 
}
/**
FULL_POINT_MSG receiveMessageFromMsgpathHandQueue(){
    FULL_POINT_MSG message;
    xQueueReceive( msgpathHandQueue, &message, portMAX_DELAY);
    return message; 
}*/
