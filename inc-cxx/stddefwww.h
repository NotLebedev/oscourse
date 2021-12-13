#ifndef JOS_INC_STDDEF_H
#define JOS_INC_STDDEF_H

#include <types.h>

namespace std {
    typedef ssize_t ptrdiff_t;
    typedef unsigned long  size_t;
    typedef double max_align_t;
    using nullptr_t = decltype(nullptr);
}

#define NULL nullptr
#define offsetof(aggregate, field) ((unsigned) (& ((aggregate*) 0)->field))
#endif /* not JOS_INC_STDDEF_H */
