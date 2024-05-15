/**********************************************************************************
* Copyright (C) 2018-2022 LASECÂ®ï¸� Telecomunicaciones S.A.P.I. de C.V.
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

#ifndef _MANAGER_NET_H
#define _MANAGER_NET_H

#include "wiced.h"
#include "string.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

#include "DCT/dct_data_write.h"
//#include "TXT/manager_menssage_vh.h"
//#include "manager_gpio_vh.h"


typedef struct net_Save
{
     char ssid [30];
     uint8_t ssid_length;
     char pass [30];
     uint8_t pass_length;
     char gate [30];
     uint8_t gate_length;
     char ipser [30];
     uint8_t ipser_length;
     char mask [30];
     uint8_t mask_length;
     uint8_t ipsta [30];
     uint8_t ipsta_length;
     uint8_t msm;

}net;


//Flag to start tcp service
uint8_t flag_tcp = 0;
unsigned char data_on[300];

uint8_t  is_config();
static wiced_result_t Set_SSID(uint8_t *data,uint8_t len,wiced_uart_t uart);
static wiced_result_t Set_KEY(char *data,uint8_t len,wiced_uart_t uart);
static wiced_result_t Set_MASK(uint8_t *data,uint8_t len,wiced_uart_t uart);
static wiced_result_t Set_GATEWAY(uint8_t *data,uint8_t len,wiced_uart_t uart);
static wiced_result_t Set_SERVER(uint8_t *data,uint8_t len,wiced_uart_t uart);
static wiced_result_t Set_IP(uint8_t *data,uint8_t len,wiced_uart_t uart);
static wiced_result_t print_app_dct( wiced_uart_t uart );


static wiced_result_t Set_SSID(uint8_t *data,uint8_t len,wiced_uart_t uart){
    //dct_read_write_app_dct_t*       app_dct                  = NULL;
    wiced_result_t res;
    platform_dct_wifi_config_t*  wifi_config;
   //Se genera un cadena sin signo para almancenar temporalmente la cadena que llega , quitando los primeros dos datos
        unsigned char str_r[len];
          strcpy(str_r,&data[2]);

          // Get a copy of the WIFT config from the DCT into RAM
          if(wiced_dct_read_lock((void**) &wifi_config, WICED_TRUE, DCT_WIFI_CONFIG_SECTION, 0, sizeof(platform_dct_wifi_config_t)) != WICED_SUCCESS )
          {
              return WICED_ERROR;
          }

          strcpy((char *) wifi_config->stored_ap_list[0].details.SSID.value, str_r);
          wifi_config->stored_ap_list[0].details.SSID.length = strlen(str_r);

          res=wiced_dct_write((const void *) wifi_config, DCT_WIFI_CONFIG_SECTION, 0, sizeof(platform_dct_wifi_config_t));
          char mensage[30];
          sprintf(mensage,"W_SSID: %s",str_r);

          if(res == WICED_SUCCESS){
              wiced_uart_transmit_bytes( uart,mensage, strlen(mensage));
            wiced_uart_transmit_bytes( uart, ("\r\n"),2);
              //return WICED_SUCCESS;
            }

          wiced_dct_read_unlock(wifi_config, WICED_TRUE);


        /* Read & print all DCT sections to check that nothing has changed */
          return WICED_SUCCESS;
}

