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

#include "gdeWPSTIMSkin.h"
#include "gdeWPSTIMSkins.h"
#include "../../../gdeWindowMain.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/skin/gdeSkin.h"
#include "../../../../gui/gdeWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeTreeItem::Ref.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

gdeWPSTIMSkins::gdeWPSTIMSkins( gdeWPSTreeModel &tree ) :
gdeWPSTreeItemModel( tree, etSkins )
{
	SetText( "Skins" );
	SetIcon( GetWindowMain().GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ) );
}

gdeWPSTIMSkins::~gdeWPSTIMSkins(){
}



// Management
///////////////

gdeWPSTIMSkin *gdeWPSTIMSkins::GetChildWith( gdeSkin *skin ) const{
	gdeWPSTIMSkin *child = ( gdeWPSTIMSkin* )GetFirstChild();
	
	while( child ){
		if( child->GetSkin() == skin ){
			return child;
		}
		child = ( gdeWPSTIMSkin* )child->GetNext();
	}
	
	return NULL;
}

void gdeWPSTIMSkins::StructureChanged(){
	const gdeSkinList &list = GetGameDefinition().GetSkins();
	const int count = list.GetCount();
	igdeTreeItem::Ref item;
	int i;
	
	// update existing and add new categories
	for( i=0; i<count; i++ ){
		gdeSkin * const skin = list.GetAt( i );
		gdeWPSTIMSkin * const modelSkin = GetChildWith( skin );
		
		if( ! modelSkin ){
			item.TakeOver( new gdeWPSTIMSkin( GetTree(), list.GetAt( i ) ) );
			AppendModel( item );
		}
	}
	
	// remove no more existing categories
	igdeTreeItem *child = GetFirstChild();
	while( child ){
		gdeWPSTIMSkin * const modelSkin = ( gdeWPSTIMSkin* )child;
		child = child->GetNext();
		
		if( ! list.Has( modelSkin->GetSkin() ) ){
			RemoveModel( modelSkin );
		}
	}
	
	// sort
	SortChildren();
}



void gdeWPSTIMSkins::OnAddedToTree(){
	const gdeSkinList &list = GetGameDefinition().GetSkins();
	const int count = list.GetCount();
	igdeTreeItem::Ref item;
	int i;
	
	for( i=0; i<count; i++ ){
		item.TakeOver( new gdeWPSTIMSkin( GetTree(), list.GetAt( i ) ) );
		AppendModel( item );
	}
	
	SortChildren();
}

void gdeWPSTIMSkins::OnContextMenu( igdeMenuCascade &contextMenu ){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( contextMenu, windowMain.GetActionSkinAdd() );
}

void gdeWPSTIMSkins::SelectBestMatching( const char *string ){
	if( ! string ){
		return;
	}
	
	const decString searchString( decString( string ).GetLower() );
	igdeTreeItem *child = GetFirstChild();
	
	while( child ){
		gdeSkin * const skin = ( ( gdeWPSTIMSkin* )child )->GetSkin();
		child = child->GetNext();
		
		if( skin->GetName().GetLower().FindString( searchString ) == -1 ){
			continue;
		}
		
		GetGameDefinition().SetActiveSkin( skin );
		GetGameDefinition().SetSelectedObjectType( gdeGameDefinition::eotSkin );
		return;
	}
}
