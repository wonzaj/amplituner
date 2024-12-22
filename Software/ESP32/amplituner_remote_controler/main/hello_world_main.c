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
#include "html/www_page.h"
#include "cJSON.h"

extern const uint8_t roundslider_css_start[] asm("_binary_roundslider_css_start");
extern const uint8_t roundslider_css_end[] asm("_binary_roundslider_css_end");

extern const uint8_t roundslider_js_start[] asm("_binary_roundslider_js_start");
extern const uint8_t roundslider_js_end[] asm("_binary_roundslider_js_end");

static const char *TAG = "AMP_CONTROLLER";

char ap_ssid[32] = "NETIASPOT-YPj9";
char ap_password[64] = "p86AW8stvjPm8";
static bool power_state = false;
static int volume_level = 50; 


static esp_err_t index_handler(httpd_req_t *req) 
{   
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, index_html, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t state_handler(httpd_req_t *req) 
{   
    char response[64];
    snprintf(response, sizeof(response), "{\"power\": \"%s\", \"volume\": %d}", power_state ? "ON" : "OFF", volume_level);
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t power_handler(httpd_req_t *req) 
{
    char content[100];
    char response[32];
    int len = httpd_req_recv(req, content, sizeof(content));
    if (len <= 0) return ESP_FAIL;

    content[len] = '\0';
    cJSON *json = cJSON_Parse(content);
    if (!json) return ESP_FAIL;

    const cJSON *power = cJSON_GetObjectItem(json, "power");
    if (power && strcmp(power->valuestring, "ON") == 0) 
    {
        power_state = true;
    } else if (power && strcmp(power->valuestring, "OFF") == 0) 
    {
        power_state = false;
    }

    cJSON_Delete(json);
    snprintf(response, sizeof(response), "{\"power\": \"%s\"}", power_state ? "ON" : "OFF");
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t volume_handler(httpd_req_t *req) 
{   
    char buf[32];
    int len = httpd_req_recv(req, buf, sizeof(buf));
    if (len <= 0) {return ESP_FAIL;}

    buf[len] = '\0'; 
    cJSON *json = cJSON_Parse(buf);
    if (!json) {return ESP_FAIL;}

    cJSON *volume_json = cJSON_GetObjectItem(json, "volume");
    if (cJSON_IsNumber(volume_json)) 
    {
        volume_level = volume_json->valueint;
        ESP_LOGI(TAG, "Set volume to: %d", volume_level);
    }

    cJSON_Delete(json);

    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, "{\"status\": \"ok\"}", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t serve_css(httpd_req_t *req) 
{
    const size_t css_size = roundslider_css_end - roundslider_css_start;
    httpd_resp_set_type(req, "text/css");
    return httpd_resp_send(req, (const char *)roundslider_css_start, css_size);
}

static esp_err_t serve_js(httpd_req_t *req) 
{
    const size_t js_size = roundslider_js_end - roundslider_js_start;
    httpd_resp_set_type(req, "application/javascript");
    return httpd_resp_send(req, (const char *)roundslider_js_start, js_size);
}

void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) 
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
        printf("Connected to WiFi!\n");
        esp_netif_ip_info_t ip_info;
        esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("WIFI_STA_DEF"), &ip_info);
        printf("Adress IP: " IPSTR "\n", IP2STR(&ip_info.ip));
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

void start_webserver(void) 
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) 
    {
        httpd_uri_t index_endpoint  = {.uri = "/", .method   = HTTP_GET, .handler  = index_handler, .user_ctx = NULL };
        httpd_uri_t state_endpoint  = {.uri = "/state", .method = HTTP_GET, .handler  = state_handler, .user_ctx = NULL };
        httpd_uri_t power_endpoint  = {.uri = "/power", .method = HTTP_POST, .handler = power_handler, .user_ctx = NULL };
        httpd_uri_t volume_endpoint = {.uri = "/volume", .method = HTTP_POST, .handler  = volume_handler, .user_ctx = NULL };
        httpd_uri_t uri_css         = {.uri = "/roundslider.css", .method = HTTP_GET, .handler = serve_css, .user_ctx = NULL };
        httpd_uri_t uri_js          = {.uri = "/roundslider.js", .method = HTTP_GET, .handler = serve_js, .user_ctx = NULL };

        httpd_register_uri_handler(server, &index_endpoint);
        httpd_register_uri_handler(server, &state_endpoint);
        httpd_register_uri_handler(server, &power_endpoint);
        httpd_register_uri_handler(server, &volume_endpoint);
        httpd_register_uri_handler(server, &uri_css);
        httpd_register_uri_handler(server, &uri_js);
    }
}

void app_main(void)
{
    esp_task_wdt_deinit();
    
    wifi_init();      
    start_webserver();   

    for (;;);
}
