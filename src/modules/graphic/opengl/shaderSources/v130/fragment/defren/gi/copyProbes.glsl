precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"

uniform sampler2DArray texProbe;

out vec4 outProbe;

void main( void ){
	outProbe = texelFetch( texProbe, ivec3( gl_FragCoord.xy, pGICascade ), 0 );
}
