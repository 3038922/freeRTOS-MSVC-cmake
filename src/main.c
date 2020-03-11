#include "main.h"

/*-----------------------------------------------------------*/
void task1(void *para)
{
    int flag = 0;
    while (1)
    {
        flag++;
        printf("hello freeRTOS:%d\n", flag);
        vTaskDelay(500);
    }
    vTaskDelete(NULL);
}
int main(void)
{
    prvInitialiseHeap();
    vTraceEnable(TRC_START);
    xTaskCreate(task1, "task1", 512, NULL, 1, NULL);
    vTaskStartScheduler();
    while (1)
        ;
    return 0;
}
/*-----------------------------------------------------------*/
