#include <stdint.h>

extern void main();

typedef void(*voidFunc)(void);

extern uint32_t __CTOR_LIST__;
extern uint32_t __DTOR_LIST__;
extern uint32_t __bss_start[];
extern uint32_t _end[];

void _main() {
    //Zero-fill bss section
    for (uint32_t* i = __bss_start; i < _end; ++i){
        *i = 0;
    }

    //Call global constructors
    for (uint32_t i = 1; i >= __CTOR_LIST__; --i){
        ((voidFunc)(*(&__CTOR_LIST__ + i)))();
    }

    //Jump to user main function
    main();

    //Call global destructors
    for (uint32_t i = 1; i <= __DTOR_LIST__; ++i){
        ((voidFunc)(*(&__CTOR_LIST__ + i)))();
    }
}