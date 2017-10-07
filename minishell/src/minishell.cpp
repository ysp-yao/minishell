

#include "minishell.h"
#include "def.h"


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

MiniShell::MiniShell(){
  //init_();
}

void MiniShell::init_(){

  int i=0;
  for (i;i<kMaxCmdNum;++i){
    int j=0;
    for (j;j<kMaxArgvNum;++j){
      cmds_[i].argv[j] = NULL; 
    }
  }

  input_cmd_ptr_ = NULL;
  
  input_cmd_ptr_ = input_cmd_;
  parsed_cmd_ptr_ = parsed_cmd_;  
  
  memset(input_file_, '\0', kMaxFileNameLength);
  memset(output_file_, '\0', kMaxFileNameLength);
 
  memset(input_cmd_, '\0', kMaxInputCmdLength);
  memset(parsed_cmd_, '\0', kMaxInputCmdLength);

  backgnd = false;

  cmd_cnt_ = 0;
}

void MiniShell::Run(){

  while(1){

    init_();
     
    printf("[minishell]$");  

    read_command();   
    
    parse_command();   

    print_command();

    exec_command();
  }
}

  
void MiniShell::read_command(){
  if(NULL == fgets(input_cmd_, 1024, stdin)){
    ERR_EXIT("get command"); 
  }
}

void MiniShell::get_command(int cmd_index){
  /*ls -l\n*/
  /*ls\0-l\0*/
  /*ls | wc -w\n*/
  /*ls\0wc\0-w\0*/
  /*cat < text.txt | grep -n public > test2.txt & */
 

  int argv_index = 0;

  while ( *input_cmd_ptr_ != '\0' ){
    
    while ( *input_cmd_ptr_  == ' ' || *input_cmd_ptr_  == '\t' ) {
      ++input_cmd_ptr_;
    }       

    if ( *input_cmd_ptr_ == '\n') {
      return;
    }
    
    cmds_[cmd_index].argv[argv_index] = parsed_cmd_ptr_;

    while (*input_cmd_ptr_ != ' ' && 
           *input_cmd_ptr_ != '\n' &&
           *input_cmd_ptr_ != '\t' && 
           *input_cmd_ptr_ != '>' && 
           *input_cmd_ptr_ != '<' && 
           *input_cmd_ptr_ != '|' && 
           *input_cmd_ptr_ != '&'){
      *parsed_cmd_ptr_++ = *input_cmd_ptr_++;
      int a =10;
      //*parsed_cmd_ptr_++ = '\0';
    }

    switch(*input_cmd_ptr_){
      case ' ':
      case '\t':
        parsed_cmd_ptr_++;
        ++argv_index;        
        break;
      case '>':
      case '<':
      case '|':
      case '$':
        cmds_[cmd_index].argv[argv_index] = NULL;
        return;
      case '\n':
        return;
      default:
        return;
    }
  }
}


int MiniShell::parse_command(){ 
  // 1. 第一条
  get_command(0);   

  // 2. 输入重定向
  if (check("<")){
    get_name(input_file_);
  }  

  // 3. 管道
  int i;
  for (i=1;i<kMaxPipeline;++i){
    if (check("|")){
      get_command(i);
    }
    else{
      break;
    }
  }
  cmd_cnt_ = i;  

  // 4. 输出重定向
  if (check(">")){
    get_name(output_file_);
  }  

  // 5. 后台
  if (check("&")){
    backgnd = true;
  }  
  // 6. 结束
  if (check("\n")){
    return 0;
  }
  else {
    fprintf(stderr, "Command Line Syntax Error\n");
    return -1;
  }  
}

void MiniShell::exec_command(){

  /* ls | grep init | wc -w*/
  int i;
  for (i=0;i<cmd_cnt_;++i){

    if (i<cmd_cnt_-1){

    }

    forkexec(i);    

  }

  wait(NULL);
}

void MiniShell::forkexec(int i){

  pid_t pid;
  pid = fork();
  if (pid == -1){
    ERR_EXIT("fork");
  }

  if (pid == 0){
    execvp(cmds_[i].argv[0], cmds_[i].argv);
    exit(EXIT_FAILURE);
  }


}

void MiniShell::print_command(){
  
  printf("===================================\n");
  printf("InputFile:%s\n", input_file_);
  printf("OutputFile:%s\n", output_file_);

  int i=0;
  for (i;i<kMaxCmdNum;++i){
    if (cmds_[i].argv[0] == NULL){
      break;
    }
    int j=0;
    for (j;j<kMaxArgvNum;++j){
      if (cmds_[i].argv[j] == NULL){
        break;
      }
      printf("[%d][%d][%s]\n",i,j, cmds_[i].argv[j]);
    }
  }

  if (backgnd == true){
    printf("backgnd:true\n");
  }
  else {
    printf("backgnd:false\n");
  }
   
  printf("===================================\n");
}

bool MiniShell::check(const char *str){
    
  while (*input_cmd_ptr_ == ' ' || *input_cmd_ptr_ == '\t'){
    ++input_cmd_ptr_;
  }  

  char *p = input_cmd_ptr_;
  while(*str != '\0' && *str == *p){
    str++;
    p++;
  }

  if (*str == '\0'){
    input_cmd_ptr_ = p;
    return true;       
  }
 
  return false;
}

void MiniShell::get_name(char *file_name){
  
  while (*input_cmd_ptr_ == ' ' || *input_cmd_ptr_ == '\t'){
    ++input_cmd_ptr_;
  }
    
  while (*input_cmd_ptr_ != ' ' && 
         *input_cmd_ptr_ != '\n' &&
         *input_cmd_ptr_ != '\t' && 
         *input_cmd_ptr_ != '>' && 
         *input_cmd_ptr_ != '<' && 
         *input_cmd_ptr_ != '|' && 
         *input_cmd_ptr_ != '&'){
    *file_name++ = *input_cmd_ptr_++;
  }
}

