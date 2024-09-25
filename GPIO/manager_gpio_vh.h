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



#ifndef _MANAGER_GPIO_VH_H
#define _MANAGER_GPIO_VH_H
#include "stdbool.h"
#include "wiced.h"
#include "platform_peripheral.h"
#include "platform_init.h"

#include "TXT/manager_menssage_vh.h"
#include "DCT/manager_dct.h"



/*
 * WICED_GPIO_19 |   | SPI_0_MISO
 * WICED_GPIO_20 |   | SPI_0_CLK
 * WICED_GPIO_21 |   | SPI_0_MOSI
 * WICED_GPIO_22 |   | SPI_0_CS
 * */

/*
 * WICED_GPIO_7  |   | PWM2
 * WICED_GPIO_12 |   | PWM3
 * WICED_GPIO_9  |   | S1
 * WICED_GPIO_8  |   | S2
 * */

//
//#define telemetry_1         WICED_GPIO_19
//#define telemetry_2         WICED_GPIO_20
//#define telemetry_3         WICED_GPIO_21
//#define telemetry_4         WICED_GPIO_22

#define telemetry_1         WICED_GPIO_15    //ok
#define telemetry_2         WICED_GPIO_16  //ok
#define telemetry_3         WICED_GPIO_5    //ok
#define telemetry_4         WICED_GPIO_7


#define STAT_CHARGER          WICED_GPIO_6     //gpio10
#define GPIO_ON_OF            WICED_GPIO_8     //gpio12
#define GPIO_HELP             WICED_GPIO_10    //gpio14

#define XRES_BLE              WICED_GPIO_2      //gpio1

//revisar los gpios
#define PWM2    WICED_GPIO_12
#define PWM3    WICED_GPIO_12

#define Sat_WiFi            WICED_GPIO_1    //gpio 0


#define WIFI_BLINK  (75)
#define TRIG_TIME   (25)
#define DETECT_BLINK  (15)

#define BUZ_TIME    (20)

#define authorization       WICED_GPIO_11     //gpio15
//#define anticolision_pulso        WICED_GPIO_12     //gpio16

#define stop_bt     "GSTOP\n"

wiced_bool_t button_down_net=WICED_FALSE;


    wiced_bool_t t1;

    wiced_bool_t t2;

    wiced_bool_t t3;

    wiced_bool_t t4;

    wiced_bool_t send_data_task=WICED_FALSE;

uint8_t c_silent=1;
wiced_bool_t silent=WICED_FALSE;
wiced_bool_t frist_seen_silent=WICED_FALSE;
wiced_bool_t _sound_flag=WICED_FALSE;

wiced_result_t set_gpio();
void Init_gpio();
void  blink_vehicle();
//void buzz();
void btn_stop();
void count_on_off();
void reset_server();

void tt1(){
    WPRINT_APP_INFO( ("gpio 1 \n") );
    wiced_gpio_input_get_bool(telemetry_1,&t1);

}
void tt2(){
    WPRINT_APP_INFO( ("gpio 2 \n") );
    wiced_gpio_input_get_bool(telemetry_2,&t2);

}
void tt3(){
    WPRINT_APP_INFO( ("gpio 3 \n") );
    wiced_gpio_input_get_bool(telemetry_3,&t3);

}
void tt4(){
    WPRINT_APP_INFO( ("gpio 4 \n") );
    wiced_gpio_input_get_bool(telemetry_4,&t4);

}
void Init_gpio(){
    set_gpio();

    wiced_gpio_input_irq_enable(GPIO_HELP, IRQ_TRIGGER_BOTH_EDGES, btn_stop, NULL); /* Setup interrupt */
//
    wiced_gpio_input_irq_enable(WICED_BUTTON1, IRQ_TRIGGER_FALLING_EDGE, reset_server, NULL); /* Setup interrupt */
//
    wiced_gpio_input_irq_enable(GPIO_ON_OF, IRQ_TRIGGER_BOTH_EDGES, count_on_off, NULL); /* Setup interrupt */

//    wiced_gpio_input_irq_enable(telemetry_1, IRQ_TRIGGER_BOTH_EDGES, tt1, NULL); /* Setup interrupt */
//    wiced_gpio_input_irq_enable(telemetry_2, IRQ_TRIGGER_BOTH_EDGES, tt2, NULL); /* Setup interrupt */
//    wiced_gpio_input_irq_enable(telemetry_3, IRQ_TRIGGER_BOTH_EDGES, tt3, NULL); /* Setup interrupt */
//    wiced_gpio_input_irq_enable(telemetry_4, IRQ_TRIGGER_BOTH_EDGES, tt4, NULL); /* Setup interrupt */

//

}


