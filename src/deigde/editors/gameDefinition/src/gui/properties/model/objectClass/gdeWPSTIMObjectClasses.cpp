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

#include "gdeWPSTIMObjectClass.h"
#include "gdeWPSTIMObjectClasses.h"
#include "../../../gdeWindowMain.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gui/gdeWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeTreeItemReference.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

gdeWPSTIMObjectClasses::gdeWPSTIMObjectClasses( gdeWPSTreeModel &tree ) :
gdeWPSTreeItemModel( tree, etObjectClasses )
{
	SetText( "Object Classes" );
	SetIcon( GetWindowMain().GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ) );
}

gdeWPSTIMObjectClasses::~gdeWPSTIMObjectClasses(){
}



// Management
///////////////

gdeWPSTIMObjectClass *gdeWPSTIMObjectClasses::GetChildWith( gdeObjectClass *objectClass ) const{
	gdeWPSTIMObjectClass *child = ( gdeWPSTIMObjectClass* )GetFirstChild();
	
	while( child ){
		if( child->GetObjectClass() == objectClass ){
			return child;
		}
		child = ( gdeWPSTIMObjectClass* )child->GetNext();
	}
	
	return NULL;
}

void gdeWPSTIMObjectClasses::StructureChanged(){
	const gdeObjectClassList &list = GetGameDefinition().GetObjectClasses();
	const int count = list.GetCount();
	igdeTreeItemReference item;
	int i;
	
	// update existing and add new categories
	for( i=0; i<count; i++ ){
		gdeObjectClass * const objectClass = list.GetAt( i );
		gdeWPSTIMObjectClass * const modelObjectClass = GetChildWith( objectClass );
		
		if( ! modelObjectClass ){
			item.TakeOver( new gdeWPSTIMObjectClass( GetTree(), list.GetAt( i ) ) );
			AppendModel( item );
		}
	}
	
	// remove no more existing categories
	igdeTreeItem *child = GetFirstChild();
	while( child ){
		gdeWPSTIMObjectClass * const modelObjectClass = ( gdeWPSTIMObjectClass* )child;
		child = child->GetNext();
		
		if( ! list.Has( modelObjectClass->GetObjectClass() ) ){
			RemoveModel( modelObjectClass );
		}
	}
	
	// sort
	SortChildren();
	
	// validate
	ValidateObjectClassName();
}

void gdeWPSTIMObjectClasses::ValidateObjectClassName(){
	gdeWPSTIMObjectClass *child = ( gdeWPSTIMObjectClass* )GetFirstChild();
	while( child ){
		child->ValidateObjectClassName();
		child = ( gdeWPSTIMObjectClass* )child->GetNext();
	}
}

void gdeWPSTIMObjectClasses::ValidateCategoryName(){
	gdeWPSTIMObjectClass *child = ( gdeWPSTIMObjectClass* )GetFirstChild();
	while( child ){
		child->ValidateCategoryName();
		child = ( gdeWPSTIMObjectClass* )child->GetNext();
	}
}



void gdeWPSTIMObjectClasses::OnAddedToTree(){
	const gdeObjectClassList &list = GetGameDefinition().GetObjectClasses();
	const int count = list.GetCount();
	igdeTreeItemReference item;
	int i;
	
	for( i=0; i<count; i++ ){
		item.TakeOver( new gdeWPSTIMObjectClass( GetTree(), list.GetAt( i ) ) );
		AppendModel( item );
	}
	
	SortChildren();
}

void gdeWPSTIMObjectClasses::OnContextMenu( igdeMenuCascade &contextMenu ){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( contextMenu, windowMain.GetActionObjectClassAdd() );
	helper.MenuCommand( contextMenu, windowMain.GetActionObjectClassPaste() );
}

void gdeWPSTIMObjectClasses::SelectBestMatching( const char *string ){
	if( ! string ){
		return;
	}
	
	const decString searchString( decString( string ).GetLower() );
	igdeTreeItem *child = GetFirstChild();
	gdeObjectClass *bestObjectClass = NULL;
	decString bestName;
	
	while( child ){
		gdeObjectClass * const objectClass = ( ( gdeWPSTIMObjectClass* )child )->GetObjectClass();
		child = child->GetNext();
		
		const decString name( objectClass->GetName().GetLower() );
		
		if( name == searchString ){
			// exact match is always best
			bestObjectClass = objectClass;
			break;
		}
		
		if( name.FindString( searchString ) == -1 ){
			continue;
		}
		
		// partial matching. best match is the one earlier in the alphabet
		if( bestObjectClass && bestName < name ){
			continue;
		}
		
		bestObjectClass = objectClass;
		bestName = name;
	}

	if( bestObjectClass ){
		GetGameDefinition().SetActiveObjectClass( bestObjectClass );
		GetGameDefinition().SetSelectedObjectType( gdeGameDefinition::eotObjectClass );
	}
}
