#include "fixedmem.h"
#include <stdio.h>

char* table[] = {
	"Hello!",
	"Goodbye!",
	"You smell funny",
	"Bob",
	"Whomst'd've?"
};

typedef struct{
	size_t next;
	char used;
	char* data;
} lldata;

FIXEDMEM_LL_EXTERN(lldata, myll, 8); //Can support 256 entries.
FIXEDMEM_LL(lldata, myll, 8); //Can support 256 entries.

int main(){
	//Validate that the empty linked list wont give you anything.
	lldata* d = myll(0);
	if(d)exit(1);
	for(int i = 0; i < 200; i++)
	{
		lldata c; 
		c.data = table[i%5];
		if(myll_insert(0,c)){
			puts("Success Placing in linked list!\n");
			lldata* d = myll(0);
			if(!d){
				puts("Unable to verify that it actually got put in the array.\n");
				exit(1);
			}
		} else {printf("Error. %d\n", i);exit(1);}
	}
	for(int i = 0; i < 210; i++){
		lldata* d = myll(i);
		if(i > 199 && d)
			printf("Something is awry all right! There seems to be an extra element!\n");
		else if(d)
			printf("Its data is %s and it should be %s\n", d->data, table[(199-i)%5]);
	}
	//Now we insert something in the middle.
	{
		lldata c; 
		c.data = "Ain't I a stinker?";
		if(myll_insert(30,c))
			puts("Successfully inserted fun text at index 30!\n");
		else 
			puts("Error. Could not place stinker.\n");
		
	}

	for(int i = 0; i < 40; i++){
			lldata* d = myll(i);
			if(d){
				if(i < 30)
					printf("Its data is %s and it should be %s\n", d->data, table[(199-i)%5]);
				else if(i > 30)
					printf("Its data is %s and it should be %s\n", d->data, table[(200-i)%5]);
				else {
					printf("Its data is %s and it should be A STINKER~~~~~~~~~~!!!!!!!!!!!!!\n", d->data);
				}
			} else {printf("Error finding stinker, %d\n", i);exit(1);}
		}
	printf("Removing the stinker... ~~~~~~~~~~~~~~\n");
	myll_remove(30);
	for(int i = 0; i < 201; i++){
		lldata* d = myll(i);
		if(i > 199 && d)
			printf("Something is awry all right! There seems to be an extra element!\n");
		else if(d)
			printf("Its data is %s and it should be %s\n", d->data, table[(199-i)%5]);
	}
}
