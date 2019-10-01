#ifndef SENSOR_STATE_H_
#define SENSOR_STATE_H_

struct state_var;

void *sesnorStateThread(void *arg0);
int state_machine(struct state_var* stat, int* timeInc, int* sensorVal);

#endif
