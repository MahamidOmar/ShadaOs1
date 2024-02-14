//
// Created by ebrah on 12/02/2024.
//
#include "RedirectionCommand.h"
#include "SmallShell.h"

void RedirectionCommandHelper(string command,string direction,int flag){
    SmallShell& smash = SmallShell::getInstance();
    int fd_out;
    DO_SYS(fd_out = dup(STDOUT_FILENO), dup);
    DO_SYS(close(STDOUT_FILENO), close);
    int fd ;
    DO_SYS(fd = open(direction.c_str(), flag | O_WRONLY | O_CREAT ,0655) , open);
    smash.executeCommand(command.c_str());
    if (fd == -1)
    {
        perror("smash error: open failed");
        DO_SYS(dup2(fd_out, STDOUT_FILENO), dup2);
        DO_SYS(close(fd_out), close);
        return;
    }else if(fd!=1){
        DO_SYS(close(fd), close);
        DO_SYS(dup2(fd_out, STDOUT_FILENO), dup2);
        DO_SYS(close(fd_out), close);
        cout<<"you have open the wrong fd"<<endl;
    }else{
        int close_result;
        DO_SYS(close_result = close(fd), close);
        if(close_result<0){
            perror("smash error: close failed");
        }
        DO_SYS(dup2(fd_out, STDOUT_FILENO), dup2);
        DO_SYS(close(fd_out), close);
    }
}

void RedirectionCommand::execute() {

    string trimmed = _trim(this->command_line);
    string command = _trim(trimmed.substr(0 , trimmed.find_first_of('>')));
    if(trimmed.find(">>") != string::npos){
        RedirectionCommandHelper(command,_trim(trimmed.substr(trimmed.find_first_of(">>") + 2)),O_APPEND);
    }
    RedirectionCommandHelper(command,_trim(trimmed.substr(trimmed.find_first_of(">") + 1)),O_TRUNC);

}

