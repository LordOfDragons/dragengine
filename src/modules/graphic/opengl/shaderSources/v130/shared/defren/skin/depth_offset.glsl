/**
 * Apply depth offset.
 */
void applyDepthOffset(){
	#ifdef DEPTH_ORTHOGONAL
		/*
		#ifdef GS_RENDER_CUBE
			vec2 depthOffset = mix( pDepthOffset[ 0 ].zw, pDepthOffset[ 0 ].xy,
				bvec2( gl_FrontFacing || pDoubleSided ) ); // mix( if-false, if-true, condition )
		#else
			vec2 depthOffset = mix( pDepthOffset[ inLayer ].zw, pDepthOffset[ inLayer ].xy,
				bvec2( gl_FrontFacing || pDoubleSided ) ); // mix( if-false, if-true, condition )
		#endif
		
		vZCoord += depthBias;
		*/
	#else
		#ifdef GS_RENDER_CUBE
			vec2 depthBias = pDepthOffset[ 0 ].xy;
		#else
			vec2 depthBias = pDepthOffset[ inLayer ].xy;
		#endif
		
		float depthSlope = length( vNormal.xy ) / max( abs( vNormal.z ), 0.01 );
		float depthScale = depthBias.x * depthSlope;
		
		#ifdef DEPTH_DISTANCE
// 			float dotNormal = dot( vNormal, normalize( vPosition ) );
// 			float scale = depthBias.x * ( 1 - abs( dotNormal ) );
			vPosition.z += vPosition.z * depthScale + depthBias.y;
		#else
			gl_Position.z += gl_Position.z * depthScale + gl_Position.w * depthBias.y;
		#endif
	#endif
}
