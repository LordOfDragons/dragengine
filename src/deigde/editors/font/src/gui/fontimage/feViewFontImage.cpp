/* 
 * Drag[en]gine IGDE Font Editor
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "feViewFontImage.h"
#include "feViewFIImage.h"
#include "feViewFontImageListener.h"
#include "../feWindowMain.h"
#include "../../configuration/feConfiguration.h"
#include "../../font/feFont.h"
#include "../../font/image/feFontImage.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeScrollBar.h>
#include <deigde/gui/event/igdeScrollBarListener.h>
#include <deigde/gui/event/igdeScrollBarListenerReference.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeComboBoxListenerReference.h>
#include <deigde/gui/layout/igdeContainerFlow.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>



// Events
///////////

class cScrollView : public igdeScrollBarListener{
protected:
	feViewFontImage &pView;
	
public:
	cScrollView( feViewFontImage &view ) : pView( view ){ }
	
	virtual void OnValueChanged( igdeScrollBar *scrollBar ){
		pView.ScrollView();
	}
};


class cChangeZoom : public igdeComboBoxListener{
protected:
	feViewFontImage &pView;
	
public:
	cChangeZoom( feViewFontImage &view ) : pView( view ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		pView.GetViewImage().SetZoom( comboBox->GetText().ToInt() );
		pView.UpdateScrollbarRanges();
		
		decString text;
		text.Format( "%d", pView.GetViewImage().GetZoom() );
		comboBox->SetText( text );
	}
};



// Class feViewFontImage
//////////////////////////

// Constructor, destructor
////////////////////////////

feViewFontImage::feViewFontImage( feWindowMain &windowMain ) :
igdeContainerBorder( windowMain.GetEnvironment() ),
pWindowMain( windowMain ),
pListener( NULL ),
pFont( NULL ),
pViewImage( NULL )
{
	pListener = new feViewFontImageListener( *this );
	
	igdeEnvironment &env = windowMain.GetEnvironment();
	
	igdeContainerReference bottomLine;
	bottomLine.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaX,
		igdeContainerFlow::esFirst ) );
	AddChild( bottomLine, igdeContainerBorder::eaBottom );
	
	igdeScrollBarListenerReference scrollView;
	scrollView.TakeOver( new cScrollView( *this ) );
	
	pSBHorizontal.TakeOver( new igdeScrollBar( env, igdeScrollBar::eoHorizontal ) );
	pSBHorizontal->AddListener( scrollView );
	bottomLine->AddChild( pSBHorizontal );
	
	pCBZoom.TakeOver( new igdeComboBox( env, 6, 7, true ) );
	pCBZoom->SetDescription( "Select zoom factor" );
	pCBZoom->AddItem( "100" );
	pCBZoom->AddItem( "150" );
	pCBZoom->AddItem( "200" );
	pCBZoom->AddItem( "300" );
	pCBZoom->AddItem( "400" );
	pCBZoom->AddItem( "600" );
	pCBZoom->AddItem( "800" );
	pCBZoom->SetSelection( 0 ); // 100
	bottomLine->AddChild( pCBZoom );
	
	igdeComboBoxListenerReference changeZoom;
	changeZoom.TakeOver( new cChangeZoom( *this ) );
	pCBZoom->AddListener( changeZoom );
	
	pSBVertical.TakeOver( new igdeScrollBar( env, igdeScrollBar::eoVertical ) );
	pSBVertical->AddListener( scrollView );
	AddChild( pSBVertical, igdeContainerBorder::eaRight );
	
	pViewImage = new feViewFIImage( windowMain );
	AddChild( pViewImage, igdeContainerBorder::eaCenter );
	pViewImage->FreeReference();
}

feViewFontImage::~feViewFontImage(){
	SetFont( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void feViewFontImage::SetFont( feFont *font ){
	if( font == pFont ){
		return;
	}
	
	if( pFont ){
		pFont->RemoveNotifier( pListener );
		pFont->FreeReference();
	}
	
	pFont = font;
	
	if( font ){
		font->AddReference();
		font->AddNotifier( pListener );
	}
	
	pViewImage->SetFont( font );
	
	UpdateScrollbarRanges();
}

void feViewFontImage::SetEnableRendering( bool enable ){
	pViewImage->SetEnableRendering( enable );
}



void feViewFontImage::OnAfterEngineStart(){
	pViewImage->OnAfterEngineStart();
}

void feViewFontImage::OnBeforeEngineStop(){
	pViewImage->OnBeforeEngineStop();
}

void feViewFontImage::ResetView(){
	pViewImage->ResetView();
}

void feViewFontImage::OnFrameUpdate( float elapsed ){
	pViewImage->OnFrameUpdate( elapsed );
}

void feViewFontImage::OnResize(){
	UpdateScrollbarRanges();
}



void feViewFontImage::UpdateScrollbarRanges(){
	const decPoint size( pViewImage->GetRenderAreaSize() );
	const decPoint &offset = pViewImage->GetOffset();
	
	pContentSize = pViewImage->GetContentSize();
	
	const decPoint range( decPoint().Largest( pContentSize - size ) );
	const decPoint pageSize( decPoint( 1, 1 ).Largest( size / 4 ) );
	
	pScrollOffset = range / 2;
	
	pSBHorizontal->SetUpper( decMath::max( range.x + pageSize.x, 0 ) );
	pSBVertical->SetUpper( decMath::max( range.y + pageSize.y, 0 ) );
	
	pSBHorizontal->SetPageSize( pageSize.x );
	pSBVertical->SetPageSize( pageSize.y );
	
	pSBHorizontal->SetValue( pScrollOffset.x - offset.x );
	pSBVertical->SetValue( pScrollOffset.y - offset.y );
	
	ScrollView();
}

void feViewFontImage::UpdateImageViewCanvas(){
	pViewImage->ResizeCanvas();
}

void feViewFontImage::ScrollView(){
	pViewImage->SetOffset( pScrollOffset - decPoint(
		pSBHorizontal->GetValue(), pSBVertical->GetValue() ) );
}
