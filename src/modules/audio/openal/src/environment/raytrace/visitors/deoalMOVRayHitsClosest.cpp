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

#include "deoalMOVRayHitsClosest.h"
#include "../deoalRayTraceResult.h"
#include "../../../component/deoalAComponent.h"
#include "../../../component/deoalAComponentTexture.h"
#include "../../../model/deoalAModel.h"
#include "../../../model/deoalModelFace.h"
#include "../../../model/octree/deoalModelOctree.h"
#include "../../../model/octree/deoalModelRTOctree.h"

#include <dragengine/common/exceptions.h>



// Class deoalMOVRayHitsClosest
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalMOVRayHitsClosest::deoalMOVRayHitsClosest( deoalAComponent &component, const deoalAModel &model ) :
pComponent( component ),
pModel( model ),
pFrontFacing( true ),
pLimitDistance( 1.0f ),
pCheckAxisX( false ),
pCheckAxisY( false ),
pCheckAxisZ( false ),
pRayLength( 0.0f ),
pHasResult( false )
{
	(void)pModel; // silence compiler warning
#ifdef MOVRAYHITSFACES_DO_TIMIING
	timing = 0.0f;
	timingCount = 0;
#endif
}

deoalMOVRayHitsClosest::~deoalMOVRayHitsClosest(){
}



// Visiting
/////////////

void deoalMOVRayHitsClosest::SetRay( const decVector &origin, const decVector &direction ){
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
}

void deoalMOVRayHitsClosest::SetFrontFacing( bool frontFacing ){
	pFrontFacing = frontFacing;
}

void deoalMOVRayHitsClosest::SetLimitDistance( float limitDistance ){
	pLimitDistance = limitDistance;
}



#include "../../../audiothread/deoalAudioThread.h"
#include "../../../audiothread/deoalATLogger.h"

void deoalMOVRayHitsClosest::VisitNode( deoalModelOctree &node ){
	pHasResult = false;
	
	pVisitNode( node );
}

void deoalMOVRayHitsClosest::VisitNode( deoalOctree*, int ){
	DETHROW( deeInvalidParam );
}

void deoalMOVRayHitsClosest::VisitOctree( const deoalModelRTOctree &octree ){
	pHasResult = false;
	
	if( octree.GetNodeCount() > 0 ){
		pVisitNode( octree, octree.GetNodes()[ 0 ] );
	}
}

void deoalMOVRayHitsClosest::VisitBVH( const deoalModelRTBVH &bvh ){
	if( bvh.GetNodeCount() > 0 ){
		pVisitNode( bvh, bvh.GetNodes()[ 0 ] );
	}
}



// Protected Functions
////////////////////////

void deoalMOVRayHitsClosest::pVisitNode( const deoalModelOctree &node ){
#ifdef MOVRAYHITSFACES_DO_TIMIING
	timer.Reset();
#endif
	
	// visit faces in the node
	const int count = node.GetFaceCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deoalModelFace &face = *node.GetFaceAt( i );
		const float dot = face.GetNormal() * pRayDirection;
		if( pFrontFacing == ( dot > 0.0f ) || fabsf( dot ) < FLOAT_SAFE_EPSILON ){
			continue; // dot>0 = back facing ; abs(dot)<delta = parallel
		}
		
		const float lambda = ( ( face.GetVertex1() - pRayOrigin ) * face.GetNormal() ) / dot;
		if( lambda < 0.0f || lambda >= pLimitDistance ){
			continue;
		}
		
		const decVector hitPoint( pRayOrigin + pRayDirection * lambda );
		
		if( face.GetEdge1Normal() * hitPoint < face.GetEdge1DistanceSafe()
		|| face.GetEdge2Normal() * hitPoint < face.GetEdge2DistanceSafe()
		|| face.GetEdge3Normal() * hitPoint < face.GetEdge3DistanceSafe() ){
			continue;
		}
		
		const int textureIndex = pComponent.GetModelTextureMappings().GetAt( face.GetTexture() );
		if( textureIndex == -1 ){
			continue;
		}
		
		if( ! pComponent.GetTextureAt( textureIndex ).GetAffectsSound() ){
			continue;
		}
		
		pSetResult( lambda, hitPoint, face.GetNormal(), face.GetIndex() );
	}
#ifdef MOVRAYHITSFACES_DO_TIMIING
	timing += timer.GetElapsedTime();
	timingCount += count;
#endif
	
	// visit child nodes if hit by ray
	float closestDistance;
	for( i=0; i<8; i++ ){
		deoalOctree * const childNode = node.GetNodeAt( i );
		if( childNode
		&& pRayHitsBox( childNode->GetCenter(), childNode->GetHalfSize(), closestDistance )
		&& closestDistance < pLimitDistance ){
			pVisitNode( *( ( deoalModelOctree* )childNode ) );
		}
	}
}

