precision mediump float;
precision mediump int;

uniform vec4 pShifts; // shift1U, shift1V, shift2U, shift3U
uniform vec3 pWeights; // weight0, weight1, weight2
uniform vec4 pClip; // minU, minV, maxU, maxV

uniform mediump sampler2D texSource;

in vec2 vTexCoord;

out vec4 outColor;

void main( void ){
	vec2 base = clamp( vTexCoord, pClip.xy, pClip.zw );
	vec2 pt1 = clamp( vTexCoord + pShifts.xy, pClip.xy, pClip.zw );
	vec2 pt2 = clamp( vTexCoord + pShifts.zw, pClip.xy, pClip.zw );
	
	outColor = vec4( texture( texSource, base ).rgb * vec3( pWeights.x ), 1.0 );
	outColor.rgb += texture( texSource, pt1 ).rgb * vec3( pWeights.y );
	outColor.rgb += texture( texSource, pt1 ).rgb * vec3( pWeights.z );
}
