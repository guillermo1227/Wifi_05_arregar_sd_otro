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



#ifndef _MAIN_UART_H
#define _MAIN_UART_H

#include "wiced.h"
#include "stdbool.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

#include "waf_platform.h"

#define  MAC_OTA   "12:23:34:45:56:67"

wiced_bool_t reg_incoming=WICED_FALSE;
wiced_bool_t tcp_down_connect=WICED_FALSE;


//#include "TXT/Defines.h"
#include "TXT/manager_menssage_vh.h"
#include "DATA/Data_control.h"
//#include "TIMER/manager_timer_vh.h"
//#include "NET/manager_tcp_client_vh.h"

#define buff_aux    100
#define filter_size     15

static wiced_thread_t UART_M3;

struct telemetry_data log_telemetry_data;
struct location_data master_data;
struct tempo AUX_BEACON[buff_aux];
struct tempo_collision aux_log_collision[buff_aux];

typedef struct condition_G
{
   int8_t menor;
   int8_t inter;
   char mac_Geosf[17];
}data_g;
struct condition_G data_geosef;

typedef struct data_in
{
     unsigned char type [17];
     unsigned char mac_bt[17];
     unsigned char Date[17];
     unsigned char rssi[4];
     unsigned char fallen[2];


}dataa;

struct data_in data_btt [100+10];


typedef struct
{
    uint8_t mac_rewrite[6]; /**< Unique 6-byte MAC address */
} bt_mc_cyp;

bt_mc_cyp mac_Re;

wiced_bool_t Save_bt_gatw=WICED_FALSE;

bt_mc_cyp re_mac(wiced_mac_t mac){
    bt_mc_cyp mm;
    int v;
    int c_c=0;
    for(int v=5;v>=0;v--){
        if(mac.octet[v]==0){
            mm.mac_rewrite[v]=c_c+1;
            c_c=c_c+1;
        }
        else
            mm.mac_rewrite[v]=mac.octet[v];
    }
    return mm;
}

wiced_uart_config_t uart_config =
{
    .baud_rate    = 230400,
    .data_width   = DATA_WIDTH_8BIT,
    .parity       = NO_PARITY,
    .stop_bits    = STOP_BITS_1,
    .flow_control = FLOW_CONTROL_DISABLED,
};
#define RX_BUFFER_SIZE    128*2
#define limit_data   100

wiced_ring_buffer_t rx_buffer;
DEFINE_RING_BUFFER_DATA( uint8_t, rx_data, RX_BUFFER_SIZE)

char c;
uint32_t expected_data_size = 1;
unsigned char rx_buffer3[RX_BUFFER_SIZE];
char recive3;

char uart3[RX_BUFFER_SIZE];
int k;

uint8_t lcd_fallen=5;
uint8_t count_v=1;
uint8_t count_l=1;
uint8_t* proximity=5;
wiced_bool_t Evacaution=WICED_FALSE;

int count_beacon=1;
int count_save=1;
int count_collision=1;
int count_save_collision=1;


struct Acarreos log_accarreos;


struct Gatw_bengala                  /* <--- Mi esrtrucutra ---> */
{
    unsigned char mac_bt_gatw[19];
    uint8_t num_lamp;
    uint8_t nodo;
};
struct Gatw_bengala log_node[100];
int limit_log=0;



int count_char(const char *cadena, char caracter) {
    int contador = 0;
    size_t longitud = strlen(cadena);

    for (size_t i = 0; i < longitud; i++) {
        if (cadena[i] == caracter) {
            contador++;
        }
    }

    return contador;
}



void SEND_OTA(unsigned char* buffer_in ){

    unsigned char s_Mac_W[20];
    unsigned char str_split[128];
    strcpy(str_split,&buffer_in[4]);

    if(strstr(str_split,MAC_OTA)){
        wiced_dct_restore_factory_reset( );
        wiced_waf_app_set_boot( DCT_FR_APP_INDEX, PLATFORM_DEFAULT_LOAD );
        wiced_framework_reboot();
    }

}

