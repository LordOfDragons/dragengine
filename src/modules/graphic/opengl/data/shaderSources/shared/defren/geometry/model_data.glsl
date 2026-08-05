
// input model data
/////////////////////

struct sModelData{
	vec3 position;
	vec3 realNormal;
	vec3 normal;
	vec4 tangent;
	vec2 texCoord;
	int weights;
};

struct sModelDataTcs1{
	vec3 position;
	vec3 realNormal;
	vec3 normal;
	vec4 tangent;
	vec2 texCoord;
	int weights;
	vec4 tangent2;
	vec2 texCoord2;
};

struct sModelDataTcs2{
	vec3 position;
	vec3 realNormal;
	vec3 normal;
	vec4 tangent;
	vec2 texCoord;
	int weights;
	vec4 tangent2;
	vec2 texCoord2;
	vec4 tangent3;
	vec2 texCoord3;
};


// output model data
//////////////////////

struct sTransformedData{
	vec3 position;
	vec3 realNormal;
	vec3 normal;
	vec4 tangent;
};

struct sTransformedDataTcs1{
	vec3 position;
	vec3 realNormal;
	vec3 normal;
	vec4 tangent;
	vec4 tangent2;
};

struct sTransformedDataTcs2{
	vec3 position;
	vec3 realNormal;
	vec3 normal;
	vec4 tangent;
	vec4 tangent2;
	vec4 tangent3;
};
