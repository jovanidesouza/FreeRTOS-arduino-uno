/*
 * Trabalho NP1 - Optativa: Tópicos em computação IV
 * Implementar solução para o jantar dos filósofos
 * Utilizando apenas os conceitos de tarefas e filas no
 * SO FreeRTOS rodando no Arduíno UNO.
 * 
 * Jovani de Souza - 1311100017
 * Ciência da Computação - UFFS
 * 
 * 
 * OBS: Nativamente todos os filófos tem o mesmo comportamento e também o mesmo tempo de execução
 * portanto o resultado pode parecer "viciado" na mesma sequencia. Para verificar o comportamento
 * de outra forma, basta remover os comentários das variáveis rand_comer e rand_pensar ao decorrer 
 * do código. e com isso, o tempo de bloqueio de cada tarefa será aleatório.
 * 
 */


#include "FreeRTOS_AVR.h"
#include "basic_io_avr.h"


static void main_task(void *pvParameters );

QueueHandle_t garfo0;
QueueHandle_t garfo2;
QueueHandle_t garfo4;
QueueHandle_t garfo6;
QueueHandle_t garfo8;


void setup() {
  
  Serial.begin(9600);

  garfo0 = xQueueCreate( 1, sizeof( long ) );
  garfo2 = xQueueCreate( 1, sizeof( long ) );
  garfo4 = xQueueCreate( 1, sizeof( long ) );
  garfo6 = xQueueCreate( 1, sizeof( long ) );
  garfo8 = xQueueCreate( 1, sizeof( long ) );

  if(garfo0 != NULL && garfo2 != NULL && garfo4 != NULL && garfo6 != NULL && garfo8 != NULL){
    

    xTaskCreate( main_task, "Filosofo1", 100, "Filosofo 1", 1, NULL );
     
    xTaskCreate( main_task, "Filosofo3", 100, "Filosofo 3", 1, NULL );

    xTaskCreate( main_task, "Filosofo5", 100, "Filosofo 5", 1, NULL );

    xTaskCreate( main_task, "Filosofo7", 100, "Filosofo 7", 1, NULL );
  
    xTaskCreate( main_task, "Filosofo9", 100, "Filosofo 9", 1, NULL );
    
      
    vTaskStartScheduler();
  
  }else{
     vPrintString("Erro ao criar garfos.\n");
  }

  for(;;){}
}

static void main_task( void *pvParameters ){

  const char *pcTaskName = (char *) pvParameters ;
  portBASE_TYPE status;
  int libera_fila=0, rand_comer=1000, rand_pensar=2000;

  vPrintString((char *) pvParameters );
  vPrintString(" está pensando.\n");
  //rand_pensar = random(1000 , 4000);
  vTaskDelay( pdMS_TO_TICKS(rand_pensar) ) ;

  for(;;){

    vTaskPrioritySet( NULL, 2  );
    pcTaskName = (char *) pvParameters;
//primeiro filósofo
    if(pcTaskName == "Filosofo 1"){

      status = xQueueSend( garfo0, 1, portMAX_DELAY );
      status = xQueueSend( garfo2, 1, portMAX_DELAY );
      
      vPrintString("Filosofo 1 está comendo.\n");
      vTaskPrioritySet( NULL, 1  );
      //rand_comer= random(100 , 1000);
      vTaskDelay( pdMS_TO_TICKS( rand_comer ) ) ;
      
      status = xQueueReceive( garfo0, &libera_fila, 0 );
      status = xQueueReceive( garfo2, &libera_fila, 0 );
      vPrintString("Filosofo 1 está pensando.\n");
      //rand_pensar= random(2000, 8000);
      vTaskDelay( pdMS_TO_TICKS( rand_pensar ) ) ;
      //taskYIELD();
   }
//segundo filósofo
   if(pcTaskName == "Filosofo 3"){

      status = xQueueSend( garfo2, 1, portMAX_DELAY );
      status = xQueueSend( garfo4, 1, portMAX_DELAY );
      
      vPrintString("Filosofo 3 está comendo.\n");
      vTaskPrioritySet( NULL, 1  );
      //rand_comer= random(100 , 1000);
      vTaskDelay( pdMS_TO_TICKS( rand_comer ) ) ;
      
      status = xQueueReceive( garfo2, &libera_fila, 0 );
      status = xQueueReceive( garfo4, &libera_fila, 0 );
      vPrintString("Filosofo 3 está pensando.\n");
      //rand_pensar= random(2000, 8000);
      vTaskDelay( pdMS_TO_TICKS( rand_pensar ) ) ;
      //taskYIELD();
   }
//terceiro filósofo
   if(pcTaskName == "Filosofo 5"){

      status = xQueueSend( garfo4, 1, portMAX_DELAY );
      status = xQueueSend( garfo6, 1, portMAX_DELAY );
      
      vPrintString("Filosofo 5 está comendo.\n");
      vTaskPrioritySet( NULL, 1  );
      //rand_comer= random(100 , 1000);
      vTaskDelay( pdMS_TO_TICKS( rand_comer ) ) ;
      
      status = xQueueReceive( garfo4, &libera_fila, 0 );
      status = xQueueReceive( garfo6, &libera_fila, 0 );
      vPrintString("Filosofo 5 está pensando.\n");
      //rand_pensar= random(2000, 8000);
      vTaskDelay( pdMS_TO_TICKS( rand_pensar ) ) ;
      //taskYIELD();
   }
//quarto filósofo
   if(pcTaskName == "Filosofo 7"){

      status = xQueueSend( garfo6, 1, portMAX_DELAY );
      status = xQueueSend( garfo8, 1, portMAX_DELAY );
      
      vPrintString("Filosofo 7 está comendo.\n");
      vTaskPrioritySet( NULL, 1  );
      //rand_comer= random(100 , 1000);
      vTaskDelay( pdMS_TO_TICKS( rand_comer ) ) ;
      
      status = xQueueReceive( garfo6, &libera_fila, 0 );
      status = xQueueReceive( garfo8, &libera_fila, 0 );
      vPrintString("Filosofo 7 está pensando.\n");
      //rand_pensar= random(2000, 8000);
      vTaskDelay( pdMS_TO_TICKS( rand_pensar ) ) ;
      //taskYIELD();
   }
//quinto filósofo
   if(pcTaskName == "Filosofo 9"){

      status = xQueueSend( garfo8, 1, portMAX_DELAY );
      status = xQueueSend( garfo0, 1, portMAX_DELAY );
      
      vPrintString("Filosofo 9 está comendo.\n");
      vTaskPrioritySet( NULL, 1  );
      //rand_comer= random(100 , 1000);
      vTaskDelay( pdMS_TO_TICKS( rand_comer ) ) ;
      
      status = xQueueReceive( garfo8, &libera_fila, 0 );
      status = xQueueReceive( garfo0, &libera_fila, 0 );
      vPrintString("Filosofo 9 está pensando.\n");
      //rand_pensar= random(2000, 8000);
      vTaskDelay( pdMS_TO_TICKS( rand_pensar ) ) ;
      //taskYIELD();
   }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
