#include "JobsList.h"
#include "Commands.h"

JobsList::JobEntry::JobEntry(int jobId, int jobPid, string jobCmdLine, jobStatus status)
        : job_id(jobId), job_pid(jobPid), job_command_line(jobCmdLine), job_status(status) {}

//void JobsList::addJob(Command *cmd, int jobPid, jobStatus status) {
//    this->removeFinishedJobs();
//    int numberOfJobs = (int) this->allJobs.size();
//
//    ////    The case that the job was previously in the list and to be added back
//    for(int i = 0 ; i < numberOfJobs ; i++)
//    {
//        if(this->allJobs[i].get()->getJobPid() == jobPid)
//        {
//            int prevJobId = this->allJobs[i].get()->getJobId();
//            shared_ptr<JobEntry> new_entry = shared_ptr<JobEntry>(new JobEntry(prevJobId, jobPid, cmd->getCommandLine(), status));
//            this->removeJobById(prevJobId);
//            this->allJobs.push_back(new_entry);
//            return;
//        }
//    }
//
//    ////    The job will be inserted for the first time
//    int maxIdForJob = this->getMaxJobId() + 1;
//    this->setMaxJobId(maxIdForJob);
//    shared_ptr<JobEntry> new_entry = shared_ptr<JobEntry>(new JobEntry(maxIdForJob, jobPid, cmd->getCommandLine(), status));
//    this->allJobs.push_back(new_entry);
//
//}
void JobsList::addJob(Command *cmd, int jobPid, jobStatus status) {
    removeFinishedJobs();
    vector<shared_ptr<JobEntry>>::iterator jobIter = find_if(allJobs.begin(), allJobs.end(), [jobPid](const shared_ptr<JobEntry>& job) { return job->getJobPid() == jobPid; });

    if (jobIter != allJobs.end()) {
        int prevJobId = (*jobIter)->getJobId();
        allJobs.push_back(make_shared<JobEntry>(prevJobId, jobPid, cmd->getCommandLine(), status));
        removeJobById(prevJobId);
    } else {
        int maxIdForJob = getMaxJobId() + 1;
        setMaxJobId(maxIdForJob);
        allJobs.push_back(make_shared<JobEntry>(maxIdForJob, jobPid, cmd->getCommandLine(), status));
    }
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
    cout << "smash: sending SIGKILL signal to " << this->allJobs.size() << " jobs:" << endl;
    for (const auto &item: this->allJobs)
    {
        cout << item.get()->getJobPid() << ": " << item.get()->getJobCmdLine() << endl;
        DO_SYS(kill(item.get()->getJobPid(), SIGKILL), kill);
    }
}


