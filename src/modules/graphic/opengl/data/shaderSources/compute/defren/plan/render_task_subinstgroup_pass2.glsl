precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/plan/counter.glsl"


UBOLAYOUT_BIND(0) readonly buffer Counters {
	sCounter pRenderComputeCounter[ pRenderComputeCounterCount ];
};

UBOLAYOUT_BIND(1) restrict buffer SubInstGroup {
	uvec4 pSubInstGroup[];
};


uniform int pStage;
uniform uint pLaneSize;


layout( local_size_x=64 ) in;


const uint cMaxLaneSize = uint( 128 );
#define cStepCount pRenderComputeCounter[ erccRenderTaskSubInstanceGroups ].counter

shared uint vGroups[ cMaxLaneSize ];


void localCompareAndSwap( in uint limit, in uvec2 i ){
	if( any( greaterThanEqual( i, uvec2( limit ) ) ) ){
		return;
	}
	if( vGroups[ i.x ] <= vGroups[ i.y ] ){
		return;
	}
	
	uint temp = vGroups[ i.x ];
	vGroups[ i.x ] = vGroups[ i.y ];
	vGroups[ i.y ] = temp;
}

void localFlip( in uint limit, in uint h ){
	uint t = gl_LocalInvocationID.x;
	barrier();
	
	uint hh = h / uint( 2 );
	uint thh = t % hh;
	
	localCompareAndSwap( limit,
		  uvec2( ( ( t * uint( 2 ) ) / h ) * h )
		+ uvec2( thh, h - thh - uint( 1 ) ) );
}

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


uint getSubInstGroup( in uint index ){
	return pSubInstGroup[ index / uint( 4 ) ][ index % uint( 4 ) ];
}

void setSubInstGroup( in uint index, in uint value ){
	pSubInstGroup[ index / uint( 4 ) ][ index % uint( 4 ) ] = value;
}

void globalCompareAndSwap( in uvec2 i ){
	if( any( greaterThanEqual( i, uvec2( cStepCount ) ) ) ){
		return;
	}
	if( getSubInstGroup( i.x ) <= getSubInstGroup( i.y ) ){
		return;
	}
	
	uint temp = getSubInstGroup( i.x );
	setSubInstGroup( i.x, getSubInstGroup( i.y ) );
	setSubInstGroup( i.y, temp );
}

void globalFlip( in uint h ){
	uint t = gl_GlobalInvocationID.x;
	uint hh = h / uint( 2 );
	uint thh = t % hh;
	
	globalCompareAndSwap(
		  uvec2( ( ( t * uint( 2 ) ) / h ) * h )
		+ uvec2( thh, h - thh - uint( 1 ) ) );
}

void globalDisperse( in uint h ){
	uint t = gl_GlobalInvocationID.x;
	uint hh = h / uint( 2 );
	uint thh = t % hh;
	
	globalCompareAndSwap(
		  uvec2( ( ( t * uint( 2 ) ) / h ) * h )
		+ uvec2( thh, thh + hh ) );
}


const int esLocalSort = 0;
const int esLocalDisperse = 1;
const int esGlobalFlip = 2;
const int esGlobalDisperse = 3;


void main( void ){
	uint t = gl_LocalInvocationID.x;
	uint offset = pLaneSize * gl_WorkGroupID.x;
	uint limit = cMaxLaneSize;
	bvec2 valid = bvec2(false);
	uvec4 i = uvec4(0);
	
	if( pStage <= esLocalDisperse ){
		if( offset >= cStepCount ){
			return;
		}
		
		limit = cStepCount - offset;
		
		i = uvec4( t * uint( 2 ) );
		i.zw += uvec2( offset );
		i.yw += uvec2( 1 );
		
		valid = lessThan( i.zw, uvec2( cStepCount ) );
		
		if( valid.x ){
			vGroups[ i.x ] = getSubInstGroup( i.z );
		}
		if( valid.y ){
			vGroups[ i.y ] = getSubInstGroup( i.w );
		}
	}
	
	if( pStage == esLocalSort ){
		localSort( limit, pLaneSize );
		
	}else if( pStage == esLocalDisperse ){
		localDisperse( limit, pLaneSize );
		
	}else if( pStage == esGlobalFlip ){
		globalFlip( pLaneSize );
		
	}else{ // pStage == esGlobalFlip
		globalDisperse( pLaneSize );
	}
	
	if( pStage <= esLocalDisperse ){
		barrier();
		
		if( valid.x ){
			setSubInstGroup( i.z, vGroups[ i.x ] );
		}
		if( valid.y ){
			setSubInstGroup( i.w, vGroups[ i.y ] );
		}
	}
}
