#ifndef _CONEXION_WIRELESS_CONFIG_H
#define _CONEXION_WIRELESS_CONFIG_H

#include "wiced.h"
#include "string.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"


#include "manager_net.h"
#include "DCT/manager_dct.h"


wiced_tcp_stream_t* tcp_stream_wr;
wiced_packet_t*          packet;
wiced_packet_t*          rx_packet;

//char sendMessage[80];
unsigned char* sendMessage;
unsigned char*                    tx_data;

#define TCP_CLIENT_INTERVAL_c               150
#define TCP_SERVER_PORT_c                   8077
#define TCP_PACKET_MAX_DATA_LENGTH        120

//Flag to start tcp service
#define TCP_SERVER_IP_ADDRESS MAKE_IPV4_ADDRESS(10,86,10,1)

/* Change the server IP address to match the TCP echo server address */
//#define TCP_SERVER_IP_ADDRESS MAKE_IPV4_ADDRESS(10,86,10,1)


uint8_t mac_wifi_Send[40];

struct net_Save net_value;




static wiced_result_t tcp_client();

/******************************************************
 *               Variable Definitions
 ******************************************************/


static wiced_tcp_socket_t  tcp_client_socket;
void select_net(uint8_t* msn1,uint8_t thread1);
void switch_msm(int status_c);



wiced_result_t tcp_client_config( void* arg )
{
    WPRINT_APP_INFO( ("Event thread tcp client config\n") );

    //    Set_SSID("-SOP_NetCode5663",18,WICED_UART_1);
    //    Set_KEY("-KCr1pT70gr@fyeAr",18,WICED_UART_1);
    //    Set_SERVER("-I10.35.56.1",17,WICED_UART_1);
    //    Set_MASK("-M255.255.248.0",18,WICED_UART_1);
    //    Set_IP("-C10.35.61.19",18,WICED_UART_1);
    //    Set_GATEWAY("-G10.35.63.254",14,WICED_UART_1);
    //    Set_config();

    wiced_result_t result;
    wiced_mac_t myMac;
    wiced_ip_address_t myIpAddress;
    wl_bss_info_t ap_info_buffer;
    wiced_security_t ap_security;
    unsigned char*                    rx_data;
    uint16_t                 rx_data_length;
    uint16_t                 available_data_length;
    int                      connection_retries;
    wiced_ip_address_t INITIALISER_IPV4_ADDRESS( server_ip_address, TCP_SERVER_IP_ADDRESS );

         /* Connect to the remote TCP server, try several times */
         connection_retries = 0;
         do
         {
             result = wiced_tcp_connect( &tcp_client_socket, &server_ip_address, TCP_SERVER_PORT_c, TCP_CLIENT_CONNECT_TIMEOUT );
             connection_retries=connection_retries+1;
         }
         while( ( result != WICED_SUCCESS ) && ( connection_retries < TCP_CONNECTION_NUMBER_OF_RETRIES ) );
         if ( result != WICED_SUCCESS )
            {
            WPRINT_APP_INFO(("Unable to connect to the server! Halt.\n"));
            try_n=try_n+1;
            if(try_n==250){
                printf("\n Reseteo 1 \n");
                wiced_framework_reboot();
            }
            wiced_uart_transmit_bytes(WICED_UART_1,TCP_CONNECT_FAILED,sizeof(TCP_CONNECT_FAILED)-1);
            return WICED_ERROR;

         }
         /* Create the TCP packet. Memory for the tx_data is automatically allocated */
         if (wiced_packet_create_tcp(&tcp_client_socket, TCP_PACKET_MAX_DATA_LENGTH, &packet, (uint8_t**)&sendMessage, &available_data_length) != WICED_SUCCESS)
         {
             WPRINT_APP_INFO(("TCP packet creation failed\n"));
             try_n=try_n+1;
             if(try_n==250){
                 printf("\n Reseteo 2 \n");
                 wiced_framework_reboot();

             }
            wiced_uart_transmit_bytes(WICED_UART_1,CREATE_FAILED,sizeof(CREATE_FAILED)-1);

             return WICED_ERROR;
         }

         /* Write the message into sendMessage"  */
       switch_msm(status_config);

         /* Set the end of the data portion */
        wiced_packet_set_data_end(packet, (uint8_t*)sendMessage + strlen(sendMessage));
        /* Send the TCP packet */
          if (wiced_tcp_send_packet(&tcp_client_socket, packet) != WICED_SUCCESS)
          {
              /* Delete packet, since the send failed */
              wiced_packet_delete(packet);

              /* Close the connection */
              wiced_tcp_disconnect(&tcp_client_socket);
              return WICED_ERROR;
          }
          else{
             wiced_uart_transmit_bytes(WICED_UART_1,(("%s\r\n",sendMessage)),strlen(sendMessage)); // echo the message so that the user can see something

          }

         result = wiced_tcp_receive(&tcp_client_socket, &rx_packet, TCP_CLIENT_RECEIVE_TIMEOUT);
         if( result != WICED_SUCCESS )
                 {
                     /* Delete packet, since the receive failed */
                     wiced_packet_delete(rx_packet);

                     /* Close the connection */
                    wiced_tcp_disconnect(&tcp_client_socket);
                     return WICED_ERROR;
                 }
                 else{
                 }
                 /* Get the contents of the received packet */
                 wiced_packet_get_data(rx_packet, 0, (uint8_t**)&rx_data, &rx_data_length, &available_data_length);

                 if (rx_data_length != available_data_length)
                 {
                     /* Delete packet, since the receive failed */
                    wiced_packet_delete(rx_packet);

                     /* Close the connection */
                    wiced_tcp_disconnect(&tcp_client_socket);
                     return WICED_ERROR;
                 }

                 /* Null terminate the received string */
                rx_data[rx_data_length] = '\x0';
//                WPRINT_APP_INFO(("%s",rx_data));

                WPRINT_APP_INFO(("\nDato: %s\r\n",rx_data));
                   select_net(rx_data,strlen(rx_data)-1);

         wiced_packet_delete(packet);
         wiced_packet_delete(rx_packet);
         wiced_tcp_disconnect(&tcp_client_socket);

        return WICED_SUCCESS;


}

