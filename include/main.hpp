#ifndef MAIN_H_
#define MAIN_H_

#include "esp32-hal-log.h"

// #include <iostream>

#include "Arduino.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <WiFi.h>
#include <WebServer.h>

#include "AppConfig.hpp"
#include "Network.hpp"
#include "Configuration.hpp"

#include "TaskWebserver.hpp"
#include "TaskProgress.hpp"


/**
 * @brief Get the cpuID object
 * 
 */
void get_cpuID(void);

/**
 * @brief
 *
 */
void setup(void);

/**
 * @brief
 *
 */
void loop(void);


#endif 