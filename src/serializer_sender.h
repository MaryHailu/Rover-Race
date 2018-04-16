/* 
 * File:   serializer_sender.h
 * Author: Mary Hailu
 *
 * Created on April 14, 2018, 3:43 PM
 */

#ifndef SERIALIZER_SENDER_H
#define	SERIALIZER_SENDER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "message.h"
#include "debug.h"
#include "messageHandlerQueue.h"
#include "path_queue.h"
 
void distribute(FULL_POINT_MSG msg);
void distributeError();


#ifdef	__cplusplus
}
#endif

#endif	/* SERIALIZER_SENDER_H */

