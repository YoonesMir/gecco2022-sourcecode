#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
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

bool IR_values[5] = {false, false, false, false, false};


int obstacle_threshold, state, delay_time,persistent_delay;
int default_persistent_delay,default_speed,light_indice;
int light_Window_size, max_light = 4095 , low_speed;
float * light_Window;
float mu,sigma, r_left_right, diffusion_rate, light_value_t;

/// HIT params
int _id;
int _max_size_message,_action_t, _sliding_windows_size, _reward_indice,_iteration;
int _params_size, _buffer_size;
int _maturationDelay, _maturationDelayDefaultValue;
int _selectionMethod,_input_size;
float _fitnessValue;
float _minValue , _maxValue;
float _transferVolume;
float _probaMutation,_mutationVolume, _probaMutationUniform,_currentSigma;
float * _rewards;
float * _params;
bool _newParamsStatus;
messageBuffer * _buffer;
int  _packetLength_HIT;

void hit_initial(){
  
   _max_size_message = 1000;
  _sliding_windows_size = 99;
  _reward_indice = 0;
  _iteration = 0;
  _params_size = 2;
  _input_size = 2;
  _buffer_size = 1;
  _maturationDelayDefaultValue = _sliding_windows_size;
  _maturationDelay = _maturationDelayDefaultValue;
  _selectionMethod = 1;
  _fitnessValue = 0.0;
  _minValue = -1.0;
  _maxValue = 1.0;
  _transferVolume = 0.5;
  _probaMutation = 0.05;
  _mutationVolume = 0.5;
  _probaMutationUniform = 1.0;
  _currentSigma = 0.0;
  _rewards = (float *)malloc(sizeof(float) * _sliding_windows_size);
  reset();
  resetFitness();
  if ( _transferVolume == 0.0 )
        _packetLength_HIT = 1; // default minimal value
  else if ( _transferVolume == 1.0 )
      _packetLength_HIT = _params_size;
  else
      _packetLength_HIT = (int)(_params_size * _transferVolume);
  
}
void setup()
{
  _id = 6;


  
  Mona_ESP_init();
  srand(analogRead(breakout_14));
  
  
  Set_LED(1, 0, 0, 0);
  Set_LED(2, 0, 0, 0);
 
  
  state = 0;
  
  // locomotion params
  obstacle_threshold = 35;
  diffusion_rate = 0.05;
  default_speed = 120;
  low_speed = 40;
  
  // random normal params
  mu = 0.0;
  sigma = 1.0;

  //delay time between 2 iteration of loop
  delay_time = 100;

  // turning delay in case of obstacle in front 
  persistent_delay = 0;
  default_persistent_delay = 7;
  r_left_right = 0.0;
  
  // params for a smooth  light value
  light_indice = 0;
  light_Window_size = 1;
  light_value_t = 0.0;
  light_Window = (float *)malloc(sizeof(float) * light_Window_size);
  for (int i = 0 ; i < light_Window_size ; i++){
    light_Window[i] = light_value_t;
  }
  
  // HIT params init
  hit_initial();
  
  delay(15000);  
}

