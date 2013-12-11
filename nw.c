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
#include <string.h>
#include <stdint.h>

#define INPUT 784
#define HIDDEN 100
#define OUTPUT 10
/* #define SAMPLE 60000 */
#define SAMPLE 900
#define TEST 200
#define TIME 100

/* RESUM用 */
#define DONE_TIME 0
#define RMS_PRE 0

#define ETA 0.1
#define ALPHA 1
#define E_RMS_LIM 0.001

/* ニューラルネットワークの構造 */
double first_weight[HIDDEN][INPUT];
double second_weight[OUTPUT][HIDDEN];
double second_value[HIDDEN];
double second_input[HIDDEN];
double second_delta[HIDDEN];
double output_value[OUTPUT];
double output_input[OUTPUT];
double output_delta[OUTPUT];
/* 各種データ */
double train_data[INPUT];
double train_label[OUTPUT];
double test_data[INPUT];
int test_label;
double rms=RMS_PRE;
/* ファイル関係 */
FILE *fp_data, *fp_label, *fp_result;

/* 関数群 */
// 数学
double sigmoid(double input);
double sigmoid_delta(double input);
// ファイル入出力
int file_read(void);
int file_read_test(void);
void file_open(void);
void file_open_test(void);
void file_close(void);
void weight_write(void);
void weight_read(void);
// ネットワーク
void calc_train(void);
void calc_test(void);
void calc_forward(double input_data[INPUT]);
void calc_backward(void);
int calc_correct(void);
void nan_check(double input, char *str);

int main(int argv, char *argc[])
{
  int sample=0, time=0;
  int correct_ratio=0;
  double e_rms;
  
  if(argv==0){
    return 0;
  }
    
  weight_read();
  
  if(strcmp(argc[1], "train") == 0){
    for(time=0; time<TIME; time++){
      file_open();
      for(sample=0; sample<SAMPLE; sample++){
	printf("\n[sample: %d]\n", sample);
	file_read();
	calc_train();
      }
      file_close();
      
      e_rms = sqrt(rms)/(double)((time+DONE_TIME+1)*OUTPUT);
      printf("E_rms[time %d] : %lf, %lf\n", time, e_rms, rms);
      fprintf(stderr, "E_rms[time %d] : %lf, %lf\n", time, e_rms, rms);
      weight_write();
      if(e_rms <= E_RMS_LIM){
	break;
      }
    }
    
  }else if(strcmp(argc[1], "test") == 0){
    file_open_test();
    for(sample=0; sample<TEST; sample++){
      printf("\n[sample: %d]\n", sample);
      file_read_test();
      calc_test();
      correct_ratio += calc_correct();
    }
    printf("Correct Ratio is : %lf\n", (double)correct_ratio/(double)TEST);
    file_close();
  }
   
  return 0;
}


double sigmoid(double input){
  return 1.0/ (1.0 + exp(-ALPHA * input));
}

double sigmoid_delta(double input){
  return ALPHA * input * (1.0 - input);
}


void file_open(void){
  int size;
  int num;
  int32_t buf;

  if ((fp_data = fopen("train-images.idx3-ubyte", "rb")) == NULL) {
    printf("file open error!!\n");
    exit(EXIT_FAILURE);
  }
  
  if ((fp_label = fopen("train-labels.idx1-ubyte", "rb")) == NULL) {
    printf("file open error!!\n");
    exit(EXIT_FAILURE);
  }
  
  for(num=0; num<4; num++){
    if(num<2){
      size = fread(&buf, sizeof(int32_t), 1, fp_label);
      printf( "label : %x  ", buf);	    
    }
    size = fread(&buf, sizeof(int32_t), 1, fp_data);    
    printf( "data : %x", buf);	
    printf("\n%d\n", sizeof(int));    
  }
}

