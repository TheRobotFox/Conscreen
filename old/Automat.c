#include "Automat.h"
#include <stdio.h>
#include <stdlib.h>
#include "winANSI.h"



typedef struct Automat{
	short type;
	DList  Q; // States
	DList  I; // Input alphabet
	Automat_state* C; // Current state
	Automat_state* S; // Start state
	union {
		struct{
			DList F; // accepted states
		} acceptor;
		struct{
			DList O; // output alphabet
		} transductor;
	};
	DList  input; // input 
} Automat;

typedef struct Automat_state{
	const char* Name;
	DList Next;
	DList Output;

} State;


void (*displayfunction)(DList*);
DList invalid_inputs;
DList automats;
static unsigned char initilized=0;


static void Automat_init_check(){
	if(!initilized){
		// Conscreen_error(0, Conscreen_WARNING, "AutomatLib not initilized!");
		exit(1);
	}
}


void Atumomat_cleanup(int dummy){
	Automat_init_check();
	if(initilized){
		initilized=0;
		Automat* data= automats.data;
		for(int i=0; i< automats.num; i++)
			Automat_free(data+i);
		DList_free(&automats);
		DList_free(&invalid_inputs);
		stopANSI(dummy); // exit(0)
	}
}


int Automat_init(){
	if(!initilized){
		EnableANSI();
		invalid_inputs=DList_create(sizeof(char));
		automats = DList_create(sizeof(Automat*));
		initilized=1;
		signal(SIGINT,Atumomat_cleanup);
		return 0;
	}
	// Conscreen_error(0,Conscreen_WARNING,"AutomatLib already initilized!");
	return 1;
};

// create template Automat
Automat* Automat_create(short type){
	Automat_init_check();

	Automat* A = malloc(sizeof(Automat));
	if(A){
		DList_push(&automats,&A);
		A->type=type;
		A->Q=DList_create(sizeof(State));
		A->I=DList_create(sizeof(char));
		A->input=DList_create(sizeof(char));
		A->C=0;
		A->S=0;
		switch(type){
			case acceptor:
				A->acceptor.F=DList_create(sizeof(Automat_state*));
				break;
			case transductor:
				A->transductor.O=DList_create(sizeof(char));
				break;
			default:
				// Conscreen_error(0,Conscreen_ERROR,"Unknown Automat type: %d", type);
				return 0;
		}
	}
	return A;
}

// add new state to Automat (A)
Automat_state* Automat_state_push(Automat* A, const char* name){
	Automat_init_check();
	Automat_state S = {0};
	S.Next=DList_create(sizeof(Automat_state**));
	S.Name=name;
	
	switch (A->type)
	{
	case transductor:
		S.Output=DList_create(sizeof(char*));
		break;
	
	default:
		break;
	}

	return (Automat_state*)DList_push(&A->Q,&S);
}

// get inputs list index in A->I
int Automat_input_to_index(Automat* A, char input){
	Automat_init_check();
	int input_index = -1;
	for(int i=0; i<A->I.num; i++){
		if(((char*)A->I.data)[i]==input){
			input_index=i;
			break;
		}
	}
	return input_index;
}


// set next stae for [inp] (input)
int Automat_acceptor_state_next(Automat* A, Automat_state* s, char inp, Automat_state* next){
	Automat_init_check();
	int index=Automat_input_to_index(A,inp);
	if(index<0)
		return index;

	if(!DList_pointer_to(&A->Q,s))
		// Conscreen_error(0,Conscreen_ERROR,"Assigning foreign state %s to Automat%d", next->Name, DList_indexof(&automats,&A));


	DList_supply(&s->Next,A->I.num); // supply an element for each input
	Automat_state** data=s->Next.data;
	data[index]=next;
	return 0;
}

void Automat_acceptor_state_set_final(Automat* A, Automat_state* s, unsigned char set){
	Automat_init_check();
	// check if Automat->type is compatable (acceptor)
	if(A->type!=acceptor){
		// Conscreen_error(0,Conscreen_ERROR,"Incompatible Automat! cannot set final");
		return;
	}
	int index=DList_indexof(&A->acceptor.F,s);
	if(set){
		if(index==-1)
			DList_push(&A->acceptor.F,s);
	}else{
		if(index!=-1)
			DList_remove(&A->acceptor.F,index);
	}
}

void Automat_state_set_name(Automat_state* s, const char* name){Automat_init_check();s->Name=name;}

// check if state is inside Q
int Automat_state_check(Automat* A, Automat_state* s){
	Automat_init_check();
	return !DList_pointer_to(&A->Q,s);
}

void Automat_start_set(Automat* A, Automat_state* s){
	Automat_init_check();
	if(Automat_state_check(A,s))
		// Conscreen_error(0,Conscreen_WARNING, "State %s not part of Automat!", s->Name);
	A->S=s;
	A->C=s;
}

void Automat_input_push(Automat* A, char input){
	Automat_init_check();
	DList_push(&A->I,&input);
}

void Automat_restart(Automat* A){
	Automat_init_check();
	A->C=A->S;
	DList_clear(&invalid_inputs);
}

void Automat_reset(Automat* A){
	Automat_init_check();
	DList_clear(&A->input);
	Automat_restart(A);
}

int Automat_save(Automat* A, char* path){
	Automat_init_check();
	return 0;
}

