# Operating_System

## User-Manager_App Requirements:

-Create a program in Bash that implements the following features:<br><br>
The end user should be able to create a username in the Ubuntu Linux environment.<br>
The end user should be able to change the password for a user.<br>
The end user should be able to delete a user.<br>
The end user should be able to list active/existing users.<br>
Each feature must be implemented in a separate script and be called from the main program script.<br>
Each time an end user creates a user, the program should add a log, including a timestamp, in the adduser_log.csv file.<br>
Each time an end user changes the password for a user, the program should increment the password reset counter in the passwordreset_counter.csv file and add a log, including a timestamp, in the passwordreset_log.csv file.<br>
Each time an end user deletes a user, the program should add a log, including a timestamp, in the userdeletion_log.csv file.<br>
The program should store active users, including their creation timestamps, in the active_users.csv file.<br>

## Assignment_1 Requirements: 

1. Please correct the producer-consumer.cpp c++ code for the consumer section and ensure that you protect the critical region/section. The producer-consumer binary serves as an example of how the binary should run and execute.<br>
2. Write a bash script that will perform the following tasks:<br>
2.1 The script should compile the producer-consumer.cpp file using the g++ compiler.<br>
2.2 The script should execute the binary.<br>
2.3 The script should create folders named Consumer1 and Consumer2.<br>
2.4 The script should separate the orders of Consumer1 and Consumer2 and save them in the corresponding file in the respective folder.<br>
2.5 The script should print the number of processed orders of each consumer.<br>
