/**********************************************************************************
* Copyright (C) 2018-2021 LASECÂ®ï¸� Telecomunicaciones S.A.P.I. de C.V.
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

#ifndef _GATEWAY_H
#define _GATEWAY_H

#include "wiced.h"
#include "string.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

#include "manager_net.h"
#include "TXT/manager_menssage_vh.h"
#include "UART/main_uart.h"
#include "GPIO/manager_gpio_vh.h"

int status_config=0;
uint8_t coun;


/******************************************************
 *                     TCP Client
 ******************************************************/
//This is needed to implement tcp services
#define TCP_CLIENT_INTERVAL               1
#define STREAM_DATA_BUFFER_LENGTH         1000
#define limit_data                        100

#define TCP_PACKET_MAX_DATA_LENGTH        60


#define TCP_SERVER_PORT                   8010
#define TCP_CLIENT_CONNECT_TIMEOUT        500
#define TCP_CLIENT_RECEIVE_TIMEOUT        200
#define TCP_CLIENT_RECEIVE_TIMEOUT_LAMP        3000

#define TCP_CONNECTION_NUMBER_OF_RETRIES  6
#define TCP_DOWN_NUMBER                   42  //60
#define TCP_CLIENT_STACK_SIZE   (10000)

#define SIID_C "-SSF-HARDWARE"
#define PASS_C "-KLasec123."



uint8_t mac_wifi_Send[40];

struct net_Save net_value;

wiced_tcp_stream_t* tcp_stream_wr;
static wiced_tcp_socket_t  tcp_client_socket;



void select_net(uint8_t* msn1,uint8_t thread1);
void switch_msm(int status_c);
wiced_interface_t interface;

static const wiced_ip_setting_t device_init_settings =
{
    INITIALISER_IPV4_ADDRESS( .ip_address, MAKE_IPV4_ADDRESS(10,86,  11,  1) ),
    INITIALISER_IPV4_ADDRESS( .netmask,    MAKE_IPV4_ADDRESS(255,255,255,  0) ),
    INITIALISER_IPV4_ADDRESS( .gateway,    MAKE_IPV4_ADDRESS(10,86,  10,  254) ),
};

char mac_wifi[40];
char mac_ap[40];
char ip[30];
char mac_wifi_bt[12];
char mac_find[24];
int s_count_x=0;
uint8_t try_n=1;
char data_out[1000];
char bt_msm[50];
int data_send_bt;

wiced_bool_t Data_B_in=WICED_FALSE;

const char s[2] = "\n";
char *token;

// Variables for network change
uint32_t rs[4];
uint32_t rn[4];
uint32_t ri[4];
uint32_t rg[4];

uint32_t d1;
uint32_t d2;
uint32_t d3;
uint32_t d4;

unsigned long s1;

void publishThread(wiced_thread_arg_t arg);

void check_sound_onoff(){
    if(silent==WICED_TRUE){
        if(_sound_flag==WICED_TRUE){
            _sound_flag=WICED_FALSE;
//            set_gpio_menssage(SOUND_ONOFF,silent);
            set_gpio_menssage(RESET_WIFI,1);
        }
    }
}
void check_sound(){
    if(_sound_flag==WICED_TRUE){
        _sound_flag=WICED_FALSE;
        set_gpio_menssage(SOUND_ONOFF,silent);
    }
}

