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

#include "igdeContainerSplitted.h"
#include "../native/toolkit.h"
#include "../igdeWidget.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/logger/deLogger.h>



// Native Widget
//////////////////

namespace{

class cNativeWidget : public FXSplitter{
	FXDECLARE( cNativeWidget )
	
protected:
	cNativeWidget();
	
public:
	enum eFoxIDs{
		ID_SELF = FXButton::ID_LAST,
	};
	
private:
	igdeContainerSplitted *pOwner;
	bool pNoUpdate;
	
public:
	cNativeWidget( igdeContainerSplitted &owner, FXComposite *parent, int layoutFlags );
	virtual ~cNativeWidget();
	
	long onResize( FXObject *sender, FXSelector selector, void *data );
	long onCommand( FXObject *sender, FXSelector selector, void *data );
	long onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data );
	
	void UpdateSplitValue();
	
	static int SplitterFlags( const igdeContainerSplitted &owner );
};


FXDEFMAP( cNativeWidget ) cNativeWidgetMap[] = {
	FXMAPFUNC( SEL_CONFIGURE, 0, cNativeWidget::onResize ),
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, cNativeWidget::onChildLayoutFlags ),
	FXMAPFUNC( SEL_COMMAND, cNativeWidget::ID_SELF, cNativeWidget::onCommand ),
	FXMAPFUNC( SEL_CHANGED, cNativeWidget::ID_SELF, cNativeWidget::onCommand ),
};

FXIMPLEMENT( cNativeWidget, FXSplitter,
	cNativeWidgetMap, ARRAYNUMBER( cNativeWidgetMap ) )

cNativeWidget::cNativeWidget(){ }

cNativeWidget::cNativeWidget(
	igdeContainerSplitted &owner, FXComposite *parent, int layoutFlags ) :
FXSplitter( parent, SplitterFlags( owner ) | layoutFlags ),
pOwner( &owner ),
pNoUpdate( false )
{
	/*
	FXColor bgColor = getBackColor();
	bgColor = makeHiliteColor( bgColor, 33 );
// 	bgColor = makeShadowColor( bgColor, 33 );
	setBackColor( bgColor );
	*/
	
	setTarget( this );
	setSelector( ID_SELF );
	UpdateSplitValue();
}

cNativeWidget::~cNativeWidget(){
}

long cNativeWidget::onResize( FXObject*, FXSelector, void* ){
	pOwner->OnResize();
	//UpdateSplitValue();
	return 1;
}

long cNativeWidget::onCommand( FXObject*, FXSelector, void* ){
	if( pNoUpdate ){
		return 0;
	}
	
	int split = 0;
	int size = 1;
	
	switch( pOwner->GetSidePlacement() ){
	case igdeContainerSplitted::espTop:
		size = decMath::max( getHeight(), 1 );
		split = getSplit( 0 );
		break;
		
	case igdeContainerSplitted::espBottom:
		size = decMath::max( getHeight(), 1 );
		split = getSplit( 1 );
		break;
		
	case igdeContainerSplitted::espLeft:
		size = decMath::max( getWidth(), 1 );
		split = getSplit( 0 );
		break;
		
	case igdeContainerSplitted::espRight:
		size = decMath::max( getWidth(), 1 );
		split = getSplit( 1 );
		break;
		
	default:
		break;
	}
	
	pNoUpdate = true;
	
	if( pOwner->GetSplitPosition() >= 0 ){
		pOwner->SetSplitPosition( split );
		
	}else{
		pOwner->SetSplitPositionRelative( ( float )split / ( float )size );
	}
	
	pNoUpdate = false;
	return 1;
}

long cNativeWidget::onChildLayoutFlags( FXObject*, FXSelector, void *data ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )data );
	
	if( pOwner->GetWidgetIn( igdeContainerSplitted::eaSide ) == clflags.widget ){
		switch( pOwner->GetSidePlacement() ){
		case igdeContainerSplitted::espTop:
		case igdeContainerSplitted::espBottom:
			clflags.flags = LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y;
			break;
			
		case igdeContainerSplitted::espLeft:
		case igdeContainerSplitted::espRight:
			clflags.flags = LAYOUT_SIDE_LEFT | LAYOUT_FILL_X | LAYOUT_FILL_Y;
			break;
			
		default:
			clflags.flags = 0; // error;
			break;
		}
		
	}else if( pOwner->GetWidgetIn( igdeContainerSplitted::eaCenter ) == clflags.widget ){
		switch( pOwner->GetSidePlacement() ){
		case igdeContainerSplitted::espTop:
		case igdeContainerSplitted::espBottom:
			clflags.flags = LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y;
			break;
			
		case igdeContainerSplitted::espLeft:
		case igdeContainerSplitted::espRight:
			clflags.flags = LAYOUT_SIDE_LEFT | LAYOUT_FILL_X | LAYOUT_FILL_Y;
			break;
			
		default:
			clflags.flags = 0; // error;
			break;
		}
		
	}else{
		clflags.flags = 0; // error;
	}
	return 1;
}

