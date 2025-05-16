precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/plan/render_task_sortable.glsl"


struct sCounter {
	uvec3 workGroupSize;
	uint counter;
};

UBOLAYOUT_BIND(0) readonly buffer Counters {
	sCounter pRenderTaskCounters;
};

UBOLAYOUT_BIND(1) restrict buffer RenderTask {
	sRenderTaskSortable pRenderTask[];
};


uniform int pStage;
uniform uint pLaneSize;


layout( local_size_x=64 ) in;


const uint cMaxLaneSize = uint( 128 );

shared sRenderTaskSortable vSteps[ cMaxLaneSize ];


// compare two steps. returns true if the step i.x is sorted equal or before i.y .
// returns false if i.x is sorted after i.y
bool localStepLessThanEqual( in uvec2 i ){
	// the full compare code looks like this:
	//   if( s1.pipeline < s2.pipeline ) return true;
	//   if( s1.pipeline > s2.pipeline ) return false;
	//   if( s1.tuc < s2.tuc ) return true;
	//   if( s1.tuc > s2.tuc ) return false;
	//   if( s1.vao < s2.vao ) return true;
	//   if( s1.vao > s2.vao ) return false;
	//   if( s1.instance <= s2.instance ) return true;
	//   return false;
	// 
	// this can be rewritten like this:
	//   if( s1.pipeline > s2.pipeline ) return false;
	//   if( s1.pipeline == s2.pipeline && s1.tuc > s2.tuc ) return false;
	//   if( s1.pipeline == s2.pipeline && s1.tuc == s2.tuc && s1.vao > s2.vao ) return false;
	//   if( s1.pipeline == s2.pipeline && s1.tuc == s2.tuc && s1.vao == s2.vao && s1.instance > s2.instance ) return false;
	//   return true;
	// 
	// the comparisons can be calculated like this:
	//   bvec4 lt = greaterThan( s1.key, s2.key );
	//   bvec3 eq = equals( s1.key.xyz, s2.key.xyz );
	// 
	// another way is to use integer math using key as (pipeline,tuc,vao,instance):
	//   ivec4 t = sign( s1.key - s2.key ) * ivec4( 8, 4, 2, 1 );
	//   t.xy += t.zw;
	//   return t.x + t.y <= 0;
	// 
	// these are the possible scenarios (worst case):
	//   if( s1.pipeline < s2.pipeline ) return true;  => (-8, 4, 2, 1) = -1
	//   if( s1.pipeline > s2.pipeline ) return false; => ( 8,-4,-2,-1) =  1
	//   if( s1.tuc < s2.tuc ) return true;            => ( 0,-4, 2, 1) = -1
	//   if( s1.tuc > s2.tuc ) return false;           => ( 0, 4,-2,-1) =  1
	//   if( s1.vao < s2.vao ) return true;            => ( 0, 0,-2, 1) = -1
	//   if( s1.vao > s2.vao ) return false;           => ( 0, 0, 2,-1) =  1
	//   if( s1.instance <= s2.instance ) return true; => ( 0, 0, 0,-1|0) = -1|0
	//   return false;                                 => ( 0, 0, 0, 1) = 1
	// 
	// this version has the advantage it does not use any conditionals
	// 
	// this can be slightly modified to use floating point instead so a dot() can be used:
	//   dot( vec4( sign( s1.key - s2.key ) ), vec4( 8, 4, 2, 1 ) ) <= 0
	// 
	// but this is not that much of an improvement so using the integer version and
	// the integer based version is slightly faster than the floating point version
	
	// steps of same pass can sort. steps of different pass newer sort. make sure
	// passes stick together for stings like decals or render order to work
	// correctly. this sorting can be included in the main sorting by using
	// multiplier 16 to give it highest priority
	
	// in total 6 values have to be compared whereas the last one is more of convenience
	// to allow better renderdoc usage and to place sub instances closer in memory.
	// 
	// the values are weighted like this:
	// - pass: params1.x = 32
	// - pipeline: params1.y = 16
	// - tuc: params1.z = 8
	// - vao: params1.w = 4
	// - instance: params2.x = 2
	// - spbIndex: params2.y = 1
	
	ivec4 t = sign( vSteps[ i.x ].params1 - vSteps[ i.y ].params1 ) * ivec4( 32, 16, 8, 4 );
	t.xy += t.zw;
	
	t.zw = sign( vSteps[ i.x ].params2.xy - vSteps[ i.y ].params2.xy ) * ivec2( 2, 1 );
	t.xy += t.zw;
	
	return t.x + t.y <= 0;
	
	/*
	float t = dot( vec4( sign( vSteps[ i.x ].params1 - vSteps[ i.y ].params1 ) ), vec4( 32, 16, 8, 4 ) );
	t += dot( vec2( sign( vSteps[ i.x ].params2.xy - vSteps[ i.y ].params2.xy ) ), vec2( 2, 1 ) );
	return t <= 0;
	*/
}


