/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "deoglREnvMapProbe.h"
#include "deoglEnvironmentMap.h"
#include "../configuration/deoglConfiguration.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../world/deoglRWorld.h"
#include "../world/deoglWorldOctree.h"

#include <dragengine/common/exceptions.h>
#include "../utils/collision/deoglDCollisionBox.h"
#include <dragengine/common/shape/decShapeVisitor.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeList.h>



class cReflectionShapeVisitor : public decShapeVisitor{
private:
	deoglREnvMapProbe &pProbe;
	
public:
	cReflectionShapeVisitor( deoglREnvMapProbe &probe ) : pProbe( probe ){}
	virtual ~cReflectionShapeVisitor(){}
	
	virtual void VisitShape( decShape &shape ){
		pProbe.GetEnvironmentMap()->SetHasReflectionBox( false );
	}
	
	virtual void VisitShapeBox( decShapeBox &box ){
		pProbe.GetEnvironmentMap()->SetMatrixReflectionBox(
			decDMatrix::CreateScale( decDVector( box.GetHalfExtends() ) )
			* decDMatrix::CreateWorld( decDVector( box.GetPosition() ), box.GetOrientation() )
			* pProbe.GetMatrix() );
		pProbe.GetEnvironmentMap()->SetHasReflectionBox( true );
	}
};



class cReflectionMaskShapeVisitor : public decShapeVisitor{
private:
	deoglREnvMapProbe &pProbe;
	
public:
	cReflectionMaskShapeVisitor( deoglREnvMapProbe &probe ) : pProbe( probe ){}
	virtual ~cReflectionMaskShapeVisitor(){}
	
	virtual void VisitShapeBox( decShapeBox &box ){
		pProbe.GetEnvironmentMap()->AddReflectionMaskBoxMatrix(
			decDMatrix::CreateScale( decDVector( box.GetHalfExtends() ) )
			* decDMatrix::CreateWorld( decDVector( box.GetPosition() ), box.GetOrientation() )
			* pProbe.GetMatrix() );
	}
};



class cInfluenceShapeVisitor : public decShapeVisitor{
private:
	deoglREnvMapProbe &pProbe;
	
public:
	cInfluenceShapeVisitor( deoglREnvMapProbe &probe ) : pProbe( probe ){}
	virtual ~cInfluenceShapeVisitor(){}
	
	virtual void VisitShape( decShape &shape ){
		deoglEnvironmentMap &envMap = *pProbe.GetEnvironmentMap();
		
		envMap.SetInfluenceBoxScale( decVector( 1.0f, 1.0f, 1.0f ) );
		envMap.SetHasInfluenceBox( false );
	}
	
	virtual void VisitShapeBox( decShapeBox &box ){
		deoglEnvironmentMap &envMap = *pProbe.GetEnvironmentMap();
		const decVector &extends = box.GetHalfExtends();
		const decVector &position = box.GetPosition();
		const float invBorderSize = 1.0f / pProbe.GetInfluenceBorderSize();
		const decDMatrix matrixBase = decDMatrix::CreateWorld( decDVector( position ), box.GetOrientation() ) * pProbe.GetMatrix();
		
		envMap.SetMatrixInfluenceBox( decDMatrix::CreateScale( decDVector( extends ) ) * matrixBase );
		
		envMap.SetInfluenceBoxScale( extends );
		envMap.SetInfluenceBoxBorderFactor( decVector( extends.x * invBorderSize, extends.y * invBorderSize, extends.z * invBorderSize ) );
		
		envMap.GetInfluenceCollisionBox().SetCenter( matrixBase.GetPosition() );
		envMap.GetInfluenceCollisionBox().SetHalfSize( decDVector( extends ) );
		envMap.GetInfluenceCollisionBox().SetOrientation( matrixBase.ToQuaternion() );
		
		envMap.SetHasInfluenceBox( true );
	}
};



// Class deoglREnvMapProbe
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglREnvMapProbe::deoglREnvMapProbe( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),

pParentWorld( NULL ),
pOctreeNode( NULL ),
pDirtyOctreeNode( true ),

pInfluenceBorderSize( 0.0f ),

pEnvMap( NULL ),

pWorldMarkedRemove( false )
{
	try{
		pEnvMap = new deoglEnvironmentMap( renderThread );
		pEnvMap->SetSize( renderThread.GetConfiguration().GetEnvMapSize() );
		pEnvMap->SetIsFloat( true );
		pEnvMap->SetSkyOnly( false );
		LEAK_CHECK_CREATE( renderThread, EnvironmentMapProbe );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglREnvMapProbe::~deoglREnvMapProbe(){
	LEAK_CHECK_FREE( pRenderThread, EnvironmentMapProbe );
	pCleanUp();
}



// Management
///////////////

void deoglREnvMapProbe::SetParentWorld( deoglRWorld *world ){
	if( world == pParentWorld ){
		return;
	}
	
	if( pParentWorld && pEnvMap ){
		pParentWorld->RemoveEnvMap( pEnvMap );
	}
	
	pParentWorld = world;
	
	if( world && pEnvMap ){
		world->AddEnvMap( pEnvMap );
	}
	
	if( pOctreeNode ){
		//pOctreeNode->GetEnvMapProbeList().Remove( this );
		pOctreeNode = NULL;
	}
	pDirtyOctreeNode = true;
}

void deoglREnvMapProbe::SetOctreeNode( deoglWorldOctree *node ){
	pOctreeNode = node;
}



void deoglREnvMapProbe::SetInfluenceBorderSize( float borderSize ){
	pInfluenceBorderSize = decMath::max( borderSize, 0.0f );
}



void deoglREnvMapProbe::SetMatrix( const decDMatrix &matrix ){
	pMatrix = matrix;
}



void deoglREnvMapProbe::UpdateInfluenceShape( const decShapeList &shapeList ){
	const int count = shapeList.GetCount();
	
	if( count > 0 ){
		cInfluenceShapeVisitor visitor( *this );
		shapeList.GetAt( 0 )->Visit( visitor );
		
	}else{
		pEnvMap->SetInfluenceBoxScale( decVector( 1.0f, 1.0f, 1.0f ) );
		pEnvMap->SetHasInfluenceBox( false );
	}
}

void deoglREnvMapProbe::UpdateReflectionShape( const decShapeList &maskShapeList, decShape *shape ){
	if( shape ){
		cReflectionShapeVisitor visitor( *this );
		shape->Visit( visitor );
		
	}else{
		pEnvMap->SetHasReflectionBox( false );
	}
	
	pEnvMap->RemoveAllReflectionMaskBoxMatrices();
	if( maskShapeList.GetCount() > 0 ){
		cReflectionMaskShapeVisitor visitor( *this );
		int i;
		
		for( i=0; i<maskShapeList.GetCount(); i++ ){
			maskShapeList.GetAt( i )->Visit( visitor );
		}
	}
}



void deoglREnvMapProbe::PrepareQuickDispose(){
	pParentWorld = NULL;
	pOctreeNode = NULL;
}



// Render world usage
///////////////////////

void deoglREnvMapProbe::SetWorldMarkedRemove( bool marked ){
	pWorldMarkedRemove = marked;
}



// Private functions
//////////////////////

void deoglREnvMapProbe::pCleanUp(){
	SetParentWorld( NULL );
	
	if( pEnvMap ){
		pEnvMap->FreeReference();
	}
}
