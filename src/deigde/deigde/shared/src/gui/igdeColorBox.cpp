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



// Native Widget
//////////////////

#define FOCUSBORDER 3               // Focus border

class cNativeIgdeColorBox : public FXFrame{
	FXDECLARE( cNativeIgdeColorBox )
	
protected:
	cNativeIgdeColorBox();
	
public:
	enum eFoxIDs{
		ID_SELF = FXFrame::ID_LAST,
	};
	
protected:
	igdeColorBox *pOwner;
	FXColor pColor;
	int pDefaultHeight;
	int pDefaultWidth;
	
public:
	// Temporary FOX Bug hack
	static decColor vCopyColor;
	
public:
	cNativeIgdeColorBox( igdeColorBox &owner, FXComposite *parent,
		int layoutFlags, const igdeGuiTheme &guitheme );
	virtual ~cNativeIgdeColorBox();
	
	virtual FXint getDefaultWidth();
	virtual FXint getDefaultHeight();
	
	virtual void UpdateColor();
	virtual void UpdateDescription();
	virtual void UpdateEnabled();
	
	long onPaint( FXObject *sender, FXSelector selector, void *data );
	long onClicked( FXObject *sender, FXSelector selector, void *data );
	long onRClicked( FXObject *sender, FXSelector selector, void *data );
	
	static FXColor ColorIgdeToFx( const decColor &color );
	static decColor ColorFxToIgde( FXColor color );
	static int ColorBoxFlags( const igdeColorBox &owner );
	static int ColorBoxHeight( const igdeGuiTheme &guitheme );
};


FXDEFMAP( cNativeIgdeColorBox ) cNativeIgdeColorBoxMap[] = {
	FXMAPFUNC( SEL_PAINT, 0, cNativeIgdeColorBox::onPaint ),
	FXMAPFUNC( SEL_LEFTBUTTONPRESS, 0, cNativeIgdeColorBox::onClicked ),
	FXMAPFUNC( SEL_RIGHTBUTTONPRESS, 0, cNativeIgdeColorBox::onRClicked )
};


decColor cNativeIgdeColorBox::vCopyColor;


FXIMPLEMENT( cNativeIgdeColorBox, FXFrame, cNativeIgdeColorBoxMap, ARRAYNUMBER( cNativeIgdeColorBoxMap ) )

cNativeIgdeColorBox::cNativeIgdeColorBox(){ }

cNativeIgdeColorBox::cNativeIgdeColorBox( igdeColorBox &owner, FXComposite *parent,
int layoutFlags, const igdeGuiTheme &guitheme ) :
FXFrame( parent, layoutFlags | ColorBoxFlags( owner ), 0, 0, 0, 0, 0, 0, 0, 0 ),
pOwner( &owner ),
pColor( ColorIgdeToFx( owner.GetColor() ) ),
pDefaultHeight( ColorBoxHeight( guitheme ) ),
pDefaultWidth( pDefaultHeight * 2 )
{
	UpdateEnabled();
	UpdateDescription();
}

cNativeIgdeColorBox::~cNativeIgdeColorBox(){
}


int cNativeIgdeColorBox::getDefaultWidth(){
	return pDefaultWidth;
}

int cNativeIgdeColorBox::getDefaultHeight(){
	return pDefaultHeight;
}


void cNativeIgdeColorBox::UpdateColor(){
	pColor = ColorIgdeToFx( pOwner->GetColor() );
	update();
}

void cNativeIgdeColorBox::UpdateDescription(){
	// missing in FXFrame
// 	setTipText( pOwner->GetDescription().GetString() );
// 	setHelpText( pOwner->GetDescription().GetString() );
}

void cNativeIgdeColorBox::UpdateEnabled(){
	if( pOwner->GetEnabled() ){
		enable();
		
	}else{
		disable();
	}
}


