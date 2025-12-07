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

#include "projDialogDistribute.h"
#include "projWindowMain.h"
#include "../project.h"
#include "../project/projProject.h"
#include "../project/profile/projProfile.h"
#include "../task/projTaskDistribute.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/event/igdeActionExternOpen.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerBox.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class projDialogDistribute
///////////////////////////////

// Constructor, destructor
////////////////////////////

projDialogDistribute::projDialogDistribute( projWindowMain &windowMain, projProfile *profile ) :
igdeDialog( windowMain.GetEnvironment(), "Distribute" ),

pWindowMain( windowMain ),
pProfile( profile ),
pTaskDistribute( NULL ),
pCloseDialogOnFinished( false ),
pPrintToConsole( false ),
pSuccess( true )
{
	DEASSERT_NOTNULL( pProfile )
	
	igdeEnvironment &env = windowMain.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	
	
	pActionShowInFSManager.TakeOver( new igdeActionExternOpen( env, "Show in File Manager",
		env.GetStockIcon( igdeEnvironment::esiOpen ), "Show DELGA file in File Manager" ) );
	
	
	igdeContainerFlow::Ref content(igdeContainerFlow::Ref::NewWith(
		env, igdeContainerFlow::eaY, igdeContainerFlow::esLast, 5));
	
	// build information
	igdeContainer::Ref groupBox;
	helper.GroupBoxStatic( content, groupBox, "DELGA:" );
	
	helper.EditString( groupBox, "DELGA File:", "DELGA file being build.", pEditDelgaPath, NULL );
	pEditDelgaPath->SetEditable( false );
	
	helper.EditString( groupBox, "DELGA Size:", "Size of DELGA file.", pEditDelgaSize, NULL );
	pEditDelgaSize->SetEditable( false );
	
	helper.EditString( groupBox, "Files:", "Number of processed files.", pEditDelgaFileCount, NULL );
	pEditDelgaFileCount->SetEditable( false );
	
	helper.EditString( groupBox, "Directories:", "Number of processed directories.", pEditDelgaDirCount, NULL );
	pEditDelgaDirCount->SetEditable( false );
	
	// logs and info line
	igdeContainerFlow::Ref containerLogs(igdeContainerFlow::Ref::NewWith(
		env, igdeContainerFlow::eaY, igdeContainerFlow::esFirst, 5));
	
	helper.GroupBoxStaticFlow( containerLogs, groupBox, "Logs:", true );
	
	helper.EditString( groupBox, "Building logs.", pEditLogs, 80, 12, NULL );
	pEditLogs->SetEditable( false );
	
	// info line
	groupBox.TakeOver( new igdeContainerBox( env, igdeContainerBox::eaX, 5 ) );
	containerLogs->AddChild( groupBox );
	
	helper.Button( groupBox, pActionShowInFSManager );
	
	content->AddChild( containerLogs );
	
	// button line
	igdeContainer::Ref buttonBar;
	CreateButtonBar( buttonBar, "Close" );
	
	AddContent( content, buttonBar );
	
	pStartBuilding();
}

projDialogDistribute::~projDialogDistribute(){
	if( pTaskDistribute ){
		delete pTaskDistribute;
	}
}



// Management
///////////////

void projDialogDistribute::LogMessage( const char *message ){
	DEASSERT_NOTNULL( message )
	
	// add to text widget
	const bool atBottom = pEditLogs->GetBottomLine() == pEditLogs->GetLineCount() - 1;
	pEditLogs->AppendText( decString( message ) + "\n" );
	if( atBottom ){
		pEditLogs->SetBottomLine( pEditLogs->GetLineCount() - 1 );
	}
	
	// log to log file
	pWindowMain.GetLogger()->LogInfo( LOGSOURCE, message );
	
	// print on console
	if( pPrintToConsole ){
		printf( "%s\n", message );
	}
}

void projDialogDistribute::OnFrameUpdate(){
	if( ! pTaskDistribute ){
		return;
	}
	
	try{
		if( ! pTaskDistribute->Step() ){
			decPath path;
			path.SetFromNative( GetEnvironment().GetGameProject()->GetDirectoryPath() );
			path.AddUnixPath( pProfile->GetDelgaPath() );
			path.RemoveLastComponent();
			pActionShowInFSManager->SetPath( path.GetPathNative() );
			
			if( pCloseDialogOnFinished ){
				Cancel();
			}
			return;
		}
		
		decString text;
		text.Format( "%s (%s bytes)",
			igdeUIHelper::FormatSize1024( pTaskDistribute->GetDelgaSize() ).GetString(),
			igdeUIHelper::FormatSizeTousand( pTaskDistribute->GetDelgaSize() ).GetString() );
		pEditDelgaSize->SetText( text );
		
		pEditDelgaDirCount->SetText( igdeUIHelper::FormatSizeTousand(
			pTaskDistribute->GetDelgaDirectoryCount() ) );
		pEditDelgaFileCount->SetText( igdeUIHelper::FormatSizeTousand(
			pTaskDistribute->GetDelgaFileCount() ) );
		
		const decString &taskMessage = pTaskDistribute->GetMessage();
		if( taskMessage != pLastTaskMessage ){
			LogMessage( taskMessage );
		}
		
	}catch( const deException &e ){
		pSuccess = false;
		
		const decString &taskMessage = pTaskDistribute->GetMessage();
		if( taskMessage != pLastTaskMessage ){
			LogMessage( taskMessage );
		}
		
		LogMessage( igdeCommonDialogs::FormatException( e ) );
		LogMessage( "Distribution Failed!" );
		
		pWindowMain.GetLogger()->LogException( LOGSOURCE, e );
	}
}

void projDialogDistribute::SetCloseDialogOnFinished( bool closeDialogOnFinished ){
	pCloseDialogOnFinished = closeDialogOnFinished;
}

void projDialogDistribute::SetPrintToConsole( bool printToConsole ){
	pPrintToConsole = printToConsole;
}



// Private Functions
//////////////////////

void projDialogDistribute::pStartBuilding(){
	decPath path;
	path.SetFromUnix( pProfile->GetDelgaPath() );
	pEditDelgaPath->SetText( path.GetLastComponent() );
	pEditDelgaSize->SetText( "0" );
	pEditDelgaFileCount->SetText( "0" );
	pEditDelgaDirCount->SetText( "0" );
	
	pTaskDistribute = new projTaskDistribute( pWindowMain, *pWindowMain.GetProject(), *pProfile );
}
