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


#ifndef _DATA_CONTROL_H
#define _DATA_CONTROL_H

#include "wiced.h"
#include "stdbool.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

wiced_bool_t _B_transit=WICED_FALSE;
wiced_bool_t fallen_f =WICED_FALSE;
wiced_bool_t risk_z=WICED_FALSE;
wiced_bool_t _flag_aca=WICED_FALSE;
wiced_bool_t _flag_driver = WICED_FALSE; /* Variable used to indicate the sound of driver */
wiced_bool_t _wifi_status=WICED_FALSE;   /* Variable used to indicate the state of wifi */
wiced_bool_t _machine_flag=WICED_FALSE;
unsigned char _lateral_veh[2];
unsigned char _lateral_lam[2];
unsigned char _HVT_Text[150];
unsigned char _HE_Text[50];
unsigned char _HE_Hola[50];  /* No va */
unsigned char _HE_OTRO[50];  /* Otra que no va */

struct bt_data
{
//    unsigned char type [18];   // de momoento no se usa
    unsigned char mac_bt[18];
    unsigned char mac_wifi[18];
    unsigned char rssi[4];
    unsigned char fallen[2];
    uint8_t flag;
};

struct location_data
{
   struct bt_data bt_device;
   unsigned char time_end[12];
   unsigned char time_start[12];
   unsigned char date[12];
   unsigned char state[13];
   int priority;
   unsigned char id[4];
   uint8_t flag;

};

struct location_data_online
{
    unsigned char all_tex[150];
    uint8_t flag;
};

struct colliosn_mac_t
{
    unsigned char mac_lamp[254];
//    uint8_t quantity_mac_lamp;
    unsigned char mac_vehc[254];
    unsigned char mac_beacon[40];
//    unsigned char mac_tag[40];
    unsigned char id[3];

//    uint8_t quantity_mac_vehc;
};


struct telemetry_data
{
   uint8_t GPIO1;
   uint8_t GPIO2;
   uint8_t GPIO3;
   uint8_t GPIO4;
   unsigned char time_start[12];
   unsigned char date[12];
   unsigned char state[13];
   unsigned char mac_wifi[18];
};

struct tempo{
    unsigned char mac_bt[18];
    unsigned char time_end[12];
    unsigned char time_start[12];
    uint8_t flag;

};

struct tempo_collision{
    unsigned char mac_bt[18];
    unsigned char time_start[20];
    uint8_t flag;
};

#define ID_name_beacon "NBC#"
#define ID_acarreo  "CAR#"
#define _MSG_TEST   "CAR#11:00:33:44:55:66|4\0"
#define _MSG_TEST2  "BNM|BC:57:29:00:2E:DB,GEOSF,-95,0"
#define _N_KDV      "KDV"
#define _NONE       "NONE"

static char _split_tama_1[] ="#";
static char _split_tama_2 []="|";

typedef enum {
    BC0,
    BC1,
    BC2,
    BC3,
    BC4
} _BEACON;

struct Acarreos
{
    char    mac_bt[19];
    int     type;
    unsigned char time_start[12];
    unsigned char date[12];
    unsigned char name[18];
    unsigned char id[4];

};