long cNativeIgdeColorBox::onPaint( FXObject*, FXSelector, void *data ){
	FXDCWindow dc( this, ( FXEvent* )data );
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

long cNativeIgdeColorBox::onClicked( FXObject*, FXSelector, void* ){
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

long cNativeIgdeColorBox::onRClicked( FXObject*, FXSelector, void *data ){
	if( ! pOwner->GetEnabled() ){
		return 0;
	}
	
	const FXEvent &event = *( ( FXEvent* )data );
	pOwner->ShowContextMenu( decPoint( event.win_x, event.win_y ) );
	return 1;
}


int cNativeIgdeColorBox::ColorBoxFlags( const igdeColorBox & ){
	return FRAME_NORMAL;
}

FXColor cNativeIgdeColorBox::ColorIgdeToFx( const decColor &color ){
	return FXRGBA(
		( int )( color.r * 255.0f ),
		( int )( color.g * 255.0f ),
		( int )( color.b * 255.0f ),
		( int )( color.a * 255.0f ) );
}

decColor cNativeIgdeColorBox::ColorFxToIgde( FXColor color ){
	return decColor(
		( float )( FXREDVAL( color ) ) / 255.0f,
		( float )( FXGREENVAL( color ) ) / 255.0f,
		( float )( FXBLUEVAL( color ) ) / 255.0f,
		( float )( FXALPHAVAL( color ) ) / 255.0f );
}

int cNativeIgdeColorBox::ColorBoxHeight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::colorBoxHeight, 15 );
}



// Actions
////////////

igdeColorBox::cActionCopy::cActionCopy( igdeColorBox &colorBox ) :
igdeAction( "Copy", colorBox.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ),
	"Copy color to clipboard" ),
pColorBox( colorBox ){
}

void igdeColorBox::cActionCopy::OnAction(){
	cNativeIgdeColorBox::vCopyColor = pColorBox.GetColor();
}


igdeColorBox::cActionCopyHex::cActionCopyHex( igdeColorBox &colorBox ) :
igdeAction( "Copy As Hex", colorBox.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ),
	"Copy color to clipboard as hex value" ),
pColorBox( colorBox ){
}

void igdeColorBox::cActionCopyHex::OnAction(){
	//cNativeIgdeColorBox::vCopyColor = pColorBox.GetColor();
	
	// TODO... access system clipboard
}


igdeColorBox::cActionPaste::cActionPaste( igdeColorBox &colorBox ) :
igdeAction( "Paste", colorBox.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ),
	"Paste color from clipboard" ),
pColorBox( colorBox ){
}

void igdeColorBox::cActionPaste::OnAction(){
	if( cNativeIgdeColorBox::vCopyColor.IsEqualTo( pColorBox.GetColor() ) ){
		return;
	}
	
	pColorBox.SetColor( cNativeIgdeColorBox::vCopyColor );
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
	if( cNativeIgdeColorBox::vCopyColor.IsEqualTo( pColorBox.GetColor() ) ){
		return;
	}
	
	pColorBox.SetColor( cNativeIgdeColorBox::vCopyColor );
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
	
	if( ! GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const foxParent = ( FXComposite* )GetParent()->GetNativeContainer();
	if( ! foxParent ){
		DETHROW( deeInvalidParam );
	}
	
	int layoutFlags = igdeUIFoxHelper::GetChildLayoutFlags( this );
	cNativeIgdeColorBox * const native = new cNativeIgdeColorBox(
		*this, foxParent, layoutFlags, *GetGuiTheme() );
	SetNativeWidget( native );
	if( foxParent->id() ){
		native->create();
	}
}

void igdeColorBox::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( cNativeIgdeColorBox* )GetNativeWidget();
	DropNativeWidget();
}

void igdeColorBox::OnColorChanged(){
	if( GetNativeWidget() ){
		( ( cNativeIgdeColorBox* )GetNativeWidget() )->UpdateColor();
	}
}

void igdeColorBox::OnEnabledChanged(){
	if( GetNativeWidget() ){
		( ( cNativeIgdeColorBox* )GetNativeWidget() )->UpdateEnabled();
	}
}

void igdeColorBox::OnDescriptionChanged(){
	if( GetNativeWidget() ){
		( ( cNativeIgdeColorBox* )GetNativeWidget() )->UpdateDescription();
	}
}
