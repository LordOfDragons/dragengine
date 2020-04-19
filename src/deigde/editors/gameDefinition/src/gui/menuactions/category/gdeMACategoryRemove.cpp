/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include "gdeMACategoryRemove.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../undosys/category/gdeUCategoryRemove.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeMACategoryRemove
//////////////////////////////

// Constructor
////////////////

gdeMACategoryRemove::gdeMACategoryRemove( gdeWindowMain &windowMain ) :
gdeBaseAction( windowMain, "Remove Category",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ),
	"Remove category" )
{
}



// Management
///////////////

igdeUndo *gdeMACategoryRemove::OnAction( gdeGameDefinition &gameDefinition ){
	gdeCategory * const category = gameDefinition.GetActiveCategory();
	if( ! category ){
		return NULL;
	}
	
	switch( gameDefinition.GetSelectedObjectType() ){
	case gdeGameDefinition::eotCategoryObjectClass:
		return new gdeUCategoryRemove( &gameDefinition, category, gdeUCategoryBase::ectObjectClass );
		
	case gdeGameDefinition::eotCategoryParticleEmitter:
		return new gdeUCategoryRemove( &gameDefinition, category, gdeUCategoryBase::ectParticleEmitter );
		
	case gdeGameDefinition::eotCategorySkin:
		return new gdeUCategoryRemove( &gameDefinition, category, gdeUCategoryBase::ectSkin );
		
	case gdeGameDefinition::eotCategorySky:
		return new gdeUCategoryRemove( &gameDefinition, category, gdeUCategoryBase::ectSky );
		
	default:
		return NULL;
	}
}

void gdeMACategoryRemove::Update(){
	gdeGameDefinition * const gameDefinition = pWindowMain.GetActiveGameDefinition();
	if( ! gameDefinition ){
		SetEnabled( false );
		return;
	}
	
	switch( gameDefinition->GetSelectedObjectType() ){
	case gdeGameDefinition::eotCategoryObjectClass:
	case gdeGameDefinition::eotCategoryParticleEmitter:
	case gdeGameDefinition::eotCategorySkin:
	case gdeGameDefinition::eotCategorySky:
		SetEnabled( gameDefinition->GetActiveCategory() != NULL );
		break;
		
	default:
		SetEnabled( false );
		break;
	}
}