void file_open_test(void){
  int size;
  int num;
  int32_t buf;

  if ((fp_data = fopen("t10k-images.idx3-ubyte", "rb")) == NULL) {
    printf("file open error!!\n");
    exit(EXIT_FAILURE);
  }
  
  if ((fp_label = fopen("t10k-labels.idx1-ubyte", "rb")) == NULL) {
    printf("file open error!!\n");
    exit(EXIT_FAILURE);
  }
  
  for(num=0; num<4; num++){
    if(num<2){
      size = fread(&buf, sizeof(int32_t), 1, fp_label);
      printf( "label : %x  ", buf); /* 初期読み取り */
    }
    size = fread(&buf, sizeof(int32_t), 1, fp_data);    
    printf( "data : %x", buf);	
    printf("\n%d\n", sizeof(int));    
  }
}

int file_read(void){
  int size, i;
  int32_t buf;
  unsigned char image[784];
  unsigned char label;
  int num;

  for(num=0; num<1; num++){
    size = fread(&label, sizeof(unsigned char), 1, fp_label);
    for(i=0; i<OUTPUT; i++){
      if(i==label) train_label[i] = 1;
      else train_label[i] = 0;
      printf ("%d\n",(int)train_label[i]);
    }

    fread(&image, sizeof(unsigned char), 784, fp_data);
    for(i=0; i<784; i++){
      train_data[i] = (double)image[i]/255.0;
      printf ("%d ", image[i]);
      if(i%28==27)
	printf("\n");     
    }

    //    printf("\n%d\n", num);
  }
  
  return 1;
}

int file_read_test(void){
  int size, i;
  int32_t buf;
  unsigned char image[784];
  unsigned char label;
  int num;

  for(num=0; num<1; num++){
    size = fread(&label, sizeof(unsigned char), 1, fp_label);
    test_label=label;
    printf("label %d\n", test_label);

    fread(&image, sizeof(unsigned char), 784, fp_data);
    for(i=0; i<784; i++){
      test_data[i] = (double)image[i]/255.0;
      printf ("%d ", image[i]);
      if(i%28==27)
	printf("\n");     
    }

    //    printf("\n%d\n", num);
  }
  
  return 1;
}


void file_close(void){
  fclose(fp_data);
  fclose(fp_label);
}

void weight_read(){
  int i, j;
  
  printf("File Read\n");

  if ((fp_result = fopen("train-result1.csv", "r")) == NULL) {
    printf("file open error!!\n");
    exit(EXIT_FAILURE);
  }

  for(i=0; i<HIDDEN; i++){
    for(j=0; j<INPUT-1; j++){
      fscanf(fp_result, "%lf ", &first_weight[i][j]);
      //printf("%lf ", first_weight[i][j]);
    }
    fscanf(fp_result, "%lf\n", &first_weight[i][INPUT-1]);

  }
  fclose(fp_result);
  
  if ((fp_result = fopen("train-result2.csv", "r")) == NULL) {
    printf("file open error!!\n");
    exit(EXIT_FAILURE);
  }
  for(i=0; i<OUTPUT; i++){
    for(j=0; j<HIDDEN-1; j++){
      fscanf(fp_result, "%lf ", &second_weight[i][j]);
    }
    fscanf(fp_result, "%lf\n", &second_weight[i][HIDDEN-1]);
  }
  fclose(fp_result);

}

void weight_write(){
  int i, j;
  if ((fp_result = fopen("train-result1.csv", "w")) == NULL) {
    printf("file open error!!\n");
    exit(EXIT_FAILURE);
  }
  
  for(i=0; i<HIDDEN; i++){
    for(j=0; j<INPUT-1; j++){
      fprintf(fp_result, "%lf ", first_weight[i][j]);
    }
    fprintf(fp_result, "%lf\n", first_weight[i][INPUT-1]);
  }
  fclose(fp_result);
  
  if ((fp_result = fopen("train-result2.csv", "w")) == NULL) {
    printf("file open error!!\n");
    exit(EXIT_FAILURE);
  }

  for(i=0; i<OUTPUT; i++){
    for(j=0; j<HIDDEN-1; j++){
      fprintf(fp_result, "%lf ", second_weight[i][j]);
    }
    fprintf(fp_result, "%lf\n", second_weight[i][HIDDEN-1]);
  }
  fclose(fp_result);
}



void calc_train(void){
  printf("[train]\n");
  calc_forward(train_data);
  calc_backward();
}

void calc_test(void){
  calc_forward(test_data);
}

