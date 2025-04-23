/*
	<!-- shared/defren/skin/relief_mapping.glsl -->
	<define>TEXTURE_HEIGHT</define>
	<define>TEXTURE_NORMAL</define>
	<define>TEXTURE_ENVMAP</define>
*/

// calculate new texture coordinates using relief mapping
#if defined TEXTURE_HEIGHT && defined TEXTURE_NORMAL && defined TEXTURE_ENVMAP
void reliefMapping( inout vec2 tc, in vec3 normal ){
	#define HEIGHT_VERSION_LINEAR 1
	//#define HEIGHT_VERSION_RELCONE 1
	
	// transform the view vector into the relief coordinate system
	int i;
	float scaleZ = 1.0 / max(pHeightRemap.x, 0.001);
	vec3 direction = vec3(
		dot( vTangent, vReflectDir ),
		dot( vBitangent, vReflectDir ),
		dot( normal, vReflectDir ) * scaleZ - pHeightRemap.y );
	
	direction /= -direction.z;
	#ifdef HEIGHT_VERSION_LINEAR
		//#define linearStepCount 30
		// map range from 1m to 20m, max 50 steps
		// 50.0 * ( vReflectDir.z - 1.0 ) / 20.0
		// f1 = 50.0 / 20.0 = 2.5
		// f2 = 50.0 * -1.0 / 20.0 = -2.5
		int linearStepCount = clamp(50 - int(vReflectDir.z * 2.5 - 2.5), 1, 50);
		direction /= float(linearStepCount);
	#endif
	
	// find first point of intersection
	#ifdef HEIGHT_VERSION_LINEAR
		vec3 end = vec3( tc, 1 ); // top
		for( i=0; i<linearStepCount; i++ ){
			end += direction;
			if(textureLod(texHeight, end.st, 0.0).r > end.z){
				break; // ran below surface
			}
		}
		
	#elif defined HEIGHT_VERSION_RELCONE
		#define coneStepCount 15
		float rayRatio = length( direction.xy );
		vec3 end = vec3( tc, 1 ); // top
		for( i=0; i<coneStepCount; i++ ){
			vec2 tap = textureLod(texHeight, end.st, 0.0).rg;
			end += direction * ( tap.y * clamp( end.z - tap.x, 0, 1 ) / max( rayRatio + tap.y, 0.001 ) );
		}
	#endif
	
	// refine using binary search
	#define binaryStepCount 6
	vec3 begin = end - direction;
	for( i=0; i<binaryStepCount; i++ ){
		direction *= vec3( 0.5 );
		vec3 test = begin + direction;
		if(textureLod(texHeight, test.st, 0.0).r > test.z){
			end = test;
		}else{
			begin = test;
		}
	}
	
	// update texture coordinates
	tc = end.st;
}

#else
#define reliefMapping(tc,normal)
#endif
