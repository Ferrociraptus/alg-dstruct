#ifndef OBJECT_H
#define OBJECT_H

#include <stdlib.h> 

#ifndef new
	#define new(type,...) type##_init(MEMORY_ALLOCATOR_FUN(type)() __VA_OPT__(,) __VA_ARGS__)
#endif
#ifndef del
	#define del(obj) free(__del(OBJECT(obj)))
#endif

#define MEMORY_ALLOCATOR(type) type* type##_memory_allocator()
#define DEFAULT_MEMORY_ALLOCATOR(type) type* type##_memory_allocator() {return ((type*)malloc(sizeof(type)));}
#define MEMORY_ALLOCATOR_FUN(type) type##_memory_allocator

#define OBJECT(obj) ((Object*)(obj))
#define CONSTRUCTOR(type, ...) type* type##_init(type* self __VA_OPT__(,) __VA_ARGS__)
#define SUPER(type) type##_init

#define DESTRUCTOR(type) type* type##_del(type* self)
#define DESTRUCTOR_TYPE(fun) ((Destructor)(fun))
#define DESTRUCTOR_FUN(type) type##_del
#define SIMPLE_DESTRUCTOR(type) DESTRUCTOR(type) {return self;}
#define REGISTER_DESTRUCTOR(obj,fun) OBJECT(obj)->destructor=DESTRUCTOR_TYPE(fun);

typedef void*(*Destructor)(void*);
typedef struct _simple_object{
	Destructor destructor;
} Object;

MEMORY_ALLOCATOR(Object);
CONSTRUCTOR(Object);
Object* __del(Object* obj);

#endif
