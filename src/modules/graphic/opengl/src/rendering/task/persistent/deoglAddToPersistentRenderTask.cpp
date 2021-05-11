/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglAddToPersistentRenderTask.h"
#include "deoglPersistentRenderTask.h"
#include "deoglPersistentRenderTaskInstance.h"
#include "deoglPersistentRenderTaskShader.h"
#include "deoglPersistentRenderTaskTexture.h"
#include "deoglPersistentRenderTaskVAO.h"
#include "deoglPersistentRenderTaskSubInstance.h"
#include "deoglPersistentRenderTaskOwner.h"
#include "../../../collidelist/deoglCollideListComponent.h"
#include "../../../component/deoglRComponent.h"
#include "../../../component/deoglRComponentTexture.h"
#include "../../../model/deoglModelLOD.h"
#include "../../../model/deoglRModel.h"
#include "../../../model/texture/deoglModelTexture.h"
#include "../../../renderthread/deoglRTShader.h"
#include "../../../renderthread/deoglRenderThread.h"
#include "../../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../../shaders/paramblock/shared/deoglSharedSPB.h"
#include "../../../shaders/paramblock/shared/deoglSharedSPBElement.h"
#include "../../../shaders/paramblock/shared/deoglSharedSPBRTIGroup.h"
#include "../../../skin/dynamic/deoglRDynamicSkin.h"
#include "../../../skin/dynamic/renderables/render/deoglRDSRenderable.h"
#include "../../../skin/shader/deoglSkinShader.h"
#include "../../../skin/state/deoglSkinState.h"
#include "../../../skin/state/deoglSkinStateRenderable.h"
#include "../../../texture/texunitsconfig/deoglTexUnitsConfigList.h"

#include <dragengine/common/exceptions.h>



// Class deoglAddToPersistentRenderTask
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglAddToPersistentRenderTask::deoglAddToPersistentRenderTask(
	deoglRenderThread &renderThread, deoglPersistentRenderTask &renderTask ) :
pRenderThread( renderThread  ),
pRenderTask( renderTask  ),

pSkinShaderType( deoglSkinTexture::estComponentGeometry ),

pSolid( false ),
pNoShadowNone( false ),
pNoNotReflected( false ),
pNoRendered( false ),
pOutline( false ),

pFilterHoles( false ),
pWithHoles( false ),

pFilterDoubleSided( false ),
pDoubleSided( false ),
pForceDoubleSided( false ),

pFilterDecal( false ),
pDecal( false ),

pFilterCubeFace( -1 ),

pUseSpecialParamBlock( false ),

pEnforceShader( NULL ),
pEnforceParamBlock( NULL ){
}

deoglAddToPersistentRenderTask::~deoglAddToPersistentRenderTask(){
}



// Management
///////////////

void deoglAddToPersistentRenderTask::SetSkinShaderType( deoglSkinTexture::eShaderTypes shaderType ){
	pSkinShaderType = shaderType;
}

void deoglAddToPersistentRenderTask::SetSolid( bool solid ){
	pSolid = solid;
}

void deoglAddToPersistentRenderTask::SetNoNotReflected( bool noNotReflected ){
	pNoNotReflected = noNotReflected;
}

void deoglAddToPersistentRenderTask::SetNoRendered( bool noRendered ){
	pNoRendered = noRendered;
}

void deoglAddToPersistentRenderTask::SetOutline( bool outline ){
	pOutline = outline;
}

void deoglAddToPersistentRenderTask::SetNoShadowNone( bool noShadowNone ){
	pNoShadowNone = noShadowNone;
}

void deoglAddToPersistentRenderTask::SetFilterHoles( bool filterHoles ){
	pFilterHoles = filterHoles;
}

void deoglAddToPersistentRenderTask::SetWithHoles( bool withHoles ){
	pWithHoles = withHoles;
}

