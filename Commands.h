#ifndef SMASH_COMMAND_H_
#define SMASH_COMMAND_H_

#include <vector>
#include <string>
#include <memory>
using namespace std;

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


 public:
  Command(const char* cmd_line);
  virtual ~Command();
  virtual void execute() = 0;
  //virtual void prepare();
  //virtual void cleanup();
  // TODO: Add your extra methods if needed
};

#endif //SMASH_COMMAND_H_
