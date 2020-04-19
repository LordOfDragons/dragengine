precision highp float;
precision highp int;

uniform mat4 pMatrixMVP;
uniform mat4x3 pMatrixMV;

in vec3 inPosition;

out vec3 vLightVolumePos;

void main( void ){
	gl_Position = pMatrixMVP * vec4( inPosition, 1.0 );
	vLightVolumePos = pMatrixMV * vec4( inPosition, 1.0 );
}