//char mac_bt_D[19]; /* Variable used in the driver identificator */
void tamagochi(char *input,struct Acarreos *acareo){
    int x=0;
    unsigned char str_split[128];
    memset(str_split,'\0',128);

    if(strstr(input,ID_acarreo)&&(strlen(acareo->mac_bt)==0)){  /* CAR#9C:20:42:A1:33:44|1 */
        strcpy(acareo->time_start,time_get(&i2c_rtc));          /* NBC#Carga */
        strcpy(acareo->date,date_get_log(&i2c_rtc));

        memcpy(str_split,input,strlen(input));

        char * frist_split;

        frist_split=strtok(str_split,_split_tama_1);
        frist_split=strtok(NULL,_split_tama_1);

//         printf("<<%s\n",frist_split);

        char *second_split;

        second_split=strtok(frist_split,_split_tama_2);
//         printf("<<%s\n",second_split);

        while(second_split!=NULL){
            switch (x)
            {
            case 0:
                /* code */
                         printf("mmmm%s\n",second_split);

                memcpy(acareo->mac_bt,second_split,strlen(second_split));
                acareo->mac_bt[strlen(second_split)]='\0';
                break;
            case 1:
                /* code */
                acareo->type=atoi(second_split);
//                switch(acareo->type){
//                case 1:
//
//                    break;
//                }
                break;
            case 2:
                /* code */
//                _flag_aca=WICED_TRUE;
//                acareo->type=atoi(second_split);
//                memcpy(acareo->name,second_split,strlen(second_split));
//                acareo->name[strlen(second_split)]='\0';
                break;
            default:
                break;
            }
            x++;
            second_split=strtok(NULL,_split_tama_2);
        }
        memcpy(_HE_OTRO,"HVT;ENTRO1",strlen("HVT;ENTRO1"));

    }
    else if(strstr(input,ID_name_beacon)){

        memcpy(str_split,input,strlen(input)); /* NBC#Carga */

        char * frist_split;

        frist_split=strtok(str_split,_split_tama_1);
        frist_split=strtok(NULL,_split_tama_1);

         printf("<<%s\n",frist_split);

        char *second_split;

        second_split=strtok(frist_split,_split_tama_2);
         printf("<<%s\n",second_split);

        while(second_split!=NULL){
            switch (x)
            {
            case 0:
                /* code */
                _flag_aca=WICED_TRUE;
                memcpy(acareo->name,second_split,strlen(second_split));
                acareo->name[strlen(second_split)]='\0';
                printf("%s",acareo->name);
                break;
            default:
                break;
            }
            x++;
            second_split=strtok(NULL,_split_tama_2);
        }
        memcpy(_HE_OTRO,"HVT;ENTRO2",strlen("HVT;ENTRO2"));
    }
//    else if(strstr(input,_N_KDV))
//        {
//            memcpy(str_split,input,strlen(input));
//            //printf("\n Copio %s\n",str_split);
//
//            char * first_split;
//
//            first_split=strtok(str_split,_split_tama_2);
//
//            while(first_split !=NULL)
//            {
//                switch(x)
//                {
//                case 0:
//                    /* No hago nada */
//                    break;
//                case 1:
//                    if(strstr(input ,_NONE))
//                    {
//                        memset(mac_bt_D,'\0',strlen(mac_bt_D));
//                    }
//                    else
//                    {
//                        memset(mac_bt_D,'\0',strlen(mac_bt_D));
//                        //memcpy(mac_bt_D, first_split,strlen(first_split));
//                        memcpy(mac_bt_D, first_split,strlen(first_split));
//                        mac_bt_D[strlen(first_split)]='\0';
//                        printf("\n %s \n",mac_bt_D);
//                    }
//
//                    break;
//                    default:
//                        break;
//                }
//                first_split = strtok(NULL,_split_tama_2);
//                x++;
//            }
//
//                _flag_driver = WICED_TRUE;
//          }
    wiced_rtos_set_semaphore(&displaySemaphore);

}

void split_reader(unsigned char* buffer_in){
    unsigned char str_split2[128];

    strcpy(str_split2,&buffer_in[3]);
//    printf("Reader %s \n",str_split2);
    int ct=0;
    unsigned char temp[8];
    memset(temp,NULL,8);

    char *cvl2 = strtok(str_split2, ",");
          while(cvl2 != NULL)
          {
//              printf("dos: %s\n",cvl2);

              strncpy(temp,cvl2,strlen(cvl2));
              cvl2=strtok(NULL, ",");

//              printf("tres %s\n",temp);
              char * token1 = strtok(temp, "-");

//              printf("Valor2> %s\n",token1);

              token1 = strtok(NULL, "-");
//              printf("Valor3 >%s\n",token1);

              if(ct==0){
                   strncpy(_lateral_veh,token1,2);
//                   printf("Valor4 >%s\n",_lateral_veh);
                   }
              if(ct==1){
                   strncpy(_lateral_lam,token1,2);
//                   printf("Valor5 >%s\n",_lateral_lam);
               }
              ct++;
          }
}


