#include "TaskWebserver.hpp"

static const char *TAG = __FILE__;

extern String cpuId;
extern Configuration Config;

extern bool states_inputs[3];
extern bool states_outputs[8];

extern bool state_progress;

WebServer server(80);


/**
 * @brief 
 * 
 */
void handleRoot() 
{
    server.send(200, "text/plain", "OK");
}

/**
 * @brief 
 * 
 */
void handleGet() 
{
    String message = "";

    message += states_inputs[0];
    message += states_inputs[1];
    message += states_inputs[2];

    server.send(200, "text/plain", message);
}

/**
 * @brief 
 * 
 */
void handleSet() 
{   
    String message = server.arg("data");

    if(message.length() == 8)
    {
        for (int i = 0; i < 8; i++)
        {
            states_outputs[i] = message.substring(i, i + 1).toInt();
        }

        state_progress = true;
        server.send(200, "text/plain", "OK");
    }
    else
    {
        server.send(500, "text/plain", "Error request\n\n");
    }
}

/**
 * @brief 
 * 
 */
void handleNotFound() 
{
    String message = "Not Found\n\n";

    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";

    for (uint8_t i = 0; i < server.args(); i++) 
    {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    
    server.send(404, "text/plain", message);
}

/**
 * @brief
 *
 * @param pvParameters
 */
void TaskWebserver(void *pvParameters)
{
    (void)pvParameters;

    ESP_LOGV(TAG, "%s", "TaskWebserver start");
    ESP_LOGV(TAG, "%s", "Init WebServer");

    server.on("/", handleRoot);
    server.on("/get", handleGet);
    server.on("/set", handleSet);
    server.onNotFound(handleNotFound);
    server.begin();

    ESP_LOGV(TAG, "%s", "WebServer started");

    for (;;)
    {
        try
        {
            server.handleClient();
        }
        catch (const std::exception &e)
        {
            ESP_LOGE(TAG, "%s", "Crash TaskWebserver");
        }

        vTaskDelay(5);
    }

    ESP_LOGE(TAG, "%s", "TaskWebserver end");
    vTaskDelete(NULL);
}
