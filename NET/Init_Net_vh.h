/**********************************************************************************
z* Copyright (C) 2018-2021 LASECÂ®ï¸� Telecomunicaciones S.A.P.I. de C.V.
* All rights reserved.
*
* This document is the property of LASECÂ®ï¸� Telecomunicaciones S.A.P.I. de C.V.
* It is considered confidential and proprietary.
*
* This document may not be reproduced or transmitted in any form,
* in whole or in part, without the express written permission of
* LASECÂ®ï¸� Telecomunicaciones S.A.P.I. de C.V.
*
*********************************************************************************/



#ifndef INIT_NET_VH_H
#define INIT_NET_VH_H

#include "wiced.h"
#include "string.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

#include "http_client.h"

//wiced_ip_address_t INITIALISER_IPV4_ADDRESS( ip_address, MAKE_IPV4_ADDRESS(177,241,233,66) );
//wiced_ip_address_t INITIALISER_IPV4_ADDRESS( ip_address, MAKE_IPV4_ADDRESS(10,174,109,30) );
//wiced_ip_address_t INITIALISER_IPV4_ADDRESS( ip_address, MAKE_IPV4_ADDRESS(10,90,120,10) );

//wiced_ip_address_t  ip_address;
wiced_result_t      result;
/* We need only 1 header to specify the host */
http_header_field_t header_get[1];
http_header_field_t header_post[3];

#include "API/api_manager.h"
#include "API/lasec_api.h"
bool ip_is_up = false;
wiced_result_t      result_ip= WICED_SUCCESS;

//#define JSON_MSG "[{\"EventDateFormatted\":\"24/02/2022-13:10:10\",\"LogId\":22,\"ProximityEventDevices\":[,{\"DeviceId\":\"00:00:00:00:00:00\",\"EventType\":2,\"StatusType\":3},]}]"
#define JSON_MSG  "[{\"EventDateFormatted\":\"24/02/2022-13:10:10\",\"LogId\":22,\"ProximityEventDevices\":[{\"DeviceId\":\"00:00:00:00:00:00\",\"EventType\":2,\"StatusType\":3},{\"DeviceId\":\"00:00:00:00:00:01\",\"EventType\":2,\"StatusType\":2}]}]"


//#include "manager_tcp_client_vh.h"
#include "gateway.h"
#include "wireless_config.h"
#include "Reception.h"

/* Time in milliseconds for automatic publish of weather info */
#define TIMER_TIME (30000)
#define LASEC_JOIN_RETRY_ATTEMPTS   (4000)
#define limit_logs_send             (100)

wiced_config_ap_entry_t* ap;
unsigned int             a;
int                      retries;

wiced_bool_t delete_class_socket=WICED_FALSE;
wiced_bool_t netdown=WICED_FALSE;
static wiced_timed_event_t tcp_client_event;

void collision_event_log(wiced_thread_arg_t arg);

#define LASEC_JOIN_RETRY_ATTEMPTS   (4000)

wiced_ip_setting_t device_init_ip_settings2;

wiced_config_ap_entry_t* ap;
unsigned int             a;
int                      retries;

#define LWIP_NETIF_HOSTNAME
wiced_mac_t myMac;
   wiced_tcp_socket_t socket;                      // The TCP socket
   wiced_tcp_stream_t stream;                      // The TCP stream
   wiced_result_t result;
   wiced_packet_t* tx_packet;
   uint8_t *tx_data;
   uint16_t available_data_length;
/*************** Tcp Configurator Thread ***************/
   char *tk;
   char *tken;
   char auxtk[200];



