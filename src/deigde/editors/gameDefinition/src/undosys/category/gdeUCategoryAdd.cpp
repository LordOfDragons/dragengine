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
#include <string.h>
#include <stdlib.h>

#include "gdeUCategoryAdd.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/category/gdeCategory.h"

#include <dragengine/common/exceptions.h>



// Class gdeUCategoryAdd
//////////////////////////

// Constructor, destructor
////////////////////////////

gdeUCategoryAdd::gdeUCategoryAdd( gdeGameDefinition *gameDefinition, gdeCategory *parent,
gdeCategory *category, eCategoryType type ) :
gdeUCategoryBase( gameDefinition, type ),
pParent( NULL ),
pCategory( NULL )
{
	if( ! category ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Category add" );
	
	pParent = parent;
	if( parent ){
		parent->AddReference();
	}
	
	pCategory = category;
	category->AddReference();
}

gdeUCategoryAdd::~gdeUCategoryAdd(){
	if( pCategory ){
		pCategory->FreeReference();
	}
	if( pParent ){
		pParent->FreeReference();
	}
}



// Management
///////////////

void gdeUCategoryAdd::Undo(){
	gdeGameDefinition &gameDefinition = *GetGameDefinition();
	
	gdeCategory *activeCheck = gameDefinition.GetActiveCategory();
	while( activeCheck ){
		if( activeCheck == pCategory ){
			switch( gameDefinition.GetSelectedObjectType() ){
			case gdeGameDefinition::eotCategoryObjectClass:
			case gdeGameDefinition::eotCategoryParticleEmitter:
			case gdeGameDefinition::eotCategorySkin:
			case gdeGameDefinition::eotCategorySky:
				if( ! activeCheck->GetParent() ){
					gameDefinition.SetSelectedObjectType( gdeGameDefinition::eotNoSelection );
				}
				break;
				
			default:
				break;
			}
			
			gameDefinition.SetActiveCategory( activeCheck->GetParent() );
			break;
		}
		
		activeCheck = activeCheck->GetParent();
	}
	
	if( pParent ){
		pParent->RemoveCategory( pCategory );
		
	}else{
		switch( GetType() ){
		case ectObjectClass:
			gameDefinition.GetCategoriesObjectClass().Remove( pCategory );
			break;
			
		case ectParticleEmitter:
			gameDefinition.GetCategoriesParticleEmitter().Remove( pCategory );
			break;
			
		case ectSkin:
			gameDefinition.GetCategoriesSkin().Remove( pCategory );
			break;
			
		case ectSky:
			gameDefinition.GetCategoriesSky().Remove( pCategory );
			break;
			
			
		default:
			DETHROW( deeInvalidParam );
		}
	}
	
	Notify();
}

void gdeUCategoryAdd::Redo(){
	gdeGameDefinition &gameDefinition = *GetGameDefinition();
	
	if( pParent ){
		pParent->AddCategory( pCategory );
		
	}else{
		switch( GetType() ){
		case ectObjectClass:
			gameDefinition.GetCategoriesObjectClass().Add( pCategory );
			break;
			
		case ectParticleEmitter:
			gameDefinition.GetCategoriesParticleEmitter().Add( pCategory );
			break;
			
		case ectSkin:
			gameDefinition.GetCategoriesSkin().Add( pCategory );
			break;
			
		case ectSky:
			gameDefinition.GetCategoriesSky().Add( pCategory );
			break;
			
		default:
			DETHROW( deeInvalidParam );
		}
	}
	
	Notify();
	
	gameDefinition.SetActiveCategory( pCategory );
	
	switch( GetType() ){
	case ectObjectClass:
		gameDefinition.SetSelectedObjectType( gdeGameDefinition::eotCategoryObjectClass );
		break;
		
	case ectParticleEmitter:
		gameDefinition.SetSelectedObjectType( gdeGameDefinition::eotCategoryParticleEmitter );
		break;
		
	case ectSkin:
		gameDefinition.SetSelectedObjectType( gdeGameDefinition::eotCategorySkin );
		break;
		
	case ectSky:
		gameDefinition.SetSelectedObjectType( gdeGameDefinition::eotCategorySky );
		break;
		
	default:
		break;
	}
}
