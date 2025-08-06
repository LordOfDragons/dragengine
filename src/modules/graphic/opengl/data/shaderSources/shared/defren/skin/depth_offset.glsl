/*
	<!-- shared/defren/skin/depth_offset.glsl -->
	<define>DEPTH_DISTANCE</define>
	<define>DEPTH_ORTHOGONAL</define>
*/

#ifndef GET_DEPTH_OFFSET_VALUE
	#define GET_DEPTH_OFFSET_VALUE(layer) pDepthOffset[layer]
#endif

void calcDepthOffset(const in int layer, const in vec3 normal, const in bool doubleSided,
out float depthScale, out vec2 depthOffset){
	// WARNING "normal" is NOT a good value for front/back face calculation. this normal is
	//         not the face normal but a smoothed normal (by normal index). if faces are flat
	//         with hard edges this will work but for faces with normals shared across (round
	//         surfaces) this can fail by shifting one face corner backward and another forward.
	//         
	//         the real solution would required to store the face normal in the VBO and using
	//         it to do the calculation. adding more data to the VBO just for this calculation
	//         is not a good thing.
	//         
	//         the other solution would be to use geometry shader since there face orientation
	//         can be calculated. this though reduces performance and is thus not a good thing.
	//         
	//         last solution is to go back again to do the calculation in the fragment shader.
	//         this disabled early z buffer and double depth write which is also a bad thing.
	//         
	//         solution pending... or rather... picking the lesser evil.
	
	float depthSlope = length( normal.xy ) / max( abs( normal.z ), 0.01 );
	
	if(DepthOrthogonal){
		bool frontFacing = normal.z < 0.0 || doubleSided;
		depthOffset = frontFacing ? GET_DEPTH_OFFSET_VALUE(layer).xy : GET_DEPTH_OFFSET_VALUE(layer).zw;
		
	}else{
		depthOffset = GET_DEPTH_OFFSET_VALUE(layer).xy;
	}
	
	depthScale = depthOffset.x * depthSlope;
}

/**
 * Apply depth offset.
 */
void applyDepthOffset(const in int layer, const in vec3 normal,
const in bool doubleSided){
	vec2 depthOffset;
	float depthScale;
	calcDepthOffset(layer, normal, doubleSided, depthScale, depthOffset);
	
	if(DepthOrthogonal){
		gl_Position.z += depthScale + depthOffset.y;
		
	}else{
		gl_Position.z += gl_Position.z * depthScale + gl_Position.w * depthOffset.y;
	}
}

void applyDepthOffset(const in int layer, const in vec3 normal,
const in bool doubleSided, inout float positionz){
	vec2 depthOffset;
	float depthScale;
	calcDepthOffset(layer, normal, doubleSided, depthScale, depthOffset);
	
	if(DepthOrthogonal){
		gl_Position.z += depthScale + depthOffset.y;
		
	}else{
// 		float dotNormal = dot( normal, normalize( position ) );
// 		float scale = depthOffset.x * ( 1 - abs( dotNormal ) );
		positionz += positionz * depthScale + depthOffset.y;
	}
}

void applyDepthOffset(const in int layer){
	float depthBias = GET_DEPTH_OFFSET_VALUE(layer).y;
	
	if(DepthOrthogonal){
		gl_Position.z += depthBias;
		
	}else{
		gl_Position.z += gl_Position.w * depthBias;
	}
}

void applyDepthOffset(const in int layer, inout float positionz){
	float depthBias = GET_DEPTH_OFFSET_VALUE(layer).y;
	
	if(DepthOrthogonal){
		gl_Position.z += depthBias;
		
	}else{
		positionz += depthBias;
	}
}
