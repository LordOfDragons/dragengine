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

#include "igdeDialogEngine.h"
#include "igdeDEStatus.h"
#include "igdeDEModuleStatus.h"
#include "igdeDEParameters.h"
#include "igdeDEConsole.h"
#include "../igdeContainerReference.h"
#include "../igdeMainWindow.h"
#include "../igdeTabBook.h"
#include "../igdeWidgetReference.h"

#include <dragengine/common/exceptions.h>



// Class igdeDialogEngine
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeDialogEngine::igdeDialogEngine( igdeMainWindow &mainWindow ) :
igdeDialog( mainWindow.GetEnvironment(), "Engine Control Center" ),
pMainWindow( mainWindow )
{
	igdeEnvironment &environment = mainWindow.GetEnvironment();
	igdeWidgetReference panel;
	
	SetInitialSize( decPoint( 800, 600 ) );
	
	pTabPanels.TakeOver( new igdeTabBook( environment ) );
	
	panel.TakeOver( new igdeDEStatus( *this ) );
	pTabPanels->AddChild( panel, "Engine Status" );
	
	panel.TakeOver( new igdeDEModuleStatus( *this ) );
	pTabPanels->AddChild( panel, "Module Status" );
	
	panel.TakeOver( new igdeDEParameters( *this ) );
	pTabPanels->AddChild( panel, "Parameters" );
	
	panel.TakeOver( new igdeDEConsole( *this ) );
	pTabPanels->AddChild( panel, "Console" );
	
	
	igdeContainerReference buttonBar;
	CreateButtonBar( buttonBar, "Close" );
	
	AddContent( pTabPanels, buttonBar );
}

igdeDialogEngine::~igdeDialogEngine(){
}
