#include "ChangeDirCommand.h"
#include "Helper.h"
#include "SmallShell.h"
#include <unistd.h>
#include <iostream>
#include <cstring>

bool checkChangeDirLegal(int command_words){
    if(command_words == 1){
        ////    When no args are given for cd command
        cerr << "smash error:>\"cd\"";
        return false;
    }
    if(command_words > 2){
        ////    When more than one argument is given for cd command
        cerr << "smash error: cd: too many arguments" << endl;
        return false;
    }
    return true;
}

void ChangeDirCommand::execute() {
    SmallShell &shell = SmallShell::getInstance();
    char* parsed_command[COMMAND_ARGS_MAX_LENGTH + 3];
    int command_words = _parseCommandLine(this->command_line.c_str(), parsed_command);
    if(!checkChangeDirLegal(command_words)){
        return;
    }
    string new_directory;
    if((string)parsed_command[1] != "-"){
        new_directory = (string)parsed_command[1];
    }else{
        if(shell.getPreviousDirectory() != ""){
            new_directory = shell.getPreviousDirectory();
        }
        else{
            ////    Here you are in the main directory and can't go back
            cerr << "smash error: cd: OLDPWD not set" << endl;
            return;
        }
    }

    char current_directory[PATH_MAX];
    if(!getcwd(current_directory, sizeof(current_directory))){
        ////    Here getcwd failes
        perror("smash error: getcwd failed");
    }
    int change_dir_result;
    DO_SYS(change_dir_result = chdir(new_directory.c_str()), chdir);
    if(change_dir_result != -1){
        shell.setPreviousDirectory(current_directory);
    }


}

