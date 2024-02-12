//
// Created by ebrah on 12/02/2024.
//

#ifndef COMMANDS_H_CHMODCOMMAND_H
#define COMMANDS_H_CHMODCOMMAND_H

#include "BuiltInCommand.h"

class ChmodCommand : public BuiltInCommand {
public:
    ChmodCommand(const char* cmd_line);
    virtual ~ChmodCommand() {}
    void execute() override;
};

#endif //COMMANDS_H_CHMODCOMMAND_H
