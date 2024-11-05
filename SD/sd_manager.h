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

#ifndef _SD_MANAGER_H
#define _SD_MANAGER_H

#define PATH_BUFFER_SIZE        FX_MAXIMUM_PATH
#define DIRECTORY_SEPARATOR_STR "/"
#define ROOT_DIRECTORY          "/tgt/"
#define DIRECTORY_SEPARATOR_CHR '/'
/* reduce FILE_BUFFER_SIZE or increse APPLICATION_STACK_SIZE in mk file,
   if you meet stack overflow */
#define FILE_BUFFER_SIZE        (65536)*2*2
#define SRC_DIR                 "/src"
//#define TGT_DIR                 "/tgt"
#define PROGRESS_UNIT           (50)
#define LOCAL_BUFFER_SIZE       (64*1024*2)

#define FILE_EXISTS             1
#define NO_FILE                 0
#define SF_ROOT                 "/SF"
#define TELE_ROOT               "/Telemetria"
#define ANTICOLISION_ROOT       "/Anticolision"
#define ACARREO_ROOT            "/Acarreo"


#define TELE                    "Telemetria"
#define SF                      "SF"
#define ANTICOLISION            "Anticolision"
#define ACARREO                 "Acarreo"


char id_count[4];
char id_count_collision[2];

#include "wiced.h"
#include "wiced_filesystem.h"
//#include "sdiovar.h"
//#include "wiced_osl.h"

#include <string.h>
#include <stdio.h>

//#include "UART/main_uart.h"

#include "DATA/Data_control.h"

//#define MEM
#ifdef MEM
    pirntf("algo paso ?\n\n\n");
#else
    DEFINE_CACHE_LINE_ALIGNED_ARRAY(volatile char, filebuf, LOCAL_BUFFER_SIZE);
//    memset(filebuf,NULL,LOCAL_BUFFER_SIZE);
#endif

extern wiced_block_device_t block_device_sdmmc;

int list_files( wiced_filesystem_t* fs_handle, const char* dir_name );

int file_verify( char* TGT_DIR,wiced_filesystem_t* fs_handle, const char* dir_name, char* date );

void create_file(char* TGT_DIR,const char* filename,wiced_filesystem_t* fs_handle);

void init_sd(wiced_filesystem_t* fs_handle);

int read_data(char* TGT_DIR,const char* filename,wiced_filesystem_t* fs_handle);

int read_data_collision(char* TGT_DIR,const char* filename,wiced_filesystem_t* fs_handle);

void write_data(char* TGT_DIR,const char* filename,struct location_data dataX,wiced_filesystem_t* fs_handle);

void write_data_collision(char* TGT_DIR,const char* filename,wiced_filesystem_t* fs_handle,struct colliosn_mac_t *data  ,struct tempo_collision * main,char * date,char * Vehi_Rep);

int dir_verify( char* dir_to_file_create,wiced_filesystem_t* fs_handle, const char* dir_name, char* date );



void init_sd(wiced_filesystem_t* fs_handle){
    wiced_result_t result;
          wiced_dir_t dir;


//          if ( !PLATFORM_FEATURE_ENAB(SDIO) )
//          {
//          printf( "\nSD card NOT supported\n" );
//          }
//while (result!=WICED_SUCCESS){

          result = wiced_filesystem_init();
          if ( result != WICED_SUCCESS  )
          {
              printf( "Error initing filesystems\n");
          }

          /* TEST FileX */
      //    printf( "\n\nTesting SD/MMC on FileX\n\n");

          result = wiced_filesystem_mount( &block_device_sdmmc, WICED_FILESYSTEM_HANDLE_FILEX, fs_handle, "TestWicedFS" );

          if ( result != WICED_SUCCESS )
          {
              printf( "Error mounting filesystem\n" );
          }
//}

}

