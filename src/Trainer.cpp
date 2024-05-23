//
// Created by spl211 on 10/11/2021.
//
#include <iostream>
#include "../include/Trainer.h"


Trainer::Trainer(int t_capacity): open(false),salary(0),capacity(t_capacity),customersList(),orderList() {}

//Copy constructor
Trainer::Trainer(const Trainer& other): open(),salary(),capacity(),customersList(),orderList() {
    capacity=other.capacity;
    open=other.open;
    salary = other.salary;
    for (size_t i = 0; i < other.customersList.size(); i++) {
        customersList.push_back(other.customersList[i]->clone());
    }
    for (size_t i = 0; i < other.orderList.size(); i++) {
        this->orderList.push_back(other.orderList[i]);
    }
}

//Move constructor
Trainer::Trainer(Trainer&& other): open(),salary(),capacity(),customersList(),orderList() {
    open=other.open;
    for (size_t i = 0; i < other.customersList.size(); i++) {
        customersList.push_back(other.customersList[i]);
        other.customersList[i] = nullptr;
    }
    orderList = other.orderList;
    salary = other.salary;
    capacity = other.capacity;
    //for (int i = 0; i < orderList.size(); i++) {
     //   orderList[i].first = other.orderList[i].first;
     //   orderList[i].second = other.orderList[i].second;                 //Don't need this shit
     //   other.orderList[i].second = nullptr;
    //}
}

//Copy-Assignment
Trainer& Trainer::operator=(const Trainer &other) {
    if (this!=&other) {
        clear();
        for (size_t i=0;i<other.customersList.size();i++) {
            this->customersList.push_back(other.customersList[i]->clone());
        }
        for (size_t i=0;i<other.orderList.size();i++) {
            this->orderList.push_back(other.orderList[i]);
        }
        capacity=other.capacity;
        open=other.open;
        salary = other.salary;
    }
    return *this;
}

void Trainer::clear() {
    customersList.clear();
    orderList.clear();
}

//Move-Assignment
Trainer &Trainer::operator=(Trainer &&other) {
    if (this!=&other) {
        clear();
        salary = other.salary;
        capacity = other.capacity;
        open = other.open;
        for (size_t i = 0; i < other.customersList.size(); i++) {
            customersList.push_back(other.customersList[i]);
            other.customersList[i] = nullptr;
        }
        orderList = other.orderList;
    }
    return *this;
}

//Destructor
Trainer::~Trainer() {
    if (!isOpen()){
        return;
    }
    else {
        orderList.clear();
        for (Customer* c: customersList){
            if (c) {
                delete c;
                c = nullptr;
            }
        }
    }
}

int Trainer::getCapacity() const {
    return capacity;
}

void Trainer::addCustomer(Customer *customer) {
    customersList.push_back(customer);
}

void Trainer::removeCustomer(int id) {
    for (size_t i = 0; i < customersList.size(); i++) {
        if (customersList[i]->getId() == id)
            customersList.erase(customersList.begin() + i);
    }
}

Customer *Trainer::getCustomer(int id) {
    for (size_t i=0; i<customersList.size();i++) {
        if (customersList[i]->getId() == id)
            return customersList[i];
    }
    return nullptr;
}

std::vector<Customer *> &Trainer::getCustomers() {
    return customersList;
}

void Trainer::openTrainer() {
    open = true;
}

void Trainer::closeTrainer() {
    for (Customer* c: customersList){
        if (c){
            delete c;
            c = nullptr;
        }
    }
    clear();
    open = false;
}

bool Trainer::isOpen() {
    return open;
}

int Trainer::getSalary() {
    return salary;
}

void Trainer::calculateSalary() {
    for (size_t i=0 ; i<orderList.size() ; i++) {
        salary += (orderList[i].second).getPrice();
    }
}

void Trainer::AddSalary(int s) {
        salary += s;
}

void Trainer::SubSalary(int s) {
    salary -= s;
}

void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout> &workout_options) {
    for (int x : workout_ids) {
        for (size_t i=0 ; i < workout_options.size() ; i++){
            if (workout_options[i].getId() == x){
                orderList.push_back(std::make_pair(customer_id,workout_options[x]));
            }
        }
    }
}

std::vector<OrderPair> &Trainer::getOrders() {
    return orderList;
}












