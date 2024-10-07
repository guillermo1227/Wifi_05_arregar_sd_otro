/**********************************************************************************
-* Copyright (C) 2018-2021 LASECÂ®ï¸� Telecomunicaciones S.A.P.I. de C.V.
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
/* Codigo probado con luis hasta ahora la version mas estable la cual cuenta con las siguientes pruebas y caracteristicas
 *
 * --> Si no cuenta con conexion a internet se reinicia a el 2:20 segudnso aprox
 * --> pruebas de reinicos pasada con exito
 * --> Cuanta con acarreos
 * --> Baudios 230400
 * --> Conexion por DHCP desde codigo
 * --> Si se reinicia vuelve a sonar
 * --> mejora de datos enviados en la trama B;
 * --> conexion con wifi solo manda 2 veces un acarreo hecho o 2 veces localizacion por beacons
 * --> Fecha 05/06/2024 */



#include "wiced.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

char aux_date_y[13];
char split_date_api[40];
char aux_time[8];


wiced_bool_t Product_f=WICED_TRUE;
wiced_bool_t GEOSF_F=WICED_FALSE;
uint8_t machineFlagControl=0;

/* RTOS global constructs */
static wiced_semaphore_t displaySemaphore;
static wiced_semaphore_t tcpGatewaySemaphore;
static wiced_semaphore_t tcpReceptionSemaphore;
static wiced_semaphore_t StateMachineSemaphore;
//static wiced_semaphore_t httpWait_post;
//static wiced_semaphore_t httpWait_get;

static wiced_thread_t ThreadHandle_W;

static wiced_thread_t ThreadHandle_C;
static wiced_semaphore_t semaphoreHandle_C;

static wiced_mutex_t HTTPMutex;

static wiced_mutex_t i2cMutex;
static wiced_mutex_t pubSubMutex;
static wiced_queue_t pubQueue;
static wiced_timer_t publishTimer;
static wiced_thread_t displayThreadHandle;
static wiced_thread_t publishThreadHandle;
static wiced_thread_t subscribeThreadHandle;

static wiced_thread_t commandThreadHandle;

struct colliosn_mac_t bt_joined;
wiced_bool_t sent_file_flag=WICED_FALSE;

uint8_t main_c=0;
wiced_i2c_device_t i2c_rtc;
wiced_result_t  flag_time_set=WICED_TRUE;
wiced_result_t  flag_time_set_PUBLISH=WICED_FALSE;

/******************************************************
 *               Variable Definitions watchdog
 ******************************************************/
wiced_system_monitor_t my_thread_monitor;
/* ****************************************************/

#include "RTC/rtc_manager.h"
#include "SD/sd_manager.h"
wiced_filesystem_t fs_handle;

/* Queue configuration */
#define MESSAGE_SIZE        (2)
#define QUEUE_SIZE          (50)

/* Thread parameters */
#define THREAD_BASE_PRIORITY    (10)
#define THREAD_STACK_SIZE       (8192)

unsigned char s_Mac_W[20];
wiced_mac_t MacW;

#include "u8g_arm.h"
#include "NET/gateway.h"
#include "NET/Init_Net_vh.h"
#include "TXT/manager_menssage_vh.h"
#include "UART/main_uart.h"
#include "LCD/lcd_interface.h"
#include "GPIO/manager_gpio_vh.h"
#include "TIMER/manager_timer_vh.h"
#include "NET/wireless_config.h"
#include "NET/manager_net.h"


