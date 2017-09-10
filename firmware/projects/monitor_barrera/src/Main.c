/* Copyright 2017, Eric Pernia
 * All rights reserved.
 *
 * This file is part of sAPI Library.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*==================[inclusiones]============================================*/

#include "sapi.h"
#include "board.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "portable.h"
#include "semphr.h"
#include "queue.h"

#include "ctype.h"

#include "Uart.h"
#include "Print.h"
#include "PcManager.h"


/*==================[definiciones y macros]==================================*/

typedef enum{noTrain, trainPassLeftDir, trainPassRightDir } barrierControlState_t;
#define PC_MSG_FORMAT_CHANGE_BARRIER_STATE "estado barrera: %s\r\n"
#define PC_MSG_FORMAT_CHANGE_TRAIN_STATE "estado tren: %s\r\n"
#define PC_MSG_BARRIER_STATE_NO_TRAIN "sin tren"
#define PC_MSG_BARRIER_STATE_LEFT_TRAIN "tren desde la izquierda"
#define PC_MSG_BARRIER_STATE_RIGHT_TRAIN "tren desde la derecha"
#define PC_MSG_TRAIN_STATE_ACTIVED "activado"
#define PC_MSG_TRAIN_STATE_DESACTIVED "desactivado"

/*==================[definiciones de datos internos]=========================*/

barrierControlState_t barrierControlState = noTrain;
modelMode_t modelMode = normal;
bool_t train_activated = false;
uint8_t barrierControlLastSensorState = 0;

/*==================[definiciones de datos externos]=========================*/

DEBUG_PRINT_ENABLE;

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