void Automat_free(Automat* A){
	Automat_init_check();

	// free states
	Automat_state* s;
	for(int i=0; i< A->Q.num; i++){
		s=A->Q.data;
		DList_free(&s[i].Next);
		switch(A->type){
			case transductor:
				DList_free(&s[i].Output);
				break;
		}
	}
	DList_free(&A->Q);

	// free inputs
	DList_free(&A->I);

	switch(A->type){
		case acceptor:
			DList_free(&A->acceptor.F);
			break;
		case transductor:
			DList_free(&A->transductor.O);
			break;
		default:
			// Conscreen_error(0,Conscreen_ERROR,"Unknown Automat type: %d", A->type);
			break;
	}


	free(A);
}

void Automat_inputsequence_load(Automat* A, DList* input){
	Automat_init_check();
	Automat_reset(A);
	DList_append(&A->input,input);
}

// do a compute cycle for Automat A
int Automat_next(Automat* A, char input){
	Automat_init_check();
	if(A->C==0)
		return -1;

	// get input as index
	int input_index = Automat_input_to_index(A,input);
	if(input_index==-1){
		int index=DList_indexof(&invalid_inputs, &input);
		if(index!=-1)
			DList_remove(&invalid_inputs, index);
		DList_push(&invalid_inputs, &input);
		return 1;
	}

	Automat_state** s = A->C->Next.data;

	// if transfer function exists
	if(A->C->Next.num<input_index)
		return -2;
	// and is valid
	if(!s[input_index])
		return -2;

	// set new state by input_index
	A->C=s[input_index];
	return 0;
}

// excecute from loaded input
int Automat_run(Automat* A){
	Automat_init_check();
	Automat_restart(A);
	int res;
	char* input=A->input.data;
	for(int i=0; i< A->input.num; i++){
		res = Automat_next(A,input[i]);
		if(res)
			break;
	}
	return res;
}

int Automat_step(Automat* A, char input){
	Automat_init_check();
	int res = Automat_next(A,input);
	switch(res){
		case -1:
			// Conscreen_error(0,Conscreen_FATAL, "Invalid State 0!");
			break;
		case -2:
			// Conscreen_error(0,Conscreen_FATAL, "Function doesn't exist for %c!", input);
			break;
		case 0:
			DList_push(&A->input,&input);
			break;

	}
	return res;
}

int Automat_run_ui(Automat* A){
	Automat_init_check();
	// Preview
	Automat_print(A);
	printf("\033[F\033[2K>");

	char input, cmd=0, no_errors=1;
	while(1){
		input= getchar();
		if(cmd)
			switch(input){
				case '\n':
					printf("\033[3F\033[2K\033[G");
					DList_clear(&invalid_inputs);
					Automat_print(A);
					printf("\033[F\033[2K>");
					no_errors=1;
					cmd=0;
					break;
				case 'c':
					Automat_reset(A);
					break;
				case 'q':
					Automat_free(A);
					printf("\033[2K\033[F\033[2KExiting!");
					return 0;
				default:
					// Conscreen_error(0,Conscreen_ERROR, "Unknown Command :%c", input);
					break;
			}   
		else 
			switch(input){
				case '\n':
					printf("\033[3F\033[2K\033[G");
					Automat_print(A);
					if(no_errors){
						printf("\033[2K");
						DList_clear(&invalid_inputs);
					}
					printf("\033[F\033[2K>");
					no_errors=1;
					break;

				case ':':
					cmd=1;
					break;


				default:{
					int res=Automat_step(A,input);
					if(res<0)
						return -1;
					if(res)
						no_errors=0;
				}
				
			}
	}
	return 0;
}

// print current state and info
int Automat_print(Automat* A){
	Automat_init_check();
	if(A->C){
		// print whole input
		printf("Input: ");
		for(int i=0; i< A->input.num; i++){
			putchar(((char*)A->input.data)[i]);
		}

		// show costum display function
		if(displayfunction && A->input.num){
			printf(" | ");
			displayfunction(&A->input);
		}

		// print current state
		int accepted=0;
		if(A->type==acceptor)
			accepted = (DList_indexof(&A->acceptor.F,A->C)!=-1);
		if(accepted)
			printf("\033[38;2;50;200;55m");
		printf("\n\033[2KC: %s ", A->C->Name);
		if(accepted)
			printf("Accepted!\033[0m");

	}else{
		// Conscreen_error(0,Conscreen_WARNING, "no start specified!");
	}
	printf("\n\n");

	if(invalid_inputs.num){
		// Conscreen_error(0,Conscreen_ERROR,"\033[G\033[2KInvalid Input: \033[38;2;240;120;120m");
		char* data=invalid_inputs.data;
		int length= invalid_inputs.num>5 ? 5 : invalid_inputs.num;
		for(int i=0; i<length; i++){
			printf("\033[38;2;240;120;120m%c\033[0m", data[invalid_inputs.num-1-i]);
			if(i<length-1)  
				printf(", ");
		}
	}

	if(Automat_state_check(A,A->C)){}
		// Conscreen_error(0,Conscreen_WARNING, "State %s not part of Automat!    ", A->C->Name);
	return 0;
}

void Automat_print_displayfunction_set(void (*func)(DList*)){
	Automat_init_check();
	displayfunction=func;
}

