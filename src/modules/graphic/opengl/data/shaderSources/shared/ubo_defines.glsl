#ifdef UBO_IDMATACCBUG
	#define MATRIX_ORDER column_major
#else
	#define MATRIX_ORDER row_major
#endif

#define UBOLAYOUT layout (std140, MATRIX_ORDER)
#define UBOLAYOUT_BIND(bp) layout (binding=bp, std140, MATRIX_ORDER)

#define SSBOLAYOUT(bp) layout (binding=bp, std430, MATRIX_ORDER)

// OpenGL 4.2 and newer allows for more expressions to be considered
// constants. hence some usage of "const" are not allowed before 4.2

#ifdef USE_UNIFORM_CONST
	#define UFCONST const
#else
	#define UFCONST
#endif

#ifdef OPENGLES
	#define VARCONST
#else
	#define VARCONST const
#endif
