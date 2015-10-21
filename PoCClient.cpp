#include "individual.h"
#include "islandSender.h"

#include <iostream>
#include <cstdlib>
#include <cstdio>

Individual createTestIndividual(const int nbFares) {
    Fare tmp;

    tmp.journeyId = 1;
    tmp.plannedElementIdx = 2;
    tmp.crewIdx = 3;
    tmp.timestamp = 4;
    tmp.typeVehicle = 5;
    tmp.price = 6;
    tmp.realTimestamp = 7;
    tmp.employeeIdx[0] = 8;
    tmp.employeeIdx[1] = 9;
    tmp.crewSize = 10;

    Individual tmpIndiv;

    for (int i = 0; i < nbFares; i++) {
        tmpIndiv.listFare.push_back(tmp);
    }

    return tmpIndiv;
}

int main(int argc, const char *argv[]) {
    const int nbFares = 1000;
    const int port = 1337;

    const auto indiv = createTestIndividual(nbFares);
    island::sender::migrate(indiv, "localhost", port);
    return 0;
}
