# SPL-Task1
# BGU Fitness Studio Management System
This is a C++ program that simulates a fitness studio management system. It allows opening the studio, assigning customers to trainers, creating workout plans, calculating trainer salaries, and performing various other actions.
Features

Open and close trainer workout sessions
Assign customers with different ordering strategies to trainers
Order workout plans based on customer strategies
Move customers between trainers
Print workout options, trainer status, and action logs
Backup and restore the studio's state

Getting Started
Prerequisites

C++11 compiler
Make utility

# Compiling

  Clone the repository
  Navigate to the project directory
  Run make to compile the program

# Running
  After compiling, run the program with the path to the configuration file as the command-line argument:
  Copy code./bin/studio <path_to_config_file>
# Configuration File Format
  The configuration file should follow this format:
  Copy code# Number of trainers
  <number_of_trainers>
  # Trainer capacities
  <trainer_1_capacity>,<trainer_2_capacity>,...
  # Workout options
  <workout_name_1>,<workout_type_1>,<workout_price_1>
  <workout_name_2>,<workout_type_2>,<workout_price_2>
  ...

    Lines starting with # are comments and will be ignored.
    Workout types can be Anaerobic, Mixed, or Cardio.
    The workout IDs are determined by the order in which they appear in the workout options section.