void select_net(uint8_t* message1,uint8_t thread1 ){
    printf("\n Select net ------------> \n");
    char* res;
    uint8_t str_r[3];
    strncpy(str_r,message1,3);
    printf("\n Esto vale str_r :%s \n",str_r);
       res = strstr(message1,"Starting_Config");  //Bandera para dar incio a la configuracion de red
       if(res){
            status_config=1;
       }
       res = strstr(str_r,"-S");  //Bandera para setear en memoria la vlan o ssid
       if(res){
           status_config=2;
           strcpy(net_value.ssid,message1);
           net_value.ssid_length=thread1;
       }
       res = strstr(str_r,"-A");
       if(res){
//         print_app_dct(WICED_UART_1);
       }
       res = strstr(str_r,"-K");  //Bnadera para setear en memoroa la contraseña de la vlan o ssid
       if(res){
          status_config=3;
           strcpy(net_value.pass,message1);
           net_value.pass_length=thread1;
       }
       res = strstr(str_r,"-I");  //Bandera para setear en memoria la ip del servidor
       if(res){
            status_config=5;
            strcpy(net_value.ipser,message1);
            net_value.ipser_length=thread1;
       }
       res = strstr(str_r,"-G");  //Bandera para setear en memoria el gateway del servidor
       if(res){
            status_config=4;
            strcpy(net_value.gate,message1);
            net_value.gate_length=thread1;
       }
       res = strstr(str_r,"-M");  //Bandera para setear en memoria la mac del servidor
       if(res){
            status_config=6;
            strcpy(net_value.mask,message1);
            net_value.mask_length=thread1;
       }
       res = strstr(str_r,"-C");  //Bandera para setear en memoria la ip del client
       if(res){
            status_config=7;
            strcpy(net_value.ipsta,message1);
            net_value.ipsta_length=thread1;
       }
        res = strstr(str_r,"-E");  //Bandera para dar reinicio al modulo y tome los nuevos valores de la memoria
       if(res){
            Set_config();
            ready_net(net_value);
            wiced_packet_delete(packet);
            wiced_packet_delete(rx_packet);
            wiced_tcp_disconnect(&tcp_client_socket);
            wiced_rtos_delay_milliseconds(2000);
            wiced_framework_reboot();
       }
       res = strstr(message1,"FLASH");  //Requiere puntero en lugar de caracter directo para comparar
       if(res){
       }
       res = strstr(message1,"All_Over_Again");  //Requiere puntero en lugar de caracter directo para comparar
       if(res){
           status_config=0;
       }
        res = strstr(message1,"MACBT_R");  //Requiere puntero en lugar de caracter directo para comparar
        if(res){
            status_config=8 ;
         }

}