float random01(){
  return ((float) rand () / RAND_MAX);
}
void random_shuffle(int *array, int n)
{
    if (n > 1) 
    {
        int i;
        for (i = 0; i < n - 1; i++) 
        {
          int j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}


float randn_normal(float mu, float sigma)
{
  float U1, U2, W, mult;
  static float X1, X2;
  static int call = 0;
 
  if (call == 1)
    {
      call = !call;
      return (mu + sigma * (float) X2);
    }
 
  do
    {
      U1 = -1 + ((float) rand () / RAND_MAX) * 2;
      U2 = -1 + ((float) rand () / RAND_MAX) * 2;
      W = pow (U1, 2) + pow (U2, 2);
    }
  while (W >= 1 || W == 0);
 
  mult = sqrt ((-2 * log (W)) / W);
  X1 = U1 * mult;
  X2 = U2 * mult;
 
  call = !call;
  return (mu + sigma * (float) X1);
}

float get_light_value(){

  light_Window[light_indice] = analogRead(breakout_14)/ ((float) max_light);
  light_indice = (light_indice + 1) % light_Window_size;
  float light_sum = 0.0;
  for (int i = 0 ; i < light_Window_size ; i++){
    light_sum += light_Window[i];
  }
  return light_sum / light_Window_size;
}


void fronter(){
 

  if (state == 0 ) { // forward with diffusion, not obstacle 
    Set_LED(1, 20, 0, 0);
    Set_LED(2, 20, 0, 0);
    float r = random01();
    if (r < diffusion_rate){
      r = randn_normal(mu,sigma);     
      if (r <=0){
        Motors_spin_left(default_speed);
      }
      else{
        Motors_spin_right(default_speed);
      }
    }
    else{
      Motors_forward(default_speed);
    }  
  
  }
  if (state == 1) { // forward slowly , obstacle in front
    Set_LED(1, 20, 0, 0);
    Set_LED(2, 20, 0, 0);
    Motors_forward(low_speed);
  }
  
  if (state == 2){ // turn to right , obstacle in right
    Set_LED(1, 20, 0, 0);
    Set_LED(2, 0, 20, 0);
    Motors_spin_right(default_speed);
  }
  
  if(state == 3){ // turn to left , obtsacle in left
    Set_LED(1, 0, 20, 0);
    Set_LED(2, 20, 0, 0);
    Motors_spin_left(default_speed);
  }
}

void aligner(){

  
   if (state == 0) { // forward with diffusion, not obstacle 
    Set_LED(1, 0, 20, 0);
    Set_LED(2, 0, 20, 0);
    float r = random01();
    if (r< diffusion_rate){
      r = randn_normal(mu,sigma);     
      if (r <=0){
        Motors_spin_left(default_speed );
      }
      else{
        Motors_spin_right(default_speed);
      }
    }
    else{
      Motors_forward(default_speed);
    }  
  }
  
  if (state == 1){ // random left or right , obsrtacle in front
    Set_LED(1, 13, 7, 0);
    Set_LED(2, 13, 7, 0);
    if (persistent_delay <= 0){
      r_left_right =  -1 + (random01() * 2); 
      persistent_delay = default_persistent_delay;
    }
    
    if (r_left_right <= 0){
      Motors_spin_left(default_speed); 
    }
    else{
      Motors_spin_right(default_speed);
    }
    persistent_delay = persistent_delay - 1;
     
  }
  
  if (state == 2){ // turn to left , obstacle in right
    Set_LED(1, 0, 20, 0);
    Set_LED(2, 20, 0, 0);
    Motors_spin_left(default_speed);
  }
  
  if(state == 3){ // turn to right , obtsacle in left
    Set_LED(1, 20, 0, 0);
    Set_LED(2, 0, 20, 0);
    Motors_spin_right(default_speed);
  }
}


void set_state(){
  
  IR_values[0] = Detect_object(1,obstacle_threshold);
  IR_values[1] = Detect_object(2,obstacle_threshold);
  IR_values[2] = Detect_object(3,obstacle_threshold);
  IR_values[3] = Detect_object(4,obstacle_threshold);
  IR_values[4] = Detect_object(5,obstacle_threshold);
 
  if ( IR_values[2]  ){ //obtacle in front
    state = 1;
  }
  
  else if (   IR_values[3] or IR_values[4]){// obstacle in right
    state = 2;
  }
  else if (IR_values[0] or IR_values[1] ){ //obstacle in left
    state = 3;
  }
 
  else{ //there is no obstacle
    state = 0;
  }
  if (state != 1){
    persistent_delay = 0;
  }
  
}
// TO DO in case of hidden layers 
int computeRequiredNumberOfWeights(){
  return 2;
}
void initController()
{
    _params_size  = computeRequiredNumberOfWeights();
    _params = (float *)malloc(sizeof(float) * _params_size);
    for(int i = 0 ; i < _params_size; i++){
      _params[i] = _minValue + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(_maxValue-_minValue)));
    }
    
}
void clearReservoir(){
  for (int i = 0 ; i <_buffer_size; i++ ){
    _buffer[i].id = -1;
    _buffer[i].timeRecive = -1;
    _buffer[i].fitness = -1.0;
    for (int j = 0; j < _params_size; j++){
      _buffer[i].recive_params[j] = 0.0;
    }
  }
}
void initEvolution(){
  
  _newParamsStatus = true;
  _buffer = (messageBuffer *)malloc(sizeof(messageBuffer) * _buffer_size);
  for (int i = 0 ; i <_buffer_size; i++ ){
      _buffer[i].recive_params = (float *)malloc(sizeof(float) * _params_size);
  }
  clearReservoir();
}

