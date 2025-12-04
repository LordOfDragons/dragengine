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

#include "igdeDialogEngine.h"
#include "igdeDEStatus.h"
#include "igdeDEModuleStatus.h"
#include "igdeDEParameters.h"
#include "igdeDEConsole.h"
#include "../igdeApplication.h"
#include "../igdeContainerReference.h"
#include "../igdeMainWindow.h"
#include "../igdeTabBook.h"
#include "../igdeWidget::Ref.h"

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
	igdeWidget::Ref panel;
	
	SetSize(igdeApplication::app().DisplayScaled(decPoint(800, 600)));
	
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
