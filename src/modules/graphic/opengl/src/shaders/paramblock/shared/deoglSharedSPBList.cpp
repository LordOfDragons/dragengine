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

#include "deoglSharedSPB.h"
#include "deoglSharedSPBList.h"
#include "deoglSharedSPBElement.h"

#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>



// Class deoglSharedSPBList
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSharedSPBList::deoglSharedSPBList( deoglRenderThread &renderThread, deoglShaderParameterBlock *layout ) :
pRenderThread( renderThread ),
pLayout( layout )
{
	DEASSERT_NOTNULL( layout )
	DEASSERT_TRUE( layout->GetElementCount() >= 1 )
	
	pSize = layout->GetElementCount();
}

deoglSharedSPBList::~deoglSharedSPBList(){
	pSPBs.RemoveAll();
}



// Management
///////////////

int deoglSharedSPBList::GetSize() const{
	return pLayout->GetElementCount();
}

int deoglSharedSPBList::GetCount() const{
	return pSPBs.GetCount();
}

deoglSharedSPB *deoglSharedSPBList::GetAt( int index ) const{
	return ( deoglSharedSPB* )pSPBs.GetAt( index );
}

deoglSharedSPBElement *deoglSharedSPBList::AddElement(){
	const int count = pSPBs.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglSharedSPBElement * const element = ( ( deoglSharedSPB* )pSPBs.GetAt( i ) )->AddElement();
		if( element ){
			return element;
		}
	}
	
	const deoglSharedSPB::Ref spb( deoglSharedSPB::Ref::New( new deoglSharedSPB( pCreateBlock() ) ) );
	pSPBs.Add( spb );
	
	return spb->AddElement();
}
