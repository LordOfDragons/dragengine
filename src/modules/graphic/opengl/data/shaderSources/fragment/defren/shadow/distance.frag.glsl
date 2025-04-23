precision HIGHP float;
precision HIGHP int;

#if defined( TRANSPARENT ) || defined( DISCARD_HOLES )
#define _USE_TEXTURE_
#endif

uniform vec4 pMaterialGamma;
uniform vec4 pOptions; // depth-scaling

#ifdef _USE_TEXTURE_
uniform mediump sampler2D texColor;
#endif

#ifdef _USE_TEXTURE_
in vec2 vTexCoord;
#endif
in vec3 vLSPosition;

#if defined( DEPTH_AS_COLOR ) || defined( TRANSPARENT )
layout(location=0) out vec4 outBuf1;
#endif
#if defined( DEPTH_AS_COLOR ) && defined( TRANSPARENT )
layout(location=1) out vec4 outBuf2;
#endif

#ifdef DEPTH_AS_COLOR
//const vec4 packShift = vec4( 1.0, 256.0, 65536.0, 16777216.0 );
//const vec4 packMask = vec4( 1.0 / 256.0, 1.0 / 256.0, 1.0 / 256.0, 0.0 );
const vec3 packShift = vec3( 1.0, 256.0, 65536.0 );
const vec3 packMask = vec3( 1.0 / 256.0, 1.0 / 256.0, 0.0 );
#endif // DEPTH_AS_COLOR

// 32-bit: depth - ( depth.yzww * packMask );
// 24-bit: depth - ( depth.yzz * packMask );

void main( void ){
	float ddepth = length( vLSPosition ) * pOptions.x + pOptions.y;
	vec2 dderiv = vec2( dFdx( ddepth ), dFdy( ddepth ) );
	
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
	
	//gl_FragDepth = clamp( length( vLSPosition ) * pOptions.x + pOptions.y, 0.0, 1.0 );
//	gl_FragDepth = length( vLSPosition ) * pOptions.x + pOptions.y;
	gl_FragDepth = ddepth + ( length( dderiv ) * pOptions.z + pOptions.w );
	
#ifdef DEPTH_AS_COLOR
	vec3 depth = fract( packShift * gl_FragDepth );
	outBuf1 = vec4( depth - ( depth.yzz * packMask ), 0.0 );
#endif // DEPTH_AS_COLOR
}
