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


#ifndef _LCD_INTERFACE_H
#define _LCD_INTERFACE_H

#include "stdbool.h"
#include "wiced.h"
#include "u8g_arm.h"

#include "GPIO/manager_gpio_vh.h"

#define online      "Connected"
#define offline     "Offline"
#define LCD_Success   "Succes_lcd"
#define LCD_Unsuccess "Unsuccess lcd"

#define fallen "FALLEN"
#define C_OLED (30)
wiced_bool_t flag_lcd_timer=WICED_FALSE;

#define MAXIMUM_ALLOWED_INTERVAL_BETWEEN_MONITOR_UPDATES (7000*MILLISECONDS)                                                       /* Watchdog */
#define EXPECTED_WORK_TIME                               (MAXIMUM_ALLOWED_INTERVAL_BETWEEN_MONITOR_UPDATES - (100*MILLISECONDS))    /* Watchdog */
#define UNEXPECTED_DELAY                                 (200*MILLISECONDS)

int coun_lcd=1;
int refresh_oled=1;
char LCD_state[14] ={0}; /* Watchdog */
char Error[2];

void Set_Warning(u8g_t* u8g,uint8_t count,unsigned char* buffer_in,char* c_l,char* c_v,wiced_bool_t flag);
void displayThread(wiced_thread_arg_t arg);
void screen_checker(void);            /* Watchdog */
extern void start_whatchdog_LCD(void);       /* Watchdog */

wiced_bool_t risk_t =WICED_FALSE;
wiced_bool_t evacution_t =WICED_FALSE;

uint8_t oled_things=1;

wiced_bool_t First_one=WICED_FALSE;



#define _BACK       "B"
#define _FRONT      "F"
#define _BOTH       "A"

/*************** OLED Display Thread ***************/

/* Initialize the OLED display */
wiced_i2c_device_t display_i2c = {
    .port          = WICED_I2C_2,
    .address       = 0x3C,
    .address_width = I2C_ADDRESS_WIDTH_7BIT,
    .flags         = 0,
    .speed_mode    = I2C_STANDARD_SPEED_MODE,
};

u8g_t display;

void int_lcd(){
    u8g_init_wiced_i2c_device(&display_i2c);
    u8g_InitComFn(&display, &u8g_dev_sh1106_128x64_2x_i2c, u8g_com_hw_i2c_fn);
    u8g_SetFont(&display, u8g_font_gdr10);
    u8g_SetFontPosTop(&display);

//    /* Registro de monitoreo*/
//    wiced_register_system_monitor( &my_thread_monitor, MAXIMUM_ALLOWED_INTERVAL_BETWEEN_MONITOR_UPDATES ); /* Monitoring the LCD screen */
//
//            if(wiced_i2c_probe_device(&display_i2c, 5) == WICED_TRUE)
//            {
//                //wiced_uart_transmit_bytes(WICED_UART_1,"LCD startup successful", strlen("LCD startup successful"));
//                memcpy(LCD_state,LCD_Success,strlen(LCD_Success));
//                printf("\n LCD init sucess \n");
//            }
//            else
//            {
//                //wiced_uart_transmit_bytes(WICED_UART_1,"No LCD successful", strlen("No LCD successful"));
//                memcpy(LCD_state,LCD_Unsuccess,strlen(LCD_Unsuccess));
//                printf("\n LCD init No sucess \n");
//            }
//            /* Actualizacion del monitoreo */
//            wiced_update_system_monitor( &my_thread_monitor, MAXIMUM_ALLOWED_INTERVAL_BETWEEN_MONITOR_UPDATES );
//            start_whatchdog_LCD();
}

