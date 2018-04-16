/* 
 * File:   message.h
 * Author: Mary Hailu
 *
 * Created on April 1, 2018, 3:56 PM
 */

#ifndef MESSAGE_H
#define	MESSAGE_H


#define MAX_MSG_SIZE 200
#include "stdio.h"


typedef enum {
	GET = 0,
	POST,
	PUT,
    ERROR
} SELECT;

//sub out TASK1, etc with actual task names
typedef enum {
	ADC = 0,
    ERROR_A
} TASK_A;

typedef enum {
	PATH = 0,
    ERROR_P
} TASK_P;

typedef struct {
	unsigned int adcvalue_UD;
    unsigned int adcvalue_LR;
}JDATA;

typedef struct {
	short int x_coord;
	short int y_coord;
} POINT;

typedef struct {
	SELECT msg_type;
    JDATA msg_adc;
	//TASK task_name;
    TASK_A task_name;
	short int seqNum;
	short int error;
} FULL_MSG;

typedef struct {
	SELECT msg_type;
	POINT msg_point;
	//TASK task_name;
    TASK_P task_name;
	short int seqNum;
	short int error;
}FULL_POINT_MSG;


typedef struct {
	char request[300];
	short int size;
} FULL_REQ;

#endif	/* MESSAGE_H */

