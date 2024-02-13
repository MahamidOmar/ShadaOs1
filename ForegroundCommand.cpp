//
// Created by ebrah on 12/02/2024.
//
#include "ForegroundCommand.h"
#include "Helper.h"
#include "SmallShell.h"

void ForegroundCommandHelper(int job_id,JobsList* list){
    SmallShell& smash = SmallShell::getInstance();
    if(job_id == 0){
        std::cerr << "smash error: fg: jobs list is empty" << endl;
        return;
    }
    JobsList::JobEntry* curr_job = list->getJobById(job_id);
    if(!curr_job){
        std::cerr << "smash error: fg: job-id " << job_id << " does not exist" << endl;
        return;
    }
    smash.setLine(curr_job->getJobCmdLine());
    smash.setCurrPid(curr_job->getJobPid());
    DO_SYS(kill(curr_job->getJobPid(),SIGCONT) , kill);
    cout << curr_job->getJobCmdLine() <<" : "<<curr_job->getJobPid() << endl;
    curr_job->setJobStatus(FOREGROUND);
    waitpid (curr_job->getJobPid() , nullptr , WUNTRACED);
    smash.setLine("") ;
    smash.setCurrPid(-1);
    return;
}

void ForegroundCommand::execute() {
    string trimmed = _trim(this->command_line);
    string splited = "";
    string job_number = "";
    if (trimmed.find_first_of(" \n") != string::npos) {
        splited = _trim(trimmed.substr(trimmed.find_first_of(" \n")));
        job_number = splited.substr(0, splited.find_first_of(" \n"));
    }
    int job_id;
    if(job_number!=""){
        job_id = stoi(job_number);
    }else{
        job_id = this->all_jobs->maxJobId;
        if (splited.find_first_of(" \n") != string::npos)
        {
            cerr << "smash error: fg: invalid arguments" << endl;
            return;
        }
    }
    ForegroundCommandHelper(job_id, this->all_jobs);
}

