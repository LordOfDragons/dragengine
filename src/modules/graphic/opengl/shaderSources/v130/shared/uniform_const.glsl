// OpenGL 4.2 and newer allows for more expressions to be considered
// constants. hence some usage of "const" are not allowed before 4.2

#ifdef USE_UNIFORM_CONST
	#define UFCONST const
#else
	#define UFCONST
#endif
