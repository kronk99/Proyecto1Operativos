#ifndef FLOW_H
#define FLOW_H

#include "cars.h"

void equity(struct CarList* left, struct CarList* right, int w);
void sing(struct CarList* left, struct CarList* right, int interval);
void fifo(struct CarList* left, struct CarList* right);

#endif
