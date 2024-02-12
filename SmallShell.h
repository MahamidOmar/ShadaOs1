//
// Created by ebrah on 12/02/2024.
//

#ifndef COMMANDS_H_SMALLSHELL_H
#define COMMANDS_H_SMALLSHELL_H

#include "Commands.h"

class SmallShell {
private:
    // TODO: Add your data members
    SmallShell();
    string currPrompt;
    int curr_pid;
    int curr_id;
    string curr_command_line;

public:
    Command *CreateCommand(const char* cmd_line);
    SmallShell(SmallShell const&)      = delete; // disable copy ctor
    void operator=(SmallShell const&)  = delete; // disable = operator
    static SmallShell& getInstance() // make SmallShell singleton
    {
        static SmallShell instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
    ~SmallShell();
    void executeCommand(const char* cmd_line);
    void setPrompt(string new_prompt){
        this->currPrompt = new_prompt;
    }

    string getPrompt(){
        return this->currPrompt;
    }

    void setCurrPid(int new_pid){
        this->curr_pid = new_pid;
    }

    int getCurrPid(){
        return this->curr_pid;
    }

    void setLine(string new_line){
        this->curr_command_line = new_line;
    }

    string getCurrLine(){
        return this->curr_command_line;
    }

    void setCurrId(int new_id){
        this->curr_id = new_id;
    }

    int getCurrId(){
        return this->curr_id;
    }


    // TODO: add extra methods as needed
};

#endif //COMMANDS_H_SMALLSHELL_H
