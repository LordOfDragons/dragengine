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

#include "igdeNativeFoxContainerSplitted.h"
#include "../../../layout/igdeContainerSplitted.h"

#include <dragengine/common/exceptions.h>



// Events
///////////

FXDEFMAP( igdeNativeFoxContainerSplitted ) igdeNativeFoxContainerSplittedMap[] = {
	FXMAPFUNC( SEL_CONFIGURE, 0, igdeNativeFoxContainerSplitted::onResize ),
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxContainerSplitted::onChildLayoutFlags ),
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxContainerSplitted::ID_SELF, igdeNativeFoxContainerSplitted::onCommand ),
	FXMAPFUNC( SEL_CHANGED, igdeNativeFoxContainerSplitted::ID_SELF, igdeNativeFoxContainerSplitted::onCommand ),
};



// Class igdeNativeFoxContainerSplitted
/////////////////////////////////////////

FXIMPLEMENT( igdeNativeFoxContainerSplitted, FXSplitter,
	igdeNativeFoxContainerSplittedMap, ARRAYNUMBER( igdeNativeFoxContainerSplittedMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxContainerSplitted::igdeNativeFoxContainerSplitted(){ }

igdeNativeFoxContainerSplitted::igdeNativeFoxContainerSplitted(
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

igdeNativeFoxContainerSplitted::~igdeNativeFoxContainerSplitted(){
}

igdeNativeFoxContainerSplitted *igdeNativeFoxContainerSplitted::CreateNativeWidget( igdeContainerSplitted &owner ){
	if( ! owner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const parent = ( FXComposite* )owner.GetParent()->GetNativeContainer();
	if( ! parent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxContainerSplitted( owner, parent,
		igdeUIFoxHelper::GetChildLayoutFlags( &owner ) );
}

void igdeNativeFoxContainerSplitted::PostCreateNativeWidget(){
	FXComposite &parent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( parent.id() ){
		create();
	}
}

void igdeNativeFoxContainerSplitted::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxContainerSplitted::UpdateSplitValue(){
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

int igdeNativeFoxContainerSplitted::SplitterFlags( const igdeContainerSplitted &owner ){
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



// Events
///////////

long igdeNativeFoxContainerSplitted::onResize( FXObject*, FXSelector, void* ){
	pOwner->OnResize();
	//UpdateSplitValue();
	return 1;
}

long igdeNativeFoxContainerSplitted::onCommand( FXObject*, FXSelector, void* ){
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

long igdeNativeFoxContainerSplitted::onChildLayoutFlags( FXObject*, FXSelector, void *data ){
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
