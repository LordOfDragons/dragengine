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

#ifdef IGDE_TOOLKIT_BEOS

#include "igdeNativeBeOSColorBox.h"
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



// Class igdeNativeBeOSColorBox
////////////////////////////////

decColor igdeNativeBeOSColorBox::vCopyColor;

// Constructor, destructor
////////////////////////////

igdeNativeBeOSColorBox::igdeNativeBeOSColorBox( igdeColorBox &owner, const igdeGuiTheme &guitheme ) :
BView( BRect( 0, 0, ColorBoxHeight( guitheme ) * 2, ColorBoxHeight( guitheme ) ), NULL, B_FOLLOW_ALL, B_WILL_DRAW ),
pOwner( owner ),
pDefaultHeight( ColorBoxHeight( guitheme ) ),
pDefaultWidth( pDefaultHeight * 2 )
{
	UpdateEnabled();
	UpdateDescription();
	UpdateColor();
}

igdeNativeBeOSColorBox::~igdeNativeBeOSColorBox(){
}

igdeNativeBeOSColorBox *igdeNativeBeOSColorBox::CreateNativeWidget( igdeColorBox &owner ){
	igdeNativeBeOSColorBox * const native = new igdeNativeBeOSColorBox( owner, *owner.GetGuiTheme() );
	igdeUIBeOSHelper::AddView( native, owner.GetParent() );
	return native;
}

void igdeNativeBeOSColorBox::PostCreateNativeWidget(){
}

void igdeNativeBeOSColorBox::DestroyNativeWidget(){
	igdeUIBeOSHelper::DestroyView( this );
}



// Management
///////////////

void igdeNativeBeOSColorBox::GetPreferredSize( float *width, float *height ){
	*width = pDefaultWidth;
	*height = pDefaultHeight;
}

void igdeNativeBeOSColorBox::MouseUp( BPoint where ){
	if( ! pOwner.GetEnabled() ){
		return;
	}
	
	/* TODO
	FXColorDialog dialog( this, "Select Color" );
	
	dialog.setOpaqueOnly( false );
	dialog.setRGBA( pColor );
	
	if( ! dialog.execute( PLACEMENT_OWNER ) ){
		return;
	}
	
	const rgb_color color = dialog.getRGBA();
	if( color == pColor ){
		return;
	}
	
	try{
		pOwner.SetColor( ColorBeOSToIgde( color ) );
		pOwner.NotifyColorChanged();
		
	}catch( const deException &e ){
		pOwner.GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( &pOwner, e );
		return;
	}
	*/
	
	/* TODO right mouse
	if( ! pOwner.GetEnabled() ){
		return;
	}
	
	pOwner.ShowContextMenu( decPoint( where.x, where.y ) );
	*/
}


void igdeNativeBeOSColorBox::UpdateColor(){
	SetViewColor( ColorIgdeToBeOS( pOwner.GetColor() ) );
}

void igdeNativeBeOSColorBox::UpdateDescription(){
	SetToolTip( pOwner.GetDescription() );
}

void igdeNativeBeOSColorBox::UpdateEnabled(){
	//SetEnabled( pOwner.GetEnabled() );
}

void igdeNativeBeOSColorBox::ClipboardPutColor( const decColor &color ){
	igdeNativeBeOSColorBox::vCopyColor = color;
}

decColor igdeNativeBeOSColorBox::ClipboardGetColor(){
	return igdeNativeBeOSColorBox::vCopyColor;
}



rgb_color igdeNativeBeOSColorBox::ColorIgdeToBeOS( const decColor &color ){
	return rgb_color{
		( uint8 )( color.r * 255.0f ),
		( uint8 )( color.g * 255.0f ),
		( uint8 )( color.b * 255.0f ),
		( uint8 )( color.a * 255.0f ) };
}

decColor igdeNativeBeOSColorBox::ColorBeOSToIgde( rgb_color color ){
	return decColor(
		( float )color.red / 255.0f,
		( float )color.green / 255.0f,
		( float )color.blue / 255.0f,
		( float )color.alpha / 255.0f );
}

int igdeNativeBeOSColorBox::ColorBoxHeight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::colorBoxHeight, 15 );
}

#endif
