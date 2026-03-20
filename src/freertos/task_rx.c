#include "freertos/task_radio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "drivers/radio/peripheral_lora1280.h"
#include "drivers/radio/radio_hal.h"
#include <string.h>
#include <stdio.h>
#include "core/message_queue.h"
#include "config.h"
#include "debug.h"
#include "task_rx.h"

#define RX_BUFFER_SIZE 255
#define RX_BUFFER_LENGTH 255

TaskHandle_t rxTaskHandle = NULL;

void rx_task(void *params) {
    printf("[RX] Starting RX task...\n");
    //static uint8_t rx_buf[RX_BUFFER_SIZE];
    //static uint16_t rx_len = 0;

    //memset(rx_buf, 0, sizeof(rx_buf));
    //lora1280_start_receive(rx_buf, &rx_len);

    while (1)
    {
        SetDioIrqParams(IRQ_TX_DONE, IRQ_RX_DONE);
        sx1280_start_receive(RX_BUFFER_SIZE, RX_BUFFER_LENGTH);
        BaseType_t notified = ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(5000));
            if (notified == 0) {
                printf("[RX] Timeout - IRQ never fired\n");
            } else {
                printf("[RX] Notification received\n");
            }

        //if (lora1280_poll_receive_done())
        // {
        //     printf("[RX] Packet received (%u bytes): ", rx_len);
        //     for (uint16_t i = 0; i < rx_len; i++)
        //     {
        //         printf("%02X ", rx_buf[i]);
        //     }
        //     printf("\n");

        //     printf("[RX] ASCII: ");
        //     for (uint16_t i = 0; i < rx_len; i++)
        //     {
        //         char c = (char)rx_buf[i];
        //         printf("%c", (c >= 32 && c <= 126) ? c : '.');
        //     }
        //     printf("\n");
        // }
        printf("[RX] Notification received \n");
    }
}

void launch_rx_task(void) {
    xTaskCreate(
        rx_task,
        "RX",
        512,
        NULL,
        tskIDLE_PRIORITY,
        &rxTaskHandle
    );
}