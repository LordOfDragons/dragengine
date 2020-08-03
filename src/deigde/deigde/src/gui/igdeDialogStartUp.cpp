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

#include <stdlib.h>

#include "igdeDialogStartUp.h"
#include "igdeWindowMain.h"
#include "igdeDialogNewGameProject.h"
#include "../environment/igdeEnvironmentIGDE.h"
#include "../configuration/igdeConfiguration.h"
#include "../loadsave/igdeLoadSaveSystem.h"
#include "../module/igdeEditorModuleManager.h"

#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeLabelReference.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/dialog/igdeDialogReference.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionSelectFile.h>
#include <deigde/gui/event/igdeListBoxListener.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>



// Actions
////////////

class igdeDialogStartUp_ListRecentProjects : public igdeListBoxListener{
	igdeDialogStartUp &pDialog;
public:
	igdeDialogStartUp_ListRecentProjects( igdeDialogStartUp &dialog ) : pDialog( dialog ){}
	
	virtual void OnDoubleClickItem( igdeListBox*, int ){
		pDialog.LoadSelectedRecentProject();
	}
};


class igdeDialogStartUp_ActionLoadRecent : public igdeAction{
	igdeDialogStartUp &pDialog;
public:
	igdeDialogStartUp_ActionLoadRecent( igdeDialogStartUp &dialog ) :
	igdeAction( "Load Recent", dialog.GetEnvironment().GetStockIcon( igdeEnvironment::esiOpen ),
		"Load selected recent game project" ), pDialog( dialog ){}
	
	virtual void OnAction(){
		pDialog.LoadSelectedRecentProject();
	}
	
	virtual void Update(){
		SetEnabled( pDialog.GetWindowMain().GetConfiguration().GetRecentProjectList().GetCount() > 0 );
		SetDefault( GetEnabled() );
	}
};

class igdeDialogStartUp_ActionLoadFile : public igdeAction{
	igdeDialogStartUp &pDialog;
public:
	igdeDialogStartUp_ActionLoadFile( igdeDialogStartUp &dialog ) :
	igdeAction( "Load Project", dialog.GetEnvironment().GetStockIcon( igdeEnvironment::esiOpen ),
		"Load game project from file" ), pDialog( dialog ){}
	
	virtual void OnAction(){
		pDialog.LoadProjectFromFile();
	}
};

class igdeDialogStartUp_ActionNewProject : public igdeAction{
	igdeDialogStartUp &pDialog;
public:
	igdeDialogStartUp_ActionNewProject( igdeDialogStartUp &dialog ) :
	igdeAction( "New Project", dialog.GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ),
		"Create new game project" ), pDialog( dialog ){}
	
	virtual void OnAction(){
		pDialog.NewGameProject();
	}
	
	virtual void Update(){
		SetDefault( pDialog.GetWindowMain().GetConfiguration().GetRecentProjectList().GetCount() == 0 );
	}
};

class igdeDialogStartUp_ActionQuit : public igdeAction{
	igdeDialogStartUp &pDialog;
public:
	igdeDialogStartUp_ActionQuit( igdeDialogStartUp &dialog ) :
	igdeAction( "Quit IGDE", dialog.GetEnvironment().GetStockIcon( igdeEnvironment::esiQuit ),
		"Quit the IGDE application" ), pDialog( dialog ){}
	
	virtual void OnAction(){
		pDialog.Cancel();
	}
};