void JobsList::removeFinishedJobs() {
    vector<shared_ptr<JobEntry>> still_running;
    for (const auto &item: this->allJobs){
        if (item -> getJobStatus() == FOREGROUND)
            continue;
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
    vector<shared_ptr<JobEntry>>::iterator jobIter = find_if(allJobs.begin(), allJobs.end(), [jobId](const shared_ptr<JobEntry>& job) { return job->getJobId() == jobId; });
    return jobIter != allJobs.end() ? jobIter->get() : nullptr;
}

void JobsList::removeJobById(int jobId) {
    allJobs.erase(remove_if(allJobs.begin(), allJobs.end(), [jobId](const shared_ptr<JobEntry>& job) { return job->getJobId() == jobId; }), allJobs.end());
    setMaxJobId(max_element(allJobs.begin(), allJobs.end(), [](const shared_ptr<JobEntry>& a, const shared_ptr<JobEntry>& b) {
        return a->getJobId() < b->getJobId();
    })->get()->getJobId());
}

JobsList::JobEntry *JobsList::getLastJob(int *lastJobId) {
    *lastJobId = getMaxJobId();
    return getJobById(*lastJobId);
}

JobsList::JobEntry *JobsList::getLastStoppedJob(int *jobId) {
    vector<shared_ptr<JobEntry>>::iterator jobIter = max_element(allJobs.begin(), allJobs.end(), [](const shared_ptr<JobEntry>& a, const shared_ptr<JobEntry>& b) {
        return a->getJobStatus() == STOPPED && a->getJobId() < b->getJobId();
    });
    *jobId = jobIter != allJobs.end() ? jobIter->get()->getJobId() : 0;
    return jobIter != allJobs.end() ? jobIter->get() : nullptr;
}

//JobsList::JobEntry *JobsList::getLastJob(int *lastJobId) {
//    for (const auto &item: this->allJobs){
//        if(item.get()->getJobId() == this->getMaxJobId()){
//            *lastJobId = item.get()->getJobId();
//            return item.get();
//        }
//    }
//    ////    Not supposed to get here
//    *lastJobId = 0;
//    return nullptr;
//}

//JobsList::JobEntry *JobsList::getLastStoppedJob(int *jobId) {
//    int max = 0;
//    JobsList::JobEntry* entry_to_return = nullptr;
//    for (const auto &item: this->allJobs){
//        if (item.get()->getJobStatus() == STOPPED && item.get()->getJobId() > max){
//            max = item.get()->getJobId();
//            entry_to_return = item.get();
//        }
//    }
//    *jobId = max;
//    return entry_to_return;
//}

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


//#include "JobsList.h"
//#include "Commands.h"
//
//JobsList::JobEntry::JobEntry(int jobId, int jobPid, string jobCmdLine, jobStatus status)
//        : job_id(jobId), job_pid(jobPid), job_command_line(jobCmdLine), job_status(status) {}
//
//void JobsList::addJob(Command *cmd, int jobPid, jobStatus status) {
//    removeFinishedJobs();
//    vector<shared_ptr<JobEntry>>::iterator jobIter = find_if(allJobs.begin(), allJobs.end(), [jobPid](const shared_ptr<JobEntry>& job) { return job->getJobPid() == jobPid; });
//
//    if (jobIter != allJobs.end()) {
//        int prevJobId = (*jobIter)->getJobId();
//        allJobs.push_back(make_shared<JobEntry>(prevJobId, jobPid, cmd->getCommandLine(), status));
//        removeJobById(prevJobId);
//    } else {
//        int maxIdForJob = getMaxJobId() + 1;
//        setMaxJobId(maxIdForJob);
//        allJobs.push_back(make_shared<JobEntry>(maxIdForJob, jobPid, cmd->getCommandLine(), status));
//    }
//}
//
//void JobsList::printJobsList() {
//    removeFinishedJobs();
//    for (const auto &item: allJobs){
//        if (item->getJobStatus() != FOREGROUND){
//            cout << "["<< item->getJobId() << "] " << item->getJobCmdLine() <<endl;
//        }
//    }
//}
//
//void JobsList::killAllJobs() {
//    cout << "smash: sending SIGKILL signal to " << allJobs.size() << " jobs:" << endl;
//    for (const auto &item: allJobs) {
//        cout << item->getJobPid() << ": " << item->getJobCmdLine() << endl;
//        DO_SYS(kill(item->getJobPid(), SIGKILL), kill);
//    }
//}
//
//void JobsList::removeFinishedJobs() {
//    vector<shared_ptr<JobEntry>> still_running;
//    copy_if(allJobs.begin(), allJobs.end(), back_inserter(still_running), [](const shared_ptr<JobEntry>& job) {
//        int result = 0;
//        result = waitpid(job->getJobPid(),NULL,WNOHANG);
//        return result == 0 && job->getJobStatus() != FOREGROUND;
//    });
//    setMaxJobId(max_element(still_running.begin(), still_running.end(), [](const shared_ptr<JobEntry>& a, const shared_ptr<JobEntry>& b) {
//        return a->getJobId() < b->getJobId();
//    })->get()->getJobId());
//    allJobs = move(still_running);
//}
//
//JobsList::JobEntry *JobsList::getJobById(int jobId) {
//    vector<shared_ptr<JobEntry>>::iterator jobIter = find_if(allJobs.begin(), allJobs.end(), [jobId](const shared_ptr<JobEntry>& job) { return job->getJobId() == jobId; });
//    return jobIter != allJobs.end() ? jobIter->get() : nullptr;
//}
//
//void JobsList::removeJobById(int jobId) {
//    allJobs.erase(remove_if(allJobs.begin(), allJobs.end(), [jobId](const shared_ptr<JobEntry>& job) { return job->getJobId() == jobId; }), allJobs.end());
//    setMaxJobId(max_element(allJobs.begin(), allJobs.end(), [](const shared_ptr<JobEntry>& a, const shared_ptr<JobEntry>& b) {
//        return a->getJobId() < b->getJobId();
//    })->get()->getJobId());
//}
//
//JobsList::JobEntry *JobsList::getLastJob(int *lastJobId) {
//    *lastJobId = getMaxJobId();
//    return getJobById(*lastJobId);
//}
//
//JobsList::JobEntry *JobsList::getLastStoppedJob(int *jobId) {
//    vector<shared_ptr<JobEntry>>::iterator jobIter = max_element(allJobs.begin(), allJobs.end(), [](const shared_ptr<JobEntry>& a, const shared_ptr<JobEntry>& b) {
//        return a->getJobStatus() == STOPPED && a->getJobId() < b->getJobId();
//    });
//    *jobId = jobIter != allJobs.end() ? jobIter->get()->getJobId() : 0;
//    return jobIter != allJobs.end() ? jobIter->get() : nullptr;
//}
//
//int JobsList::JobEntry::getJobId() {
//    return job_id;
//}
//
//int JobsList::JobEntry::getJobPid() {
//    return job_pid;
//}
//
//string JobsList::JobEntry::getJobCmdLine() {
//    return job_command_line;
//}
//
//jobStatus JobsList::JobEntry::getJobStatus() {
//    return job_status;
//}
//
//void JobsList::setMaxJobId(int id) {
//    maxJobId = id;
//}
//
//int JobsList::getMaxJobId() {
//    return maxJobId;
//}