// Prototipo de funcion de la tarea
void barrierControlTask( void* taskParmPtr );
void trainControlTask( void* taskParmPtr );

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main(void)
{
   // ---------- CONFIGURACIONES ------------------------------
   // Inicializar y configurar la plataforma
   boardConfig();

   // UART for debug messages
    initUart();
    uartWriteString(UART_USB,"Firmware maqueta monitor de barrera\r\n");
   //debugPrintConfigUart( UART_USB, 115200 );
   //debugPrintlnString( "Blinky con freeRTOS y sAPI." );

   // Led para dar se�al de vida
    gpioWrite( DO0, OFF );
    gpioWrite( DO1, OFF );
    gpioWrite( DO2, OFF );
    gpioWrite( DO3, OFF );
    gpioWrite( DO4, OFF );
    gpioWrite( DO5, OFF );
    gpioWrite( DO6, OFF );
    gpioWrite( DO7, OFF );

   // Crear tarea en freeRTOS
    initUartTask(tskIDLE_PRIORITY+1);

	xTaskCreate(
            barrierControlTask,                     // Funcion de la tarea a ejecutar
		          (const char *)"barrierControl",     // Nombre de la tarea como String amigable para el usuario
					 configMINIMAL_STACK_SIZE*2, // Tama�o del stack de la tarea
					 0,                          // Parametros de tarea
					 tskIDLE_PRIORITY+2,         // Prioridad de la tarea
					 0                           // Puntero a la tarea creada en el sistema
              );
    xTaskCreate(
            trainControlTask,                     // Funcion de la tarea a ejecutar
            (const char *)"trainControl",     // Nombre de la tarea como String amigable para el usuario
            configMINIMAL_STACK_SIZE*2, // Tama�o del stack de la tarea
            0,                          // Parametros de tarea
            tskIDLE_PRIORITY+1,         // Prioridad de la tarea
            0                           // Puntero a la tarea creada en el sistema
    );

    initPcManagerTask(tskIDLE_PRIORITY+3);

   // Iniciar scheduler
	vTaskStartScheduler();

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {
      // Si cae en este while 1 significa que no pudo iniciar el scheduler
   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontroladore y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}


/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

// Implementacion de funcion de la tarea
void barrierControlTask( void* taskParmPtr )
{
   // ---------- CONFIGURACIONES ------------------------------



   // ---------- REPETIR POR SIEMPRE --------------------------
	while(TRUE)
   {

       //intento actualizar el modo
       xQueueReceive(modelModeQueue, &modelMode, 0);

       switch (modelMode){
           case safeFail:
               //si es falla segura bajo la barrera siempre
               gpioWrite(DO0,ON);
               break;
           case unsafeFail:
               //si es falla segura subo la barrera siempre
               gpioWrite(DO0,OFF);
               break;
           case normal:
               break;
       }

      // Intercambia el estado del LEDB
       bool_t leftSensorLeftDirAct = !gpioRead(DI0);
       bool_t leftSensorRightDirAct = !gpioRead(DI1);
       bool_t rightSensorLeftDirAct = !gpioRead(DI2);
       bool_t rightSensorRightDirAct = !gpioRead(DI3);

       uint8_t newSensorState = 0;
       newSensorState |= leftSensorLeftDirAct << 3;
       newSensorState |= leftSensorRightDirAct << 2;
       newSensorState |= rightSensorLeftDirAct << 1;
       newSensorState |= rightSensorRightDirAct << 0;

       if(newSensorState == barrierControlLastSensorState){
           switch (barrierControlState){

               case noTrain:
                   if(!leftSensorLeftDirAct && leftSensorRightDirAct && !rightSensorLeftDirAct && !rightSensorRightDirAct){
                       barrierControlState = trainPassRightDir;
                       pc_uart(PC_MSG_FORMAT_CHANGE_BARRIER_STATE,PC_MSG_BARRIER_STATE_RIGHT_TRAIN);
                       if(modelMode == normal){
                           gpioWrite(DO0,ON);
                       }

                   } else if (!leftSensorLeftDirAct && !leftSensorRightDirAct && rightSensorLeftDirAct && !rightSensorRightDirAct){
                       barrierControlState = trainPassLeftDir;
                       pc_uart(PC_MSG_FORMAT_CHANGE_BARRIER_STATE,PC_MSG_BARRIER_STATE_LEFT_TRAIN);
                       if(modelMode == normal){
                           gpioWrite(DO0,ON);
                       }
                   }
                   break;
               case trainPassRightDir:
                   if(!leftSensorLeftDirAct && !leftSensorRightDirAct && !rightSensorLeftDirAct && rightSensorRightDirAct){
                       barrierControlState = noTrain;
                       pc_uart(PC_MSG_FORMAT_CHANGE_BARRIER_STATE,PC_MSG_BARRIER_STATE_NO_TRAIN);
                       if(modelMode == normal){
                           gpioWrite(DO0,OFF);
                       }
                   } else if (leftSensorLeftDirAct && !leftSensorRightDirAct && !rightSensorLeftDirAct && !rightSensorRightDirAct){
                       barrierControlState = noTrain;
                       pc_uart(PC_MSG_FORMAT_CHANGE_BARRIER_STATE,PC_MSG_BARRIER_STATE_NO_TRAIN);
                       if(modelMode == normal){
                           gpioWrite(DO0,OFF);
                       }
                   }
                   break;
               case trainPassLeftDir:
                   if(leftSensorLeftDirAct && !leftSensorRightDirAct && !rightSensorLeftDirAct && !rightSensorRightDirAct){
                       barrierControlState = noTrain;
                       pc_uart(PC_MSG_FORMAT_CHANGE_BARRIER_STATE,PC_MSG_BARRIER_STATE_NO_TRAIN);
                       if(modelMode == normal){
                           gpioWrite(DO0,OFF);
                       }
                   } else if(!leftSensorLeftDirAct && !leftSensorRightDirAct && !rightSensorLeftDirAct && rightSensorRightDirAct){
                       barrierControlState = noTrain;
                       pc_uart(PC_MSG_FORMAT_CHANGE_BARRIER_STATE,PC_MSG_BARRIER_STATE_NO_TRAIN);
                       if(modelMode == normal){
                           gpioWrite(DO0,OFF);
                       }
                   }
                   break;
           }
       }

       barrierControlLastSensorState = newSensorState;


      // Envia la tarea al estado bloqueado durante 100ms
		vTaskDelay( 100 / portTICK_RATE_MS );
	}
}


void trainControlTask( void* taskParmPtr ){
    while(TRUE) {
        bool_t endLeftSensorLeftDirAct = !gpioRead(DI4);
        bool_t endLeftSensorRightDirAct = !gpioRead(DI5);
        bool_t endRightSensorLeftDirAct = !gpioRead(DI6);
        bool_t endRightSensorRightDirAct = !gpioRead(DI7);

        if (endLeftSensorLeftDirAct || endLeftSensorRightDirAct || endRightSensorLeftDirAct ||
            endRightSensorRightDirAct) {
            if(train_activated){
                gpioWrite(DO1, OFF);
                pc_uart(PC_MSG_FORMAT_CHANGE_TRAIN_STATE,PC_MSG_TRAIN_STATE_DESACTIVED);
                train_activated = false;
            }
        } else {
            if(!train_activated) {
                gpioWrite(DO1, ON);
                pc_uart(PC_MSG_FORMAT_CHANGE_TRAIN_STATE, PC_MSG_TRAIN_STATE_ACTIVED);
                train_activated = true;
            }
        }

        vTaskDelay(200 / portTICK_RATE_MS);
    }
}

/*==================[fin del archivo]========================================*/
