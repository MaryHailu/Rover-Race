/* 
 * File:   serializer_state.h
 * Author: Mary Hailu
 *
 * Created on April 14, 2018, 2:31 PM
 */

#ifndef SERIALIZER_STATE_H
#define	SERIALIZER_STATE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
    
#include "jsmn.h"
#include "data_build.h"
#include "message.h"

#define HEADER_MAX 35
#define JSON_MAX 250
    
typedef enum {
		START_H,
		START_T,
		START_T1,
		START_P,
		HEADER,
		START_PL,
		PAYLOAD,
		FOOTER,
		PUT_GOOD,
		EXP_H_HAND,
		FINISH
	}SERIAL_STATE_TYPE;

typedef struct {
		char payload[JSON_MAX];
		short int openCount;
		short int closedCount;
		bool payload_start;
		short int checksum;
		short int lengthNum;
	} PAYLOAD_DATA;
  
typedef struct {
		char header[HEADER_MAX];
		char length[HEADER_MAX];
		bool OK;
	}HEADER_DATA;
  
typedef struct {
		SERIAL_STATE_TYPE state;
		HEADER_DATA header_data;
		PAYLOAD_DATA payload_data;
		jsmn_parser j_parse;
		jsmntok_t tok[30];
		short int pos;
		short int err_count;
		bool isFooter;
        bool msgGood;
		short int c_count;
		char footerNum[5];
		short int foot_pos;
		FULL_MSG o_msg; /*******************************************************/
        FULL_POINT_MSG f_msg;
		short int seqNum;
	}SERIAL_STATE;

void reset_FSM(SERIAL_STATE *curFSM);
bool verifyResponse(SERIAL_STATE *curFSM);
void serializer_FSM(SERIAL_STATE *fsm, unsigned char msgChar);
void checksum(SERIAL_STATE *fsm);
void parseandsend(SERIAL_STATE *fsm);


#ifdef	__cplusplus
}
#endif

#endif	/* SERIALIZER_STATE_H */

