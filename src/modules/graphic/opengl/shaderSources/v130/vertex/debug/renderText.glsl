precision mediump float;
precision mediump int;

uniform vec4 pPosTransform; // scaleX, scaleY, offsetX, offsetY
uniform vec4 pTCTransform; // scaleX, scaleY, offsetX, offsetY

in vec2 inPosition;
in vec2 inTexCoord;
in vec4 inColor;

out vec4 vColor;
out vec2 vTexCoord;

void main( void ){
	gl_Position = vec4( inPosition, 0.0, 1.0 );
	vTexCoord = inTexCoord;
	vColor = inColor;
}