// local compare and swap on shared memory
void localCompareAndSwap( in uint limit, in uvec2 i ){
	// if flip would touch values outside the step count return false
	// to never sort otherwise wrong values end up in the sorted result
	if( any( greaterThanEqual( i, uvec2( limit ) ) ) ){
		return;
	}
	
	// if first step is sorted same as step 2 or before do not swap
	if( localStepLessThanEqual( i ) ){
		return;
	}
	
	// swap steps
	ivec4 temp = vSteps[ i.x ].params1;
	vSteps[ i.x ].params1 = vSteps[ i.y ].params1;
	vSteps[ i.y ].params1 = temp;
	
	temp = vSteps[ i.x ].params2;
	vSteps[ i.x ].params2 = vSteps[ i.y ].params2;
	vSteps[ i.y ].params2 = temp;
}

// local flip on shared memory
void localFlip( in uint limit, in uint h ){
	uint t = gl_LocalInvocationID.x;
	barrier();
	
	uint hh = h / uint( 2 );
	uint thh = t % hh;
	
	localCompareAndSwap( limit,
		  uvec2( ( ( t * uint( 2 ) ) / h ) * h )
		+ uvec2( thh, h - thh - uint( 1 ) ) );
}

// local disperse on shared memory
void localDisperse( in uint limit, in uint h ){
	uint t = gl_LocalInvocationID.x;
	while( h > uint( 1 ) ){
		barrier();
		
		uint hh = h / uint( 2 );
		uint thh = t % hh;
		
		localCompareAndSwap( limit,
			  uvec2( ( ( t * uint( 2 ) ) / h ) * h )
			+ uvec2( thh, hh + thh ) );
		
		h = hh;
	}
}

// local bitonis merge sort on shared memory
void localSort( in uint limit, in uint h ){
	uint t = gl_LocalInvocationID.x;
	uint hh = uint( 1 );
	
	while( hh < h ){
		uint hhn = hh * uint( 2 );
		localFlip( limit, hhn );
		localDisperse( limit, hh );
		hh = hhn;
	}
}


// compare two steps. returns true if the step i.x is sorted equal or before i.y .
// returns false if i.x is sorted after i.y
bool globalStepLessThanEqual( in uvec2 i ){
	ivec4 t = sign( pRenderTask[ i.x ].params1 - pRenderTask[ i.y ].params1 ) * ivec4( 32, 16, 8, 4 );
	t.xy += t.zw;
	
	t.zw = sign( pRenderTask[ i.x ].params2.xy - pRenderTask[ i.y ].params2.xy ) * ivec2( 2, 1 );
	t.xy += t.zw;
	
	return t.x + t.y <= 0;
}

