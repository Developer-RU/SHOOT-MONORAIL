#include "main.hpp"

static const char* TAG = __FILE__;

String cpuId = "";
Configuration Config;

extern bool states_inputs[3];
extern bool states_outputs[8];

extern bool state_progress;


/**
 * @brief Get the cpuID object
 * 
 */
void get_cpuID(void)
{
    uint64_t chipid = ESP.getEfuseMac();
    cpuId = String((uint16_t)(chipid>>32)) + String((uint32_t)chipid);  
}

/**
 * @brief
 *
 */
void setup()
{
    get_cpuID();

    ESP_LOGV (TAG, "----------------------------------------------------------------------------");
    ESP_LOGI (TAG, "FIRMWARE NAME   - : %s", FIRMWARE_NAME);
    ESP_LOGI (TAG, "SERIAL NUMBER   - : %s", cpuId.c_str());
    ESP_LOGI (TAG, "VERSION NAME    - : %s", VERSION);
    ESP_LOGI (TAG, "REVISION        - : %s", REVISION);
    ESP_LOGI (TAG, "MANUFACTURE     - : %s", MANUFACTURE);
    ESP_LOGI (TAG, "MODEL           - : %s", MODEL);
    ESP_LOGV (TAG, "----------------------------------------------------------------------------");

    ESP_LOGD (TAG, "%s", "LittleFS Mount start" );

    if(!LITTLEFS.begin(true))
    {
        ESP_LOGE (TAG, "LittleFS Mount failed");
        return;
    }
    else
    {
        ESP_LOGD (TAG, "LittleFS Mount conplete");
    }

    ESP_LOGD (TAG, "%s", "LittleFS Load config" );

    // Config.get_configs();

    ESP_LOGV (TAG, "----------------------------------------------------------------------------");
    ESP_LOGV (TAG, "Files configurations from Flash Ram");
    Config.listDir(LITTLEFS, "/", 3);
    ESP_LOGV (TAG, "----------------------------------------------------------------------------");

    wlan_init();

    xTaskCreate(TaskProgress,  "TaskProgress",  STACK_SIZE_PROGRESS,  NULL, 1, NULL);
    xTaskCreate(TaskWebserver, "TaskWebserver", STACK_SIZE_WEBSERVER, NULL, 1, NULL);
}

/**
 * @brief
 *
 */
void loop()
{
    if(WiFi.status() != WL_CONNECTED)
    {
        // Reconnect WiFi
        ESP_LOGI(TAG, "WiFi not connected, reconnecting");
    }

    ESP_LOGV(TAG, "INPUTS: %d%d%d, OUTPUTS: %d%d%d%d%d%d%d%d", states_inputs[0], states_inputs[1], states_inputs[2], states_outputs[0], states_outputs[1],  states_outputs[2], states_outputs[3], states_outputs[4], states_outputs[5], states_outputs[6], states_outputs[7]);
    delay(1000);
}