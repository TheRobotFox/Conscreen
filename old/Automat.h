#include "DynamicList/DynamicList.h"
// #include "CWM/CWM.h"

#define STD_LIST_PADD 20

enum Automat_type{
	acceptor,
	transductor
};

typedef struct Automat Automat;
typedef struct Automat_state Automat_state;


int Automat_init();
Automat* Automat_create(short type);

Automat_state* Automat_state_push(Automat* A, const char* name);
int Automat_acceptor_state_next(Automat* A, Automat_state* s, char inp, Automat_state* next);
void Automat_acceptor_state_set_final(Automat* A, Automat_state* s, unsigned char set);
void Automat_state_set_name(Automat_state* s, const char* name);

void Automat_start_set(Automat* A, Automat_state* s);

void Automat_reset(Automat* A);

void Automat_input_push(Automat* A, char input);

void Automat_inputsequence_load(Automat* A, DList* input);

int Automat_run(Automat* A);
int Automat_step(Automat* A, char input);
int Automat_run_ui(Automat* A);

int Automat_print(Automat* A);
void Automat_print_displayfunction_set(void (*func)(DList*));
void Automat_free(Automat* A);