#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "animal.h"
#include "lw_new.h"

klass_info local_animal_klass = {animal_init};
klass_info *animal_klass = &local_animal_klass;

void animal_init(void) {
  if (animal_klass->init) {
    animal_klass->init = NULL;
    animal_klass->super = NULL;
    animal_klass->class_name = "animal_class";
    animal_klass->size = sizeof(animal);
    animal_klass->ctor = animal_ctor;
    animal_klass->dtor = animal_dtor;
  }
}

void *animal_ctor(animal *self) {
  memset(self->name, 0x00, sizeof(self->name));
  self->weight = 0;
  return self;
}
void *animal_dtor(animal *self) {
  printf("%s\n", __FUNCTION__);
  return self;
}

char *animal_get_name(animal *self) { return self->name; }
void animal_set_name(animal *self, char *name) {
  strncpy(self->name, name, sizeof(self->name));
}
int animal_get_weight(animal *self) { return self->weight; }
void animal_set_weight(animal *self, int weight) { self->weight = weight; }