/*
 * main loop to uart process  1 info.
 *
 * */
void main_uart(wiced_thread_arg_t arg){
        char str;
        while(1){
//            WPRINT_APP_INFO( ("mutex \n") );

        /* Wait for user input. If received, echo it back to the terminal */
        while ( wiced_uart_receive_bytes( WICED_UART_1, &c, &expected_data_size, NEVER_TIMEOUT   ) == WICED_SUCCESS )
        {
            wiced_rtos_lock_mutex(&HTTPMutex);

//            wiced_uart_transmit_bytes( WICED_UART_1, &c, 1);

            if((c == '\n')||(k==RX_BUFFER_SIZE)){
                if(k==0){
                    memset(&rx_buffer3,'\0',RX_BUFFER_SIZE);
                    memset(&rx_buffer,'\0',100);
                    memset(&c,'\0',k);
                    k=0;
                    expected_data_size=1;
                }
                else{
                    sprintf(uart3,"%s\n",rx_buffer3);
//                    wiced_uart_transmit_bytes( WICED_UART_1, uart3, strlen(uart3));

                    fill_dat_bengala(rx_buffer3); /* Localizacion MESH|NODO|LAMP/VEHICULE|MAC */

                    //fill_Node_bengala(rx_buffer3); /* Fill node information  */

                    memset(&rx_buffer3,'\0',RX_BUFFER_SIZE);
                    memset(&rx_buffer,'\0',100);
                    memset(&c,'\0',k);
                    k=0;
                    expected_data_size=1;
                }

            }
            else if(recive3 == '\r'){}
            else{ rx_buffer3[k++] = c; }

            expected_data_size = 1;
            wiced_rtos_unlock_mutex(&HTTPMutex);
            }
        }
        wiced_rtos_delay_milliseconds(1); // Yield control when button is not pressed
    }

void uart_int(){
    /* Initialise ring buffer */
//       ring_buffer_init(&rx_buffer, rx_data, RX_BUFFER_SIZE );

       /* Initialise UART. A ring buffer is used to hold received characters */
       wiced_uart_init( WICED_UART_1, &uart_config, NULL );
   //    wiced_gpio_output_low(WICE_GPIO_12);

       /* Send a test string to the terminal */
       wiced_uart_transmit_bytes( WICED_UART_1, TEST_STR, sizeof( TEST_STR ) - 1 );


       wiced_rtos_create_thread(&UART_M3, THREAD_BASE_PRIORITY+2, "UART MAIN", main_uart, THREAD_STACK_SIZE, NULL);
}

