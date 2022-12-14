#ifndef JOS_INC_SYS_CDEFS_H
#define JOS_INC_SYS_CDEFS_H

#if defined(__cplusplus)

#define	__BEGIN_DECLS	extern "C" {
#define	__END_DECLS		}

#define _Noreturn [[noreturn]]
#define restrict

#define typeof __typeof__

#define __ENTRY_POINT main
#define __ENTRY_DECL extern "C++" int

#else

#define	__BEGIN_DECLS
#define	__END_DECLS

#define ENTRY_POINT umain
#define __ENTRY_DECL void
#endif

#endif
