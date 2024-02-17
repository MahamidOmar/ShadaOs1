//
// Created by ebrah on 12/02/2024.
//
#include "ForegroundCommand.h"
#include "SmallShell.h"
#include <cctype>
#include <stdexcept>
#include <algorithm>

void ForegroundCommandHelper(int job_id,JobsList::JobEntry* curr_job){
    SmallShell& smash = SmallShell::getInstance();
    if(job_id == 0){
        std::cerr << "smash error: fg: jobs list is empty" << endl;
        return;
    }
//    JobsList::JobEntry* curr_job = list->getJobById(job_id);
//    if(!curr_job){
//        std::cerr << "smash error: fg: job-id " << job_id << " does not exist" << endl;
//        return;
//    }
    smash.setLine(curr_job->getJobCmdLine());
    smash.setCurrPid(curr_job->getJobPid());
    DO_SYS(kill(curr_job->getJobPid(),SIGCONT) , kill);
    cout << curr_job->getJobCmdLine() <<" "<<curr_job->getJobPid() << endl;
    curr_job->setJobStatus(FOREGROUND);
    waitpid (curr_job->getJobPid() , nullptr , WUNTRACED);
    smash.setLine("") ;
    smash.setCurrPid(-1);
    return;
}

bool isNumber(string str){
    int i = 0;
    if (str[0] == '-')
        i = 1;
    for(; i < str.length() ; ++i){
        if(std::isdigit(str[i]) == 0){
            return false;
        }
    }
    return true;
}

void ForegroundCommand::execute() {
    string trimmed = _trim(this->command_line);
    string splited = "";
    string job_number = "";
    if (trimmed.find_first_of(" \n") != string::npos) {
        splited = _trim(trimmed.substr(trimmed.find_first_of(" \n")));
        job_number = splited.substr(0, splited.find_first_of(" \n"));
    }
    char* parsed_command[COMMAND_MAX_ARGS];
    int num_of_args = _parseCommandLine(this->command_line.c_str(), parsed_command);
    if (!isNumber(parsed_command[1])){
        cerr << "smash error: fg: invalid arguments" << endl;
        return;
    }

//    bool ValidNumber = true;
//    std::for_each(splited.begin(), splited.end(), [&](const char &c) {
//        if (!std::isdigit(c))
//            ValidNumber = false;
//    });
//    if (!ValidNumber) {
//        cerr << "smash error: fg: invalid arguments" << endl;
//        return;
//    }

    int job_id;
    JobsList::JobEntry* curr_job;
    if(num_of_args >=2){
        job_id = stoi(job_number);
        curr_job = this->all_jobs->getJobById(job_id);
        if(!curr_job){
            std::cerr << "smash error: fg: job-id " << job_id << " does not exist" << endl;
            return;
        }
    }else{
        job_id = this->all_jobs->maxJobId;
        curr_job = this->all_jobs->getJobById(job_id);
        if (splited.find_first_of(" \n") != string::npos)
        {
            cerr << "smash error: fg: invalid arguments" << endl;
            return;
        }
    }
    if(this->all_jobs->allJobs.size() == 0){
        std::cerr << "smash error: fg: jobs list is empty" << endl;
        return;
    }

    if (num_of_args>2){
        cerr << "smash error: fg: invalid arguments" << endl;
        return;
    }
    ForegroundCommandHelper(job_id, curr_job);
}

