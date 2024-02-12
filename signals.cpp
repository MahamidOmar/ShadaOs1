#include <iostream>
#include <signal.h>
#include "signals.h"
#include "Commands.h"
#include "SmallShell.h"

using namespace std;

void ctrlCHandler(int sig_num) {
    /**
         std::cout<<"smash: got ctrl-C" << endl;
    shared_ptr<Command> cmd = SmallShell::getInstance().getCurrentCmd();
    if(cmd){
        if(kill(cmd->getProcessId(), SIGINT) < 0) perror("smash error: kill failed");
        std::cout<<"smash: process "<< cmd->getProcessId() <<" was killed\n";
        if(SmallShell::getInstance().fg == 1) {
            SmallShell::getInstance().getJobsList().removeJobByPid(cmd->getProcessId());
        }
    }
     * */


    SmallShell& smash = SmallShell::getInstance();
    cout <<"smash: got ctrl-C"<<endl;
    if(smash.getCurrPid() == -1){
        return;
    }
    int result;
    DO_SYS(result = kill(smash.getCurrPid() , SIGKILL),kill);
    if(result == -1){
        return;
    }
    cout<< "smash: process " << smash.getCurrPid() << " was killed"<<endl;

  // TODO: Add your implementation
}

void alarmHandler(int sig_num) {
  // TODO: Add your implementation
}

