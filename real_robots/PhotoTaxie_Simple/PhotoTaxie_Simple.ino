 #include <Wire.h>
#include "Mona_ESP_lib.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
 


bool IR_values[5] = {false, false, false, false, false};

// 0 -move forward , 1 - forward obstacle , 2 - right proximity , 3 - left proximity
int obstacle_threshold, state, delay_time,persistent_delay, default_persistent_delay,default_speed,light_indice, light_Window_size, max_light = 4095 , low_speed;
float * light_Window;
float mu,sigma,r, r_left_right, diffusion_rate, light_value_t;

void setup()
{
  
  Mona_ESP_init();
  
  state = 0;
  r = 0;
  
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
  
  // params for a smooth  light value
  light_indice = 0;
  light_Window_size = (int) delay_time / 10;
  light_Window = (float *)malloc(sizeof(float) * light_Window_size);
  for (int i = 0 ; i < light_Window_size ; i++){
    light_Window[i] = 0;
  }
  
  srand(time(NULL));
  delay(15000);  
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
  float light_mean = 0.0;
  for (int i = 0 ; i < light_Window_size ; i++){
    light_mean += light_Window[i];
  }
  
  return light_mean / light_Window_size;
}


void fronter(){
 

  if (state == 0 ) { // forward with diffusion, not obstacle 
    Set_LED(1, 20, 0, 0);
    Set_LED(2, 20, 0, 0);
    r = ((float)rand () / RAND_MAX)  ;
    if (r < diffusion_rate){
      r = -1 + ((float) rand () / RAND_MAX) * 2;     
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
  if (state == 1) { // forward slowly with diffusion, obstacle in front
    Set_LED(1, 20, 0, 0);
    Set_LED(2, 20, 0, 0);
    r = ((float)rand () / RAND_MAX)  ;
    if (r < diffusion_rate){
      r = -1 + ((float) rand () / RAND_MAX) * 2;     
      if (r <=0){
        Motors_spin_left(low_speed);
      }
      else{
        Motors_spin_right(low_speed);
      }
    }
    else{
      Motors_forward(low_speed);
    }  
  
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

  
   if (state == 0) { // forward with diffusion, not obstacle or obstacle in left and right
    Set_LED(1, 0, 20, 0);
    Set_LED(2, 0, 20, 0);
    r = ((float)rand () / RAND_MAX);
    if (r< diffusion_rate){
      r = -1 + ((float) rand () / RAND_MAX) * 2;     
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
      r_left_right =  -1 + ((float) rand () / RAND_MAX) * 2; 
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
    persistent_delay = 0;
    state = 0;
  }
  if (state != 1){
    persistent_delay = 0;
  }
  
}

void loop(){

  set_state();
  light_value_t = get_light_value();
  if ( light_value_t > 0.8){

      fronter();
      
     }
     else{
      aligner();
     }
  
  delay(delay_time);
}
