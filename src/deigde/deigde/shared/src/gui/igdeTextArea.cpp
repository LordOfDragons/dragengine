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

#include "igdeTextArea.h"
#include "igdeContainer.h"
#include "igdeCommonDialogs.h"
#include "native/toolkit.h"
#include "event/igdeAction.h"
#include "event/igdeTextAreaListener.h"
#include "native/fox/igdeNativeFoxTextArea.h"
#include "resources/igdeFont.h"
#include "resources/igdeTextStyle.h"
#include "resources/igdeTextSegment.h"
#include "resources/igdeTextSegmentReference.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeTextArea
///////////////////////

// Constructor, destructor
////////////////////////////

igdeTextArea::igdeTextArea( igdeEnvironment &environment, int columns, int rows,
const char *description ) :
igdeWidget( environment ),
pEnabled( true ),
pColumns( columns ),
pRows( rows ),
pEditable( true ),
pDescription( description )
{
	if( columns < 1 || rows < 0 ){
		DETHROW( deeInvalidParam );
	}
}

igdeTextArea::igdeTextArea( igdeEnvironment &environment, int columns, int rows,
	bool editable, const char *description ) :
igdeWidget( environment ),
pEnabled( true ),
pColumns( columns ),
pRows( rows ),
pEditable( editable ),
pDescription( description )
{
	if( columns < 1 || rows < 1 ){
		DETHROW( deeInvalidParam );
	}
}

igdeTextArea::~igdeTextArea(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeTextArea::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeTextArea::SetColumns( int columns ){
	if( columns < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pColumns == columns ){
		return;
	}
	
	pColumns = columns;
	OnColumnsChanged();
}

void igdeTextArea::SetRows( int rows ){
	if( rows < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pRows == rows ){
		return;
	}
	
	pRows = rows;
	OnRowsChanged();
}

void igdeTextArea::SetEditable( bool editable ){
	if( pEditable == editable ){
		return;
	}
	
	pEditable = editable;
	OnEditableChanged();
}

void igdeTextArea::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}



int igdeTextArea::GetStyleCount() const{
	return pStyles.GetCount();
}

igdeTextStyle *igdeTextArea::GetStyleAt( int index ) const{
	return ( igdeTextStyle* )pStyles.GetAt( index );
}

igdeTextStyle *igdeTextArea::GetStyleNamed( const char *name ) const{
	const int count = pStyles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		igdeTextStyle * const style = ( igdeTextStyle* )pStyles.GetAt( i );
		if( style->GetName() == name ){
			return style;
		}
	}
	
	return NULL;
}

int igdeTextArea::IndexOfStyleNamed( const char *name ) const{
	const int count = pStyles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( igdeTextStyle* )pStyles.GetAt( i ) )->GetName() == name ){
			return i;
		}
	}
	
	return -1;
}