static wiced_result_t Set_KEY(char *data,uint8_t len,wiced_uart_t uart){
    //dct_read_write_app_dct_t*       app_dct                  = NULL;
        wiced_result_t res;
        platform_dct_wifi_config_t*  wifi_config;
       //Se genera un cadena sin signo para almancenar temporalmente la cadena que llega , quitando los primeros dos datos

       unsigned char str_r[len];
       strncpy(str_r,&data[2],len);
       // Get a copy of the WIFT config from the DCT into RAM
       if(wiced_dct_read_lock((void**) &wifi_config, WICED_TRUE, DCT_WIFI_CONFIG_SECTION, 0, sizeof(platform_dct_wifi_config_t)) != WICED_SUCCESS )
       {
           return WICED_ERROR;
       }

       strcpy((char *) wifi_config->stored_ap_list[0].security_key, str_r);
       wifi_config->stored_ap_list[0].security_key_length = strlen(str_r);

      res= wiced_dct_write((const void *) wifi_config, DCT_WIFI_CONFIG_SECTION, 0, sizeof(platform_dct_wifi_config_t));
       char mensage[30];
       sprintf(mensage,"W_KEY: %s",str_r);

       if(res == WICED_SUCCESS){
           wiced_uart_transmit_bytes( uart,mensage, strlen(mensage));
         wiced_uart_transmit_bytes( uart, ("\r\n"),2);
           //return WICED_SUCCESS;
         }
       wiced_dct_read_unlock(wifi_config, WICED_TRUE);


     /* Read & print all DCT sections to check that nothing has changed */
       return WICED_SUCCESS;
}
static wiced_result_t Set_MASK(uint8_t *data,uint8_t len,wiced_uart_t uart){
        dct_read_write_app_dct_t*       app_dct                  = NULL;
        wiced_result_t res;

        unsigned char str_r[len];
        strcpy(str_r,&data[2]);

        /* get the App config section for modifying, any memory allocation required would be done inside wiced_dct_read_lock() */
        wiced_dct_read_lock( (void**) &app_dct, WICED_TRUE, DCT_APP_SECTION, 0, sizeof( *app_dct ) );
        /* Modify string_var by writing the whole DCT */
        strcpy( app_dct->MASK, str_r );
        res=wiced_dct_write( (const void*) app_dct, DCT_APP_SECTION, 0, sizeof(dct_read_write_app_dct_t) );
        char mensage[30];
        sprintf(mensage,"W_MASK: %s",str_r);

        if(res == WICED_SUCCESS){
            wiced_uart_transmit_bytes( uart,mensage, strlen(mensage));
          wiced_uart_transmit_bytes( uart, ("\r\n"),2);
            //return WICED_SUCCESS;
          }
        wiced_dct_read_unlock( app_dct, WICED_FALSE);

        /* Read & print all DCT sections to check that nothing has changed */
        return WICED_SUCCESS;

}