void end_point_post(){

    wiced_rtos_lock_mutex(&HTTPMutex);
        wiced_ip_address_t INITIALISER_IPV4_ADDRESS( ip_address, s1);



        read_data_collision(ANTICOLISION_ROOT,date_get(&i2c_rtc),&fs_handle);
        tk = strtok(filebuf, s);

//        while( tk != NULL ) {
        for(int f=0;f<=limit_logs_send;f++){
//            printf("><%s\n",tk);

        if(!connected_p)    {
           /* Connect to the server */
             if ( ( result = http_client_connect( &client_post, (const wiced_ip_address_t*)&ip_address, SERVER_PORT_http, HTTP_USE_TLS, CONNECT_TIMEOUT_MS ) ) == WICED_SUCCESS )
             {
                 connected_p = WICED_TRUE;
                 WPRINT_APP_INFO( ( "Connected to %s\n", SERVER_HOST ) );
             }
             else
             {
                 WPRINT_APP_INFO( ( "Error: failed to connect to post server: %u\n", result) );
                 wiced_rtos_set_semaphore(&httpWait_post); // Set semaphore that allows the next request to start
        //              return; /* Connection failed - exit program */

             }
           }

        wiced_rtos_delay_milliseconds(35);
        send_post(&header_post,tk);
        wiced_rtos_get_semaphore(&httpWait_post, WICED_WAIT_FOREVER);


            tk = strtok(NULL, s);
        }

        http_client_disconnect( &client_post );
        http_client_deinit( &client_post );
        memset(filebuf,NULL,LOCAL_BUFFER_SIZE);
        memset(tk,NULL,LOCAL_BUFFER_SIZE);

        wiced_rtos_delay_milliseconds(200);
          if(!connected_g) {
                if ( ( result = http_client_connect( &client_get, (const wiced_ip_address_t*)&ip_address, SERVER_PORT_http, HTTP_USE_TLS, CONNECT_TIMEOUT_MS ) ) == WICED_SUCCESS )
                {
                    connected_g = WICED_TRUE;
                    WPRINT_APP_INFO( ( "Connected to %s\n", SERVER_HOST ) );
                }
                else
                {
                    WPRINT_APP_INFO( ( "Error: failed to connect to get server: %u\n", result) );
                    wiced_rtos_set_semaphore(&httpWait_get); // Set semaphore that allows the next request to start
      //              return; /* Connection failed - exit program */
                }

              }
              send_get();

              wiced_rtos_get_semaphore(&httpWait_get, WICED_WAIT_FOREVER);
              http_client_disconnect( &client_get );
                    http_client_deinit( &client_get );

        wiced_rtos_set_semaphore(&tcpGatewaySemaphore); // Set semaphore that allows the next request to start


        wiced_rtos_unlock_mutex(&HTTPMutex);


}

void net_vehicle(){
    d1= ((rg[0]<< 24) | (rg[1] << 16) | ( rg[2] << 8) | (rg[3]));
    d2= ((rn[0]<< 24) | (rn[1] << 16) | ( rn[2] << 8) | (rn[3]));
    d3= ((ri[0]<< 24) | (ri[1] << 16) | ( ri[2] << 8) | (ri[3]));

    s1 = MAKE_IPV4_ADDRESS(rs[0],rs[1],rs[2],rs[3]);

    device_init_ip_settings2.gateway.ip.v4=(uint32_t)d1;
    device_init_ip_settings2.netmask.ip.v4=(uint32_t)d2;
    device_init_ip_settings2.ip_address.ip.v4=(uint32_t)d3;

    /* Bring up the network interface */
      wiced_result_t result;

      WPRINT_APP_INFO( ("Net config\r\n") );

//      result = wiced_network_up(WICED_CONFIG_INTERFACE, WICED_USE_EXTERNAL_DHCP_SERVER,&device_init_settings);

      //while(!ip_is_up){
      do{
          for ( retries = LASEC_JOIN_RETRY_ATTEMPTS; retries != 0; --retries )
          {
              wiced_dct_read_lock( (void**) &ap, WICED_FALSE, DCT_WIFI_CONFIG_SECTION, (uint32_t) ( OFFSETOF(platform_dct_wifi_config_t,stored_ap_list) + a * sizeof(wiced_config_ap_entry_t) ), sizeof(wiced_config_ap_entry_t) ) ;
              if ( ap->details.SSID.length != 0 )
              {
                  result=wiced_join_ap_specific( &ap->details, ap->security_key_length, ap->security_key );
                  wiced_dct_read_unlock( (wiced_config_ap_entry_t*) ap, WICED_FALSE );

              }
              if ( result == WICED_SUCCESS )
              {
                  //printf("\n encontro un SSID QUE NO ESTA SOLO Y LA CONTRASE�A ES LA MISMA \n");
                  break;
              }
          }

          wiced_network_set_hostname(V_h);    /* Host name */

          result_ip = wiced_ip_up( interface, WICED_USE_EXTERNAL_DHCP_SERVER, &device_init_ip_settings2 );
          //result_ip = wiced_ip_up( interface, WICED_USE_STATIC_IP, &device_init_ip_settings2 );
          if((result_ip==WICED_SUCCESS)){
              printf("\n ** Asignacion de IP correcta ** \n");
              ip_is_up=true;
              //break;
          }
      }while(!ip_is_up);

      send_request_date();

      wiced_rtos_get_semaphore(&tcpGatewaySemaphore,WICED_WAIT_FOREVER);

//
      /* Create a TCP socket */
      if ( wiced_tcp_create_socket( &tcp_client_socket, interface ) != WICED_SUCCESS )
      {
          WPRINT_APP_INFO( ("TCP socket creation failed\n") );
      }
      else
          WPRINT_APP_INFO( ("TCP socket creation \n") );


      /* Bind to the socket */
      wiced_tcp_bind( &tcp_client_socket, TCP_SERVER_PORT );

      /*Initialize tcp_stream*/
      tcp_stream_wr = (wiced_tcp_stream_t*)malloc(sizeof(wiced_tcp_stream_t));
      if (wiced_tcp_stream_init(tcp_stream_wr, &tcp_client_socket)!= WICED_SUCCESS)
          WPRINT_APP_INFO(("TCP stream init failed \n"));
      else
          WPRINT_APP_INFO(("TCP stream init \n"));


//       wiced_rtos_register_timed_event( &tcp_client_event, WICED_NETWORKING_WORKER_THREAD, &tcp_client_rec, 3000, 0 );
//
//              wiced_rtos_create_thread(&ThreadHandle_C, THREAD_BASE_PRIORITY+5, "Collision", collision_event_log, THREAD_STACK_SIZE, NULL);
//
      wiced_rtos_create_thread(&publishThreadHandle, THREAD_BASE_PRIORITY+1, NULL, Main_Thread_TCP, THREAD_STACK_SIZE, NULL);

//      Procesos previos necesarios para enviar los archivos uno por uno

//      coun=read_data(ACARREO_ROOT,date_get(&i2c_rtc),&fs_handle);

//      wiced_rtos_register_timed_event( &tcp_client_event, WICED_NETWORKING_WORKER_THREAD, &tcp_client,3000, 0 );

//      wiced_rtos_create_thread(&publishThreadHandle, THREAD_BASE_PRIORITY+1, NULL, publishThread, THREAD_STACK_SIZE, NULL);
//      wiced_rtos_create_thread(&publishThreadHandle, THREAD_BASE_PRIORITY+1, NULL, tcp_client, THREAD_STACK_SIZE, NULL);
}



