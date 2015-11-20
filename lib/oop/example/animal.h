#ifndef _ANIMAL_H_
#define _ANIMAL_H_

#include "lw_new.h"
extern klass_info *animal_klass;

typedef struct _animal animal;
struct _animal {
	klass_info *klass;
	char name[256];
	int weight;
};

char *animal_get_name(animal *self);
void animal_set_name(animal *self, char *name);
int animal_get_weight(animal *self);
void animal_set_weight(animal *self, int weight);

void animal_init(void);

void *animal_ctor(animal *self);
void *animal_dtor(animal *self);

#endif