static wiced_result_t Set_IP(uint8_t *data,uint8_t len,wiced_uart_t uart){

    dct_read_write_app_dct_t*       app_dct                  = NULL;
    wiced_result_t res;
           unsigned char str_r[len];
           strcpy(str_r,&data[2]);
           /* get the App config section for modifying, any memory allocation required would be done inside wiced_dct_read_lock() */
           wiced_dct_read_lock( (void**) &app_dct, WICED_TRUE, DCT_APP_SECTION, 0, sizeof( *app_dct ) );

           /* Modify string_var by writing the whole DCT */
           strcpy( app_dct->IP, str_r );

           res=wiced_dct_write( (const void*) app_dct, DCT_APP_SECTION, 0, sizeof(dct_read_write_app_dct_t) );
           char mensage[30];
           sprintf(mensage,"W_IP: %s",str_r);
           if(res == WICED_SUCCESS){
               wiced_uart_transmit_bytes( uart,mensage, strlen(mensage));
               wiced_uart_transmit_bytes( uart, ("\r\n"),2);
               //return WICED_SUCCESS;
             }

           /* release the read lock */
           wiced_dct_read_unlock( app_dct, WICED_FALSE);

         /* Read & print all DCT sections to check that nothing has changed */
           return WICED_SUCCESS;
}
static wiced_result_t Set_SERVER(uint8_t *data,uint8_t len,wiced_uart_t uart){

    dct_read_write_app_dct_t*       app_dct                  = NULL;
    wiced_result_t res;

    unsigned char str_r[len];
          strcpy(str_r,&data[2]);

           /* get the App config section for modifying, any memory allocation required would be done inside wiced_dct_read_lock() */
           if(wiced_dct_read_lock( (void**) &app_dct, WICED_TRUE, DCT_APP_SECTION, 0, sizeof( *app_dct ) ) != WICED_SUCCESS )
           {
               return WICED_ERROR;
           }

           /* Modify string_var by writing the whole DCT */
           strcpy( app_dct->SERVER, str_r );

           res=wiced_dct_write( (const void*) app_dct, DCT_APP_SECTION, 0, sizeof(dct_read_write_app_dct_t) );
           char mensage[30];
           sprintf(mensage,"W_SERVER: %s",str_r);
           if(res == WICED_SUCCESS){
               wiced_uart_transmit_bytes( uart,mensage, strlen(mensage));
             wiced_uart_transmit_bytes( uart, ("\r\n"),2);
               //return WICED_SUCCESS;
             }
           /* release the read lock */
           wiced_dct_read_unlock( app_dct, WICED_FALSE);

         /* Read & print all DCT sections to check that nothing has changed */
           return WICED_SUCCESS;
}
static wiced_result_t Set_GATEWAY(uint8_t *data,uint8_t len,wiced_uart_t uart){
    dct_read_write_app_dct_t*       app_dct                  = NULL;
    wiced_result_t res;
           unsigned char str_r[len];
           strcpy(str_r,&data[2]);
           /* get the App config section for modifying, any memory allocation required would be done inside wiced_dct_read_lock() */
           wiced_dct_read_lock( (void**) &app_dct, WICED_TRUE, DCT_APP_SECTION, 0, sizeof( *app_dct ) );

           /* Modify string_var by writing the whole DCT */
           strcpy( app_dct->GATE, str_r );

           res=wiced_dct_write( (const void*) app_dct, DCT_APP_SECTION, 0, sizeof(dct_read_write_app_dct_t) );
           char mensage[30];
           sprintf(mensage,"W_GATEWAY: %s",str_r);
           if(res == WICED_SUCCESS){
               wiced_uart_transmit_bytes( uart,mensage, strlen(mensage));
             wiced_uart_transmit_bytes( uart, ("\r\n"),2);
               //return WICED_SUCCESS;
             }
           /* release the read lock */
           wiced_dct_read_unlock( app_dct, WICED_FALSE);

         /* Read & print all DCT sections to check that nothing has changed */
           return WICED_SUCCESS;
}

uint8_t is_config(){
    dct_read_write_app_dct_t* dct_app = NULL;
    uint8_t value;
//    if ( wiced_dct_read_lock( (void**) &dct_app, WICED_FALSE, DCT_APP_SECTION, 0, sizeof( *dct_app ) ) != WICED_SUCCESS )
//        {
    if ( wiced_dct_read_lock( (void**) &dct_app, WICED_TRUE, DCT_APP_SECTION, 0, sizeof( *dct_app ) ) != WICED_SUCCESS )
    {
        return WICED_ERROR;
    }
    // Get a copy of the WIFT config from the DCT into RAM
    value=(int)dct_app->F_Sate;
    WPRINT_APP_INFO(("\n Estado  principal : %d\n",value));

    wiced_dct_read_unlock( dct_app, WICED_FALSE );

return value;
}

static wiced_result_t ready_net(struct net_Save sf){
    WPRINT_APP_INFO(("\nSetting Network\r\n"));

    Set_SSID(sf.ssid,sf.ssid_length,WICED_UART_1);
    Set_KEY(sf.pass,sf.pass_length,WICED_UART_1);
    Set_SERVER(sf.ipser,sf.ipser_length,WICED_UART_1);
    Set_GATEWAY(sf.gate,sf.gate_length,WICED_UART_1);
    Set_MASK(sf.mask,sf.mask_length,WICED_UART_1);
//    Set_IP("-C10.86.10.151",15,WICED_UART_1);
    Set_IP(sf.ipsta,sf.ipsta_length,WICED_UART_1);

    return WICED_SUCCESS;

}

#endif  /* stdbool.h */
