#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#if defined( TRANSPARENT ) || defined( DISCARD_HOLES )
#define _USE_TEXTURE_
#endif

uniform vec4 pMaterialGamma;
uniform vec4 pOptions; // scale, offset
uniform vec3 pMask; // scale, first, second

#ifdef _USE_TEXTURE_
layout(binding=0) uniform mediump sampler2D texColor;
#endif

#ifdef _USE_TEXTURE_
VARYING_BIND(0) in vec2 vTexCoord;
#endif
in vec3 vLSPosition;

#if defined( DEPTH_AS_COLOR ) || defined( TRANSPARENT )
layout(location=0) out vec4 outBuf1;
#endif
#if defined( DEPTH_AS_COLOR ) && defined( TRANSPARENT )
layout(location=1) out vec4 outBuf2;
#endif

const vec2 maskShift = vec2( -0.5, -0.5 );

#ifdef DEPTH_AS_COLOR
const vec3 packShift = vec3( 1.0, 256.0, 65536.0 );
const vec3 packMask = vec3( 1.0 / 256.0, 1.0 / 256.0, 0.0 );
#endif // DEPTH_AS_COLOR

void main( void ){
	float ddepth = length( vLSPosition ) * pOptions.x + pOptions.y;
	vec2 dderiv = vec2( dFdx( ddepth ), dFdy( ddepth ) );
	
	vec2 maskCoord = gl_FragCoord.xy * pMask.xx + maskShift;
	if( dot( maskCoord, pMask.yz ) < abs( dot( maskCoord, pMask.zy ) ) ) discard;
	
#ifdef DISCARD_HOLES
	//if( texture( texColor, vTexCoord ).a < 0.5 ) discard;
	if( texture( texColor, vTexCoord ).a < 0.35 ) discard;
#elif defined( TRANSPARENT )
#ifdef DEPTH_AS_COLOR
	outBuf2 = pow( texture( texColor, vTexCoord ), pMaterialGamma );
#else // DEPTH_AS_COLOR
	outBuf1 = pow( texture( texColor, vTexCoord ), pMaterialGamma );
#endif // DEPTH_AS_COLOR
#endif // TRANSPARENT
	
//	gl_FragDepth = length( vLSPosition ) * pOptions.x + pOptions.y;
	gl_FragDepth = ddepth + ( length( dderiv ) * pOptions.z + pOptions.w );
	
#ifdef DEPTH_AS_COLOR
	vec3 depth = fract( packShift * gl_FragDepth );
	outBuf1.rgb = depth - ( depth.yzz * packMask );
#endif // DEPTH_AS_COLOR
}
