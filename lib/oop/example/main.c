#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "animal.h"
#include "dog.h"
#include "lw_new.h"

int main(int argc, char const *argv[]) {
  animal *animal1 = lw_new(animal_klass);
  animal_set_name(animal1, "Jack");
  animal_set_weight(animal1, 20);
  printf("Animal1 name=%s , weight = %d\n", animal_get_name(animal1),
         animal_get_weight(animal1));
  lw_destory(animal1);

  dog *dog1 = lw_new(dog_klass);
  animal_set_name(ANIMAL(dog1), "Dog Name");
  animal_set_weight(ANIMAL(dog1), 10);
  dog_set_age(dog1, 30);
  printf("Dog1 name = %s , weight = %d , age=%d\n",
         animal_get_name(ANIMAL(dog1)), animal_get_weight(ANIMAL(dog1)),
         dog_get_age(dog1));
  lw_destory(dog1);
  return 0;
}