void lcd_data_update(unsigned char* buffer_in,uint8_t *c_v,uint8_t *c_l,wiced_bool_t *proximity){

    unsigned char str_switch[3];
    char *cvl;
    int x=0;
    char delim[] = ",";     //establece como  realizara el split
    unsigned char str_split[128];

    int count_lcd_q[4];

//    WPRINT_APP_INFO( (">>VBT \r\n") );
//    WPRINT_APP_INFO( ("%s \r\n",buffer_in) );

    strncpy(str_switch,buffer_in,3);
    strcpy(str_split,&buffer_in[3]);

    if(strstr(str_switch,"CVL")){
        char *cvl1 = strtok(str_split, delim);
        while(cvl1 != NULL)
        {
            printf("Split: %s\n",cvl1);
            count_lcd_q[x]= atoi(cvl1);
            cvl1=strtok(NULL, delim);
           x++;

           if(x==3){
               printf(">>%s\n",cvl1);
               if(strstr(cvl1,"ON"))
                    printf("Encendido\n");
               else
                   printf("Apagado\n");
           }
        }

        split_reader(buffer_in);

        x=0;
        *c_v=count_lcd_q[0];
        *c_l=count_lcd_q[1];
        *proximity=count_lcd_q[2];

//        WPRINT_APP_INFO( ("strtok -> %d and %d \r\n",(*c_v),(*c_l)) );
        wiced_rtos_set_semaphore(&displaySemaphore);


    }

}


void lcd_fallen_update(unsigned char* buffer_in,uint8_t *fallen){
    unsigned char str_temp[17];
    unsigned char str_switch[3];
    char *cvl;
    int x=0;
    char delim[] = ",";     //establece como  realizara el split
    unsigned char str_split[128];

    strncpy(str_switch,buffer_in,4);
    strcpy(str_split,&buffer_in[4]);

    int fal;

    int t=0;
    char fall_at[2];

    if(strstr(str_switch,"BNM|")){

            unsigned char *cvf = strtok(str_split, delim);
            while(cvf != NULL){
                switch (t) {
                    case 0:
                    break;
                    case 1:
                        strcpy(str_temp,cvf);
                        if((strstr(str_temp,"BEAC"))){
                           risk_z=WICED_TRUE;
                           //_B_transit=WICED_TRUE; /* Esto estaba descomentado */
                           wiced_rtos_set_semaphore(&displaySemaphore);
                        }
                    break;
                    case 2:
                    break;
                    case 3:
                        strcpy(fall_at,cvf);
                        fal=atoi(fall_at);
                        *fallen=fal;
                        if(fal==1){
                            fallen_f=WICED_TRUE;
                            wiced_rtos_set_semaphore(&displaySemaphore);

                        }
//                        WPRINT_APP_INFO(("\r\n\r\n\r\nProcess: %d \r\n",*fallen));
                        break;
                    default:
                        break;
                }
                t++;
                cvf=strtok(NULL, delim);
            }
            t=0;

        }




}


char* data_to_json_telemetry(struct telemetry_data *data  ){
    char* res;
    res=malloc(sizeof(char)*150);
           sprintf(res,
           "{\"MacWiFi\":\"%s\",\"Time\":\"%s\",\"Date\":\"%s\",\"GPIO1\":\"%d\",\"GPIO2\":\"%d\",\"GPIO3\":\"%d\",\"GPIO4\":\"%d\"}\n"
           ,data->mac_wifi,data->time_start,data->date,data->GPIO1,data->GPIO2,data->GPIO3,data->GPIO4);
    return res;
}


char* data_to_json(struct location_data *data  ){
    char* res;
    res=malloc(sizeof(char)*150);

//    {"MacBt":"11:22:55:11:11:45","MacWiFi":"11:22:55:11:11:45","TimeStart":"10;02;01","TimeEnd":"10;08;05","Date":"27_02_22","Type":"BEAC"}\n
//    HV:{"LogId":1,"DeviceId":"00:00:00:00:00:01","Reader":"00:00:00:00:00:02","EnterTime":"20/09/2023-16:11:23", "OutTime":"20/09/2023-17:11:23"}
    sprintf(res,
    "{\"LogId\":%s,\"DeviceId\":\"%s\",\"Reader\":\"%s\",\"EnterTime\":\"%s-%s\",\"OutTime\":\"%s-%s\"}\n"
    ,data->id,data->bt_device.mac_wifi,data->bt_device.mac_bt,data->date,data->time_start,data->date,data->time_end);

    return res;
}


