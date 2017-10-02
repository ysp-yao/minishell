
#ifndef MINISHELL_H_
#define MINISHELL_H_

class MiniShell {

public:
  void Run();

private:
  
  void get_command();

  void parse_command();

  void exec_command();

  void print_command();

private:
  
  char command[1024];  

};


#endif // MINISHELL_H_
