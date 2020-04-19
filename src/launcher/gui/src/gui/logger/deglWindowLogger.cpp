/* 
 * Drag[en]gine GUI Launcher
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

#include <stdlib.h>
#include <string.h>

#include "deglWindowLogger.h"
#include "deglWindowLoggerListener.h"
#include "deglWindowLoggerTable.h"
#include "deglWindowLoggerTableItem.h"
#include "../../logger/deglLoggerHistory.h"
#include "../../logger/deglLoggerHistoryEntry.h"

#include <dragengine/common/exceptions.h>



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

deglWindowLogger::deglWindowLogger( FXApp *app ) :
FXTopWindow( app, "Logging History", NULL, NULL, DECOR_ALL, 0, 0, 600, 450, 0, 0, 0, 0, 0, 0 ){
	FXVerticalFrame *content;
	
	pListener = NULL;
	pLogger = NULL;
	pFontNormal = NULL;
	pFontBold = NULL;
	
	// create listener
	pListener = new deglWindowLoggerListener( this );
	if( ! pListener ) DETHROW( deeOutOfMemory );
	
	// create window
	content = new FXVerticalFrame( this, LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT, 0, 0, 0, 0, 0, 0, 0, 0 );
	
	pTableLogs = new deglWindowLoggerTable( content, NULL, 0, TABLE_COL_SIZABLE | TABLE_NO_COLSELECT | TABLE_NO_ROWSELECT
		| LAYOUT_FILL_X | LAYOUT_FILL_Y );
	if( ! pTableLogs ) DETHROW( deeOutOfMemory );
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
	SetLogger( NULL );
	if( pListener ) pListener->FreeReference();
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
	if( ! pFontNormal ) DETHROW( deeOutOfMemory );
	pFontNormal->create();
	
	fontDesc.weight = FXFont::Bold;
	pFontBold = new FXFont( getApp(), fontDesc );
	if( ! pFontBold ) DETHROW( deeOutOfMemory );
	pFontBold->create();
}



void deglWindowLogger::SetLogger( deglLoggerHistory *logger ){
	if( logger != pLogger ){
		if( pLogger ){
			pLogger->RemoveListener( pListener );
			pLogger->FreeReference();
		}
		
		pLogger = logger;
		
		if( logger ){
			logger->AddReference();
			logger->AddListener( pListener );
		}
		
		UpdateLogs();
	}
}



void deglWindowLogger::UpdateLogs(){
	ClearLogsTable();
	
	if( pLogger ){
		pLogger->GetMutex().Lock();
		
		const int count = pLogger->GetEntryCount();
		int i;
		
		try{
			for( i=0; i<count; i++ ){
				AddLogToTable( pLogger->GetEntryAt( i ) );
			}
			
		}catch( const deException & ){
			pLogger->GetMutex().Unlock();
			throw;
		}
		
		pLogger->GetMutex().Unlock();
	}
}

void deglWindowLogger::AddLogToTable( const deglLoggerHistoryEntry &entry ){
	int index = pTableLogs->getNumRows();
	FXFont *font = pFontNormal;
	FXColor color = 0;
	
	if( entry.GetType() == deglLoggerHistoryEntry::emtWarn ){
		//color = FXRGB( 255, 192, 0 );
		color = FXRGB( 128, 0, 255 );
		font = pFontBold;
		
	}else if( entry.GetType() == deglLoggerHistoryEntry::emtError ){
		color = FXRGB( 255, 0, 0 );
		font = pFontBold;
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

long deglWindowLogger::onMap( FXObject *sender, FXSelector selector, void *data ){
	int index = pTableLogs->getNumRows() - 1;
	
	raise();
	
	pTableLogs->selectRow( index );
	pTableLogs->makePositionVisible( index, 1 );
	
	return 1;
}

long deglWindowLogger::onClose( FXObject *sender, FXSelector selector, void *data ){
	hide();
	return 1;
}