/*************** Tcp Configurator Thread ***************/
/*************** Tcp Configurator Thread ***************/
void publishThread(wiced_thread_arg_t arg)
{
    wiced_mac_t myMac;
    wiced_tcp_socket_t socket;                      // The TCP socket
    wiced_tcp_stream_t stream;                      // The TCP stream
    char sendMessage[80];
    wiced_result_t result;
    wiced_packet_t* tx_packet;
    uint8_t *tx_data;
    uint16_t available_data_length;


///* Create a TCP socket */
//        if ( wiced_tcp_create_socket( &tcp_client_socket, interface ) != WICED_SUCCESS )
//        {
//            WPRINT_APP_INFO( ("TCP socket creation failed\n") );
//        }
//        else
//            WPRINT_APP_INFO( ("TCP socket creation \n") );
//
//
//        /* Bind to the socket */
//        wiced_tcp_bind( &tcp_client_socket, TCP_SERVER_PORT );
//
//        /*Initialize tcp_stream*/
//        tcp_stream_wr = (wiced_tcp_stream_t*)malloc(sizeof(wiced_tcp_stream_t));
//        if (wiced_tcp_stream_init(tcp_stream_wr, &tcp_client_socket)!= WICED_SUCCESS)
//            WPRINT_APP_INFO(("TCP stream init failed \n"));
//        else
//            WPRINT_APP_INFO(("TCP stream init \n"));


//        wiced_rtos_set_semaphore(&tcpGatewaySemaphore);

//    end_point_post();
    wiced_ip_address_t INITIALISER_IPV4_ADDRESS( ip_address, s1);

//    switch(wiced_network_is_ip_up(WICED_STA_INTERFACE)){
//        case 1:
//            end_point_post();
//
//            break;
//    }
    flag_time_set_PUBLISH=WICED_TRUE;


    while ( 1 )
    {



//            wiced_rtos_get_semaphore(&tcpGatewaySemaphore,WICED_WAIT_FOREVER);

//        send_get();

        //        wiced_rtos_set_semaphore(&displaySemaphore);
        wiced_rtos_lock_mutex(&pubSubMutex);

        wiced_ip_address_t INITIALISER_IPV4_ADDRESS( server_ip_address, s1 );


        WPRINT_APP_INFO(("Event Thread Tcp client\n"));
//        WPRINT_APP_INFO( ("Count of bluetooth with mac sssssssss -> %d \r\n",s_count_x) );

//        wiced_tcp_socket_t socket;                      // The TCP socket
//        wiced_tcp_stream_t stream;                      // The TCP stream
//        wiced_result_t result;
        // Open the connection to the remote server via a Socket
        result = wiced_tcp_create_socket(&socket, WICED_STA_INTERFACE);

        if ( result != WICED_SUCCESS )
        {
            try_n=try_n+1;
            if(try_n==TCP_DOWN_NUMBER){
//                   set_name();
                check_sound_onoff();
                 wiced_rtos_delay_milliseconds(100);
//                     set_name();
               wiced_framework_reboot();
            }

//            WPRINT_APP_INFO(("falied 1\n"));
//            if((strlen(aux_date_y)==8)&&(strlen(aux_time)==8)&&(flag_time_set_PUBLISH==WICED_TRUE)){ // Realizar la condicion correcto o quitarla la condicional
//                flag_time_set=WICED_FALSE;
//                flag_time_set_PUBLISH=WICED_FALSE;
//                            date_set(aux_date_y,&i2c_rtc);
//                wiced_rtos_delay_microseconds(100);
//                            time_set(aux_time,&i2c_rtc);
//                printf("%s,%s\n",aux_date_y,aux_time);
//                wiced_rtos_delay_microseconds(2000);
//
//            }

        }
        else{
//            try_n=0;
            WPRINT_APP_INFO(("e  falied 1\n"));

        }

        result = wiced_tcp_bind( &socket, WICED_ANY_PORT ); /* Poner any port para que actualice el puerto de manera automatica */
        if(result!=WICED_SUCCESS)
        {
            try_n=try_n+1;
            if(try_n==TCP_DOWN_NUMBER){
//                   set_name();
                check_sound_onoff();

                 wiced_rtos_delay_milliseconds(100);
//                     set_name();
//                 if((strlen(aux_date_y)==8)&&(strlen(aux_time)==8)&&(flag_time_set_PUBLISH==WICED_TRUE)){ // Realizar la condicion correcto o quitarla la condicional
//                     flag_time_set=WICED_FALSE;
//                     flag_time_set_PUBLISH=WICED_FALSE;
//                                 date_set(aux_date_y,&i2c_rtc);
//                     wiced_rtos_delay_microseconds(100);
//                                 time_set(aux_time,&i2c_rtc);
//                     printf("%s,%s\n",aux_date_y,aux_time);
//                     wiced_rtos_delay_microseconds(2000);
//
//                 }
               wiced_framework_reboot();
            }

            WPRINT_APP_INFO(("falied 2\n"));

            wiced_tcp_delete_socket(&socket); /* Delete socket and return*/
        }
        else{
            //try_n=0;
            WPRINT_APP_INFO((" e falied 2\n"));

        }

        result = wiced_tcp_connect(&socket,&server_ip_address,TCP_SERVER_PORT,2500); // 2 second timeout
        if ( result != WICED_SUCCESS )
        {
            try_n=try_n+1;
            if(try_n==TCP_DOWN_NUMBER){
////                   set_name();
                check_sound_onoff();
                     wiced_rtos_delay_milliseconds(100);
//                     set_name();
//            if((strlen(aux_date_y)==8)&&(strlen(aux_time)==8)&&(flag_time_set_PUBLISH==WICED_TRUE)){ // Realizar la condicion correcto o quitarla la condicional
//                flag_time_set=WICED_FALSE;
//                flag_time_set_PUBLISH=WICED_FALSE;
//                            date_set(aux_date_y,&i2c_rtc);
//                wiced_rtos_delay_microseconds(100);
//                            time_set(aux_time,&i2c_rtc);
//                printf("%s,%s\n",aux_date_y,aux_time);
//                wiced_rtos_delay_microseconds(2000);
//
//            }

               wiced_framework_reboot();
            }

            WPRINT_APP_INFO(("falied 3\n"));

            wiced_tcp_delete_socket(&socket);

        }
        else{
            try_n=0;
            WPRINT_APP_INFO(("  e  falied 3\n"));

        }

        WPRINT_APP_INFO(("try %d\n",try_n));


        wiced_ip_address_t myIpAddress;

        wl_bss_info_t ap_info_buffer;
        wiced_security_t ap_security;
        // Format the data per the specification in section 6
        wwd_wifi_get_ap_info(&ap_info_buffer, &ap_security);            // Se obtiene la MAC de la red a la que estamos conectados
        wiced_ip_get_ipv4_address( WICED_STA_INTERFACE, &myIpAddress);  // Se obtiene la IP del dispositivo
        wiced_wifi_get_mac_address(&myMac);                             // Se obtiene la MAC del dispositivo



        sprintf(mac_wifi,"%02X:%02X:%02X:%02X:%02X:%02X",myMac.octet[0],myMac.octet[1],myMac.octet[2],myMac.octet[3],myMac.octet[4],myMac.octet[5]);

        sprintf(mac_ap,"%02X:%02X:%02X:%02X:%02X:%02X",ap_info_buffer.BSSID.octet[0], ap_info_buffer.BSSID.octet[1],ap_info_buffer.BSSID.octet[2],ap_info_buffer.BSSID.octet[3],ap_info_buffer.BSSID.octet[4],ap_info_buffer.BSSID.octet[5]);
        sprintf(ip,"%u.%u.%u.%u", (uint8_t)(GET_IPV4_ADDRESS(myIpAddress) >> 24),(uint8_t)(GET_IPV4_ADDRESS(myIpAddress)>> 16),(uint8_t)(GET_IPV4_ADDRESS(myIpAddress) >> 8),(uint8_t)(GET_IPV4_ADDRESS(myIpAddress) >> 0));

        // Initialize the TCP stream
        wiced_tcp_stream_init(&stream, &socket);
//
        for(int f=0;f<100;f++){
            memcpy(data_btt[f].mac_bt,"01:01:01:01:01:01",17);
            memcpy(data_btt[f].rssi,"-85",5);
            memcpy(data_btt[f].fallen,"0",2);
        }
//
////
//
//        s_count_x=100;
        uint8_t coun;
        // Send the data via the stream
              if(sent_file_flag==WICED_TRUE){

                  coun=read_data(ACARREO_ROOT,date_get(&i2c_rtc),&fs_handle);
                  /* get the first token */
                  token = strtok(filebuf, s);
                  /* walk through other tokens */
                  if(coun!=0){

                      while( token != NULL ) {
                          //            printf( " >>>>>  %s\n", token );
                            wiced_rtos_delay_microseconds( 10 );
                            sprintf(data_out,"\nHV:%s\r\n",token);
                            result=wiced_tcp_stream_write(&stream, data_out, strlen(data_out));

                               if(result==WICED_TCPIP_SUCCESS){
                                   wiced_uart_transmit_bytes(WICED_UART_1,(("%s",data_out)),strlen(data_out));
                                   send_data_task=WICED_TRUE;

                                }
                           token = strtok(NULL, s);
                           coun--;
                          }
                  }

                          wiced_rtos_set_semaphore(&tcpGatewaySemaphore);

                        memset(filebuf,NULL,LOCAL_BUFFER_SIZE);

                        if(coun<=2){
//                            sent_file_flag=WICED_FALSE;
//                            break;
                        }
                        else{

                        }
                  wiced_rtos_get_semaphore(&tcpGatewaySemaphore,WICED_WAIT_FOREVER);

              }

              if((s_count_x<=limit_data)&&(sent_file_flag==WICED_FALSE)){
                  WPRINT_APP_INFO(("Multiple Tcp client\n"));
                  if(s_count_x==limit_data){
                      data_send_bt=limit_data;
      //                count_bt=0;
                  }
                  else if(s_count_x<limit_data){
                      data_send_bt=s_count_x;
      //                count_bt=0;
                  }
                  if(s_count_x!=0){
                      WPRINT_APP_INFO( (">> es igual a %d en %s\n\n\n",s_count_x,mac_ap) );

                      for(int f=0;f<data_send_bt;f++){  /* Supongamos que data_b_send == 100 */
                          if(f==0){
      //                    sprintf(data_out,"\nV;%s,%s,%s,%s,%s\r\n",mac_wifi,mac_ap,ip,time_get(&i2c_rtc),date_get(&i2c_rtc));
                              sprintf(data_out,"\nH;%s,%s,%s,%s,%s\r\n",mac_wifi,mac_ap,ip,time_get(&i2c_rtc),date_get_log(&i2c_rtc));
                              result=wiced_tcp_stream_write(&stream, data_out, strlen(data_out));
                              memcpy(data_btt[f].mac_bt,NULL,17);
                                memcpy(data_btt[f].type,NULL,17);
                                memcpy(data_btt[f].rssi,NULL,4);
                                memcpy(data_btt[f].fallen,NULL,2);
                              if(result==WICED_TCPIP_SUCCESS){
//                                  wiced_uart_transmit_bytes(WICED_UART_1,(("%s",data_out)),strlen(data_out));
                                  send_data_task=WICED_TRUE;
                              }
                          }
                          else{
                          wiced_rtos_delay_microseconds( 10 );
                          sprintf(data_out,"\nB;%s,%s,%s,%s,%s\r\n",mac_ap,data_btt[f].mac_bt,mac_wifi,data_btt[f].type,data_btt[f].rssi);
//                          sprintf(data_out,"\nB;%s,%s,%s,%s\r\n",mac_ap,data_btt[f].mac_bt,mac_wifi,data_btt[f].rssi);

                                          memcpy(data_btt[f].mac_bt,NULL,17);
                                          memcpy(data_btt[f].type,NULL,17);
                                          memcpy(data_btt[f].rssi,NULL,4);
                                          memcpy(data_btt[f].fallen,NULL,2);


                          result=wiced_tcp_stream_write(&stream, data_out, strlen(data_out));
                          if(result==WICED_TCPIP_SUCCESS){

                              wiced_uart_transmit_bytes(WICED_UART_1,(("%s",data_out)),strlen(data_out));
                              send_data_task=WICED_TRUE;

                           }
                          }
                      }
                      s_count_x=0;
                      data_send_bt=0;
                  }
                  else{
      //                sprintf(data_out,"\nV;%s,%s,%s,%s,%s\r\n",mac_wifi,mac_ap,ip,time_get(&i2c_rtc),date_get(&i2c_rtc));
//                      sprintf(data_out,"\nU;%s,1600,%d%d%d%d0000000000000,%s,%s,%s,%s\r\n",mac_wifi,t1,t2,t3,t4,mac_ap,ip,time_get(&i2c_rtc),date_get(&i2c_rtc));
                      sprintf(data_out,"\nH;%s,%s,%s,%s,%s\r\n",mac_wifi,mac_ap,ip,time_get(&i2c_rtc),date_get_log(&i2c_rtc));

                      result=wiced_tcp_stream_write(&stream, data_out, strlen(data_out));
                         if(result==WICED_TCPIP_SUCCESS){
                             wiced_uart_transmit_bytes(WICED_UART_1,(("%s",data_out)),strlen(data_out));
                             send_data_task=WICED_TRUE;

                          }

                  }

              }

//              memset(sendMessage,NULL,80);
//              memset(data_out,NULL,1000);

              sent_file_flag=!sent_file_flag;


        wiced_tcp_stream_flush(&stream);


        // Delete the stream and socket
        wiced_tcp_stream_deinit(&stream);
        wiced_tcp_delete_socket(&socket);
        wiced_rtos_delay_milliseconds( 4000 );


    wiced_rtos_unlock_mutex(&pubSubMutex);

    wiced_rtos_set_semaphore(&tcpReceptionSemaphore);

    }
}