void displayThread(wiced_thread_arg_t arg)
{
    char eva[25];
    uint8_t queue_str[2];
    char intro[25];
    char lamp[2];
    char veh[2];

    int_lcd();

    sprintf(intro,"SMART FLOW");
    sprintf(eva,"RISK ZONE");
    sprintf(Error,"xx");

    u8g_FirstPage(&display);
    wiced_rtos_lock_mutex(&i2cMutex);

    do {
        u8g_DrawStr(&display, 5, 1,  intro);

    } while (u8g_NextPage(&display));

    while(1)
    {
        /* Wait until new data is ready to display */
        wiced_rtos_get_semaphore(&displaySemaphore, WICED_WAIT_FOREVER);
        wiced_rtos_pop_from_queue(&pubQueue, &queue_str, WICED_WAIT_FOREVER);
//        WPRINT_APP_INFO(("informacion reviuda del queue =%s & %d \n",queue_str,count_l));
//        sprintf(queue_str,"%s",queue_str);

//        if((lcd_fallen==1)&&( Frsit_r==WICED_FALSE)){
//                Frsit_r=WICED_TRUE;
//        }

        if((fallen_f==WICED_TRUE)&&(First_one==WICED_TRUE)){
            fallen_f=WICED_FALSE;
            First_one=WICED_FALSE;
            oled_things=2;
        }
        else if ((risk_z==WICED_TRUE)&&(First_one==WICED_TRUE)){
            risk_z=WICED_FALSE;
            First_one=WICED_FALSE;
            oled_things=3;
        }
        else if((Evacaution==WICED_TRUE)&&(First_one==WICED_TRUE)){
            Evacaution=WICED_FALSE;
            First_one=WICED_FALSE;
            oled_things=4;
        }
        else if((_flag_aca==WICED_TRUE)){
            First_one=WICED_FALSE;
            _flag_aca=WICED_FALSE;
            oled_things=5;
        }
        else if((_B_transit==WICED_TRUE)&&(First_one==WICED_TRUE)){
             First_one=WICED_FALSE;
             _B_transit=WICED_FALSE;
             oled_things=6;
         }
        else if ((Evacaution==WICED_FALSE)&&(risk_z==WICED_FALSE)&&(fallen_f==WICED_FALSE)&&(First_one==WICED_TRUE)&&(_flag_aca==WICED_FALSE)) {
            First_one=WICED_FALSE;
            oled_things=1;
        }

        sprintf(lamp,"%d",count_l);
        sprintf(veh,"%d",count_v);

        /* Send data to the display */
        u8g_FirstPage(&display);
        wiced_rtos_lock_mutex(&i2cMutex);
        do {
            coun_lcd=coun_lcd+1;
            refresh_oled=refresh_oled+1;

//            Set_Warning(&display,coun_lcd,"  ",&lamp,&veh,WICED_FALSE);
            if(frist_seen_silent==WICED_TRUE){
               if((silent==WICED_TRUE)){
                   Set_Warning(&display,coun_lcd,SOUND_OFF,&lamp,&veh,3);
               }
               else{
                   Set_Warning(&display,coun_lcd,SOUND_ON,&lamp,&veh,3);
               }
               if (coun_lcd==(C_OLED*2)){
//                   int_lcd();
                   frist_seen_silent=WICED_FALSE;
                   coun_lcd=0;
                   oled_things=1;
               }

            }else{

                switch(oled_things){
                case 1:
                    Set_Warning(&display,coun_lcd," ",&lamp,&veh,WICED_FALSE);
                    break;
                case 2:
                    Set_Warning(&display,coun_lcd,FALLEN_MAN,&lamp,&veh,WICED_TRUE);
                    break;
                case 3:
                    Set_Warning(&display,coun_lcd,RISK_ZONE,&lamp,&veh,WICED_TRUE);
                    break;
                case 4:
                    Set_Warning(&display,coun_lcd,"Evacuati",&lamp,&veh,WICED_TRUE);
                    break;
                case 5:
                    Set_Warning(&display,coun_lcd,&log_accarreos.name,&lamp,&veh,WICED_TRUE);
                    break;
                case 6:
                    Set_Warning(&display,coun_lcd,TRANSIT,&lamp,&veh,WICED_TRUE);
                    break;
                }
            }




            if (coun_lcd==(C_OLED*2)){
                  coun_lcd=0;
//                  int_lcd();
                  oled_things=1;
                  First_one=WICED_TRUE;
               }

            if((refresh_oled==100)||(flag_lcd_timer==WICED_TRUE)){
                refresh_oled=0;
//                int_lcd();
                flag_lcd_timer=WICED_FALSE;

            }


        } while (u8g_NextPage(&display));


        wiced_rtos_unlock_mutex(&i2cMutex);
    }
}
#define a 2
void Set_Warning(u8g_t* u8g,uint8_t count,unsigned char* buffer_in,char* c_l,char* c_v,uint8_t flag){
    char eva[25];

    wiced_bool_t upnet=WICED_FALSE;

    sprintf(eva,"%s",buffer_in);


     upnet=wiced_network_is_up(WICED_STA_INTERFACE);
     if((upnet==WICED_TRUE)&&(wiced_network_is_ip_up(WICED_STA_INTERFACE)==WICED_TRUE)){
         u8g_SetFont(u8g, u8g_font_gdr10);
         u8g_SetFontPosTop(u8g);

         u8g_DrawLine(u8g, 57,0,71,0);
         u8g_DrawLine(u8g, 57,1,71,1);

         u8g_DrawLine(u8g, 52,4,57,0);
         u8g_DrawLine(u8g, 52,5,57,1);

         u8g_DrawLine(u8g, 76,3,71,0);
         u8g_DrawLine(u8g, 76,4,71,1);

         u8g_DrawLine(u8g, 61,4,67,4);
         u8g_DrawLine(u8g, 61,5,67,5);

         u8g_DrawLine(u8g, 56,7,61,4);
         u8g_DrawLine(u8g, 56,8,61,5);

         u8g_DrawLine(u8g, 72,7,67,4);
         u8g_DrawLine(u8g, 72,8,67,5);

         u8g_DrawBox(u8g, 63,9,3,3);
     }
//     else{
//         u8g_DrawLine(u8g, 52,0,76,10);
//         u8g_DrawLine(u8g, 52,10,76,0);
//
//         u8g_SetFont(u8g, u8g_font_gdr10);
//         u8g_SetFontPosTop(u8g);
//         u8g_DrawLine(u8g, 57,0,71,0);
//         u8g_DrawLine(u8g, 52,3,57,0);
//         u8g_DrawLine(u8g, 76,3,71,0);
//         u8g_DrawLine(u8g, 61,4,67,4);
//         u8g_DrawLine(u8g, 56,7,61,4);
//         u8g_DrawLine(u8g, 72,7,67,4);
//         u8g_DrawBox(u8g, 63,8,3,3);
//     }
     u8g_SetFont(u8g, u8g_font_gdr10);
      u8g_SetFontPosTop(u8g);


      if(strstr(_lateral_lam,_BACK)){
          u8g_DrawBox(u8g, 4,0,40,10);
      }
      else if(strstr(_lateral_lam,_FRONT)){
          u8g_DrawBox(u8g, 4,52,40,62);

      }
      else if(strstr(_lateral_lam,_BOTH)){
          u8g_DrawBox(u8g, 4,52,40,62);
          u8g_DrawBox(u8g, 4,0,40,10);
      }

      if(strstr(_lateral_veh,_BACK)){
          u8g_DrawBox(u8g, 88,0,128,10);
      }
      else if(strstr(_lateral_veh,_FRONT))
      {
          u8g_DrawBox(u8g, 88,52,128,62);
      }
      else if(strstr(_lateral_veh,_BOTH)){
          u8g_DrawBox(u8g, 88,52,128,62);
          u8g_DrawBox(u8g, 88,0,128,10);
      }

//     u8g_DrawBox(u8g, 4,52,50,62);
//     u8g_DrawBox(u8g, 80,52,100,62);

      if(flag==WICED_TRUE){
        if(count<=C_OLED){
          u8g_SetFont(u8g, u8g_font_gdb14);
          u8g_SetFontPosTop(u8g);
          u8g_DrawStr(u8g, 0, 19,  eva);
       }
       else if((count>C_OLED)&&(count<(C_OLED*2)+15)){
         u8g_SetFont(u8g, u8g_font_gdb30n);
         u8g_SetFontPosTop(u8g);
         if(atoi(c_l)<56)
         {
             u8g_DrawStr(u8g, 5, 19, c_l);
         }
         else if(atoi(c_l)>=56)
         {
             u8g_SetFont(u8g, u8g_font_gdb30);
             u8g_SetFontPosTop(u8g);
             u8g_DrawStr(&display, 5, 10, Error); /* 16 */
         }

          if(atoi(c_v)<10){
             u8g_DrawStr(u8g, 100, 19,c_v);
          }
          else if(atoi(c_v)<56){
             u8g_DrawStr(u8g, 80, 19,c_v);
          }
          else if(atoi(c_v)>=56){
              u8g_SetFont(u8g, u8g_font_gdb30);
              u8g_SetFontPosTop(u8g);
              u8g_DrawStr(&display, 77, 10, Error);
          }
       }
    }
    else if(flag==WICED_FALSE) {
        u8g_SetFont(u8g, u8g_font_gdb30n);
        u8g_SetFontPosTop(u8g);
        if(atoi(c_l) < 56)
        {
            u8g_DrawStr(u8g, 5, 19, c_l);
        }
        else if(atoi(c_l) >= 56)
        {
            u8g_SetFont(u8g,u8g_font_gdb30);
            u8g_SetFontPosTop(u8g);
            u8g_DrawStr(u8g, 5, 10, Error);
        }

         if(atoi(c_v)<10){
            u8g_DrawStr(u8g, 100, 19,c_v);
         }
         else if(atoi(c_v) < 56){  /* 58 */
            u8g_DrawStr(u8g, 80, 19,c_v);
         }
         else if(atoi(c_v)>=56){
             u8g_SetFont(u8g, u8g_font_gdb30);
             u8g_SetFontPosTop(u8g);
             u8g_DrawStr(&display, 77, 10, Error);
         }
    }
    else if(flag==3){
        if(count<=(C_OLED*2)){
          u8g_SetFont(u8g, u8g_font_gdb14);
          u8g_SetFontPosTop(u8g);
          u8g_DrawStr(u8g, 2, 19,  eva);
       }
    }


}