void collision_event_log(wiced_thread_arg_t arg){

//    printf("incio de hilo collision\n");
//    wiced_ip_address_t INITIALISER_IPV4_ADDRESS( ip_address, s1);
            wiced_rtos_delay_milliseconds(2000);


    wiced_ip_address_t INITIALISER_IPV4_ADDRESS( ip_address, s1);
    end_point_post();


    while(1){
        switch(wiced_network_is_ip_up(WICED_STA_INTERFACE)){
            case 1:
                end_point_post();

                break;
        }

//      if(wiced_network_is_up(WICED_STA_INTERFACE)==WICED_TRUE){
////          wiced_rtos_delay_milliseconds(7000);
//          init_endpoint();
//
//      }
//        wiced_rtos_delay_milliseconds(200);
//              send_get();
//              wiced_rtos_get_semaphore(&httpWait_get, WICED_WAIT_FOREVER);

        if((strlen(aux_date_y)==8)&&(strlen(aux_time)==8)&&(flag_time_set_PUBLISH==WICED_TRUE)&&(flag_time_set_PUBLISH==WICED_TRUE)){ // Realizar la condicion correcto o quitarla la condicional
            flag_time_set=WICED_FALSE;
            flag_time_set_PUBLISH=WICED_FALSE;
            date_set(aux_date_y,&i2c_rtc);
            wiced_rtos_delay_microseconds(100);
//            time_set(aux_time,&i2c_rtc);
            printf("%s,%s\n",aux_date_y,aux_time);
            wiced_rtos_delay_microseconds(1000);
    //          wiced_rtos_set_semaphore(&httpWait); // Set semaphore that allows the next request to start
            wiced_rtos_set_semaphore(&tcpGatewaySemaphore);

        }
        else if ((strlen(aux_date_y)!=8)&&(strlen(aux_time)!=8)&&(flag_time_set_PUBLISH==WICED_TRUE)&&(flag_time_set_PUBLISH==WICED_TRUE)){
            wiced_rtos_set_semaphore(&tcpGatewaySemaphore);
        }

        wiced_rtos_delay_milliseconds(2000);

    }

}


