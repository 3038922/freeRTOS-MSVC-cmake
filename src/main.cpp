extern "C" {
#include "main.h"
}

TaskHandle_t xHandle1 = NULL;
TaskHandle_t xHandle2 = NULL;
void *a = xHandle1;
void *b = xHandle2;
/*-----------------------------------------------------------*/
void test(const char *str)
{
    int flag = 0;
    while (1)
    {
        flag++;
        printf("[%u]ms %s:%d\n", xTaskGetTickCount(), str, flag);
        vTaskDelay(500);
    }
}
void task1(void *para)
{
    test("i am task1");
    vTaskDelete(NULL);
}
void task2(void *para)
{
    test("i am task2");
    vTaskDelete(NULL);
}
void task3(void *para)
{
    while (1)
    {
        if (xTaskGetTickCount() >= 5000)
        {
            vTaskDelete((TaskHandle_t)a);
            xTaskCreate(task2, "task2", 512, NULL, 1, (TaskHandle_t *)b);
            break;
        }
    }
    vTaskDelete(NULL);
}

int main(void)
{
    prvInitialiseHeap();
    vTraceEnable(TRC_START);

    xTaskCreate(task1, "task1", 512, NULL, 1, (TaskHandle_t *)(&a));
    xTaskCreate(task3, "task3", 512, NULL, 1, NULL);
    vTaskStartScheduler();
    while (1)
        ;
    return 0;
}
/*-----------------------------------------------------------*/
