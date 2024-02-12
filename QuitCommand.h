//
// Created by ebrah on 12/02/2024.
//

#ifndef COMMANDS_H_QUITCOMMAND_H
#define COMMANDS_H_QUITCOMMAND_H
#include "JobsList.h"
#include "BuiltInCommand.h"

class QuitCommand : public BuiltInCommand {
// TODO: Add your data members public:
    QuitCommand(const char* cmd_line, JobsList* jobs);
    virtual ~QuitCommand() {}
    void execute() override;
};

#endif //COMMANDS_H_QUITCOMMAND_H
