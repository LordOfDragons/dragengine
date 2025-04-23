precision HIGHP float;
precision HIGHP int;

uniform lowp sampler2DArray texAO;

in vec2 vTexCoord;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

layout(location=0) out vec3 outAO; // ao, ssao, solidity


void main( void ){
	outAO = vec3( textureLod( texAO, vec3( vTexCoord, vLayer ), 0.0 ) );
}
