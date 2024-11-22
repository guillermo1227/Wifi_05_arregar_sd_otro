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
#define buff_aux_4    7
#define filter_size     15

static wiced_thread_t UART_M3;

struct telemetry_data log_telemetry_data;
struct location_data master_data;
//struct location_data master_data2[30];
struct location_data_online master_data2[30];
struct location_data data_alone; /* ********* */
struct location_data master_data3;
struct tempo AUX_BEACON[buff_aux];
struct tempo_collision aux_log_collision[buff_aux];


typedef struct data_in
{
     unsigned char type [17];
     unsigned char mac_bt[17];
     unsigned char Date[17];
     unsigned char rssi[4];
     unsigned char fallen[2];


}dataa;

struct data_in data_btt [100+10];
struct data_in data_B;

typedef struct
{
    uint8_t mac_rewrite[6]; /**< Unique 6-byte MAC address */
} bt_mc_cyp;

bt_mc_cyp mac_Re;

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

                    lcd_data_update(rx_buffer3,&count_v,&count_l,&proximity);
                    lcd_fallen_update(rx_buffer3,&lcd_fallen);      //Aqui si llega BNM y BEAC ---> _B_transit=WICED_TRUE; si fal==1 ---> fallen_f=WICED_TRUE;
//                    SEND_OTA(rx_buffer3);
                    data_file_write(rx_buffer3);       /* HE */
//                    get_join_macbt(rx_buffer3);
//                    collision_event_macbt(rx_buffer3);
//                    collision_event_beacon(rx_buffer3);
                    data_bt_send(rx_buffer3);                   /* Acomodo datos para karim Gateway*/

                    tamagochi(rx_buffer3,&log_accarreos);
//                    limit_log=id_revived(rx_buffer3);

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


void data_file_write(unsigned char* buffer_in ){      /* Funcion donde se llenan los datos de localizacion */
    unsigned char str_switch[4];
    unsigned char str_split[128];
    uint8_t count_HE=0;

    wiced_bool_t wirte1=WICED_FALSE;

    strncpy(str_switch,buffer_in,4);
    memcpy(str_split,buffer_in,strlen(buffer_in));

    char delim[2] = ",";
    int x=0;

    if((strstr(buffer_in,"BNM|"))&&((strstr(buffer_in,"GEOSF")))&&((strstr(buffer_in,"LAMP"))==NULL)&&((strstr(buffer_in,"VEHI"))==NULL)){
        _B_transit=WICED_TRUE;
        unsigned char *cvl_1 = strtok(str_split, "|");
        cvl_1=strtok(NULL, "|");
        reg_incoming=WICED_TRUE;
                //            count_save=1;
        GEOSF_F=WICED_TRUE;

        unsigned char *cvl1 = strtok(cvl_1, delim);
        while(cvl1 != NULL){

            switch (x) {
                case 0:
                    if((strlen(cvl1)>=filter_size)&&(count_char(cvl1,':')==5)){  //Tama�o mayor a 15, y si hay 5 : ocalizados en la cadena
                        for(int b=1;b<buff_aux_4;b++){
                            if(!(strstr(AUX_BEACON[b].mac_bt,cvl1))){ /* No esta aqui */
                            //                                AUX_BEACON[b].flag=0;
                            //                                printf("no existe \n");
                            //                                wirte=WICED_FALSE;

                            }
                            else{
                                AUX_BEACON[b].flag=1;
                                printf("si existe \n");
                                if(strlen(AUX_BEACON[b].time_start)!=0){ /* Si ya tiene registro de entrada, se pone el registro de salida */
                                    strcpy(AUX_BEACON[b].time_end,time_get(&i2c_rtc));
                                    printf("OK end\n");
                                }
                                AUX_BEACON[b].flag=1;
                                wirte1=WICED_TRUE;
                            }
                        }
                        if(wirte1==WICED_FALSE){  /* No esta, guardamos la mac de beacon */
                            wirte1=WICED_TRUE;
                            //if(count_beacon<buff_aux){
                                printf("-------->Si Guardo\n");
                                for(uint8_t i=1;i<buff_aux_4;i++)  /* count beacon = 3 */
                                {
                                    if(strlen(AUX_BEACON[i].mac_bt)==0 || strlen(AUX_BEACON[i].mac_bt)==NULL ||
                                            strlen(AUX_BEACON[i].mac_bt) < 3)
                                    {
                                        memcpy(AUX_BEACON[i].mac_bt,cvl1,17);
                                        if(strlen(AUX_BEACON[i].time_start)<1)
                                        {
                                            strcpy(AUX_BEACON[i].time_start,time_get(&i2c_rtc));
                                            //printf("OK BEAC GEOSF\n");
                                            printf("\n Variable para flujo de lo guardado de GEOSF %d\n",i);
                                            //count_beacon=+1;
                                        }
                                        break;
                                    }
                                }
                            //                                count_beacon++;
                        }   /* Termina */
                    }
                break;
                case 1:
//                        strcpy(data_btt[s_count_x+1].type,cvl1);
//                        memcpy(master_data.bt_device.type,cvl1,17);

                    break;
                case 2:
//                        strcpy(data_btt[s_count_x+1].rssi,cvl1);
                    break;
                case 3:
//                        strcpy(data_btt[s_count_x+1].fallen,cvl1);
                    break;
                default:
                    break;
            }
            x++;
            cvl1=strtok(NULL, delim);
        }
        x=0;

    }
}