void switch_msm(int status_c){
    wiced_mac_t mac;
    char mac_wifi_con[40];
    if ( wwd_wifi_get_mac_address( &mac, WWD_STA_INTERFACE ) == WWD_SUCCESS )
     {
        sprintf(mac_wifi_con,"%02X:%02X:%02X:%02X:%02X:%02X",mac.octet[0],mac.octet[1],mac.octet[2],mac.octet[3],mac.octet[4],mac.octet[5]);
     }
    switch(status_c  ){
    case 0:

//            sprintf(sendMessage,"\nMAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n",mac.octet[0],mac.octet[1],mac.octet[2],mac.octet[3],mac.octet[4],mac.octet[5]);
            sprintf(sendMessage,"\nMAC: %s\r\n",mac_wifi_con);
//         }
        break;
    case 1:
//        if ( wwd_wifi_get_mac_address( &mac, WWD_STA_INTERFACE ) == WWD_SUCCESS )
//         {
//            sprintf(sendMessage,"\nMAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n%s\r\n",mac.octet[0],mac.octet[1],mac.octet[2],mac.octet[3],mac.octet[4],mac.octet[5],OK_A);
            sprintf(sendMessage,"\nMAC: %s\r\n%s,%s\r\n",mac_wifi_con,OK_A,V_C);
//         }
            break;
    case 2:
//        if ( wwd_wifi_get_mac_address( &mac, WWD_STA_INTERFACE ) == WWD_SUCCESS )
//        {
//            sprintf(sendMessage,"\nMAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n%s\r\n",mac.octet[0],mac.octet[1],mac.octet[2],mac.octet[3],mac.octet[4],mac.octet[5],OK_SC);
            sprintf(sendMessage,"\nMAC: %s\r\n%s\r\n",mac_wifi_con,OK_SC);
//        }
            break;
    case 3:
//        if ( wwd_wifi_get_mac_address( &mac, WWD_STA_INTERFACE ) == WWD_SUCCESS )
//        {
//             sprintf(sendMessage,"\nMAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n%s\r\n",mac.octet[0],mac.octet[1],mac.octet[2],mac.octet[3],mac.octet[4],mac.octet[5],OK_K);
            sprintf(sendMessage,"\nMAC: %s\r\n%s\r\n",mac_wifi_con,OK_K);
//        }
            break;
    case 4:
//        if ( wwd_wifi_get_mac_address( &mac, WWD_STA_INTERFACE ) == WWD_SUCCESS )
//        {
//                sprintf(sendMessage,"\nMAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n%s\r\n",mac.octet[0],mac.octet[1],mac.octet[2],mac.octet[3],mac.octet[4],mac.octet[5],OK_G);
            sprintf(sendMessage,"\nMAC: %s\r\n%s\r\n",mac_wifi_con,OK_G);
//        }
            break;
    case 5:
//        if ( wwd_wifi_get_mac_address( &mac, WWD_STA_INTERFACE ) == WWD_SUCCESS )
//        {
//            sprintf(sendMessage,"\nMAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n%s\r\n",mac.octet[0],mac.octet[1],mac.octet[2],mac.octet[3],mac.octet[4],mac.octet[5],OK_I);
            sprintf(sendMessage,"\nMAC: %s\r\n%s\r\n",mac_wifi_con,OK_I);
//        }
            break;
    case 6:
//        if ( wwd_wifi_get_mac_address( &mac, WWD_STA_INTERFACE ) == WWD_SUCCESS )
//        {
//            sprintf(sendMessage,"\nMAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n%s\r\n",mac.octet[0],mac.octet[1],mac.octet[2],mac.octet[3],mac.octet[4],mac.octet[5],OK_M);
            sprintf(sendMessage,"\nMAC: %s\r\n%s\r\n",mac_wifi_con,OK_M);
//        }
            break;
    case 7:
//        if ( wwd_wifi_get_mac_address( &mac, WWD_STA_INTERFACE ) == WWD_SUCCESS )
//        {
//            sprintf(sendMessage,"\nMAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n%s\r\n",mac.octet[0],mac.octet[1],mac.octet[2],mac.octet[3],mac.octet[4],mac.octet[5],OK_C);
            sprintf(sendMessage,"\nMAC: %s\r\n%s\r\n",mac_wifi_con,OK_C);
//        }
            break;
    case 8:
//        if ( wwd_wifi_get_mac_address( &mac, WWD_STA_INTERFACE ) == WWD_SUCCESS )
//        {
//            sprintf(sendMessage,"\nMAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n MACBT:%s\r\n",mac.octet[0],mac.octet[1],mac.octet[2],mac.octet[3],mac.octet[4],mac.octet[5],mac_wifi_Send);
            sprintf(sendMessage,"\nMAC: %s\r\n MACBT:%s\r\n",mac_wifi_con,mac_wifi_Send);
//        }
            break;
    default:
        break;

    }

}



