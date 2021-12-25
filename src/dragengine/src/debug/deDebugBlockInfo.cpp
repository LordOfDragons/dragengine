/* 
 * Drag[en]gine Game Engine
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

#include "deDebugBlockInfo.h"
#include "../deEngine.h"
#include "../common/exceptions.h"
#include "../common/string/unicode/decUTF8Decoder.h"
#include "../resources/canvas/deCanvasView.h"
#include "../resources/canvas/deCanvasText.h"
#include "../resources/canvas/deCanvasPaint.h"
#include "../resources/canvas/deCanvasManager.h"
#include "../resources/font/deFont.h"
#include "../resources/font/deFontManager.h"
#include "../systems/deGraphicSystem.h"



// Class deDebugBlockInfo
///////////////////////////

// Constructor, destructor
////////////////////////////

deDebugBlockInfo::deDebugBlockInfo( deEngine &engine ) :
pEngine( engine ),
pPadding( 1 ),
pSpacing( 10 )
{
	pView.TakeOver( engine.GetCanvasManager()->CreateCanvasView() );
	pFont.TakeOver( engine.GetFontManager()->LoadDebugFont() );
	
	pViewTitleBg.TakeOver( engine.GetCanvasManager()->CreateCanvasPaint() );
	pViewTitleBg->SetShapeType( deCanvasPaint::estRectangle );
	pViewTitleBg->SetThickness( 0.0f );
	pViewTitleBg->SetFillColor( decColor( 0.5f, 0.25f, 0.25f, 0.5f ) );
	pViewTitleBg->SetLineColor( decColor( 0.0f, 0.0f, 0.0f, 0.0f ) );
	pViewTitleBg->SetOrder( 0.0f );
	pView->AddCanvas( pViewTitleBg );
	
	pViewTitle.TakeOver( engine.GetCanvasManager()->CreateCanvasText() );
	pViewTitle->SetFont( pFont );
	pViewTitle->SetColor( decColor( 1.0f, 1.0f, 1.0f ) );
	pViewTitle->SetFontSize( ( float )pFont->GetLineHeight() );
	pViewTitle->SetText( "" );
	pViewTitle->SetOrder( 1.0f );
	pViewTitle->SetSize( pTextSize( pFont, "" ) );
	pView->AddCanvas( pViewTitle );
}

deDebugBlockInfo::~deDebugBlockInfo(){
	RemoveAllEntries();
	RemoveFromOverlay();
}



// Management
///////////////

void deDebugBlockInfo::SetFont( deFont *font ){
	if( ! font ){
		DETHROW( deeInvalidParam );
	}
	
	if( font == pFont ){
		return;
	}
	
	pFont = font;
	
	pViewTitle->SetFont( font );
	
	const int count = pEntries.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		sEntry &entry = *( ( sEntry* )pEntries.GetAt( i ) );
		entry.viewLabel->SetFont( font );
		entry.viewText->SetFont( font );
		entry.viewLabel->SetSize( pTextSize( *font, entry.label ) );
		entry.viewText->SetSize( pTextSize( *font, entry.text ) );
	}
	
	UpdateView();
}

void deDebugBlockInfo::SetPadding( int padding ){
	if( padding == pPadding ){
		return;
	}
	if( padding < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pPadding = padding;
	
	UpdateView();
}

void deDebugBlockInfo::SetSpacing( int spacing ){
	if( spacing == pSpacing ){
		return;
	}
	if( spacing < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pSpacing = spacing;
	
	UpdateView();
}



void deDebugBlockInfo::SetTitle( const char *title ){
	pViewTitle->SetText( title );
	pViewTitle->SetSize( pTextSize( pFont, title ) );
}

void deDebugBlockInfo::SetTitleColor( const decColor &color ){
	pViewTitle->SetColor( color );
}

void deDebugBlockInfo::SetTitleBgColor( const decColor &color ){
	pViewTitleBg->SetFillColor( color );
}



void deDebugBlockInfo::AddToOverlay(){
	if( pView->GetParentView() ){
		return;
	}
	
	pEngine.GetGraphicSystem()->GetDebugOverlayCanvas()->AddCanvas( pView );
}

void deDebugBlockInfo::RemoveFromOverlay(){
	if( ! pView->GetParentView() ){
		return;
	}
	
	pEngine.GetGraphicSystem()->GetDebugOverlayCanvas()->RemoveCanvas( pView );
}



int deDebugBlockInfo::GetEntryCount() const{
	return pEntries.GetCount();
}

void deDebugBlockInfo::AddEntry( const char *label, const char *text,
const decColor &color, const decColor &bgColor ){
	deCanvasManager &canvasManager = *pEngine.GetCanvasManager();
	sEntry * entry = NULL;
	
	try{
		entry = new sEntry;
		entry->label = label;
		entry->text = text;
		entry->color = color;
		entry->bgColor = bgColor;
		
		entry->view.TakeOver( canvasManager.CreateCanvasView() );
		
		entry->viewBg.TakeOver( canvasManager.CreateCanvasPaint() );
		entry->viewBg->SetShapeType( deCanvasPaint::estRectangle );
		entry->viewBg->SetThickness( 0.0f );
		entry->viewBg->SetFillColor( bgColor );
		entry->viewBg->SetLineColor( decColor( 0.0f, 0.0f, 0.0f, 0.0f ) );
		entry->viewBg->SetOrder( 0.0f );
		entry->view->AddCanvas( entry->viewBg );
		
		entry->viewLabel.TakeOver( canvasManager.CreateCanvasText() );
		entry->viewLabel->SetFont( pFont );
		entry->viewLabel->SetColor( color );
		entry->viewLabel->SetFontSize( ( float )pFont->GetLineHeight() );
		entry->viewLabel->SetText( label );
		entry->viewLabel->SetOrder( 1.0f );
		entry->viewLabel->SetSize( pTextSize( pFont, label ) );
		entry->view->AddCanvas( entry->viewLabel );
		
		entry->viewText.TakeOver( canvasManager.CreateCanvasText() );
		entry->viewText->SetFont( pFont );
		entry->viewText->SetColor( color );
		entry->viewText->SetFontSize( ( float )pFont->GetLineHeight() );
		entry->viewText->SetText( text );
		entry->viewText->SetOrder( 2.0f );
		entry->viewText->SetSize( pTextSize( pFont, text ) );
		entry->view->AddCanvas( entry->viewText );
		
		pView->AddCanvas( entry->view );
		
		pEntries.Add( entry );
		
	}catch( const deException & ){
		if( entry ){
			delete entry;
		}
		throw;
	}
}

void deDebugBlockInfo::SetEntryLabel( int index, const char *label ){
	sEntry &entry = *( ( sEntry* )pEntries.GetAt( index ) );
	entry.label = label;
	entry.viewLabel->SetText( label );
	entry.viewLabel->SetSize( pTextSize( pFont, label ) );
}

void deDebugBlockInfo::SetEntryText( int index, const char *text ){
	sEntry &entry = *( ( sEntry* )pEntries.GetAt( index ) );
	entry.text = text;
	entry.viewText->SetText( text );
	entry.viewText->SetSize( pTextSize( pFont, text ) );
}

void deDebugBlockInfo::SetEntryColor( int index, const decColor &color ){
	sEntry &entry = *( ( sEntry* )pEntries.GetAt( index ) );
	entry.color = color;
	entry.viewLabel->SetColor( color );
	entry.viewText->SetColor( color );
}

void deDebugBlockInfo::SetEntryBgColor( int index, const decColor &color ){
	sEntry &entry = *( ( sEntry* )pEntries.GetAt( index ) );
	entry.bgColor = color;
	entry.viewBg->SetFillColor( color );
}

void deDebugBlockInfo::RemoveEntry( int index ){
	sEntry * const entry = ( sEntry* )pEntries.GetAt( index );
	pView->RemoveCanvas( entry->view );
	delete entry;
	pEntries.RemoveFrom( index );
}

void deDebugBlockInfo::RemoveAllEntries(){
	while( pEntries.GetCount() > 0 ){
		RemoveEntry( pEntries.GetCount() - 1 );
	}
}



void deDebugBlockInfo::UpdateView(){
	const int count = pEntries.GetCount();
	int maxLabelWidth = 0;
	int maxTextWidth = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		const sEntry &entry = *( ( sEntry* )pEntries.GetAt( i ) );
		const decPoint &labelSize = entry.viewLabel->GetSize();
		const decPoint &textSize = entry.viewText->GetSize();
		if( labelSize.x > maxLabelWidth ){
			maxLabelWidth = labelSize.x;
		}
		if( textSize.x > maxTextWidth ){
			maxTextWidth = textSize.x;
		}
	}
	
	const int offsetLabel = pPadding;
	const int offsetText = pPadding + maxLabelWidth + pSpacing;
	const int width = pPadding + decMath::max( pViewTitle->GetSize().x,
		maxLabelWidth + pSpacing + maxTextWidth ) + pPadding;
	decPoint position;
	
	pViewTitle->SetPosition( decPoint( pPadding, pPadding ) );
	pViewTitleBg->SetSize( decPoint( width, pPadding * 2 + pViewTitle->GetSize().y ) );
	position.y += pViewTitleBg->GetSize().y;
	
	for( i=0; i<count; i++ ){
		const sEntry &entry = *( ( sEntry* )pEntries.GetAt( i ) );
		entry.viewLabel->SetPosition( decPoint( offsetLabel, pPadding ) );
		entry.viewText->SetPosition( decPoint( offsetText, pPadding ) );
		entry.view->SetSize( decPoint( width, pPadding * 2 + decMath::max(
			entry.viewLabel->GetSize().y, entry.viewText->GetSize().y ) ) );
		entry.viewBg->SetSize( entry.view->GetSize() );
		entry.view->SetPosition( position );
		position.y += entry.view->GetSize().y;
	}
	
	pView->SetSize( decPoint( width, position.y ) );
}



decPoint deDebugBlockInfo::pTextSize( const deFont &font, const char *text ){
	decUTF8Decoder utf8Decoder;
	utf8Decoder.SetString( text );
	
	int textHeight = font.GetLineHeight();
	int lineWidth = 0;
	int textWidth = 0;
	
	while( ! utf8Decoder.HasReachedEnd() ){
		const int character = utf8Decoder.DecodeNextCharacter();
		
		if( character == '\n' ){
			textHeight += font.GetLineHeight();
			lineWidth = 0;
			
		}else if( character >= 0 ){
			const deFontGlyph &glyph = font.GetGlyph( character );
			const int extendEnd = glyph.GetWidth() - glyph.GetAdvance() - glyph.GetBearing();
			lineWidth += glyph.GetAdvance();
			if( lineWidth + extendEnd > textWidth ){
				textWidth = lineWidth + extendEnd;
			}
		}
	}
	
	return decPoint( textWidth, textHeight );
}
