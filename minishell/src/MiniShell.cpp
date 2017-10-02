

#include "MiniShell.h"
#include "def.h"


#include <stdio.h>
#include <unistd.h>


void MiniShell::Run(){
  

  while(1){
     
    printf("[minishell]$");  
   
    get_command();   
    
    print_command();
  }


}

  
void MiniShell::get_command(){

 // if(NULL == fgets(command, 1024, stdin)){
 //   ERR_EXIT("get command"); 
 // }
  
  
 fgets(command, 1024, stdin);

}

void MiniShell::parse_command(){
}

void MiniShell::exec_command(){


}


void MiniShell::print_command(){
  printf("====%s", command);
}



