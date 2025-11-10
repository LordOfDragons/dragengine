/*
	<!-- shared/defren/skin/relief_mapping.glsl -->
	<define>TEXTURE_HEIGHT</define>
	<define>TEXTURE_NORMAL</define>
	<define>TEXTURE_ENVMAP</define>
*/

const int ReliefMappingMethodLinear = 0;
const int ReliefMappingMethodCone = 1;

const int reliefMappingMethod = ReliefMappingMethodLinear;

const int reliefMappingConeStepCount = 15;
const int reliefMappingBinaryStepCount = 6;


// calculate new texture coordinates using relief mapping
void reliefMapping(inout vec2 tc, const in vec3 normal, const int spbIndex){
	if(!TextureHeight || !TextureNormal || !TextureEnvMap){
		return;
	}
	
	// transform the view vector into the relief coordinate system
	int i;
	float scaleZ = 1.0 / max(getHeightRemap(spbIndex).x, 0.001);
	vec3 direction = vec3(
		dot( vTangent, vReflectDir ),
		dot( vBitangent, vReflectDir ),
		dot( normal, vReflectDir ) * scaleZ - getHeightRemap(spbIndex).y );
	
	direction /= -direction.z;
	int linearStepCount;
	if(reliefMappingMethod == ReliefMappingMethodLinear){
		//#define linearStepCount 30
		// map range from 1m to 20m, max 50 steps
		// 50.0 * ( vReflectDir.z - 1.0 ) / 20.0
		// f1 = 50.0 / 20.0 = 2.5
		// f2 = 50.0 * -1.0 / 20.0 = -2.5
		linearStepCount = clamp(50 - int(vReflectDir.z * 2.5 - 2.5), 1, 50);
		direction /= float(linearStepCount);
	}
	
	// find first point of intersection
	vec3 end;
	
	if(reliefMappingMethod == ReliefMappingMethodLinear){
		end = vec3( tc, 1 ); // top
		for( i=0; i<linearStepCount; i++ ){
			end += direction;
			if(TEXTURE_LOD(texHeight, end.st, 0.0).r > end.z){
				break; // ran below surface
			}
		}
		
	}else if(reliefMappingMethod == ReliefMappingMethodCone){
		float rayRatio = length( direction.xy );
		vec3 end = vec3( tc, 1 ); // top
		for( i=0; i<reliefMappingConeStepCount; i++ ){
			vec2 tap = TEXTURE_LOD(texHeight, end.st, 0.0).rg;
			end += direction * ( tap.y * clamp(end.z - tap.x, 0.0, 1.0) / max( rayRatio + tap.y, 0.001 ) );
		}
	}
	
	// refine using binary search
	vec3 begin = end - direction;
	for( i=0; i<reliefMappingBinaryStepCount; i++ ){
		direction *= vec3( 0.5 );
		vec3 test = begin + direction;
		if(TEXTURE_LOD(texHeight, test.st, 0.0).r > test.z){
			end = test;
		}else{
			begin = test;
		}
	}
	
	// update texture coordinates
	tc = end.st;
}
