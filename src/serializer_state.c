#include "serializer_state.h"

static short int seqCounterFSM(short int curNum){
    if(curNum < 24){
        ++curNum;
    }
    else {
        curNum = 0;
    }
    return curNum;
}


/*--------------------------------------------
* reset_FSM is used to reset the state
* machine. Function is called to initialize,
* after a POST response or a bad response.
--------------------------------------------*/
void reset_FSM(SERIAL_STATE *curFSM) {
	curFSM->pos = 0;
	memset(&curFSM->header_data.header, 0, sizeof(curFSM->header_data.header));
	curFSM->state = START_H;
	curFSM->header_data.OK = false;
	curFSM->payload_data.openCount = 0;
	curFSM->payload_data.closedCount = 0;
	curFSM->payload_data.payload_start = false;
	curFSM->isFooter = false;
    curFSM->msgGood = false;
	curFSM->c_count = 0;
	curFSM->foot_pos = 0;
	jsmn_init(&curFSM->j_parse);
	curFSM->err_count = 0;
	curFSM->payload_data.checksum = 0;
}

/*--------------------------------------------
* verifyResponse is used to verify a http
* response header. If the response doesn't
* include 'HTTP/1.1' then it will return
* false. Also, if a GET or POST response
* can't be verified it will return false.
* Otherwise, function will return true,
* indicating a valid header response.
--------------------------------------------*/
bool verifyResponse(SERIAL_STATE *curFSM) {
	char head[9];
	char statusCode[4];
	char statusPhrase[10];
	short int count = 0;
	for (; count < curFSM->pos; ++count) {
		if (count < 9) {
			if (count == 8) {
				head[count] = '\0';
			}
			else
				head[count] = curFSM->header_data.header[count];
		}
		else if (count < 13) {
			if (count == 12) {
				statusCode[count - 9] = '\0';
			}
			else
				statusCode[count - 9] = curFSM->header_data.header[count];
		}
		else
			statusPhrase[count - 13] = curFSM->header_data.header[count];
	}

	statusPhrase[count - 13] = '\0';

	if ((strcmp(head, "HTTP/1.1"))) {
		return false;
	}
	if (!(strcmp(statusCode, "200")) && !(strcmp(statusPhrase, "OK"))) {
		curFSM->header_data.OK = true;
		return true;
	}
	else
		return false;
}

