#include <stdio.h>
#include "debug.h"
#include "system_config.h"
#include "system_definitions.h"

/* Working UART debuger to convert integer and print value */
void dbgUARTVal(unsigned int valueToUart){
   
    VALUE_TO_DEBUG debugData; 
    
    sprintf(debugData.stringValue, "%d", valueToUart);
    
    int i;
     // make sure the transmit buffer is not full before trying to write byte 
    if(!(DRV_USART_TRANSFER_STATUS_TRANSMIT_FULL & DRV_USART0_TransferStatus()) )
    {   
         for(i = 0; debugData.stringValue[i] != '\0'; ++i){ 
              DRV_USART0_WriteByte(debugData.stringValue[i]);
          }     
    }   
}

/* Working Print string of up to 9 characters*/
void dbgUARTcharVal(unsigned char* valuecharValue){
    
    VALUE_TO_DEBUG debugData; 
    
     strncpy(debugData.stringValue, valuecharValue, 9); 
     int i;
     
     if(!(DRV_USART_TRANSFER_STATUS_TRANSMIT_FULL & DRV_USART0_TransferStatus()) )
     {
         for(i = 0; debugData.stringValue[i] != '\0'; ++i){ 
              DRV_USART0_WriteByte(debugData.stringValue[i]);
          }  
     }
}

/*unsigned int dbgOutputRX(char outVal){
    if (outVal > 127 )
    {
        return 1;
    } 	
    else 
    {
		unsigned int mask = PORTE;
		mask =  0x80 & ~(mask);
		PORTE = mask | (unsigned int)outVal;
    }
    return 0;
}*/

unsigned int dbgOutputLoc(unsigned int outVal) {
    // Used to find out when and where our ISR 
    //  is being run  

    // be careful using static
    // DLOC_STATE dloc = outVal;
    if (outVal > 127 )
    {
        return 1;
    } 	
    else 
    {
		unsigned int mask = PORTE;
		mask =  0x80 & ~(mask);
		PORTE = mask | outVal;
    }   
    return 0;
}

void badRoutine(unsigned int badState) {

  // Halt everything and continue to output random value
  // that indicates that this routine is running.
  // can be done with dbgOutputLoc or blinking an LED.
  // kill all interrupts
  //  LED1On();
  SYS_INT_Disable();
  while (1) {
	 dbgOutputLoc(badState);
  } 
}
