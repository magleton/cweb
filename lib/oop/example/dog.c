#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dog.h"
#include "lw_new.h"
#include "animal.h"

klass_info local_dog_klass = { dog_init };
klass_info *dog_klass = &local_dog_klass;

void dog_init(void) {
	if (dog_klass->init) {
		animal_init();
		memcpy(dog_klass, animal_klass, sizeof(klass_info));
		dog_klass->super = animal_klass;
		dog_klass->class_name = "dog_klass";
		dog_klass->size = sizeof(dog);
		dog_klass->ctor = dog_ctor;
		dog_klass->dtor = dog_dtor;
	}
}

void *dog_ctor(dog *self) {
	((void_fun) (klass_of(self)->super->ctor))(self);
	self->age = 0;
	return self;
}

void *dog_dtor(dog *self) {
	((void_fun) (klass_of(self)->super->dtor))(self);
	return self;
}

int dog_get_age(dog *self) {
	return self->age;
}
void dog_set_age(dog *self, int age) {
	self->age = age;
}