int read_data(char* TGT_DIR,const char* filename,wiced_filesystem_t* fs_handle){

    int count_file=0;
    char res;
    uint64_t read_count = 1;
    char g_date[15];

    sprintf(g_date,"%s.txt",filename);

    wiced_dir_t dir;
    wiced_file_t f_src;

//    char* filebuf;
//    filebuf=malloc(sizeof(char)*LOCAL_BUFFER_SIZE);
//    DEFINE_CACHE_LINE_ALIGNED_ARRAY(char, filebuf, LOCAL_BUFFER_SIZE);
    memset(filebuf,NULL,LOCAL_BUFFER_SIZE);
    wiced_filesystem_dir_open( fs_handle, &dir, TGT_DIR );
    wiced_filesystem_file_open(fs_handle, &f_src, g_date, WICED_FILESYSTEM_OPEN_FOR_READ );
//    while ( read_count )
//    {
    wiced_filesystem_file_read( &f_src, filebuf, ( uint64_t ) LOCAL_BUFFER_SIZE, &read_count );
  //  Agregar un split es mejor para separar los archivos

      for(long int j=0;j<=LOCAL_BUFFER_SIZE;j++){
          if((filebuf[j]=='\n')){
              count_file=count_file+1;

          }
          else{

          }

      }
//      const char s[2] = "\n";
//         char *token;
//
//         /* get the first token */
//         token = strtok(filebuf, s);
//
//         /* walk through other tokens */
//         while( token != NULL ) {
////            printf( " >>>>>  %s\n", token );
//
//            token = strtok(NULL, s);
//         }
      sprintf(id_count,"%d",count_file+1);
      printf("\t lecutra del documento:\n numero de carcacteres %d, numero de filas: %d \n\n",strlen(filebuf),count_file);
//      printf("%s",filebuf);
      //    }
//      memset(filebuf,NULL,LOCAL_BUFFER_SIZE);
//      printf("ya no hay %s",filebuf);

      wiced_filesystem_file_close( &f_src );
      wiced_filesystem_dir_close(&dir);
//      free(filebuf);
//      wiced_filesystem_unmount(fs_handle);
//      res=count_file;
      printf("return %s \n",id_count);
      return  count_file;
}

int read_data_collision(char* TGT_DIR,const char* filename,wiced_filesystem_t* fs_handle){

    int count_file=0;
    char res;
    uint64_t read_count = 1;
    char g_date[15];

    sprintf(g_date,"%s.txt",filename);

    wiced_dir_t dir;
    wiced_file_t f_src;

//    char* filebuf;
//    filebuf=malloc(sizeof(char)*LOCAL_BUFFER_SIZE);
//    DEFINE_CACHE_LINE_ALIGNED_ARRAY(char, filebuf, LOCAL_BUFFER_SIZE);
    memset(filebuf,NULL,LOCAL_BUFFER_SIZE);
    wiced_filesystem_dir_open( fs_handle, &dir, TGT_DIR );
    wiced_filesystem_file_open(fs_handle, &f_src, g_date, WICED_FILESYSTEM_OPEN_FOR_READ );
//    while ( read_count )
//    {
    wiced_filesystem_file_read( &f_src, filebuf, ( uint64_t ) LOCAL_BUFFER_SIZE, &read_count );
  //  Agregar un split es mejor para separar los archivos

      for(long int j=0;j<=LOCAL_BUFFER_SIZE;j++){
          if((filebuf[j]=='\n')){
              count_file=count_file+1;

          }
          else{

          }

      }
//      const char s[2] = "\n";
//         char *token;
//
//         /* get the first token */
//         token = strtok(filebuf, s);
//
//         /* walk through other tokens */
//         while( token != NULL ) {
////            printf( " >>>>>  %s\n", token );
//
//            token = strtok(NULL, s);
//         }
      sprintf(id_count_collision,"%d",count_file+1);
      printf("\t lecutra del documento:\n numero de carcacteres %d, numero de filas: %d \n\n",strlen(filebuf),count_file);
//      printf("%s",filebuf);
      //    }
//      memset(filebuf,NULL,LOCAL_BUFFER_SIZE);
//      printf("ya no hay %s",filebuf);

      wiced_filesystem_file_close( &f_src );
      wiced_filesystem_dir_close(&dir);
//      free(filebuf);
//      wiced_filesystem_unmount(fs_handle);
//      res=count_file;
      printf("return %s \n",id_count_collision);
//      return  res;
}

void write_data(char* TGT_DIR,const char* filename,struct location_data dataX,wiced_filesystem_t* fs_handle){
    wiced_result_t result;
    wiced_dir_t dir;
    uint64_t write_count = 1;
    wiced_file_t f_src;
//    txt=malloc(sizeof(char)*200);
    char g_date[15];

    sprintf(g_date,"%s.txt",filename);
//    data_to_json(&data)
//    strcpy(dataX->id,"off");

    wiced_filesystem_dir_open( fs_handle, &dir, TGT_DIR );
    wiced_filesystem_file_open( fs_handle, &f_src, g_date, WICED_FILESYSTEM_OPEN_APPEND );
//    uint64_t t_c=(uint64_t)sizeof(txt);
    printf("\n %s numero de byts a escribir %d \n",data_to_json(&dataX),strlen(data_to_json(&dataX)));
    wiced_filesystem_file_write( &f_src, (const char *)data_to_json(&dataX),strlen(data_to_json(&dataX)), &write_count );


    wiced_filesystem_file_close( &f_src );
    wiced_filesystem_dir_close(&dir);


}