void cNativeWidget::UpdateSplitValue(){
	// FXSplitter applies the split values directly as height/width to the widgets.
	// this especially means that for the SPLITTER_REVERSED setSplit has to be called
	// with index 1 instead of 0 since the height/width of the second widget is changed
	// instead of the height/width of the first widget
	
	if( numChildren() != 2 ){
		return;
	}
	
	switch( pOwner->GetSidePlacement() ){
	case igdeContainerSplitted::espTop:
		if( pOwner->GetSplitPosition() >= 0 ){
			setSplit( 0, pOwner->GetSplitPosition() );
			
		}else{
			setSplit( 0, ( int )( pOwner->GetSplitPositionRelative() * getHeight() + 0.5f ) );
		}
		break;
		
	case igdeContainerSplitted::espBottom:
		if( pOwner->GetSplitPosition() >= 0 ){
			setSplit( 1, pOwner->GetSplitPosition() );
			
		}else{
			setSplit( 1, ( int )( pOwner->GetSplitPositionRelative() * getHeight() + 0.5f ) );
		}
		break;
		
	case igdeContainerSplitted::espLeft:
		if( pOwner->GetSplitPosition() >= 0 ){
			setSplit( 0, pOwner->GetSplitPosition() );
		}else{
			setSplit( 0, ( int )( pOwner->GetSplitPositionRelative() * getWidth() + 0.5f ) );
		}
		break;
		
	case igdeContainerSplitted::espRight:
		if( pOwner->GetSplitPosition() >= 0 ){
			setSplit( 1, pOwner->GetSplitPosition() );
		}else{
			setSplit( 1, ( int )( pOwner->GetSplitPositionRelative() * getWidth() + 0.5f ) );
		}
		break;
		
	default:
		break;
	}
}

int cNativeWidget::SplitterFlags( const igdeContainerSplitted &owner ){
	int flags = 0;
	
	switch( owner.GetSidePlacement() ){
	case igdeContainerSplitted::espTop:
		flags |= SPLITTER_VERTICAL | SPLITTER_TRACKING;
		break;
		
	case igdeContainerSplitted::espBottom:
		flags |= SPLITTER_VERTICAL | SPLITTER_REVERSED | SPLITTER_TRACKING;
		break;
		
	case igdeContainerSplitted::espLeft:
		flags |= SPLITTER_HORIZONTAL | SPLITTER_TRACKING;
		break;
		
	case igdeContainerSplitted::espRight:
		flags |= SPLITTER_HORIZONTAL | SPLITTER_REVERSED | SPLITTER_TRACKING;
		break;
		
	default:
		break;
	}
	
	return flags;
}

}



// Class igdeContainerSplitted
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeContainerSplitted::igdeContainerSplitted( igdeEnvironment &environment,
	eSidePlacement sidePlacement ) :
igdeContainer( environment ),
pSidePlacement( sidePlacement ),
pSplitPosition( -1 ),
pSplitPositionRelative( 0.5f ),
pWidgetSide( NULL ),
pWidgetCenter( NULL ){
}

igdeContainerSplitted::igdeContainerSplitted( igdeEnvironment &environment,
	eSidePlacement sidePlacement, int splitPosition ) :
igdeContainer( environment ),
pSidePlacement( sidePlacement ),
pSplitPosition( splitPosition ),
pSplitPositionRelative( 0.0f ),
pWidgetSide( NULL ),
pWidgetCenter( NULL ){
}

igdeContainerSplitted::igdeContainerSplitted( igdeEnvironment &environment,
	eSidePlacement sidePlacement, float splitPosition ) :
igdeContainer( environment ),
pSidePlacement( sidePlacement ),
pSplitPosition( -1 ),
pSplitPositionRelative( decMath::clamp( splitPosition, 0.0f, 1.0f ) ),
pWidgetSide( NULL ),
pWidgetCenter( NULL ){
}

