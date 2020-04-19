/* 
 * Drag[en]gine OpenAL Audio Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoalRTWorldBVHVisitor.h"
#include "../../component/deoalAComponent.h"

#include <dragengine/common/exceptions.h>



// Class deoalRTWorldBVHVisitor
/////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalRTWorldBVHVisitor::deoalRTWorldBVHVisitor() :
pCheckAxisX( false ),
pCheckAxisY( false ),
pCheckAxisZ( false ),
pRayLength( 0.0f )
//,pSphereDot( 0.0f )
{
}

deoalRTWorldBVHVisitor::~deoalRTWorldBVHVisitor(){
}



// Visiting
/////////////

void deoalRTWorldBVHVisitor::SetRay( const decVector &origin, const decVector &direction ){
	pRayOrigin = origin;
	pRayTarget = origin + direction;
	pRayDirection = direction;
	pRayLength = direction.Length();
	
	pInvRayDirection.SetZero();
	pCheckAxisX = fabsf( direction.x ) > FLOAT_SAFE_EPSILON;
	pCheckAxisY = fabsf( direction.y ) > FLOAT_SAFE_EPSILON;
	pCheckAxisZ = fabsf( direction.z ) > FLOAT_SAFE_EPSILON;
	
	if( pCheckAxisX ){
		pInvRayDirection.x = 1.0f / direction.x;
	}
	if( pCheckAxisY ){
		pInvRayDirection.y = 1.0f / direction.y;
	}
	if( pCheckAxisZ ){
		pInvRayDirection.z = 1.0f / direction.z;
	}
	
	const decVector margin( 0.0005f, 0.0005f, 0.0005f );
	pRayBoxMin = pRayOrigin.Smallest( pRayTarget ) - margin;
	pRayBoxMax = pRayOrigin.Largest( pRayTarget ) + margin;
	
// 	pRayNormDir = pRayDirection / pRayLength;
// 	pSphereDot = -( origin * pRayNormDir );
}

void deoalRTWorldBVHVisitor::SetRay( const deoalRTWorldBVH &bvh,
const decDVector &origin, const decDVector &direction ){
	SetRay( origin - bvh.GetPosition(), direction );
}



void deoalRTWorldBVHVisitor::VisitBVH( const deoalRTWorldBVH &bvh ){
	if( bvh.GetVisitNodeCount() > 0 ){
		pVisitNode( bvh, bvh.GetVisitNodes()[ 0 ] );
	}
}

void deoalRTWorldBVHVisitor::VisitComponent( const deoalRTWorldBVH::sVisitComponent & ){
}



// Protected Functions
////////////////////////

void deoalRTWorldBVHVisitor::pVisitNode( const deoalRTWorldBVH &bvh,
const deoalRTWorldBVH::sVisitNode &node ){
	// visit components
	if( node.componentCount > 0 ){
		const deoalRTWorldBVH::sVisitComponent * const components =
			bvh.GetVisitComponents() + node.firstComponent;
		int i;
		
		for( i=0; i<node.componentCount; i++ ){
			const deoalRTWorldBVH::sVisitComponent &component = components[ i ];
			if( /*pRayHitsSphere( component.sphereCenter, component.sphereRadiusSquared )
			&& */pRayHitsBox( component.center, component.halfSize ) ){
				VisitComponent( component );
			}
		}
		
	// visit child nodes if hit by ray
	}else{
		const deoalRTWorldBVH::sVisitNode &child1 = bvh.GetVisitNodes()[ node.node1 ];
		if( pRayHitsBox( child1.center, child1.halfSize ) ){
			pVisitNode( bvh, child1 );
		}
		
		const deoalRTWorldBVH::sVisitNode &child2 = bvh.GetVisitNodes()[ node.node2 ];
		if( pRayHitsBox( child2.center, child2.halfSize ) ){
			pVisitNode( bvh, child2 );
		}
	}
}

