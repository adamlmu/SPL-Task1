#ifndef SPL_HW_TRAINER_H_
#define SPL_HW_TRAINER_H_

#include <vector>
#include "Customer.h"
#include "Workout.h"

typedef std::pair<int, Workout> OrderPair;

class Trainer{
public:
    Trainer(int t_capacity);
    Trainer(const Trainer& trainer);             //Copy constructor
    Trainer(Trainer &&other);                    //move constructor
    Trainer& operator=(const Trainer &other);    //Copy assignment operator
    Trainer& operator=(Trainer &&other);    //Move assignment operator
    ~Trainer();
    void clear();
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary();
    bool isOpen();
    void calculateSalary();
    void AddSalary(int s);
    void SubSalary(int s);
private:
    bool open;
    int salary;
    int capacity;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
};


#endif