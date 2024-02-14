#include "QuitCommand.h"

void QuitCommand::execute() {
    string current_command_trimmed = _trim(this->command_line);

    ////    This checks that the quit command recieved at least one parameter
    if(current_command_trimmed.find_first_of(" \n") != string::npos){
        string command_parameters = _trim(current_command_trimmed.substr(current_command_trimmed.find_first_of(" \n")));
        if(command_parameters.substr(0, command_parameters.find_first_of(" \n")) == "kill"){
//            cout << "smash: sending SIGKILL signal to " << this->all_jobs.size() << " jobs:" << endl;
            this->all_jobs->killAllJobs();
        }
        exit(0);
    }
}
