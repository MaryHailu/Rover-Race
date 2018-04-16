/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    pathpoint.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#ifndef _PATHPOINT_H
#define _PATHPOINT_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "system_config.h"
#include "system_definitions.h"
#include "message.h"
#include "path_queue.h"
#include "messageHandlerQueue.h"


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END 

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

#define ROW 20
#define COL 20
#define TRUE 1
#define FALSE 0
#define OBSTACLE 0
#define FREE 1
    

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */


typedef struct
{
    unsigned int x;
    unsigned int y;
    unsigned int grid[ROW][COL];
    
} PATHPOINT_DATA;

typedef struct
{
    unsigned int x_point;
    unsigned int y_point;
    double f;
}G_POINT;


typedef struct{
    unsigned int x_o;
    unsigned int y_o;
    double f;
}OP;

typedef struct{
    unsigned int x_o;
    unsigned int y_o;
    double f;
    OP p[20];
}OPEN;

typedef struct
{
    unsigned int x_c;
    unsigned int y_c;
    double f;
    OP o[20];
}CLOSED;

	
typedef struct 
{
    unsigned int x_src;
    unsigned int y_src;
    
}SRC;

typedef struct 
{
   unsigned int parent_i, parent_j;
   unsigned int i, j;
}RES;
 
// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************
typedef struct 
{
    unsigned int x_d;
    unsigned int y_d;
    unsigned int x_des;
    unsigned int y_des;
 
}DES;

    
/*******************************************************************************
  Function:
    void PATHPOINT_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the 
    application in its initial state and prepares it to run so that its 
    APP_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    PATHPOINT_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void PATHPOINT_Initialize ( void );
void insert(OPEN *o, unsigned int x, unsigned int y, double f, unsigned int *count); // working
void inserttoclosed(CLOSED *c, unsigned int x, unsigned int y, double f, unsigned int *coutclose); //working
OPEN getsmallf(OPEN *o_l, unsigned int *count); //working
CLOSED closedlist(CLOSED *c_l);
int gridvalu(PATHPOINT_DATA *grid, PATHPOINT_DATA *value); //
bool isValidlocation(unsigned int x, unsigned int y);       // working
bool nobstacle(PATHPOINT_DATA *data, unsigned int row, unsigned int col);   //working 
bool isDestination(unsigned int x_cur, unsigned int y_cur, unsigned int goal_x, unsigned int goal_y); //working
double heuristics(unsigned int row, unsigned int col, unsigned int x_des, unsigned int y_des); // working 
bool inClosedlist(CLOSED *c, unsigned int x , unsigned int y, unsigned int *coutclose); // working 
void delefront(OPEN *o, unsigned int x , unsigned int y, double f, unsigned int *count);//
void path(RES *r,unsigned int x, unsigned int y, unsigned int z1, unsigned int z2,double *f,DES *d);
/*******************************************************************************
  Function:
    void PATHPOINT_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    PATHPOINT_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void PATHPOINT_Tasks( void );


#endif /* _PATHPOINT_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