bool deoalRTWorldBVHVisitor::pRayHitsBox( const decVector &center, const decVector &halfExtends ) const{
	const decVector point( pRayOrigin - center );
	if( point.Absolute() <= halfExtends || ( pRayTarget - center ).Absolute() <= halfExtends ){
		return true;
	}
	
	// x axis
	if( pCheckAxisX ){
		// face on the positive side
		const float lambda1 = pInvRayDirection.x * ( halfExtends.x - point.x );
		if( lambda1 >= 0.0f && lambda1 <= 1.0f ){
			const float y = point.y + pRayDirection.y * lambda1;
			if( y >= -halfExtends.y && y <= halfExtends.y ){
				const float z = point.z + pRayDirection.z * lambda1;
				if( z >= -halfExtends.z && z <= halfExtends.z ){
					return true;
				}
			}
		}
		
		// face on the negative side
		const float lambda2 = pInvRayDirection.x * ( -halfExtends.x - point.x );
		if( lambda2 >= 0.0f && lambda2 <= 1.0f ){
			const float y = point.y + pRayDirection.y * lambda2;
			if( y >= -halfExtends.y && y <= halfExtends.y ){
				const float z = point.z + pRayDirection.z * lambda2;
				if( z >= -halfExtends.z && z <= halfExtends.z ){
					return true;
				}
			}
		}
	}
	
	// y axis
	if( pCheckAxisY ){
		// face on the positive side
		const float lambda1 = pInvRayDirection.y * ( halfExtends.y - point.y );
		if( lambda1 >= 0.0f && lambda1 <= 1.0f ){
			const float x = point.x + pRayDirection.x * lambda1;
			if( x >= -halfExtends.x && x <= halfExtends.x ){
				const float z = point.z + pRayDirection.z * lambda1;
				if( z >= -halfExtends.z && z <= halfExtends.z ){
					return true;
				}
			}
		}
		
		// face on the negative side
		const float lambda2 = pInvRayDirection.y * ( -halfExtends.y - point.y );
		if( lambda2 >= 0.0f && lambda2 <= 1.0f ){
			const float x = point.x + pRayDirection.x * lambda2;
			if( x >= -halfExtends.x && x <= halfExtends.x ){
				const float z = point.z + pRayDirection.z * lambda2;
				if( z >= -halfExtends.z && z <= halfExtends.z ){
					return true;
				}
			}
		}
	}
	
	// z axis
	if( pCheckAxisZ ){
		// face on the positive side
		const float lambda1 = pInvRayDirection.z * ( halfExtends.z - point.z );
		if( lambda1 >= 0.0f && lambda1 <= 1.0f ){
			const float x = point.x + pRayDirection.x * lambda1;
			if( x >= -halfExtends.x && x <= halfExtends.x ){
				const float y = point.y + pRayDirection.y * lambda1;
				if( y >= -halfExtends.y && y <= halfExtends.y ){
					return true;
				}
			}
		}
		
		// face on the negative side
		const float lambda2 = pInvRayDirection.z * ( -halfExtends.z - point.z );
		if( lambda2 >= 0.0f && lambda2 <= 1.0f ){
			const float x = point.x + pRayDirection.x * lambda2;
			if( x >= -halfExtends.x && x <= halfExtends.x ){
				const float y = point.y + pRayDirection.y * lambda2;
				if( y >= -halfExtends.y && y <= halfExtends.y ){
					return true;
				}
			}
		}
	}
	
	// no hit
	return false;
}

/*
bool deoalRTWorldBVHVisitor::pRayHitsSphere( const decVector &center, float radiusSquared ) const{
	const float distance = decMath::clamp( center * pRayNormDir + pSphereDot, 0.0f, pRayLength );
	const decVector closestPoint( pRayOrigin + pRayNormDir * distance );
	return ( center - closestPoint ).LengthSquared() <= radiusSquared;
}
*/

