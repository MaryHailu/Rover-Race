/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    pathpoint.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "pathpoint.h"
#include "message.h"
#include "path_queue.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

PATHPOINT_DATA pathpointData;

// *****************************************************************************
// *****************************************************************************
// Section: Local Function delet from open list
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Local Function openlist
// *****************************************************************************
// *****************************************************************************

void insert(OPEN *o, unsigned int x, unsigned int y, double f, unsigned int *count){
    
    if(*count < 30){
        o->p[*count].x_o = x;
        o->p[*count].y_o = y;
        o->p[*count].f = f;
        *count += 1;
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Local Function closedlist
// *****************************************************************************
// *****************************************************************************


void inserttoclosed(CLOSED *c, unsigned int x, unsigned int y, double f, unsigned int *coutclose){
    if(*coutclose != 30){
        
        c->o[*coutclose].x_o = x;
        c->o[*coutclose].y_o = y;
        c->o[*coutclose].f = f;
       *coutclose = *coutclose + 1;
     }
    
}
// *****************************************************************************
// *****************************************************************************
// Section: Function return the smallest value with f 
// *****************************************************************************
// *****************************************************************************
OPEN getsmallf(OPEN *o_l, unsigned int *count){
   
    unsigned int i , j, x, y;
    double fn;
    
    for(i=0; i< *count; i++){
        
           for (j = i+1; j < *count; ++j){ 
               
            if(o_l->p[i].f > o_l->p[j].f){
               
                fn = o_l->p[i].f;
                x = o_l->p[i].x_o;
                y = o_l->p[i].y_o;
                
                o_l->p[i].f = o_l->p[j].f;
                o_l->p[i].x_o = o_l->p[j].x_o;
                o_l->p[i].y_o = o_l->p[j].y_o;
                
                o_l->p[j].f = fn;
                o_l->p[j].x_o = x;
                o_l->p[j].y_o = y;
             }
        }
    }
   return *o_l;
    
}
// *****************************************************************************
// *****************************************************************************
// Section: Function isValid
// *****************************************************************************
// *****************************************************************************

bool isValidlocation(unsigned int x, unsigned int y){
    
    if((x > 0) && (x < ROW) && (y > 0) && (y < COL)){
        return TRUE;
    }
    else{
        return FALSE;
    }
}
// *****************************************************************************
// *****************************************************************************
// Section: Function is row and col are the the destination point
// *****************************************************************************
// *****************************************************************************

bool isDestination(unsigned int x_cur, unsigned int y_cur, unsigned int goal_x, unsigned int goal_y){
    if((x_cur == goal_x) && (y_cur == goal_y)){
        return TRUE;
    }
    else {
        return FALSE;
    }
}
// *****************************************************************************
// *****************************************************************************
// Section: Function heuristics 
// *****************************************************************************
// *****************************************************************************
double heuristics(unsigned int row, unsigned int col, unsigned int x_des, unsigned int y_des){
    double distance; 
    distance = sqrt((row - x_des ) * (row - x_des ) + 
                    (col - y_des) *(col - y_des));
    return distance;
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************
bool nobstacle(PATHPOINT_DATA *data, unsigned int row, unsigned int col){
    if(data->grid[row][col] == 1){
        return TRUE;
    }
    else{
        return FALSE;
    }
}
// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************
bool inClosedlist(CLOSED *c, unsigned int x , unsigned int y, unsigned int *coutclose){
    unsigned int i;
    
    for (i=0; i <= *coutclose; i++) {
        
        if ((c->o[i].x_o == x) && (c->o[i].y_o == y)){
            return TRUE;
        }
     }
    return FALSE;
}

// *****************************************************************************
// *****************************************************************************
// Section: delete from open list 
// *****************************************************************************
// *****************************************************************************
void delefront(OPEN *o, unsigned int x , unsigned int y, double f, unsigned int *count){
    unsigned int i;
    if((o->p[0].x_o == x) && (o->p[0].y_o == y) && (o->p[0].f == f)){
        for(i=1; i< *count; i++){
            o->p[i - 1].x_o = o->p[i].x_o;
            o->p[i - 1].y_o = o->p[i].y_o;
            o->p[i - 1].f = o->p[i].f;
            *count = *count - 1;
        }
    }
}


// *****************************************************************************
// *****************************************************************************
// Section: to return the path
// *****************************************************************************
// *****************************************************************************
//void path(RES *cellD[ROW][COL], DES *d){
void path(RES *r,unsigned int x, unsigned int y, unsigned int z1,unsigned int z2, double *f, DES *d){
//void path(unsigned int x, unsigned int y, DES *d){
    RES p[ROW][COL];
    p[x][y].parent_i= z1;
    p[x][y].parent_j= z2;
    unsigned int i =0, row, col;
    
    FULL_POINT_MSG msgOut;
    short int local_seqNum = 0;
    short int local_error = 0;
   
    row = d->x_des; col = d->y_des;
   
   // if((p[x][y].parent_i <= 6) && (p[x][y].parent_j == 1)){
        msgOut.msg_type = PUT;
        msgOut.task_name = PATH;
        msgOut.msg_point.x_coord =p[x][y].parent_i;
        msgOut.msg_point.y_coord =p[x][y].parent_j;
        msgOut.seqNum = local_seqNum;
        msgOut.error = local_error;

        if(pdTRUE != sendMessageToMsgHandQueueFromPathTask(msgOut)){
                 badRoutine(0xFF);
                 dbgUARTVal(0);
        }
    //}
      
}


/*******************************************************************************
  Function:
    void PATHPOINT_Initialize ( void )

  Remarks:
    See prototype in pathpoint.h.
 */

void PATHPOINT_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    initpathQueue();
}


/**
static void astarsearch(SRC *src, DES *des, PATHPOINT_DATA *data){
        FULL_POINT_MSG msgOut;
        short int local_seqNum = 0;
        short int local_error = 0;
        OPEN open, smallf;  unsigned int count = 0; 
        CLOSED close; unsigned int coutclose = 0;
        unsigned int cell_x, cell_y, cell_f;
        double g=0.0, h=0.0, f=0.0; 
        RES *r; unsigned int countres =0;
       
          
        //Add node_start to the OPEN list
         insert(&open, src->x_src, src->y_src, f, &count);
                  
         bool foundDest = FALSE;
      
        //while the OPEN list is not empty
         while(open.p[0].x_o > 0){ 
             
            open = getsmallf(&open, &count);
           
            cell_x = open.p[0].x_o; cell_y = open.p[0].y_o, cell_f = open.p[0].f;
       
            delefront(&open, open.p[0].x_o , open.p[0].y_o, open.p[0].f, &count);//----------------
            
            inserttoclosed(&close, cell_x, cell_y, cell_f,&coutclose);
           
           //IF n is the same as node_goal 
           if(isDestination(cell_x, cell_y, des->x_des, des->y_des) == TRUE){
               
               msgOut.msg_type = PUT;
               msgOut.task_name = PATH;
               msgOut.msg_point.x_coord = cell_x;
               msgOut.msg_point.y_coord = cell_y;
               msgOut.seqNum = local_seqNum;
               msgOut.error = local_error;

               /*if(pdTRUE != sendMessageToMsgHandQueueFromPathTask(msgOut)){
                   badRoutine(0xFF);
                   dbgUARTVal(0);
                }*
                path(r, cell_x, cell_y, cell_x, cell_y, &f, des);
                foundDest = TRUE;
            }
           else{
               // Generate each successor node n' of n
                /* Cell-->Popped Cell (i, j)
                   N -->  North       (i-1, j)
                   S -->  South       (i+1, j)
                   E -->  East        (i, j+1)
                   W -->  West        (i, j-1)
                   N.E--> North-East  (i-1, j+1)
                   N.W--> North-West  (i-1, j-1)
                   S.E--> South-East  (i+1, j+1)
                   S.W--> South-West  (i+1, j-1)
                */
               /******************************************************************************************************
               if(isValidlocation((cell_x-1), cell_y)== TRUE){             // N |
                    
                   if(isDestination((cell_x-1), cell_y, des->x_des, des->y_des) == TRUE){
                      
                       path(r,(cell_x-1), cell_y, cell_x, cell_y, &f, des);
                       //path(cellD, des);
                       foundDest = TRUE;
                       break;
                    }
                   
                    // If there is no obstacle on the cell and it is not in closed list continue
                    else if((nobstacle(data, (cell_x-1), cell_y) == TRUE) &&
                           (inClosedlist(&close, (cell_x-1), cell_y, &coutclose) == FALSE)){  // there is 0 at (9,3)
                       g = g + 1.0; 
                       h = heuristics((cell_x-1),cell_y, des->x_des, des->y_des);
                       f = g + h;
                       
                       //Add node_start to the OPEN list
                       insert(&open, (cell_x-1), cell_y, f, &count);
                       
                       path(r, (cell_x-1), cell_y, cell_x, cell_y, &f, des);
                     
                    }
               }
                /******************************************************************************************************
                if(isValidlocation((cell_x+1), cell_y)== TRUE){             // S |
                    
                   if(isDestination((cell_x+1), cell_y, des->x_des, des->y_des) == TRUE){
                        
                        path(r, (cell_x+1), cell_y, cell_x, cell_y, &f, des);
                       
                        foundDest = TRUE;
                        break;
                    }
                    // If there is no obstacle on the cell and it is not in closed list continue
                    else if((nobstacle(data, (cell_x+1), cell_y) == TRUE) &&
                           (inClosedlist(&close, (cell_x+1), cell_y, &coutclose) == FALSE)){  // there is 0 at (9,3)
                       
                       g = g + 1.0; 
                       h = heuristics((cell_x+1),cell_y, des->x_des, des->y_des);
                       f = g + h;
                       //Add node_start to the OPEN list
                       insert(&open, (cell_x+1), cell_y, f, &count);
                      
                       path(r, (cell_x+1), cell_y, cell_x, cell_y, &f, des);
                   
                       
                    }
               }
               
              /******************************************************************************************************
              if(isValidlocation(cell_x, (cell_y+1))== TRUE){             // E |
                    
                   if(isDestination(cell_x, (cell_y+1), des->x_des, des->y_des) == TRUE){
    
                        path(r, cell_x, (cell_y+1), cell_x, cell_y, &f, des);
                        //path(cellD, des);
                        
                        foundDest = TRUE;
                        break;
                    }
                    // If there is no obstacle on the cell and it is not in closed list continue
                    else if((nobstacle(data, cell_x, (cell_y+1)) == TRUE) &&
                           (inClosedlist(&close, cell_x, (cell_y+1), &coutclose) == FALSE)){  // there is 0 at (9,3)
                       
                       g = g + 1.0; 
                       h = heuristics(cell_x, (cell_y+1), des->x_des, des->y_des);
                       f = g + h;
                       //Add node_start to the OPEN list
                       insert(&open, cell_x, (cell_y+1), f, &count);
                  
                       path(r, cell_x, (cell_y+1), cell_x, cell_y, &f, des);
                    }
               }
               
               /******************************************************************************************************
               if(isValidlocation(cell_x, (cell_y-1))== TRUE){             // W |
                    
                   if(isDestination(cell_x, (cell_y-1), des->x_des, des->y_des) == TRUE){

                        path(r, cell_x, (cell_y-1), cell_x, cell_y, &f, des);
                        //path(cellD, des);
                        foundDest = TRUE;
                        break;
                    }
                    // If there is no obstacle on the cell and it is not in closed list continue
                    else if((nobstacle(data, cell_x, (cell_y-1)) == TRUE) &&
                           (inClosedlist(&close, cell_x, (cell_y-1), &coutclose) == FALSE)){  // there is 0 at (9,3)
                       
                       g = g + 1.0; 
                       h = heuristics(cell_x,(cell_y-1), des->x_des, des->y_des);
                       f = g + h;
                       //Add node_start to the OPEN list
                       insert(&open, cell_x, (cell_y-1), f, &count);
                       
                       path(r, cell_x, (cell_y-1), cell_x, cell_y, &f, des);
                       
                   }
               }
               /******************************************************************************************************
               if(isValidlocation((cell_x-1), (cell_y+1))== TRUE){             // NE
                    
                   if(isDestination((cell_x-1), (cell_y+1), des->x_des, des->y_des) == TRUE){
                       
                        path(r, (cell_x-1), (cell_y+1), cell_x, cell_y, &f, des);
                        //path(cellD, des);
                        foundDest = TRUE;
                        break;
                    }
                    // If there is no obstacle on the cell and it is not in closed list continue
                    else if((nobstacle(data, (cell_x-1), (cell_y+1)) == TRUE) &&
                           (inClosedlist(&close, (cell_x-1), (cell_y+1), &coutclose) == FALSE)){  // there is 0 at (9,3)
                       g = g + 1.414; 
                       h = heuristics((cell_x-1),(cell_y+1), des->x_des, des->y_des);
                       f = g + h;
                       //Add node_start to the OPEN list
                       insert(&open, (cell_x-1), (cell_y+1), f, &count);
                       
                       path(r, (cell_x-1), (cell_y+1), cell_x, cell_y, &f, des);
                      
                   }
               }
               /******************************************************************************************************
               if(isValidlocation((cell_x-1), (cell_y-1))== TRUE){             // NW
                    
                   if(isDestination((cell_x-1), (cell_y-1), des->x_des, des->y_des) == TRUE){
                      
                        path(r, (cell_x-1), (cell_y-1), cell_x, cell_y, &f, des);
                        //path(cellD, des);
                        foundDest = TRUE;
                        break;
                    }
                    // If there is no obstacle on the cell and it is not in closed list continue
                    else if((nobstacle(data, (cell_x-1), (cell_y-1)) == TRUE) &&
                           (inClosedlist(&close, (cell_x-1), (cell_y-1), &coutclose) == FALSE)){  // there is 0 at (9,3)
                       g = g + 1.414; 
                       h = heuristics((cell_x-1),(cell_y-1), des->x_des, des->y_des);
                       f = g + h;
                       //Add node_start to the OPEN list
                       insert(&open, (cell_x-1), (cell_y-1), f, &count);
                     
                       path(r, (cell_x-1), (cell_y-1), cell_x, cell_y, &f, des);
                       
                   }
               }
               /******************************************************************************************************
               if(isValidlocation((cell_x+1), (cell_y+1))== TRUE){             // SE
                    
                   if(isDestination((cell_x+1), (cell_y+1), des->x_des, des->y_des) == TRUE){
                       
                        path(r, (cell_x+1), (cell_y+1), cell_x, cell_y, &f, des);
                        //path(cellD, des);
                        foundDest = TRUE;
                        break;
                    }
                    // If there is no obstacle on the cell and it is not in closed list continue
                    else if((nobstacle(data, (cell_x+1), (cell_y+1)) == TRUE) &&
                           (inClosedlist(&close, (cell_x+1), (cell_y+1), &coutclose) == FALSE)){  // there is 0 at (9,3)
                       g = g + 1.414; 
                       h = heuristics((cell_x+1),(cell_y+1), des->x_des, des->y_des);
                       f = g + h;
                       //Add node_start to the OPEN list
                       insert(&open, (cell_x+1), (cell_y+1), f, &count);
                       
                       path(r, (cell_x+1), (cell_y+1), cell_x, cell_y, &f, des);
                     
                   }
               }
               /******************************************************************************************************
               if(isValidlocation((cell_x+1), (cell_y-1))== TRUE){             // SW
                    
                   if(isDestination((cell_x+1), (cell_y-1), des->x_des, des->y_des) == TRUE){
                       // backtrack and return the values
                       
                        path(r, (cell_x+1), (cell_y-1), cell_x, cell_y, &f, des);
                        //path(cellD, des);
                        foundDest = TRUE;
                        break;
                    }
                    // If there is no obstacle on the cell and it is not in closed list continue
                    else if((nobstacle(data, (cell_x+1), (cell_y-1)) == TRUE) &&
                           (inClosedlist(&close, (cell_x+1), (cell_y-1), &coutclose) == FALSE)){  // there is 0 at (9,3)
                       g = g + 1.414; 
                       h = heuristics((cell_x+1),(cell_y-1), des->x_des, des->y_des);
                       f = g + h;
                       //Add node_start to the OPEN list
                       insert(&open, (cell_x+1), (cell_y-1), f, &count);
                       
                       path(r, (cell_x+1), (cell_y-1), cell_x, cell_y, &f, des);
                      
                    }
                    
                }
             /******************************************************************************************************
       
        }
      }
    if(foundDest == FALSE){
        msgOut.msg_type = PUT;
        msgOut.task_name = PATH;
        msgOut.msg_point.x_coord = 0;
        msgOut.msg_point.y_coord = 0;
        msgOut.seqNum = local_seqNum;
        msgOut.error = local_error;

       if(pdTRUE != sendMessageToMsgHandQueueFromPathTask(msgOut)){
          badRoutine(0xFF);
          dbgUARTVal(0);
        }
    }
}*/

/******************************************************************************
  Function:
    void PATHPOINT_Tasks ( void )

  Remarks:
    See prototype in pathpoint.h.
 */
/**
void PATHPOINT_Tasks ( void )
{
    PATHPOINT_DATA data; DES des; SRC src;
    unsigned int i;
    
    // 0 set for the obstacles 
    for(data.x=1; data.x < ROW; data.x++){
        for(data.y=1; data.y < COL; data.y++){
            data.grid[data.x][data.y] = 1;
        }
    }
  
    data.grid[2][4] = 0;
    data.grid[5][3] = 0;
    data.grid[7][4] = 0;
    data.grid[6][5] = 0;
    
    // 1 set to non obstacle 
   /* for(data.x=1; data.x < ROW; data.x++){
        for(data.y=1; data.y < COL; data.y++){
            for(i= 1; i <5; i++){
                data.grid[rand() % ROW][rand() % COL] = 1;
            }
        }
    }*
    
    src.x_src = 6; src.y_src = 1;
    des.x_des = 7; des.y_des = 3;
            
    while(1){
         astarsearch(&src, &des, &data);
    }
}*/

void PATHPOINT_Tasks ( void ){
    
     while (1){
        //dbgOutputLoc();
        PATH_MSG point;
        
        while (1){
           
            //dbgOutputLoc(110);
            point = receiveMessageFromPathQueue();
            
            FULL_POINT_MSG msgOut;
            
            msgOut.msg_type = PUT;
            msgOut.task_name = PATH;
           
            msgOut.msg_point.x_coord = point.position.x;
            msgOut.msg_point.y_coord = point.position.y;
            //msgOut.msg_point.x_coord = 1;
            //msgOut.msg_point.y_coord = 2;
            
            if(pdTRUE != sendMessageToMsgHandQueueFromPathTask(msgOut)){
                badRoutine(0xFF);
            }
        }
        //dbgOutputLoc();
    }
}



/*******************************************************************************
 End of File
 */
