#include <Wire.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include "Mona_ESP_lib.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include<string>
 
typedef struct messageBuffer {
    int id;
    int timeRecive;
    float fitness;
    float *recive_params;
}messageBuffer;


const char* ssid = "Mona_ESP";
const char* password =  "monaisgreat";
WiFiServer server(80);


// server buffer 
int _id, _max_size_message;
messageBuffer * _ServerBuffer;
int _ServerBufferSize, _params_size, _server_iteration;
bool _random_choice;

void initial_server_buffer(){
    _id = 5;
    _random_choice = true;
    _params_size = 2;
   _ServerBufferSize = 8;

   _server_iteration = 0;
   _max_size_message = 1000;
   _ServerBuffer = (messageBuffer *)malloc(sizeof(messageBuffer) * _ServerBufferSize);
  for (int i = 0 ; i <_ServerBufferSize; i++ ){
      _ServerBuffer[i].recive_params = (float *)malloc(sizeof(float) * _params_size);
  }
  for (int i = 0 ; i <_ServerBufferSize; i++ ){
    _ServerBuffer[i].id = -1;
    _ServerBuffer[i].timeRecive = -1;
    _ServerBuffer[i].fitness = -1.0;
    for (int j = 0; j < _params_size; j++){
      _ServerBuffer[i].recive_params[j] = 0.0;
    }
  }
}


void setup()
{
  

  srand(analogRead(breakout_14));
  
  Mona_ESP_init();
  
  
  WiFi.softAP(ssid);
  IPAddress myIP = WiFi.softAPIP();
  server.begin();
  initial_server_buffer();
 
  delay(150);  
}





void loop(){
  listenParams();
  
}

void listenParams(){

  
  WiFiClient client = server.available();   // listen for incoming clients
  if (client) { 
      int cpt_message = 0;
      char recive_message[_max_size_message * 3];
      String currentLine = "";             
      while (client.connected()) {           
        if (client.available()) {             
          char c = client.read();             
          recive_message[cpt_message] = c;
          cpt_message += 1;
          if (c == '\n') {                   
            if (currentLine.length() == 0) {
         
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();
              int index_selected = -1;
              if(_random_choice){
                index_selected = choice_random_message(_params_size);  
              }
              else{
                  index_selected = choice_best_message(_params_size);  
                  
              }
    
              if (index_selected != -1){
                char send_message[_max_size_message];
                char startMessage[] = "";
                write_message(send_message,startMessage ,_ServerBuffer[index_selected].id, _ServerBuffer[index_selected].fitness, _ServerBuffer[index_selected].recive_params ,_params_size ) ;
                client.println(send_message); 
              }
              else{
                client.println("NOTHING");
              }
              client.println();
            
              break;
            } else {    
              currentLine = "";
            }
          } else if (c != '\r') {  // if you got anything else but a carriage return character,
            currentLine += c;      // add it to the end of the currentLine
          }
        }
      }
      read_and_save_message(recive_message,_ServerBuffer, _ServerBufferSize,_params_size);
      client.stop();
    }
}

int choice_random_message(int paramSize){

 
  bool buffervide = true;
  
  for (int i = 0; i < _ServerBufferSize ; i++){
    if(_ServerBuffer[i].id != -1){
        buffervide = false;
    }
  }
  if(buffervide){
    return -1;
  }
  int randomIndex = ( rand () % _ServerBufferSize);
  while(_ServerBuffer[randomIndex].id == -1){
    randomIndex = ( rand () % _ServerBufferSize);
  }
  return randomIndex;
}

// TO DO
int choice_best_message(int paramSize){
  return choice_random_message(paramSize);
}

void write_message(char *message,char * startMessage ,int id , float fitness, float *params , int paramSize ){

  char start[] = "S";
  char next[] = "N";
  char END[] = "E";
  char fin[] = "\0";
  strcpy(message, startMessage);
  strcat(message, start);
  char id_string[_max_size_message/ 50];
  sprintf(id_string, "%d",id);
  strcat(id_string, next);
  strcat(message, id_string);
  char fitness_string[_max_size_message/ 50];
  sprintf(fitness_string, "%3.5f",fitness);
  strcat(fitness_string, next);
  strcat(message, fitness_string);
  for (int i = 0 ; i < paramSize ; i++){
    char params_string[_max_size_message/ 50];
    sprintf(params_string,"%2.6f", params[i]);
    strcat(params_string,next );
    strcat(message, params_string);
  }
  strcat(message, END);
  strcat(message, fin);
}


void save_message(int id_recive,float fitness_recive,float *params_recive, int paramSize,messageBuffer * bufferToSave, int buffSize ){
  // find if there is a message from this sender 
  int index = -1;
  int i = 0;
  while(i< buffSize and index == -1){
    messageBuffer elem = bufferToSave[i];
    if(elem.id == id_recive){
        index = i;     
     }
     i +=1;
  }
  // find if there is a free place in buffer
  if (index == -1){
    i = 0;
    while(i < buffSize and index == -1){
      messageBuffer elem = bufferToSave[i];
      if(elem.id == -1){
        index = i;
      }
      i += 1;
    }
  }
  if(index != -1){
     bufferToSave[index].id = id_recive;
     bufferToSave[index].timeRecive = 0;
     bufferToSave[index].fitness =  fitness_recive;
     for (i = 0 ; i <paramSize ; i++){
        bufferToSave[index].recive_params[i] = params_recive[i];
     }  
     
  }
  free(params_recive);
}
void read_and_save_message(char *message, messageBuffer * bufferToSave, int buffSize, int paramSize){
 

    char messagecopy[_max_size_message];
    int i = 0;
    int j = 0;
    // message has a end mark
    while(i < _max_size_message * 3 and message[i] != 'E'){ i+=1;}
    if (i == _max_size_message* 3){return;}
    i = 0;
    // find start of message
    while(message[i] != 'S'){i+=1;}
    i+=1;
    // extract real message from revive message 
    while(message[i] != 'E'){
      messagecopy[j] = message[i];
      i +=1;
      j +=1;
    }
    messagecopy[j] = '\0';
    // extract id
    char id_string[_max_size_message/ 50];
    i = 0;
    j = 0;
    while(messagecopy[i] != 'N'){
      id_string[j] = messagecopy[i];
      i +=1;
      j +=1;
    }
    i += 1;
    id_string[j] = '\0';
    int id_recive = atoi(id_string);
    // extract fitness
    char fitness_string[_max_size_message/ 50];
    j = 0;
    while(messagecopy[i] != 'N'){
      fitness_string[j] = messagecopy[i];
      i +=1;
      j +=1;
    }
    i += 1;
    fitness_string[j] = '\0';
    float fitness_recive = atof(fitness_string);
    // params extract
    char **other_params = (char **)malloc(sizeof(char *)* paramSize);
    for (int k = 0 ; k < paramSize; k++){
      other_params[k] = (char *)malloc(sizeof(char)* (_max_size_message/ 50));
    }
    for (int k = 0; k < paramSize; k++ ){
      j = 0;
      while(messagecopy[i] != 'N'){
        other_params[k][j] = messagecopy[i];
        i +=1;
        j +=1;
      }
      other_params[k][j]= '\0';
      i += 1;
    }
    float *params_recive = (float *)malloc(sizeof(float) * paramSize);
    for (int k = 0; k < paramSize; k++ ){
      params_recive[k] = atof(other_params[k]);
      free( other_params[k]);
    }
    free(other_params);
    save_message(id_recive,fitness_recive,params_recive, paramSize, bufferToSave,buffSize );
}
