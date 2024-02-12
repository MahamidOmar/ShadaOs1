#include "ShowPidCommand.h"
#include "SmallShell.h"
#include "Helper.h"

void ShowPidCommand::execute(){
    SmallShell& shell = SmallShell::getInstance();
    cout << "smash pid is " << shell.getSmashPid() << endl;
}

