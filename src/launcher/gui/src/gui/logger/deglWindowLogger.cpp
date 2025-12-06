/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include <string.h>

#include "deglWindowLogger.h"
#include "deglWindowLoggerListener.h"
#include "deglWindowLoggerTable.h"
#include "deglWindowLoggerTableItem.h"

#include <delauncher/logger/delLoggerHistoryEntry.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/threading/deMutexGuard.h>



// Events
///////////

FXDEFMAP( deglWindowLogger ) deglWindowLoggerMap[]={
	FXMAPFUNC( SEL_MAP, 0, deglWindowLogger::onMap ),
	FXMAPFUNC( SEL_CLOSE, 0, deglWindowLogger::onClose ),
	FXMAPFUNC( SEL_CHORE, FXTopWindow::ID_CLOSE, deglWindowLogger::onClose ),
	FXMAPFUNC( SEL_SIGNAL, FXTopWindow::ID_CLOSE, deglWindowLogger::onClose ),
	FXMAPFUNC( SEL_TIMEOUT, FXTopWindow::ID_CLOSE, deglWindowLogger::onClose ),
	FXMAPFUNC( SEL_COMMAND, FXTopWindow::ID_CLOSE, deglWindowLogger::onClose ),
};

	

// Class deglWindowLogger
///////////////////////////

FXIMPLEMENT( deglWindowLogger, FXTopWindow, deglWindowLoggerMap, ARRAYNUMBER( deglWindowLoggerMap ) )

// Constructor, destructor
////////////////////////////

deglWindowLogger::deglWindowLogger(){ }

deglWindowLogger::deglWindowLogger( FXApp *papp ) :
FXTopWindow( papp, "Logging History", nullptr, nullptr, DECOR_ALL, 0, 0, 600, 450, 0, 0, 0, 0, 0, 0 ),
pListener( delLoggerHistoryListener::Ref::NewWith(*this) ),
pFontNormal( nullptr ),
pFontBold( nullptr ),
pTableLogs( nullptr )
{
	// create window
	FXVerticalFrame * const content = new FXVerticalFrame( this,
		LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT, 0, 0, 0, 0, 0, 0, 0, 0 );
	
	pTableLogs = new deglWindowLoggerTable( content, nullptr, 0,
		TABLE_COL_SIZABLE | TABLE_NO_COLSELECT | TABLE_NO_ROWSELECT | LAYOUT_FILL_X | LAYOUT_FILL_Y );
	pTableLogs->setEditable( false );
	pTableLogs->setTableSize( 0, 2 );
	
	pTableLogs->getRowHeader()->setWidth( 0 );
	
	pTableLogs->setColumnText( 0, "Source" );
	pTableLogs->setColumnJustify( 0, FXHeaderItem::CENTER_X );
	pTableLogs->setColumnWidth( 0, 150 );
	
	pTableLogs->setColumnText( 1, "Message" );
	pTableLogs->setColumnJustify( 1, FXHeaderItem::LEFT );
	pTableLogs->setColumnWidth( 1, 420 );
	
	create();
}

deglWindowLogger::~deglWindowLogger(){
	SetLogger( nullptr );
}



// Management
///////////////

void deglWindowLogger::create(){
	FXTopWindow::create();
	
	FXFontDesc fontDesc;
	
	fontDesc = getApp()->getNormalFont()->getFontDesc();
//#ifdef OS_UNIX
	fontDesc.size = ( int )( ( float )fontDesc.size * 0.85f );
//#else
//	fontDesc.size = ( int )( ( float )fontDesc.size * 0.1f );
//#endif
	pFontNormal = new FXFont( getApp(), fontDesc );
	pFontNormal->create();
	
	fontDesc.weight = FXFont::Bold;
	pFontBold = new FXFont( getApp(), fontDesc );
	pFontBold->create();
}



void deglWindowLogger::SetLogger( delLoggerHistory *logger ){
	if( pLogger == logger ){
		return;
	}
	
	if( pLogger ){
		pLogger->RemoveListener( pListener );
	}
	
	pLogger = logger;
	
	if( logger ){
		logger->AddListener( pListener );
	}
	
	UpdateLogs();
}



void deglWindowLogger::UpdateLogs(){
	ClearLogsTable();
	
	if( ! pLogger ){
		return;
	}
	
	const deMutexGuard lock( pLogger->GetMutex() );
	const int count = pLogger->GetEntryCount();
	int i;
	for( i=0; i<count; i++ ){
		AddLogToTable( pLogger->GetEntryAt( i ) );
	}
}

void deglWindowLogger::AddLogToTable( const delLoggerHistoryEntry &entry ){
	int index = pTableLogs->getNumRows();
	FXFont *font = pFontNormal;
	FXColor color = 0;
	
	switch( entry.GetType() ){
	case delLoggerHistoryEntry::emtWarn:
		//color = FXRGB( 255, 192, 0 );
		color = FXRGB( 128, 0, 255 );
		font = pFontBold;
		break;
		
	case delLoggerHistoryEntry::emtError:
		color = FXRGB( 255, 0, 0 );
		font = pFontBold;
		break;
		
	case delLoggerHistoryEntry::emtInfo:
		font = pFontNormal;
		color = 0;
		break;
	}
	
	pTableLogs->insertRows( index );
	
	pTableLogs->setItemText( index, 0, entry.GetSource().GetString() );
	pTableLogs->setItemJustify( index, 0, FXTableItem::LEFT );
	pTableLogs->SetCellTextProps( index, 0, color, font );
	
	pTableLogs->setItemText( index, 1, entry.GetMessage().GetString() );
	pTableLogs->setItemJustify( index, 1, FXTableItem::LEFT );
	pTableLogs->SetCellTextProps( index, 1, color, font );
	
	if( index == pLogger->GetHistorySize() ){
		pTableLogs->removeRows( 0 );
		index--;
	}
	
	pTableLogs->selectRow( index );
	pTableLogs->makePositionVisible( index, 1 );
	
	/*if( shown() ){
		forceRefresh();
		repaint();
	}*/
}

void deglWindowLogger::ClearLogsTable(){
	if( pTableLogs->getNumRows() > 0 ){
		pTableLogs->removeRows( 0, pTableLogs->getNumRows() );
	}
	
	/*if( shown() ){
		forceRefresh();
		repaint();
	}*/
}



// Events
///////////

long deglWindowLogger::onMap( FXObject*, FXSelector, void* ){
	int index = pTableLogs->getNumRows() - 1;
	
	raise();
	
	pTableLogs->selectRow( index );
	pTableLogs->makePositionVisible( index, 1 );
	
	return 1;
}

long deglWindowLogger::onClose( FXObject*, FXSelector, void* ){
	hide();
	return 1;
}
