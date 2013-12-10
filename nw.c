/*
 * nw.c - 
 *
 * Copyright (C) 2013 by Ryo Soga <coffeegg89@gmail.com>
 *                     Time-stamp: <2013-12-08 14:09:51 mechuser>
 *                     License   : 
 *
 */

// #include "nw.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INPUT 100
#define HIDDEN 100
#define OUTPUT 10

#define ETA 0.01
#define ALPHA 1

/* ニューラルネットワークの構造 */
float first_weight[HIDDEN][INPUT];
float second_weight[OUTPUT][HIDDEN];
float second_value[HIDDEN];
float second_input[HIDDEN];
float second_delta[HIDDEN];
float output_value[OUTPUT];
float output_input[OUTPUT];
float output_delta[OUTPUT];
/* 各種データ */
float train_data[INPUT];
float train_label[OUTPUT];
float test_data[INPUT];
float test_label[OUTPUT];

/* 関数群 */
// 数学
float sigmoid(float input);
float sigmoid_delta(float input);
// ファイル入出力
int file_read(char *fname, float data[INPUT]);
// ネットワーク
void calc_train(void);
void calc_test(void);
void calc_forward(float input_data[INPUT]);
void calc_backward(void);


int main()
{
  /* if(file_read(, input_data)){ */
  /*   return -1; */
  /* } */
  
  calc_train();
  
  return 0;
}


float sigmoid(float input){
  return 1.0/ (1.0 + exp(-ALPHA * input));
}

float sigmoid_delta(float input){
  return ALPHA * input * (1.0 - input);
}

int file_read(char *fname, float data[INPUT]){
  /* if(!){ */
  /*   fprintf(stderr, "%s doees not exist.", fname); */
  /*   return 0; */
  /* } */
  /* input_data に格納する */
  
  return 1;
}

void calc_train(void){
  calc_forward(train_data);
  calc_backward();
}

void calc_test(void){
  calc_forward(test_data);
}

void calc_forward(float input_data[INPUT]){
  int i,j;
  
  /* 第一層→第二層 */
  for(i=0; i<HIDDEN; i++){
    second_input[i] = 0;
    for(j=0; j<INPUT; j++){
      second_input[i] += first_weight[i][j] * input_data[j];
    }
    second_value[i] = sigmoid(second_input[i]);
  }
  
  /* 第二層→第三層 */
  for(i=0; i<OUTPUT; i++){
    output_input[i] = 0;
    for(j=0; j<HIDDEN; j++){
      output_input[i] += second_weight[i][j] * second_value[j];
    }
    output_value[i] = sigmoid(output_input[i]);
  }
}

/* 教育用にのみ用いる */
void calc_backward(void){
  int i,j;

  /* 第三層　誤差算出 */
  for(i=0; i<OUTPUT; i++){
    output_delta[i] = (output_value[i] - train_label[i]) * sigmoid_delta(output_input[i]);
  }
  
  /* 第二層　誤差算出 */
  for(i=0; i<HIDDEN; i++){
    second_delta[i] = 0;
    for(j=0; j<OUTPUT; j++){
      second_delta[i] += second_weight[j][i] * output_delta[j];
    }
    second_delta[i] = second_delta[i] * sigmoid_delta(second_input[i]);
  }

  
  /* 重さ算出 */
  /* 第一層→第二層 */
  for(i=0; i<HIDDEN; i++){
    for(j=0; j<INPUT; j++){
      first_weight[i][j] = first_weight[i][j] - ETA*train_data[i]*second_delta[i];
    }
  }

  /* 第二層→第三層 */
  for(i=0; i<OUTPUT; i++){
    for(j=0; j<HIDDEN; j++){
      second_weight[i][j] = second_weight[i][j] - ETA*second_value[i]*output_delta[i];
    }
  }
}
