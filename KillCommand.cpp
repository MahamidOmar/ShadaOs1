#include "KillCommand.h"
#include "Helper.h"
#include "SmallShell.h"

bool checkLegalSignal(int signal){
    if(signal > 64 || signal < 1){
        cerr << "smash error: kill: invalid arguments" << endl;
        return false;
    }
    return true;
}

bool isNumber(string str){
    if(str[0] == '-'){
        return false;
    }else{
        for(int i = 0 ; i < str.length() ; ++i){
            if(std::isdigit(str[i]) == 0){
                return false;
            }
        }
    }
    return true;
}

void KillCommand::execute()
{
    char* parsed_command[COMMAND_MAX_ARGS];
    int num_of_args = _parseCommandLine(this->command_line.c_str(), parsed_command);
    if(num_of_args != 3 || !isNumber(parsed_command[1]) || !isNumber(parsed_command[2])){
        cerr << "smash error: kill: invalid arguments" << endl;
        return;
    }

    int signal = abs(stoi(parsed_command[1]));
    if(signal > 31 || signal < 0){
        cerr << "smash error: kill: invalid arguments" << endl;
        return;
    }
    int job_id = stoi(parsed_command[2]);
    JobsList::JobEntry* jobEntry = this->all_jobs->getJobById(job_id);
    if(jobEntry == nullptr){
        cerr << "smash error: kill: invalid arguments" << endl;
        return;
    }

    int job_pid = jobEntry->getJobPid();
    DO_SYS(kill(job_pid, signal), kill);
    if(signal == SIGSTOP){
        jobEntry->setJobStatus(STOPPED);
    }
    if(signal == SIGCONT){
        jobEntry->setJobStatus(BACKGROUND);
    }
    cout << "signal number " << signal << " was sent to pid " << job_pid << endl;

}