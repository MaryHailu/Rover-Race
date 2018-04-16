/* 
 * File:   build_request.h
 * Author: Mary Hailu
 *
 * Created on April 1, 2018, 3:59 PM
 */

#ifndef BUILD_REQUEST_H
#define	BUILD_REQUEST_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <stdio.h>
#include "messagehandler.h"
#include "msgqueue.h"
#include "message.h"
    
    

void buildRequest_A(FULL_MSG *data, FULL_REQ *req);
void buildRequest_P(FULL_POINT_MSG *path, FULL_REQ *req);


#ifdef	__cplusplus
}
#endif

#endif	/* BUILD_REQUEST_H */

