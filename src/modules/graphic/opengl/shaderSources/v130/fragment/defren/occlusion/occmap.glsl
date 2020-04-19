precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"

UBOLAYOUT uniform RenderParameters{
	mat4 pMatrixVP[ 6 ];
	mat4x3 pMatrixV[ 6 ];
	vec4 pTransformZ[ 6 ];
	vec2 pZToDepth;
};

#ifdef PERSPECTIVE_TO_LINEAR
in float vDepth;
#endif
#ifdef DEPTH_DISTANCE
in vec3 vPosition;
#endif

void main( void ){
	#ifdef PERSPECTIVE_TO_LINEAR
	gl_FragDepth = vDepth;
	#endif
	#ifdef DEPTH_DISTANCE
	gl_FragDepth = length( vPosition ) * pZToDepth.x + pZToDepth.y;
	#endif
}
