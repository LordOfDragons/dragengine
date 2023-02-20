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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "igdeNativeFoxTextArea.h"
#include "igdeNativeFoxResizer.h"
#include "../../igdeContainer.h"
#include "../../igdeTextArea.h"
#include "../../igdeCommonDialogs.h"
#include "../../event/igdeAction.h"
#include "../../resources/igdeFont.h"
#include "../../resources/igdeTextSegment.h"
#include "../../resources/igdeTextStyle.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../theme/propertyNames.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Event map
//////////////

FXDEFMAP( igdeNativeFoxTextArea ) igdeNativeFoxTextAreaMap[] = {
	FXMAPFUNC( SEL_LEFTBUTTONPRESS, igdeNativeFoxTextArea::ID_SELF,
		igdeNativeFoxTextArea::onMouseLeftPress ),
	
	FXMAPFUNC( SEL_LEFTBUTTONRELEASE, igdeNativeFoxTextArea::ID_SELF,
		igdeNativeFoxTextArea::onMouseLeftRelease ),
	
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxTextArea::ID_SELF,
		igdeNativeFoxTextArea::onCommand ),
	
	FXMAPFUNC( SEL_FOCUSOUT, igdeNativeFoxTextArea::ID_SELF,
		igdeNativeFoxTextArea::onCommand ),
	
	FXMAPFUNC( SEL_CHANGED, igdeNativeFoxTextArea::ID_SELF,
		igdeNativeFoxTextArea::onChanged ),
	
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxTextArea::ID_RESIZER,
		igdeNativeFoxTextArea::onResizerDrag )
};


FXIMPLEMENT( igdeNativeFoxTextArea, FXVerticalFrame,
	igdeNativeFoxTextAreaMap, ARRAYNUMBER( igdeNativeFoxTextAreaMap ) )



// Class igdeNativeFoxTextArea
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeFoxTextArea::igdeNativeFoxTextArea(){ }

igdeNativeFoxTextArea::igdeNativeFoxTextArea( igdeTextArea &powner, FXComposite *pparent,
	const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags, const igdeGuiTheme &guitheme ) :
FXVerticalFrame( pparent, layoutFlags.flags | TextAreaFlagsBorder( powner ), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ),
pOwner( &powner ),
pFont( TextAreaFont( powner, guitheme ) ),
pTextArea( new FXText( this, this, ID_SELF, TextAreaFlags( powner ), 0, 0, 0, 0,
	TextAreaPadLeft( guitheme ), TextAreaPadRight( guitheme ),
	TextAreaPadTop( guitheme ), TextAreaPadBottom( guitheme ) ) ),
pStyles( NULL ),
pResizer( NULL )
{
	if( ! pOwner->GetVisible() ){
		hide();
	}
	
	pTextArea->setVisibleColumns( powner.GetColumns() );
	pTextArea->setVisibleRows( powner.GetRows() );
	pTextArea->setFont( (FXFont*)pFont->GetNativeFont() );
	
	UpdateEditable();
	UpdateEnabled();
	UpdateText();
	UpdateDescription();
	
	UpdateStyles();
	
	if( layoutFlags.canResizeVertical || ( layoutFlags.flags & LAYOUT_FILL_Y ) == 0 ){
		pResizer = new igdeNativeFoxResizer( this, this, ID_RESIZER );
	}
}

igdeNativeFoxTextArea::~igdeNativeFoxTextArea(){
	if( pTextArea ){
		// this is annoying. FXText stores unchecked indices to styles. since FXTextArea
		// is potentially destroyed in the super-class destructor we need to make sure
		// FXText does not access anymore the styles array we are going to delete
		pTextArea->clearText();
		pTextArea->setHiliteStyles( NULL );
		pTextArea->setStyled( false );
	}
	
	if( pStyles ){
		delete [] pStyles;
	}
}

