/*
 * Trabalho NP2.1 - Optativa: Tópicos em computação IV
 * Implementar solução para o problema Sleeping Barber Problem
 * Utilizando mutex e semáfaros
 * SO FreeRTOS rodando no Arduíno UNO.
 * 
 * Jovani de Souza - 1311100017
 * Ciência da Computação - UFFS
 * 
 */

 //#define INCLUDE_vTaskSuspend     1 / in FreeRTOSConfig.h

#include "FreeRTOS_AVR.h"
#include "basic_io_avr.h"


static void main_task(void *pvParameters );//task cliente

static void barber_task(void *pvParameters);//task barbeiro


SemaphoreHandle_t xMutex;//mutex
SemaphoreHandle_t xSemaphore;//counting semafaro

TaskHandle_t xHandleBarbeiro;// handle da task barbeiro


void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);

  xMutex = xSemaphoreCreateMutex(); 
  xSemaphore = xSemaphoreCreateCounting( 2, 2 );
  

  if(xMutex != NULL && xSemaphore != NULL ){

    xTaskCreate( main_task, "CLIENTE1", 100, "cliente 1", 2, NULL );
     
    xTaskCreate( main_task, "CLIENTE2", 100, "cliente 2", 2, NULL );

    xTaskCreate( main_task, "CLIENTE3", 100, "cliente 3", 2, NULL );

    xTaskCreate( main_task, "CLIENTE4", 100, "cliente 4", 2, NULL );

    xTaskCreate( main_task, "CLIENTE5", 100, "cliente 5", 2, NULL );
     
    //xTaskCreate( main_task, "CLIENTE6", 100, "cliente 6", 2, NULL );

    //xTaskCreate( main_task, "CLIENTE7", 100, "cliente 7", 2, NULL );

    //xTaskCreate( main_task, "CLIENTE8", 100, "cliente 8", 2, NULL );
    


    xTaskCreate(barber_task, "BARBEIRO", 100, "barbeiro", 1, &xHandleBarbeiro);

  
    vTaskStartScheduler();
  
  }else{
     vPrintString("Erro ao criar Mutex e semáfaro\n");
  }

  for(;;){}

}

static void main_task( void *pvParameters ){//terafa dos clientes

  const char *pcTaskName = (char *) pvParameters ;
  const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;
  int rand_esperar = 1000;
  int rand_voltar = 10000;

  rand_voltar = random(5000 , 10000);
  rand_esperar = random(1000 , 4000);
  vTaskDelay(rand_esperar / portTICK_PERIOD_MS);//coloca as tasks imediatamente para esperar um tempo randomico antes de irem para o barbeiro

  for(;;){

    //code cliente here
    
    if(xSemaphoreTake( xSemaphore,  pdMS_TO_TICKS(100) ) == pdTRUE){//se conseguir um lugar na fila
      //vPrintString("\n acorde.\n");
      vTaskResume( xHandleBarbeiro);//acorda o barbeiro
       
       if(xSemaphoreTake( xMutex,  portMAX_DELAY ) == pdTRUE){//tenta cortar o cabelo pegando o mutex
        
        xSemaphoreGive( xSemaphore );//libera o lugar dele na fila
        vPrintString((char *) pvParameters );
        vPrintString("\nCliente está sendo atendido.\n");
        vTaskDelay(xDelay*2);//tempo para cortar o cabelo
        xSemaphoreGive( xMutex );//devolve o mutex para o proximo ser atendido
        //vTaskDelay(rand_voltar / portTICK_PERIOD_MS);//tempo que leva para voltar à barbearia
        vTaskDelay(xDelay*10);//alterar para rand para variar o tempo de volta a barbearia
       }
    }else{
      
      vTaskDelay(rand_voltar / portTICK_PERIOD_MS);//vai entra nesse else, caso não tenha lugar na barbearia, espera um tempo e retorna para a barbearia
    
    }
      
  }//fim do loop
  
}//fim da tarefa clientes

static void barber_task(void *pvParameters){

  const char *pcTaskName = (char *) pvParameters ;

   for(;;){
    
      vTaskDelay(50 / portTICK_PERIOD_MS);
      if(xSemaphoreTake( xMutex,  portMAX_DELAY ) == pdTRUE){//condição para o barbeiro dormir
        
        vPrintString("\nBarbeiro: vou dormir.\n\n");
        xSemaphoreGive( xMutex );
        vTaskSuspend(xHandleBarbeiro);
      }
   
   }
}



void loop() {
  // put your main code here, to run repeatedly:

}
