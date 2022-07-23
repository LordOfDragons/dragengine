precision highp float;
precision highp int;

uniform ivec2 pTCClamp;
#ifdef DOWNSAMPLE
	uniform int pMipMapLevel;
#endif

uniform HIGHP sampler2DArray texData;

#ifdef GS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

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
			data1.x = dot( texelFetch( texData, ivec3( tc.xy, vLayer ), 0 ).rgb, unpackDepth );
			data1.y = dot( texelFetch( texData, ivec3( tc.zy, vLayer ), 0 ).rgb, unpackDepth );
			data1.z = dot( texelFetch( texData, ivec3( tc.xw, vLayer ), 0 ).rgb, unpackDepth );
			data1.w = dot( texelFetch( texData, ivec3( tc.zw, vLayer ), 0 ).rgb, unpackDepth );
		#else
			data1.x = texelFetch( texData, ivec3( tc.xy, vLayer ), 0 ).r; // (s*2, t*2)
			data1.y = texelFetch( texData, ivec3( tc.zy, vLayer ), 0 ).r; // (s*2+1, t*2)
			data1.z = texelFetch( texData, ivec3( tc.xw, vLayer ), 0 ).r; // (s*2, t*2+1)
			data1.w = texelFetch( texData, ivec3( tc.zw, vLayer ), 0 ).r; // (s*2+1, t*2+1)
		#endif
		
		data2.xz = min( data1.xy, data1.zw );
		data2.yw = max( data1.xy, data1.zw );
		
	#elif DOWNSAMPLE
		data1.xy = texelFetch( texData, ivec3( tc.xy, vLayer ), pMipMapLevel ).rg; // (s*2, t*2)
		data1.zw = texelFetch( texData, ivec3( tc.zy, vLayer ), pMipMapLevel ).rg; // (s*2+1, t*2)
		data2.xy = texelFetch( texData, ivec3( tc.xw, vLayer ), pMipMapLevel ).rg; // (s*2, t*2+1)
		data2.zw = texelFetch( texData, ivec3( tc.zw, vLayer ), pMipMapLevel ).rg; // (s*2+1, t*2+1)
		
		data2.xz = min( data1.xz, data2.xz );
		data2.yw = max( data1.yw, data2.yw );
	#endif
	
	outData.x = min( data2.x, data2.z );
	outData.y = max( data2.y, data2.w );
}
