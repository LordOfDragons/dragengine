precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"

flat in int vInstanceID;

out vec4 outOffset;

void main( void ){
	uint flags = uint( pGIProbePosition[ vInstanceID ].w );
	if( ( flags & uint( 1 ) ) == uint( 1 ) ){
		discard;
	}
	
	outOffset = vec4( 0.0, 0.0, 0.0, flags );
}
