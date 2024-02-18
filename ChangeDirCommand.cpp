#include "ChangeDirCommand.h"
#include "SmallShell.h"
#include <unistd.h>
#include <iostream>

bool isChangeDirCommandValid(int commandWords){
    if(commandWords == 1){
        std::cerr << "smash error:>\"cd\"";
        return false;
    }
    if(commandWords > 2){
        std::cerr << "smash error: cd: too many arguments" << std::endl;
        return false;
    }
    return true;
}

void ChangeDirCommand::execute() {
    SmallShell &shell = SmallShell::getInstance();
    char* parsedCommand[COMMAND_ARGS_MAX_LENGTH + 3];
    int commandWords = _parseCommandLine(this->command_line.c_str(), parsedCommand);
    if(!isChangeDirCommandValid(commandWords)){
        return;
    }
    std::string newDirectory;
    if(std::string(parsedCommand[1]) != "-"){
        newDirectory = std::string(parsedCommand[1]);
    }else{
        if(!shell.getPreviousDirectory().empty()){
            newDirectory = shell.getPreviousDirectory();
        }
        else{
            std::cerr << "smash error: cd: OLDPWD not set" << std::endl;
            return;
        }
    }

    char currentDirectory[PATH_MAX];
    if(!getcwd(currentDirectory, sizeof(currentDirectory))){
        perror("smash error: getcwd failed");
    }
    int changeDirResult;
    DO_SYS(changeDirResult = chdir(newDirectory.c_str()), chdir);
    if(changeDirResult != -1){
        shell.setPreviousDirectory(currentDirectory);
    }
}

//#include "ChangeDirCommand.h"
//#include "SmallShell.h"
//#include <unistd.h>
//#include <iostream>
//
//bool checkChangeDirLegal(int command_words){
//    if(command_words == 1){
//        ////    When no args are given for cd command
//        cerr << "smash error:>\"cd\"";
//        return false;
//    }
//    if(command_words > 2){
//        ////    When more than one argument is given for cd command
//        cerr << "smash error: cd: too many arguments" << endl;
//        return false;
//    }
//    return true;
//}
//
//void ChangeDirCommand::execute() {
//    SmallShell &shell = SmallShell::getInstance();
//    char* parsed_command[COMMAND_ARGS_MAX_LENGTH + 3];
//    int command_words = _parseCommandLine(this->command_line.c_str(), parsed_command);
//    if(!checkChangeDirLegal(command_words)){
//        return;
//    }
//    string new_directory;
//    if((string)parsed_command[1] != "-"){
//        new_directory = (string)parsed_command[1];
//    }else{
//        if(shell.getPreviousDirectory() != ""){
//            new_directory = shell.getPreviousDirectory();
//        }
//        else{
//            ////    Here you are in the main directory and can't go back
//            cerr << "smash error: cd: OLDPWD not set" << endl;
//            return;
//        }
//    }
//
//    char current_directory[PATH_MAX];
//    if(!getcwd(current_directory, sizeof(current_directory))){
//        ////    Here getcwd failes
//        perror("smash error: getcwd failed");
//    }
//    int change_dir_result;
//    DO_SYS(change_dir_result = chdir(new_directory.c_str()), chdir);
//    if(change_dir_result != -1){
//        shell.setPreviousDirectory(current_directory);
//    }
//
//
//}
//
