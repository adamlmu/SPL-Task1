#include "../include/Action.h"
#include "../include/Trainer.h"
#include "../include/Studio.h"
#include <sstream>
#include <utility>

extern Studio* backup;

BaseAction::BaseAction(): errorMsg(),status() {}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = COMPLETED;
}

void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    this->errorMsg = std::move(errorMsg);                                       //WTF is move?
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList): BaseAction(), trainerId(id), customers(customersList), input() {
    input = "open ";
    input += std::to_string(trainerId)+" ";
    for (size_t i=0 ; i < customers.size() ; i++){
        input+= customers[i]->getName()+","+customers[i]->toString()+" ";
    }
}

void OpenTrainer::act(Studio &studio) {
    if (trainerId >= studio.getNumOfTrainers()) {
        this->error("Workout session does not exist or is already open\n");
        std::cout << getErrorMsg();
        return;
    }
    Trainer* t = studio.getTrainer(trainerId);
    if (t->isOpen()){
        this->error("Workout session does not exist or is already open\n");
        std::cout << getErrorMsg();
        return;
    }
    if (t->getCapacity()<(int)(customers.size())) {
        for (int i=0 ; i < t->getCapacity() ; i++){
            t->addCustomer(customers[i]->clone());
        }
    }
    else {
        for (size_t i=0 ; i < customers.size() ; i++){
            t->addCustomer(customers[i]->clone());
        }
    }
    t->openTrainer();
    complete();
}

std::string OpenTrainer::toString() const {
    std::string str = input;
    if (this->getStatus() == ERROR){
        str += "Error: "+this->getErrorMsg();
    }
    else {
        str += "Completed\n";
    }
    return str;
}

OpenTrainer::~OpenTrainer() {
    for (Customer* c: customers){
        if (c){
            delete c;
            c = nullptr;
        }
    }
}

//ORDER:
Order::Order(int id): BaseAction(), trainerId(id), successMsg() {}

void Order::act(Studio &studio) {
    if (trainerId >= studio.getNumOfTrainers()) {       //FISHY
        this->error("Trainer does not exist or is not open\n");
        std::cout << getErrorMsg();
        return;
    }
    Trainer* t = studio.getTrainer(trainerId);
    if (!t->isOpen()){
        this->error("Trainer does not exist or is not open\n");
        std::cout << getErrorMsg();
        return;
    }
    std::vector<Workout>* wo = &studio.getWorkoutOptions();
    std::vector<Customer*> customers = t->getCustomers();

    for (size_t i=0; i<customers.size(); i++){
        std::vector<int> tempOrderList = customers[i]->order(*wo);
        if (tempOrderList.empty()) {
            t->removeCustomer(customers[i]->getId());
        }
        else {
            t->order(customers[i]->getId(), tempOrderList, *wo);
        }
    }
    for (size_t i=0; i<t->getOrders().size() ;i++) {
        OrderPair pair = (t->getOrders())[i];
        Customer* tempCustomer = t->getCustomer(pair.first);
        Workout* tempWorkout = &(pair.second);
        successMsg+= tempCustomer->getName()+" Is Doing "+tempWorkout->getName()+"\n";
    }
    t->calculateSalary();
    std::cout << successMsg;                             //Print to the user
    complete();
}

std::string Order::toString() const {
    std::string str = "order "+ std::to_string(trainerId);
    if (this->getStatus() == ERROR){
        str += " Error: "+this->getErrorMsg();
    }
    else {
        str += " Completed\n";
    }
    return str;
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId): BaseAction(), srcTrainer(src), dstTrainer(dst), id(customerId) {}

