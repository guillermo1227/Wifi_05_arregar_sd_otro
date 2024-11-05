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

#ifndef _Reception_H
#define _Reception_H

#define TCP_NUMBER      1
#define GEOLOC

uint8_t count_tcp=0;
uint8_t count_stream=0;

void conect_tcp(void);
/*************** Tcp Configurator Thread ***************/
/* Thread to publish data to the cloud */

void Main_Thread_TCP(wiced_thread_arg_t arg){
    uint8_t  key=1;

    uint8_t _send_file;

    while (1){
        wiced_rtos_lock_mutex(&pubSubMutex);
        switch(key){
            case 1:
                key= tcp_client_gateway();
                break;
        }

        wiced_rtos_unlock_mutex(&pubSubMutex);

    }
}

int tcp_client_gateway(void)
{
    send_data_task=WICED_TRUE;

    int state=0;
    wiced_mac_t myMac;
    wiced_tcp_socket_t socket;                      // The TCP socket
    wiced_tcp_stream_t stream;                      // The TCP stream
    wiced_result_t result;
    wiced_packet_t* tx_packet;
    uint8_t *tx_data;
    uint16_t available_data_length;

    wiced_ip_address_t INITIALISER_IPV4_ADDRESS( ip_address, s1);
    wiced_ip_address_t INITIALISER_IPV4_ADDRESS( server_ip_address, s1 );

    printf("Crate socket\n");
    result = wiced_tcp_create_socket(&socket, WICED_STA_INTERFACE);
    if ( result != WICED_SUCCESS )
    {
        try_n=try_n+1;
        if(try_n>=TCP_DOWN_NUMBER)
        {
            check_sound_onoff();
            wiced_rtos_delay_milliseconds(100);
            tcp_down_connect=WICED_TRUE;
            wiced_framework_reboot();
        }
    }
    else{
        //  try_n=0;
        WPRINT_APP_INFO(("e  falied 1\n"));
        }
        printf("Bind socket\n");
        result = wiced_tcp_bind( &socket, WICED_ANY_PORT ); /* Poner any port para que actualice el puerto de manera automatica */
        if(result!=WICED_SUCCESS)
        {
            try_n=try_n+1;
            if(try_n>=TCP_DOWN_NUMBER){
                check_sound_onoff();
                wiced_rtos_delay_milliseconds(100);
                tcp_down_connect=WICED_TRUE;
                wiced_framework_reboot();
            }
            WPRINT_APP_INFO(("falied 2\n"));
            wiced_tcp_delete_socket(&socket); /* Delete socket and return*/
        }
        else{
            //try_n=0;
            WPRINT_APP_INFO((" e falied 2\n"));

        }
        printf("Conect socket\n");
        result = wiced_tcp_connect(&socket,&server_ip_address,TCP_SERVER_PORT,2500); // 2 second timeout
        if ( result != WICED_SUCCESS )
        {
            try_n=try_n+1;
            if(try_n>=TCP_DOWN_NUMBER)
            {
                check_sound_onoff();
                wiced_rtos_delay_milliseconds(100);
                tcp_down_connect=WICED_TRUE;
                wiced_framework_reboot();
            }

            WPRINT_APP_INFO(("falied 3\n"));
            wiced_tcp_delete_socket(&socket);
        }
        else{
            try_n=0;
            WPRINT_APP_INFO(("  e  falied 3\n"));
        }
        printf("Status try_n %d\n",try_n);


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
    result = wiced_tcp_stream_init(&stream, &socket);
    if( result != WICED_TCPIP_SUCCESS)
    {
        printf("\n No se pudo inicializar el socket \n");
    }


        char* res;
        for(int i=0;i<100;i++)
        {
            res=malloc(sizeof(char)*150);

            if(strlen(log_node[i].mac_bt_gatw != 0))
            {
                sprintf(data_out,"\nF;%s\r\n",fill_jaso_dat(res,i));
                result=wiced_tcp_stream_write(&stream, data_out, strlen(data_out));
                printf("\n %s  in i= %d \n",data_out,i);
            }
            free(res);
        }

    memset(data_out,NULL,1000);

    wiced_tcp_stream_flush(&stream);
    wiced_rtos_delay_milliseconds( 1500 );

    wiced_tcp_stream_deinit(&stream);
    wiced_tcp_delete_socket(&socket);
    return 1;
}

