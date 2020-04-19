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

#include "gdeMACategoryAdd.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/category/gdeCategory.h"
#include "../../../undosys/category/gdeUCategoryAdd.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>

#include <dragengine/deEngine.h>
#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>



// Class gdeMACategoryAdd
///////////////////////////

// Constructor
////////////////

gdeMACategoryAdd::gdeMACategoryAdd( gdeWindowMain &windowMain ) :
gdeBaseAction( windowMain, "Add Child Category...",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ),
	"Add category to active category" )
{
}

gdeMACategoryAdd::gdeMACategoryAdd( gdeWindowMain &windowMain, const char *text,
	igdeIcon *icon, const char *description ) :
gdeBaseAction( windowMain, text, icon, description )
{
}



// Management
///////////////

igdeUndo *gdeMACategoryAdd::OnAction( gdeGameDefinition &gameDefinition ){
	gdeUCategoryBase::eCategoryType categoryType;
	
	switch( gameDefinition.GetSelectedObjectType() ){
	case gdeGameDefinition::eotCategoryObjectClass:
		categoryType = gdeUCategoryBase::ectObjectClass;
		break;
		
	case gdeGameDefinition::eotCategoryParticleEmitter:
		categoryType = gdeUCategoryBase::ectParticleEmitter;
		break;
		
	case gdeGameDefinition::eotCategorySkin:
		categoryType = gdeUCategoryBase::ectSkin;
		break;
		
	case gdeGameDefinition::eotCategorySky:
		categoryType = gdeUCategoryBase::ectSky;
		break;
		
	default:
		return NULL;
	}
	
	gdeCategory * const category = gameDefinition.GetActiveCategory();
	if( ! category ){
		return NULL;
	}
	
	return AddCategory( gameDefinition, category, category->GetCategories(), categoryType );
}



igdeUndo *gdeMACategoryAdd::AddCategory( gdeGameDefinition &gameDefinition,
gdeCategory *parent, const gdeCategoryList &list, gdeUCategoryBase::eCategoryType categoryType ) const{
	decString name( "Category" );
	
	while( igdeCommonDialogs::GetString( &pWindowMain, "Add Category", "Name:", name ) ){
		if( list.HasNamed( name ) ){
			igdeCommonDialogs::Error( &pWindowMain, "Add Category", "Category exists already." );
			continue;
		}
		
		deObjectReference category;
		category.TakeOver( new gdeCategory( name ) );
		return new gdeUCategoryAdd( &gameDefinition, parent,
			( gdeCategory* )( deObject* )category, categoryType );
	}
	
	return NULL;
}