void deoalMOVRayHitsClosest::pVisitNode( const deoalModelRTOctree &octree,
const deoalModelRTOctree::sNode &node ){
#ifdef MOVRAYHITSFACES_DO_TIMIING
	timer.Reset();
#endif
	// visit faces
	const deoalModelRTOctree::sFace * const faces = octree.GetFaces() + node.firstFace;
	int i;
	
	for( i=0; i<node.faceCount; i++ ){
		const deoalModelRTOctree::sFace &face = faces[ i ];
		const float dot = face.normal * pRayDirection;
		if( pFrontFacing == ( dot > 0.0f ) || fabsf( dot ) < FLOAT_SAFE_EPSILON ){
			continue; // dot>0 = back facing ; abs(dot)<delta = parallel
		}
		
		const float lambda = ( ( face.baseVertex - pRayOrigin ) * face.normal ) / dot;
		if( lambda < 0.0f || lambda > pLimitDistance ){
			continue;
		}
		
		const decVector hitPoint( pRayOrigin + pRayDirection * lambda );
		
		if( face.edgeNormal[ 0 ] * hitPoint < face.edgeDistance[ 0 ]
		|| face.edgeNormal[ 1 ] * hitPoint < face.edgeDistance[ 1 ]
		|| face.edgeNormal[ 2 ] * hitPoint < face.edgeDistance[ 2 ] ){
			continue;
		}
		
		const int textureIndex = pComponent.GetModelTextureMappings().GetAt( face.indexTexture );
		if( textureIndex == -1 ){
			continue;
		}
		
		if( ! pComponent.GetTextureAt( textureIndex ).GetAffectsSound() ){
			continue;
		}
		
		pSetResult( lambda, hitPoint, face.normal, face.indexFace );
	}
#ifdef MOVRAYHITSFACES_DO_TIMIING
	timing += timer.GetElapsedTime();
	timingCount += count;
#endif
	
	// visit child nodes if hit by ray
	const deoalModelRTOctree::sNode * const nodes = octree.GetNodes() + node.firstNode;
	float closestDistance;
	for( i=0; i<node.nodeCount; i++ ){
		const deoalModelRTOctree::sNode &child = nodes[ i ];
		if( pRayHitsBox( child.center, child.halfSize, closestDistance )
		&& closestDistance < pLimitDistance ){
			pVisitNode( octree, child );
		}
	}
}

void deoalMOVRayHitsClosest::pVisitNode( const deoalModelRTBVH &bvh, const deoalModelRTBVH::sNode &node ){
	if( node.faceCount > 0 ){
#ifdef MOVRAYHITSFACES_DO_TIMIING
		timer.Reset();
#endif
		const deoalModelRTBVH::sFace * const faces = bvh.GetFaces() + node.firstFace;
		int i;
		
		for( i=0; i<node.faceCount; i++ ){
			const deoalModelRTBVH::sFace &face = faces[ i ];
			
			const float dot = face.normal * pRayDirection;
			if( pFrontFacing == ( dot > 0.0f ) || fabsf( dot ) < FLOAT_SAFE_EPSILON ){
				continue; // dot>0 = back facing ; abs(dot)<delta = parallel
			}
			
			const float lambda = ( ( face.baseVertex - pRayOrigin ) * face.normal ) / dot;
			if( lambda < 0.0f || lambda > pLimitDistance ){
				continue;
			}
			
			const decVector hitPoint( pRayOrigin + pRayDirection * lambda );
			
			if( face.edgeNormal[ 0 ] * hitPoint < face.edgeDistance[ 0 ]
			|| face.edgeNormal[ 1 ] * hitPoint < face.edgeDistance[ 1 ]
			|| face.edgeNormal[ 2 ] * hitPoint < face.edgeDistance[ 2 ] ){
				continue;
			}
			
			const int textureIndex = pComponent.GetModelTextureMappings().GetAt( face.indexTexture );
			if( textureIndex == -1 ){
				continue;
			}
			
			if( ! pComponent.GetTextureAt( textureIndex ).GetAffectsSound() ){
				continue;
			}
			
			pSetResult( lambda, hitPoint, face.normal, face.indexFace );
		}
#ifdef MOVRAYHITSFACES_DO_TIMIING
		timing += timer.GetElapsedTime();
		timingCount += count;
#endif
		
	}else{
		const deoalModelRTBVH::sNode &child1 = bvh.GetNodes()[ node.node1 ];
		const deoalModelRTBVH::sNode &child2 = bvh.GetNodes()[ node.node2 ];
		float closestDistance;
		
		if( ( child2.center - child1.center ) * GetRayDirection() > 0.0f ){
			if( pRayHitsBox( child1.center, child1.halfSize, closestDistance )
			&& closestDistance < pLimitDistance ){
				pVisitNode( bvh, child1 );
			}
			
			if( pRayHitsBox( child2.center, child2.halfSize, closestDistance )
			&& closestDistance < pLimitDistance ){
				pVisitNode( bvh, child2 );
			}
			
		}else{
			if( pRayHitsBox( child2.center, child2.halfSize, closestDistance )
			&& closestDistance < pLimitDistance ){
				pVisitNode( bvh, child2 );
			}
			
			if( pRayHitsBox( child1.center, child1.halfSize, closestDistance )
			&& closestDistance < pLimitDistance ){
				pVisitNode( bvh, child1 );
			}
		}
	}
	
}

bool deoalMOVRayHitsClosest::pRayHitsBox( const decVector &center, const decVector &halfExtends,
float &closestDistance ){
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

void deoalMOVRayHitsClosest::pSetResult( float distance, const decVector &point,
const decVector &normal, int face ){
	pResultDistance = distance;
	pResultPoint = point;
	pResultNormal = &normal;
	pResultFace = face;
	pHasResult = true;
	pLimitDistance = distance;
}
