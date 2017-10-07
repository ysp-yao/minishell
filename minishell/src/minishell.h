
#ifndef MINISHELL_H_
#define MINISHELL_H_

#include "def.h"


class MiniShell {

public:

  MiniShell();

  void Run();

private:

  struct Cmd {
    char *argv[kMaxArgvNum]; 
  };

  void init_();
 
  bool check(const char *str);

  void get_name(char *file_name);
 
  void read_command();

  void get_command(int cmd_index);

  int parse_command();

  void exec_command();

  void forkexec(int i);

  void print_command();

private:

  bool backgnd;

  int cmd_cnt_;

  char *input_cmd_ptr_, *parsed_cmd_ptr_;

  char input_file_[kMaxFileNameLength];
  char output_file_[kMaxFileNameLength];

  char input_cmd_[kMaxInputCmdLength];  
  char parsed_cmd_[kMaxInputCmdLength];  
  
  Cmd cmds_[kMaxCmdNum]; 
};


#endif // MINISHELL_H_
