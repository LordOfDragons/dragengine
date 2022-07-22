precision highp float;
precision highp int;

uniform vec2 pOptions;

uniform mediump sampler2DArray texColor;
uniform mediump sampler2D texToneMapParams;

in vec2 vTexCoord;

#ifdef GS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

out vec3 outColor;

const float epsilon = 0.0001;
const ivec2 tcParams = ivec2( 0 );
const vec3 lumiFactors = vec3( 0.2125, 0.7154, 0.0721 );
//const vec3 lumiFactors = vec3( 0.3086, 0.6094, 0.0820 ); // nVidia

void main( void ){
	vec4 params = texelFetch( texToneMapParams, tcParams, 0 ); // r=avgLum, g=imageKey, b=lwhite, a=brightPassThreshold
	vec3 color = textureLod( texColor, vec3( vTexCoord, vLayer ), 0 ).rgb;
	
	float luminance = dot( color, lumiFactors );
	float finalLum = max( luminance - params.w, 0 );
	outColor = color * vec3( finalLum / ( luminance + epsilon ) );
	
	/*
	float luminance = dot( color, lumiFactors );
	float finalLum = max( luminance - params.x, 0.0 );
	outColor = color * vec3( finalLum / ( luminance + epsilon ) );
	*/
	
	/*
	float adjLum = luminance * params.g;
	float finalLum = adjLum * ( 1.0 + adjLum * params.b ) / ( 1.0 + adjLum );
	
	color *= vec3( finalLum / ( luminance + epsilon ) );
	
	outColor = max( color - vec3( pOptions.x ), c0_0 );
	*/
	//color = max( color * vec3( params.g - pOptions.x ), c0_0 );
	//outColor = color / ( color + vec3( pOptions.y ) );
}