void deoglAddToPersistentRenderTask::SetFilterDoubleSided( bool filterDoubleSided ){
	pFilterDoubleSided = filterDoubleSided;
}
void deoglAddToPersistentRenderTask::SetDoubleSided( bool doubleSided ){
	pDoubleSided = doubleSided;
}

void deoglAddToPersistentRenderTask::SetForceDoubleSided( bool doubleSided ){
	pForceDoubleSided = doubleSided;
}

void deoglAddToPersistentRenderTask::SetFilterDecal( bool filterDecal ){
	pFilterDecal = filterDecal;
}

void deoglAddToPersistentRenderTask::SetDecal( bool decal ){
	pDecal = decal;
}

void deoglAddToPersistentRenderTask::SetFilterCubeFace( int cubeFace ){
	pFilterCubeFace = cubeFace;
}

void deoglAddToPersistentRenderTask::SetUseSpecialParamBlock( bool use ){
	pUseSpecialParamBlock = use;
}

void deoglAddToPersistentRenderTask::SetEnforceShader( const deoglShaderProgram *shader ){
	pEnforceShader = shader;
}

void deoglAddToPersistentRenderTask::SetEnforceParamBlock( const deoglSPBlockUBO *block ){
	pEnforceParamBlock = block;
}

void deoglAddToPersistentRenderTask::AddComponent( deoglPersistentRenderTaskOwner &owner,
const deoglRComponent &component, int lodLevel ){
	if( ! component.GetParentWorld() || ! component.GetModel() ){
		return;
	}
	if( pFilterCubeFace != -1 && ! component.GetCubeFaceVisible( pFilterCubeFace ) ){
		return;
	}
	
	const deoglRModel &model = *component.GetModel();
	const deoglModelLOD &modelLOD = model.GetLODAt( lodLevel );
	const int textureCount = modelLOD.GetTextureCount();
	int i;
	
	for( i=0; i<textureCount; i++ ){
		const deoglModelTexture &texture = modelLOD.GetTextureAt( i );
		if( texture.GetFaceCount() > 0 ){
			AddComponentFaces( owner, component, i, texture.GetFirstFace(), texture.GetFaceCount(), lodLevel );
		}
	}
}

void deoglAddToPersistentRenderTask::AddComponent( deoglPersistentRenderTaskOwner &owner,
const deoglCollideListComponent &clcomponent ){
	AddComponent( owner, *clcomponent.GetComponent(), clcomponent.GetLODLevel() );
}

void deoglAddToPersistentRenderTask::AddComponentFaces( deoglPersistentRenderTaskOwner &owner,
const deoglRComponent &component, int texture, int lodLevel ){
	if( ! component.GetModel() ){
		return;
	}
	
	const deoglModelTexture &t = component.GetModel()->GetLODAt( lodLevel ).GetTextureAt( texture );
	if( t.GetFaceCount() > 0 ){
		AddComponentFaces( owner, component, texture, t.GetFirstFace(), t.GetFaceCount(), lodLevel );
	}
}

void deoglAddToPersistentRenderTask::AddComponentFaces( deoglPersistentRenderTaskOwner &owner,
const deoglRComponent &component, int texture, int firstFace, int faceCount, int lodLevel ){
	const deoglRComponentTexture &componentTexture = component.GetTextureAt( texture );
	if( componentTexture.GetIsRendered() ){
		return;
	}
	
	const deoglSkinTexture * const skinTexture = componentTexture.GetUseSkinTexture();
	if( ! skinTexture ){
		return;
	}
	if( pFilterReject( skinTexture ) ){
		return;
	}
	
	const bool doubleSided = componentTexture.GetUseDoubleSided();
	if( pFilterDoubleSided && pDoubleSided != doubleSided ){
		return;
	}
	if( pFilterDecal && pDecal != componentTexture.GetUseDecal() ){
		return;
	}
	
	// obtain render task vao and add faces
	deoglPersistentRenderTaskVAO * const rtvao = pGetTaskVAO( pSkinShaderType, *skinTexture,
		componentTexture.GetTUCForShaderType( pSkinShaderType ), component.GetVAO( lodLevel ) );
	
	const deoglSharedSPBElement &spbElement = *componentTexture.GetSharedSPBElement();
	const deoglSharedSPBRTIGroup &group = componentTexture.GetSharedSPBRTIGroup( lodLevel );
	
	deoglPersistentRenderTaskInstance *rti = rtvao->GetInstanceWith( &group );
	if( ! rti ){
		rti = rtvao->AddInstance( &spbElement.GetSPB(), &group );
		rti->SetFirstPoint( component.GetPointOffset( lodLevel ) );
		rti->SetFirstIndex( component.GetIndexOffset( lodLevel ) + firstFace * 3 );
		rti->SetIndexCount( faceCount * 3 );
		rti->SetDoubleSided( doubleSided | pForceDoubleSided );
	}
	
	owner.AddSubInstance( rti->AddSubInstance( spbElement.GetIndex(), component.GetSpecialFlags() ) );
}



