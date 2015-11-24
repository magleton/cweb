#ifndef _LW_NEW_H_
#define _LW_NEW_H_

typedef void *(*void_fun)(void *);
typedef void (*init_fun)(void);

#define KLASS(_object_) ((klass_info *)_object_)

typedef struct _klass_info klass_info;

struct _klass_info {
	void *init;
	klass_info *super;
	char *class_name;
	size_t size;
	int field_numbers;
	void *ctor;
	void *dtor;
};

klass_info *klass_of(void *klass);
void *lw_new(void *klass);
void lw_destory(void *self);
size_t klass_size_of(void* self);

#endif