void data_bt_send(unsigned char* buffer_in ){
    Data_B_in = WICED_FALSE;
    unsigned char str_switch[4];
       unsigned char str_split[128];

       strncpy(str_switch,buffer_in,4);
       strcpy(str_split,&buffer_in[4]);

       char delim[] = ",";     //establece como  realizara el split
       int x=0;
    if(strstr(str_switch,"BNM|")){

           if(s_count_x==limit_data){
               data_send_bt=s_count_x;
           }
           else if(s_count_x<limit_data){

               unsigned char *cvl1 = strtok(str_split, delim);
               while(cvl1 != NULL)
               {
                   switch (x) {
                   case 0:
                       //memcpy(data_btt[s_count_x+1].mac_bt,cvl1,17);  /* Copia la mac sin importar de que sea */
                       memcpy(data_B.mac_bt,cvl1,17);
                       break;
                   case 1:
                       if(strstr(buffer_in,"LAMP")||(strstr(buffer_in,"VEHI"))){
                           strcpy(data_btt[s_count_x+1].type,"LAMP");
                           //strcpy(data_B.type,"LAMP");
                       }
                       else if(strstr(buffer_in,"BEAC")){
                           strcpy(data_btt[s_count_x+1].type,"BEAC");
                           //strcpy(data_B.type,"BEAC");
                           GEOSF_F=WICED_TRUE;
                           //printf("\n **** BEACON ACARO **** \n");
                       }
                       else{
                           strcpy(data_btt[s_count_x+1].type,"BEAC"); //GEOSF y lo restante
                           //strcpy(data_B.type,"BEAC");
                       }
                       break;
                   case 2:
                       //strcpy(data_btt[s_count_x+1].rssi,cvl1);
                       strcpy(data_B.rssi,cvl1);
                       break;
                   case 3:
                       //strcpy(data_btt[s_count_x+1].fallen,cvl1);
                       strcpy(data_B.fallen,cvl1);
                       break;
                   default:
                       break;
                   }
                   x++;
                   cvl1=strtok(NULL, delim);
               }
               /* Guardado solo si no se tiene en la estrcutra, si esta guardado solo se actualiza su RSSI */
               int in_v=0;
               for(int i=0; i<s_count_x;i++)
               {
                   if(strstr(data_btt[i+1].mac_bt,data_B.mac_bt))
                   {
                       Data_B_in = WICED_TRUE;
                       in_v = i+1;     /* Keep the position that was detect */
                       break;
                   }
               }
               if(Data_B_in == WICED_FALSE)    /* Save vehicule mac */
               {
                   //memcpy(data_btt[s_count_x+1].type,data_B.type,4);
                   memcpy(data_btt[s_count_x+1].rssi,data_B.rssi,4); /* Only update the RSSI value */
                   memcpy(data_btt[s_count_x+1].mac_bt,data_B.mac_bt,17);
                   s_count_x++;
                   data_send_bt=s_count_x;
               }
               else
               {
                   sprintf(data_btt[in_v].rssi,"R%s",data_B.rssi);
                   //memcpy(data_btt[in_v].rssi,data_B.rssi,4); /* Only update the RSSI value */
               }
               /* ****************** */
               x=0;
//               s_count_x++;
//               data_send_bt=s_count_x;
           }
           memset(data_B.mac_bt,NULL,17);
           memset(data_B.type,NULL,17);
           memset(data_B.rssi,NULL,4);
           memset(data_B.fallen,NULL,2);
    }

    wiced_rtos_set_semaphore(&displaySemaphore);

}




#endif  /* stdbool.h */
