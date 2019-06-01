#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#define KEY  "./queuekey"


int main() {
        sleep(1);
        int val = 0;


	/**********************************
	Otworz kolejke systemu V "reprezentowana" przez KEY
	**********************************/

	


	while(1)
 	{	
	    /**********************************
	    Odczytaj zapisane w kolejce wartosci i przypisz je do zmiennej val
	    obowiazuja funkcje systemu V
	    ************************************/

        	 printf("%d square is: %d\n", val, val*val);
 
	}

	/*******************************
	posprzataj
	********************************/


     return 0;
   }