void write_data_acarreo(char* TGT_DIR,const char* filename,struct Acarreos *dataX,char * Vehi_Rep,wiced_filesystem_t* fs_handle){
    wiced_result_t result;
    wiced_dir_t dir;
    uint64_t write_count = 1;
    wiced_file_t f_src;
//    txt=malloc(sizeof(char)*200);
    char g_date[15];

    sprintf(g_date,"%s.txt",filename);
//    data_to_json(&data)
//    strcpy(dataX->id,"off");

    wiced_filesystem_dir_open( fs_handle, &dir, TGT_DIR );
    wiced_filesystem_file_open( fs_handle, &f_src, g_date, WICED_FILESYSTEM_OPEN_APPEND );
//    uint64_t t_c=(uint64_t)sizeof(txt);
    printf("\n %s numero de byts a escribir %d \n",data_to_json_acarreo(dataX,Vehi_Rep),strlen(data_to_json_acarreo(dataX,Vehi_Rep)));
    wiced_filesystem_file_write( &f_src, (const char *)data_to_json_acarreo(dataX,Vehi_Rep),strlen(data_to_json_acarreo(dataX,Vehi_Rep)), &write_count );


    wiced_filesystem_file_close( &f_src );
    wiced_filesystem_dir_close(&dir);


}

void write_data_collision(char* TGT_DIR,const char* filename,wiced_filesystem_t* fs_handle,struct colliosn_mac_t *data  ,struct tempo_collision * main,char * date,char * Vehi_Rep){
    wiced_result_t result;
    wiced_dir_t dir;
    uint64_t write_count = 1;
    wiced_file_t f_src;
    char g_date[15];
    char save_log[250];

//    sprintf(save_log,"%s|%s-%s|%s|%s|%s|%s|%s\n",data->id,date,main->time_start,Vehi_Rep,main->mac_bt,data->mac_beacon,data->mac_lamp,data->mac_vehc);
//    printf("%s\n",save_log);

//    char *tk;
//
//    tk = strtok(save_log, "|");
//    while( tk != NULL ) {
//
//        printf("<>%s\n",tk);
//        tk = strtok(NULL, "|");
//        }


    sprintf(g_date,"%s.txt",filename);

    wiced_filesystem_dir_open( fs_handle, &dir, TGT_DIR );
    wiced_filesystem_file_open( fs_handle, &f_src, g_date, WICED_FILESYSTEM_OPEN_APPEND );

    printf("\n %s numero de byts a escribir %d \n",data_to_json_collision(data,main,date,Vehi_Rep),strlen(data_to_json_collision(data,main,date,Vehi_Rep)));
    wiced_filesystem_file_write( &f_src, (const char *)data_to_json_collision(data,main,date,Vehi_Rep),strlen(data_to_json_collision(data,main,date,Vehi_Rep)), &write_count );


    wiced_filesystem_file_close( &f_src );
    wiced_filesystem_dir_close(&dir);


}


void create_file(char* TGT_DIR,const char* filename,wiced_filesystem_t* fs_handle){
    wiced_result_t result;
    wiced_dir_t dir;
    uint64_t write_count = 1;
    wiced_file_t f_src;


    wiced_filesystem_dir_open( fs_handle, &dir, TGT_DIR );
    wiced_filesystem_file_open( fs_handle, &f_src, filename, WICED_FILESYSTEM_OPEN_WRITE_CREATE );
    wiced_filesystem_file_close( &f_src );


}

int file_verify( char* dir_to_file_create,wiced_filesystem_t* fs_handle, const char* dir_name, char* date )
{
    wiced_bool_t flag_create=WICED_FALSE;
    char * split;
    wiced_dir_t   dir;
    int len;
    char namebuf[PATH_BUFFER_SIZE];
    char pathbuf[PATH_BUFFER_SIZE];
    wiced_dir_entry_type_t    entry_type;
    wiced_dir_entry_details_t details;
    char g_date[15];

    sprintf(g_date,"%s.txt",date);

    printf( "list_files: %s\n", dir_name );

    strcpy(namebuf, dir_name);
    len = strlen(namebuf);
    if ( len > 1 && namebuf[ len - 1 ] == DIRECTORY_SEPARATOR_CHR )
    {
        namebuf[ len - 1 ] = '\0';
    }

    /* Open the Wiced directory */
    if ( WICED_SUCCESS != wiced_filesystem_dir_open( fs_handle, &dir, namebuf ) )
    {
        printf( "%d: Error opening dir %s\n", __LINE__, namebuf );
        return -1;
    }

    while ( 1 != wiced_filesystem_dir_end_reached( &dir ) )
    {
        if ( WICED_SUCCESS == wiced_filesystem_dir_read( &dir, namebuf, sizeof(namebuf), &entry_type, &details ) )
        {
            // printf( "%d: %s%s\n", entry_type, dir_name, namebuf );
//            printf( ">>\t %s\n", namebuf );
//            if ( entry_type == WICED_FILESYSTEM_DIR && 0 != strcmp( ".", namebuf ) && 0 != strcmp( "..", namebuf ) )
//            {
//                sprintf( pathbuf, "%s%s%s", dir_name, namebuf, DIRECTORY_SEPARATOR_STR );
//                list_files( fs_handle, pathbuf );
//            }
//            split=strtok(namebuf,".");
//            printf(">%s<\n",split);
            if(strcmp(namebuf,g_date)==0){
                printf("existe el documento\n");
                flag_create=WICED_FALSE;
            }
            else{
                flag_create=WICED_TRUE;
            }
        }
        else
        {
            break;
        }
    }

    if(flag_create==WICED_TRUE){
        create_file(dir_to_file_create,g_date,fs_handle);
        //return NO_FILE;

    }

    wiced_filesystem_dir_close( &dir );

    return 0;
}

