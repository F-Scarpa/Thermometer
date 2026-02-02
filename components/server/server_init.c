#include "server_init.h"
#include "esp_log.h"
#include "urls.h"


httpd_handle_t server = NULL;   //server handle






void init_server()     // can be used to store paths
{
 
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();   //working configuration of our server
  config.uri_match_fn = httpd_uri_match_wildcard;   //enables the use of wildcards in routes, (/*)
  ESP_ERROR_CHECK(httpd_start(&server, &config));   //start the server

  //websocket
  httpd_uri_t web_socket_url = {
    .uri = "/ws",         
    .method = HTTP_GET,
    .handler = on_WEB_SOCKET_url,
    .is_websocket = true              //for websockets only
    };
  httpd_register_uri_handler(server, &web_socket_url);




  //endpoints
  httpd_uri_t disable_mode_url = {
      .uri = "/api/disable",         
      .method = HTTP_GET,
      .handler = on_disable_mode_url    
    };
  httpd_register_uri_handler(server, &disable_mode_url);    

    httpd_uri_t auto_mode_url = {
      .uri = "/api/auto",         
      .method = HTTP_GET,
      .handler = on_auto_mode_url    
    };
  httpd_register_uri_handler(server, &auto_mode_url);   

      httpd_uri_t error_mode_url = {
      .uri = "/api/error",         
      .method = HTTP_GET,
      .handler = on_error_mode_url    
    };
  httpd_register_uri_handler(server, &error_mode_url);  

    httpd_uri_t pedestrian_call_mode_url = {
      .uri = "/api/pedestrian-call",         
      .method = HTTP_GET,
      .handler = on_pedestrian_call_mode_url    
    };
  httpd_register_uri_handler(server, &pedestrian_call_mode_url);    

    httpd_uri_t text_url = {
      .uri = "/api/text",         
      .method = HTTP_GET,
      .handler = on_text_url    
    };
  httpd_register_uri_handler(server, &text_url);  

  

  httpd_uri_t default_url = {   
    .uri ="/*",                  
    .method = HTTP_GET,
    .handler = on_default_url   
                                
  };
  httpd_register_uri_handler(server,&default_url);    
}