wiced_result_t set_gpio(){
    WPRINT_APP_INFO( ("SET GPIO MCU \n") );

    wiced_gpio_deinit(WICED_LED1); // Need to de-init the GPIO
    wiced_gpio_deinit(WICED_LED2); // Need to de-init the GPIO
    wiced_gpio_deinit(GPIO_HELP); // Need to de-init the GPIO
    wiced_gpio_deinit(GPIO_ON_OF); // Need to de-init the GPIO
    wiced_gpio_deinit(STAT_CHARGER); // Need to de-init the GPIO
    wiced_gpio_deinit(Sat_WiFi); // Need to de-init the GPIO
    wiced_gpio_deinit(WICED_PWM_1); // Need to de-init the GPIO if it is already set to drive the LED

//
//    wiced_gpio_deinit(WICED_PWM_1);
//    wiced_gpio_deinit(WICED_PWM_2);
//    wiced_gpio_deinit(WICED_PWM_3);
//    wiced_gpio_deinit(WICED_PWM_4);
//    wiced_gpio_deinit(WICED_PWM_5);
//
//    wiced_gpio_deinit(telemetry_1);
//    wiced_gpio_deinit(telemetry_2);
//    wiced_gpio_deinit(telemetry_3);
//    wiced_gpio_deinit(telemetry_4);


//    wiced_gpio_deinit(WICED_BUTTON1);

//  outputs


    //  Lectura si esta en el centro de despacho
    wiced_gpio_init( authorization, OUTPUT_OPEN_DRAIN_PULL_UP );
    //  Lectura de anticolision
//    wiced_gpio_init( anticolision_pulso, OUTPUT_OPEN_DRAIN_PULL_UP );
    //  Lectura de PWM 2
    wiced_gpio_init( PWM2, OUTPUT_OPEN_DRAIN_PULL_UP );
//      Lectura de PWM 3
    wiced_gpio_init( PWM3, OUTPUT_OPEN_DRAIN_PULL_UP );
    wiced_gpio_init( STAT_CHARGER, OUTPUT_PUSH_PULL );

    wiced_gpio_init( Sat_WiFi, OUTPUT_PUSH_PULL );

//    wiced_gpio_init( XRES_BLE, OUTPUT_OPEN_DRAIN_PULL_UP );


//  Inputs
    wiced_gpio_init( GPIO_ON_OF, INPUT_PULL_UP);
    wiced_gpio_init( GPIO_HELP, INPUT_PULL_UP );

//    wiced_gpio_init( telemetry_1, INPUT_PULL_UP);
//    wiced_gpio_init( telemetry_2, INPUT_PULL_UP);
//    wiced_gpio_init( telemetry_3, INPUT_PULL_UP);
//    wiced_gpio_init( telemetry_4, INPUT_PULL_UP);


//
//    wiced_gpio_output_high(PWM2);
//    wiced_gpio_output_high(PWM3);
//
//    wiced_gpio_output_high(XRES_BLE);

    wiced_gpio_output_low(STAT_CHARGER);
//    wiced_gpio_output_high(Sat_WiFi);
//    wiced_rtos_delay_milliseconds( BUZ_TIME);
    wiced_gpio_output_low(Sat_WiFi);

//
//    wiced_gpio_output_low(PWM2);
//     wiced_gpio_output_low(PWM3);

    return WICED_SUCCESS;
}

