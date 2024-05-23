#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"
#include <sstream>

enum ActionStatus{
    COMPLETED, ERROR
};

//Forward declaration
class Studio;

class BaseAction{
public:
    BaseAction();
    //BaseAction(const BaseAction& baseAction);           //Copy constructor
    ActionStatus getStatus() const;
    virtual void act(Studio& studio)=0;
    virtual std::string toString() const=0;
    virtual ~BaseAction()=default;
    virtual BaseAction* clone() const=0;
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTrainer : public BaseAction {
public:
    OpenTrainer(int id, std::vector<Customer *> &customersList);
    OpenTrainer(const OpenTrainer& openTrainer);
    void act(Studio &studio);
    std::string toString() const;
    virtual ~OpenTrainer();
    virtual BaseAction* clone() const;
private:
	const int trainerId;
	std::vector<Customer*> customers;
    std::string input;
};


class Order : public BaseAction {
public:
    Order(int id);
    //Order(const Order& order);
    void act(Studio &studio);
    std::string toString() const;
    virtual BaseAction* clone() const;
private:
    const int trainerId;
    std::string successMsg;         //if we'll need this for more actions - then we'll move this to the abstract class

};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    //MoveCustomer(const MoveCustomer& MoveCustomers);
    void act(Studio &studio);
    std::string toString() const;
    virtual BaseAction* clone() const;
private:
    const int srcTrainer;
    const int dstTrainer;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    //Close(const Close& close);        //Copy constructor
    void act(Studio &studio);
    std::string toString() const;
    virtual BaseAction* clone() const;
private:
    const int trainerId;
};

class CloseAll : public BaseAction {
public:
    CloseAll();
    //CloseAll(const CloseAll& closeAll);        //Copy constructor
    void act(Studio &studio);
    std::string toString() const;
    virtual BaseAction* clone() const;
private:
};


class PrintWorkoutOptions : public BaseAction {
public:
    PrintWorkoutOptions();
    //PrintWorkoutOptions(const PrintWorkoutOptions& printWorkoutOptions);        //Copy constructor
    void act(Studio &studio);
    std::string toString() const;
    virtual BaseAction* clone() const;
private:
};


class PrintTrainerStatus : public BaseAction {
public:
    PrintTrainerStatus(int id);
    //PrintTrainerStatus(const PrintTrainerStatus& printStatus);        //Copy constructor
    void act(Studio &studio);
    std::string toString() const;
    virtual BaseAction* clone() const;
private:
    const int trainerId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    //PrintActionsLog(const PrintActionsLog& printLog);        //Copy constructor
    void act(Studio &studio);
    std::string toString() const;
    virtual BaseAction* clone() const;
private:
};


class BackupStudio : public BaseAction {
public:
    BackupStudio();
    //BackupStudio(const BackupStudio& backup);        //Copy constructor
    void act(Studio &studio);
    std::string toString() const;
    virtual BaseAction* clone() const;
private:
};


class RestoreStudio : public BaseAction {
public:
    RestoreStudio();
    //RestoreStudio(const RestoreStudio& restore);        //Copy constructor
    void act(Studio &studio);
    std::string toString() const;
    virtual BaseAction* clone() const;


};


#endif