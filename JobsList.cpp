#include "JobsList.h"
#include "Helper.h"
#include <cstring>
#include <iostream>
#include <vector>
#include <sys/wait.h>
#include "Commands.h"

JobsList::JobEntry::JobEntry(int jobId, int jobPid, string jobCmdLine, jobStatus status) {
    this->job_id = jobId;
    this->job_pid = jobPid;
    this->job_command_line = jobCmdLine;
    this->job_status = status;
}

void JobsList::addJob(Command *cmd, int jobPid, jobStatus status) {
    this->removeFinishedJobs();
    int numberOfJobs = (int) this->allJobs.size();

    ////    The case that the job was previously in the list and to be added back
    for(int i = 0 ; i < numberOfJobs ; i++)
    {
        if(this->allJobs[i].get()->getJobPid() == jobPid)
        {
            int prevJobId = this->allJobs[i].get()->getJobId();
            shared_ptr<JobEntry> new_entry = shared_ptr<JobEntry>(new JobEntry(prevJobId, jobPid, cmd->getCommandLine(), status));
            this->removeJobById(prevJobId);
            this->allJobs.push_back(new_entry);
            return;
        }
    }

    ////    The job will be inserted for the first time
    int maxIdForJob = this->getMaxJobId() + 1;
    this->setMaxJobId(maxIdForJob);
    shared_ptr<JobEntry> new_entry = shared_ptr<JobEntry>(new JobEntry(maxIdForJob, jobPid, cmd->getCommandLine(), status));
    this->allJobs.push_back(new_entry);

}

void JobsList::printJobsList() {
    this->removeFinishedJobs();
    for (const auto &item: this->allJobs){
        if (item.get()->getJobStatus() == FOREGROUND){
            continue;
        }
        cout << "["<< item.get()->getJobId() << "] " <<item.get()->getJobCmdLine() <<endl;
    }
}

void JobsList::killAllJobs() {
    for (const auto &item: this->allJobs)
    {
        cout << item.get()->getJobPid() << ":" << item.get()->getJobCmdLine() << endl;
        DO_SYS(kill(item.get()->getJobPid(), SIGKILL), kill);
    }
}

void JobsList::removeFinishedJobs() {
    vector<shared_ptr<JobEntry>> still_running;
    for (const auto &item: this->allJobs){
        int result = 0;
        DO_SYS(result = waitpid(item.get()->getJobPid(),NULL,WNOHANG),waitpid);
        if(result == 0){
            still_running.push_back(item);
        }
    }
    int new_max = 0;
    for (const auto &item: still_running){
        if(item.get()->getJobId() > new_max){
            new_max = item.get()->getJobId();
        }
    }
    this->setMaxJobId(new_max);
    this->allJobs = still_running;
}

JobsList::JobEntry *JobsList::getJobById(int jobId) {
    for (const auto &item: this->allJobs){
        if (item.get()->getJobId() == jobId){
            return item.get();
        }
    }
    return nullptr;
}

void JobsList::removeJobById(int jobId) {
    for(auto item = this->allJobs.begin() ; item < this->allJobs.end() ; item++ ){
        if (item->get()->getJobId() == jobId){
            this->allJobs.erase(item);
            break;
        }
    }
    int new_max = 0;
    for (const auto &item: this->allJobs){
        if(item.get()->getJobId() > new_max){
            new_max = item.get()->getJobId();
        }
    }
    this->setMaxJobId(new_max);
}

JobsList::JobEntry *JobsList::getLastJob(int *lastJobId) {
    for (const auto &item: this->allJobs){
        if(item.get()->getJobId() == this->getMaxJobId()){
            *lastJobId = item.get()->getJobId();
            return item.get();
        }
    }
    ////    Not supposed to get here
    *lastJobId = 0;
    return nullptr;
}

JobsList::JobEntry *JobsList::getLastStoppedJob(int *jobId) {
    int max = 0;
    JobsList::JobEntry* entry_to_return = nullptr;
    for (const auto &item: this->allJobs){
        if (item.get()->getJobStatus() == STOPPED && item.get()->getJobId() > max){
            max = item.get()->getJobId();
            entry_to_return = item.get();
        }
    }
    *jobId = max;
    return entry_to_return;
}

int JobsList::JobEntry::getJobId() {
    return this->job_id;
}

int JobsList::JobEntry::getJobPid() {
    return this->job_pid;
}

string JobsList::JobEntry::getJobCmdLine() {
    return this->job_command_line;
}

jobStatus JobsList::JobEntry::getJobStatus() {
    return this->job_status;
}

void JobsList::setMaxJobId(int id) {
    this->maxJobId = id;
}

int JobsList::getMaxJobId() {
    return this->maxJobId;
}