uint8_t Nodo, T_lamparas=0,N_lamp=0, down_v,up_v, position=0;
void fill_dat_bengala(char* input)
{
    static uint8_t count=0;
    if(count ==0)   /* Limpio todo */
    {
        for(uint8_t i=0;i <= 100;i++)           /* Termino en 100 ya que el 0 no lo uso */
        {
            memset(log_node[i].mac_bt_gatw,0,19);
        }
        count++;
    }

    _Mac_Gat = WICED_FALSE;
    int x=0;
    unsigned char str_split[40], mac_bt[17],device[5];
    memset(str_split,'\0',40);

    memcpy(text_gatw,input,3);

    if(strstr(text_gatw,_GAT_MENS))
    {
        memcpy(str_split,input,strlen(input));

        /* Realizo el strtok para separar texto de nelson */
        char * frist_split;
        frist_split=strtok(str_split,_split_tama_2);

        while(frist_split!=NULL)
        {
            switch (x)
            {
            case 0:
                /* MESH no hago nada */        /* MESH|    5    |     3     |      5      | LAMP|   Mac Bt  */
                break;                                 /* NODO   No. lampara  Cantidad To         Mac Bluetooth*/
            case 1:
                /* Numero de nodo */          /* 5 */
                Nodo = atoi(frist_split);
                break;
            case 2:
                /* ID lampara, significa el numero de lampara que ocupa esa lampara detectada  si ve 10, enviaria un numero 2 */
                N_lamp = atoi(frist_split);   /* 4 */
                break;
            case 3:
                /* Cantidad de lamparas */
                T_lamparas = atoi(frist_split);
                break;
            case 4:
                memcpy(device,frist_split,4);  /* LAMP o VEHI */
                break;
            case 5:
                /* Mac del dispositivo */
                memcpy(mac_bt,frist_split,17);
                printf("MAC:%s\n",mac_bt);
                break;
            default:
                break;
            }
            x++;
            frist_split=strtok(NULL,_split_tama_2);
        }

        /* 1.- Identifico donde lo voy a poner, seccio de lamapras o seccion de vehiclos */

        /* si dependiedo el tipo de nodo en esta posicion guardo algo, tengo 100 dato, al nodo 1 le doy
         * 1 a 5, al nodo 2 le doy de 6 a 10 */
        /* 1.- Primero dependiendo del numero de nodo voy a sacar desde que parte hasta que parte el puede guardar informacion  */
        if(strstr("LAMP",device))
        {
            up_v   = Nodo * 5;
            down_v = up_v - 5;
            position = down_v + N_lamp;
        }
        else    /* Vehicule */
        {
            up_v   = Nodo * 10;
            down_v = up_v - 5;
            position = down_v + N_lamp;
        }
        printf("Posicion donde se guardara el dato en mi estrucutra %d \n",position);
        /* 2.- Copiar dato en mi estructura */

            log_node[position].num_lamp=N_lamp;                               /* numero de lampara  */
            memcpy(log_node[position].mac_bt_gatw,mac_bt,strlen(mac_bt)); /* Mac */
            log_node[position].num_lamp = N_lamp;
    }
    else if(strstr(text_gatw,_GAT_CLEAN)) /* Elimino todo */
    {
        for(int i=0;i <= 100;i++)
        {
            memset(log_node[i].mac_bt_gatw,NULL,19);
            log_node[i].num_lamp=0;
            Save_bt_gatw = WICED_FALSE;
        }
    }
}

char* fill_jaso_dat(char* res, int num)
{
    //printf("\n Longitud:%d\n",strlen(log_bengala[num].mac_bt_gatw));${ProjDirPath}/make.exe ww101.Bengala_gateway.Tag_Vehiculo_3_0-CYW943907AEVAL1F ALWAYS=1

    sprintf(res,"{\"Node\":\"%d\",\"T_lamp\":\"%d\",\"No_lamp\":\"[%d]\",\"Mac\":%s}",Nodo,T_lamparas,log_node[num].num_lamp,log_node[num].mac_bt_gatw);
    //printf("\n ***************** Mac %s \n",log_bengala[num].mac_bt_gatw);
    return res;
}

//fill_Node_bengala(char* input)
//{
//    int x=0;
//        unsigned char str_split[40], mac_bt[17];
//        memset(str_split,'\0',40);
//
//        memcpy(text_gatw,input,3);
//
//        if(strstr(text_gatw,_NODE_MENS)) /* Suponiendo que llega el mensaje OPND|TYPE|MAC|ID|INPUT   NODE: Significa el numero de nodo a abrir, es decir en que punto abrira la puerta*/
//        {                                                   /* OPND|1|9C:50:D1:80:44:30|NODE1|0 *//* TYPE: Es igual a abierto 1 o cerrado 0 */
//            memcpy(str_split,input,strlen(input));                                                /* ID: Significa el nuermo de nodo que o esta activando node1 o node2, etc */
//                                                                                                  /* INOUT: Significa el gpio para activar 0:gpio numero 0  1:gpio numero 1*/
//                    /* Realizo el strtok para separar texto de nelson */
//                    char * frist_split;
//                    frist_split=strtok(str_split,_split_tama_2);
//
//                    while(frist_split!=NULL)
//                    {
//                        switch (x)
//                        {
//                        case 0:
//
//                            break;
//                        case 1:
//                            break;
//                        case 2:
//                            break;
//                        case 3:
//                            break;
//                        }
//                        default:
//                            break;
//                    }
//        }
//}

#endif  /* stdbool.h */
