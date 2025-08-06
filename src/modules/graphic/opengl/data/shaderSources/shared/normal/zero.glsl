/*
	<!-- shared/normal_zero.glsl -->
	<define>MATERIAL_NORMAL_DEC</define>
	<define>MATERIAL_NORMAL_ENC</define>
*/

// zero-normal encoded using respective method. some methods can not represent
// a true zero normal. in this case (0,0,-1) is used
const vec3 normalZeroShiftedFloat = vec3( 0.5 );
const vec3 normalZeroShiftedInt = vec3( 127.0 / 255.0 );
const vec2 normalZeroSphereMap = vec2( 0.5 );
const vec3 normalZeroDirect = vec3( 0.0 );

// zero-normal encoded using respective method. some methods can not represent a true
// zero normal. in this case (0,0,-1) is used. constant is padded to vec3 with 0
const vec3 normalZeroMaterialDec =
	MaterialNormalDec == MaterialNormalModeFloatBasic ? normalZeroShiftedFloat :
	MaterialNormalDec == MaterialNormalModeIntBasic ? normalZeroShiftedInt :
	MaterialNormalDec == MaterialNormalModeSpheremap ? vec3(normalZeroSphereMap, 0.0) :
	normalZeroDirect;

const vec3 normalZeroMaterialEnc =
	MaterialNormalEnc == MaterialNormalModeFloatBasic ? normalZeroShiftedFloat :
	MaterialNormalEnc == MaterialNormalModeIntBasic ? normalZeroShiftedInt :
	MaterialNormalEnc == MaterialNormalModeSpheremap ? vec3(normalZeroSphereMap, 0.0) :
	normalZeroDirect;
