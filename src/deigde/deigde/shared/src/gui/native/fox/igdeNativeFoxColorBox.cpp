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

#ifdef IGDE_TOOLKIT_FOX

#include "igdeNativeFoxColorBox.h"
#include "foxIcons.h"
#include "../../igdeColorBox.h"
#include "../../igdeContainer.h"
#include "../../igdeCommonDialogs.h"
#include "../../event/igdeAction.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../theme/propertyNames.h"
#include "../../resources/igdeIcon.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

#define FOCUSBORDER 3               // Focus border



// Events
///////////

FXDEFMAP( igdeNativeFoxColorBox ) igdeNativeFoxColorBoxMap[] = {
	FXMAPFUNC( SEL_PAINT, 0, igdeNativeFoxColorBox::onPaint ),
	FXMAPFUNC( SEL_LEFTBUTTONPRESS, 0, igdeNativeFoxColorBox::onClicked ),
	FXMAPFUNC( SEL_RIGHTBUTTONPRESS, 0, igdeNativeFoxColorBox::onRClicked )
};



// Class igdeNativeFoxColorBox
////////////////////////////////

decColor igdeNativeFoxColorBox::vCopyColor;

FXIMPLEMENT( igdeNativeFoxColorBox, FXFrame, igdeNativeFoxColorBoxMap, ARRAYNUMBER( igdeNativeFoxColorBoxMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxColorBox::igdeNativeFoxColorBox(){ }

igdeNativeFoxColorBox::igdeNativeFoxColorBox( igdeColorBox &powner, FXComposite *pparent,
int layoutFlags, const igdeGuiTheme &guitheme ) :
FXFrame( pparent, layoutFlags | ColorBoxFlags( powner ), 0, 0, 0, 0, 0, 0, 0, 0 ),
pOwner( &powner ),
pColor( ColorIgdeToFx( powner.GetColor() ) ),
pDefaultHeight( ColorBoxHeight( guitheme ) ),
pDefaultWidth( pDefaultHeight * 2 )
{
	UpdateEnabled();
	UpdateDescription();
}

igdeNativeFoxColorBox::~igdeNativeFoxColorBox(){
}

igdeNativeFoxColorBox *igdeNativeFoxColorBox::CreateNativeWidget( igdeColorBox &powner ){
	if( ! powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const pparent = ( FXComposite* ) powner.GetParent()->GetNativeContainer();
	if( ! pparent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxColorBox( powner, pparent,
		igdeUIFoxHelper::GetChildLayoutFlags( &powner ), *powner.GetGuiTheme() );
}

void igdeNativeFoxColorBox::PostCreateNativeWidget(){
	FXComposite &pparent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( pparent.id() ){
		create();
	}
}

void igdeNativeFoxColorBox::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

int igdeNativeFoxColorBox::getDefaultWidth(){
	return pDefaultWidth;
}

int igdeNativeFoxColorBox::getDefaultHeight(){
	return pDefaultHeight;
}


void igdeNativeFoxColorBox::UpdateColor(){
	pColor = ColorIgdeToFx( pOwner->GetColor() );
	update();
}

void igdeNativeFoxColorBox::UpdateDescription(){
	// missing in FXFrame
// 	setTipText( pOwner->GetDescription().GetString() );
// 	setHelpText( pOwner->GetDescription().GetString() );
}

void igdeNativeFoxColorBox::UpdateEnabled(){
	if( pOwner->GetEnabled() ){
		enable();
		
	}else{
		disable();
	}
}

void igdeNativeFoxColorBox::ClipboardPutColor( const decColor &color ){
	igdeNativeFoxColorBox::vCopyColor = color;
}

decColor igdeNativeFoxColorBox::ClipboardGetColor(){
	return igdeNativeFoxColorBox::vCopyColor;
}



int igdeNativeFoxColorBox::ColorBoxFlags( const igdeColorBox & ){
	return FRAME_NORMAL;
}

FXColor igdeNativeFoxColorBox::ColorIgdeToFx( const decColor &color ){
	return FXRGBA(
		( int )( color.r * 255.0f ),
		( int )( color.g * 255.0f ),
		( int )( color.b * 255.0f ),
		( int )( color.a * 255.0f ) );
}

decColor igdeNativeFoxColorBox::ColorFxToIgde( FXColor color ){
	return decColor(
		( float )( FXREDVAL( color ) ) / 255.0f,
		( float )( FXGREENVAL( color ) ) / 255.0f,
		( float )( FXBLUEVAL( color ) ) / 255.0f,
		( float )( FXALPHAVAL( color ) ) / 255.0f );
}

int igdeNativeFoxColorBox::ColorBoxHeight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::colorBoxHeight, 15 );
}



// Events
///////////

long igdeNativeFoxColorBox::onPaint( FXObject*, FXSelector, void *pdata ){
	FXDCWindow dc( this, ( FXEvent* )pdata );
	int focusBorder = 0;
	
	dc.setForeground( backColor );
	
	if( hasSelection() ){
		focusBorder = FOCUSBORDER;
	}
	
	dc.fillRectangle( 0, 0, width, focusBorder );
	dc.fillRectangle( 0, focusBorder, focusBorder, height - focusBorder * 2 );
	dc.fillRectangle( width - focusBorder, focusBorder, focusBorder, height - focusBorder * 2 );
	dc.fillRectangle( 0, height - focusBorder, width, focusBorder );
	if( hasSelection() ){
		dc.setForeground( borderColor );
		dc.drawRectangle( 1, 1, width - 3, height - 3 );
	}
	
	dc.setForeground( pColor );
	dc.fillRectangle( focusBorder + 1, focusBorder + 1,
		width - focusBorder * 2 - 2, height - focusBorder * 2 - 2 );
	
	drawSunkenRectangle( dc, focusBorder, focusBorder,
		width - focusBorder * 2, height - focusBorder * 2 );
	
	if( hasFocus() ){
		dc.drawFocusRectangle( 0, 0, width, height );
	}
	
	return 1;
}

long igdeNativeFoxColorBox::onClicked( FXObject*, FXSelector, void* ){
	if( ! pOwner->GetEnabled() ){
		return 0;
	}
	
	FXColorDialog dialog( this, "Select Color" );
	
	dialog.setOpaqueOnly( false );
	dialog.setRGBA( pColor );
	
	if( ! dialog.execute( PLACEMENT_OWNER ) ){
		return 0;
	}
	
	const FXColor color = dialog.getRGBA();
	if( color == pColor ){
		return 0;
	}
	
	try{
		pOwner->SetColor( ColorFxToIgde( color ) );
		pOwner->NotifyColorChanged();
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
		return 0;
	}
	
	return 1;
}

long igdeNativeFoxColorBox::onRClicked( FXObject*, FXSelector, void *pdata ){
	if( ! pOwner->GetEnabled() ){
		return 0;
	}
	
	const FXEvent &event = *( ( FXEvent* )pdata );
	pOwner->ShowContextMenu( decPoint( event.win_x, event.win_y ) );
	return 1;
}

#endif
