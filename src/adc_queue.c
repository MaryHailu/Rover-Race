#include "adc_queue.h"

static QueueHandle_t xadcQueue = NULL;

void adcTransmitQueue(){
  xadcQueue = xQueueCreate(1, sizeof(JDATA));  
}

//----------------------------------------------------------------------------------
//BaseType_t sendMessegeToQueueFromADCISR(unsigned int messegeValue){
BaseType_t sendMessegeToQueueFromADCISR(unsigned int UD, unsigned int LR){
  JDATA msg;
  msg.adcvalue_UD = UD;
  msg.adcvalue_LR = LR;
  
  BaseType_t xStatus, xHigherPriorityTaskWoken = pdFALSE;
  
  //xStatus = xQueueSendToBackFromISR( xadcQueue, &messegeValue, &xHigherPriorityTaskWoken );
  xStatus = xQueueSendToBackFromISR( xadcQueue, &msg, &xHigherPriorityTaskWoken );
  
   /* Data was not successfully send to the queue. *
   if( xStatus != pdPASS ){
     // LED1On();  
    }
   else{
     //LED1Off();
   }*/
   
   return xHigherPriorityTaskWoken;
}

JDATA receiveMessageFromADCQueue(){
    
    JDATA data;
    xQueueReceive( xadcQueue, &data, portMAX_DELAY);
    return data;
}

// Receive From Message Queue working ---------------------------------------------------------------------------
unsigned int receiveMessageFromQueue(){
    
    
    unsigned int xRecivedFromQueue;
    
    xQueueReceive( xadcQueue, &xRecivedFromQueue, portMAX_DELAY);
    
    return xRecivedFromQueue;
   
   /* if( xadcQueue != 0 )
    {
        if( (xQueueReceive( xadcQueue, &xRecivedFromQueue, portMAX_DELAY)) )
        {
            return xRecivedFromQueue;
           // convertToChar(xRecivedFromQueue); 
        }
    }
    else{
        // should be led to debug 
       //convertToChar(1);  
     }*/
    //return xRecivedFromQueue;
    
}
