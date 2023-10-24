
#ifndef FUNCTIONS_CONFIG_H_
#define FUNCTIONS_CONFIG_H_


#include "main.hpp"

#include "ArduinoJson.h"
#include "FS.h"
#include "LITTLEFS.h"


/**
 * @brief 
 * 
 */
struct Config_WLAN
{   
    String sta_ssid         = DEFAULT_SSID;
    String sta_password     = DEFAULT_PASS;
};


/**
 * @brief 
 * 
 */
class Configuration 
{
    public:

    Config_WLAN _wlan;
       
    /**
     * @brief 
     * 
     */
    Configuration(void);

    /**
     * @brief Destroy the Configuration object
     * 
     */
    virtual ~Configuration(void);

    /**
     * @brief Get the configs object
     * 
     */
    void get_configs(void);

    /**
     * @brief Получение - настроек сети
     * 
     */
    void get_wlan_cfg(void);

    /**
     * @brief Установка - настроек сети
     * 
     */
    void set_wlan_cfg(void);

    /**
     * @brief Установка дефолтных значений - настроек сети
     * 
     */
    void default_wlan_cfg(void);

    /**
     * @brief 
     * 
     */
    void restore_all(void);

    /**
     * @brief 
     * 
     * @param fs 
     * @param dirname 
     * @param levels 
     */
    void listDir(fs::FS &fs, const char * dirname, uint8_t levels);

    /**
     * @brief 
     * 
     * @param fs 
     * @param path 
     */
    void readFile(fs::FS &fs, const char * path);
};


#endif 