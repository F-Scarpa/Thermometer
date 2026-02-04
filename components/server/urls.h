#pragma once
#include "esp_http_server.h" 
#include "pinsSetup.h"

extern httpd_handle_t server;
extern QueueHandle_t motor_c_data;

esp_err_t on_default_url(httpd_req_t *req);

//extern int mode;
esp_err_t on_disable_mode_url(httpd_req_t *req);


esp_err_t on_WEB_SOCKET_url(httpd_req_t *req);
esp_err_t send_ws_message(char* message);


typedef struct      //struct to send to queue
{
  int motor_mode;
} HttpCommand_t;

