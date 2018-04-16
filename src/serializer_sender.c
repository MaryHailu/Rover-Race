#include "serializer_sender.h"

void distribute(FULL_POINT_MSG msg){
    //FULL_POINT_MSG msg;
    PATH_MSG data;
    //data.position.x = msg.msg_point.x_coord;
    //data.position.y = msg.msg_point.y_coord;
    data.position.x = 9;
    data.position.y = 9;
      
            //dbgOutputLoc(BEFORE_SENT_TO_LOCQ);
    if(pdTRUE == sendMessageToPathQueue(data)){
         //dbgOutputLoc(AFTER_SENT_TO_LOCQ);
     }
}

void distributeError(){
    FULL_POINT_MSG errMsg;
    errMsg.msg_type = ERROR;
    if(pdTRUE != sendMessageToMsgHandQueueFromPathTask(errMsg)){
      //  badRoutine(0xFF);
    }
}
