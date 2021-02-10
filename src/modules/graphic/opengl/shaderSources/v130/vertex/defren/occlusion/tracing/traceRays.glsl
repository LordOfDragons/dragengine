precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/occtracing.glsl"

in vec3 inPosition;

void main( void ){
	gl_Position = vec4( vec3( inPosition.xy, 0.0 ), 1.0 );
}
