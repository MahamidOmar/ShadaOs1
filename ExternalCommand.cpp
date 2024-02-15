//
// Created by ebrah on 12/02/2024.
//
#include "ExternalCommand.h"
#include "SmallShell.h"


void ExternalCommand::execute() {
    SmallShell &smash = SmallShell::getInstance();
    char* command = (char*)malloc(strlen(this->command_line.c_str())+1);
    strcpy(command, this->command_line.c_str());
    bool simple_command = true;
    if(this->command_line.find('?') != string::npos || this->command_line.find('*') != string::npos){
        simple_command = false;
    }
    char *parsed[COMMAND_MAX_ARGS + 1];
    _parseCommandLine(this->command_line.c_str(), parsed);
    if (_isBackgroundComamnd(command)) {
        _removeBackgroundSign(command);
    }
    _parseCommandLine(command, parsed);
    int pid;
    DO_SYS(pid = fork(), fork);
    if (pid == 0) {
        DO_SYS(setpgrp() , setpgrp);
        if (simple_command){
            if(execvp(parsed[0],parsed) == -1)
            {
                perror("smash error: execvp failed");
                exit(1);
            }
            //DO_SYS(execvp(parsed[0],parsed) , execvp );
        }else{
            char *argv[] = {"/bin/bash", "-c", command, nullptr};
            DO_SYS(execv("/bin/bash", argv), execv);
        }
        exit(0);
    }
    if( _isBackgroundComamnd(this->command_line.c_str()) ){
        smash.getJobList()->removeFinishedJobs();
        smash.getJobList()->addJob(this,pid,BACKGROUND);
    }
    else{
        smash.setLine(this->command_line);
        smash.setCurrPid(pid);
        DO_SYS(waitpid(pid, NULL, WUNTRACED) , waitpid);
        smash.setLine("") ;
        smash.setCurrPid(-1);
        smash.setCurrId(-1);
    }
}

