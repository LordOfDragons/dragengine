precision highp float;
precision highp int;

uniform vec4 pPosTransform;

uniform HIGHP sampler2D texDepth;

in vec3 vScreenCoord; // vec3( inPosition.xy * pPosTransform.zw, 1.0 )

out vec3 outPosition;

#ifdef DECODE_IN_DEPTH
	const vec3 unpackDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );
#endif



void main( void ){
	#ifdef DECODE_IN_DEPTH
		float depth = dot( texelFetch( texDepth, ivec2( gl_FragCoord.xy ), 0 ).rgb, unpackDepth );
	#else
		float depth = texelFetch( texDepth, ivec2( gl_FragCoord.xy ), 0 ).r;
	#endif
	
	outPosition = vScreenCoord * vec3( pPosTransform.x / ( pPosTransform.y - depth ) );
}
