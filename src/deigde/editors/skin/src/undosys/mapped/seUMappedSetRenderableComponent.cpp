/* 
 * Drag[en]gine IGDE Skin Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "seUMappedSetRenderableComponent.h"

#include <dragengine/common/exceptions.h>



// Class seUMappedSetRenderableComponent
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

seUMappedSetRenderableComponent::seUMappedSetRenderableComponent(
	seMapped *mapped, deSkinMapped::eRenderableComponent newComponent ) :
pMapped( mapped ),
pOldValue( mapped ? mapped->GetRenderableComponent() : deSkinMapped::ercRed ),
pNewValue( newComponent )
{
	DEASSERT_NOTNULL( pMapped )
	
	SetShortInfo( "Set Mapped Renderable Component" );
}

seUMappedSetRenderableComponent::~seUMappedSetRenderableComponent(){
}



// Management
///////////////

void seUMappedSetRenderableComponent::Undo(){
	pMapped->SetRenderableComponent( pOldValue );
}

void seUMappedSetRenderableComponent::Redo(){
	pMapped->SetRenderableComponent( pNewValue );
}
