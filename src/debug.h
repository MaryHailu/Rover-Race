/* 
 * File:   debug.h
 * Author: Mary Hailu
 *
 * Created on April 1, 2018, 3:13 PM
 */

#ifndef DEBUG_H
#define	DEBUG_H

typedef enum
{
    //app.c
    APP_INIT_BEGIN=0,       //0
    APP_INIT_END,           //1
    APP_IN,                 //2
    APP_OUT,                //3
    APP_WHILE,              //4
    //messagehandler.c
    MSGHAND_INIT_BEGIN,     //5
    MSGHAND_INIT_END,       //6
    MSGHAND_IN,             //7
    MSGHAND_OUT,            //8
    MSGHAND_WHILE,          //9
    //serializer.c
    SERIAL_INIT_BEGIN,      //10
    SERIAL_INIT_END,        //11
    SERIAL_IN,              //12
    SERIAL_OUT,             //13
    SERIAL_WHILE,           //14
    //uart isr
    UART_ISR_START,         //15
    UART_ISR_END,           //16
    SET_SOURCE_UART_TX,     //17
    DISABLE_SOURCE_UART_TX, //18
    UART_TX_SENT,           //19
    UART_TX_FLAG_IS_UP,     //20
    UART_TX_NOT_FULL,       //21
	UART_RX_FLAG_IS_UP,		//22
    UART_ERROR,				//23
    //transmit queue
	BEFORE_GET_TXQ,         //24
    AFTER_GET_TXQ,          //25
    BEFORE_POST_TO_TXQ,     //26
    AFTER_POST_TO_TXQ,      //27
    TXQ_FULL,               //28
    //serializer state
    HTTP_GET_RESP_GOOD,     //29
	HTTP_POST_RESP_GOOD,    //30
	HTTP_RESP_BAD,          //31
    BEFORE_GET_SERIAL_Q,    //32
    AFTER_GET_SERIAL_Q,     //33
    AFTER_SERIAL_STATE,     //34
    SERIAL_FINISH,          //35
    FINISHED_PAYLOAD,       //36
    GOOD_PAYLOAD,           //37
    BAD_PAYLOAD,            //38
	//location.c
	LOC_INIT_BEGIN,         //39
    LOC_INIT_END,           //40
    LOC_IN,                 //41
    LOC_OUT,                //42
    LOC_WHILE,              //43
	BEFORE_GET_LOCQ,        //44
	AFTER_GET_LOCQ,         //45
	BEFORE_SENT_TO_LOCQ,    //46
	AFTER_SENT_TO_LOCQ,     //47
	IN_DIST,                //48
	OUT_DIST,               //49
	DIST_TASK1,             //50
	START_BUILD_DATA,       //51
	END_BUILD_DATA,         //52
	//msg queue
	BEFORE_GET_MSGQ,        //53
	AFTER_GET_MSGQ,         //54
	BEFORE_POST_TO_MSGQ,    //55
	AFTER_POST_MSGQ,        //56
	//timer 0
	TMR0_ISR_START,         //57
	TMR0_ISR_END,           //58
     
    IN_SERIAL_Q,            //59
    OUT_SERIAL_Q,            //60
            IN_HEADER,
            LENGTH_ERROR
} DLOC_STATE;


typedef struct
{
    unsigned char stringValue[10];
 
} VALUE_TO_DEBUG;


unsigned int dbgOutputLoc(unsigned int outVal); 
void dbgUARTVal(unsigned int valueToUart);
void dbgUARTcharVal(unsigned char* valuecharValue);
void  badRoutine(unsigned int badState);   
unsigned int dbgOutputRX(char outVal);


#endif	/* DEBUG_H */

