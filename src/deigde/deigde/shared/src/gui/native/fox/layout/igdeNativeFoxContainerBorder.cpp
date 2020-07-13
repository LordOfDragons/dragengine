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

#include "igdeNativeFoxContainerBorder.h"
#include "../../../layout/igdeContainerBorder.h"

#include <dragengine/common/exceptions.h>



// Events
///////////

FXDEFMAP( igdeNativeFoxContainerBorder ) igdeNativeFoxContainerBorderMap[] = {
	FXMAPFUNC( SEL_CONFIGURE, 0, igdeNativeFoxContainerBorder::onResize ),
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxContainerBorder::onChildLayoutFlags )
};



// Class igdeNativeFoxContainerBorder
//////////////////////////////

FXIMPLEMENT( igdeNativeFoxContainerBorder, FXPacker,
	igdeNativeFoxContainerBorderMap, ARRAYNUMBER( igdeNativeFoxContainerBorderMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxContainerBorder::igdeNativeFoxContainerBorder(){ }

igdeNativeFoxContainerBorder::igdeNativeFoxContainerBorder(
igdeContainerBorder &owner, FXComposite *parent, int layoutFlags ) :
FXPacker( parent, layoutFlags, 0, 0, 0, 0, 0, 0, 0, 0, owner.GetSpacing(), owner.GetSpacing() ),
pOwner( &owner ){
}

igdeNativeFoxContainerBorder::~igdeNativeFoxContainerBorder(){
}

igdeNativeFoxContainerBorder *igdeNativeFoxContainerBorder::CreateNativeWidget( igdeContainerBorder &owner ){
	if( ! owner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const parent = ( FXComposite* )owner.GetParent()->GetNativeContainer();
	if( ! parent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxContainerBorder( owner, parent,
		igdeUIFoxHelper::GetChildLayoutFlags( &owner ) );
}

void igdeNativeFoxContainerBorder::PostCreateNativeWidget(){
	FXComposite &parent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( parent.id() ){
		create();
	}
}

void igdeNativeFoxContainerBorder::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxContainerBorder::layout(){
	// unfortunately wee need to re-implement the layout code from FXPacker::layout().
	// this is required since for border layout to work properly the widgets have to be
	// processed in the correct order or the result is incorrect.
	// 
	// so why not just add them in the right order? the problem is that it is allowed to
	// add widgets while the native widget is already visible. since FOX has no way to
	// move windows index wise inside inside an already created parent window we would
	// have to delete all native child widgets and adding them again in the right order.
	// the other solution is to overwrite FXPacker::layout() to ensure the right order
	// is used. this is not so bad as it sounds since we know what layout flags we allow
	// and thus not required code can be dropped. the rest is copy-pasted and adjusted
	// to the coding style used.
	// 
	// as a goody this allows to re-implement the layout if it shows the existing packer
	// code is not fully suitable. so far it is.
	enum eSides{ esTop, esBottom, esLeft, esRight, esCenter };
	
	igdeWidget * const children[ 5 ] = {
		pOwner->GetWidgetIn( igdeContainerBorder::eaTop ),
		pOwner->GetWidgetIn( igdeContainerBorder::eaBottom ),
		pOwner->GetWidgetIn( igdeContainerBorder::eaLeft ),
		pOwner->GetWidgetIn( igdeContainerBorder::eaRight ),
		pOwner->GetWidgetIn( igdeContainerBorder::eaCenter )
	};
	
	int left = border + padleft;
	int right = width - border - padright;
	int top = border + padtop;
	int bottom = height - border - padbottom;
	int i;
	
	for( i=0; i<5; i++ ){
		if( ! children[ i ] ){
			continue;
		}
		
		FXWindow * const child = ( FXWindow* )children[ i ]->GetNativeWidget();
		if( ! child ){
			continue;
		}
		
		if( ! child->shown() ){
			continue;
		}
		
		const FXuint hints = child->getLayoutHints();
		int x = child->getX();
		int y = child->getY();
		int w, h;
		
		// height and width
		if( hints & LAYOUT_FIX_HEIGHT ){
			h = child->getHeight();
			
		}else if( hints & LAYOUT_FILL_Y ){
			h = bottom - top;
			
		}else{
			h = child->getDefaultHeight();
		}
		
		if( hints & LAYOUT_FIX_WIDTH ){
			w = child->getWidth();
			
		}else if( hints & LAYOUT_FILL_X ){
			w = right - left;
			
		}else{
			w = child->getDefaultWidth();
		}
		
		// position. original fox code uses LAYOUT_SIDE_* but this is error prone since things
		// like LAYOUT_SIDE_RIGHT is actually an or combination of LAYOUT_SIDE_LEFT and
		// LAYOUT_SIDE_BOTTOM which messes up the if conditions. changed to use the actual
		// widget area since we can do this in this situation here
		switch( i ){
		case esTop:
			x = left;
			y = top;
			top += h + vspacing;
			break;
			
		case esBottom:
			x = left;
			y = bottom - h;
			bottom -= h + vspacing;
			break;
			
		case esLeft:
			x = left;
			left += w + hspacing;
			y = top;
			break;
			
		case esRight:
			x = right - w;
			right -= w + hspacing;
			y = top;
			break;
			
		default: //esCenter
			x = left;
			y = top;
			break;
		}
		
		child->position( x, y, w, h );
	}
	
	flags &= ~FLAG_DIRTY;
}



// Events
///////////

long igdeNativeFoxContainerBorder::onResize( FXObject *sender, FXSelector selector, void *data ){
	pOwner->OnResize();
	return 1;
}

long igdeNativeFoxContainerBorder::onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )data );
	
	if( pOwner->GetWidgetIn( igdeContainerBorder::eaTop ) == clflags.widget ){
		clflags.flags = LAYOUT_SIDE_TOP | LAYOUT_FILL_X;
		clflags.canResizeVertical = true;
		
	}else if( pOwner->GetWidgetIn( igdeContainerBorder::eaBottom ) == clflags.widget ){
		clflags.flags = LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X;
		clflags.canResizeVertical = true;
		
	}else if( pOwner->GetWidgetIn( igdeContainerBorder::eaLeft ) == clflags.widget ){
		clflags.flags = LAYOUT_SIDE_LEFT | LAYOUT_FILL_Y;
		clflags.canResizeHorizontal = true;
		
	}else if( pOwner->GetWidgetIn( igdeContainerBorder::eaRight ) == clflags.widget ){
		clflags.flags = LAYOUT_SIDE_RIGHT | LAYOUT_FILL_Y;
		clflags.canResizeHorizontal = true;
		
	}else if( pOwner->GetWidgetIn( igdeContainerBorder::eaCenter ) == clflags.widget ){
		clflags.flags = LAYOUT_FILL_X | LAYOUT_FILL_Y;
		
	}else{
		clflags.flags = 0; // error
	}
	return 1;
}
