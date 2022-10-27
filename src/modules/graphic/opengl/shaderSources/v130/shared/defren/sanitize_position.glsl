// OpenGL guarantees gap-free rendering only if positions are binary equal
// after leaving the vertex shader stage. if different objects use
// different matrices for points located at the same point after
// transformation OpenGL still manages to produce gaps due to tiny
// differences. the sanitize method rounds positions to avoid problems

#ifndef SANITIZE_POSITION_FACTOR
	//#define SANITIZE_POSITION_FACTOR 1e-5
	#define SANITIZE_POSITION_FACTOR 1e-4
#endif

vec3 sanitizePosition( in vec3 position ){
	return round( position / vec3( SANITIZE_POSITION_FACTOR ) ) * vec3( SANITIZE_POSITION_FACTOR );
}
