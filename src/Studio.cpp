//
// Created by spl211 on 10/11/2021.
//

#include <fstream>
#include <algorithm>
#include <iostream>
#include <ostream>
#include "../include/Studio.h"
using namespace std;

Studio::Studio(): open(),trainers(),workout_options(),actionsLog() {}

Studio::Studio(const string &configFilePath): open(),trainers(),workout_options(),actionsLog() {
    open = false;
    std::ifstream reader(configFilePath);
    if (!reader) {
        cout << "Could not open the file '"
             << configFilePath << "'" << endl;
    }
    string input((istreambuf_iterator<char>(reader)), istreambuf_iterator<char>());
    vector<string> lines;
    stringstream stream(input);
    string line;
    int count = 1;
    int workoutId = 0;
    while (getline(stream, line)) {
        lines.push_back(line);
    }
    for (string tempLine: lines) {
        vector<string> words;
        if (tempLine.length() != 0 && tempLine[0] != '#') {
            split(tempLine, words,',');
            if (count == 2) {
                for (string word: words) {
                    Trainer *t = new Trainer(stoi(word));
                    this->trainers.push_back(t);
                }
            }
            if (count >= 3) {
                WorkoutType type;
                string name = words[0];
                int price;
                if (words[1] == " Anaerobic") {
                    type = ANAEROBIC;
                } else if (words[1] == " Mixed") {
                    type = MIXED;
                } else {
                    type = CARDIO;
                }
                string tempPrice = (words[2]).substr(1, (words[2].length()) - 1);
                price = stoi(tempPrice);
                Workout w = Workout(workoutId, name, price, type);
                this->workout_options.push_back(w);
                workoutId++;
            }
            count++;
        }
    }
}


Trainer *Studio::getTrainer(int tid) {
    return trainers[tid];
}

const vector<BaseAction *> &Studio::getActionsLog() const {
    return actionsLog;
}

void Studio::split(string line, vector<string> &words,char c) {
    string temp;
    for(size_t i=0;i<line.length();++i){
        if(line[i]==c){
            words.push_back(temp);
            temp = "";
        }
        else{
            temp.push_back(line[i]);
        }
    }
    words.push_back(temp);
}

vector<Workout> &Studio::getWorkoutOptions() {
    return workout_options;
}

void Studio::start() {
    string command;
    open = true;
    cout << "Studio is open\n";
    int customerId=0;
    while (open) {
        vector<string> words;
        getline(cin,command);
        split(command,words,' ');
        if (words[0]=="open") {
            int trainerId = stoi(words[1]);
            vector<Customer*> tempCustomers;
            for (size_t i=2;i<words.size();i++) {
                vector<string> customer;
                split(words[i],customer,',');
                string name = customer[0];
                if (customer[1]=="swt") {
                    tempCustomers.push_back(new SweatyCustomer(name,customerId));
                }
                else if (customer[1]=="chp") {
                    tempCustomers.push_back(new CheapCustomer(name,customerId));
                }
                else if (customer[1]=="mcl") {
                    tempCustomers.push_back(new HeavyMuscleCustomer(name,customerId));
                }
                else {
                    tempCustomers.push_back(new FullBodyCustomer(name,customerId));
                }
                customerId++;
            }
            OpenTrainer* ot = new OpenTrainer(trainerId,tempCustomers);
            actionsLog.push_back(ot);
            ot->act(*this);
        }

        else if (words[0]=="order") {
            Order* o = new Order(stoi(words[1]));
            actionsLog.push_back(o);
            o->act(*this);
        }

        else if (words[0]=="move") {
            MoveCustomer* move = new MoveCustomer(stoi(words[1]),stoi(words[2]),stoi(words[3]));
            actionsLog.push_back(move);
            move->act(*this);
        }

        else if (words[0]=="close") {
            Close* close = new Close(stoi(words[1]));
            actionsLog.push_back(close);
            close->act(*this);
        }

        else if (words[0]=="workout_options") {
            PrintWorkoutOptions* printWO = new PrintWorkoutOptions();
            actionsLog.push_back(printWO);
            printWO->act(*this);
        }

        else if (words[0]=="status") {
            PrintTrainerStatus* printStatus = new PrintTrainerStatus(stoi(words[1]));
            actionsLog.push_back(printStatus);
            printStatus->act(*this);
        }

        else if (words[0]=="log") {
            PrintActionsLog* printLog = new PrintActionsLog();
            actionsLog.push_back(printLog);
            printLog->act(*this);
        }
        else if (words[0]=="closeall") {
            CloseAll* closeAll = new CloseAll();
            actionsLog.push_back(closeAll);
            closeAll->act(*this);
        }
        else if (words[0]=="backup") {
            BackupStudio* backUp = new BackupStudio();
            actionsLog.push_back(backUp);
            backUp->act(*this);
        }
        else if (words[0]=="restore") {
            RestoreStudio* restore = new RestoreStudio();
            actionsLog.push_back(restore);
            restore->act(*this);
        }
    }
}

int Studio::getNumOfTrainers() const {
    return  (int)trainers.size();
}

void Studio::changeStatus(bool status) {
    open = status;
}

Studio::~Studio() {
    for (int i=0 ; i<getNumOfTrainers() ; i++){
        delete trainers[i];
        trainers[i]=nullptr;
    }
    for (size_t i=0; i<actionsLog.size() ; i++){
        delete actionsLog[i];
        actionsLog[i] = nullptr;
    }
}

Studio::Studio(const Studio &other): open(),trainers(),workout_options(),actionsLog() {
    open = other.open;
    for (Trainer* t:other.trainers){
        Trainer* tempT = new Trainer(*t);
        this->trainers.push_back(tempT);
    }
    for (size_t i=0;i<other.actionsLog.size();i++){
        this->actionsLog.push_back(other.actionsLog[i]->clone());
    }
    workout_options = other.workout_options;
}

Studio &Studio::operator=(const Studio& other) {
    if (this != &other){
        open = other.open;
        for (Trainer* t: trainers){
            delete t;
        }
        trainers.clear();
        for (Trainer* s:other.trainers){
            trainers.push_back(new Trainer(*s));
        }
        for (auto* a: actionsLog){
            delete a;
        }
        actionsLog.clear();
        for (auto* a: other.actionsLog){
            actionsLog.push_back(a->clone()); //clone
        }
        changeStatus(other.open);
        workout_options = other.workout_options;
    }
    return *this;
}

Studio& Studio::operator=(Studio &&other) {
    if (this != &other) {
        trainers.clear();
        actionsLog.clear();
        open = other.open;
        workout_options = other.workout_options;
        for (size_t i = 0; i < (other.trainers).size(); i++) {
            trainers.push_back(other.trainers[i]);
            other.trainers[i] = nullptr;
        }
        for (size_t i = 0; i < (other.actionsLog).size(); i++) {
            actionsLog.push_back(other.actionsLog[i]);
            other.actionsLog[i] = nullptr;
        }
    }
    return *this;
}

Studio::Studio(Studio &&other): open(),trainers(),workout_options(),actionsLog() {
    open = other.open;
    workout_options = other.workout_options;
    for (Trainer* t : other.trainers){
        trainers.push_back(t);
        t= nullptr;
    }
    for (BaseAction* a : other.actionsLog){
        actionsLog.push_back(a);
        a = nullptr;
    }
}