void buzz(uint8_t time,uint8_t prox){
//    int o;
//    if(prox==0){
//        for(o=0;o<=1;o++){
//            wiced_gpio_output_high(PWM3);
//            wiced_gpio_output_high(PWM2);
//            wiced_rtos_delay_milliseconds( time);
//            wiced_gpio_output_low(PWM3);
//            wiced_gpio_output_low(PWM2);
//            wiced_rtos_delay_milliseconds( time);
//        }
//    }
//    else if(prox==1){
//            wiced_gpio_output_high(PWM3);
//            wiced_gpio_output_high(PWM2);
//            wiced_rtos_delay_milliseconds( time);
//            wiced_gpio_output_low(PWM3);
//            wiced_gpio_output_low(PWM2);
//            wiced_rtos_delay_milliseconds( time);
//            wiced_gpio_output_high(PWM3);
//            wiced_gpio_output_high(PWM2);
//            wiced_rtos_delay_milliseconds( time);
//            wiced_gpio_output_low(PWM3);
//            wiced_gpio_output_low(PWM2);
//            wiced_rtos_delay_milliseconds( time);
//            wiced_gpio_output_high(PWM3);
//            wiced_gpio_output_high(PWM2);
//            wiced_rtos_delay_milliseconds( time);
//            wiced_gpio_output_low(PWM3);
//            wiced_gpio_output_low(PWM2);
//            wiced_rtos_delay_milliseconds( time);
////        }
//
//    }
//    else if(prox==2)
//        {
//            wiced_gpio_output_low(PWM3);        /* Primero lo callo*/
//            wiced_gpio_output_low(PWM2);
//            wiced_rtos_delay_milliseconds( 50);  /* Inicio sonoro */
//            wiced_gpio_output_high(PWM3);
//            wiced_gpio_output_high(PWM2);
//            wiced_rtos_delay_milliseconds( time+50);
//            wiced_gpio_output_low(PWM3);
//            wiced_gpio_output_low(PWM2);
//            wiced_rtos_delay_milliseconds( time);
//            wiced_gpio_output_high(PWM3);
//            wiced_gpio_output_high(PWM2);
//            wiced_rtos_delay_milliseconds( time-100);
//            wiced_gpio_output_low(PWM3);
//            wiced_gpio_output_low(PWM2);
//            wiced_rtos_delay_milliseconds(time-100);
//
//            wiced_gpio_output_high(PWM3);
//            wiced_gpio_output_high(PWM2);
//            wiced_rtos_delay_milliseconds( time-100);
//            wiced_gpio_output_low(PWM3);
//            wiced_gpio_output_low(PWM2);
//            wiced_rtos_delay_milliseconds(time-100);
//        }
//
////        wiced_rtos_delay_milliseconds( time);
//        WPRINT_APP_INFO( ("Sound buzz %d\r\n",time) );

}


void  blink_vehicle(){
//    wiced_gpio_output_low(Sat_WiFi );
//    wiced_rtos_delay_milliseconds( WIFI_BLINK);
    wiced_gpio_output_high(Sat_WiFi);
    wiced_rtos_delay_milliseconds(2* WIFI_BLINK);
    wiced_gpio_output_low(Sat_WiFi);
    wiced_rtos_delay_milliseconds( WIFI_BLINK);
    wiced_gpio_output_high(Sat_WiFi);
    wiced_rtos_delay_milliseconds(2* WIFI_BLINK);
    wiced_gpio_output_low(Sat_WiFi);
}

void btn_stop(){
    wiced_bool_t st_b;
    wiced_gpio_input_get_bool(GPIO_HELP,&st_b);

    if(st_b==WICED_FALSE){
        wiced_gpio_output_high(authorization);
        WPRINT_APP_INFO( ("Button user pressed 2\r\n") );
    }else{
        WPRINT_APP_INFO( ("nnnnnnnnnnButton user pressed 2\r\n") );
        wiced_gpio_output_low(authorization);
    }

}

void count_on_off(){
    wiced_bool_t on_off_b;
    wiced_gpio_input_get_bool(GPIO_ON_OF,&on_off_b);
//    WPRINT_APP_INFO(
    if(on_off_b==WICED_FALSE){
//        if(c_silent  ){
//
//        }
//        wiced_gpio_output_high(anticolision_pulso);
//        WPRINT_APP_INFO( ("STV\n") );
        wiced_uart_transmit_bytes(WICED_UART_1,BTN_ABOR,strlen(BTN_ABOR));


    }
    else{
//        WPRINT_APP_INFO( ("NNN Button on/off pressed 1\r\n") );
//        wiced_gpio_output_low(anticolision_pulso);

    }

}

void reset_server(){
    button_down_net=WICED_TRUE;
//    set_name();
//    wiced_framework_reboot();
    WPRINT_APP_INFO( ("Buton pressed gpio 3\r\n") );

    wiced_uart_transmit_bytes(WICED_UART_1,"Reset device", strlen("Reset device"));
}
void reset_sequence(){
    for (int v=0;v<=7;v++){
        wiced_gpio_output_high(Sat_WiFi);
        wiced_rtos_delay_milliseconds( 75);
        wiced_gpio_output_low(Sat_WiFi);
        wiced_rtos_delay_milliseconds( 75);

    }
    wiced_gpio_output_low(Sat_WiFi);
}


#endif  /* stdbool.h */
