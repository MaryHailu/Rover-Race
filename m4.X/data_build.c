#include "data_build.h"

TASK_P findTask(char task[]) {
    TASK_P t_name;
    if ((!strcmp(task, "PATH"))){
        t_name = PATH;
    }
    return t_name;
}

void packData(FULL_POINT_MSG *j, CUR_DATA *c_data){
    
    if (!(strcmp(c_data->key, "task"))){
        j->task_name = findTask(c_data->value);
    }
    else if (!(strcmp(c_data->key, "x"))){
        j->msg_point.x_coord = atoi(c_data->value);
    }
    else if (!(strcmp(c_data->key, "y"))){
        j->msg_point.y_coord = atoi(c_data->value);
    } 
    /* Why is this commented out? - Nathan */ 
 
    /*else if (!(strcmp(c_data->key, "ticks"))){
        unsigned int tempTick = atoi(c_data->value);
        j->m_data.ticks[c_data->value_num] = tempTick;
        ++c_data->value_num;
        if(c_data->value_num > 1){
            c_data->value_num = 0;
        }
    }
    else if (!(strcmp(c_data->key, "revs"))){
        unsigned int tempRev = atoi(c_data->value);
        j->m_data.revs[c_data->value_num] = tempRev;
        ++c_data->value_num;
        if(c_data->value_num > 1){
            c_data->value_num = 0;
        }
    }*/
}

FULL_POINT_MSG build_data(jsmntok_t *tokens, char msg[], unsigned int size){
    FULL_POINT_MSG new_msg;
    CUR_DATA current_data;
    current_data.value_num = 0;
    
    unsigned int i = 0;
    unsigned int count = 0;
    unsigned int begin;

    bool isKey = true;

    /** End of Initiliaze variables **/ 
    
    for (; i < size; ++i){
        
	char c[FIELD_MAX];

        if (tokens->type != JSMN_OBJECT) {
            
            begin = tokens->start;
            for(; begin != tokens->end; ++begin) {
                c[count] = msg[begin];
                ++count;
            }
        
        c[count] = '\0';
        
	
                if (isKey) { // isKey declared as true 
                        if (strcmp(c, "data") != 0) {
                            strcpy(current_data.key, c);
                            isKey = false;
                        }
                }
                    else  {
                    
                        if(tokens->type != JSMN_ARRAY){
                                strcpy(current_data.value, c);
                                packData(&new_msg, &current_data);

                                if(current_data.value_num == 0)
                                {   isKey = true;   }
                        }
                    }
                        count = 0; 
        }

                    ++tokens;
    }

    return new_msg;
    //dbgOutputLoc(END_BUILD_DATA);
}
