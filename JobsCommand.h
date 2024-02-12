//
// Created by ebrah on 12/02/2024.
//

#ifndef COMMANDS_H_JOBSCOMMAND_H
#define COMMANDS_H_JOBSCOMMAND_H

#include "BuiltInCommand.h"
#include "JobsList.h"

class JobsCommand : public BuiltInCommand {
    // TODO: Add your data members
public:
    JobsCommand(const char* cmd_line, JobsList* jobs);
    virtual ~JobsCommand() {}
    void execute() override;
};

#endif //COMMANDS_H_JOBSCOMMAND_H
