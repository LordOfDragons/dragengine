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
		
		const gdeCategory::Ref category(gdeCategory::Ref::NewWith(name));
		return new gdeUCategoryAdd( &gameDefinition, parent,
			category, categoryType );
	}
	
	return NULL;
}
