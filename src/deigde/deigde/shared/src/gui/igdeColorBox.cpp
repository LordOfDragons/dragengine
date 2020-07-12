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

#include "igdeColorBox.h"
#include "igdeContainer.h"
#include "native/toolkit.h"
#include "igdeCommonDialogs.h"
#include "igdeUIHelper.h"
#include "event/igdeColorBoxListener.h"
#include "menu/igdeMenuCascade.h"
#include "menu/igdeMenuCascadeReference.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../codec/igdeCodecPropertyString.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Actions
////////////

igdeColorBox::cActionCopy::cActionCopy( igdeColorBox &colorBox ) :
igdeAction( "Copy", colorBox.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ),
	"Copy color to clipboard" ),
pColorBox( colorBox ){
}

void igdeColorBox::cActionCopy::OnAction(){
	if( ! pColorBox.GetNativeWidget() ){
		DETHROW( deeInvalidParam );
	}
	( ( igdeNativeColorBox* )pColorBox.GetNativeWidget() )->ClipboardPutColor( pColorBox.GetColor() );
}


igdeColorBox::cActionCopyHex::cActionCopyHex( igdeColorBox &colorBox ) :
igdeAction( "Copy As Hex", colorBox.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ),
	"Copy color to clipboard as hex value" ),
pColorBox( colorBox ){
}

void igdeColorBox::cActionCopyHex::OnAction(){
	// TODO... access system clipboard
}


igdeColorBox::cActionPaste::cActionPaste( igdeColorBox &colorBox ) :
igdeAction( "Paste", colorBox.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ),
	"Paste color from clipboard" ),
pColorBox( colorBox ){
}

void igdeColorBox::cActionPaste::OnAction(){
	const decColor color( ( ( igdeNativeColorBox* )pColorBox.GetNativeWidget() )->ClipboardGetColor() );
	if( color.IsEqualTo( pColorBox.GetColor() ) ){
		return;
	}
	
	if( ! pColorBox.GetNativeWidget() ){
		DETHROW( deeInvalidParam );
	}
	pColorBox.SetColor( color );
	pColorBox.NotifyColorChanged();
}


igdeColorBox::cActionPasteHex::cActionPasteHex( igdeColorBox &colorBox ) :
igdeAction( "Paste From Hex", colorBox.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ),
	"Paste color from clipboard as hex value" ),
pColorBox( colorBox ){
}

void igdeColorBox::cActionPasteHex::OnAction(){
	// TODO... access system clipboard
	
	/*
	if( igdeNativeFoxColorBox::vCopyColor.IsEqualTo( pColorBox.GetColor() ) ){
		return;
	}
	
	pColorBox.SetColor( igdeNativeFoxColorBox::vCopyColor );
	pColorBox.NotifyColorChanged();
	*/
}


igdeColorBox::cActionEditValues::cActionEditValues( igdeColorBox &colorBox ) :
igdeAction( "Edit Values", NULL, "Edit values directly" ),
pColorBox( colorBox ){
}

void igdeColorBox::cActionEditValues::OnAction(){
	igdeCodecPropertyString codec;
	decString value;
	
	codec.EncodeColor4( pColorBox.GetColor(), value );
	
	if( ! igdeCommonDialogs::GetString( &pColorBox, GetText(), "Color:", value ) ){
		return;
	}
	
	float components[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f };
	codec.DecodeFixedFloatList( value, components, 4 );
	
	const decColor color( components[ 0 ], components[ 1 ], components[ 2 ], components[ 3 ] );
	if( color.IsEqualTo( pColorBox.GetColor() ) ){
		return;
	}
	
	pColorBox.SetColor( color );
	pColorBox.NotifyColorChanged();
}


igdeColorBox::cActionEditHex::cActionEditHex( igdeColorBox &colorBox ) :
igdeAction( "Edit Hex", NULL, "Edit hex directly" ),
pColorBox( colorBox ){
}

