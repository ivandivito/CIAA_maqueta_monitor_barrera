//
// Created by ivan on 09/09/17.
//

#include "PcManager.h"

char cmd_buffer[CMD_BUFFER_SIZE];
int cmd_buffer_index = 0;

//esta tarea procesa los comandos provinientes por USB
void pcManagerTask(void * a){

    char cmdChar;
    modelMode_t modelMode;

    while(TRUE) {
        //espero a recibir un comando entero
        while(TRUE){

            //levanto proximo caracter
            xQueueReceive(inputUartUsbQueue, &cmdChar, portMAX_DELAY);

            if(cmdChar == '\r' || cmdChar == '\n'){
                cmd_buffer[cmd_buffer_index] = '\0';
                cmd_buffer_index = 0;

                //si recibo un comando entero corto el while para procesarlo
                break;
            } else {
                //guardo y reinicio el buffer en caso de overflow
                cmd_buffer[cmd_buffer_index] = cmdChar;
                cmd_buffer_index++;
                if(cmd_buffer_index >= CMD_BUFFER_SIZE){
                    cmd_buffer_index = 0;
                }
            }

        }

        //busco el largo del comando
        size_t cmd_length = getCmdLength();

        //si la linea de comando no empieza con el caracter establecido o es mas corta que el largo minimo corto
        if(cmd_length < 2 || cmd_buffer[0] != SERIAL_OPERATOR_CMD){
            continue;
        }

        //reviso que comando es

        if(!isdigit(cmd_buffer[1])){

            switch (cmd_buffer[1]){
                //comando de cambio de modo de la maqueta
                case SERIAL_OPERATOR_MODE:
                    if(cmd_length != 4 || cmd_buffer[2] != SERIAL_OPERATOR_ASSIGN)
                        break;

                    switch (cmd_buffer[3]){
                        case SERIAL_OPERATOR_MODE_NORMAL:
                            modelMode = normal;
                            xQueueSendToBack(modelModeQueue,&modelMode,portTICK_PERIOD_MS);
                            break;

                        case SERIAL_OPERATOR_MODE_SAFE_FAIL:
                            modelMode = safeFail;
                            xQueueSendToBack(modelModeQueue,&modelMode,portTICK_PERIOD_MS);
                            break;

                        case SERIAL_OPERATOR_MODE_UNSAFE_FAIL:
                            modelMode = unsafeFail;
                            xQueueSendToBack(modelModeQueue,&modelMode,portTICK_PERIOD_MS);
                            break;

                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }

        }

    }
}

//inicio las tareas de procesamiento de comandos
void initPcManagerTask(uint32_t priority){

    modelModeQueue = xQueueCreate(QUEUE_SIZE, sizeof(modelMode_t));

    xTaskCreate(pcManagerTask, (const char *)"pcManagerTask", configMINIMAL_STACK_SIZE, 0, priority, 0);
}

//busco el largo del comando
size_t getCmdLength(){

    size_t cmd_length = 0;
    while (cmd_buffer[cmd_length] != '\0'){
        cmd_length++;
        //si recorri el buffer y no encotnre el caracter de finalizacion no proceso el comando
        if(cmd_length == CMD_BUFFER_SIZE){
            return 0;
        }
    }
    return cmd_length;
}
