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

#include "gdeWPSTIMParticleEmitter.h"
#include "gdeWPSTIMParticleEmitters.h"
#include "../../../gdeWindowMain.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/particleemitter/gdeParticleEmitter.h"
#include "../../../../gui/gdeWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeTreeItemReference.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

gdeWPSTIMParticleEmitters::gdeWPSTIMParticleEmitters( gdeWPSTreeModel &tree ) :
gdeWPSTreeItemModel( tree, etParticleEmitters )
{
	SetText( "Particle Emitters" );
	SetIcon( GetWindowMain().GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ) );
}

gdeWPSTIMParticleEmitters::~gdeWPSTIMParticleEmitters(){
}



// Management
///////////////

gdeWPSTIMParticleEmitter *gdeWPSTIMParticleEmitters::GetChildWith(
gdeParticleEmitter *particleEmitter ) const{
	gdeWPSTIMParticleEmitter *child = ( gdeWPSTIMParticleEmitter* )GetFirstChild();
	
	while( child ){
		if( child->GetParticleEmitter() == particleEmitter ){
			return child;
		}
		child = ( gdeWPSTIMParticleEmitter* )child->GetNext();
	}
	
	return NULL;
}

void gdeWPSTIMParticleEmitters::StructureChanged(){
	const gdeParticleEmitterList &list = GetGameDefinition().GetParticleEmitters();
	const int count = list.GetCount();
	igdeTreeItemReference item;
	int i;
	
	// update existing and add new categories
	for( i=0; i<count; i++ ){
		gdeParticleEmitter * const particleEmitter = list.GetAt( i );
		gdeWPSTIMParticleEmitter * const modelParticleEmitter = GetChildWith( particleEmitter );
		
		if( ! modelParticleEmitter ){
			item.TakeOver( new gdeWPSTIMParticleEmitter( GetTree(), list.GetAt( i ) ) );
			AppendModel( item );
		}
	}
	
	// remove no more existing categories
	igdeTreeItem *child = GetFirstChild();
	while( child ){
		gdeWPSTIMParticleEmitter * const modelParticleEmitter = ( gdeWPSTIMParticleEmitter* )child;
		child = child->GetNext();
		
		if( ! list.Has( modelParticleEmitter->GetParticleEmitter() ) ){
			RemoveModel( modelParticleEmitter );
		}
	}
	
	// sort
	SortChildren();
}



void gdeWPSTIMParticleEmitters::OnAddedToTree(){
	const gdeParticleEmitterList &list = GetGameDefinition().GetParticleEmitters();
	const int count = list.GetCount();
	igdeTreeItemReference item;
	int i;
	
	for( i=0; i<count; i++ ){
		item.TakeOver( new gdeWPSTIMParticleEmitter( GetTree(), list.GetAt( i ) ) );
		AppendModel( item );
	}
	
	SortChildren();
}

void gdeWPSTIMParticleEmitters::OnContextMenu( igdeMenuCascade &contextMenu ){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( contextMenu, windowMain.GetActionParticleEmitterAdd() );
}

void gdeWPSTIMParticleEmitters::SelectBestMatching( const char *string ){
	if( ! string ){
		return;
	}
	
	const decString searchString( decString( string ).GetLower() );
	igdeTreeItem *child = GetFirstChild();
	
	while( child ){
		gdeParticleEmitter * const particleEmitter = ( ( gdeWPSTIMParticleEmitter* )child )->GetParticleEmitter();
		child = child->GetNext();
		
		if( particleEmitter->GetName().GetLower().FindString( searchString ) == -1 ){
			continue;
		}
		
		GetGameDefinition().SetActiveParticleEmitter( particleEmitter );
		GetGameDefinition().SetSelectedObjectType( gdeGameDefinition::eotParticleEmitter );
		return;
	}
}
