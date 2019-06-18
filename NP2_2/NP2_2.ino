/*
 * Trabalho NP2.2 - Optativa: Tópicos em computação IV
 * Implementar solução para o problema Sleeping Barber Problem
 * Utilizando task notification
 * SO FreeRTOS rodando no Arduíno UNO.
 * 
 * Jovani de Souza - 1311100017
 * Ciência da Computação - UFFS
 * 
 */


#include "FreeRTOS_AVR.h"
#include "basic_io_avr.h"


static void cliente_task(void *pvParameters );//task cliente
static void barber_task(void *pvParameters);//task barbeiro

TaskHandle_t xHandleBarbeiro;// handle da task barbeiro
TaskHandle_t xCliente1;
TaskHandle_t xCliente2;
TaskHandle_t xCliente3;
TaskHandle_t xCliente4;
//TaskHandle_t xCliente5;

int vagas = 2;

QueueHandle_t fila;

const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;

SemaphoreHandle_t xMutex;//mutex


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  
  fila = xQueueCreate( 2, sizeof(TaskHandle_t * ) );
  xMutex = xSemaphoreCreateMutex(); 
  

  if(fila != NULL){
    
    xTaskCreate( cliente_task, "CLIENTE1", 100, "cliente 1", 2, &xCliente1 );
     
    xTaskCreate( cliente_task, "CLIENTE2", 100, "cliente 2", 2, &xCliente2 );

    xTaskCreate( cliente_task, "CLIENTE3", 100, "cliente 3", 2, &xCliente3 );

    xTaskCreate( cliente_task, "CLIENTE4", 100, "cliente 4", 2, &xCliente4 );

    //xTaskCreate( cliente_task, "CLIENTE5", 100, "cliente 5", 2, &xCliente5 );


    xTaskCreate(barber_task, "BARBEIRO", 100, "barbeiro", 1, &xHandleBarbeiro);


    vTaskStartScheduler();
  
  
  }else{
     vPrintString("Erro ao criar fila\n");
  }

  for(;;){}

  

}//fim setup

static void cliente_task(void *pvParameters ){

  const char *pcTaskName = (char *) pvParameters ;
  
  int rand_esperar = 1000;
  
  int x, y;
  rand_esperar = random(1000 , 4000);
  
  vTaskDelay(rand_esperar / portTICK_PERIOD_MS);//coloca as tasks imediatamente para esperar um tempo randomico antes de irem para o barbeiro

  for(;;){
    
    if(vagas > 0){
      vagas--;
      
      if(pcTaskName == "cliente 1"){
        xQueueSend( fila, ( void * ) &xCliente1, 0 );
      }else if(pcTaskName == "cliente 2"){
        xQueueSend( fila, ( void * ) &xCliente2, 0 );
      }else if(pcTaskName == "cliente 3"){
        xQueueSend( fila, ( void * ) &xCliente3, 0 );
      }else if(pcTaskName == "cliente 4"){
        xQueueSend( fila, ( void * ) &xCliente4, 0 );
      }
    

      xTaskNotifyGive(xHandleBarbeiro);

      x = ulTaskNotifyTake( pdTRUE, portMAX_DELAY );//bloqueio
      vagas++;
      vPrintString("\n\nbarbeiro atendendo o cliente:\n\n");
      vPrintString((char *) pvParameters );

      y = ulTaskNotifyTake( pdTRUE, portMAX_DELAY );//bloqueio
      vPrintString("\n\nserviço finalizado.\n\n");
      vTaskDelay(xDelay*10);//tempo para precisar do serviço novamente

    }else{//volta mais tarde
      vPrintString("\n\n");
      vPrintString((char *) pvParameters );
      vPrintString("\nvolto depois\n");
      vTaskDelay(xDelay*5);
    }
    
    
    //if(xSemaphoreTake( xMutex,  portMAX_DELAY ) == pdTRUE){
      //vPrintString((char *) pvParameters );
      //vTaskDelay(xDelay);
      //xTaskNotifyGive(xHandleBarbeiro);
      //vTaskDelay(xDelay);
      //xSemaphoreGive( xMutex );
      //vTaskDelay(xDelay*5);
    //}

  }

}//fim task cliente

static void barber_task(void *pvParameters ){

  TaskHandle_t corte;
  int Wakeup = 0;
  
  for(;;){
    
    Wakeup = ulTaskNotifyTake( pdTRUE, portMAX_DELAY );//max wakep == tamanhop da fila

    if( Wakeup != 0 ){
      Wakeup--;
      xQueueReceive( fila, &( corte ), portMAX_DELAY);//pega da fila o primeiro que vai ser atendido
      xTaskNotifyGive( corte );//notifica o cliente que ele sera atendido
      vTaskDelay(xDelay*2);//tempo de corte
      xTaskNotifyGive( corte );//notifica o cliente que o serviço está finalizado
      
      
    }else{
       vPrintString("\nBarbeiro: vou dormir.\n\n"); 
    }
    
  }

}//fim task barbeiro

void loop() {
  // put your main code here, to run repeatedly:

}