/*--------------------------------------------
* serializer_FSM is the state machine
* implementation for the serializer task.
--------------------------------------------*/
void serializer_FSM(SERIAL_STATE *fsm, unsigned char msgChar) {
	if (msgChar == 'H') {
		if (fsm->state == START_H) {
			fsm->header_data.header[fsm->pos] = msgChar;
			++fsm->pos;
			fsm->state = START_T;
		}
		else {
			fsm->state = EXP_H_HAND;
		}
	}
	else {
		switch (fsm->state) {
		/*case START_H: {
			if (msgChar == 'T') {
                dbgOutputLoc(15);
				fsm->header_data.header[fsm->pos] = 'H';
				++fsm->pos;
				fsm->header_data.header[fsm->pos] = msgChar;
				++fsm->pos;
				fsm->state = START_T;
			}
			break;
		}*/
		case START_T: {
			if (msgChar == 'T') {
				fsm->header_data.header[fsm->pos] = msgChar;
				++fsm->pos;
				fsm->state = START_T1;
			}
			else {
                dbgOutputLoc(25);
				fsm->seqNum = seqCounterFSM(fsm->seqNum);
				++fsm->err_count;
				fsm->state = FINISH;
			}
			break;
		}
		case START_T1: {
			if (msgChar == 'T') {
				fsm->header_data.header[fsm->pos] = msgChar;
				++fsm->pos;
				fsm->state = START_P;
			}
			else {
                if (msgChar != 'M'){
                    dbgOutputLoc(30);
                    fsm->seqNum = seqCounterFSM(fsm->seqNum);
                    ++fsm->err_count;
                    fsm->state = FINISH;
                }
                else{
                    fsm->state = START_H;
                }
			}
			break;
		}
		case START_P: {
			if (msgChar == 'P') {
				fsm->header_data.header[fsm->pos] = msgChar;
				++fsm->pos;
				fsm->state = HEADER;
			}
			else {
                dbgOutputLoc(35);
				fsm->seqNum = seqCounterFSM(fsm->seqNum);
				++fsm->err_count;
				fsm->state = FINISH;
			}
			break;
		}
		case HEADER: {
			//dbgOutputLoc(IN_HEADER);
			if (msgChar != '\r') {
				fsm->header_data.header[fsm->pos] = msgChar;
				++fsm->pos;
			}
			else {
				if (verifyResponse(fsm)) {
					if (fsm->header_data.OK) {
						//dbgOutputLoc(HTTP_GET_RESP_GOOD);
						fsm->state = START_PL;
						fsm->pos = 0;
					}
					else {
						dbgOutputLoc(40);
						fsm->seqNum = seqCounterFSM(fsm->seqNum);
						++fsm->err_count;
						fsm->state = FINISH;
					}
				}
				else {
                    dbgOutputLoc(45);
					fsm->seqNum = seqCounterFSM(fsm->seqNum);
					++fsm->err_count;
					fsm->state = FINISH;
				}
			}

			break;
		}
		case START_PL: {
			if (msgChar == '<') {
				fsm->payload_data.payload_start = true;
				break;
			}
			if (fsm->payload_data.payload_start) {
				if (msgChar == '{') {
					fsm->payload_data.payload[fsm->pos] = msgChar;
					++fsm->pos;
					++fsm->payload_data.openCount;
					fsm->state = PAYLOAD;
				}
				else if (msgChar == 'P') {
					fsm->state = PUT_GOOD;
				}
				else if (msgChar == '>') {
                    dbgOutputLoc(50);
					fsm->seqNum = seqCounterFSM(fsm->seqNum);
					++fsm->err_count;
					fsm->state = FINISH;
				}
			}
			else {
				if (msgChar == '>') {
                    dbgOutputLoc(55);
					fsm->seqNum = seqCounterFSM(fsm->seqNum);
					++fsm->err_count;
					fsm->state = FINISH;
				}
			}
			break;
		}
		case PAYLOAD: {
			if (msgChar == '{') {
				++fsm->payload_data.openCount;
			}
			else if (msgChar == '}') {
				++fsm->payload_data.closedCount;
			}

			//if '{ 'count is not equal to the '}' count
			if (fsm->payload_data.openCount != fsm->payload_data.closedCount) {
				if (msgChar > 31) {
					fsm->payload_data.payload[fsm->pos] = msgChar;
					++fsm->pos;
				}
			}
			else {
				if (fsm->payload_data.payload_start) {
					fsm->payload_data.payload[fsm->pos] = msgChar;
					//dbgOutputLoc(FINISHED_PAYLOAD);
					fsm->payload_data.payload[fsm->pos + 1] = '\0';
					checksum(fsm);
					fsm->state = FOOTER;
				}
			}
			break;
		}

		case FOOTER: {
			if (msgChar < 32 || msgChar > 127) {
				break;
			}
			if (!fsm->isFooter) {
				if (msgChar == ',') {
					fsm->isFooter = true;
				}
			}
			else {
				if (fsm->c_count == 0) { // length
					if (msgChar != ',') {
						fsm->footerNum[fsm->foot_pos] = msgChar;
						++fsm->foot_pos;
					}
					else {
                        fsm->footerNum[fsm->foot_pos] = '\0';
						if (atoi(fsm->footerNum) != strlen(fsm->payload_data.payload)) {                                    
                            dbgOutputLoc(95);
							fsm->seqNum = seqCounterFSM(fsm->seqNum);
							++fsm->err_count;
							fsm->state = FINISH;
						}
						fsm->foot_pos = 0;
						++fsm->c_count;
					}
				}
				else if (fsm->c_count == 1) { // checksum
					if (msgChar != ',') {
						fsm->footerNum[fsm->foot_pos] = msgChar;
						++fsm->foot_pos;
					}
					else {
						fsm->footerNum[fsm->foot_pos] = '\0';
						if (atoi(fsm->footerNum) != fsm->payload_data.checksum) {
                            dbgOutputLoc(85);
							fsm->seqNum = seqCounterFSM(fsm->seqNum);
							++fsm->err_count;
							fsm->state = FINISH;
						}
						fsm->foot_pos = 0;
						++fsm->c_count;
					}
				}
				else if (fsm->c_count == 2) { // sequence number
					if (msgChar != '>') {
						fsm->footerNum[fsm->foot_pos] = msgChar;
						++fsm->foot_pos;
					}
					else {
						fsm->footerNum[fsm->foot_pos] = '\0';
						if (atoi(fsm->footerNum) != fsm->seqNum) {
                            dbgOutputLoc(atoi(fsm->footerNum));
                            dbgOutputLoc(75);
                            dbgOutputLoc(fsm->seqNum);
							fsm->seqNum = atoi(fsm->footerNum);
                            fsm->seqNum = seqCounterFSM(fsm->seqNum);
							++fsm->err_count;
							fsm->state = FINISH;
						}
                        else {
                            fsm->msgGood = true;
                            fsm->seqNum = seqCounterFSM(fsm->seqNum);
                            fsm->state = FINISH;
                        }
					}
				}
			}
			break;
		}
		case PUT_GOOD: {
			if (msgChar == ',') {
				fsm->payload_data.payload_start = true;
				break;
			}
			
			if (fsm->payload_data.payload_start) {
				if (msgChar != '>') {
					fsm->footerNum[fsm->foot_pos] = msgChar;
					++fsm->foot_pos;
				}
				else {
					fsm->footerNum[fsm->foot_pos] = '\0';
					if (atoi(fsm->footerNum) != fsm->seqNum) {
                        dbgOutputLoc(atoi(fsm->footerNum));
                            dbgOutputLoc(103);
                            dbgOutputLoc(fsm->seqNum);
                        fsm->seqNum = seqCounterFSM(fsm->seqNum);
						++fsm->err_count;
						fsm->state = FINISH;
					}
                    else {
                        fsm->seqNum = seqCounterFSM(fsm->seqNum);
                        fsm->state = FINISH;
                    }
                }
			}
			break;
		}
		case EXP_H_HAND: {
			if (msgChar == 'o') {
				fsm->state = PAYLOAD;
			}
			else if (msgChar == 'T') {
                dbgOutputLoc(65);
				fsm->seqNum = seqCounterFSM(fsm->seqNum);
				++fsm->err_count;
				fsm->pos = 0;
				memset(&fsm->header_data.header, 0, sizeof(fsm->header_data.header));
				fsm->header_data.header[fsm->pos] = 'H';
				++fsm->pos;
				fsm->header_data.header[fsm->pos] = msgChar;
				++fsm->pos;
				fsm->state = START_T1;
			}
			break;
		}
		default: {
			break;
		}
		}
	}
}


