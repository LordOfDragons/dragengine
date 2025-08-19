// usage:
// #include "shared/interface/skin/emissivity.glsl"

float finalEmissivityIntensity(in float intensity){
	return mix(intensity, intensity * pCameraAdaptedIntensity, getEmissivityCameraAdapted(vSPBIndex));
}

vec3 finalEmissivityIntensity(in vec3 intensity){
	return mix(intensity, intensity * vec3(pCameraAdaptedIntensity), bvec3(getEmissivityCameraAdapted(vSPBIndex)));
}
