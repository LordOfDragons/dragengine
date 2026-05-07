// ST2084 PQ OETF and BT.709 to BT.2020 gamut conversion for HDR output.
//
// Reference white: 203 cd/m2 (ITU-R BT.2408-4 SDR reference on HDR display).
// A rendered linear scene value of 1.0 maps to 203 cd/m2.
// The PQ signal range [0,1] represents 0..10000 cd/m2.

// Convert sRGB non-linear RGB to linear RGB
vec3 sRGB2Linear(vec3 c){
	bvec3 isLo = lessThanEqual(c, vec3(0.04045));
	vec3 loPart = c / vec3(12.92);
	vec3 hiPart = pow((c + vec3(0.055)) / vec3(1.055), vec3(12.0 / 5.0));
	return mix(hiPart, loPart, isLo);
}

// ST2084 PQ constants (SMPTE ST 2084)
const float pqM1 = (2610.0 / 4096.0) * (1.0 / 4.0); // ~0.159
const float pqM2 = (2523.0 / 4096.0) * 128.0; // ~78.844
const float pqC1 = 3424.0 / 4096.0; // ~0.836
const float pqC2 = (2413.0 / 4096.0) * 32.0; // ~18.852
const float pqC3 = (2392.0 / 4096.0) * 32.0; // ~18.688

// Normalised linear value -> PQ signal [0,1]
vec3 pqOETF(vec3 lin){
	vec3 Lp = pow(max(lin, 0.0), vec3(pqM1));
	vec3 num = vec3(pqC1) + vec3(pqC2) * Lp;
	vec3 den = vec3(1.0) + vec3(pqC3) * Lp;
	return pow(num / den, vec3(pqM2));
}

// BT.709 (sRGB primaries) linear RGB -> BT.2020 linear RGB
const mat3 matBt709ToBt2020 = mat3(
	0.627402, 0.069095, 0.016394,
	0.329292, 0.919544, 0.088028,
	0.043306, 0.011360, 0.895578
);

// Full HDR encoding: linear BT.709 RGB (scene-linear, 1.0 = refWhiteNits) -> PQ BT.2020
vec3 encodeHdrOutput(vec3 linearBt709, float maxNits, float refWhiteNits){
	return pqOETF((matBt709ToBt2020 * linearBt709) * vec3(refWhiteNits / maxNits));
}