int dir_verify( char* dir_to_file_create,wiced_filesystem_t* fs_handle, const char* dir_name, char* date )
{
    wiced_bool_t flag_create=WICED_FALSE;
    wiced_dir_t   dir;
    int len;
    char namebuf[PATH_BUFFER_SIZE];
    wiced_dir_entry_type_t    entry_type;
    wiced_dir_entry_details_t details;

    printf( "list_files: %s\n", dir_name );

    strcpy(namebuf, dir_name);
    len = strlen(namebuf);
    if ( len > 1 && namebuf[ len - 1 ] == DIRECTORY_SEPARATOR_CHR )
    {
        namebuf[ len - 1 ] = '\0';
    }

    /* Open the Wiced directory */
    if ( WICED_SUCCESS != wiced_filesystem_dir_open( fs_handle, &dir, namebuf ) )
    {
        printf( "%d: Error opening dir %s\n", __LINE__, namebuf );
        return -1;
    }

    while ( 1 != wiced_filesystem_dir_end_reached( &dir ) )
    {
        if ( WICED_SUCCESS == wiced_filesystem_dir_read( &dir, namebuf, sizeof(namebuf), &entry_type, &details ) )
        {

            if(strcmp(namebuf,date)==0){
                printf("existe el documento\n");
                flag_create=WICED_FALSE;
            }
            else{
                flag_create=WICED_TRUE;
//                printf("no existe el documento\n");

            }
        }
        else
        {
            break;
        }
    }

    if(flag_create==WICED_TRUE){
        wiced_filesystem_dir_create(fs_handle,date);

//        create_file(dir_to_file_create,g_date,fs_handle);
        //return NO_FILE;

    }

    wiced_filesystem_dir_close( &dir );

    return 0;
}

int list_files( wiced_filesystem_t* fs_handle, const char* dir_name )
{
    wiced_dir_t   dir;
    int len;
    char namebuf[PATH_BUFFER_SIZE];
    char pathbuf[PATH_BUFFER_SIZE];
    wiced_dir_entry_type_t    entry_type;
    wiced_dir_entry_details_t details;

    printf( "list_files: %s\n", dir_name );

    strcpy(namebuf, dir_name);
    len = strlen(namebuf);
    if ( len > 1 && namebuf[ len - 1 ] == DIRECTORY_SEPARATOR_CHR )
    {
        namebuf[ len - 1 ] = '\0';
    }

    /* Open the Wiced directory */
    if ( WICED_SUCCESS != wiced_filesystem_dir_open( fs_handle, &dir, namebuf ) )
    {
        printf( "%d: Error opening dir %s\n", __LINE__, namebuf );
        return -1;
    }

    while ( 1 != wiced_filesystem_dir_end_reached( &dir ) )
    {
        if ( WICED_SUCCESS == wiced_filesystem_dir_read( &dir, namebuf, sizeof(namebuf), &entry_type, &details ) )
        {
            // printf( "%d: %s%s\n", entry_type, dir_name, namebuf );
            printf( ">>\t %s\n", namebuf );
//            if ( entry_type == WICED_FILESYSTEM_DIR && 0 != strcmp( ".", namebuf ) && 0 != strcmp( "..", namebuf ) )
//            {
//                sprintf( pathbuf, "%s%s%s", dir_name, namebuf, DIRECTORY_SEPARATOR_STR );
//                list_files( fs_handle, pathbuf );
//            }
        }
        else
        {
            break;
        }
    }

    wiced_filesystem_dir_close( &dir );

    return 0;
}


#endif
