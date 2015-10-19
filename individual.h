#pragma once
#include <iostream>
#include <cmath>
#include <climits>
#include <cfloat>
#include <memory.h>
#include <vector>

#define MAX_EMPLOYEE_PER_CREW 2
namespace evo {
class Fare {
  public:
    int journeyId;
    int plannedElementIdx;
    int crewIdx;
    int timestamp;
    int typeVehicle;
    int price;
    int realTimestamp;
    int employeeIdx[MAX_EMPLOYEE_PER_CREW];
    char crewSize;
};

class Individual {
  public:
    std::vector<int> timeWorked;
    std::vector<float> f;
    std::vector<Fare> listFare;
    std::vector<int> vehiclesCount;
    float fitness;
};

}