void MoveCustomer::act(Studio &studio) {
    if ( srcTrainer >= studio.getNumOfTrainers() || dstTrainer >= studio.getNumOfTrainers()) {       //FISHY
        this->error("Cannot move customer\n");
        std::cout << getErrorMsg();
        return;
    }
    Trainer* og = studio.getTrainer(srcTrainer);
    Trainer* dst = studio.getTrainer(dstTrainer);
    if (!og->isOpen() || !dst->isOpen() || og->getCustomer(id)== nullptr || (dst->getCapacity()) <= (int)(dst->getCustomers().size())){
        this->error("Cannot move customer\n");
        std::cout << getErrorMsg();
        return;
    }
    int def=0;
    //Adding the customer's pairs to the destination trainer's OrderList:
    for (OrderPair tempPair: og->getOrders()) {
        if (id==tempPair.first) {
            def+=(tempPair.second).getPrice();
            dst->getOrders().push_back(tempPair);
        }
    }

    //Updating the source trainer OrderList:
    vector<OrderPair> tempSrcPairs;
    for (OrderPair o:og->getOrders()){
        if (id != o.first){
            tempSrcPairs.push_back(o);
        }
    }
    og->getOrders().clear();
    og->getOrders() = tempSrcPairs;
    og->SubSalary(def);
    dst->AddSalary(def);
    Customer* customer = og->getCustomer(id);
    dst->addCustomer(customer);
    og->removeCustomer(id);
    if (og->getCustomers().empty()){
        og->closeTrainer();
    }
    complete();
}

std::string MoveCustomer::toString() const {
    std::string str =  "move "+std::to_string(srcTrainer)+" "+std::to_string(dstTrainer)+" "+std::to_string(id);
    if (this->getStatus() == ERROR){
        str += " Error: "+this->getErrorMsg();
    }
    else {
        str += " Completed\n";
    }
    return str;
}

Close::Close(int id): BaseAction(), trainerId(id){}

void Close::act(Studio &studio) {
    if (trainerId >= studio.getNumOfTrainers()) {       //FISHY
        this->error("Trainer does not exist or is not open\n");
        std::cout << getErrorMsg();
        return;
    }
    Trainer* t = studio.getTrainer(trainerId);
    if (!t->isOpen()){
        this->error("Trainer does not exist or is not open\n");
        std::cout << getErrorMsg();
        return;
    }
    string salary = to_string(t->getSalary());
    t->closeTrainer();
    string id = to_string(trainerId);
    std::cout << "Trainer "+id+" closed. Salary " +salary+ "NIS\n";
    complete();
}

std::string Close::toString() const {
    std::string str =  "close "+ std::to_string(trainerId);
    if (this->getStatus() == ERROR){
        str += " Error: "+this->getErrorMsg();
    }
    else {
        str += " Completed\n";
    }
    return str;
}

CloseAll::CloseAll(): BaseAction() {}

void CloseAll::act(Studio &studio) {
    for (int i=0 ; i<studio.getNumOfTrainers() ; i++){
        if ((studio.getTrainer(i))->isOpen()) {
            string salary = to_string((studio.getTrainer(i))->getSalary());
            (studio.getTrainer(i))->closeTrainer();
            string id = to_string(i);
            std::cout << "Trainer " + id + " closed. Salary " + salary + "NIS\n";
        }
    }
    studio.changeStatus(false);
    complete();
}

std::string CloseAll::toString() const {
    return "closeall Completed\n";
}

PrintWorkoutOptions::PrintWorkoutOptions(): BaseAction() {}

void PrintWorkoutOptions::act(Studio &studio) {
    std::string str;
    for (Workout x: studio.getWorkoutOptions()){
        string price = to_string(x.getPrice());
        if (x.getType()==ANAEROBIC){
            str += x.getName()+", Anaerobic, "+price+"\n";
        }
        else if (x.getType()==MIXED){
            str += x.getName()+", Mixed, "+price+"\n";
        }
        else{
            str += x.getName()+", Cardio, "+price+"\n";
        }
    }
    std::cout << str;
    complete();
}

std::string PrintWorkoutOptions::toString() const {
    return "workout_options Completed\n";
}