void net_config(){
    printf("\n ---------> net_config \n");
    /* Bring up the network interface */
    wiced_result_t result;

    //Set_SSID(SIID_C,sizeof(SIID_C),WICED_UART_1);
    //Set_KEY(PASS_C,sizeof(PASS_C),WICED_UART_1);

    WPRINT_APP_INFO( ("Net config\r\n") );
    //      result = wiced_network_up(WICED_CONFIG_INTERFACE, WICED_USE_EXTERNAL_DHCP_SERVER,&device_init_settings);

//    result = wiced_network_up(WICED_CONFIG_INTERFACE, WICED_USE_EXTERNAL_DHCP_SERVER,&device_init_settings);

    for ( retries = LASEC_JOIN_RETRY_ATTEMPTS; retries != 0; --retries )
      {
   wiced_dct_read_lock( (void**) &ap, WICED_FALSE, DCT_WIFI_CONFIG_SECTION, (uint32_t) ( OFFSETOF(platform_dct_wifi_config_t,stored_ap_list) + a * sizeof(wiced_config_ap_entry_t) ), sizeof(wiced_config_ap_entry_t) ) ;
                if ( ap->details.SSID.length != 0 )
                {
                    result=wiced_join_ap_specific( &ap->details, ap->security_key_length, ap->security_key );
                    wiced_dct_read_unlock( (wiced_config_ap_entry_t*) ap, WICED_FALSE );
                }
        if ( result == WICED_SUCCESS )
        {
           break;
        }

      }
    wiced_network_set_hostname("LASEC");
    result = wiced_ip_up( interface, WICED_USE_EXTERNAL_DHCP_SERVER, &device_init_settings );


    /* Create a TCP socket */
    if ( wiced_tcp_create_socket( &tcp_client_socket, interface ) != WICED_SUCCESS )
    {

    }
    /* Bind to the socket */
    wiced_tcp_bind( &tcp_client_socket, TCP_SERVER_PORT_c );
    /* Register a function to send TCP packets */
    //printf("\n ---------> Go to event \n");
    wiced_rtos_register_timed_event( &tcp_client_event, WICED_NETWORKING_WORKER_THREAD, &tcp_client_config, TCP_CLIENT_INTERVAL_c, 0 );


}

void set_name(){
   wiced_mac_t mac;

    if ( wwd_wifi_get_mac_address( &mac, WWD_STA_INTERFACE ) == WWD_SUCCESS )
    {
        WPRINT_NETWORK_INFO(("WLAN MAC : %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac.octet[0],mac.octet[1],mac.octet[2],mac.octet[3],mac.octet[4],mac.octet[5]));
        mac_Re=re_mac(mac);
        sprintf(mac_wifi,"%02X:%02X:%02X:%02X:%02X:%02X", mac.octet[0],mac.octet[1],mac.octet[2],mac.octet[3],mac.octet[4],mac.octet[5]);
        sprintf(mac_wifi_bt,"WMA%c%c%c%c%c%c\n\n", mac_Re.mac_rewrite[0] ,mac_Re.mac_rewrite[1],mac_Re.mac_rewrite[2],mac_Re.mac_rewrite[3],mac_Re.mac_rewrite[4],mac_Re.mac_rewrite[5]);
        sprintf(mac_wifi_Send,"%02X:%02X:%02X:%02X:%02X:%02X\n",mac_Re.mac_rewrite[0],mac_Re.mac_rewrite[1],mac_Re.mac_rewrite[2],mac_Re.mac_rewrite[3],mac_Re.mac_rewrite[4],mac_Re.mac_rewrite[5]);
        sprintf(mac_find,"WMA%c%c%c%c%c%c\n\n",mac_Re.mac_rewrite[0],mac_Re.mac_rewrite[1],mac_Re.mac_rewrite[2],mac_Re.mac_rewrite[3],mac_Re.mac_rewrite[4],mac_Re.mac_rewrite[5]);

    }
    wiced_uart_transmit_bytes( WICED_UART_1, mac_wifi_bt, sizeof(  mac_wifi_bt ) - 1 );
    wiced_uart_transmit_bytes( WICED_UART_1, mac_find, sizeof( mac_find ) - 1 );
//    wiced_uart_transmit_bytes( WICED_UART_1, "WMA123456\n\n", sizeof( mac_find ) - 1 );
//    wiced_uart_transmit_bytes( WICED_UART_1, "WMA123456\n\n", sizeof( mac_find ) - 1 );

    WPRINT_APP_INFO(("Mac bluetooth:  %s  \n",mac_wifi_bt));

}

#endif  /* stdbool.h */
