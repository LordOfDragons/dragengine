precision highp float;
precision highp int;

uniform mediump sampler2DArray texColor;

in vec2 vTexCoord;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

out vec3 outColor;

void main( void ){
	outColor = textureLod( texColor, vec3( vTexCoord, vLayer ), 0 ).rgb;
}
