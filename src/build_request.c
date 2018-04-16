#include "build_request.h"
#include "message.h"
#include "debug.h"

static unsigned int checksumBuild(char *p, size_t size) {
	int i;
	unsigned int cSum = 0;
	for (i = 0; i < size; ++i) {
		cSum ^= *p;
		++p;
	}
	return cSum;
}

static void adc_put_request(FULL_MSG *data, FULL_REQ *req){
    //switch (data->task_name || path->task_name) {
    switch (data->task_name) {
 
        case ADC: 
        {
            char payload[150];
            char header[150];
            //sprintf(payload, "{\"updown\":%d}", data->msg_adc.adcvalue_UD, );
            sprintf(payload, "{\"updown\":%d,\"LeftRight\":%d}", data->msg_adc.adcvalue_UD, data->msg_adc.adcvalue_LR);
            unsigned int len = strlen(payload);
            //sprintf(header, "PUT /adcData/%d/%d/%d/%d/1 HTTP/1.1\r\nContent-Length: %d\r\nHost: 192.168.4.2:80\r\nContent-Type: application/json\r\n\r\n", data->seqNum, data->error, len, checksumBuild(payload, len), len);
            sprintf(header, "PUT /adcData/%d/%d/%d/%d/0 HTTP/1.1\r\nContent-Length: %d\r\nHost: 192.168.4.2:80\r\nContent-Type: application/json\r\n\r\n", data->seqNum, data->error, len, checksumBuild(payload, len), len);
            //sprintf(header, "PUT /adcData/%d/%d/%d/%d/1 HTTP/1.1\r\nContent-Length: %d\r\nHost: 192.168.3.1:80\r\nContent-Type: application/json\r\n\r\n", data->seqNum, data->error, len, checksumBuild(payload, len), len);
            sprintf(req->request, "%s%s", header, payload);
            req->size = strlen(req->request);
            break;
        }
        case ERROR_A:
        {
            break;
        }
       
    }
}

static void path_put_request(FULL_POINT_MSG *path, FULL_REQ *req){
    switch (path->task_name) {
        
        case PATH:
        {

            char payload[150];
            char header[150];
            sprintf(payload, "{\"x\":%d,\"y\":%d}", path->msg_point.x_coord, path->msg_point.y_coord);
            unsigned int len = strlen(payload);
            sprintf(header, "PUT /pointData/%d/%d/%d/%d/0 HTTP/1.1\r\nContent-Length: %d\r\nHost: 192.168.4.2:80\r\nContent-Type: application/json\r\n\r\n", path->seqNum, path->error, len, checksumBuild(payload, len), len);
            //sprintf(header, "PUT /pointData/%d/%d/%d/%d/0 HTTP/1.1\r\nContent-Length: %d\r\nHost: 192.168.3.1:80\r\nContent-Type: application/json\r\n\r\n", path->seqNum, path->error, len, checksumBuild(payload, len), len);
            sprintf(req->request, "%s%s", header, payload);
            req->size = strlen(req->request);
            break;
        }
        case ERROR_P:
        {
            break;
        }
        
    }
}

static void get_request_A(FULL_MSG *data, FULL_REQ *req) {
	
    switch (data->task_name) {
        
        case ADC:
        {
          sprintf(req->request, "GET /adcData/%d/%d/0 HTTP/1.1\r\nHost: 198.162.4.2:80\r\n\r\n", data->seqNum, data->error);
          //sprintf(req->request, "GET /adcData/%d/%d/0 HTTP/1.1\r\nHost: 198.168.3.1:80\r\n\r\n", data->seqNum, data->error);
          req->size = strlen(req->request);
          break;
        }
     }
}

