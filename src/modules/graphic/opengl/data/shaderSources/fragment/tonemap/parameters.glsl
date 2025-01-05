precision highp float;
precision highp int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"

uniform vec4 pAvgLogLumTCs; // tc1.s, tc1.t, tc2.s, tc2.t

uniform mediump sampler2DArray texAvgLogLum;
uniform mediump sampler2D texLastParams;
uniform mediump samplerCube texEnvMap;

out vec4 outParams;

const ivec2 tcParameters = ivec2( 0, 0 );
const vec4 weightAvgLum = vec4( 0.25 );
const vec3 lumiFactors = vec3( 0.2125, 0.7154, 0.0721 );
//const vec3 lumiFactors = vec3( 0.3086, 0.6094, 0.0820 ); // nVidia

void main( void ){
	vec4 avgLums;
	
	avgLums.x = textureLod( texAvgLogLum, vec3( pAvgLogLumTCs.xy, 0 ), 0.0 ).r; // tc1.s, tc1.t
	avgLums.y = textureLod( texAvgLogLum, vec3( pAvgLogLumTCs.xw, 0 ), 0.0 ).r; // tc1.s, tc2.t
	avgLums.z = textureLod( texAvgLogLum, vec3( pAvgLogLumTCs.zy, 0 ), 0.0 ).r; // tc2.s, tc1.t
	avgLums.w = textureLod( texAvgLogLum, vec3( pAvgLogLumTCs.zw, 0 ), 0.0 ).r; // tc2.s, tc2.t
	
	#ifdef SAMPLE_STEREO
	avgLums.x += textureLod( texAvgLogLum, vec3( pAvgLogLumTCs.xy, 1 ), 0.0 ).r; // tc1.s, tc1.t
	avgLums.y += textureLod( texAvgLogLum, vec3( pAvgLogLumTCs.xw, 1 ), 0.0 ).r; // tc1.s, tc2.t
	avgLums.z += textureLod( texAvgLogLum, vec3( pAvgLogLumTCs.zy, 1 ), 0.0 ).r; // tc2.s, tc1.t
	avgLums.w += textureLod( texAvgLogLum, vec3( pAvgLogLumTCs.zw, 1 ), 0.0 ).r; // tc2.s, tc2.t
	avgLums /= vec4( 2 );
	#endif
	
	//vec3 envLight = texture( texEnvMap, vec3( 0.0, 1.0, 0.0 ) ).rgb;
	//float envLuminance = dot( envLight, lumiFactors );
	
	float averageLuminance = exp( dot( avgLums, weightAvgLum ) );
	vec4 lastParams = texelFetch( texLastParams, tcParameters, 0 );
	
	float lwhite = 1; // 1 / ( pToneMapWhiteScale * pToneMapWhiteScale );
	
	float ckey = 0.18; //0.27; // average constant key: ( 0.18 + 0.36 ) / 2
	
	// limit the luminance
	vec2 avglLimits = vec2( pToneMapLowLuminance, pToneMapHighLuminance ) * vec2( ckey );
	
	averageLuminance = clamp( averageLuminance, avglLimits.x, avglLimits.y );
	lastParams.x = clamp( lastParams.x, avglLimits.x, avglLimits.y ); // avoid long change if limits change
	
	// adjust luminance over time
	float adaptionFactor = pToneMapAdaptionTime;
	if( averageLuminance < lastParams.x ){
		adaptionFactor * 0.25; // hack: 4 times longer to adapt to darkness than to adapt to lightness
	}
	averageLuminance = max( mix( lastParams.x, averageLuminance, clamp( adaptionFactor, 0, 1 ) ), 1e-4 );
	
	// calculate the image key
//	float key = 1.03 - 2.0 / ( 2.0 + log( averageLuminance + 1.0 ) / log( 10.0 ) ); // paper
// 	ckey = 1.03 - 3 / ( 3 + log( averageLuminance + 1 ) / log( 10 ) );
	//float key = max( 0.0, 1.5 - 1.5 / ( averageLuminance * 0.1 + 1.0 ) ) + 0.1; // ogre
	float maxLum = max( averageLuminance / ckey, 0.01 );
	float scaleLum = 1 / maxLum;
	
	// adjust the image key using the user chosen exposure
	scaleLum *= pToneMapExposure;
	
	// apply white intensity scaling
	scaleLum *= pToneMapWhiteScale;
	
	// calculate the maximum white factor
	//float lwhite = 1.0 / ( scaleLum * scaleLum ); // = 1.0 / ( ( key / averageLuminance ) ** 2 )
	//gl_FragColor.b = 1.0 / ( ( 2.0 * scaleLum ) * ( 2.0 * scaleLum ) );
	//     float lwhite = 1.0 / ( pToneMapWhiteScale * pToneMapWhiteScale );
	//float lwhite = 1.0 / ( pToneMapHighLuminance * pToneMapHighLuminance * scaleLum * scaleLum );
	//float brightPassThreshold = ( 1.0 / scaleLum - 1.0 ) / ( scaleLum * lwhite - 1.0 );
	
	// 0.8 results in bright cut-off around 1.47 * camera intensity. this equals roughly averageLuminance in the
	// test case. higher values require higher intensities. 1.1 results in bright cut-off around 2 * camera
	// intensity. 1.6 results in bright cut-off around 3 * camera intensity. this can be exactly calculated using
	// a sample scene with camera intensity of 3 which equals white. 3*scaleLum = 0.542 , 6*scaleLum = 1.084
	// and 9*scaleLum = 1.626 . This results in a formula that cen be used to calculate the value from the white
	// value: brightPassThreshold = ( 0.542 * whitePoint )
	// 
	// w = math.sqrt( (l*s*s)/(1.0+l*s-s) ) / s
	// calculates which w is required for lwhite = 1.0 / ( w*s * w*s ) for l = camera-intensity to give white
	// 
	// intensity  3, al=0.783691, sl=0.180664,  key=0.141584931488 :  3*sl=0.542 , w=1.4845
	// intensity 10, al=3.72266,  sl=0.0757446, key=0.2819714683806: 10*sl=0.757446 , w=2.4385
	// intensity 0.1, al=0.00502396, sl=6.18359, key=0.0310722924064: 0.1*sl=0.618359 , w=inf
	// 
	// possible calculation for lwhite:
	//   w = key * 10.0 * scaleLum
	//   lwhite = 1.0 / ( w * w )
	float brightPassThreshold = maxLum * pToneMapBloomIntensity;
	
	// write parameters
	outParams = vec4( averageLuminance, scaleLum, lwhite, brightPassThreshold );
}