bool deoalRTWorldBVHVisitor::pRayHitsBox( const decVector &center, const decVector &halfExtends,
float &closestDistance ) const{
	const decVector point( pRayOrigin - center );
	if( point.Absolute() <= halfExtends ){
		closestDistance = 0.0f;
		return true;
	}
	
	bool hasHit = false;
	closestDistance = 1.0f;
	
	// x axis
	if( pCheckAxisX ){
		// face on the positive side
		const float lambda1 = pInvRayDirection.x * ( halfExtends.x - point.x );
		if( lambda1 >= 0.0f && lambda1 <= 1.0f ){
			const float y = point.y + pRayDirection.y * lambda1;
			if( y >= -halfExtends.y && y <= halfExtends.y ){
				const float z = point.z + pRayDirection.z * lambda1;
				if( z >= -halfExtends.z && z <= halfExtends.z ){
					if( lambda1 < closestDistance ){
						closestDistance = lambda1;
					}
					hasHit = true;
				}
			}
		}
		
		// face on the negative side
		const float lambda2 = pInvRayDirection.x * ( -halfExtends.x - point.x );
		if( lambda2 >= 0.0f && lambda2 <= 1.0f ){
			const float y = point.y + pRayDirection.y * lambda2;
			if( y >= -halfExtends.y && y <= halfExtends.y ){
				const float z = point.z + pRayDirection.z * lambda2;
				if( z >= -halfExtends.z && z <= halfExtends.z ){
					if( lambda2 < closestDistance ){
						closestDistance = lambda2;
					}
					hasHit = true;
				}
			}
		}
	}
	
	// y axis
	if( pCheckAxisY ){
		// face on the positive side
		const float lambda1 = pInvRayDirection.y * ( halfExtends.y - point.y );
		if( lambda1 >= 0.0f && lambda1 <= 1.0f ){
			const float x = point.x + pRayDirection.x * lambda1;
			if( x >= -halfExtends.x && x <= halfExtends.x ){
				const float z = point.z + pRayDirection.z * lambda1;
				if( z >= -halfExtends.z && z <= halfExtends.z ){
					if( lambda1 < closestDistance ){
						closestDistance = lambda1;
					}
					hasHit = true;
				}
			}
		}
		
		// face on the negative side
		const float lambda2 = pInvRayDirection.y * ( -halfExtends.y - point.y );
		if( lambda2 >= 0.0f && lambda2 <= 1.0f ){
			const float x = point.x + pRayDirection.x * lambda2;
			if( x >= -halfExtends.x && x <= halfExtends.x ){
				const float z = point.z + pRayDirection.z * lambda2;
				if( z >= -halfExtends.z && z <= halfExtends.z ){
					if( lambda2 < closestDistance ){
						closestDistance = lambda2;
					}
					hasHit = true;
				}
			}
		}
	}
	
	// z axis
	if( pCheckAxisZ ){
		// face on the positive side
		const float lambda1 = pInvRayDirection.z * ( halfExtends.z - point.z );
		if( lambda1 >= 0.0f && lambda1 <= 1.0f ){
			const float x = point.x + pRayDirection.x * lambda1;
			if( x >= -halfExtends.x && x <= halfExtends.x ){
				const float y = point.y + pRayDirection.y * lambda1;
				if( y >= -halfExtends.y && y <= halfExtends.y ){
					if( lambda1 < closestDistance ){
						closestDistance = lambda1;
					}
					hasHit = true;
				}
			}
		}
		
		// face on the negative side
		const float lambda2 = pInvRayDirection.z * ( -halfExtends.z - point.z );
		if( lambda2 >= 0.0f && lambda2 <= 1.0f ){
			const float x = point.x + pRayDirection.x * lambda2;
			if( x >= -halfExtends.x && x <= halfExtends.x ){
				const float y = point.y + pRayDirection.y * lambda2;
				if( y >= -halfExtends.y && y <= halfExtends.y ){
					if( lambda2 < closestDistance ){
						closestDistance = lambda2;
					}
					hasHit = true;
				}
			}
		}
	}
	
	return hasHit;
}
