#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/read.h"

void readConfigurationFile(const char *fileName, RoadData *data) {
    FILE *file = fopen(fileName, "r");
    if (!file) {
        perror("Error opening the file");
        return;
    }

    // Initialize the structure
    memset(data, 0, sizeof(RoadData));

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "Method:", 7) == 0) {
            sscanf(line, "Method: %d", &data->method);
        } else if (strncmp(line, "StreetLength:", 13) == 0) {
            sscanf(line, "StreetLength: %d", &data->streetLength);
        } else if (strncmp(line, "SignChangeTime:", 15) == 0) {
            sscanf(line, "SignChangeTime: %d", &data->signChangeTime);
        } else if (strncmp(line, "ParameterW:", 11) == 0) {
            sscanf(line, "ParameterW: %d", &data->parameterW);
        } else if (strncmp(line, "Scheduler:", 10) == 0) {
            sscanf(line, "Scheduler: %d", &data->scheduler);  // ← nuevo campo
        } else if (strncmp(line, "Car", 3) == 0) {
            CarData car;
            sscanf(line, "Car%*d: id=%d, position=%d, side=%d, speed=%d, priority=%d, burstTime=%d, type=%d",
                   &car.id, &car.position, &car.side, &car.speed, &car.priority, &car.burstTime, &car.type);
            if (data->carCount < MAX_CARS) {
                data->cars[data->carCount++] = car;
            } else {
                fprintf(stderr, "Maximum number of cars (%d) reached. Car ignored.\n", MAX_CARS);
            }
        }
    }
    

    fclose(file);
}