char* data_to_json_collision(struct colliosn_mac_t *data  ,struct tempo_collision * main,char * date,char * Vehi_Rep){
    char* res;
    res=malloc(sizeof(char)*350);
    uint8_t key=0;

    unsigned char reporting[75];
    unsigned char alert[75];
    unsigned char beacon[75];
    unsigned char a_mac_lamp[100];
//    unsigned char a_mac_veh[100];

    memset(reporting,NULL,75);
    memset(alert,NULL,75);
    memset(beacon,NULL,75);
    memset(a_mac_lamp,NULL,100);
//    memset(a_mac_veh,NULL,75);

    sprintf(reporting,"{\"DeviceId\":\"%s\",\"EventType\":2,\"StatusType\":1}",Vehi_Rep);

    sprintf(alert,",{\"DeviceId\":\"%s\",\"EventType\":1,\"StatusType\":3}",main->mac_bt);

    if((strlen(data->mac_beacon)!=0)){
        sprintf(beacon,",{\"DeviceId\":\"%s\",\"EventType\":4,\"StatusType\":4}",data->mac_beacon);
    }
    if((strlen(data->mac_lamp)!=0)){
        sprintf(a_mac_lamp,",{\"DeviceId\":\"%s\",\"EventType\":3,\"StatusType\":2}",data->mac_lamp);
    }
//    if((strlen(data->mac_vehc)!=0)){
//        sprintf(a_mac_veh,",{\"DeviceId\":\"%s\",\"EventType\":2,\"StatusType\":2}",data->mac_vehc);
//    }


    sprintf(res,
              "{\"EventDateFormatted\":\"%s-%s\",\"LogId\":%s,\"ProximityEventDevices\":[%s %s %s %s ]}\n"
              ,date,main->time_start,data->id,reporting,alert,beacon,a_mac_lamp);


    return res;
}


char* data_to_json_acarreo(struct Acarreos * main,char * Vehi_Rep){
    char* res;
    res=malloc(sizeof(char)*200);

    sprintf(res,
              "{\"EventId\":\"%s\",\"EventDate\":\"%s-%s\",\"MACBeacon\":\"%s\",\"MACOperator\":\"\",\"MACVehicle\":\"%s\",\"Status\":%d}\n"
              ,main->id,main->date,main->time_start,main->mac_bt,Vehi_Rep,main->type);

    return res;
}



int id_revived(unsigned char* buffer_in){
    int id;
    unsigned char str_switch[4];
        unsigned char str_split[128];
        unsigned char pem_mac[17];
        unsigned char str_temp[17];


        strncpy(str_switch,buffer_in,4);
        memcpy(str_split,buffer_in,strlen(buffer_in)+4);

        char delim[] = ":";     //establece como  realizara el split
        int x=0;
        if(strstr(str_switch,"HVT:")){

            unsigned char *cvl_1 = strtok(str_split, ":");
            cvl_1=strtok(NULL, ":");


//            printf("%s\n",cvl_1);
            unsigned char *cvl1 = strtok(cvl_1, delim);
            while(cvl1 != NULL){
            //                WPRINT_APP_INFO( ("strtok -> %s \r\n",cvl1) );
            //                strcpy(pem_mac,cvl1);
            //                WPRINT_APP_INFO( ("memecpy -> %s \r\n",pem_mac) );
                switch (x) {
                    case 0:
//                        strncpy(data_btt[s_count_x+1].mac_bt,cvl1,17);

                        printf("0 %s\n",cvl1);
                        id=atoi(cvl1);
                    break;
                    case 1:
//                        strncpy(data_btt[s_count_x+1].type,cvl1,17);
                        printf("1 %s\n",cvl1);

                        break;

                    default:
                        break;
                }
                x++;
                cvl1=strtok(NULL, delim);
            }

        }


        return id;
}




#endif  /* stdbool.h */
