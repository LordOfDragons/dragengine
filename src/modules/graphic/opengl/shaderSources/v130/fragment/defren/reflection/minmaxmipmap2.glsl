precision highp float;
precision highp int;

uniform ivec2 pTCClamp;
#ifdef DOWNSAMPLE
	uniform int pMipMapLevel;
#endif

uniform HIGHP sampler2D texData;

out vec2 outData;

#ifdef DECODE_IN_DEPTH
	const vec3 unpackDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );
#endif
const ivec4 tcScale = ivec4( 2 );
const ivec4 tcOffset = ivec4( 0, 0, 1, 1 );

void main( void ){
	ivec4 tc = min( ivec4( gl_FragCoord.xyxy ) * tcScale + tcOffset, pTCClamp.xyxy ); // s*2, t*2, s*2+1, t*2+1
	vec4 data1, data2;
	
	#ifdef INITIAL
		#ifdef DECODE_IN_DEPTH
			data1.x = dot( texelFetch( texData, tc.xy, 0 ).rgb, unpackDepth );
			data1.y = dot( texelFetch( texData, tc.zy, 0 ).rgb, unpackDepth );
			data1.z = dot( texelFetch( texData, tc.xw, 0 ).rgb, unpackDepth );
			data1.w = dot( texelFetch( texData, tc.zw, 0 ).rgb, unpackDepth );
		#else
			data1.x = texelFetch( texData, tc.xy, 0 ).r; // (s*2, t*2)
			data1.y = texelFetch( texData, tc.zy, 0 ).r; // (s*2+1, t*2)
			data1.z = texelFetch( texData, tc.xw, 0 ).r; // (s*2, t*2+1)
			data1.w = texelFetch( texData, tc.zw, 0 ).r; // (s*2+1, t*2+1)
		#endif
		
		data2.xz = min( data1.xy, data1.zw );
		data2.yw = max( data1.xy, data1.zw );
		
	#elif DOWNSAMPLE
		data1.xy = texelFetch( texData, tc.xy, pMipMapLevel ).rg; // (s*2, t*2)
		data1.zw = texelFetch( texData, tc.zy, pMipMapLevel ).rg; // (s*2+1, t*2)
		data2.xy = texelFetch( texData, tc.xw, pMipMapLevel ).rg; // (s*2, t*2+1)
		data2.zw = texelFetch( texData, tc.zw, pMipMapLevel ).rg; // (s*2+1, t*2+1)
		
		data2.xz = min( data1.xz, data2.xz );
		data2.yw = max( data1.yw, data2.yw );
	#endif
	
	outData.x = min( data2.x, data2.z );
	outData.y = max( data2.y, data2.w );
}
