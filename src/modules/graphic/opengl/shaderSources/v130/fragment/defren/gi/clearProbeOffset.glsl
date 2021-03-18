precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"

flat in int vInstanceID;

out vec3 outOffset;

void main( void ){
	int flags = int( pGIProbePosition[ vInstanceID ].w );
	if( ( flags & 1 ) == 1 ){
		discard;
	}
	
	outOffset = vec3( 0.0 );
}