// global compare and swap on shared memory
void globalCompareAndSwap( in uint stepCount, in uvec2 i ){
	// if flip would touch values outside the step count return false
	// to never sort otherwise wrong values end up in the sorted result
	if( any( greaterThanEqual( i, uvec2( stepCount ) ) ) ){
		return;
	}
	
	// if first step is sorted same as step 2 or before do not swap
	if( globalStepLessThanEqual( i ) ){
		return;
	}
	
	// swap steps
	ivec4 temp = pRenderTask[ i.x ].params1;
	pRenderTask[ i.x ].params1 = pRenderTask[ i.y ].params1;
	pRenderTask[ i.y ].params1 = temp;
	
	temp = pRenderTask[ i.x ].params2;
	pRenderTask[ i.x ].params2 = pRenderTask[ i.y ].params2;
	pRenderTask[ i.y ].params2 = temp;
}

// global flip on ssbo memory. local memory is not helping here since each used
// value is touched exactly one.
void globalFlip( in uint stepCount, in uint h ){
	uint t = gl_GlobalInvocationID.x;
	uint hh = h / uint( 2 );
	uint thh = t % hh;
	
	globalCompareAndSwap( stepCount,
		  uvec2( ( ( t * uint( 2 ) ) / h ) * h )
		+ uvec2( thh, h - thh - uint( 1 ) ) );
}

// global disperse on ssbo memory. local memory is not helping here since each used
// value is touched exactly one.
void globalDisperse( in uint stepCount, in uint h ){
	uint t = gl_GlobalInvocationID.x;
	uint hh = h / uint( 2 );
	uint thh = t % hh;
	
	globalCompareAndSwap( stepCount,
		  uvec2( ( ( t * uint( 2 ) ) / h ) * h )
		+ uvec2( thh, thh + hh ) );
}


// stage constants
const int esLocalSort = 0;
const int esLocalDisperse = 1;
const int esGlobalFlip = 2;
const int esGlobalDisperse = 3;


void main( void ){
	uint stepCount = min( pRenderTaskCounters.counter, uint( pRenderTask.length() ) );
	uint t = gl_LocalInvocationID.x;
	uint offset = pLaneSize * gl_WorkGroupID.x;
	uint limit = cMaxLaneSize;
	bvec2 valid = bvec2(false);
	uvec4 i = uvec4(0);
	
	// cooperative copying steps from ssbo into shared memory
	if( pStage <= esLocalDisperse ){
		// entire block is outside range
		if( offset >= stepCount ){
			return;
		}
		
		limit = stepCount - offset;
		
		// t*2 => offset+t*2 and t*2+1 => offset+t*2+1
		i = uvec4( t * uint( 2 ) );
		i.zw += uvec2( offset );
		i.yw += uvec2( 1 );
		
		valid = lessThan( i.zw, uvec2( stepCount ) );
		
		if( valid.x ){
			vSteps[ i.x ].params1 = pRenderTask[ i.z ].params1;
			vSteps[ i.x ].params2 = pRenderTask[ i.z ].params2;
		}
		if( valid.y ){
			vSteps[ i.y ].params1 = pRenderTask[ i.w ].params1;
			vSteps[ i.y ].params2 = pRenderTask[ i.w ].params2;
		}
	}
	
	// per invocation processing
	if( pStage == esLocalSort ){
		localSort( limit, pLaneSize );
		
	}else if( pStage == esLocalDisperse ){
		localDisperse( limit, pLaneSize );
		
	}else if( pStage == esGlobalFlip ){
		globalFlip( stepCount, pLaneSize );
		
	}else{ // pStage == esGlobalFlip
		globalDisperse( stepCount, pLaneSize );
	}
	
	// cooperative copying steps from shared memory into ssbo
	if( pStage <= esLocalDisperse ){
		barrier();
		
		if( valid.x ){
			pRenderTask[ i.z ].params1 = vSteps[ i.x ].params1;
			pRenderTask[ i.z ].params2 = vSteps[ i.x ].params2;
		}
		if( valid.y ){
			pRenderTask[ i.w ].params1 = vSteps[ i.y ].params1;
			pRenderTask[ i.w ].params2 = vSteps[ i.y ].params2;
		}
	}
}
