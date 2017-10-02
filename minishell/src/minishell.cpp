

#include "minishell.h"
#include "def.h"


#include <stdio.h>
#include <unistd.h>


void MiniShell::Run(){
  

  while(1){
     
    printf("[minishell]$");  
   
    get_command();   
 
    parse_command();   
    print_command();
  }


}

  
void MiniShell::get_command(){
  if(NULL == fgets(command_, 1024, stdin)){
    ERR_EXIT("get command"); 
  }
}

void MiniShell::parse_command(){
  /*ls -l*/
  /*ls\0-l\0*/

  



}

void MiniShell::exec_command(){


}


void MiniShell::print_command(){
  printf("====%s", command_);
}



