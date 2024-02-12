//
// Created by ebrah on 12/02/2024.
//

#ifndef COMMANDS_H_JOBSLIST_H
#define COMMANDS_H_JOBSLIST_H

#include "Commands.h"

class JobsList {
public:
    class JobEntry {
        // TODO: Add your data members
    };
    // TODO: Add your data members
public:
    JobsList();
    ~JobsList();
    void addJob(Command* cmd, bool isStopped = false);
    void printJobsList();
    void killAllJobs();
    void removeFinishedJobs();
    JobEntry * getJobById(int jobId);
    void removeJobById(int jobId);
    JobEntry * getLastJob(int* lastJobId);
    JobEntry *getLastStoppedJob(int *jobId);
    // TODO: Add extra methods or modify exisitng ones as needed
};

#endif //COMMANDS_H_JOBSLIST_H
