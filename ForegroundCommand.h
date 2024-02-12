//
// Created by ebrah on 12/02/2024.
//

#ifndef COMMANDS_H_FOREGROUNDCOMMAND_H
#define COMMANDS_H_FOREGROUNDCOMMAND_H

#include "BuiltInCommand.h"
#include "JobsList.h"

class ForegroundCommand : public BuiltInCommand {
    // TODO: Add your data members
public:
    ForegroundCommand(const char* cmd_line, JobsList* jobs);
    virtual ~ForegroundCommand() {}
    void execute() override;
};

#endif //COMMANDS_H_FOREGROUNDCOMMAND_H
