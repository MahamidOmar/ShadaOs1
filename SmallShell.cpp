//
// Created by ebrah on 12/02/2024.
//
#include "SmallShell.h"
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <iomanip>

const std::string WHITESPACE = " \n\r\t\f\v";

#if 0
#define FUNC_ENTRY()  \
  cout << __PRETTY_FUNCTION__ << " --> " << endl;

#define FUNC_EXIT()  \
  cout << __PRETTY_FUNCTION__ << " <-- " << endl;
#else
#define FUNC_ENTRY()
#define FUNC_EXIT()
#endif

string _ltrim(const std::string& s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

string _rtrim(const std::string& s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

string _trim(const std::string& s)
{
    return _rtrim(_ltrim(s));
}

int _parseCommandLine(const char* cmd_line, char** args) {
    FUNC_ENTRY()
    int i = 0;
    std::istringstream iss(_trim(string(cmd_line)).c_str());
    for(std::string s; iss >> s; ) {
        args[i] = (char*)malloc(s.length()+1);
        memset(args[i], 0, s.length()+1);
        strcpy(args[i], s.c_str());
        args[++i] = NULL;
    }
    return i;

    FUNC_EXIT()
}

bool _isBackgroundComamnd(const char* cmd_line) {
    const string str(cmd_line);
    return str[str.find_last_not_of(WHITESPACE)] == '&';
}

void _removeBackgroundSign(char* cmd_line) {
    const string str(cmd_line);
    // find last character other than spaces
    unsigned int idx = str.find_last_not_of(WHITESPACE);
    // if all characters are spaces then return
    if (idx == string::npos) {
        return;
    }
    // if the command line does not end with & then return
    if (cmd_line[idx] != '&') {
        return;
    }
    // replace the & (background sign) with space and then remove all tailing spaces.
    cmd_line[idx] = ' ';
    // truncate the command line string up to the last non-space character
    cmd_line[str.find_last_not_of(WHITESPACE, idx) + 1] = 0;
}


CommandsType  checkCommandType(string cmd){
    if (cmd.find('>') != string::npos){
        return REDIRECTION;
    }
    if (cmd.find('|') != string::npos){
        return PIPE;
    }
    if(cmd == "quit"){
        return QUIT;
    }
    if(cmd == "chprompt"){
        return CHPROMPT;
    }
    if(cmd == "showpid"){
        return SHOWPID;
    }
    if(cmd == "pwd"){
        return PWD;
    }
    if(cmd == "cd"){
        return CD;
    }
    if(cmd == "jobs"){
        return JOBS;
    }
    if(cmd == "kill"){
        return KILL;
    }
    if(cmd == "fg"){
        return FG;
    }
    if(cmd == "chmod"){
        return CHMOD;
    }
    if(cmd == ""){
        return NOCOMMAND;
    }
    return EXTERNAL;
}

void Chprompt(SmallShell* shell,string cmd){
    if (cmd.find_first_of(" \n") == string::npos){
        shell->setPrompt("smash");
        return;
    }
    string commandsParams = _trim(cmd.substr(cmd.find_first_of(" \n")));
    shell->setPrompt(commandsParams.substr(0, commandsParams.find_first_of(" \n")));
}

Command *SmallShell::CreateCommand(const char *cmd_line) {
    string trimmed_command_line = _trim(cmd_line);
    int end_line = trimmed_command_line.find_first_of("\n");
    string new_command = trimmed_command_line.substr(0,end_line);
    CommandsType curr_type = checkCommandType(new_command);
    switch (curr_type) {
        case CHPROMPT:
            Chprompt(this, trimmed_command_line);
            return nullptr;
        case SHOWPID:
            return new ShowPidCommand(cmd_line);
        case PWD:
            return new GetCurrDirCommand(cmd_line);
        case CD:
            return new ChangeDirCommand(cmd_line);
        case JOBS:
            return new JobsCommand(cmd_line, this->all_jobs);
        case FG:
            return new ForegroundCommand(cmd_line, this->all_jobs);
        case QUIT:
            return new QuitCommand(cmd_line, this->all_jobs);
        case KILL:
            return new KillCommand(cmd_line, this->all_jobs);
        case EXTERNAL:
            return new ExternalCommand(cmd_line);
        case REDIRECTION:
            return new RedirectionCommand(cmd_line);
            break;
        case PIPE:
            return new PipeCommand(cmd_line);
        case CHMOD:
            return new ChmodCommand(cmd_line);
        default:
            return nullptr;
    }
    return nullptr;
}

