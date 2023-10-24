#include "TaskProgress.hpp"

static const char *TAG = __FILE__;

extern String cpuId;
extern Configuration Config;

int input_pins[3] = {INPUT_PIN_A, INPUT_PIN_B, INPUT_PIN_C};
int output_pins[8] = {OUT_PIN_A, OUT_PIN_B, OUT_PIN_C, OUT_PIN_D, OUT_PIN_E, OUT_PIN_F, OUT_PIN_G, OUT_PIN_H};

bool states_inputs[3] = {false, false, false};
bool states_outputs[8] = {false, false, false, false, false, false, false, false};

bool state_progress = false;

/**
 * @brief
 *
 */
void write()
{
    for (int i = 0; i < 8; i++)
    {
        // ESP_LOGV(TAG, "Num pin output = %d", output_pins[i]);

        digitalWrite(output_pins[i], states_outputs[i]);
        vTaskDelay(2);
    }
}

/**
 * @brief
 *
 */
void read()
{
    for (int i = 0; i < 3; i++)
    {
        // ESP_LOGV(TAG, "Num pin input = %d", input_pins[i]);

        states_inputs[i] = digitalRead(input_pins[i]);
        vTaskDelay(2);
    }
}

/**
 * @brief
 *
 */
void clear()
{
    for (int i = 0; i < 8; i++)
    {
        states_outputs[i] = false;
    }
}

/**
 * @brief
 *
 * @param pvParameters
 */
void TaskProgress(void *pvParameters)
{
    (void)pvParameters;

    ESP_LOGV(TAG, "%s", "TaskProgress start");

    for (int i = 0; i < 3; i++)
    {
        pinMode(input_pins[i], INPUT);
    }

    for (int i = 0; i < 8; i++)
    {
        pinMode(output_pins[i], OUTPUT);
        digitalWrite(output_pins[i], LOW);
    }

    for (;;)
    {
        try
        {
            if (state_progress)
            {
                unsigned long time_run = millis();

                ESP_LOGV(TAG, "### Start command new state outputs");

                write();

                while (millis() < time_run + TIME_PROGRESS)
                {
                    read();
                }

                clear();
                write();

                ESP_LOGV(TAG, "### Stop command new state outputs");

                state_progress = false;
            }
        }
        catch (const std::exception &e)
        {
            ESP_LOGE(TAG, "%s", "Crash TaskProgress");
        }

        vTaskDelay(10);
    }

    ESP_LOGE(TAG, "%s", "TaskProgress end");
    vTaskDelete(NULL);
}
