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

#include "igdeWPUndoHistory.h"
#include "../igdeUIHelper.h"
#include "../igdeButton.h"
#include "../igdeListBox.h"
#include "../igdeGroupBox.h"
#include "../igdeGroupBoxReference.h"
#include "../igdeContainerReference.h"
#include "../event/igdeActionUndo.h"
#include "../event/igdeActionRedo.h"
#include "../event/igdeActionClearUndo.h"
#include "../layout/igdeContainerBorder.h"
#include "../layout/igdeContainerBorderReference.h"
#include "../../environment/igdeEnvironment.h"
#include "../../undo/igdeUndo.h"
#include "../../undo/igdeUndoSystem.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class igdeWPUndoHistory
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWPUndoHistory::igdeWPUndoHistory( igdeEnvironment &environment ) :
igdeContainerBox( environment, igdeContainerBox::eaY ),
pUndoSystem( NULL )
{
	igdeUIHelper &helper = environment.GetUIHelperProperties();
	
	// group box
	igdeGroupBoxReference groupBoxFrame;
	groupBoxFrame.TakeOver( new igdeGroupBox( environment, "Undo History:" ) );
	groupBoxFrame->SetStretchLast( true );
	AddChild( groupBoxFrame );
	
	igdeContainerBorderReference groupBox;
	groupBox.TakeOver( new igdeContainerBorder( environment ) );
	groupBoxFrame->AddChild( groupBox );
	
	// buttons
	igdeContainerReference buttons;
	buttons.TakeOver( new igdeContainerBox( environment, igdeContainerBox::eaY ) );
	groupBox->AddChild( buttons, igdeContainerBorder::eaTop );
	
	// button row 1
	igdeContainerReference buttonLine;
	buttonLine.TakeOver( new igdeContainerBox( environment, igdeContainerBox::eaX ) );
	buttons->AddChild( buttonLine );
	
	pActionUndo.TakeOver( new igdeActionUndo( environment ) );
	helper.Button( buttonLine, pActionUndo );
	
	pActionRedo.TakeOver( new igdeActionRedo( environment ) );
	helper.Button( buttonLine, pActionRedo );
	
	// button row 2
	buttonLine.TakeOver( new igdeContainerBox( environment, igdeContainerBox::eaX ) );
	buttons->AddChild( buttonLine );
	
	pActionClear.TakeOver( new igdeActionClearUndo( environment ) );
	helper.Button( buttonLine, pActionClear );
	
	// undo action list
	pListUndo.TakeOver( new igdeListBox( environment, 10 ) );
	groupBox->AddChild( pListUndo, igdeContainerBorder::eaCenter );
}

igdeWPUndoHistory::~igdeWPUndoHistory(){
}



// Management
///////////////

void igdeWPUndoHistory::SetUndoSystem( igdeUndoSystem *undoSystem ){
	if( undoSystem == pUndoSystem ){
		return;
	}
	
	pUndoSystem = undoSystem;
	
	pActionUndo->SetUndoSystem( undoSystem );
	pActionRedo->SetUndoSystem( undoSystem );
	pActionClear->SetUndoSystem( undoSystem );
	
	UpdateUndo();
}



void igdeWPUndoHistory::UpdateUndo(){
	pListUndo->RemoveAllItems();
	
	if( ! pUndoSystem ){
		return;
	}
	
	igdeIcon * const iconUndo = pActionUndo->GetIcon();
	igdeIcon * const iconRedo = pActionRedo->GetIcon();
	const int redoCount = pUndoSystem->GetRedoableCount();
	const int count = pUndoSystem->GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const igdeUndo &undo = *pUndoSystem->GetAt( i );
		//undo.GetLongInfo(), undo.GetMemoryConsumption()
		
		if( i < redoCount ){
			pListUndo->AddItem( undo.GetShortInfo(), iconRedo );
			
		}else{
			pListUndo->AddItem( undo.GetShortInfo(), iconUndo );
		}
	}
	
	if( count > 0 ){
		pListUndo->SetSelection( decMath::min( redoCount, count - 1 ) );
	}
}
