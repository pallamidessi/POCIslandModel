#pragma once
#include <iostream>
#include <cmath>
#include <climits>
#include <cfloat>
#include <memory.h>
#include <vector>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/vector.hpp>

#define MAX_EMPLOYEE_PER_CREW 2
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
    Fare() {};
    virtual ~Fare() {};
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar
        & journeyId
        & plannedElementIdx
        & crewIdx
        & timestamp
        & typeVehicle
        & price
        & realTimestamp
        & employeeIdx
        & crewSize;

    }
};

BOOST_CLASS_VERSION(Fare, 1)

class Individual {
  public:
    std::vector<int> timeWorked;
    std::vector<float> f;
    std::vector<Fare> listFare;
    std::vector<int> vehiclesCount;
    float fitness;
    Individual() {};
    virtual ~Individual() {};

  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar
        & timeWorked
        & f
        & listFare
        & vehiclesCount;
    }
};