// Class igdeDialogStartUp
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDialogStartUp::igdeDialogStartUp( igdeWindowMain &windowMain ) :
igdeDialog( windowMain.GetEnvironment(), "IGDE Start-Up", windowMain.GetIconApplication() ),
pWindowMain( windowMain )
{
	igdeEnvironment &env = windowMain.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	
	SetInitialSize( decPoint( 600, 0 ) );
	
	igdeLabelReference label;
	label.TakeOver( new igdeLabel( env, "Recently used Game Projects" ) );
	helper.ListBox( 10, "Recently loaded game projects", pListRecentProjects,
		new igdeDialogStartUp_ListRecentProjects( *this ) );
	
	
	pActionLoadRecent.TakeOver( new igdeDialogStartUp_ActionLoadRecent( *this ) );
	pActionLoadFile.TakeOver( new igdeDialogStartUp_ActionLoadFile( *this ) );
	pActionNewProject.TakeOver( new igdeDialogStartUp_ActionNewProject( *this ) );
	pActionQuit.TakeOver( new igdeDialogStartUp_ActionQuit( *this ) );
	
	igdeAction *actions[ 4 ] = { pActionLoadRecent, pActionLoadFile, pActionNewProject, pActionQuit };
	igdeContainerReference buttonBar;
	CreateButtonBar( buttonBar, actions, 4 );
	
	
	pUpdateRecentProjectList();
	
	AddContent( pListRecentProjects, label, buttonBar );
}

igdeDialogStartUp::~igdeDialogStartUp(){
}



// Management
///////////////

void igdeDialogStartUp::LoadSelectedRecentProject(){
	const int selection = pListRecentProjects->GetSelection();
	if( selection == -1 ){
		return;
	}
	
	if( pWindowMain.LoadGameProject( pWindowMain.GetConfiguration().GetRecentProjectList().GetAt( selection ) ) ){
		Accept();
	}
}

void igdeDialogStartUp::LoadProjectFromFile(){
	decPath path;
	path.SetFromNative( pWindowMain.GetConfiguration().GetPathProjects() );
	path.AddComponent( "project.degp" );
	
	decString filename( path.GetPathNative() );
	
	/*
	const decStringList &recentProjectList = pWindowMain.GetConfiguration().GetRecentProjectList();
	if( recentProjectList.GetCount() == 0 ){
		filename = recentProjectList.GetAt( 0 );
	}
	*/
	
	if( ! igdeCommonDialogs::GetFileOpen( this, "Open Game Project",
	pWindowMain.GetLoadSaveSystem()->GetOpenFilePatternList( igdeLoadSaveSystem::efplGameProject ), filename ) ){
		return;
	}
	
	if( pWindowMain.LoadGameProject( filename ) ){
		Accept();
	}
}

void igdeDialogStartUp::NewGameProject(){
	igdeDialogReference dialog;
	dialog.TakeOver( new igdeDialogNewGameProject( pWindowMain ) );
	if( ! dialog->Run( this ) ){
		return;
	}
	
	pWindowMain.SetGameProject( ( ( igdeDialogNewGameProject& )( igdeDialog& )dialog ).GetNewProject() );
	pWindowMain.GetModuleManager().ActivateProjectManager();
	Accept();
}



// Private Functions
//////////////////////

void igdeDialogStartUp::pUpdateRecentProjectList(){
	const decStringList &list = pWindowMain.GetConfiguration().GetRecentProjectList();
	const int count = list.GetCount();
	decString text;
	int i;
	
	pListRecentProjects->RemoveAllItems();
	
	for( i=0; i<count; i++ ){
		const decString &filename = list.GetAt( i );
		decPath path( decPath::CreatePathNative( filename ) );
		
		const decString filetitle( path.GetLastComponent() );
		
		path.RemoveLastComponent();
		
		decString filedir( path.GetPathNative() );
		if( filedir.GetLength() > 40 ){
			filedir = decString( "..." ) + filedir.GetRight( 40 );
		}
		
		text.Format( "%s [%s]", filetitle.GetString(), filedir.GetString() );
		
		pListRecentProjects->AddItem( text );
	}
	
	if( pListRecentProjects->GetItemCount() > 0 ){
		pListRecentProjects->SetSelection( 0 );
	}
	
	pActionLoadRecent->Update();
	pActionLoadFile->Update();
	pActionNewProject->Update();
	pActionQuit->Update();
	
	DefaultActionChanged();
}