// Private Functions
//////////////////////

bool deoglAddToPersistentRenderTask::pFilterReject( const deoglSkinTexture *skinTexture ) const{
	if( pOutline ){
		if( ! skinTexture->GetHasOutline() ){
			return true;
		}
		if( pSolid != skinTexture->GetIsOutlineSolid() ){
			return true;
		}
		
	}else{
		if( pSolid != skinTexture->GetSolid() ){
			return true;
		}
	}
	
	if( pFilterRejectNoSolid( skinTexture ) ){
		return true;
	}
	return false;
}

bool deoglAddToPersistentRenderTask::pFilterRejectNoSolid( const deoglSkinTexture *skinTexture ) const{
	if( ! skinTexture ){
		return true;
	}
	if( pFilterHoles && pWithHoles != skinTexture->GetHasHoles() ){
		return true;
	}
	if( pNoRendered && skinTexture->GetRendered() ){
		return true;
	}
	if( pNoShadowNone && skinTexture->GetShadowNone() ){
		return true;
	}
	if( pNoNotReflected && ! skinTexture->GetReflected() ){
		return true;
	}
	return false;
}

deoglPersistentRenderTaskVAO *deoglAddToPersistentRenderTask::pGetTaskVAO(
deoglSkinTexture::eShaderTypes shaderType, const deoglSkinTexture &skinTexture,
const deoglTexUnitsConfig *tuc, const deoglVAO *vao ) const{
	// retrieve the shader and texture units configuration to use
	const deoglShaderProgram *shader = pEnforceShader;
	int spbInstanceIndexBase = -1;
	
	if( ! shader ){
		const deoglSkinShader * const skinShader = skinTexture.GetShaderFor( shaderType );
		if( skinShader ){
			shader = skinShader->GetShader();
			spbInstanceIndexBase = skinShader->GetTargetSPBInstanceIndexBase();
		}
	}
	
	if( ! shader ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! tuc ){
		tuc = pRenderThread.GetShader().GetTexUnitsConfigList().GetEmptyNoUsage();
	}
	
	deoglPersistentRenderTaskShader *rtshader = pRenderTask.GetShaderWith( shader );
	if( ! rtshader ){
		rtshader = pRenderTask.AddShader( shader );
		rtshader->SetParameterBlock( NULL );
		rtshader->SetSPBInstanceIndexBase( spbInstanceIndexBase );
	}
	
	deoglPersistentRenderTaskTexture *rttexture = rtshader->GetTextureWith( tuc );
	if( ! rttexture ){
		rttexture = rtshader->AddTexture( tuc );
		rttexture->SetParameterBlock( skinTexture.GetSharedSPBElement()->GetSPB().GetParameterBlock() );
	}
	
	deoglPersistentRenderTaskVAO *rtvao = rttexture->GetVAOWith( vao );
	if( ! rtvao ){
		rtvao = rttexture->AddVAO( vao );
	}
	
	return rtvao;
}
