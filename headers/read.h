#ifndef CONFIGURATION_FILE_H
#define CONFIGURATION_FILE_H

#define MAX_CARS 100

typedef struct {
    int id;
    int position;
    int side;
    int speed;
    int priority;
    int burstTime;
    int type;
} CarData;

typedef struct {
    int method;
    int streetLength;
    int signChangeTime;
    int parameterW;
    int scheduler;         
    int carCount;
    CarData cars[MAX_CARS];
} RoadData;

void readConfigurationFile(const char *fileName, RoadData *data);

#endif // CONFIGURATION_FILE_H