PrintTrainerStatus::PrintTrainerStatus(int id): BaseAction(), trainerId(id) {}

void PrintTrainerStatus::act(Studio &studio) {
    if (trainerId >= studio.getNumOfTrainers()) {       //FISHY
        this->error("Trainer does not exist or is not open\n");
        std::cout << getErrorMsg();
        return;
    }
    Trainer* t = studio.getTrainer(trainerId);
    std::string str;
    std::string strId = to_string(trainerId);
    if (!t->isOpen()){
        std::cout << "Trainer "+strId+" status: closed\n";
    }
    else{
        str += "Trainer "+strId+" status: open"+"\n"+"Customers: \n";
        for (Customer* c : t->getCustomers()){
            std::string strId1 = to_string(c->getId());
            str += strId1+" "+c->getName()+"\n";
        }
        str+= "Orders: \n";
        for (OrderPair y:t->getOrders()){
            string price = to_string(((y.second).getPrice()));
            string id = to_string(y.first);
            string name = (y.second).getName();
            str += (name + " " + price + "NIS " + id + "\n");
        }
        string salary = to_string(t->getSalary());
        str += "Current Trainer's Salary: "+salary+"NIS\n";
    }
    std::cout << str;
    complete();
}

std::string PrintTrainerStatus::toString() const {
    std::string str =  "status "+std::to_string(trainerId);
    if (this->getStatus() == ERROR){
        str += " Error: "+this->getErrorMsg();
    }
    else {
        str += " Completed\n";
    }
    return str;
}

PrintActionsLog::PrintActionsLog(): BaseAction() {}

void PrintActionsLog::act(Studio &studio) {
    std::vector<BaseAction*> tempBA = studio.getActionsLog();
    std::string str;
    for (size_t i=0; i<tempBA.size(); i++){
        str = tempBA[i]->toString();
        std::cout << str;
    }
    complete();
}

std::string PrintActionsLog::toString() const {
    return "log Completed\n";
}

BackupStudio::BackupStudio(): BaseAction() {}

void BackupStudio::act(Studio &studio) {
    if (backup == nullptr){
        backup = new Studio(studio);
    }
    else{
        delete backup;
        backup = new Studio(studio);
    }
    complete();
}

std::string BackupStudio::toString() const {
    return "backup Completed\n";
}

RestoreStudio::RestoreStudio(): BaseAction() {}

void RestoreStudio::act(Studio &studio) {
    if (backup == nullptr){
        error("No backup available\n");
        std::cout << getErrorMsg();
        return;
    }
    else {
        studio = (*backup);
    }
}

std::string RestoreStudio::toString() const {
    std::string str =  "restore";
    if (this->getStatus() == ERROR){
        str += " Error: "+this->getErrorMsg();
    }
    else {
        str += " Completed\n";
    }
    return str;
}

OpenTrainer::OpenTrainer(const OpenTrainer& other): BaseAction(other), trainerId(other.trainerId), customers(), input(other.input) {     //Check if this is right
    for (Customer* c: other.customers) {
        customers.push_back(c->clone());
    }
}

BaseAction *OpenTrainer::clone() const {
    return new OpenTrainer(*this);
}

BaseAction *Order::clone() const {
    return new Order(*this);
}

BaseAction *MoveCustomer::clone() const {
    return new MoveCustomer(*this);
}

BaseAction *Close::clone() const {
    return new Close(*this);
}

BaseAction *CloseAll::clone() const {
    return new CloseAll(*this);
}

BaseAction *PrintWorkoutOptions::clone() const {
    return new PrintWorkoutOptions(*this);
}

BaseAction *PrintTrainerStatus::clone() const {
    return new PrintTrainerStatus(*this);
}

BaseAction *PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

BaseAction *BackupStudio::clone() const {
    return new BackupStudio(*this);
}

BaseAction *RestoreStudio::clone() const {
    return new RestoreStudio(*this);
}