/*--------------------------------------------
* checksum creates a value by xor'ing each
* char in the payload array. This will be
* used by the PIC and server to verify the
* message sent.
--------------------------------------------*/
void checksum(SERIAL_STATE *fsm) {
	int i;
	int sz = strlen(fsm->payload_data.payload);
	for (i = 0; i < sz; ++i) {
		fsm->payload_data.checksum ^= fsm->payload_data.payload[i];
	}
}

void parseandsend(SERIAL_STATE *fsm) {
	int tokenNum;
    
    if(fsm->msgGood){
		tokenNum = jsmn_parse(&fsm->j_parse, fsm->payload_data.payload, fsm->pos + 1, fsm->tok, sizeof(fsm->tok) / (sizeof(fsm->tok[0])));
		if (tokenNum <= 0) {
            dbgOutputLoc(58);
			distributeError();
			//dbgOutputLoc(BAD_PAYLOAD);
			//dbgOutputLoc(tokenNum);
		}
		fsm->f_msg = build_data(fsm->tok, fsm->payload_data.payload, fsm->j_parse.toknext);
		distribute(fsm->f_msg);
        while (fsm->err_count != 0){
            distributeError();
            --fsm->err_count;
        }
    }
    else {
        while (fsm->err_count != 0){
            distributeError();
            --fsm->err_count;
        }
    }
}
