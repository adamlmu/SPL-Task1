//
// Created by spl211 on 10/11/2021.
//

#include "../include/Workout.h"

int Workout::getPrice() const {
    return price;
}

int Workout::getId() const {
    return id;
}

std::string Workout::getName() const {
    return name;
}

WorkoutType Workout::getType() const {
    return type;
}

Workout::Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type):
        id(w_id), name(w_name), price(w_price), type(w_type) {}

Workout::Workout(const Workout& other): id(other.id), name(other.name), price(other.price), type(other.type) {}

Workout &Workout::operator=(const Workout &other) {
    return *this;
}


