/*******************************************************************************
 System Interrupts File

  File Name:
    system_interrupt.c

  Summary:
    Raw ISR definitions.

  Description:
    This file contains a definitions of the raw ISRs required to support the
    interrupt sub-system.

  Summary:
    This file contains source code for the interrupt vector functions in the
    system.

  Description:
    This file contains source code for the interrupt vector functions in the
    system.  It implements the system and part specific vector "stub" functions
    from which the individual "Tasks" functions are called for any modules
    executing interrupt-driven in the MPLAB Harmony system.

  Remarks:
    This file requires access to the systemObjects global data structure that
    contains the object handles to all MPLAB Harmony module objects executing
    interrupt-driven in the system.  These handles are passed into the individual
    module "Tasks" functions to identify the instance of the module to maintain.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2011-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "system/common/sys_common.h"
#include "joyadc.h"
#include "pathpoint.h"
#include "messagehandler.h"
#include "serializer.h"
#include "system_definitions.h"
#include "msgqueue.h"
#include "adc_queue.h"
#include "serializer_queue.h"


JOYADC_DATA joystickData;
// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector Functions
// *****************************************************************************
// *****************************************************************************
void IntHandlerDrvUsartInstance0(void)
{
   // DRV_USART_TasksTransmit(sysObj.drvUsart0);
   // DRV_USART_TasksError(sysObj.drvUsart0);
   // DRV_USART_TasksReceive(sysObj.drvUsart0);
    
    BaseType_t uartHPTW = pdFALSE; 
    
    if(SYS_INT_SourceStatusGet(INT_SOURCE_USART_1_TRANSMIT)){
        if(!PLIB_USART_TransmitterBufferIsFull(USART_ID_1)){
             if(!isTransmitQueueEmpty()){
				char c;
				c = receiveCharMessageFromTransmitQueueFromISR(&uartHPTW);
                PLIB_USART_TransmitterByteSend(USART_ID_1, c);
             
             }
             else{
				SYS_INT_SourceDisable(INT_SOURCE_USART_1_TRANSMIT);
				//dbgOutputLoc(DISABLE_SOURCE_UART_TX);
			}      
        }
    }
   /*********************************************************************************/
    
    if(SYS_INT_SourceStatusGet(INT_SOURCE_USART_1_RECEIVE)){
         if(PLIB_USART_ReceiverDataIsAvailable(USART_ID_1)){
             // write to rx queue from isr
            unsigned char c = PLIB_USART_ReceiverByteReceive(USART_ID_1);
            sendMessegeToSerialQueue(c, &uartHPTW);
         }
         SYS_INT_SourceStatusClear(INT_SOURCE_USART_1_RECEIVE);
    }
    if(SYS_INT_SourceStatusGet(INT_SOURCE_USART_1_ERROR)){
		badRoutine(UART_ERROR);
	}
    portEND_SWITCHING_ISR(uartHPTW);
}

void IntHandlerDrvTmrInstance0(void)
{
   static short int counter = 0;
    //static short int test_counter = 0;
    BaseType_t tmr0_HPTW;
    
    if(counter > 10){
        FULL_POINT_MSG msg;
        msg.msg_type = GET;
        msg.task_name = PATH;
        
        //dbgOutputLoc(BEFORE_POST_TO_MSGQ);
       // sendMessegeToMsgHandQueueFromISR(msg, &tmr0_HPTW);
       sendMessegeToMsgHandQueueFromISR(msg, &tmr0_HPTW);
              
        //dbgOutputLoc(AFTER_POST_MSGQ);
        counter = 0;
    }
   PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_2);
   portEND_SWITCHING_ISR(tmr0_HPTW);
   ++counter;
}

 
 /*************************************************************************************/
void IntHandlerDrvAdc(void)
{
    int i;
    BaseType_t xHigherPriorityTaskWoken;
       /* Task is not woken at start of the ISR */
    xHigherPriorityTaskWoken = pdFALSE; 
    
    for(i=0; i < ADC_NUM_SAMPLE_PER_AVERAGE;  i+=2)
    {
         joystickData.ch1 = DRV_ADC_SamplesRead(i);
         joystickData.ch2 = DRV_ADC_SamplesRead(i+1);
    }
    
    joystickData.udValue = ((float)joystickData.ch1/ 8.0) + 0.5;
    joystickData.lrValue = ((float)joystickData.ch2/ 8.0) + 0.5;
    
    xHigherPriorityTaskWoken = sendMessegeToQueueFromADCISR(joystickData.udValue, joystickData.lrValue);
   
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_ADC_1);
    
    PLIB_ADC_SampleAutoStartEnable(ADC_ID_1);
    
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}
 


void IntHandlerDrvTmrInstance1(void)
{
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_4);
}
 /*******************************************************************************
 End of File
*/
