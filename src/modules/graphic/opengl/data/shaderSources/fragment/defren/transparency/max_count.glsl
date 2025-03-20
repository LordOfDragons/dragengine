precision HIGHP float;
precision HIGHP int;

uniform mediump ivec4 pClampTC; // minU, minV, maxU, maxV
uniform lowp ivec4 pOffsets1; // off1.x, off1.y, off2.x, off2.y
uniform lowp ivec4 pOffsets2; // off3.x, off3.y, off4.x, off4.y
uniform lowp ivec4 pOffsets3; // off5.x, off5.y, off6.x, off6.y
uniform lowp ivec4 pOffsets4; // off7.x, off7.y, scaleX, scaleY

uniform lowp sampler2DArray texValues;

in mediump vec2 vTexCoord;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

out lowp float outValue;

void main( void ){
	// we are taking 8 taps. this way we can determine the maximum
	// by only using 3 max instructions. the tc variable is only used
	// to reduce the number of clamping calls
	ivec2 baseTC = ivec2( gl_FragCoord.xy ) * pOffsets4.zw;
	vec4 values1, values2;
	ivec4 tc;
	
	values1.x = texelFetch( texValues, ivec3( baseTC, vLayer ), 0 ).r;
	tc = min( baseTC.xyxy + pOffsets1, pClampTC.zwzw );
	values1.y = texelFetch( texValues, ivec3( tc.xy, vLayer ), 0 ).r; // vTexCoord + pOffsets1.xy
	values1.z = texelFetch( texValues, ivec3( tc.zw, vLayer ), 0 ).r; // vTexCoord + pOffsets1.zw
	tc = min( baseTC.xyxy + pOffsets2, pClampTC.zwzw );
	values1.w = texelFetch( texValues, ivec3( tc.xy, vLayer ), 0 ).r; // vTexCoord + pOffsets2.xy
	values2.x = texelFetch( texValues, ivec3( tc.zw, vLayer ), 0 ).r; // vTexCoord + pOffsets2.zw
	tc = min( baseTC.xyxy + pOffsets3, pClampTC.zwzw );
	values2.y = texelFetch( texValues, ivec3( tc.xy, vLayer ), 0 ).r; // vTexCoord + pOffsets3.xy
	values2.z = texelFetch( texValues, ivec3( tc.zw, vLayer ), 0 ).r; // vTexCoord + pOffsets3.zw
	values2.w = texelFetch( texValues, ivec3( min( baseTC + pOffsets4.xy, pClampTC.zw ), vLayer ), 0 ).r;
	
	values1 = max( values1, values2 );
	values1.xy = max( values1.xy, values1.zw );
	outValue = max( values1.x, values1.y );
	
	// NOTE: another solution with even more throughput would be using fetch4.
	// using fetch4 8 blocks of 4 pixels each could be tapped and maxed one
	// by one as vec4 values.
}
