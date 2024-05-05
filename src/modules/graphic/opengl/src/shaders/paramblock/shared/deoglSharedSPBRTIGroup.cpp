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