void draw_text(u8g_t* u8g,unsigned char *txt,const u8g_fntpgm_uint8_t  *font,uint8_t x,uint8_t y)
{
    char logo_txt[30];
    u8g_SetFont(u8g, font);
    u8g_SetFontPosTop(u8g);
    sprintf(logo_txt,"%s",txt);
    u8g_DrawStr(u8g, x, y, logo_txt);

}

void lcd_draw_count(u8g_t* u8g,uint8_t tc_v,uint8_t tc_l, const u8g_fntpgm_uint8_t  *font,int x, int y)
{

        u8g_SetFont(u8g, font);
        u8g_SetFontPosTop(u8g);
        char lamp[2];
        char veh[2];
        sprintf(lamp,"%d",tc_l);
        sprintf(veh,"%d",tc_v);

        u8g_DrawStr(u8g, x, y, lamp);
        u8g_DrawStr(u8g, x+70, y,veh);

}

void screen_checker()
{
    static uint8_t counter_lcd=0;
//    if(wiced_update_system_monitor( &my_thread_monitor, MAXIMUM_ALLOWED_INTERVAL_BETWEEN_MONITOR_UPDATES ) == WICED_SUCCESS)
//    {
//        printf("\n Receteo del watchdog \n");
//    }

//    if(wiced_i2c_probe_device(&display_i2c, 5) == WICED_TRUE)
//    {
//        wiced_update_system_monitor( &my_thread_monitor, MAXIMUM_ALLOWED_INTERVAL_BETWEEN_MONITOR_UPDATES );
//        printf("\n LCD continue sucesful \n");
//        //wiced_uart_transmit_bytes(WICED_UART_1,"LCD continue successful\n", strlen("LCD continue successful\n"));
//        sprintf(LCD_state,"%s",LCD_Success);
//        counter_lcd=0;
//    }
//    else if(counter_lcd <= 3)
//    {
//        if(wiced_update_system_monitor( &my_thread_monitor, MAXIMUM_ALLOWED_INTERVAL_BETWEEN_MONITOR_UPDATES )== WICED_SUCCESS)
//        {
//            printf("\n LCD continue No sucesful \n");
//        }
//        //wiced_uart_transmit_bytes(WICED_UART_1,"LCD no continue successful\n", strlen("LCD no continue successful\n"));
//        sprintf(LCD_state,"%s",LCD_Unsuccess);
//        counter_lcd++;   /* Se visualizara el mensaje de LCD no conetada por 2 ciclos, despues se reiniciara */
//    }
//    wiced_update_system_monitor( &my_thread_monitor, MAXIMUM_ALLOWED_INTERVAL_BETWEEN_MONITOR_UPDATES );
}

#endif  /* stdbool.h */
