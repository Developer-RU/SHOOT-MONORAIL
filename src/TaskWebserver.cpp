#include "TaskWebserver.hpp"

static const char *TAG = __FILE__;

extern String cpuId;
extern Configuration Config;

extern bool states_inputs[3];
extern bool states_outputs[8];


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
    String response = "";

    response += states_inputs[0];
    response += states_inputs[1];
    response += states_inputs[2];

    server.send(200, "text/plain", "{\"data\":" + response + "}");
}

/**
 * @brief 
 * 
 */
void handleSet() 
{   
    String request = server.arg("data");

    if(request.length() == 8)
    {
        for (int i = 0; i < 8; i++)
        {
            states_outputs[i] = request.substring(i, i + 1).toInt();
        }
            
        server.send(200, "text/plain", "OK");        
    }
    else
    {
        server.send(500, "text/plain", "ER");
    }
}

/**
 * @brief 
 * 
 */
void handleNotFound() 
{
    String message = "File Not Found\n\n";

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