void get_join_macbt(unsigned char* buffer_in){
    unsigned char str_switch[3];
        unsigned char str_split[RX_BUFFER_SIZE];
        unsigned char pem_mac[17];
        unsigned char str_temp[17];


        strncpy(str_switch,buffer_in,3);
//        WPRINT_APP_INFO( (">>VBT \r\n") );
//         WPRINT_APP_INFO( ("%s \r\n",buffer_in) );

        char delim[] = "$";     //establece como  realizara el split
        int x=0;
        if((memcmp(str_switch,"MBL",3)==0)&&(strlen(buffer_in)<=RX_BUFFER_SIZE-1)){

            memcpy(str_split,&buffer_in[3],strlen(buffer_in)-2);

            WPRINT_APP_INFO( ("%s \r\n",str_split) );

            unsigned char *cvl1 = strtok(str_split, delim);
            while(cvl1 != NULL){
//                WPRINT_APP_INFO( ("strtok -> %s \r\n",cvl1) );
//                strcpy(pem_mac,cvl1);
//                WPRINT_APP_INFO( ("memecpy -> %s \r\n",pem_mac) );
                switch (x) {
                    case 0:
//                        memcpy(data_btt[s_count_x+1].mac_bt,cvl1,17);
//                        printf("\tMAC ABORDADAS 0\t %s\n",cvl1 );

                    break;
                    case 1:
//                        strcpy(data_btt[s_count_x+1].type,cvl1);
                        printf("\tMAC ABORDADAS 1\t %s longuitud %d \t\n",cvl1 ,strlen(cvl1));
//                       bt_joined.mac_lamp=cvl1;
//                        char aux[strlen(cvl1)];
//                        sprintf(aux,"%s",cvl1);
                        memcpy(bt_joined.mac_lamp,cvl1,strlen(cvl1)-1);

                        break;

                    default:
                        break;
                }
                x++;
                cvl1=strtok(NULL, delim);
            }
            x=0;


    }
}

/*
 * BNM:9C:50:D1:1A:4D:C0,LAMP,0,1
 * */
