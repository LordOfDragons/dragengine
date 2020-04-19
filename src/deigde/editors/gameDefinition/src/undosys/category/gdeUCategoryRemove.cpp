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

#include "gdeUCategoryRemove.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/category/gdeCategory.h"

#include <dragengine/common/exceptions.h>



// Class gdeUCategoryRemove
/////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUCategoryRemove::gdeUCategoryRemove( gdeGameDefinition *gameDefinition,
gdeCategory *category, eCategoryType type ) :
gdeUCategoryBase( gameDefinition, type ),
pParent( NULL ),
pCategory( NULL )
{
	if( ! category ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Category remove" );
	
	gdeCategory * const parent = category->GetParent();
	if( parent ){
		if( ! parent->GetCategories().Has( category ) ){
		   DETHROW( deeInvalidParam );
		}
		
		pParent = parent;
		parent->AddReference();
		
	}else{
		switch( type ){
		case ectObjectClass:
			if( ! gameDefinition->GetCategoriesObjectClass().Has( category ) ){
				DETHROW( deeInvalidParam );
			}
			break;
			
		case ectParticleEmitter:
			if( ! gameDefinition->GetCategoriesParticleEmitter().Has( category ) ){
				DETHROW( deeInvalidParam );
			}
			break;
			
		case ectSkin:
			if( ! gameDefinition->GetCategoriesSkin().Has( category ) ){
				DETHROW( deeInvalidParam );
			}
			break;
			
		case ectSky:
			if( ! gameDefinition->GetCategoriesSky().Has( category ) ){
				DETHROW( deeInvalidParam );
			}
			break;
			
			
		default:
			DETHROW( deeInvalidParam );
		}
	}
	
	pCategory = category;
	category->AddReference();
}

gdeUCategoryRemove::~gdeUCategoryRemove(){
	if( pCategory ){
		pCategory->FreeReference();
	}
	if( pParent ){
		pParent->FreeReference();
	}
}



// Management
///////////////

void gdeUCategoryRemove::Undo(){
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
}

void gdeUCategoryRemove::Redo(){
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