igdeNativeFoxTextArea *igdeNativeFoxTextArea::CreateNativeWidget( igdeTextArea &powner ){
	if( ! powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const pparent = ( FXComposite* ) powner.GetParent()->GetNativeContainer();
	if( ! pparent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxTextArea( powner, pparent,
		igdeUIFoxHelper::GetChildLayoutFlagsAll( &powner ), *powner.GetGuiTheme() );
}

void igdeNativeFoxTextArea::PostCreateNativeWidget(){
	FXComposite &pparent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( pparent.id() ){
		create();
	}
}

void igdeNativeFoxTextArea::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxTextArea::UpdateStyles(){
	if( pStyles ){
		pTextArea->setHiliteStyles( NULL );
		pTextArea->setStyled( false );
		
		delete [] pStyles;
		pStyles = NULL;
	}
	
	pBuildStylesArray();
	
	if( pStyles ){
		pTextArea->setHiliteStyles( pStyles );
		pTextArea->setStyled( true );
		
		ApplyStyles();
	}
}

void igdeNativeFoxTextArea::ApplyStyles(){
	if( ! pStyles ){
		return;
	}
	
	const int count = pOwner->GetSegmentCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const igdeTextSegment &segment = pOwner->GetSegmentAt( i );
		
		const decString &styleName = segment.GetStyle();
		if( styleName.IsEmpty() ){
			continue;
		}
		
		const int style = pOwner->IndexOfStyleNamed( styleName );
		if( style == -1 ){
			continue;
		}
		
		pTextArea->changeStyle( segment.GetBegin(), segment.GetLength(), style + 1 );
	}
}

void igdeNativeFoxTextArea::UpdateText(){
	if( pOwner->GetText() != pTextArea->getText().text() ){
		pTextArea->setText( pOwner->GetText().GetString() );
	}
	
	ApplyStyles();
}

void igdeNativeFoxTextArea::UpdateEnabled(){
	if( pOwner->GetEnabled() ){
		pTextArea->enable();
		
	}else{
		pTextArea->disable();
	}
}

void igdeNativeFoxTextArea::UpdateDescription(){
	pTextArea->setTipText( pOwner->GetDescription().GetString() );
	pTextArea->setHelpText( pOwner->GetDescription().GetString() );
}

void igdeNativeFoxTextArea::UpdateEditable(){
	pTextArea->setEditable( pOwner->GetEditable() );
}

void igdeNativeFoxTextArea::Focus(){
	pTextArea->setFocus();
}

int igdeNativeFoxTextArea::GetCursorPosition() const{
	return pTextArea->getCursorPos();
}

void igdeNativeFoxTextArea::SetCursorPosition( int position ){
	pTextArea->setCursorPos( position );
}

int igdeNativeFoxTextArea::GetCursorColumn() const{
	return pTextArea->getCursorColumn();
}

int igdeNativeFoxTextArea::GetCursorRow() const{
	return pTextArea->getCursorRow();
}

void igdeNativeFoxTextArea::SetCursorColumn( int column ){
	pTextArea->setCursorColumn( column );
}

void igdeNativeFoxTextArea::SetCursorRow( int row ){
	pTextArea->setCursorRow( row );
}

int igdeNativeFoxTextArea::GetTopLine() const{
	int offset = pTextArea->getTopLine();
	int line = 0;
	
	while( offset > 0 ){
		line++;
		offset = pTextArea->prevLine( offset );
	}
	
	return line;
}

void igdeNativeFoxTextArea::SetTopLine( int line ){
	pTextArea->setTopLine( pTextArea->nextLine( 0, line ) );
}

int igdeNativeFoxTextArea::GetBottomLine() const{
	int offset = pTextArea->getBottomLine();
	int line = 0;
	
	while( offset > 0 ){
		line++;
		offset = pTextArea->prevLine( offset );
	}
	
	return line;
}

void igdeNativeFoxTextArea::SetBottomLine( int line ){
	pTextArea->setBottomLine( pTextArea->nextLine( 0, line ) );
}

int igdeNativeFoxTextArea::GetLineCount() const{
	return pTextArea->countLines( 0, pOwner->GetText().GetLength() ) + 1;
}

void igdeNativeFoxTextArea::UpdateColumns(){
	pTextArea->setVisibleColumns( pOwner->GetColumns() );
}

void igdeNativeFoxTextArea::UpdateRows(){
	pTextArea->setVisibleRows( pOwner->GetRows() );
}



int igdeNativeFoxTextArea::TextAreaFlagsBorder( const igdeTextArea & ){
	return FRAME_SUNKEN;
}

int igdeNativeFoxTextArea::TextAreaFlags( const igdeTextArea & ){
	return LAYOUT_FILL_X | LAYOUT_FILL_Y | TEXT_WORDWRAP;
}

igdeFont *igdeNativeFoxTextArea::TextAreaFont( const igdeTextArea &powner, const igdeGuiTheme &guitheme ){
	igdeFont::sConfiguration configuration;
	powner.GetEnvironment().GetApplicationFont( configuration );
	
	if( guitheme.HasProperty( igdeGuiThemePropertyNames::textFieldFontSizeAbsolute ) ){
		configuration.size = ( float )guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::textFieldFontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::textFieldFontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::textFieldFontSize, 1.0f );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSizeAbsolute ) ){
		configuration.size = ( float )guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f );
	}
	
	return powner.GetEnvironment().GetSharedFont( configuration );
}

int igdeNativeFoxTextArea::TextAreaPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::textFieldPaddingLeft, DEFAULT_PAD );
}

int igdeNativeFoxTextArea::TextAreaPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::textFieldPaddingRight, DEFAULT_PAD );
}

int igdeNativeFoxTextArea::TextAreaPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::textFieldPaddingTop, DEFAULT_PAD );
}

int igdeNativeFoxTextArea::TextAreaPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::textFieldPaddingBottom, DEFAULT_PAD );
}



// Events
///////////

