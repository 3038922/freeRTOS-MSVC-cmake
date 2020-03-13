#ifndef FREERTOS_MAIN_H
#define FREERTOS_MAIN_H
/* Standard includes. */
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#include <intrin.h>

/* Kernel includes. */
#include "./freeRTOS/FreeRTOS.h"
#include "./freeRTOS/queue.h"
#include "./freeRTOS/semphr.h"
#include "./freeRTOS/task.h"
#include "./freeRTOS/timers.h"

#define mainCREATE_SIMPLE_BLINKY_DEMO_ONLY 1

#define mainREGION_1_SIZE 8201
#define mainREGION_2_SIZE 29905
#define mainREGION_3_SIZE 7607

/*-----------------------------------------------------------*/
/*
 * main_blinky() is used when mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is set to 1.
 * main_full() is used when mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is set to 0.
 */
extern void main_blinky(void);
__attribute__((unused)) static void prvInitialiseHeap()
{
    /* The Windows demo could create one large heap region, in which case it would
be appropriate to use heap_4.  However, purely for demonstration purposes,
heap_5 is used instead, so start by defining some heap regions.  No
initialisation is required when any other heap implementation is used.  See
http://www.freertos.org/a00111.html for more information.

The xHeapRegions structure requires the regions to be defined in start address
order, so this just creates one big array, then populates the structure with
offsets into the array - with gaps in between and messy alignment just for test
purposes. */
    static uint8_t ucHeap[configTOTAL_HEAP_SIZE];
    volatile uint32_t ulAdditionalOffset = 19; /* Just to prevent 'condition is always true' warnings in configASSERT(). */
    const HeapRegion_t xHeapRegions[] =
        {
            /* Start address with dummy offsets						Size */
            {ucHeap + 1, mainREGION_1_SIZE},
            {ucHeap + 15 + mainREGION_1_SIZE, mainREGION_2_SIZE},
            {ucHeap + 19 + mainREGION_1_SIZE + mainREGION_2_SIZE, mainREGION_3_SIZE},
            {NULL, 0}};

    /* Sanity check that the sizes and offsets defined actually fit into the
	array. */
    configASSERT((ulAdditionalOffset + mainREGION_1_SIZE + mainREGION_2_SIZE + mainREGION_3_SIZE) < configTOTAL_HEAP_SIZE);

    /* Prevent compiler warnings when configASSERT() is not defined. */
    (void)ulAdditionalOffset;

    vPortDefineHeapRegions(xHeapRegions);
}
#endif