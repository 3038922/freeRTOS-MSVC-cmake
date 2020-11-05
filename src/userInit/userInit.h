#pragma once

/* Standard includes. */
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

/* Visual studio intrinsics used so the __debugbreak() function is available
should an assert get hit. */
#include <intrin.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

typedef void *queue_t;
typedef queue_t sem_t;
typedef queue_t mutex_t;
typedef struct QueueDefinition
{
    int8_t *pcHead;    /*< Points to the beginning of the queue storage area. */
    int8_t *pcTail;    /*< Points to the byte at the end of the queue storage area.  Once more byte is allocated than necessary to store the queue items, this is used as a marker. */
    int8_t *pcWriteTo; /*< Points to the free next place in the storage area. */

    union /* Use of a union is an exception to the coding standard to ensure two mutually exclusive structure members don't appear simultaneously (wasting RAM). */
    {
        int8_t *pcReadFrom;            /*< Points to the last place that a queued item was read from when the structure is used as a queue. */
        uint32_t uxRecursiveCallCount; /*< Maintains a count of the number of times a recursive mutex has been recursively 'taken' when the structure is used as a mutex. */
    } u;

    List_t xTasksWaitingToSend;    /*< List of tasks that are blocked waiting to post onto this queue.  Stored in priority order. */
    List_t xTasksWaitingToReceive; /*< List of tasks that are blocked waiting to read from this queue.  Stored in priority order. */

    volatile uint32_t uxMessagesWaiting; /*< The number of items currently in the queue. */
    uint32_t uxLength;                   /*< The length of the queue defined as the number of items it will hold, not the number of bytes. */
    uint32_t uxItemSize;                 /*< The size of each items that the queue will hold. */

    volatile int8_t cRxLock; /*< Stores the number of items received from the queue (removed from the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */
    volatile int8_t cTxLock; /*< Stores the number of items transmitted to the queue (added to the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */

#if ((configSUPPORT_STATIC_ALLOCATION == 1) && (configSUPPORT_DYNAMIC_ALLOCATION == 1))
    uint8_t ucStaticallyAllocated; /*< Set to pdTRUE if the memory used by the queue was statically allocated to ensure no attempt is made to free the memory. */
#endif

#if (configUSE_QUEUE_SETS == 1)
    struct QueueDefinition *pxQueueSetContainer;
#endif

#if (configUSE_TRACE_FACILITY == 1)
    uint32_t uxQueueNumber;
    uint8_t ucQueueType;
#endif

} xQUEUE;

/* The old xQUEUE name is maintained above then typedefed to the new Queue_t
name below to enable the use of older kernel aware debuggers. */
typedef xQUEUE Queue_t;

/* This project provides two demo applications.  A simple blinky style demo
application, and a more comprehensive test and demo application.  The
mainCREATE_SIMPLE_BLINKY_DEMO_ONLY setting is used to select between the two.

If mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is 1 then the blinky demo will be built.
The blinky demo is implemented and described in main_blinky.c.

If mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is not 1 then the comprehensive test and
demo application will be built.  The comprehensive test and demo application is
implemented and described in main_full.c. */
#define mainCREATE_SIMPLE_BLINKY_DEMO_ONLY 1

/* This demo uses heap_5.c, and these constants define the sizes of the regions
that make up the total heap.  heap_5 is only used for test and example purposes
as this demo could easily create one large heap region instead of multiple
smaller heap regions - in which case heap_4.c would be the more appropriate
choice.  See http://www.freertos.org/a00111.html for an explanation. */
#define mainREGION_1_SIZE 8201
#define mainREGION_2_SIZE 29905
#define mainREGION_3_SIZE 7607

/*-----------------------------------------------------------*/

/*
 * Only the comprehensive demo uses application hook (callback) functions.  See
 * http://www.freertos.org/a00016.html for more information.
 */
void vFullDemoTickHookFunction(void);
void vFullDemoIdleFunction(void);

/*
 * This demo uses heap_5.c, so start by defining some heap regions.  It is not
 * necessary for this demo to use heap_5, as it could define one large heap
 * region.  Heap_5 is only used for test and example purposes.  See
 * http://www.freertos.org/a00111.html for an explanation.
 */
static void prvInitialiseHeap(void);

/*
 * Prototypes for the standard FreeRTOS application hook (callback) functions
 * implemented within this file.  See http://www.freertos.org/a00016.html .
 */
void vApplicationMallocFailedHook(void);
void vApplicationIdleHook(void);
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName);
void vApplicationTickHook(void);
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize);

/*
 * Writes trace data to a disk file when the trace recording is stopped.
 * This function will simply overwrite any trace files that already exist.
 */
static void prvSaveTraceFile(void);

/*-----------------------------------------------------------*/

/* When configSUPPORT_STATIC_ALLOCATION is set to 1 the application writer can
use a callback function to optionally provide the memory required by the idle
and timer tasks.  This is the stack that will be used by the timer task.  It is
declared here, as a global, so it can be checked by a test that is implemented
in a different file. */
StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

/* Notes if the trace is running or not. */
static BaseType_t xTraceRunning = pdTRUE;
