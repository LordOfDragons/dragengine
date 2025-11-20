#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"
// #include "shared/defren/sanitize_light.glsl"

UNIFORM_BIND(3) uniform vec4 pTCBloomTransform;
UNIFORM_BIND(4) uniform vec2 pTCBloomClamp;

layout(binding=0) uniform mediump sampler2DArray texColor;
layout(binding=1) uniform HIGHP sampler2D texToneMapParams;
layout(binding=2) uniform mediump sampler2DArray texBloom;

// WithToneMapCurve
layout(binding=3) uniform HIGHP sampler2D texToneMapCurve;

#include "shared/interface/2d/fragment.glsl"

layout(location=0) out vec4 outColor;

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
	
	float w0 = 1.0 - smoothstep(0.0, m, x);
	float w2 = step(m + l0, x);
	float w1 = 1.0 - w0 - w2;
	
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
	
	float w0 = 1.0 - smoothstep(0.0, m, x);
	float w2 = step(m + l0, x);
	float w1 = 1.0 - w0 - w2;
	
	float T = m * pow(x / m, c);
	float S = p - (p - S01) * exp(CP * (x - S01));
	
	return T * w0 + x * w1 + S01 * w2;
}
*/

float uchimura(float x) {
	// original parameters
	/*
	VARCONST float P = 1.0;  // max display brightness [1..100]
	VARCONST float a = 1.0;  // contrast [0..5]
	VARCONST float m = 0.22; // linear section start [0..1]
	VARCONST float l = 0.4;  // linear section length [0..1]
	VARCONST float c = 1.33; // black [1..3]
	VARCONST float b = 0.0;  // pedestal [0..1]
	
	return uchimura(x, P, a, m, l, c, b);
	*/
	
	// modified parameters to flatten the excessive white curve.
	// this avoids colors washing out at the top end
	VARCONST float P = 1.0;  // max display brightness [1..100]
	VARCONST float a = 1.0;  // contrast [0..5]
	VARCONST float m = 0.1;  // linear section start
	VARCONST float l = 0.4; //0.0;  // linear section length [0..1]
	VARCONST float c = 1.3;  // black
	VARCONST float b = 0.0;  // pedestal [0..1]
	
	//return uchimuraModified(x, p, m, c);
	return uchimura(x, P, a, m, l, c, b);
}

void main( void ){
	ivec3 tc = ivec3(gl_FragCoord.xy, vLayer);
	
	vec3 params = texelFetch(texToneMapParams, tcParams, 0).rgb; // r=avgLum, g=scaleLum, b=lwhite, a=n/a
	vec4 color = texelFetch(texColor, tc, 0);
	
// 	color.rgb = sanitizeLight(color.rgb); // final safe guard
	
	vec2 tcBloom = min(vTexCoord * pTCBloomTransform.xy + pTCBloomTransform.zw, pTCBloomClamp);
	
	// classic reinhard
//	float luminance = dot(color, lumiFactors) * pToneMapBloomBlend;
//	luminance = luminance / (1.0 + luminance);
	
	// enhanced reinhard
// 	color.rgb += textureLod(texBloom, vec3(tcBloom, vLayer), 0.0).rgb * vec3(pToneMapBloomBlend);
// 	
// 	float luminance = dot(color.rgb, lumiFactors);
// 	float adjLum = luminance * params.g;
// 	float finalLum = adjLum * (1.0 + adjLum * params.b) / (1.0 + adjLum);
// 	outColor = vec4(color.rgb * vec3(finalLum / (luminance + epsilon)), color.a);
	
	// uchimura, applied per channel
	color.rgb *= params.g;
	
	outColor.a = color.a;
	
	if(WithToneMapCurve){
		outColor.r = texture(texToneMapCurve, vec2(color.r, 0.0)).r;
		outColor.g = texture(texToneMapCurve, vec2(color.g, 0.0)).r;
		outColor.b = texture(texToneMapCurve, vec2(color.b, 0.0)).r;
		
	}else{
		outColor.r = uchimura(color.r);
		outColor.g = uchimura(color.g);
		outColor.b = uchimura(color.b);
	}
	
	// bloom
	outColor.rgb += textureLod(texBloom, vec3(tcBloom, vLayer), 0.0).rgb * vec3(pToneMapBloomBlend);
	
	// NOTE nice comparison of curves: https://www.shadertoy.com/view/WdjSW3
	//      the uchimura curve is the only one which has a linear left side. all others try
	//      to add burning out the blacks which is annoying. the tone map operator should
	//      provide a balance output. if an artist wants burning out blacks he should do
	//      so after the tone mapping is done (still float buffer)
}
