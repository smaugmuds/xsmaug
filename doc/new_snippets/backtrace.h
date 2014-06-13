#ifndef WIN32
#include <string.h>
#include <dlfcn.h>
/* Backtracing courtesy of Kiasyn and
   Clifford Wolf (http://www.clifford.at/cfun/elfstuff/)
   We can't do this while the mud is booting up :b causes strange errors. */ extern bool fBootDb;
#define BACKTRACE() \
if ( !fBootDb ) \ { \
	Dl_info dli; \
	int backx = 0; \
	void *returnaddr; \
	for ( backx = 0; backx < 4; backx++ ) \
	{ \
		returnaddr = (void *) 1; \
		switch( backx ) \
		{ \
			case 0: returnaddr = __builtin_return_address(0); break; \
			case 1: returnaddr = __builtin_return_address(1); break; \
			case 2: returnaddr = __builtin_return_address(2); break; \
			case 3: returnaddr = __builtin_return_address(3); break; \
			case 4: returnaddr = __builtin_return_address(4); break; \
			default: returnaddr = 0; break; \
		}                                                                       \
		if ( !returnaddr ) \
		break; \
		dladdr( returnaddr, &dli); \
		if ( !&dli ) \
		break; \
		debug( " ... backtrace: %*s called by %p #%d [ %s(%p) %s(%p) ].", \
			(int)strlen(__func__ ), backx == 0 ? __func__ : "...", \
			returnaddr, backx, \
			strrchr(dli.dli_fname, '/') ? strrchr( dli.dli_fname, '/')+1 : dli.dli_fname, \
			dli.dli_fbase, dli.dli_sname, dli.dli_saddr ); \
                }       \
    }
#else define BACKTRACE() do { debug( "Backtrace not enabled under windows" ); } while(0) endif
