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
    // TODO: add extra methods as needed
};

#endif //COMMANDS_H_SMALLSHELL_H