igdeContainerSplitted::~igdeContainerSplitted(){
}



// Management
///////////////

void igdeContainerSplitted::SetSplitPosition( int splitPosition ){
	if( splitPosition == pSplitPosition ){
		return;
	}
	
	pSplitPosition = splitPosition;
	
	cNativeWidget * const foxWidget = ( cNativeWidget* )GetNativeWidget();
	if( foxWidget ){
		foxWidget->UpdateSplitValue();
	}
}

void igdeContainerSplitted::SetSplitPositionRelative( float splitPosition ){
	if( fabsf( splitPosition - pSplitPositionRelative ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pSplitPositionRelative = splitPosition;
	pSplitPosition = -1;
	
	cNativeWidget * const foxWidget = ( cNativeWidget* )GetNativeWidget();
	if( foxWidget ){
		foxWidget->UpdateSplitValue();
	}
}



void igdeContainerSplitted::AddChild( igdeWidget *child, eArea area ){
	if( ! child || child->GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	if( child == pWidgetSide || child == pWidgetCenter ){
		DETHROW( deeInvalidParam );
	}
	
	switch( area ){
	case eaSide:
		if( pWidgetSide ){
			DETHROW( deeInvalidParam );
		}
		pWidgetSide = child;
		
		try{
			igdeContainer::AddChild( child );
			
		}catch( const deException & ){
			pWidgetSide = NULL;
			throw;
		}
		break;
		
	case eaCenter:
		if( pWidgetCenter ){
			DETHROW( deeInvalidParam );
		}
		pWidgetCenter = child;
		
		try{
			igdeContainer::AddChild( child );
			
		}catch( const deException & ){
			pWidgetCenter = NULL;
			throw;
		}
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
	
	cNativeWidget * const native = ( cNativeWidget* )GetNativeWidget();
	if( native ){
		native->UpdateSplitValue();
	}
}

void igdeContainerSplitted::AddChild( igdeWidget* ){
	DETHROW( deeInvalidParam );
}

void igdeContainerSplitted::RemoveChild( igdeWidget *child ){
	igdeContainer::RemoveChild( child );
	
	if( child == pWidgetSide ){
		pWidgetSide = NULL;
		
	}else if( child == pWidgetCenter ){
		pWidgetCenter = NULL;
	}
}

void igdeContainerSplitted::RemoveAllChildren(){
	igdeContainer::RemoveAllChildren();
	
	pWidgetSide = NULL;
	pWidgetCenter = NULL;
}

igdeWidget *igdeContainerSplitted::GetWidgetIn( eArea area ) const{
	switch( area ){
	case eaSide:
		return pWidgetSide;
		
	case eaCenter:
		return pWidgetCenter;
		
	default:
		DETHROW( deeInvalidParam );
	}
}



void igdeContainerSplitted::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeContainer * const parent = GetParent();
	if( ! parent ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const foxParent = ( FXComposite* )parent->GetNativeContainer();
	if( ! foxParent ){
		DETHROW( deeInvalidParam );
	}
	
	int layoutFlags = igdeUIFoxHelper::GetChildLayoutFlags( this );
	cNativeWidget * const native = new cNativeWidget( *this, foxParent, layoutFlags );
	
	SetNativeWidget( native );
	if( foxParent->id() ){
		native->create();
	}
	
	CreateChildWidgetNativeWidgets();
	
	native->UpdateSplitValue();
}

void igdeContainerSplitted::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeWidget * const native = ( cNativeWidget* )GetNativeWidget();
	DropNativeWidget();
	delete native;
}

void igdeContainerSplitted::CreateChildWidgetNativeWidgets(){
	switch( pSidePlacement ){
	case igdeContainerSplitted::espBottom:
	case igdeContainerSplitted::espRight:
		if( pWidgetCenter ){
			pWidgetCenter->CreateNativeWidget();
		}
		if( pWidgetSide ){
			pWidgetSide->CreateNativeWidget();
		}
		break;
		
	case igdeContainerSplitted::espTop:
	case igdeContainerSplitted::espLeft:
	default:
		if( pWidgetSide ){
			pWidgetSide->CreateNativeWidget();
		}
		if( pWidgetCenter ){
			pWidgetCenter->CreateNativeWidget();
		}
		break;
	}
}
