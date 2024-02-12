//
// Created by ebrah on 12/02/2024.
//

#ifndef COMMANDS_H_PIPECOMMAND_H
#define COMMANDS_H_PIPECOMMAND_H
#include "Commands.h"

class PipeCommand : public Command {
    // TODO: Add your data members
    bool is_background;
public:
    PipeCommand(const char* cmd_line,bool is_background): Command(cmd_line){}
    virtual ~PipeCommand() {}
    void execute() override;
};

#endif //COMMANDS_H_PIPECOMMAND_H