void send_request_date()
{
    wiced_tcp_socket_t socket;                      // The TCP socket
    wiced_tcp_stream_t stream;                      // The TCP stream
    char sendMessage[140];
    wiced_result_t result;
    wiced_ip_address_t INITIALISER_IPV4_ADDRESS( server_ip_address, s1 );
    wiced_mac_t myMac;
    wiced_ip_address_t myIpAddress;
    wl_bss_info_t ap_info_buffer;
    wiced_security_t ap_security;

    // Open the connection to the remote server via a socket
    result = wiced_tcp_create_socket(&socket, WICED_STA_INTERFACE);
    if(result!=WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("Failed to create socket %d\n",result));
        return;
    }
    else {
        printf("ok1\n");
    }

    result = wiced_tcp_bind(&socket,WICED_ANY_PORT);
    if(result!=WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("Failed to bind socket %d\n",result));
        wiced_tcp_delete_socket(&socket);
        return;
    }
    else {
        printf("ok2\n");
    }


    result = wiced_tcp_connect(&socket,&server_ip_address,TCP_SERVER_PORT,TCP_CLIENT_CONNECT_TIMEOUT); // 2 second timeout
    if ( result != WICED_SUCCESS )
    {
        WPRINT_APP_INFO(( "Failed connect = [%d]\n", result ));
        wiced_tcp_delete_socket(&socket);
        wiced_rtos_set_semaphore(&tcpGatewaySemaphore);
        return;
    }

    wiced_wifi_get_mac_address(&myMac);                             // Se obtiene la MAC del dispositivo
    wiced_ip_get_ipv4_address( WICED_STA_INTERFACE, &myIpAddress);  // Se obtiene la IP del dispositivo
    wwd_wifi_get_ap_info(&ap_info_buffer, &ap_security);            // Se obtiene la MAC de la red a la que estamos conectados
    /*Ectract data to peripherals*/



    sprintf(mac_wifi,"%02X:%02X:%02X:%02X:%02X:%02X",myMac.octet[0],myMac.octet[1],myMac.octet[2],myMac.octet[3],myMac.octet[4],myMac.octet[5]);
    sprintf(mac_ap,"%02X:%02X:%02X:%02X:%02X:%02X",ap_info_buffer.BSSID.octet[0], ap_info_buffer.BSSID.octet[1],ap_info_buffer.BSSID.octet[2],ap_info_buffer.BSSID.octet[3],ap_info_buffer.BSSID.octet[4],ap_info_buffer.BSSID.octet[5]);
    sprintf(ip,"%u.%u.%u.%u", (uint8_t)(GET_IPV4_ADDRESS(myIpAddress) >> 24),(uint8_t)(GET_IPV4_ADDRESS(myIpAddress)>> 16),(uint8_t)(GET_IPV4_ADDRESS(myIpAddress) >> 8),(uint8_t)(GET_IPV4_ADDRESS(myIpAddress) >> 0));

    sprintf(sendMessage,"\nH;%s,%s,%s,%s,%s\r\n",mac_wifi,mac_ap,ip,time_get(&i2c_rtc),date_get_log(&i2c_rtc));


    // Format the data per the specification in section 6
//    sprintf(sendMessage,"\nhola\r\n"); // 5 is the register from the lab manual
    WPRINT_APP_INFO(("Sent Message=%s\n",sendMessage)); // echo the message so that the user can see something

    // Initialize the TCP stream
    wiced_tcp_stream_init(&stream, &socket);

    // Send the data via the stream
    wiced_tcp_stream_write(&stream, sendMessage, strlen(sendMessage));
    // Force the data to be sent right away even if the packet isn't full yet
    wiced_tcp_stream_flush(&stream);


    // Get the response back from the WWEP server
      char rbuffer[49] = {0}; // The first 11 bytes of the buffer will be sent by the server. Byte 12 will stay 0 to null terminate the string
      result = wiced_tcp_stream_read(&stream, rbuffer, 49, 4500); // Read 11 bytes from the buffer - wait up to 500ms for a response
      if(result == WICED_SUCCESS)
      {
          WPRINT_APP_INFO(("Server Response=%s\n",rbuffer));
      }
      else
      {
          WPRINT_APP_INFO(("Malformed response\n"));
      }
      char _cresult [23];
      memset(_cresult,NULL,23);
      if(strstr(rbuffer,"T:")){
        strncpy(_cresult,strstr(rbuffer,"T:"),21);
//          T:24/04/2023-13:51:06
          split_date_endpoint(&_cresult,strlen(_cresult));
          time_set(aux_time,&i2c_rtc);
          date_set(aux_date_y,&i2c_rtc);

        printf(">%s\n",_cresult);
//          printf("Aqui ira la funcion para setear la hora:  posicion %s \n",strstr(rbuffer,"T:"));
      }
    // Delete the stream and socket
    wiced_tcp_stream_deinit(&stream);
    wiced_tcp_delete_socket(&socket);
    wiced_rtos_set_semaphore(&tcpGatewaySemaphore);

}

#endif  /* stdbool.h */
