#include <stdio.h>
#include <devices.h>
#include <task.h>

#include "opcodes.h"
#include "test.h"

void vTask() {
    printf(" Task created\n");
    //while (1) {
        RunTest(ONE_TEST, iadd);
        vTaskDelay(2000 / portTICK_RATE_MS);
    //}
}


int main()
{
    getchar();
    xTaskCreate(vTask, "vTask", 20480, NULL, 3, NULL);

    return 0;
}