void igdeTextArea::AddStyle( igdeTextStyle *style ){
	if( ! style || GetStyleNamed( style->GetName() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pStyles.Add( style );
	StylesChanged();
}

void igdeTextArea::RemoveAllStyles(){
	if( pStyles.GetCount() == 0 ){
		return;
	}
	
	pStyles.RemoveAll();
	StylesChanged();
}

void igdeTextArea::StylesChanged(){
	OnStylesChanged();
}



void igdeTextArea::SetText( const char *text ){
	if( pText == text ){
		return;
	}
	
	pSegments.RemoveAll();
	pText = text;
	OnTextChanged();
}

void igdeTextArea::AppendText( const char *text ){
	pText += text;
	OnTextChanged();
}

void igdeTextArea::AppendText( const char *text, const char *style ){
	AppendText( text, style, NULL );
}

void igdeTextArea::AppendText( const char *text, const char *style, igdeAction *action ){
	if( ! style ){
		DETHROW( deeInvalidParam );
	}
	
	if( style[ 0 ] == 0 && ! action ){
		AppendText( text );
		return;
	}
	
	const int begin = pText.GetLength();
	pText += text;
	const int end = pText.GetLength() - 1;
	
	igdeTextSegmentReference segment;
	segment.TakeOver( new igdeTextSegment( begin, end, style, action ) );
	pSegments.Add( segment.operator->() );
	
	OnTextChanged();
}

void igdeTextArea::DeleteText( int begin, int end ){
	if( begin < 0 || begin > pText.GetLength() || end < begin ){
		DETHROW( deeInvalidParam );
	}
	
	if( begin == end ){
		return;
	}
	
	const int count = pSegments.GetCount();
	igdeTextSegmentReference newSegment;
	const int length = end - begin;
	int i;
	
	for( i=count-1; i>=0; i-- ){
		const igdeTextSegment &segment = *( ( igdeTextSegment* )pSegments.GetAt( i ) );
		
		if( segment.GetBegin() >= begin && segment.GetEnd() < end ){
			pSegments.RemoveFrom( i );
			
		}else if( segment.GetBegin() < begin && segment.GetEnd() >= end ){
			newSegment.TakeOver( new igdeTextSegment( segment.GetBegin(),
				segment.GetEnd() - length, segment.GetStyle(), segment.GetAction() ) );
			pSegments.SetAt( i, newSegment.operator->() );
			
		}else if( segment.GetBegin() < begin && segment.GetEnd() >= begin ){
			newSegment.TakeOver( new igdeTextSegment( segment.GetBegin(), begin - 1,
				segment.GetStyle(), segment.GetAction() ) );
			pSegments.SetAt( i, newSegment.operator->() );
			
		}else if( segment.GetEnd() >= end && segment.GetBegin() < end ){
			newSegment.TakeOver( new igdeTextSegment( end, segment.GetEnd(),
				segment.GetStyle(), segment.GetAction() ) );
			pSegments.SetAt( i, newSegment.operator->() );
			
		}else if( segment.GetBegin() >= end ){
			newSegment.TakeOver( new igdeTextSegment( segment.GetBegin() - length,
				segment.GetEnd() - length, segment.GetStyle(), segment.GetAction() ) );
			pSegments.SetAt( i, newSegment.operator->() );
		}
	}
	
	pText = pText.GetLeft( begin ) + pText.GetMiddle( end );
	OnTextChanged();
}

void igdeTextArea::ClearText(){
	if( pText.IsEmpty() ){
		return;
	}
	
	pSegments.RemoveAll();
	pText.Empty();
	OnTextChanged();
}



int igdeTextArea::GetCursorPosition() const{
	if( ! GetNativeWidget() ){
		return 0;
	}
	
	igdeNativeFoxTextArea &native = *( ( igdeNativeFoxTextArea* )GetNativeWidget() );
	return native.GetTextArea().getCursorPos();
}

void igdeTextArea::SetCursorPosition( int position ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	if( position < 0 || position >= pText.GetLength() ){
		DETHROW( deeInvalidParam );
	}
	
	igdeNativeFoxTextArea &native = *( ( igdeNativeFoxTextArea* )GetNativeWidget() );
	native.GetTextArea().setCursorPos( position );
}

decPoint igdeTextArea::GetCursorCoordinate() const{
	if( ! GetNativeWidget() ){
		return decPoint();
	}
	
	igdeNativeFoxTextArea &native = *( ( igdeNativeFoxTextArea* )GetNativeWidget() );
	return decPoint( native.GetTextArea().getCursorColumn(), native.GetTextArea().getCursorRow() );
}

void igdeTextArea::SetCursorCoordinate( const decPoint &coordinate ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxTextArea &native = *( ( igdeNativeFoxTextArea* )GetNativeWidget() );
	native.GetTextArea().setCursorRow( coordinate.y );
	native.GetTextArea().setCursorColor( coordinate.x );
}



int igdeTextArea::GetTopLine() const{
	if( ! GetNativeWidget() ){
		return 0;
	}
	
	igdeNativeFoxTextArea &native = *( ( igdeNativeFoxTextArea* )GetNativeWidget() );
	FXText &widget = native.GetTextArea();
	
	int offset = widget.getTopLine();
	int line = 0;
	
	while( offset > 0 ){
		line++;
		offset = widget.prevLine( offset );
	}
	
	return line;
}

void igdeTextArea::SetTopLine( int line ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxTextArea &native = *( ( igdeNativeFoxTextArea* )GetNativeWidget() );
	native.GetTextArea().setTopLine( native.GetTextArea().nextLine( 0, line ) );
}

int igdeTextArea::GetBottomLine() const{
	if( ! GetNativeWidget() ){
		return 0;
	}
	
	igdeNativeFoxTextArea &native = *( ( igdeNativeFoxTextArea* )GetNativeWidget() );
	FXText &widget = native.GetTextArea();
	
	int offset = widget.getBottomLine();
	int line = 0;
	
	while( offset > 0 ){
		line++;
		offset = widget.prevLine( offset );
	}
	
	return line;
}

void igdeTextArea::SetBottomLine( int line ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxTextArea &native = *( ( igdeNativeFoxTextArea* )GetNativeWidget() );
	native.GetTextArea().setBottomLine( native.GetTextArea().nextLine( 0, line ) );
}

int igdeTextArea::GetLineCount() const{
	if( ! GetNativeWidget() ){
		return 0;
	}
	
	igdeNativeFoxTextArea &native = *( ( igdeNativeFoxTextArea* )GetNativeWidget() );
	return native.GetTextArea().countLines( 0, pText.GetLength() ) + 1;
}

void igdeTextArea::Focus(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxTextArea* )GetNativeWidget() )->Focus();
	}
}



int igdeTextArea::GetSegmentCount() const{
	return pSegments.GetCount();
}

const igdeTextSegment &igdeTextArea::GetSegmentAt( int index ) const{
	return *( ( igdeTextSegment* )pSegments.GetAt( index ) );
}

