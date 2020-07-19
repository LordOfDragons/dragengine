/* 
 * Drag[en]gine IGDE
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

#include "igdeNVNode.h"
#include "igdeNVNodeListener.h"
#include "igdeNVSlot.h"
#include "../native/toolkit.h"
#include "../igdeCommonDialogs.h"
#include "../igdeContainer.h"
#include "../igdeUIHelper.h"
#include "../event/igdeAction.h"
#include "../resources/igdeIcon.h"
#include "../resources/igdeFont.h"
#include "../resources/igdeFontReference.h"
#include "../theme/igdeGuiTheme.h"
#include "../menu/igdeMenuCascade.h"
#include "../menu/igdeMenuCascadeReference.h"
#include "../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>


// Class igdeNVNode
/////////////////////

// Constructor, destructor
////////////////////////////

igdeNVNode::igdeNVNode( igdeEnvironment &environment, const char *title, const char *description ) :
igdeContainer( environment ),
pTitle( title ),
pDescription( description ),
pEnabled( true ),
pActive( false ),
pBgColor( 150.0f / 255.0f, 150.0f / 255.0f, 150.0f / 255.0f ),
pBorderColor( 60.0f / 255.0f, 60.0f / 255.0f, 60.0f / 255.0f ),
pActiveTitleBgColor( 165.0f / 255.0f, 200.0f / 255.0f, 200.0f / 255.0f ),
pInactiveTitleBgColor( 150.0f / 255.0f, 150.0f / 255.0f, 185.0f / 255.0f ),
pOwnerBoard( NULL ){
}

igdeNVNode::~igdeNVNode(){
	DestroyNativeWidget();
	RemoveAllSlots();
}



// Management
///////////////

void igdeNVNode::SetTitle( const char *title ){
	if( pTitle == title ){
		return;
	}
	
	pTitle = title;
	OnTitleChanged();
}

void igdeNVNode::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeNVNode::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeNVNode::SetActive( bool active ){
	if( pActive == active ){
		return;
	}
	
	pActive = active;
	OnActiveChanged();
}



void igdeNVNode::SetBgColor( const decColor &color ){
	if( color.IsEqualTo( pBgColor ) ){
		return;
	}
	
	pBgColor = color;
	OnColorsChanged();
}

void igdeNVNode::SetBorderColor( const decColor &color ){
	if( color.IsEqualTo( pBorderColor ) ){
		return;
	}
	
	pBorderColor = color;
	OnColorsChanged();
}

void igdeNVNode::SetActiveTitleBgColor( const decColor &color ){
	if( color.IsEqualTo( pActiveTitleBgColor ) ){
		return;
	}
	
	pActiveTitleBgColor = color;
	OnColorsChanged();
}

void igdeNVNode::SetInactiveTitleBgColor( const decColor &color ){
	if( color.IsEqualTo( pInactiveTitleBgColor ) ){
		return;
	}
	
	pInactiveTitleBgColor = color;
	OnColorsChanged();
}



void igdeNVNode::SetPosition( const decPoint &position ){
	if( position == pPosition ){
		return;
	}
	
	pPosition = position;
	OnPositionChanged();
}

decPoint igdeNVNode::GetSize() const{
	if( ! GetNativeWidget() ){
		return decPoint();
	}
	
	return ( ( igdeNativeNVNode* )GetNativeWidget() )->GetSize();
}

void igdeNVNode::SetOwnerBoard( igdeNVBoard *board ){
	if( board == pOwnerBoard ){
		return;
	}
	
	pOwnerBoard = board;
	
	if( board ){
		OnPositionChanged();
	}
}

void igdeNVNode::OnBoardOffsetChanged(){
	OnPositionChanged();
}



int igdeNVNode::GetSlotCount() const{
	return pSlots.GetCount();
}

igdeNVSlot *igdeNVNode::GetSlotAt( int index ) const{
	return ( igdeNVSlot* )pSlots.GetAt( index );
}

int igdeNVNode::IndexOfSlot( igdeNVSlot *slot ) const{
	return pSlots.IndexOf( slot );
}

bool igdeNVNode::HasSlot( igdeNVSlot *slot ) const{
	return pSlots.Has( slot );
}

void igdeNVNode::AddSlot( igdeNVSlot *slot ){
	if( ! slot || HasSlot( slot ) ){
		DETHROW( deeInvalidParam );
	}
	
	igdeContainer::AddChild( slot );
	pSlots.Add( slot );
	slot->SetOwnerNode( this );
	OnSlotsChanged();
}

void igdeNVNode::RemoveSlot( igdeNVSlot *slot ){
	if( ! slot || ! HasSlot( slot ) ){
		DETHROW( deeInvalidParam );
	}
	
	igdeContainer::RemoveChild( slot );
	slot->SetOwnerNode( NULL );
	pSlots.Remove( slot );
	OnSlotsChanged();
}

void igdeNVNode::RemoveAllSlots(){
	if( pSlots.GetCount() == 0 ){
		return;
	}
	
	while( pSlots.GetCount() > 0 ){
		const int index = pSlots.GetCount() - 1;
		igdeNVSlot * const slot = ( igdeNVSlot* )pSlots.GetAt( index );
		slot->SetOwnerNode( NULL );
		igdeContainer::RemoveChild( slot );
		pSlots.RemoveFrom( index );
	}
	
	OnSlotsChanged();
}

void igdeNVNode::ShowContextMenu( const decPoint &position ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	igdeMenuCascadeReference menu;
	menu.TakeOver( new igdeMenuCascade( helper.GetEnvironment() ) );
	
	const int count = pListeners.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( igdeNVNodeListener* )pListeners.GetAt( i ) )->AddContextMenuEntries( this, menu );
	}
	
	if( menu->GetChildCount() > 0 ){
		menu->Popup( *this, position );
	}
}



void igdeNVNode::AddListener( igdeNVNodeListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void igdeNVNode::RemoveListener( igdeNVNodeListener *listener ){
	pListeners.Remove( listener );
}

void igdeNVNode::NotifyActivated(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeNVNodeListener* )listeners.GetAt( i ) )->OnActivated( this );
	}
}

void igdeNVNode::NotifyDeactivated(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeNVNodeListener* )listeners.GetAt( i ) )->OnDeactivated( this );
	}
}

void igdeNVNode::NotifyDragBegin(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeNVNodeListener* )listeners.GetAt( i ) )->OnDragBegin( this );
	}
}

void igdeNVNode::NotifyDraging(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeNVNodeListener* )listeners.GetAt( i ) )->OnDraging( this );
	}
}

void igdeNVNode::NotifyDragEnd(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeNVNodeListener* )listeners.GetAt( i ) )->OnDragEnd( this );
	}
}



void igdeNVNode::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeNativeNVNode * const native = igdeNativeNVNode::CreateNativeWidget( *this );
	SetNativeWidget( native );
	native->create();
	
	CreateChildWidgetNativeWidgets();
	
	native->FitSizeToContent();
}

void igdeNVNode::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeNVNode* )GetNativeWidget() )->DestroyNativeWidget();
	DropNativeWidget();
}



void igdeNVNode::OnTitleChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeNVNode* )GetNativeWidget() )->UpdateTitle();
	}
}

void igdeNVNode::OnDescriptionChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeNVNode* )GetNativeWidget() )->UpdateDescription();
	}
}

void igdeNVNode::OnEnabledChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeNVNode* )GetNativeWidget() )->UpdateEnabled();
	}
}

void igdeNVNode::OnActiveChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeNVNode* )GetNativeWidget() )->UpdateActive();
	}
}

void igdeNVNode::OnColorsChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeNVNode* )GetNativeWidget() )->UpdateColors();
	}
}

void igdeNVNode::OnSlotsChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeNVNode* )GetNativeWidget() )->FitSizeToContent();
	}
}

void igdeNVNode::OnPositionChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeNVNode* )GetNativeWidget() )->UpdatePosition();
	}
}