void reset()
{
    initController();
    initEvolution();
}

void  resetFitness() {
  for(int i = 0 ; i < _sliding_windows_size; i++){
    _rewards[i] = 0.0;
  }
  _fitnessValue = 0.0;
}

void updateFitness(){
  light_value_t = get_light_value();
  
  _rewards[_reward_indice] = light_value_t;
  _reward_indice = (_reward_indice +1) % _sliding_windows_size;
  _fitnessValue = 0.0;
  for (int i = 0 ; i < _sliding_windows_size; i++){
    _fitnessValue += _rewards[i];
  }
}



int getNbMessage(){
  int valRet = 0;
  for (int i = 0 ; i < _buffer_size; i++){
    if(_buffer[i].id != -1){
      valRet += 1;
    }
  }
  return valRet;
}

int selectBest(){
  int bestindex = -1;
  float bestValue = -1.0;
  for (int i = 0 ; i < _buffer_size; i++){
    if(_buffer[i].id != -1 and _buffer[i].fitness >= bestValue){
      bestValue = _buffer[i].fitness;
      bestindex = i;
    }
  }
  return bestindex;
}

bool isAuthorizedSelection(int index){
  if (index < 0){
    return false;
  }
  if (_buffer[index].fitness <= _fitnessValue){
    return false;
  }
  return true;
}

void transfertParams(int index){
  int * indexes = (int * ) malloc(sizeof(int) *_params_size ) ;
  for ( int i = 0 ; i < _params_size; i++ ){
      indexes[i] = i;
  }
      
  if (_transferVolume < 1.0 ){
    random_shuffle(indexes,_params_size);
  }
  for ( int i = 0 ; i < _packetLength_HIT ; i++ ){
    _params[indexes[i]] = _buffer[index].recive_params[indexes[i]];
  }
  free(indexes);
}

 


// TO DO , for now juste random mutation
float getBoundedGaussianValue(double value, double sigma, double minValue, double maxValue ){
  float retValue = random01() * ( _maxValue - _minValue )  + _minValue;
  return retValue;
}

void mutateMixte(){
    int nbGenesToMutate = (int) (_mutationVolume * _params_size);
    if (nbGenesToMutate <= 0){
        nbGenesToMutate = 1;
    }
    int * indexes = (int * ) malloc(sizeof(int) *_params_size ) ;
    for (int i = 0 ; i < _params_size; i++ ){
        indexes[i] = i;
    }
 
    random_shuffle( indexes, _params_size );
    for ( int i = 0 ; i < nbGenesToMutate ; i++ ){
        if (random01() <= _probaMutationUniform){
            _params[indexes[i]] = random01() * ( _maxValue - _minValue )  + _minValue;
        }
        else{
            float tmp = getBoundedGaussianValue( _params[indexes[i]], _currentSigma, _minValue, _maxValue );
            _params[indexes[i]] = tmp;
        }
    }
    free(indexes);
}
void performVariation(){
  if (random01() <  _probaMutation  and _newParamsStatus == false and _maturationDelay  <= 0  ){
      mutateMixte();
      _newParamsStatus = true;
  }
}

void performSelection(){
  if( _maturationDelay <= 0){
      int buffsizeActuel = getNbMessage();
      if(buffsizeActuel  >= _buffer_size  ) 
      {
          int  index = -1;       
          switch (_selectionMethod) {
              case 1:
                  index = selectBest();
                  break;
              default:
                  exit(-1);
          }
          if(isAuthorizedSelection(index)){
              _newParamsStatus = false;
              transfertParams(index);
              performVariation();
              _newParamsStatus = true;
          }
          else{
              clearReservoir();
          }
      } 
    }
}

