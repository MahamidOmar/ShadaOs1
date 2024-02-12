//
// Created by ebrah on 12/02/2024.
//
#include "PipeCommand.h"
#include "Helper.h"
#include "SmallShell.h"

void pipeHelper(string left_command,string right_command,int num_fd){
    SmallShell& shell = SmallShell::getInstance();
    pid_t left_pid,right_pid;
    int pipe_commands[2];
    DO_SYS(pipe(pipe_commands), pipe);
    DO_SYS(left_pid=fork(),fork);
    if(left_pid == 0){
        setpgrp();
        DO_SYS(dup2(pipe_commands[1] , num_fd) , dup2);
        DO_SYS(close(pipe_commands[0]) , close);
        DO_SYS(close(pipe_commands[1]) , close);
        shell.executeCommand(left_command.c_str());
        exit(0);
    }
    DO_SYS(right_pid = fork() , fork);
    if(right_pid == 0){
        setpgrp();
        DO_SYS(dup2(pipe_commands[0] , STDIN_FILENO) , dup2);
        DO_SYS(close(pipe_commands[0]) , close);
        DO_SYS(close(pipe_commands[1]) , close);
        shell.executeCommand(right_command.c_str());
        exit(0);
    }
    DO_SYS(close(pipe_commands[0]) , close);
    DO_SYS(close(pipe_commands[1]) , close);

    DO_SYS(waitpid(left_pid , NULL , 0) , waitpid);
    DO_SYS(waitpid(right_pid , NULL , 0) , waitpid);
}

void PipeCommand::execute() {
    string curr_command_line = this->command_line;
    curr_command_line = _trim(curr_command_line);
    string left_command = _trim(curr_command_line.substr(0, curr_command_line.find_first_of("|")));
    if (curr_command_line.find("|&") != string::npos){
        pipeHelper(left_command,curr_command_line.substr(curr_command_line.find_first_of("|&") + 2),STDERR_FILENO);
        return;
    }
    pipeHelper(left_command,curr_command_line.substr(curr_command_line.find_first_of('|') + 1),STDOUT_FILENO);
}

