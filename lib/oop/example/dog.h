#ifndef _DOG_H_
#define _DOG_H_

#include "animal.h"
#include "lw_new.h"

#define ANIMAL(_object_) ((animal *)_object_)

extern klass_info *dog_klass;
typedef struct _dog dog;

struct _dog {
  animal base;
  int age;
};

int dog_get_age(dog *self);
void dog_set_age(dog *self, int age);

void dog_init(void);

void *dog_ctor(dog *self);
void *dog_dtor(dog *self);

#endif