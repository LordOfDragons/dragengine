#version 450

layout (location = 0) in vec3 vColor;

layout (location = 0) out vec4 outColor;

void main() {
	//outColor = vec4( gl_FragCoord.x / 63, gl_FragCoord.y / 31, 0, 1 );
// 	outColor = vec4( gl_FragCoord.y / 31, 0, 0, 1 );
// 	outColor = vec4(1, 0, 0, 1);
	outColor = vec4( vColor, 1 );
}