void calc_forward(double input_data[INPUT]){
  int i,j;
  char str[256];

  printf("[calc_forward]\n");
  printf("\tsecond layer\n");
  /* 第一層→第二層 */
  for(i=0; i<HIDDEN; i++){
    second_input[i] = 0;
    for(j=0; j<INPUT; j++){
      second_input[i] += first_weight[i][j] * input_data[j];
    }    
    sprintf(str, "second_input fail (%d, %d)", i, j);
    nan_check(second_input[i], str);

    second_value[i] = sigmoid(second_input[i]);
  }
  printf("%lf ", second_input[59]);
  printf("\n\toutput layer\n");
  /* 第二層→第三層 */
  for(i=0; i<OUTPUT; i++){
    output_input[i] = 0;
    for(j=0; j<HIDDEN; j++){
      output_input[i] += second_weight[i][j] * second_value[j];
      /* printf("%lf ", second_weight[i][j]); */
    }
    sprintf(str, "output_input fail (%d, %d)", i, j);
    nan_check(output_input[i], str);

      //    printf("%lf ", output_input[i]);
    output_value[i] = sigmoid(output_input[i]);
  }
}

/* 教育用にのみ用いる */
void calc_backward(void){
  int i,j;
  char str[256];
  
  printf("[calc_backward]\n");
  /* 第三層　誤差算出 */
  for(i=0; i<OUTPUT; i++){
    output_delta[i] = (output_value[i] - train_label[i]) * sigmoid_delta(output_value[i]);
    rms += pow((output_value[i] - train_label[i]), 2); /* RMS加算 */
  }
  
  /* 第二層　誤差算出 */
  printf("second_delta:\n");
  for(i=0; i<HIDDEN; i++){
    second_delta[i] = 0;
    for(j=0; j<OUTPUT; j++){
      second_delta[i] += second_weight[j][i] * output_delta[j];
    }
    second_delta[i] = second_delta[i] * sigmoid_delta(second_value[i]);
    sprintf(str, "second_delta fail (%d, %d)", i, j);
    nan_check(second_delta[i], str);
  }
  printf("second_input(8): %lf\n", second_input[59]);
  printf("second_delta_calc[59]: %lf\n", sigmoid_delta(second_input[59]));
  
  /* 重さ算出 */
  /* 第一層→第二層 */
  printf("first_weight:\n");
  for(i=0; i<HIDDEN; i++){
    for(j=0; j<INPUT; j++){
      first_weight[i][j] = first_weight[i][j] - ETA*train_data[j]*second_delta[i];
      //printf("%lf ", train_data[j]);
      sprintf(str, "first_weight_calc fail (%d, %d)", i, j);
      nan_check(ETA*train_data[j]*second_delta[i], str);

      sprintf(str, "first_weight fail (%d, %d)", i, j);
      nan_check(first_weight[i][j], str);
    }
  }
  printf("first_weight(8, 0): %lf\n", first_weight[59][0]);
  printf("train_data[0]: %lf\n", train_data[0]);
  printf("second_delta[59]: %lf\n", second_delta[59]);


  /* 第二層→第三層 */
   printf("second_weight:\n");
  for(i=0; i<OUTPUT; i++){
    for(j=0; j<HIDDEN; j++){
      second_weight[i][j] = second_weight[i][j] - ETA*second_value[j]*output_delta[i];
      //      printf("%lf ", second_value[j]);
      sprintf(str, "second_weight fail (%d, %d)", i, j);
      nan_check(second_weight[i][j], str);
    }
    //    printf("\n");
  }
}

int calc_correct(void){
  int i;
  double max=0; 
  int max_idx;

  /* outputノードの最大値indexをとる */
  for(i=0; i<OUTPUT; i++){
    printf("%lf ", output_value[i]);
    if(max < output_value[i]){
      max = output_value[i];
      max_idx = i;
    }
  }
  printf("\nOUT, max : %d %lf\n", max_idx, max);
  if(max_idx == test_label){
    return 1;
  }else{
    return 0;
  }
}

void nan_check(double input, char *str){
  if(isnan(input)){
    fprintf(stderr, "<!! NAN FAILURE> %s\n", str);
    exit(EXIT_FAILURE);
  }
}
