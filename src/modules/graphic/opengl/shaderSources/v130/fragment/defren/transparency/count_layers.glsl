precision mediump float;
precision mediump int;

uniform vec4 pClipPlane; // normal.xyz, distance

uniform lowp sampler2DArray texDiffuse;

#ifdef USE_CLIP_PLANE
in vec3 vClipCoord;
#endif

#ifdef GS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

out lowp float outValue;

const float valueOne = 1.0 / 255.0;

void main( void ){
#ifdef USE_CLIP_PLANE
	if( dot( vClipCoord, pClipPlane.xyz ) <= pClipPlane.w ) discard;
#endif
	
	outValue = valueOne;
}
