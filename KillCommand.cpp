//#include "KillCommand.h"
//#include "SmallShell.h"
//
//bool isNumber(string str){
//    int i = 0;
//    if (str[0] == '-')
//        i = 1;
//    for(; i < str.length() ; ++i){
//        if(std::isdigit(str[i]) == 0){
//            return false;
//        }
//    }
//    return true;
//}
//
//void KillCommand::execute()
//{
//    char* parsed_command[COMMAND_MAX_ARGS];
//    int num_of_args = _parseCommandLine(this->command_line.c_str(), parsed_command);
//    if (num_of_args<3){
//        cerr <<"smash error: kill: invalid arguments"<<endl;
//        return;
//    }
//    if (isNumber(parsed_command[2]) == 0){
//        cerr <<"smash error: kill: invalid arguments"<<endl;
//        return;
//    }
//    if (!this->all_jobs->getJobById(stoi(parsed_command[2]))){
//        cerr << "smash error: kill: job-id "<<stoi(parsed_command[2])<< " does not exist"<<endl;
//        return;
//    }
//    if(num_of_args >3){
//        cerr <<"smash error: kill: invalid arguments"<<endl;
//        return;
//    }
//    if(isNumber(parsed_command[1]) == 0 || parsed_command[1][0] != '-' ){
//        cerr << "smash error: kill: invalid arguments" << endl;
//        return;
//    }
//
//    int signal = abs(stoi(parsed_command[1]));
//    if(signal > 31 || signal < 0){
//        cerr << "smash error: kill failed: Invalid argument" << endl;
//        return;
//    }
//    int job_id;
//    try {
//        job_id = stoi(parsed_command[2]);
//    } catch (...) {
//        cerr << "smash error: kill: invalid arguments" << endl;
//        return;
//    }
//    JobsList::JobEntry* jobEntry = this->all_jobs->getJobById(job_id);
//    if(jobEntry == nullptr){
//        cerr << "smash error: kill: job-id " << job_id << " does not exist" << endl;
//        return;
//    }
//
//    int job_pid = jobEntry->getJobPid();
//    DO_SYS(kill(job_pid, signal), kill);
//    if(signal == SIGSTOP){
//        jobEntry->setJobStatus(STOPPED);
//    }
//    if(signal == SIGCONT){
//        jobEntry->setJobStatus(BACKGROUND);
//    }
//    std::cout << "signal number " << signal << " was sent to pid " << jobEntry -> getJobPid() << std::endl;
//}

#include "KillCommand.h"
#include "SmallShell.h"

bool isNumber(const string& str){
    return !str.empty() &&
           (str[0] == '-' ? std::all_of(str.begin() + 1, str.end(), ::isdigit) :
            std::all_of(str.begin(), str.end(), ::isdigit));
}

void KillCommand::execute() {
    char* parsed_command[COMMAND_MAX_ARGS];
    int num_of_args = _parseCommandLine(this->command_line.c_str(), parsed_command);

    if (num_of_args != 3 || !isNumber(parsed_command[1]) || parsed_command[1][0] != '-' || !isNumber(parsed_command[2])) {
        cerr << "smash error: kill: invalid arguments" << endl;
        return;
    }

    int signal = abs(stoi(parsed_command[1]));
    if(signal > 31 || signal < 0){
        cerr << "smash error: kill failed: Invalid argument" << endl;
        return;
    }

    int job_id = stoi(parsed_command[2]);
    JobsList::JobEntry* jobEntry = this->all_jobs->getJobById(job_id);
    if(jobEntry == nullptr){
        cerr << "smash error: kill: job-id " << job_id << " does not exist" << endl;
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
    std::cout << "signal number " << signal << " was sent to pid " << jobEntry -> getJobPid() << std::endl;
}
