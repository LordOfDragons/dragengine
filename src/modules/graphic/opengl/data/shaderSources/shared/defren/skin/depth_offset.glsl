/**
 * Apply depth offset.
 */
void applyDepthOffset( in int layer, in vec3 normal, in bool doubleSided ){
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
	
	#ifdef DEPTH_ORTHOGONAL
		bool frontFacing = normal.z < 0 || doubleSided;
		vec2 depthOffset = frontFacing ? pDepthOffset[ layer ].xy : pDepthOffset[ layer ].zw;
	#else
		vec2 depthOffset = pDepthOffset[ layer ].xy;
	#endif
	
	float depthScale = depthOffset.x * depthSlope;
	
	#ifdef DEPTH_ORTHOGONAL
		gl_Position.z += depthScale + depthOffset.y;
	#elif defined DEPTH_DISTANCE
// 		float dotNormal = dot( normal, normalize( vPosition ) );
// 		float scale = depthOffset.x * ( 1 - abs( dotNormal ) );
		vPosition.z += vPosition.z * depthScale + depthOffset.y;
	#else
		gl_Position.z += gl_Position.z * depthScale + gl_Position.w * depthOffset.y;
	#endif
}

void applyDepthOffset( in int layer ){
	float depthBias = pDepthOffset[ layer ].y;
	
	#ifdef DEPTH_ORTHOGONAL
		gl_Position.z += depthBias;
	#elif defined DEPTH_DISTANCE
		vPosition.z += depthBias;
	#else
		gl_Position.z += gl_Position.w * depthBias;
	#endif
}
