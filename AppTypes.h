#ifndef APP_TYPES_H
#define APP_TYPES_H

#include <Arduino.h>

#define MAX_PROFILES 10
#define MAX_PROFILE_NAME 32
#define TOTAL_STEPS 64

typedef struct {
  int speed;
  int time;
  int acc;
  bool valid;
  int dispenser;
} step_t;

typedef struct {
  char name[MAX_PROFILE_NAME];
  step_t steps[TOTAL_STEPS];
  bool used;
} profile_t;

#endif
