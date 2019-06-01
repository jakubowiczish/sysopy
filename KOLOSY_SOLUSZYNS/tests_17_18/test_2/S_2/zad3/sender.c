#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define KEY "./queuekey"

int main(int argc, char* argv[])
{
 
 if(argc !=2){
   printf("Not a suitable number of program parameters\n");
   return(1);
 }

 /******************************************************
 Utworz kolejke komunikatow systemu V "reprezentowana" przez KEY
 Wyslij do niej wartosc przekazana jako parametr wywolania programu 
 Obowiazuja funkcje systemu V
 ******************************************************/


  return 0;
}



