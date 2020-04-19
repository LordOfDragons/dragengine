precision highp float;
precision highp int;

uniform vec4 pPosTransform;

in vec3 inPosition;

out vec3 vScreenCoord;



void main( void ){
	gl_Position = vec4( inPosition, 1.0 );
	vScreenCoord = vec3( inPosition.xy * pPosTransform.zw, 1.0 );
}
