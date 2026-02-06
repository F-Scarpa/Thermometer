#include "urls.h"
#include "esp_log.h"
#include <string.h>
#include <stdio.h>
#include "freertos/semphr.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"   
//#include "dht_read.h"


static const char *TAG = "SERVER";
extern uint16_t high_temp_threshold;

//extern TaskHandle_t dht_test_handle;

//------------------------websocket------------------------//
#define WS_MAX_SIZE 1024
static int client_session_id;
esp_err_t send_ws_message(char* message)
{
  //error if websocket isn't connected on the frontend
  if(!client_session_id)
  {
    ESP_LOGE(TAG, "no client id");
    return -1;
  }
  //message config
  httpd_ws_frame_t ws_message = {
    .final = true,
    .fragmented = false,
    .len = strlen(message),
    .payload = (uint8_t *)message,
    .type = HTTPD_WS_TYPE_TEXT

  };
  httpd_ws_send_frame_async(server, client_session_id, &ws_message);
  return 1;
}

//websocket url callback
esp_err_t on_WEB_SOCKET_url(httpd_req_t *req)
{
  client_session_id = httpd_req_to_sockfd(req);
  if(req->method == HTTP_GET) return ESP_OK;
  
  httpd_ws_frame_t ws_pkt;
  memset(&ws_pkt,0,sizeof(httpd_ws_frame_t)); //reset/clean memory on ws.pkt address
  ws_pkt.type = HTTPD_WS_TYPE_TEXT;
  ws_pkt.payload = malloc(WS_MAX_SIZE);         //create our own heap memory
  httpd_ws_recv_frame(req,&ws_pkt,WS_MAX_SIZE);   //copy memory 
  printf("ws payload: %.*s\n", ws_pkt.len, ws_pkt.payload);     //1. length to print 2. start index pointer 
  free(ws_pkt.payload);   //free memory

  char* response = "{\"status\":\"connected OK\"}";   //response to a successfully connected websocket
  httpd_ws_frame_t ws_response = {
    .final = true,
    .fragmented = false,
    .type = HTTPD_WS_TYPE_TEXT,
    .payload = (uint8_t *)response,
    .len = strlen(response), 
  };
  return httpd_ws_send_frame(req, &ws_response);
  
}
//------------------------websocket------------------------//


//-----------------------default url-----------------------//
esp_err_t on_default_url(httpd_req_t *req)    
{                                                     //http home url page
    ESP_LOGI(TAG,"URL: %s",req->uri);

    char path[600];
    sprintf(path, "/store%s", req->uri);      //write a string inside a variable

    //******************    setup mimes    *******************************


    char* ext = strrchr(req -> uri, '.');
    if(ext)
    {
      if(strcmp(ext,".css") == 0) httpd_resp_set_type(req,"text/css");
      if(strcmp(ext,".js") == 0) httpd_resp_set_type(req,"text/javascript");
      if(strcmp(ext,".png") == 0) httpd_resp_set_type(req,"image/png");
      if(strcmp(ext,".jpg") == 0) httpd_resp_set_type(req,"image/jpg");
      if(strcmp(ext,".svg") == 0) httpd_resp_set_type(req,"image/svg+xml");
    };

    //******************************************************************** 
    FILE *file = fopen(path, "r"); 
    //if path does not exist                   
    if(file == NULL)
    {
      file = fopen("/store/index.html", "r");    //open index page
      if(file == NULL)
      {
        httpd_resp_send_404(req);   //send error 404
      }
      ESP_LOGI(TAG, "STORE/INDEX.HTML NOT FOUND");
    }
    char buffer[1024];
    int bytes_read = 0;
    while((bytes_read = fread(buffer, sizeof(char), sizeof(buffer),file)) > 0)   //read the file, end file
                                                                                 //when byres_read is 0
    {
      httpd_resp_send_chunk(req, buffer, bytes_read); //send response while buffer is filled with file data
    }
    fclose(file);   //close file
    httpd_resp_send_chunk(req, NULL, 0); //empty http data chunk

    
    return ESP_OK;
}

//-----------------------default url-----------------------//

//what happen when on_disable_mode_url is visited/called

esp_err_t on_disable_mode_url(httpd_req_t *req)
 {
  HttpCommand_t cmd;    //extract struct created in header file

  cmd.motor_mode = 0; //modify value
  BaseType_t ok = xQueueSend( motor_c_data, &cmd, 1000/portTICK_PERIOD_MS); //send data in queue (&cmd), the whole struct

  //ok respond as pdTrue or pdFalse
  if(ok == pdTRUE){
    printf("disable mode queued\n");
  }
  else{
    printf("Failed to queue disable mode\n");
  }
  
  httpd_resp_set_status(req,"204 NO CONTENT");      //set http status
  httpd_resp_send(req,NULL,0);                      //send http status
  return ESP_OK;
 }



//auto mode
 esp_err_t on_auto_mode_url(httpd_req_t *req)
 {
  HttpCommand_t cmd;    //extract struct created in header file

  cmd.motor_mode = 1; //modify value
  BaseType_t ok = xQueueSend( motor_c_data, &cmd, 1000/portTICK_PERIOD_MS); //send data in queue (&cmd), the whole struct

  //ok respond as pdTrue or pdFalse
  if(ok == pdTRUE){
    printf("auto mode queued\n");
  }
  else{
    printf("failed to queue auto mode\n");
  }
  
  httpd_resp_set_status(req,"204 NO CONTENT");      //set http status
  httpd_resp_send(req,NULL,0);                      //send http status
  return ESP_OK;
 }

 esp_err_t on_set_threshold_mode_url(httpd_req_t *req)
 {
    char buf[100];
    memset(&buf,0, sizeof(buf));                         //clean buffer
    int ret = httpd_req_recv(req, buf, req->content_len);   //write inside buffer and save it to a int variable
    if (ret > 0) {
        int new_threshold = atoi(buf);
        high_temp_threshold = new_threshold;  
        printf("new high threshold : %d\n",high_temp_threshold);
/*
        if (dht_test_handle != NULL)
        {
            xTaskNotify(dht_test_handle,DHT_CMD_UPDATE | DHT_CMD_RESET,eSetBits);   //send update and reset to dht_test_handle's task (dht_read)
                                                                                    //2. data to send = DHT_CMD_UPDATE | DHT_CMD_RESET = 00000011
        }*/
        
        httpd_resp_send(req, "OK", 2);

    }

    return ESP_OK;
 }


 //man mode
  esp_err_t on_man_mode_url(httpd_req_t *req)
 {
  HttpCommand_t cmd;    //extract struct created in header file

  cmd.motor_mode = 2; //modify value
  BaseType_t ok = xQueueSend( motor_c_data, &cmd, 1000/portTICK_PERIOD_MS); //send data in queue (&cmd), the whole struct

  //ok respond as pdTrue or pdFalse
  if(ok == pdTRUE){
    printf("man mode queued\n");
  }
  else{
    printf("failed to queue man mode\n");
  }
  
  httpd_resp_set_status(req,"204 NO CONTENT");      //set http status
  httpd_resp_send(req,NULL,0);                      //send http status
  return ESP_OK;
 }

 

