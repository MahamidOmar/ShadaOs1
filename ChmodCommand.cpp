//
// Created by ebrah on 12/02/2024.
//
#include "ChmodCommand.h"
#include "Helper.h"

void ChmodCommand::execute() {
    char* arguments[COMMAND_MAX_ARGS];
    int num_of_args = _parseCommandLine(this->command_line.c_str(),arguments);
    if(num_of_args!=3){
        cerr<<"smash error: chmod: invalid arguments\n";
        return;
    }else{
        if(atoi(arguments[1]) < 0){
            cerr<<"smash error: chmod: invalid arguments\n";
            return;
        }
    }
    DO_SYS(chmod(arguments[2], strtol(arguments[1], NULL, 8)),chmod );
}