void igdeColorBox::cActionEditHex::OnAction(){
	decColor color( pColorBox.GetColor() );
	decString value;
	
	value.Format( "%02X%02X%02X%02X",
		decMath::clamp( ( int )( color.r * 255.0f ), 0, 255 ),
		decMath::clamp( ( int )( color.g * 255.0f ), 0, 255 ),
		decMath::clamp( ( int )( color.b * 255.0f ), 0, 255 ),
		decMath::clamp( ( int )( color.a * 255.0f ), 0, 255 ) );
	
	if( ! igdeCommonDialogs::GetString( &pColorBox, GetText(), "Color:", value ) ){
		return;
	}
	
	value.ToLower();
	
	if( value.BeginsWith( "0x" ) ){
		value = value.GetMiddle( 2 );
	}
	
	const int length = value.GetLength();
	color.SetZero();
	if( length >= 2 ){
		color.r = ( float )strtol( value.GetLeft( 2 ), NULL, 16 ) / 255.0f;
	}
	if( length >= 4 ){
		color.g = ( float )strtol( value.GetMiddle( 2, 2 ), NULL, 16 ) / 255.0f;
	}
	if( length >= 6 ){
		color.b = ( float )strtol( value.GetMiddle( 4, 2 ), NULL, 16 ) / 255.0f;
	}
	if( length >= 8 ){
		color.a = ( float )strtol( value.GetMiddle( 6, 2 ), NULL, 16 ) / 255.0f;
	}
	
	if( color.IsEqualTo( pColorBox.GetColor() ) ){
		return;
	}
	
	pColorBox.SetColor( color );
	pColorBox.NotifyColorChanged();
}



// Class igdeColorBox
///////////////////////

// Constructor, destructor
////////////////////////////

igdeColorBox::igdeColorBox( igdeEnvironment &environment ) :
igdeWidget( environment ),
pEnabled( true ){
}

igdeColorBox::igdeColorBox( igdeEnvironment &environment, const char *description ) :
igdeWidget( environment ),
pDescription( description ),
pEnabled( true ){
}

igdeColorBox::~igdeColorBox(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeColorBox::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeColorBox::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}



void igdeColorBox::SetColor( const decColor &color ){
	if( pColor.IsEqualTo( color ) ){
		return;
	}
	
	pColor = color;
	OnColorChanged();
}

void igdeColorBox::Focus(){
}



void igdeColorBox::AddListener( igdeColorBoxListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void igdeColorBox::RemoveListener( igdeColorBoxListener *listener ){
	pListeners.Remove( listener );
}

void igdeColorBox::NotifyColorChanged(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeColorBoxListener* )listeners.GetAt( i ) )->OnColorChanged( this );
	}
}

void igdeColorBox::ShowContextMenu( const decPoint &position ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeMenuCascadeReference menu;
	menu.TakeOver( new igdeMenuCascade( GetEnvironment() ) );
	
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	helper.MenuCommand( menu, new cActionCopy( *this ), true );
	helper.MenuCommand( menu, new cActionPaste( *this ), true );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, new cActionEditValues( *this ), true );
	helper.MenuCommand( menu, new cActionEditHex( *this ), true );
	
	menu->Popup( *this, position );
}



void igdeColorBox::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeNativeColorBox * const native = igdeNativeFoxColorBox::CreateNativeWidget( *this );
	SetNativeWidget( native );
	native->PostCreateNativeWidget();
}

void igdeColorBox::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeColorBox* )GetNativeWidget() )->DestroyNativeWidget();
	DropNativeWidget();
}

void igdeColorBox::OnColorChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeColorBox* )GetNativeWidget() )->UpdateColor();
	}
}

void igdeColorBox::OnEnabledChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeColorBox* )GetNativeWidget() )->UpdateEnabled();
	}
}

void igdeColorBox::OnDescriptionChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeColorBox* )GetNativeWidget() )->UpdateDescription();
	}
}
