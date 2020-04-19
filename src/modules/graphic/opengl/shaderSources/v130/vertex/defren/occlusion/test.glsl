precision highp float;
precision highp int;

uniform vec4 pPosTransform; // scaleX, scaleY, offsetX, offsetY
uniform float pInvWidth;
uniform mat4 pMatrix; // camera-rotation and projection

in vec3 inMinExtend;
in vec3 inMaxExtend;

out vec3 vMinExtend;
out vec2 vMaxExtend;

const float znear = -1.0;



// calculates the screen space position of a point
void calcScreenPosition( in vec4 pointWorld, out vec4 pointScreen ){
	pointScreen = pMatrix * pointWorld;
	
	if( pointScreen.z < znear ){ // behind near z plane, clamp coordinates to the closest border
		pointScreen.xyz = vec3( mix( vec2( -1.0 ), vec2( 1.0 ), bvec2( step( 0.5, pointScreen.xy ) ) ), -1.0 );
		
	}else{ // in front of the near z plane, do normal projection
		pointScreen.xy = clamp( pointScreen.xy / vec2( pointScreen.w ), -1.0, 1.0 );
	}
}

// calculates the screen space AABB projection
#ifdef ENSURE_MIN_SIZE
	const vec3 epsilonSize = vec3( 0.005 );
	const vec3 zeroSize = vec3( 0.0 );
#endif

void calcScreenAABB( out vec3 minExtend, out vec2 maxExtend ){
	vec4 pointWorld, pointScreen;
	
	// ensure the box is not too small
#ifdef ENSURE_MIN_SIZE
	//vec3 adjustExtends = step( ( inMaxExtend - inMinExtend ), epsilonSize ) * epsilonSize;
	vec3 adjustExtends = mix( zeroSize, epsilonSize, lessThan( ( inMaxExtend - inMinExtend ), epsilonSize ) );
	vec3 safeInMinExtend = inMinExtend - adjustExtends;
	vec3 safeInMaxExtend = inMaxExtend + adjustExtends;
	#define IN_MIN_EXTEND safeInMinExtend
	#define IN_MAX_EXTEND safeInMaxExtend
#else
	#define IN_MIN_EXTEND inMinExtend
	#define IN_MAX_EXTEND inMaxExtend
#endif
	
	// point -x, -y, -z
	pointWorld = vec4( IN_MIN_EXTEND, 1.0 );
	calcScreenPosition( pointWorld, pointScreen );
	minExtend = pointScreen.xyz;
	maxExtend = pointScreen.xy;
	
	// point  x, -y, -z
	pointWorld.x = IN_MAX_EXTEND.x;
	calcScreenPosition( pointWorld, pointScreen );
	minExtend = min( minExtend, vec3( pointScreen ) );
	maxExtend = max( maxExtend, vec2( pointScreen ) );
	
	// point  x,  y, -z
	pointWorld.y = IN_MAX_EXTEND.y;
	calcScreenPosition( pointWorld, pointScreen );
	minExtend = min( minExtend, vec3( pointScreen ) );
	maxExtend = max( maxExtend, vec2( pointScreen ) );
	
	// point -x,  y, -z
	pointWorld.x = IN_MIN_EXTEND.x;
	calcScreenPosition( pointWorld, pointScreen );
	minExtend = min( minExtend, vec3( pointScreen ) );
	maxExtend = max( maxExtend, vec2( pointScreen ) );
	
	// point -x,  y,  z
	pointWorld.z = IN_MAX_EXTEND.z;
	calcScreenPosition( pointWorld, pointScreen );
	minExtend = min( minExtend, vec3( pointScreen ) );
	maxExtend = max( maxExtend, vec2( pointScreen ) );
	
	// point  x,  y,  z
	pointWorld.x = IN_MAX_EXTEND.x;
	calcScreenPosition( pointWorld, pointScreen );
	minExtend = min( minExtend, vec3( pointScreen ) );
	maxExtend = max( maxExtend, vec2( pointScreen ) );
	
	// point  x, -y,  z
	pointWorld.y = IN_MIN_EXTEND.y;
	calcScreenPosition( pointWorld, pointScreen );
	minExtend = min( minExtend, vec3( pointScreen ) );
	maxExtend = max( maxExtend, vec2( pointScreen ) );
	
	// point -x, -y,  z
	pointWorld.x = IN_MIN_EXTEND.x;
	calcScreenPosition( pointWorld, pointScreen );
	minExtend = min( minExtend, vec3( pointScreen ) );
	maxExtend = max( maxExtend, vec2( pointScreen ) );
}



void main( void ){
	vec2 pos;
	
	pos.x = modf( float( gl_VertexID ) * pInvWidth, pos.y );
	gl_Position = vec4( vec3( pos * pPosTransform.xy + pPosTransform.zw, 0.0 ), 1.0 );
	
	calcScreenAABB( vMinExtend, vMaxExtend );
	vMinExtend = vMinExtend * vec3( 0.5 ) + vec3( 0.5 );
	vMaxExtend = vMaxExtend * vec2( 0.5 ) + vec2( 0.5 );
}