long igdeNativeFoxTextArea::onMouseLeftPress( FXObject*, FXSelector, void *pdata ){
	const FXEvent &event = *( ( FXEvent* )pdata );
	const int position = pTextArea->getPosAt( event.win_x, event.win_y );
	
	const igdeTextSegment * const segment = pOwner->GetSegmentWith( position );
	if( ! segment || ! segment->GetAction() ){
		return 0;
	}
	
	return 1;
}

long igdeNativeFoxTextArea::onMouseLeftRelease( FXObject*, FXSelector, void *pdata ){
	const FXEvent &event = *( ( FXEvent* )pdata );
	const int position = pTextArea->getPosAt( event.win_x, event.win_y );
	
	const igdeTextSegment * const segment = pOwner->GetSegmentWith( position );
	if( ! segment || ! segment->GetAction() ){
		return 0;
	}
	
	if( event.click_count != 1 ){
		return 1;
	}
	
	try{
		segment->GetAction()->OnAction();
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
	}
	
	return 1;
}

long igdeNativeFoxTextArea::onCommand( FXObject*, FXSelector, void* ){
	if( ! pOwner->GetEnabled() ){
		return 0;
	}
	
	try{
		pOwner->SetText( pTextArea->getText().text() );
		pOwner->NotifyTextChanged();
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
		return 0;
	}
	
	return 1;
}

long igdeNativeFoxTextArea::onChanged( FXObject*, FXSelector, void* ){
	if( ! pOwner->GetEnabled() ){
		return 0;
	}
	
	try{
		pOwner->SetText( pTextArea->getText().text() );
		pOwner->NotifyTextChanging();
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
		return 0;
	}
	
	return 1;
}

long igdeNativeFoxTextArea::onResizerDrag( FXObject*, FXSelector, void *pdata ){
	const int distance = igdeNativeFoxResizer::SelCommandDraggedDistance( pdata );
	const int newHeight = getHeight() + distance;
	
	const int margin = pTextArea->getMarginTop() + pTextArea->getMarginBottom();
	const int lineHeight = pTextArea->getFont()->getFontHeight();
	
	pOwner->SetRows( decMath::max( ( newHeight - margin + lineHeight / 2 ) / lineHeight, 1 ) );
	
	return 0;
}



// Private Functions
//////////////////////

void igdeNativeFoxTextArea::pBuildStylesArray(){
	const int count = decMath::min( pOwner->GetStyleCount(), 254 );
	if( count == 0 ){
		return;
	}
	
	pStyles = new FXHiliteStyle[ count + 1 ];
	
	// FXHiliteStyle is a struct:
	// - FXColor normalForeColor
	// - FXColor normalBackColor
	// - FXColor selectForeColor
	// - FXColor selectBackColor
	// - FXColor hiliteForeColor
	// - FXColor hiliteBackColor
	// - FXColor activeBackColor
	// - FXuint style
	// 
	// whereas style can be these flags:
	// - STYLE_UNDERLINE
	// - STYLE_STRIKEOUT
	// - STYLE_BOLD
	// or 0
	
	const FXApp &aapp = *pTextArea->getApp();
	FXHiliteStyle defaultStyle;
	defaultStyle.normalForeColor = aapp.getForeColor();
	defaultStyle.normalBackColor = aapp.getBackColor();
	defaultStyle.selectForeColor = aapp.getSelforeColor();
	defaultStyle.selectBackColor = aapp.getSelbackColor();
	defaultStyle.hiliteForeColor = aapp.getHiliteColor();
	defaultStyle.hiliteBackColor = aapp.getShadowColor();
	defaultStyle.activeBackColor = aapp.getBackColor();
	defaultStyle.style = 0;
	
	int i;
	for( i=0; i<count; i++ ){
		const igdeTextStyle &style = *pOwner->GetStyleAt( i );
		
		FXHiliteStyle &foxStyle = pStyles[ i ];
		foxStyle = defaultStyle;
		
		if( style.GetHasColor() ){
			const decColor &color = style.GetColor();
			foxStyle.normalForeColor = FXRGB( ( FXint )( color.r * 255.0f ),
				( FXint )( color.g * 255.0f ), ( FXint )( color.b * 255.0f ) );
		}
		
		if( style.GetHasBgColor() ){
			const decColor &color = style.GetBgColor();
			foxStyle.normalBackColor = FXRGB( ( FXint )( color.r * 255.0f ),
				( FXint )( color.g * 255.0f ), ( FXint )( color.b * 255.0f ) );
		}
		
		foxStyle.style = 0;
		
		if( style.GetBold() ){
			foxStyle.style |= FXText::STYLE_BOLD;
		}
		if( style.GetUnderline() ){
			foxStyle.style |= FXText::STYLE_UNDERLINE;
		}
		if( style.GetStrikeThrough() ){
			foxStyle.style |= FXText::STYLE_STRIKEOUT;
		}
	}
}

#endif
