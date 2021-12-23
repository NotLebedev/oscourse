/* Called from entry.S to get us going.
 * entry.S already took care of defining envs, pages, uvpd, and uvpt */

#include "inc/lib.h"
#include "inc/x86.h"

const volatile struct Env *thisenv;
const char *binaryname = "<unknown>";

extern "C"
void
libmain(int argc, char **argv) {
    /* Call all global constructors */
    extern void (*__ctors_start)(), (*__ctors_end)();
    void (**ctor)() = &__ctors_start;
    while (ctor < &__ctors_end)
        (*ctor++)();

    /* Set thisenv to point at our Env structure in envs[]. */
    thisenv = &envs[ENVX(sys_getenvid())];

    /* Save the name of the program so that panic() can use it */
    if (argc > 0) binaryname = argv[0];

    /* Call user main routine */
    main(argc, argv);

    exit();
}