const igdeTextSegment *igdeTextArea::GetSegmentWith( int offset ) const{
	const int count = pSegments.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const igdeTextSegment * const segment = ( const igdeTextSegment * )pSegments.GetAt( i );
		if( offset >= segment->GetBegin() && offset <= segment->GetEnd() ){
			return segment;
		}
	}
	
	return NULL;
}

void igdeTextArea::SetTextSegment( int begin, int end, const char *style, igdeAction *action ){
	bool changed = pClearSegment( begin, end );
	
	if( style[ 0 ] || action ){
		igdeTextSegmentReference segment;
		segment.TakeOver( new igdeTextSegment( begin, end, style, action ) );
		pSegments.Add( segment.operator->() );
		changed = true;
	}
	
	if( changed ){
		OnTextChanged();
	}
}

void igdeTextArea::ClearTextSegment( int begin, int end ){
	if( pClearSegment( begin, end ) ){
		OnTextChanged();
	}
}



void igdeTextArea::AddListener( igdeTextAreaListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void igdeTextArea::RemoveListener( igdeTextAreaListener *listener ){
	pListeners.Remove( listener );
}

void igdeTextArea::NotifyTextChanged(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeTextAreaListener* )listeners.GetAt( i ) )->OnTextChanged( this );
	}
}

void igdeTextArea::NotifyTextChanging(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeTextAreaListener* )listeners.GetAt( i ) )->OnTextChanging( this );
	}
}



void igdeTextArea::CreateNativeWidget(){
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
	
	igdeNativeFoxTextArea * const foxWidget = new igdeNativeFoxTextArea(
		*this, foxParent, igdeUIFoxHelper::GetChildLayoutFlagsAll( this ), *GetGuiTheme() );
	SetNativeWidget( foxWidget );
	if( foxParent->id() ){
		foxWidget->create();
	}
}

void igdeTextArea::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( igdeNativeFoxTextArea* )GetNativeWidget();
	DropNativeWidget();
}

void igdeTextArea::OnTextChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxTextArea* )GetNativeWidget() )->UpdateText();
	}
}

void igdeTextArea::OnColumnsChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxTextArea &textField = *( ( igdeNativeFoxTextArea* )GetNativeWidget() );
	textField.GetTextArea().setVisibleColumns( pColumns );
}

void igdeTextArea::OnRowsChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxTextArea &textField = *( ( igdeNativeFoxTextArea* )GetNativeWidget() );
	textField.GetTextArea().setVisibleRows( pRows );
}

void igdeTextArea::OnEnabledChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxTextArea* )GetNativeWidget() )->UpdateEnabled();
	}
}

void igdeTextArea::OnEditableChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxTextArea* )GetNativeWidget() )->UpdateEditable();
	}
}

void igdeTextArea::OnDescriptionChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxTextArea* )GetNativeWidget() )->UpdateDescription();
	}
}

void igdeTextArea::OnStylesChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxTextArea* )GetNativeWidget() )->UpdateStyles();
	}
}

bool igdeTextArea::pClearSegment( int begin, int end ){
	if( begin < 0 || end >= pText.GetLength() ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pSegments.GetCount();
	igdeTextSegmentReference newSegment;
	bool changed = false;
	int i;
	
	for( i=count-1; i>=0; i-- ){
		const igdeTextSegment &segment = *( ( igdeTextSegment* )pSegments.GetAt( i ) );
		
		if( segment.GetBegin() >= begin && segment.GetEnd() <= end ){
			pSegments.RemoveFrom( i );
			changed = true;
			
		}else if( segment.GetBegin() < begin && segment.GetEnd() > end ){
			newSegment.TakeOver( new igdeTextSegment( end + 1, segment.GetEnd(),
				segment.GetStyle(), segment.GetAction() ) );
			pSegments.Add( newSegment.operator->() );
			
			newSegment.TakeOver( new igdeTextSegment( segment.GetBegin(), begin - 1,
				segment.GetStyle(), segment.GetAction() ) );
			pSegments.SetAt( i, newSegment.operator->() );
			
			changed = true;
			
		}else if( segment.GetBegin() < begin && segment.GetEnd() >= begin ){
			newSegment.TakeOver( new igdeTextSegment( segment.GetBegin(), begin - 1,
				segment.GetStyle(), segment.GetAction() ) );
			pSegments.SetAt( i, newSegment.operator->() );
			changed = true;
			
		}else if( segment.GetBegin() <= end && segment.GetEnd() > end ){
			newSegment.TakeOver( new igdeTextSegment( end + 1, segment.GetEnd(),
				segment.GetStyle(), segment.GetAction() ) );
			pSegments.SetAt( i, newSegment.operator->() );
			changed = true;
		}
		// otherwise segment is fully before start or after end so not affected
	}
	
	return changed;
}
