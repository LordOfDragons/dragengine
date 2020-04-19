/* 
 * Drag[en]gine Basic Crash Recovery Module
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

// includes
#include "decrbPanelErrorTrace.h"
#include "decrbWindowMain.h"
#include "dragengine/deEngine.h"
#include "dragengine/errortracing/deErrorTrace.h"
#include "dragengine/errortracing/deErrorTracePoint.h"
#include "dragengine/errortracing/deErrorTraceValue.h"
#include "dragengine/systems/modules/deLoadableModule.h"
#include "dragengine/common/exceptions.h"



// Events
///////////
FXDEFMAP( decrbPanelErrorTrace ) decrbPanelErrorTraceMap[] = {
};



// Class decrbPanelErrorTrace
///////////////////////////////
	
FXIMPLEMENT( decrbPanelErrorTrace, FXVerticalFrame, decrbPanelErrorTraceMap, ARRAYNUMBER( decrbPanelErrorTraceMap ) )

// Constructor, destructor
////////////////////////////

decrbPanelErrorTrace::decrbPanelErrorTrace(){ }

decrbPanelErrorTrace::decrbPanelErrorTrace( decrbWindowMain *windowMain, FXComposite *container ) :
FXVerticalFrame( container, LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT, 5, 5, 5, 5 ){
	if( ! windowMain ) DETHROW( deeInvalidParam );
	int padding = 3;
	int spacing = 3;
	
	// prepare
	pWndMain = windowMain;
	
	// engine status
	FXHorizontalFrame *frameLine = new FXHorizontalFrame( this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
	new FXLabel( frameLine, "Error:" );
	pEditError = new FXTextField( frameLine, 10, NULL, 0, TEXTFIELD_READONLY | FRAME_SUNKEN | LAYOUT_FILL_X );
	
	FXGroupBox *groupBox = new FXGroupBox( this, "Trace:", GROUPBOX_TITLE_LEFT | FRAME_RIDGE | LAYOUT_FILL_X
		| LAYOUT_FILL_Y, 0, 0, 0, 0,
		padding, padding, padding, padding );
	FXVerticalFrame *frameBox = new FXVerticalFrame( groupBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
	pTreeTrace = new FXTreeList( frameBox, NULL, ID_TREETRACE, TREELIST_EXTENDEDSELECT
		| TREELIST_SHOWS_LINES | TREELIST_SHOWS_BOXES | TREELIST_ROOT_BOXES
		| FRAME_SUNKEN | LAYOUT_FILL_X | LAYOUT_FILL_Y );
	
	UpdateTrace();
}

decrbPanelErrorTrace::~decrbPanelErrorTrace(){
}



// Management
///////////////

void decrbPanelErrorTrace::UpdateTrace(){
	deErrorTrace *trace = pWndMain->GetEngine()->GetErrorTrace();
	int i, pointCount = trace->GetPointCount();
	deErrorTracePoint *tracePoint;
	FXTreeItem *treeItem;
	FXString traceText;
	int j, valueCount;
	
	pEditError->setText( decrbWindowMain::GetTextForError( trace->GetError() ) );
	
	for( i=0; i<pointCount; i++ ){
		tracePoint = trace->GetPoint( i );
		
		/*
		if( tracePoint->GetSourceModule() ){
			traceText.format( "%i) %s, %s at %i\n", i + 1, tracePoint->GetSourceModule()->GetName(),
				tracePoint->GetSourceFunction(), tracePoint->GetSourceLine() );
		}else{
			traceText.format( "%i) Game Engine, %s at %i\n", i + 1, tracePoint->GetSourceFunction(),
				tracePoint->GetSourceLine() );
		}
		pEditTrace->appendText( traceText.text(), traceText.length() );
		*/
		
		if( tracePoint->GetSourceModule() ){
			traceText.format( "%i. %s, %s at %i", i + 1,
				tracePoint->GetSourceModule()->GetName().GetString(),
				tracePoint->GetSourceFunction(),
				tracePoint->GetSourceLine() );
			
		}else{
			traceText.format( "%i. Game Engine, %s at %i", i + 1,
				tracePoint->GetSourceFunction(),
				tracePoint->GetSourceLine() );
		}
		
		treeItem = pTreeTrace->appendItem( NULL, traceText, NULL, NULL, NULL, false );
		valueCount = tracePoint->GetValueCount();
		for( j=0; j<valueCount; j++ ){
			pAddErrorTraceSubValues( 0, treeItem, tracePoint->GetValue( j ) );
		}
		
		//pEditTrace->appendText( "\n", 1 );
	}
}



// Events
///////////



// Private Functions
//////////////////////

void decrbPanelErrorTrace::pAddErrorTraceSubValues( int level, FXTreeItem *treeItem, deErrorTraceValue *traceValue ){
	int i, valueCount = traceValue->GetSubValueCount();
	FXTreeItem *subTreeItem;
	FXString traceText;
	
	// add value
	/*
	if( traceValue->GetSubValueCount() > 0 ){
		boxChar = '+';
	}else{
		boxChar = '-';
	}
	traceText.format( "  %c %s = '%s'\n", boxChar, traceValue->GetName(), traceValue->GetValue() );
	for( i=0; i<level; i++ ) traceText.prepend( "  " );
	pEditTrace->appendText( traceText.text(), traceText.length() );
	*/
	
	traceText.format( "%s = %s", traceValue->GetName(), traceValue->GetValue() );
	subTreeItem = pTreeTrace->appendItem( treeItem, traceText, NULL, NULL, NULL, false );
	
	for( i=0; i<valueCount; i++ ){
		pAddErrorTraceSubValues( level + 1, subTreeItem, traceValue->GetSubValue( i ) );
	}
}
