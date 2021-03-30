#include "ibus.h"


// Analógico // --

//Numero de entradas analógicas,cada analógico tem 2 potenciometros a serem ligados,totalizando 2 potenciometros a serem lidos
#define ANALOG_INPUTS_COUNT 2 


//Portas analógicas dos eixos do Analógico 
#define AxisX A0
#define AxisY A1

//Portas analógicas do Arduino para conectar os fios do analógico
byte analogPins[] = {AxisX,AxisY};





// Botões de ação // --

//Portas digitais dos botões de ação
#define buttonA 2
#define buttonB 3
#define StickButton 4

// Vetor para portas digitais 
byte digitalPins[] = {buttonA,buttonB,StickButton};

//Vetor para o estado das portas digitais,os botões estão ligados em lógica invertida,por isso usam esse vetor para inverter seus estados no código e retornarem eles a uma lógica normal
int Button_State[]={LOW,LOW,LOW};

//LEDs dos botões de ação
#define LED_buttonA 5
#define LED_buttonB  6





//Numero de entradas digitais,que são o numero de botões liga/desliga a serem usados no controle
#define DIGITAL_INPUTS_COUNT 3 


//Taxa de atualização da comunicação em milisegundos(Não mexer)
#define UPDATE_INTERVAL 10 


//Taxa de comunicação serial,esse valor é especifico para o progama do controle virutal
#define BAUD_RATE 115200 


//Os tipos de entradas pro controle são somados 
#define NUM_CHANNELS ( (ANALOG_INPUTS_COUNT)+ (DIGITAL_INPUTS_COUNT) )

//E aqui esse valor informa quantos canais vão receber os dados dos botões e analógicos por voce controlados
IBus ibus(NUM_CHANNELS);

int i = 0;

void setup()
{
  
  pinMode(LED_buttonA,OUTPUT);
  pinMode(LED_buttonB,OUTPUT);
    for(i=0;i<DIGITAL_INPUTS_COUNT;i++)
  {
    pinMode(digitalPins[i],INPUT_PULLUP);
     
  }

   //é aqui que se inicia a comunicação dos dados do Arduino com o computador e as aplicações de Joystic Virtual
   Serial.begin(BAUD_RATE);
  
}


 void loop()
{
   //Inverte a lógica do input pullup e caso o botão esteja pressionado acende o led
    for(i=0;i<DIGITAL_INPUTS_COUNT;i++)
    {
      
      if(digitalRead(digitalPins[i])==LOW)
      {
        Button_State[i]=HIGH;
        if(i==0)
        {
         digitalWrite(LED_buttonA,HIGH);
        }
        else if(i==1)
        {
          digitalWrite(LED_buttonB,HIGH);
        }
        else if(i==2)
        {
          digitalWrite(LED_buttonA,HIGH);
           digitalWrite(LED_buttonB,HIGH);
        }
      }
      else if(digitalRead(digitalPins[i])==HIGH)
       {
         Button_State[i]=LOW;
        if(i==0)
        {
         digitalWrite(LED_buttonA,LOW);
        }
        else if(i==1)
        {
          digitalWrite(LED_buttonB,LOW);
        }
        else if(i==2)
        {
          digitalWrite(LED_buttonA,LOW);
           digitalWrite(LED_buttonB,LOW);
        }
        
       }
    }
  
  

 //Aqui se inicia a comunicação dos estados dos botões e analógicos com o PC
   unsigned long time = millis();
   

   
    ibus.begin();
  
    //Aqui os estados dos botões e do analógico são lidos e enviados para a comunicação serial
      for(i=0; i < ANALOG_INPUTS_COUNT; i++)
    ibus.write(analogRead(analogPins[i]));

    for(i=0; i < DIGITAL_INPUTS_COUNT; i++)
    ibus.write(Button_State[i] == HIGH ? 1023 : 0);
    
    ibus.end();

    time = millis() - time; // time elapsed in reading the inputs
    if(time < UPDATE_INTERVAL)
    // sleep till it is time for the next update
    delay(UPDATE_INTERVAL  - time);

}
