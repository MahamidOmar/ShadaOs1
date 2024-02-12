//
// Created by ebrah on 12/02/2024.
//

#ifndef COMMANDS_H_KILLCOMMAND_H
#define COMMANDS_H_KILLCOMMAND_H

#include "BuiltInCommand.h"
#include "JobsList.h"

class KillCommand : public BuiltInCommand {
    // TODO: Add your data members
public:
    KillCommand(const char* cmd_line, JobsList* jobs);
    virtual ~KillCommand() {}
    void execute() override;
};

#endif //COMMANDS_H_KILLCOMMAND_H
