#include "JobsList.h"

//using namespace std;

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

}

void JobsList::removeFinishedJobs() {

}

JobsList::JobEntry *JobsList::getJobById(int jobId) {
    return nullptr;
}

void JobsList::removeJobById(int jobId) {

}

JobsList::JobEntry *JobsList::getLastJob(int *lastJobId) {
    return nullptr;
}

JobsList::JobEntry *JobsList::getLastStoppedJob(int *jobId) {
    return nullptr;
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