void collision_event_macbt(unsigned char* buffer_in){
    unsigned char str_switch[4];
    unsigned char str_split[128];
    unsigned char pem_mac[17];
    unsigned char str_temp[17];

    wiced_bool_t wirte_1=WICED_FALSE;
    printf("%s\n",buffer_in);

    strncpy(str_switch,buffer_in,4);
    memcpy(str_split,buffer_in,strlen(buffer_in));

    char delim[] = ",";     //establece como  realizara el split
    int x=0;
    if((strstr(buffer_in,"BNM|"))&&(strstr(buffer_in,"X"))&&((strstr(buffer_in,"GEOSF")==NULL))){

        unsigned char *cvl_1 = strtok(str_split, "|");
            cvl_1=strtok(NULL, "|");


//            printf("%s\n",str_split);
//            printf("%s\n",cvl_1);
    unsigned char *cvl1 = strtok(cvl_1, delim);
    while(cvl1 != NULL){
//                WPRINT_APP_INFO( ("strtok -> %s \r\n",cvl1) );
//                strcpy(pem_mac,cvl1);
//                WPRINT_APP_INFO( ("memecpy -> %s \r\n",pem_mac) );
        switch (x) {
            case 0:
//                memcpy(data_btt[s_count_x+1].mac_bt,cvl1,17);
//                printf("%s\n",cvl1);
//                strcpy( bt_joined.mac_tag,cvl1);
/*--------------------------------------------------------------------------------*/
                printf("cuentas %d\n",count_char(cvl1,':'));
                if((strlen(cvl1)>=filter_size)&&(count_char(cvl1,':')==5)){
                        for(int x=1;x<buff_aux;x++){
                            if((strstr(aux_log_collision[x].mac_bt,cvl1))){
                                printf("the collision continues: %s \n",cvl1);
                                wirte_1=WICED_TRUE;
                                aux_log_collision[x].flag=1;
                            }
                        }
                        if(wirte_1==WICED_FALSE){
                            wirte_1=WICED_TRUE;
                            printf("Collison number %d\n",count_collision);
                            if(count_collision<buff_aux){
                                memset(aux_log_collision[count_collision].mac_bt,NULL,18);
                                strncpy(aux_log_collision[count_collision].mac_bt,cvl1,17);
//                                definir la condicion

                                if(strlen(AUX_BEACON[count_beacon].time_start)<1){
                                    strncpy(aux_log_collision[count_collision].time_start,time_get(&i2c_rtc),17);
                                }
                                count_collision=count_collision+1;
                            }
                        }
                }
/*--------------------------------------------------------------------------------*/

            break;
            case 1:
//                strcpy(data_btt[s_count_x+1].type,cvl1);
                break;
            case 2:
//                strcpy(data_btt[s_count_x+1].rssi,cvl1);
                break;
            case 3:
//                strcpy(data_btt[s_count_x+1].fallen,cvl1);
                break;
            case 4:
//                        stun mrcpy(data_btt[s_count_x+1].fallen,cvl1);
                printf("%s\n",cvl1);
//                wiced_rtos_set_semaphore(&semaphoreHandle_C); /* Set the semaphore */

                break;
            default:
                break;
        }
        x++;
        cvl1=strtok(NULL, delim);
    }
    x=0;



//
    }
//
//
//
//
//
}


void collision_event_beacon(unsigned char* buffer_in){
    unsigned char str_switch[4];
    unsigned char str_split[128];
    unsigned char pem_mac[17];
    unsigned char str_temp[17];

    strncpy(str_switch,buffer_in,4);
    strcpy(str_split,&buffer_in[4]);

    char delim[] = ",";     //establece como  realizara el split
    int x=0;
    if((strstr(str_switch,"BNM:"))&&(strstr(str_switch,"BEAC"))&&((strstr(str_switch,"GEOSF")==NULL))){

    unsigned char *cvl1 = strtok(str_split, delim);
    while(cvl1 != NULL){
//                WPRINT_APP_INFO( ("strtok -> %s \r\n",cvl1) );
//                strcpy(pem_mac,cvl1);
//                WPRINT_APP_INFO( ("memecpy -> %s \r\n",pem_mac) );
        switch (x) {
            case 0:
//                memcpy(data_btt[s_count_x+1].mac_bt,cvl1,17);
//                printf("%s\n",cvl1);
                strcpy( bt_joined.mac_beacon,cvl1);

//                bt_joined.mac_beacon
//                printf("\t%s\n",bt_joined.mac_tag);
//                printf("\t%s\n",bt_joined.mac_beacon);
//                printf("\t%s\n",bt_joined.mac_lamp);
//                printf("\t%s\n",bt_joined.mac_vehc);

            break;
            case 1:
//                strcpy(data_btt[s_count_x+1].type,cvl1);
                break;
            case 2:
//                strcpy(data_btt[s_count_x+1].rssi,cvl1);
                break;
            case 3:
//                strcpy(data_btt[s_count_x+1].fallen,cvl1);
                break;
            case 4:
//                        stun mrcpy(data_btt[s_count_x+1].fallen,cvl1);
                printf("%s\n",cvl1);

                break;
            default:
                break;
        }
        x++;
        cvl1=strtok(NULL, delim);
    }
    x=0;


    }

}
#endif  /* stdbool.h */
