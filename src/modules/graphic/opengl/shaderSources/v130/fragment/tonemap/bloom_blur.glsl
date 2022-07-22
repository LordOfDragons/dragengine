precision highp float;
precision highp int;

uniform vec4 pOffsets1; // offset1.s, offset1.t, offset2.s, offset2.t
uniform vec4 pOffsets2; // offset3.s, offset3.t, offset4.s, offset4.t
uniform vec4 pOffsets3; // offset5.s, offset5.t, offset6.s, offset6.t
uniform vec4 pOffsets4; // offset7.s, offset7.t, offset8.s, offset8.t
uniform vec4 pOffsets5; // offset9.s, offset9.t, offset10.s, offset10.t
uniform vec4 pWeights1; // weightCenter, weight1, weight2, weight3
uniform vec4 pWeights2; // weight4, weight5, n/a, n/a
uniform vec2 pClamp; // clamp.s, clamp.t

uniform mediump sampler2DArray texColor;

in vec2 vTexCoord;

#ifdef GS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

out vec3 outColor;

void main( void ){
	outColor = textureLod( texColor, vec3( vTexCoord, vLayer ), 0 ).rgb * vec3( pWeights1.x );
	
	vec4 tc = min( vTexCoord.stst + pOffsets1, pClamp.stst );
	outColor += textureLod( texColor, vec3( tc.xy, vLayer ), 0 ).rgb * vec3( pWeights1.y );
	outColor += textureLod( texColor, vec3( tc.zw, vLayer ), 0 ).rgb * vec3( pWeights1.y );
	
	tc = min( vTexCoord.stst + pOffsets2, pClamp.stst );
	outColor += textureLod( texColor, vec3( tc.xy, vLayer ), 0 ).rgb * vec3( pWeights1.z );
	outColor += textureLod( texColor, vec3( tc.zw, vLayer ), 0 ).rgb * vec3( pWeights1.z );
	
	tc = min( vTexCoord.stst + pOffsets3, pClamp.stst );
	outColor += textureLod( texColor, vec3( tc.xy, vLayer ), 0 ).rgb * vec3( pWeights1.w );
	outColor += textureLod( texColor, vec3( tc.zw, vLayer ), 0 ).rgb * vec3( pWeights1.w );
	
	tc = min( vTexCoord.stst + pOffsets4, pClamp.stst );
	outColor += textureLod( texColor, vec3( tc.xy, vLayer ), 0 ).rgb * vec3( pWeights2.x );
	outColor += textureLod( texColor, vec3( tc.zw, vLayer ), 0 ).rgb * vec3( pWeights2.x );
	
	tc = min( vTexCoord.stst + pOffsets5, pClamp.stst );
	outColor += textureLod( texColor, vec3( tc.xy, vLayer ), 0 ).rgb * vec3( pWeights2.y );
	outColor += textureLod( texColor, vec3( tc.zw, vLayer ), 0 ).rgb * vec3( pWeights2.y );
}