static wiced_result_t init_tcp_w(){

    WPRINT_APP_INFO( ("INICIO DE VALORES TCP\n") );

    dct_read_write_app_dct_t* dct_app = NULL;
    wiced_result_t result;
    char *p1;
    char *p2;
    char *p3;
    int x=0;


    if ( wiced_dct_read_lock( (void**) &dct_app, WICED_FALSE, DCT_APP_SECTION, 0, sizeof( *dct_app ) ) != WICED_SUCCESS )
    {
        return WICED_ERROR;
    }

    char cad_aux[sizeof(dct_app->IP)];
    /* Modify string_var by writing the whole DCT */
    strcpy(cad_aux,(char*) (dct_read_write_app_dct_t*)dct_app->IP);

    //establece como  realizara el split
    char delim[] = ".";
    char *ptr1 = strtok(cad_aux, delim);

    while(ptr1 != NULL)
    {

        ri[x]= strtol(ptr1, &p1, 10);
        x++;
        ptr1=strtok(NULL, delim);
    }
    x=0;

    /* Modify string_var by writing the whole DCT */
    strcpy(cad_aux,(char*) (dct_read_write_app_dct_t*)dct_app->MASK);

    //establece como  realizara el split
    char *ptr2 = strtok(cad_aux, delim);

    while(ptr2 != NULL)
    {

        rn[x]= strtol(ptr2, &p2, 10);
        x++;
        ptr2=strtok(NULL, delim);
    }
    x=0;

    /* Modify string_var by writing the whole DCT */
    strcpy(cad_aux,(char*) (dct_read_write_app_dct_t*)dct_app->SERVER);

    //establece como  realizara el split
    char *ptr3 = strtok(cad_aux, delim);

    while(ptr3 != NULL)
    {

        rs[x]= strtol(ptr3, &p3, 10);
        x++;
        ptr3=strtok(NULL, delim);
    }
    x=0;


    d1= ((rg[0]<< 24) | (rg[1] << 16) | ( rg[2] << 8) | (rg[3]));
    d2= ((rn[0]<< 24) | (rn[1] << 16) | ( rn[2] << 8) | (rn[3]));
    d3= ((ri[0]<< 24) | (ri[1] << 16) | ( ri[2] << 8) | (ri[3]));
    d4= ((rs[0]<< 24) | (rs[1] << 16) | ( rs[2] << 8) | (rs[3]));

    wiced_dct_read_unlock( dct_app, WICED_FALSE);

    /* Read & print all DCT sections to check that nothing has changed */
    return WICED_SUCCESS;

}


#endif  /* stdbool.h */