void application_start( ){

    wiced_init();   /* Initialize the WICED device */

    /* Setup Thread Control functions */
    wiced_rtos_init_mutex(&i2cMutex);
    wiced_rtos_init_mutex(&pubSubMutex);
    wiced_rtos_init_mutex(&HTTPMutex);

    wiced_rtos_init_semaphore(&displaySemaphore);
    wiced_rtos_init_semaphore(&tcpGatewaySemaphore);
    wiced_rtos_init_semaphore(&StateMachineSemaphore);

    wiced_rtos_init_semaphore(&httpWait_get);
    wiced_rtos_init_semaphore(&httpWait_post);


    wiced_rtos_init_semaphore(&semaphoreHandle_C);

    wiced_rtos_init_semaphore(&tcpReceptionSemaphore);

    wiced_rtos_init_queue(&pubQueue, NULL, MESSAGE_SIZE, QUEUE_SIZE);

    wwd_wifi_set_preferred_association_band(WLC_BAND_2G );

//    wwd_wifi_set_bandwidth(40);
//
//    wwd_wifi_set_roam_trigger(2);

    wwd_wifi_set_tx_power(31);

//    wwd_wifi_set_mcs_rate (WICED_STA_INTERFACE, 0, WICED_TRUE);

    Init_gpio();
    init_rtc(&i2c_rtc);
    init_sd(&fs_handle);
    uart_int();
    init_all_timer();



//    date_set("20/12/25",&i2c_rtc);
//     time_set("11:80:00",&i2c_rtc);

    wiced_rtos_create_thread(&displayThreadHandle, THREAD_BASE_PRIORITY+4, NULL, displayThread, THREAD_STACK_SIZE, NULL);

    wiced_wifi_get_mac_address(&MacW);                             // Se obtiene la MAC del dispositivo

    sprintf(s_Mac_W,"%02X:%02X:%02X:%02X:%02X:%02X",MacW.octet[0],MacW.octet[1],MacW.octet[2],MacW.octet[3],MacW.octet[4],MacW.octet[5]);

    dir_verify("/",&fs_handle,"/",ANTICOLISION);
    dir_verify("/",&fs_handle,"/",SF);
    dir_verify("/",&fs_handle,"/",ACARREO);


    file_verify(ACARREO_ROOT,&fs_handle,ACARREO_ROOT,date_get(&i2c_rtc));
    file_verify(ANTICOLISION_ROOT,&fs_handle,ANTICOLISION_ROOT,date_get(&i2c_rtc));
    file_verify(SF_ROOT,&fs_handle,SF_ROOT,date_get(&i2c_rtc));


//    list_files(&fs_handle,ANTICOLISION_ROOT);
//    list_files(&fs_handle,SF_ROOT);
//        list_files(&fs_handle,ACARREO_ROOT);


//    -----version resto de minas----------------------------------------------------------------------
        if(get_gpio_menssage(RESET_WIFI)==1){
//    //        set_gpio_menssage(RESET_WIFI,0);
//    //        silent=get_gpio_menssage(SOUND_ONOFF);
//    //        set_gpio_menssage(SOUND_ONOFF,0);
                silent=WICED_TRUE;
        }

    //-----version velardena descomentar linea de silent-----------------------------------------------------
    //silent=get_gpio_menssage(SOUND_ONOFF); //*******************
//    silent=WICED_TRUE;
//    silent=WIC
//    printf("antes %s \n",bt_joined.mac_lamp);
//    bt_joined.mac_lamp="asaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
//    printf("despues %s \n",bt_joined.mac_lamp);

////
        // Red para provar en lasec
//    Set_SSID("-SCSM-MONITOREO",20,WICED_UART_3);
//    Set_KEY("-KM0n1t0r30#21",20,WICED_UART_3);
    //Set_SERVER("-I10.90.120.10",17,WICED_UART_3);
//    Set_MASK("-M255.255.248.0",15,WICED_UART_3);
//    Set_GATEWAY("-G10.90.120.1",16,WICED_UART_3);
//    Set_IP("-C10.90.120.41",16,WICED_UART_3);

//
////

////
    //Set_SSID("-SSF-DEMO",13,WICED_UART_3);
    //Set_KEY("-KD3_lasec2020.,",17,WICED_UART_3);
    //Set_SERVER("-I10.174.109.33",17,WICED_UART_3);
//    Set_MASK("-M255.255.248.0",15,WICED_UART_3);
//    Set_IP("-C10.174.108.60",16,WICED_UART_3);
//    Set_GATEWAY("-G10.174.107.30",16,WICED_UART_3);
////
//    Set_config();
//
////

//    Set_SSID("-SNEM-OT-OPS",20,WICED_UART_3);
//    Set_KEY("-KOp3r4t!0n0t",20,WICED_UART_3);
//    Set_SERVER("-I10.116.0.230",17,WICED_UART_3);
//    Set_MASK("-M255.255.248.0",15,WICED_UART_3);
//    Set_IP("-C10.117.104.24",20,WICED_UART_3);
//    Set_GATEWAY("-G10.117.103.254",16,WICED_UART_3);


//        Set_SSID("-STracking-SD",13,WICED_UART_3);
//        Set_KEY("-KX%baCU$rC5YC",17,WICED_UART_3);
//        Set_SERVER("-I10.178.72.76",17,WICED_UART_3);
//        Set_MASK("-M255.255.252.0",15,WICED_UART_3);
//        Set_IP("-C10.178.55.50",16,WICED_UART_3);
//        Set_GATEWAY("-G10.178.55.254",16,WICED_UART_3);

//        /* Karim */
        //Set_SSID("-S87377-0184",15,WICED_UART_3);
        //Set_KEY("-KE$#c65&MiK7&uwxc12",22,WICED_UART_3);
        //Set_SERVER("-I172.16.10.40",18,WICED_UART_3);    //10.174.110.100

//        // Fernando tlayoltita
//           Set_SSID("-Ssmartflow-dev",20,WICED_UART_3);
//           Set_KEY("-KLasec123.",20,WICED_UART_3);
//           Set_SERVER("-I10.174.109.33",20,WICED_UART_3); //10.174.107.60
//           Set_MASK("-M255.255.248.0",16,WICED_UART_3);
//           Set_GATEWAY("-G10.174.107.30",16,WICED_UART_3);
//           Set_IP("-C10.174.104.4",16,WICED_UART_3);
//
//        Set_config();

//            silent=WICED_TRUE;
    sent_file_flag=WICED_TRUE;
//    Set_SSID("-SL2",18,WICED_UART_3);
//    Set_KEY("-KLasec123.",18,WICED_UART_3);

//    buzz(10,0);
//    buzz(150,0);
            main_c=is_config();
            printf("\n Esto vale el is_config %d\n",main_c);



    switch (main_c) {
              case 0:
                  set_name();

//                  Set_SSID("-Ssmartflow-dev",20,WICED_UART_3);
//                  Set_KEY("-KLasec123.",20,WICED_UART_3);
//                  Set_SERVER("-I10.90.120.10",17,WICED_UART_3);
//                  Set_config();

                  Set_SSID("-Ssmartflow-dev",20,WICED_UART_3);
                  Set_KEY("-KLasec123.",20,WICED_UART_3);
                  Set_SERVER("-I10.174.109.33",20,WICED_UART_3);
//                  Set_MASK("-M255.255.248.0",16,WICED_UART_3);
//                  Set_GATEWAY("-G10.174.107.30",16,WICED_UART_3);
//                  Set_IP("-C10.174.104.5",16,WICED_UART_3);
//                  Set_config();

                  net_config();
                  break;
              case 3:
                  init_tcp_w();
                   net_vehicle();
                  break;
              case 51:

                  break;
              default:
                  break;
          }

}

/*************** UART Command Interface Thread ***************/
/* Tread to handle UART command input/output */
void commandThread(wiced_thread_arg_t arg)
{
//    check_sound();
}
