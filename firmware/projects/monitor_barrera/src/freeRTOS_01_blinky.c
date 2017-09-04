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

/*==================[inlcusiones]============================================*/

// Includes de FreeRTOS
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include "sapi.h"        // <= sAPI header

/*==================[definiciones y macros]==================================*/

typedef enum{noTrain, trainPassLeftDir, trainPassRightDir } barrierControlState_t;

/*==================[definiciones de datos internos]=========================*/

barrierControlState_t barrierControlState = noTrain;
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
   debugPrintConfigUart( UART_USB, 115200 );
   debugPrintlnString( "Blinky con freeRTOS y sAPI." );

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
                       gpioWrite(DO0,ON);
                   } else if (!leftSensorLeftDirAct && !leftSensorRightDirAct && rightSensorLeftDirAct && !rightSensorRightDirAct){
                       barrierControlState = trainPassLeftDir;
                       gpioWrite(DO0,ON);
                   }
                   break;
               case trainPassRightDir:
                   if(!leftSensorLeftDirAct && !leftSensorRightDirAct && !rightSensorLeftDirAct && rightSensorRightDirAct){
                       barrierControlState = noTrain;
                       gpioWrite(DO0,OFF);
                   }
                   break;
               case trainPassLeftDir:
                   if(leftSensorLeftDirAct && !leftSensorRightDirAct && !rightSensorLeftDirAct && !rightSensorRightDirAct){
                       barrierControlState = noTrain;
                       gpioWrite(DO0,OFF);
                   }
                   break;
           }
       }

       barrierControlLastSensorState = newSensorState;


      // Envia la tarea al estado bloqueado durante 00ms
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
            gpioWrite(DO1, OFF);
        } else {
            gpioWrite(DO1, ON);
        }

        vTaskDelay(100 / portTICK_RATE_MS);
    }
}

/*==================[fin del archivo]========================================*/
