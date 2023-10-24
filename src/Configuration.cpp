#include "Configuration.hpp"

static const char* TAG = __FILE__;

extern String cpuId;
extern Configuration Config;


/**
 * @brief Construct a new Configuration:: Configuration object
 * 
 */
Configuration::Configuration(void)
{

}

/**
 * @brief Destroy the Configuration:: Configuration object
 * 
 */
Configuration::~Configuration(void) 
{

}

/**
 * @brief Получение - настроек сети
 * 
 */
void Configuration::get_wlan_cfg(void)
{    
    if(!LITTLEFS.exists(FILE_WLAN))
    {
        default_wlan_cfg();
    } 

    File file = LITTLEFS.open(FILE_WLAN,  FILE_READ);

    StaticJsonBuffer<256> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(file);
  
    if (!root.success()) 
    {
        default_wlan_cfg();
    }
    else
    {
        _wlan.sta_ssid          = root["sta_ssid"].as<String>();
        _wlan.sta_password      = root["sta_password"].as<String>();
    }

    file.close();
}

/**
 * @brief Установка - настроек сети
 * 
 */
void Configuration::set_wlan_cfg(void)
{
    if(LITTLEFS.remove(FILE_WLAN)) 
    {
        ESP_LOGD (TAG, "%s - file deleted", FILE_WLAN);
    } 
    else 
    {
        ESP_LOGE (TAG, "%s - delete failed", FILE_WLAN);
    }

    File file = LITTLEFS.open(FILE_WLAN, FILE_WRITE);
    
    if(!file || file.isDirectory())
    {
        ESP_LOGE (TAG, "Failed to create file -  %s", FILE_WLAN);
        return;
    }
  
    StaticJsonBuffer<256> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
  
    root["sta_ssid"]        = _wlan.sta_ssid;
    root["sta_password"]    = _wlan.sta_password;

    if (root.printTo(file) == 0)
    {
        ESP_LOGE (TAG, "Failed to write to file");
    }

    file.close();
}

/**
 * @brief Установка дефолтных значений - настроек сети
 * 
 */
void Configuration::default_wlan_cfg(void)
{
    if(LITTLEFS.remove(FILE_WLAN)) 
    {
        ESP_LOGD (TAG, "%s - file deleted", FILE_WLAN);
    } 
    else 
    {
        ESP_LOGE (TAG, "%s - delete failed", FILE_WLAN);
    }

    File file = LITTLEFS.open(FILE_WLAN, FILE_WRITE);
    
    if(!file || file.isDirectory())
    {
        ESP_LOGE (TAG, "Failed to create file -  %s", FILE_WLAN);
        return;
    }
  
    StaticJsonBuffer<256> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
  
    _wlan.sta_ssid          = "";
    _wlan.sta_password      = "";

    root["sta_ssid"]        = _wlan.sta_ssid;
    root["sta_password"]    = _wlan.sta_password;

    if (root.printTo(file) == 0)
    {
        ESP_LOGE (TAG, "Failed to write to file");
    }

    file.close();
}

/**
 * @brief 
 * 
 */
void Configuration::restore_all()
{     
    default_wlan_cfg();
    delay(50);
}

/**
 * @brief 
 * 
 * @param fs 
 * @param dirname 
 * @param levels 
 */
void Configuration::listDir(fs::FS &fs, const char * dirname, uint8_t levels)
{
    File root = fs.open(dirname);

    if(!root)
    {
        ESP_LOGV (TAG, "%s - failed to open directory", dirname);   
        return;
    }

    if(!root.isDirectory())
    {
        ESP_LOGV (TAG, "%s - not a directory", dirname);   
        return;
    }

    File file = root.openNextFile();

    while(file)
    {
        if(!file.isDirectory())
        {
            ESP_LOGV (TAG, "FILE: %s, SIZE: %d", file.name(), file.size());           
        }

        file = root.openNextFile();
    }
}
