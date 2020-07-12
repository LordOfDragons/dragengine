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

#include "igdeNVSlot.h"
#include "igdeNVNode.h"
#include "igdeNVBoard.h"
#include "igdeNVLink.h"
#include "../igdeContainer.h"
#include "../native/toolkit.h"
#include "../igdeCommonDialogs.h"
#include "../event/igdeAction.h"
#include "../resources/igdeIcon.h"
#include "../resources/igdeFont.h"
#include "../resources/igdeFontReference.h"
#include "../theme/igdeGuiTheme.h"
#include "../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



/*

// TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST
#include <deigde/gui/nodeview/igdeNVSlot.h>
#include <deigde/gui/nodeview/igdeNVSlotReference.h>
// TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST
	
	// TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST
	igdeContainerReference nvnode;
	nvnode.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	igdeNVSlotReference nvslot1, nvslot2, nvslot3, nvslot4;
	nvslot1.TakeOver( new igdeNVSlot( env, "Input 1", "This is a test input", true ) );
	nvnode->AddChild( nvslot1 );
	nvslot2.TakeOver( new igdeNVSlot( env, "Input 2", "This is a test input", true ) );
	nvslot2->SetColor( decColor( 1.0f, 0.0f, 0.0f ) );
	nvnode->AddChild( nvslot2 );
	nvslot3.TakeOver( new igdeNVSlot( env, "Output 1", "This is a test output", false ) );
	nvnode->AddChild( nvslot3 );
	nvslot4.TakeOver( new igdeNVSlot( env, "Output 2", "This is a test output", false ) );
	nvslot4->SetColor( decColor( 0.0f, 0.0f, 1.0f ) );
	nvnode->AddChild( nvslot4 );
	nvslot2->AddLink( nvslot4 );
	sidePanel->AddChild( nvnode );
	// TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST

*/


// Class igdeNVSlot
/////////////////////

// Constructor, destructor
////////////////////////////

igdeNVSlot::igdeNVSlot( igdeEnvironment &environment, const char *text, bool isInput ) :
igdeContainer( environment ),
pText( text ),
pEnabled( true ),
pIsInput( isInput ),
pColor( 0.0f, 0.0f, 0.0f ),
pOwnerNode( NULL ){
}

igdeNVSlot::igdeNVSlot( igdeEnvironment &environment, const char *text, const char *description, bool isInput ) :
igdeContainer( environment ),
pText( text ),
pDescription( description ),
pEnabled( true ),
pIsInput( isInput ),
pColor( 0.0f, 0.0f, 0.0f ),
pOwnerNode( NULL ){
}

igdeNVSlot::~igdeNVSlot(){
	DestroyNativeWidget();
	RemoveAllLinks();
}



// Management
///////////////

void igdeNVSlot::SetText( const char *text ){
	if( pText == text ){
		return;
	}
	
	pText = text;
	OnTextChanged();
}

void igdeNVSlot::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeNVSlot::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeNVSlot::SetColor( const decColor &color ){
	if( color.IsEqualTo( pColor ) ){
		return;
	}
	
	pColor = color;
	OnColorChanged();
}

void igdeNVSlot::SetOwnerNode( igdeNVNode *node ){
	pOwnerNode = node;
}



int igdeNVSlot::GetLinkCount() const{
	return pLinks.GetCount();
}

igdeNVLink *igdeNVSlot::GetLinkAt( int index ) const{
	return ( igdeNVLink* )pLinks.GetAt( index );
}

bool igdeNVSlot::HasLink( igdeNVLink *link ) const{
	return pLinks.Has( link );
}

void igdeNVSlot::AddLink( igdeNVLink *link ){
	if( ! link ){
		DETHROW( deeInvalidParam );
	}
	
	pLinks.AddIfAbsent( link );
	OnLinksChanged();
}

void igdeNVSlot::RemoveLink( igdeNVLink *link ){
	pLinks.RemoveIfPresent( link );
	OnLinksChanged();
}

void igdeNVSlot::RemoveAllLinks(){
	pLinks.RemoveAll();
	OnLinksChanged();
}



decPoint igdeNVSlot::GetCenter() const{
	if( ! GetNativeWidget() ){
		return decPoint();
	}
	
	return ( ( igdeNativeNVSlot* )GetNativeWidget() )->GetCenter();
}

decPoint igdeNVSlot::GetCenterNode() const{
	if( ! GetNativeWidget() ){
		return decPoint();
	}
	
	return ( ( igdeNativeNVSlot* )GetNativeWidget() )->GetCenterNode();
}

decPoint igdeNVSlot::GetCenterBoard() const{
	if( ! GetNativeWidget() ){
		return decPoint();
	}
	
	return ( ( igdeNativeNVSlot* )GetNativeWidget() )->GetCenterBoard();
}

decPoint igdeNVSlot::GetConnector() const{
	if( ! GetNativeWidget() ){
		return decPoint();
	}
	
	return ( ( igdeNativeNVSlot* )GetNativeWidget() )->GetConnector();
}

decPoint igdeNVSlot::GetConnectorNode() const{
	if( ! GetNativeWidget() ){
		return decPoint();
	}
	
	return ( ( igdeNativeNVSlot* )GetNativeWidget() )->GetConnectorNode();
}

decPoint igdeNVSlot::GetConnectorBoard() const{
	if( ! GetNativeWidget() ){
		return decPoint();
	}
	
	return ( ( igdeNativeNVSlot* )GetNativeWidget() )->GetConnectorBoard();
}



void igdeNVSlot::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	if( ! GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const nativeParent = ( FXComposite* )GetParent()->GetNativeContainer();
	if( ! nativeParent ){
		DETHROW( deeInvalidParam );
	}
	
	igdeNativeNVSlot * const native = igdeNativeNVSlot::CreateNativeWidget( *this );
	SetNativeWidget( native );
	native->PostCreateNativeWidget();
	
	CreateChildWidgetNativeWidgets();
}

void igdeNVSlot::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeNVSlot* )GetNativeWidget() )->DestroyNativeWidget();
	DropNativeWidget();
}



void igdeNVSlot::OnTextChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeNVSlot* )GetNativeWidget() )->UpdateText();
}

void igdeNVSlot::OnDescriptionChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeNVSlot* )GetNativeWidget() )->UpdateDescription();
}

void igdeNVSlot::OnEnabledChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeNVSlot* )GetNativeWidget() )->UpdateEnabled();
}

void igdeNVSlot::OnColorChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeNVSlot* )GetNativeWidget() )->UpdateColor();
}

void igdeNVSlot::OnLinksChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeNVSlot* )GetNativeWidget() )->UpdateLinkedState();
}
