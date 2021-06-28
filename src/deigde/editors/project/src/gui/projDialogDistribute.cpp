/* 
 * Drag[en]gine IGDE Project Editor
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
#include <deigde/gui/igdeContainerReference.h>
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

projDialogDistribute::projDialogDistribute( projWindowMain &windowMain ) :
igdeDialog( windowMain.GetEnvironment(), "Distribute" ),

pWindowMain( windowMain ),
pProfile( windowMain.GetProject()->GetActiveProfile() ),
pTaskDistribute( NULL )
{
	if( ! pProfile ){
		DETHROW( deeInvalidParam );
	}
	
	igdeEnvironment &env = windowMain.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	
	
	pActionShowInFSManager.TakeOver( new igdeActionExternOpen( env, "Show in File Manager",
		env.GetStockIcon( igdeEnvironment::esiOpen ), "Show DELGA file in File Manager" ) );
	
	
	igdeContainerReference content;
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY, igdeContainerFlow::esLast, 5 ) );
	
	// build information
	igdeContainerReference groupBox;
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
	igdeContainerReference containerLogs;
	containerLogs.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY, igdeContainerFlow::esFirst, 5 ) );
	
	helper.GroupBoxStaticFlow( containerLogs, groupBox, "Logs:", true );
	
	helper.EditString( groupBox, "Building logs.", pEditLogs, 80, 12, NULL );
	pEditLogs->SetEditable( false );
	
	// info line
	groupBox.TakeOver( new igdeContainerBox( env, igdeContainerBox::eaX, 5 ) );
	containerLogs->AddChild( groupBox );
	
	helper.Button( groupBox, pActionShowInFSManager );
	
	content->AddChild( containerLogs );
	
	// button line
	igdeContainerReference buttonBar;
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
	if( ! message ){
		DETHROW( deeInvalidParam );
	}
	
	const bool atBottom = pEditLogs->GetBottomLine() == pEditLogs->GetLineCount() - 1;
	pEditLogs->AppendText( message );
	if( atBottom ){
		pEditLogs->SetBottomLine( pEditLogs->GetLineCount() - 1 );
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
			LogMessage( taskMessage + "\n" );
		}
		
	}catch( const deException &e ){
		const decString &taskMessage = pTaskDistribute->GetMessage();
		if( taskMessage != pLastTaskMessage ){
			LogMessage( taskMessage + "\n" );
		}
		
		LogMessage( igdeCommonDialogs::FormatException( e ) );
		LogMessage( "Distribution Failed!" );
		
		pWindowMain.GetLogger()->LogException( LOGSOURCE, e );
	}
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