static void get_request_P(FULL_POINT_MSG *path, FULL_REQ *req){
    
    switch (path->task_name) {
    
        case PATH:
        {
            sprintf(req->request, "GET /pointData/%d/%d/0 HTTP/1.1\r\nHost: 198.162.4.2:80\r\n\r\n", path->seqNum, path->error);
            //sprintf(req->request, "GET /pointData/%d/%d/0 HTTP/1.1\r\nHost: 198.168.3.1:80\r\n\r\n", path->seqNum, path->error);
            req->size = strlen(req->request);
            break;
            
        }
    }
 }

static short int seqCounter(short int curNum){
    if(curNum < 24){
        ++curNum;
    }
    else {
        curNum = 0;
    }
    return curNum;
}

//void buildRequest(FULL_MSG *data, FULL_POINT_MSG *path, FULL_REQ *req) {
void buildRequest_A(FULL_MSG *data, FULL_REQ *req){
    int i = 0;
    switch(data->msg_type){
        case PUT: {
            adc_put_request(data, req);
            while(i < req->size){
                if (pdTRUE == sendSingleCharMessageToTrasmitQueue(req->request[i])){
                //dbgOutputLoc(AFTER_POST_TO_TXQ);
                SYS_INT_SourceEnable(INT_SOURCE_USART_1_TRANSMIT);
                //dbgOutputLoc(SET_SOURCE_UART_TX);
                ++i;
                }
                else{
                    //life is over EVERYBODY DIES!!!!
                    //badRoutine(TXQ_FULL);
                }
            }
            data->error = 0;
            data->seqNum = seqCounter(data->seqNum);
            break;   
        }
        
        case GET: {
            get_request_A(data, req);
            while(i < req->size){
                if (pdTRUE == sendSingleCharMessageToTrasmitQueue(req->request[i])){
                    //dbgOutputLoc(AFTER_POST_TO_TXQ);
                    SYS_INT_SourceEnable(INT_SOURCE_USART_1_TRANSMIT);
                    //dbgOutputLoc(SET_SOURCE_UART_TX);
                    ++i;
                }
                else{
                    //life is over EVERYBODY DIES!!!!
                    //badRoutine(TXQ_FULL);
                }
            }
            data->error = 0;
            data->seqNum = seqCounter(data->seqNum);
            break;
        }
        
        case ERROR: {
            dbgOutputLoc(5);
            ++data->error;
        }
    }
}
/*************************************************************************************8*/

void buildRequest_P(FULL_POINT_MSG *path, FULL_REQ *req){
    //dbgUARTVal(data->msg_adc);
    int i = 0;
    switch(path->msg_type){
        case PUT: {
            path_put_request(path, req);
            while(i < req->size){
                if (pdTRUE == sendSingleCharMessageToTrasmitQueue(req->request[i])){
                //dbgOutputLoc(AFTER_POST_TO_TXQ);
                SYS_INT_SourceEnable(INT_SOURCE_USART_1_TRANSMIT);
                //dbgOutputLoc(SET_SOURCE_UART_TX);
                ++i;
                }
                else{
                    //life is over EVERYBODY DIES!!!!
                    //badRoutine(TXQ_FULL);
                }
            }
            path->error = 0;
            path->seqNum = seqCounter(path->seqNum);
            break;   
        }
        
        case GET: {
            get_request_P(path, req);
            while(i < req->size){
                if (pdTRUE == sendSingleCharMessageToTrasmitQueue(req->request[i])){
                    //dbgOutputLoc(AFTER_POST_TO_TXQ);
                    SYS_INT_SourceEnable(INT_SOURCE_USART_1_TRANSMIT);
                    //dbgOutputLoc(SET_SOURCE_UART_TX);
                    ++i;
                }
                else{
                    //life is over EVERYBODY DIES!!!!
                    //badRoutine(TXQ_FULL);
                }
            }
            path->error = 0;
            path->seqNum = seqCounter(path->seqNum);
            break;
        }
        
        case ERROR: {
            dbgOutputLoc(5);
            ++path->error;
        }
    }
}
/*************************************************************************************8*/


