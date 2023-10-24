#include "Network.hpp"

static const char *TAG = __FILE__;

extern String cpuId;
extern Configuration Config;


/**
 * @brief 
 * 
 */
void wlan_init(void)
{
    if(Config._wlan.sta_ssid != "")
    {
        ESP_LOGI(TAG, "WiFi connecting . . .   .");

        WiFi.mode(WIFI_STA);
        WiFi.begin(Config._wlan.sta_ssid.c_str(), Config._wlan.sta_password.c_str());

        unsigned long timeconnect = millis();
                    
        while(millis() <= timeconnect + 10000)
        {                           
            if(WiFi.status() == WL_CONNECTED)
            {   
                // ESP_LOGI(TAG, "IP address: %s", String(WiFi.localIP()));
                break;
            }
            else
            {
                delay(200);
            }                
        }

        if(WiFi.status() != WL_CONNECTED)
        {
            ESP_LOGI(TAG, "WiFi not connected!!!");
            ESP.restart();
        }

        WiFi.setAutoReconnect(true);
    }

    ESP_LOGV (TAG, "----------------------------------------------------------------------------");
}
