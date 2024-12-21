/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_task_wdt.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_http_server.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "www_page.h"
#include "cJSON.h"

static const char * TAG = "MAIN";

char ap_ssid[32] = "NETIASPOT-YPj9";
char ap_password[64] = "p86AW8stvjPm8";
static bool power_state = false;

static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) 
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) 
    {
        esp_wifi_connect();
    } 
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) 
    {
        esp_wifi_connect();
    } 
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) 
    {
        printf("Połączono z siecią WiFi!\n");
        esp_netif_ip_info_t ip_info;
        esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("WIFI_STA_DEF"), &ip_info);
        printf("Adres IP: " IPSTR "\n", IP2STR(&ip_info.ip));
    }
}

void wifi_init() 
{
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    wifi_config_t wifi_config = 
    {
        .sta = 
        {
            .ssid = "NETIASPOT-YPj9",          
            .password = "p86AW8stvjPm8"     
        },
    };

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }

    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    esp_wifi_init(&cfg);
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, NULL);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    esp_wifi_start();
}

// Handler dla zmiany stanu przycisku (zachowany z poprzedniego kodu)
static esp_err_t power_handler(httpd_req_t *req) 
{
    char content[100];
    int len = httpd_req_recv(req, content, sizeof(content));
    if (len <= 0) return ESP_FAIL;

    content[len] = '\0'; // Null-terminate the received string

    // Parsowanie JSON
    cJSON *json = cJSON_Parse(content);
    if (!json) return ESP_FAIL;

    const cJSON *power = cJSON_GetObjectItem(json, "power");
    if (power && strcmp(power->valuestring, "ON") == 0) {
        power_state = true;
    } else if (power && strcmp(power->valuestring, "OFF") == 0) {
        power_state = false;
    }

    cJSON_Delete(json);

    // Odpowiedź z aktualnym stanem
    char response[32];
    snprintf(response, sizeof(response), "{\"power\": \"%s\"}", power_state ? "ON" : "OFF");
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// Handler dla pobierania aktualnego stanu
static esp_err_t state_handler(httpd_req_t *req) 
{
    char response[32];
    snprintf(response, sizeof(response), "{\"power\": \"%s\"}", power_state ? "ON" : "OFF");
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

void start_server(void) 
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t state_endpoint = {
            .uri      = "/state",
            .method   = HTTP_GET,
            .handler  = state_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &state_endpoint);

        httpd_uri_t power_endpoint = {
            .uri      = "/power",
            .method   = HTTP_POST,
            .handler  = power_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &power_endpoint);
    }
}

void app_main(void)
{
    esp_task_wdt_deinit();
    
    wifi_init();      
    start_server();   

    for (;;);
}
