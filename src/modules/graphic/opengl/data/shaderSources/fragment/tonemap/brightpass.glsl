precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"

uniform mediump sampler2DArray texColor;
uniform HIGHP sampler2D texToneMapParams;

in vec2 vTexCoord;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

out vec3 outColor;

const float epsilon = 0.0001;
const ivec2 tcParams = ivec2( 0 );
const vec3 lumiFactors = vec3( 0.2125, 0.7154, 0.0721 );
//const vec3 lumiFactors = vec3( 0.3086, 0.6094, 0.0820 ); // nVidia

const vec2 options = vec2( 2.5, 1.0 ); // m$: 5, 10 // ogre: 2.5, 1

void main( void ){
	vec4 params = texelFetch( texToneMapParams, tcParams, 0 ); // r=avgLum, g=imageKey, b=lwhite, a=brightPassThreshold
	vec3 color = textureLod( texColor, vec3( vTexCoord, vLayer ), 0.0 ).rgb;
	
	float luminance = max( dot( color, lumiFactors ), 0.0 );
	float finalLum = max( luminance - params.w, 0.0 ) * pToneMapBloomStrength;
	outColor = color * vec3( finalLum / ( luminance + epsilon ) );
	
	/*
	float luminance = max( dot( color, lumiFactors ), 0.0 );
	float finalLum = max( luminance - params.x, 0.0 );
	outColor = color * vec3( finalLum / ( luminance + epsilon ) );
	*/
	
	/*
	float adjLum = luminance * params.g;
	float finalLum = adjLum * ( 1.0 + adjLum * params.b ) / ( 1.0 + adjLum );
	
	color *= vec3( finalLum / ( luminance + epsilon ) );
	
	outColor = max( color - vec3( options.x ), c0_0 );
	*/
	//color = max( color * vec3( params.g - options.x ), c0_0 );
	//outColor = color / ( color + vec3( options.y ) );
}
