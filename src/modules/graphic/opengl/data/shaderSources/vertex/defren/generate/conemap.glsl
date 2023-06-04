precision highp float;
precision highp int;

uniform int pWidth;
uniform vec4 pSrcTCTransform;
uniform vec4 pDestTCTransform;

in vec2 inPosition;

out vec2 vTexCoord;
out vec2 vDestTC;

const vec2 tcTransform = vec2( 0.5 );

void main( void ){
	gl_Position = vec4( inPosition, 0.0, 1.0 );
	vTexCoord = inPosition * pSrcTCTransform.xy + pSrcTCTransform.zw;
	vDestTC = vec2( ivec2( gl_InstanceID % pWidth, gl_InstanceID / pWidth ) ) * pDestTCTransform.xy + pDestTCTransform.zw;
}
