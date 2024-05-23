#ifndef SPL_HW_CUSTOMER_H
#define SPL_HW_CUSTOMER_H

#include <vector>
#include <string>
#include "Workout.h"

class Customer{
public:
    Customer(std::string c_name, int c_id);
    Customer(const Customer& customer);
    virtual std::vector<int> order(const std::vector<Workout> &workout_options)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
    virtual Customer* clone() const=0;
    virtual ~Customer()=default;
private:
    const std::string name;
    const int id;
//protected:
 //   std::string type;
};

class SweatyCustomer : public Customer {
public:
    SweatyCustomer(std::string name, int id);
    SweatyCustomer(const SweatyCustomer& customer);                                                //added
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    virtual SweatyCustomer* clone() const;
    virtual ~SweatyCustomer();

private:
};

class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    CheapCustomer(const CheapCustomer& customer);                                                   //added
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    virtual CheapCustomer* clone() const;
    int GetMinimalPriceWorkoutId(const std::vector<Workout> &workoutOptions);
    virtual ~CheapCustomer();
private:
};


class HeavyMuscleCustomer : public Customer {
public:
    HeavyMuscleCustomer(std::string name, int id);
    HeavyMuscleCustomer(const HeavyMuscleCustomer& customer);                                        //added
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    virtual HeavyMuscleCustomer* clone() const;
    virtual ~HeavyMuscleCustomer();
private:
};


class FullBodyCustomer : public Customer {
public:
    FullBodyCustomer(std::string name, int id);
    FullBodyCustomer(const FullBodyCustomer& customer);                                              //added
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    virtual FullBodyCustomer* clone() const;
    int GetMinimalPriceWorkoutId(const std::vector<Workout> &workoutOptions);
    int GetMaximalPriceWorkoutId(const std::vector<Workout> &workoutOptions);
    virtual ~FullBodyCustomer();

private:
};


#endif