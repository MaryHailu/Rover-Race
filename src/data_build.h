/* 
 * File:   data_build.h
 * Author: Mary Hailu
 *
 * Created on April 14, 2018, 2:34 PM
 */

#ifndef DATA_BUILD_H
#define	DATA_BUILD_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "jsmn.h"
#include "debug.h"
#include "message.h"

#define FIELD_MAX 30
    
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct{
    char key[FIELD_MAX];
    char value[FIELD_MAX];
    unsigned int value_num;
} CUR_DATA;


TASK_P findTask(char task[]);
void packData(FULL_POINT_MSG *j, CUR_DATA *c_data);
//FULL_MSG build_data(jsmntok_t *tokens, char msg[], unsigned int size);
FULL_POINT_MSG build_data(jsmntok_t *tokens, char msg[], unsigned int size);



#ifdef	__cplusplus
}
#endif

#endif	/* DATA_BUILD_H */

