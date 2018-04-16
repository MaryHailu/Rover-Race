/* 
 * File:   path_queue.h
 * Author: Mary Hailu
 *
 * Created on April 2, 2018, 1:08 AM
 */

#ifndef PATH_QUEUE_H
#define	PATH_QUEUE_H
#include <stdbool.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "message.h"

 typedef struct {
        unsigned int dist;
        char location[5];
    }LOC_DIST;
    
    typedef struct {
        short int x;
        short int y;
        short int angle;
    }LOC_POS;
    
    typedef struct{
        bool isPos;
        LOC_DIST distance;
        LOC_POS position;
    } PATH_MSG;

void initpathQueue();
BaseType_t sendMessageToPathQueue(PATH_MSG data);
PATH_MSG receiveMessageFromPathQueue();
unsigned int isLocationQueueEmpty();




#endif	/* PATH_QUEUE_H */

