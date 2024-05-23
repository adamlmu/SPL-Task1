//
// Created by spl211 on 10/11/2021.
//

#include "Customer.h"
#include <algorithm>
#include <iostream>
using namespace std;

//Constructor:
Customer::Customer(std::string c_name, int c_id): name(c_name),id(c_id) {}

//Copy constructor:
Customer::Customer(const Customer& other): name(other.name),id(other.id) {}

SweatyCustomer::SweatyCustomer(std::string name, int id) : Customer(name, id) {}
SweatyCustomer::SweatyCustomer(const SweatyCustomer& other) : Customer(other) {}

CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id) {}
CheapCustomer::CheapCustomer(const CheapCustomer& other) : Customer(other) {}

HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id) : Customer(name, id) {}
HeavyMuscleCustomer::HeavyMuscleCustomer(const HeavyMuscleCustomer& other) : Customer(other) {}

FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(name, id) {}
FullBodyCustomer::FullBodyCustomer(const FullBodyCustomer& other) : Customer(other) {}

SweatyCustomer *SweatyCustomer::clone() const {
    return new SweatyCustomer(*this);
}

CheapCustomer *CheapCustomer::clone() const {
    return new CheapCustomer(*this);
}

HeavyMuscleCustomer *HeavyMuscleCustomer::clone() const {
    return new HeavyMuscleCustomer(*this);
}

FullBodyCustomer *FullBodyCustomer::clone() const {
    return new FullBodyCustomer(*this);
}

std::string Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}

std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> x;
    for (size_t i = 0;i < workout_options.size();i++) {
        if (workout_options[i].getType() == CARDIO) {
            x.push_back(workout_options[i].getId()); }
    }
    return x;
}

std::string SweatyCustomer::toString() const {
    return "swt";
}

SweatyCustomer::~SweatyCustomer()=default;

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> x;

    if (workout_options.empty())
        return x;
    //x.push_back(workout_options[0].getId());
    x.push_back(GetMinimalPriceWorkoutId(workout_options));
    return x;
}

int CheapCustomer::GetMinimalPriceWorkoutId(const std::vector<Workout> &workout_options) {
    int minimalPriceId = workout_options[0].getId();
    int minimalPrice = workout_options[0].getPrice();
    for (size_t i=1;i<workout_options.size();i++) {
        if (workout_options[i].getPrice() < minimalPrice) {
            minimalPriceId = workout_options[i].getId();
            minimalPrice = workout_options[i].getPrice();
        }
    }
    return minimalPriceId;
}

std::string CheapCustomer::toString() const {
    return "chp";
}

CheapCustomer::~CheapCustomer() = default;

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> anaerobicList;
    for (size_t i = 0;i<workout_options.size();i++) {
        if (workout_options[i].getType() == ANAEROBIC)
            anaerobicList.push_back(workout_options[i].getId());
    }
    sort(anaerobicList.begin(), anaerobicList.end(),
            [&](const int & a, int & b) -> bool
    {
        return (workout_options[a].getPrice() > workout_options[b].getPrice());
    });
    return anaerobicList;
}

std::string HeavyMuscleCustomer::toString() const {
    return "mcl";
}

HeavyMuscleCustomer::~HeavyMuscleCustomer() = default;

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> x;
    std::vector<Workout> cardio;
    std::vector<Workout> anaerobic;
    std::vector<Workout> mixed;
    for (size_t i = 0; i < workout_options.size(); i++) {
        if (workout_options[i].getType() == CARDIO)
            cardio.push_back(workout_options[i]);
        else if (workout_options[i].getType() == ANAEROBIC)
            anaerobic.push_back(workout_options[i]);
        else
            mixed.push_back(workout_options[i]);
    }
    if (cardio.empty()||anaerobic.empty()||mixed.empty())
        return x;

    x.push_back(GetMinimalPriceWorkoutId(cardio));
    x.push_back(GetMaximalPriceWorkoutId(mixed));
    x.push_back(GetMinimalPriceWorkoutId(anaerobic));
    return x;
}

int FullBodyCustomer::GetMinimalPriceWorkoutId(const std::vector<Workout> &workout_options) {
    int minimalPriceId = workout_options[0].getId();
    int minimalPrice = workout_options[0].getPrice();
    for (size_t i=1;i<workout_options.size();i++) {
        if (workout_options[i].getPrice() < minimalPrice) {
            minimalPriceId = workout_options[i].getId();
            minimalPrice = workout_options[i].getPrice();
        }
    }
    return minimalPriceId;
}

int FullBodyCustomer::GetMaximalPriceWorkoutId(const std::vector<Workout> &workout_options) {
    int maximalPriceId = workout_options[0].getId();
    int maximalPrice = workout_options[0].getPrice();
    for (size_t i=1;i<workout_options.size();i++) {
        if (workout_options[i].getPrice() > maximalPrice) {
            maximalPriceId = workout_options[i].getId();
            maximalPrice = workout_options[i].getPrice();
        }
    }
    return maximalPriceId;
}

std::string FullBodyCustomer::toString() const {
    return "fbd";
}

FullBodyCustomer::~FullBodyCustomer() = default;
