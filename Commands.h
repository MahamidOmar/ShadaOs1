#ifndef SMASH_COMMAND_H_
#define SMASH_COMMAND_H_

#include <vector>
#include <string>
#include <memory>
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::shared_ptr;

typedef enum {STOPPED , FOREGROUND , BACKGROUND , FINISHED , EMPTY}jobStatus;

#define COMMAND_ARGS_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)

#define DO_SYS(syscall, name) do { \
    if((syscall) == -1){           \
        perror("smash error: " #name " failed"); \
        return;\
    }                               \
}while(0)  \


class Command {
// TODO: Add your data members

 protected:
    string command_line;

 public:
  Command(const char* cmd_line):command_line(cmd_line){}
  virtual ~Command() = default;
  virtual void execute() = 0;
  string getCommandLine(){
      return this->command_line;
  }
  //virtual void prepare();
  //virtual void cleanup();
  // TODO: Add your extra methods if needed
};

#endif //SMASH_COMMAND_H_
