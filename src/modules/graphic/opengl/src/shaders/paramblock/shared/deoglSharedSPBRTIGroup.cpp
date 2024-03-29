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

#include "deoglSharedSPBRTIGroup.h"
#include "deoglSharedSPBRTIGroupList.h"
#include "../../../rendering/task/shared/deoglRenderTaskSharedPool.h"
#include "../../../rendering/task/shared/deoglRenderTaskSharedInstance.h"
#include "../../../renderthread/deoglRenderThread.h"
#include "../../../renderthread/deoglRTUniqueKey.h"

#include <dragengine/common/exceptions.h>



// Class deoglSharedSPBRTIGroup
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSharedSPBRTIGroup::deoglSharedSPBRTIGroup( deoglSharedSPBRTIGroupList *parent,
deoglSharedSPB &sharedSPB, int textureCount ) :
pParent( parent ),
pSharedSPB( sharedSPB ),
pTextureCount( textureCount ),
pRTSInstance( NULL )
{
	DEASSERT_NOTNULL( parent )
	
	pRTSInstance = parent->GetRenderThread().GetRenderTaskSharedPool().GetInstance();
	pUniqueKey = parent->GetRenderThread().GetUniqueKey().Get();
}

deoglSharedSPBRTIGroup::~deoglSharedSPBRTIGroup(){
	if( pRTSInstance ){
		pRTSInstance->ReturnToPool();
	}
	
	pParent->GetRenderThread().GetUniqueKey().Return( pUniqueKey );
	pParent->Remove( this );
}
