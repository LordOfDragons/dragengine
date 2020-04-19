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

#include "deoglPropField.h"
#include "deoglPropFieldType.h"
#include "deoglRPropFieldType.h"
#include "../deGraphicOpenGl.h"
#include "../model/deoglModel.h"
#include "../model/deoglRModel.h"
#include "../skin/deoglRSkin.h"
#include "../skin/deoglSkin.h"
#include "../sky/deoglSky.h"
#include "../world/deoglWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/propfield/dePropFieldType.h>
#include <dragengine/resources/propfield/dePropFieldInstance.h>
#include <dragengine/resources/propfield/dePropFieldBendState.h>
#include <dragengine/resources/skin/deSkin.h>



// Class deoglPropFieldType
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPropFieldType::deoglPropFieldType( deoglPropField &propField, const dePropFieldType &type ) :
pPropField( propField ),
pType( type ),
pRType( NULL ),
 
pDirtyType( true ),
pDirtyInstances( true ),
pDirtyBendStates( true )
{
	try{
		pRType = new deoglRPropFieldType( *propField.GetRPropField() );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglPropFieldType::~deoglPropFieldType(){
	pCleanUp();
}



// Management
///////////////

void deoglPropFieldType::SyncToRender(){
	if( pDirtyType ){
		deoglRModel *model = NULL;
		deoglRSkin *skin = NULL;
		
		if( pType.GetModel() ){
			model = ( ( deoglModel* )pType.GetModel()->GetPeerGraphic() )->GetRModel();
		}
		if( pType.GetSkin() ){
			skin = ( ( deoglSkin* )pType.GetSkin()->GetPeerGraphic() )->GetRSkin();
		}
		
		pRType->SetModel( model );
		pRType->SetSkin( skin );
		
		pDirtyType = false;
		pDirtyInstances = true; // if model or skin changes instances change too
	}
	
	if( pDirtyInstances ){
		pRType->RebuildInstances( pType );
		pDirtyInstances = false;
		pDirtyBendStates = true;
	}
	
	if( pDirtyBendStates ){
		pRType->PrepareBendStateData( pType );
		pDirtyBendStates = false;
	}
}



void deoglPropFieldType::TypeChanged(){
	pDirtyType = true;
}

void deoglPropFieldType::InstancesChanged(){
	pDirtyInstances = true;
}

void deoglPropFieldType::AssignmentsChanged(){
	pDirtyInstances = true;
}

void deoglPropFieldType::BendStatesChanged(){
	pDirtyBendStates = true;
}

void deoglPropFieldType::InstanceCountChanged(){
	pDirtyInstances = true;
}


// Private functions
//////////////////////

void deoglPropFieldType::pCleanUp(){
	if( pRType ){
		pRType->FreeReference();
		pRType = NULL;
	}
}
