#ifdef UBO_IDMATACCBUG
	#define MATRIX_ORDER column_major
#else
	#define MATRIX_ORDER row_major
#endif
#define UBOLAYOUT layout (std140, MATRIX_ORDER)

#define UBOLAYOUT_BIND(bp) layout (binding=bp, std140, MATRIX_ORDER)