void stepEvolution(){
  if ( _newParamsStatus) // check for new NN parameters
    {
      _newParamsStatus = false;
      _maturationDelay = _maturationDelayDefaultValue;
      resetFitness();
      clearReservoir();
    }
    else
    {
        if(_maturationDelay > 0){
            _maturationDelay -= 1;
        }
        else
        {
            _newParamsStatus = false;
            broadcastParams();
            performSelection(); 
            if (_newParamsStatus == false){
              _maturationDelay = _maturationDelayDefaultValue;
              clearReservoir();
            }
            
        } 
    }
}
float * getInputs(){
  float *retVal = (float *)malloc(sizeof(float) * _input_size);
  retVal[0] = 1.0; //b
  retVal[1] = get_light_value();
  return retVal;
  
}
// TO DO in case hidden layer
int stepController(){
    int retVal = 0;
    float  * inputs = getInputs();
    float valOutput = 0.0;
    for (int i = 0 ; i < _params_size; i++){
          valOutput += inputs[i] * _params[i];
    }
    free(inputs);
    if(valOutput >= 0){
        retVal = 0;
    }
    else{
        retVal = 1;
    }
    return retVal;
}

int hit_algo_step(){
  updateFitness();
  stepEvolution();
  return stepController();
}


void loop(){
  _iteration += 1;
  //set_state();
  //_action_t = hit_algo_step();
  /*
  if ( _action_t == 0){
    aligner();
   }
  else{
    fronter();
   }*/
   if (_iteration % 2== 0){
      Set_LED(1, 20, 0, 0);
      Set_LED(2, 0, 20, 0);
   }
   else{
      Set_LED(1, 0, 20, 0);
      Set_LED(2, 20, 0, 0);
   }
   
  delay(100);
  Set_LED(1, 0, 0, 0);
      Set_LED(2, 0, 0, 0);
       delay(100);
 
  
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
  // find if server send me my params
  if (id_recive == _id){
    free(params_recive);
    return;
   }
  int index = -1;
  int i = 0;
   // find if there is a message from this sender
  while(i< buffSize and index == -1){
    messageBuffer elem = bufferToSave[i];
    if(elem.id == id_recive){
        index = i;
     }
     i +=1;
  }
  // find if ther is a free place in buffer
  if (index == -1){
    i = 0;
    while(i < buffSize and index == -1){
      messageBuffer elem = bufferToSave[i];
      if (elem.id == -1){
        index = i;
      }
    }
  }
  if(index != -1){
     bufferToSave[index].id = id_recive;
     bufferToSave[index].timeRecive = _iteration;
     bufferToSave[index].fitness =  fitness_recive;
     for (i = 0 ; i < paramSize ; i++){
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
    while(i < _max_size_message and message[i] != 'E'){ i+=1;}
    if (i == _max_size_message){return;}
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

void broadcastParams(){
  WiFi.begin(ssid);
  Motors_stop();
  while (WiFi.status() != WL_CONNECTED) {
    Set_LED(1, 0, 0, 0);
    Set_LED(2, 0, 0, 0);
    delay(10);
    Set_LED(1, 0, 0, 255);
    Set_LED(2, 0, 0, 255);
   }
  
  
  HTTPClient http;
  char message[_max_size_message];
  char http_string[] = "http://192.168.4.1/";

  write_message(message,http_string ,_id, _fitnessValue, _params , _params_size );
  
  http.begin(message);
  int httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString();
    char recive_message[_max_size_message * 2];
    int i = 0 ;
    while ( i < (_max_size_message*2)-1 and i < payload.length()){
      recive_message[i] = payload[i];
      i+=1;
    }
    recive_message[i] = '\0';
    read_and_save_message(recive_message,_buffer, _buffer_size,_params_size);
  }
  http.end(); 
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}
