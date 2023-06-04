
// zero-normal encoded using respective method. some methods can not represent
// a true zero normal. in this case (0,0,-1) is used
const vec3 normalZeroShiftedFloat = vec3( 0.5 );
const vec3 normalZeroShiftedInt = vec3( 127.0 / 255.0 );
const vec2 normalZeroSphereMap = vec2( 0.5 );
const vec3 normalZeroDirect = vec3( 0.0 );

// zero-normal encoded using respective method. some methods can not represent a true
// zero normal. in this case (0,0,-1) is used. constant is padded to vec3 with 0
#ifdef MATERIAL_NORMAL_DEC_FLOATBASIC
const vec3 normalZeroMaterialDec = normalZeroShiftedFloat;

#elif defined MATERIAL_NORMAL_DEC_INTBASIC
const vec3 normalZeroMaterialDec = normalZeroShiftedInt;

#elif defined MATERIAL_NORMAL_DEC_SPHEREMAP
const vec3 normalZeroMaterialDec = vec3( normalZeroSphereMap, 0.0 );

#else
const vec3 normalZeroMaterialDec = normalZeroDirect;
#endif

#ifdef MATERIAL_NORMAL_DEC_FLOATBASIC
const vec3 normalZeroMaterialEnc = normalZeroShiftedFloat;

#elif defined MATERIAL_NORMAL_DEC_INTBASIC
const vec3 normalZeroMaterialEnc = normalZeroShiftedInt;

#elif defined MATERIAL_NORMAL_DEC_SPHEREMAP
const vec3 normalZeroMaterialEnc = vec3( normalZeroSphereMap, 0.0 );

#else
const vec3 normalZeroMaterialEnc = normalZeroDirect;
#endif
