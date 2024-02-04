precision highp float;
precision highp int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"
// #include "shared/defren/sanitize_light.glsl"

uniform vec4 pTCBloomTransform;
uniform vec2 pTCBloomClamp;

uniform mediump sampler2DArray texColor;
uniform mediump sampler2D texToneMapParams;
uniform mediump sampler2DArray texBloom;
#ifdef WITH_TONEMAP_CURVE
uniform mediump sampler2D texToneMapCurve;
#endif

in vec2 vTexCoord;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

out vec4 outColor;

const float epsilon = 0.0001;
const ivec2 tcParams = ivec2( 0, 0 );
const vec3 lumiFactors = vec3( 0.2125, 0.7154, 0.0721 );
//const vec3 lumiFactors = vec3( 0.3086, 0.6094, 0.0820 ); // nVidia

float uchimura(float x, float P, float a, float m, float l, float c, float b) {
	// Uchimura 2017, "HDR theory and practice"
	// Math: https://www.desmos.com/calculator/gslcdxvipg
	// Source: https://www.slideshare.net/nikuque/hdr-theory-and-practicce-jp
	float l0 = ((P - m) * l) / a;
	//float L0 = m - m / a; // unused
	//float L1 = m + (1.0 - m) / a; // unused
	float S0 = m + l0;
	float S1 = m + a * l0;
	float C2 = (a * P) / (P - S1);
	float CP = -C2 / P;
	
	float w0 = 1 - smoothstep(0, m, x);
	float w2 = step(m + l0, x);
	float w1 = 1 - w0 - w2;
	
	float T = m * pow(x / m, c) + b;
	float S = P - (P - S1) * exp(CP * (x - S0));
	float L = m + a * (x - m);
	
	return T * w0 + L * w1 + S * w2;
}

/*
float uchimuraModified(float x, float p, float m, float c) {
	float l0 = p - m;
	float S01 = m + l0;
	float C2 = p / (p - S01);
	float CP = -C2 / p;
	
	float w0 = 1 - smoothstep(0, m, x);
	float w2 = step(m + l0, x);
	float w1 = 1 - w0 - w2;
	
	float T = m * pow(x / m, c);
	float S = p - (p - S01) * exp(CP * (x - S01));
	
	return T * w0 + x * w1 + S01 * w2;
}
*/

float uchimura(float x) {
	// original parameters
	/*
	const float P = 1.0;  // max display brightness [1..100]
	const float a = 1.0;  // contrast [0..5]
	const float m = 0.22; // linear section start [0..1]
	const float l = 0.4;  // linear section length [0..1]
	const float c = 1.33; // black [1..3]
	const float b = 0.0;  // pedestal [0..1]
	
	return uchimura(x, P, a, m, l, c, b);
	*/
	
	// modified parameters to flatten the excessive white curve.
	// this avoids colors washing out at the top end
	const float P = 1;  // max display brightness [1..100]
	const float a = 1;  // contrast [0..5]
	const float m = 0.1;  // linear section start
	const float l = 0.4; //0;  // linear section length [0..1]
	const float c = 1.3;  // black
	const float b = 0;  // pedestal [0..1]
	
	//return uchimuraModified(x, p, m, c);
	return uchimura(x, P, a, m, l, c, b);
}

void main( void ){
	ivec3 tc = ivec3( gl_FragCoord.xy, vLayer );
	
	vec3 params = texelFetch( texToneMapParams, tcParams, 0 ).rgb; // r=avgLum, g=scaleLum, b=lwhite, a=n/a
	vec4 color = texelFetch( texColor, tc, 0 );
	
// 	color.rgb = sanitizeLight( color.rgb ); // final safe guard
	
	vec2 tcBloom = min( vTexCoord * pTCBloomTransform.xy + pTCBloomTransform.zw, pTCBloomClamp );
	
	// classic reinhard
//	float luminance = dot( color, lumiFactors ) * pToneMapBloomBlend;
//	luminance = luminance / ( 1.0 + luminance );
	
	// enhanced reinhard
// 	color.rgb += textureLod( texBloom, vec3( tcBloom, vLayer ), 0 ).rgb * vec3( pToneMapBloomBlend );
// 	
// 	float luminance = dot( color.rgb, lumiFactors );
// 	float adjLum = luminance * params.g;
// 	float finalLum = adjLum * ( 1.0 + adjLum * params.b ) / ( 1.0 + adjLum );
// 	outColor = vec4( color.rgb * vec3( finalLum / ( luminance + epsilon ) ), color.a );
	
	// uchimura, applied per channel
	color *= params.g;
	
	outColor.a = color.a;
	
	#ifdef WITH_TONEMAP_CURVE
		outColor.r = texture( texToneMapCurve, vec2( color.r, 0 ) ).r;
		outColor.g = texture( texToneMapCurve, vec2( color.g, 0 ) ).r;
		outColor.b = texture( texToneMapCurve, vec2( color.b, 0 ) ).r;
	#else
		outColor.r = uchimura( color.r );
		outColor.g = uchimura( color.g );
		outColor.b = uchimura( color.b );
	#endif
	
	// bloom
	outColor.rgb += textureLod( texBloom, vec3( tcBloom, vLayer ), 0 ).rgb * vec3( pToneMapBloomBlend );
	
	// NOTE nice comparison of curves: https://www.shadertoy.com/view/WdjSW3
	//      the uchimura curve is the only one which has a linear left side. all others try
	//      to add burning out the blacks which is annoying. the tone map operator should
	//      provide a balance output. if an artist wants burning out blacks he should do
	//      so after the tone mapping is done (still float buffer)
}
