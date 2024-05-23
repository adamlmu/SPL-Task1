#ifndef SPL_HW_STUDIO_H_
#define SPL_HW_STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"
using namespace std;

class Studio{		
public:
	Studio();
    Studio(const std::string &configFilePath);              //Constructor
    Studio(const Studio& other);                            //Copy constructor
    Studio(Studio &&other);                                 //Move Constructor
    Studio& operator=(const Studio& other);                 //Copy Assignment constructor
    Studio& operator=(Studio &&other);                      //Move Assignment constructor
    ~Studio();
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    void split(string line, vector<string> &words,char c);
    void changeStatus(bool status);

private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
};

#endif