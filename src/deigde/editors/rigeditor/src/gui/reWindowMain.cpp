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

#include <stdio.h>
#include <stdlib.h>

#include "reWindowMain.h"
#include "reWindowMainListener.h"
#include "reView3D.h"
#include "properties/reWindowProperties.h"
#include "reDialogImportBone.h"
#include "reTaskSyncGameDefinition.h"
#include "../rigeditor.h"
#include "../configuration/reConfiguration.h"
#include "../reIGDEModule.h"
#include "../clipboard/reClipboard.h"
#include "../loadsave/reLoadSaveSystem.h"
#include "../rig/reRig.h"
#include "../rig/bone/reRigBone.h"
#include "../rig/bone/reSelectionBones.h"
#include "../rig/shape/reRigShapeBox.h"
#include "../rig/shape/reRigShapeSphere.h"
#include "../rig/shape/reRigShapeCapsule.h"
#include "../rig/shape/reRigShapeCylinder.h"
#include "../rig/shape/reRigShapeList.h"
#include "../rig/shape/reSelectionShapes.h"
#include "../rig/push/reRigPush.h"
#include "../rig/push/reRigPushList.h"
#include "../rig/push/reSelectionPushes.h"
#include "../rig/constraint/reRigConstraint.h"
#include "../rig/constraint/reRigConstraintList.h"
#include "../rig/constraint/reSelectionConstraints.h"
#include "../undosys/gui/bone/reUBoneMirror.h"
#include "../undosys/gui/bone/reUBoneScaleMass.h"
#include "../undosys/gui/bone/reUBoneImportFromFile.h"
#include "../undosys/gui/bone/reUBoneMassFromVolume.h"
#include "../undosys/gui/shape/reUAddShape.h"
#include "../undosys/gui/shape/reURemoveShape.h"
#include "../undosys/gui/push/reUAddPush.h"
#include "../undosys/gui/push/reURemovePush.h"
#include "../undosys/gui/constraint/reUAddConstraint.h"
#include "../undosys/gui/constraint/reURemoveConstraint.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeToolBar.h>
#include <deigde/gui/igdeToolBarDock.h>
#include <deigde/gui/igdeToolBarSeparator.h>
#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/dialog/igdeDialogReference.h>
#include <deigde/gui/layout/igdeContainerSplitted.h>
#include <deigde/gui/layout/igdeContainerSplittedReference.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>
#include <deigde/gui/menu/igdeMenuCommand.h>
#include <deigde/gui/menu/igdeMenuSeparator.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionRedo.h>
#include <deigde/gui/event/igdeActionUndo.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/resources/igdeIcon.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/deEngine.h>
#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/rig/deRig.h>



// Class reWindowMain
///////////////////////

// Constructor, destructor
////////////////////////////

reWindowMain::reWindowMain( reIGDEModule &module ) :
igdeEditorWindow( module ),

pListener( NULL ),

pConfiguration( NULL ),
pClipboard( NULL ),
pLoadSaveSystem( NULL ),

pView3D( NULL ),
pWindowProperties( NULL ),

pRig( NULL )
{
	igdeEnvironment &env = GetEnvironment();
	
	pLoadIcons();
	pCreateActions();
	pCreateMenu();
	
	pListener = new reWindowMainListener( *this );
	pLoadSaveSystem = new reLoadSaveSystem( *this );
	pConfiguration = new reConfiguration( *this );
	pClipboard = new reClipboard;
	
	pConfiguration->LoadConfiguration();
	
	// content
	pCreateToolBarFile();
	pCreateToolBarEdit();
	
	igdeContainerSplittedReference splitted;
	splitted.TakeOver(new igdeContainerSplitted(env, igdeContainerSplitted::espLeft,
		igdeApplication::app().DisplayScaled(300)));
	AddChild( splitted );
	
	pWindowProperties = new reWindowProperties( *this );
	splitted->AddChild( pWindowProperties, igdeContainerSplitted::eaSide );
	
	pView3D = new reView3D( *this );
	splitted->AddChild( pView3D, igdeContainerSplitted::eaCenter );
	
	CreateNewRig();
	ResetViews();
}

reWindowMain::~reWindowMain(){
	if( pConfiguration ){
		pConfiguration->SaveConfiguration();
	}
	if( pClipboard ){
		delete pClipboard;
	}
	
	SetRig( NULL );
	
	if( pWindowProperties ){
		pWindowProperties->FreeReference();
		pWindowProperties = NULL;
	}
	if( pView3D ){
		pView3D->FreeReference();
		pView3D = NULL;
	}
	
	if( pConfiguration ){
		delete pConfiguration;
	}
	if( pLoadSaveSystem ){
		delete pLoadSaveSystem;
	}
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

bool reWindowMain::QuitRequest(){
	return true;
}

void reWindowMain::ResetViews(){
	pView3D->ResetView();
}



void reWindowMain::SetRig( reRig *rig ){
	if( rig == pRig ){
		return;
	}
	
	pWindowProperties->SetRig( NULL );
	pView3D->SetRig( NULL );
	pActionEditUndo->SetUndoSystem( NULL );
	pActionEditRedo->SetUndoSystem( NULL );
	
	if( pRig ){
		pRig->RemoveNotifier( pListener );
		pRig->Dispose();
		pRig->FreeReference();
	}
	
	pRig = rig;
	
	if( rig ){
		rig->AddReference();
		rig->AddNotifier( pListener );
		
		pActionEditUndo->SetUndoSystem( rig->GetUndoSystem() );
		pActionEditRedo->SetUndoSystem( rig->GetUndoSystem() );
	}
	
	pView3D->SetRig( rig );
	pWindowProperties->SetRig( rig );
	
	if( rig && GetEngineController().GetRunning() ){
		rig->InitDelegates();
	}
}

void reWindowMain::CreateNewRig(){
	deObjectReference refRig;
	refRig.TakeOver( new reRig( &GetEnvironment() ) );
	reRig * const rig = ( reRig* )refRig.operator->();
	
	SetRig( rig );
}

void reWindowMain::SaveRig( const char *filename ){
	pLoadSaveSystem->SaveRig( pRig, filename );
	pRig->SetFilePath( filename );
	pRig->SetChanged( false );
	pRig->SetSaved( true );
	GetRecentFiles().AddFile( filename );
}



void reWindowMain::OnBeforeEngineStart(){
}

void reWindowMain::OnAfterEngineStart(){
	pView3D->OnAfterEngineStart();
	pLoadSaveSystem->UpdateLSRigs();
	
	if( pRig ){
		pRig->InitDelegates();
	}
}

void reWindowMain::OnBeforeEngineStop(){
	pView3D->OnBeforeEngineStop();
}

void reWindowMain::OnAfterEngineStop(){
	pLoadSaveSystem->UpdateLSRigs();
}

void reWindowMain::OnActivate(){
	igdeEditorWindow::OnActivate();
	pView3D->SetEnableRendering( true );
}

void reWindowMain::OnDeactivate(){
	pView3D->SetEnableRendering( false );
	igdeEditorWindow::OnDeactivate();
}

void reWindowMain::OnFrameUpdate( float elapsed ){
	if( ! GetActiveModule() ){
		return;
	}
	
	pView3D->OnFrameUpdate( elapsed );
}



void reWindowMain::GetChangedDocuments( decStringList &list ){
	if( pRig && pRig->GetChanged() ){
		list.Add( pRig->GetFilePath() );
	}
}

void reWindowMain::LoadDocument( const char *filename ){
	if( pRig && pRig->GetChanged() ){
		if( igdeCommonDialogs::Question( this, igdeCommonDialogs::ebsYesNo, "Open Rig",
		"Open rig discards changes. Is this ok?" ) == igdeCommonDialogs::ebNo ){
			return;
		}
	}
	
	deObjectReference rig;
	rig.TakeOver( pLoadSaveSystem->LoadRig( filename ) );
	SetRig( ( reRig* )( deObject* )rig );
	GetRecentFiles().AddFile( filename );
}

bool reWindowMain::SaveDocument( const char *filename ){
	if( pRig && pRig->GetFilePath().Equals( filename ) ){
		pActionFileSave->OnAction();
		return true; // TODO better implement this so failure can be detected
	}
	
	return false;
}

void reWindowMain::RecentFilesChanged(){
	pConfiguration->SaveConfiguration();
}

void reWindowMain::OnGameProjectChanged(){
	pConfiguration->LoadConfiguration();
	CreateNewRig();
}

igdeStepableTask *reWindowMain::OnGameDefinitionChanged(){
	return new reTaskSyncGameDefinition( *this );
}



// Actions
////////////

namespace{

class cActionBase : public igdeAction{
protected:
	reWindowMain &pWindow;
	
public:
	cActionBase( reWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		int modifiers = deInputEvent::esmNone, deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined ) :
	igdeAction( text, icon, description, mnemonic, igdeHotKey( modifiers, keyCode ) ),
	pWindow( window ){}
	
	cActionBase( reWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic ) :
	igdeAction( text, icon, description, mnemonic ),
	pWindow( window ){}
	
	virtual void OnAction(){
		igdeUndoReference undo;
		undo.TakeOver( OnAction( pWindow.GetRig() ) );
		if( undo ){
			pWindow.GetRig()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnAction( reRig *rig ) = 0;
	
	virtual void Update(){
		if( pWindow.GetRig() ){
			Update( *pWindow.GetRig() );
			
		}else{
			SetEnabled( false );
			SetSelected( false );
		}
	}
	
	virtual void Update( const reRig & ){
		SetEnabled( true );
		SetSelected( false );
	}
};

class cActionBaseBone : public cActionBase{
public:
	cActionBaseBone( reWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		int modifiers = deInputEvent::esmNone, deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined ) :
	cActionBase( window, text, icon, description, modifiers, keyCode, mnemonic ){}
	
	cActionBaseBone( reWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic ) :
	cActionBase( window, text, icon, description, mnemonic ){}
	
	virtual igdeUndo *OnAction( reRig *rig ){
		reRigBone * const bone = rig->GetSelectionBones()->GetActiveBone();
		if( ! bone ){
			return NULL;
		}
		return OnActionBone( rig, bone );
	}
	
	virtual igdeUndo *OnActionBone( reRig *rig, reRigBone *bone ) = 0;
	
	virtual void Update(){
		SetEnabled( pWindow.GetRig()->GetSelectionBones()->GetActiveBone() != NULL );
	}
};



class cActionFileNew : public igdeAction{
	reWindowMain &pWindow;
public:
	cActionFileNew( reWindowMain &window ) : igdeAction( "New",
		window.GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ), "Creates a new rig",
		deInputEvent::ekcN, igdeHotKey( deInputEvent::esmControl, deInputEvent::ekcN ) ), pWindow( window ){}
	
	virtual void OnAction(){
		if( ! pWindow.GetRig() || ! pWindow.GetRig()->GetChanged()
		|| igdeCommonDialogs::Question( &pWindow, igdeCommonDialogs::ebsYesNo, "New Rig",
		"Creating a new rig discarding the current one is that ok?" ) == igdeCommonDialogs::ebYes ){
			pWindow.CreateNewRig();
		}
	}
};

class cActionFileOpen : public igdeAction{
	reWindowMain &pWindow;
public:
	cActionFileOpen( reWindowMain &window ) : igdeAction( "Open...",
		window.GetEnvironment().GetStockIcon( igdeEnvironment::esiOpen ), "Opens a rig from file",
		deInputEvent::ekcO, igdeHotKey( deInputEvent::esmControl, deInputEvent::ekcO ) ), pWindow( window ){}
	
	virtual void OnAction(){
		if( pWindow.GetRig() && pWindow.GetRig()->GetChanged() ){
			if( igdeCommonDialogs::Question( &pWindow, igdeCommonDialogs::ebsYesNo, "Open Rig",
			"Open rig discards changes. Is this ok?" ) == igdeCommonDialogs::ebNo ){
				return;
			}
		}
		
		decString filename( pWindow.GetRig() ? pWindow.GetRig()->GetFilePath()
			: pWindow.GetGameProject()->GetPathData() );
		if( ! igdeCommonDialogs::GetFileOpen( &pWindow, "Open Rig",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetEnvironment().GetOpenFilePatternList( igdeEnvironment::efpltRig ), filename ) ){
			return;
		}
		
		deObjectReference rig;
		rig.TakeOver( pWindow.GetLoadSaveSystem().LoadRig( filename ) );
		pWindow.SetRig( ( reRig* )( deObject* )rig );
		pWindow.GetRecentFiles().AddFile( filename );
	}
};

class cActionFileSaveAs : public cActionBase{
public:
	cActionFileSaveAs( reWindowMain &window ) : cActionBase( window,
		"Save As...", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiSave ),
		"Saves rig under a differen file", deInputEvent::ekcA ){}
	
	virtual igdeUndo *OnAction( reRig *rig ){
		decString filename( rig->GetFilePath() );
		if( igdeCommonDialogs::GetFileSave( &pWindow, "Save Rig",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetEnvironment().GetSaveFilePatternList( igdeEnvironment::efpltRig ), filename ) ){
			pWindow.SaveRig( filename );
		}
		return NULL;
	}
};

class cActionFileSave : public cActionFileSaveAs{
public:
	cActionFileSave( reWindowMain &window ) : cActionFileSaveAs( window ){
		SetText( "Save" );
		SetIcon( window.GetEnvironment().GetStockIcon( igdeEnvironment::esiSaveAs ) );
		SetDescription( "Saves rig to file" );
		SetHotKey( igdeHotKey( deInputEvent::esmControl, deInputEvent::ekcS ) );
		SetMnemonic( deInputEvent::ekcS );
	}
	
	virtual igdeUndo *OnAction( reRig *rig ){
		if( rig->GetSaved() ){
			if( rig->GetChanged() ){
				pWindow.SaveRig( rig->GetFilePath() );
			}
			
		}else{
			cActionFileSaveAs::OnAction( rig );
		}
		return NULL;
	}
	
	virtual void Update( const reRig &rig ){
		SetEnabled( rig.GetChanged() );
	}
};


class cActionEditCut : public cActionBase{
public:
	cActionEditCut( reWindowMain &window ) : cActionBase( window,
		"Cut", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiCut ),
		"Cut selected objects", deInputEvent::esmControl,
		deInputEvent::ekcX, deInputEvent::ekcT ){}
	
	virtual igdeUndo *OnAction( reRig* ){
		return NULL;
	}
	
	virtual void Update( const reRig & ){
		SetEnabled( false );
	}
};

class cActionEditCopy : public cActionBase{
public:
	cActionEditCopy( reWindowMain &window ) : cActionBase( window,
		"Copy", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ),
		"Copies selected objects", deInputEvent::esmControl,
		deInputEvent::ekcC, deInputEvent::ekcC ){}
	
	virtual igdeUndo *OnAction( reRig* ){
		return NULL;
	}
	
	virtual void Update( const reRig & ){
		SetEnabled( false );
	}
};

class cActionEditPaste : public cActionBase{
public:
	cActionEditPaste( reWindowMain &window ) : cActionBase( window,
		"Paste", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ),
		"Paste objects", deInputEvent::esmControl,
		deInputEvent::ekcV, deInputEvent::ekcP ){}
	
	virtual igdeUndo *OnAction( reRig* ){
		return NULL;
	}
	
	virtual void Update( const reRig & ){
		SetEnabled( pWindow.GetClipboard().HasClip() );
	}
};



class cActionEditElementMode : public cActionBase{
protected:
	const reRig::eElementModes pMode;
	
public:
	cActionEditElementMode( reWindowMain &window, reRig::eElementModes mode, const char *text,
		igdeIcon *icon, const char *description, int modifiers,
		deInputEvent::eKeyCodes keyCode, deInputEvent::eKeyCodes mnemonic ) :
	cActionBase( window, text, icon, description, modifiers, keyCode, mnemonic ),
	pMode( mode ){}
	
	virtual igdeUndo *OnAction( reRig *rig ){
		rig->SetElementMode( pMode );
		return NULL;
	}
	
	virtual void Update( const reRig &rig ){
		SetSelected( rig.GetElementMode() == pMode );
	}
};

class cActionEditWorkMode : public cActionBase{
protected:
	const reRig::eWorkModes pMode;
	
public:
	cActionEditWorkMode( reWindowMain &window, reRig::eWorkModes mode, const char *text,
		igdeIcon *icon, const char *description, int modifiers,
		deInputEvent::eKeyCodes keyCode, deInputEvent::eKeyCodes mnemonic ) :
	cActionBase( window, text, icon, description, modifiers, keyCode, mnemonic ),
	pMode( mode ){}
	
	virtual igdeUndo *OnAction( reRig *rig ){
		rig->SetWorkMode( pMode );
		return NULL;
	}
	
	virtual void Update( const reRig &rig ){
		SetSelected( rig.GetWorkMode() == pMode );
	}
};

class cActionEditLockAxisX : public cActionBase{
public:
	cActionEditLockAxisX( reWindowMain &window ) : cActionBase( window,
		"Lock X-Axis", window.GetIconEditLockAxisX(), "Lock X coordinates during editing",
		deInputEvent::esmControl | deInputEvent::esmShift, deInputEvent::ekcX ){}
	
	virtual igdeUndo *OnAction( reRig *rig ){
		rig->SetLockAxisX( ! rig->GetLockAxisX() );
		return NULL;
	}
	
	virtual void Update( const reRig &rig ){
		SetSelected( rig.GetLockAxisX() );
	}
};

class cActionEditLockAxisY : public cActionBase{
public:
	cActionEditLockAxisY( reWindowMain &window ) : cActionBase( window,
		"Lock Y-Axis", window.GetIconEditLockAxisY(), "Lock Y coordinates during editing",
		deInputEvent::esmControl | deInputEvent::esmShift, deInputEvent::ekcY ){}
	
	virtual igdeUndo *OnAction( reRig *rig ){
		rig->SetLockAxisY( ! rig->GetLockAxisY() );
		return NULL;
	}
	
	virtual void Update( const reRig &rig ){
		SetSelected( rig.GetLockAxisY() );
	}
};

class cActionEditLockAxisZ : public cActionBase{
public:
	cActionEditLockAxisZ( reWindowMain &window ) : cActionBase( window,
		"Lock Z-Axis", window.GetIconEditLockAxisZ(), "Lock Z coordinates during editing",
		deInputEvent::esmControl | deInputEvent::esmShift, deInputEvent::ekcZ ){}
	
	virtual igdeUndo *OnAction( reRig *rig ){
		rig->SetLockAxisZ( ! rig->GetLockAxisZ() );
		return NULL;
	}
	
	virtual void Update( const reRig &rig ){
		SetSelected( rig.GetLockAxisZ() );
	}
};

class cActionEditLockLocal : public cActionBase{
public:
	cActionEditLockLocal( reWindowMain &window ) : cActionBase( window,
		"Use local coordinates", window.GetIconEditLockLocal(),
		"Uses local coordinates for editing instead of world coordinates",
		deInputEvent::esmControl | deInputEvent::esmShift, deInputEvent::ekcL ){}
	
	virtual igdeUndo *OnAction( reRig *rig ){
		rig->SetUseLocal( ! rig->GetUseLocal() );
		return NULL;
	}
	
	virtual void Update( const reRig &rig ){
		SetSelected( rig.GetUseLocal() );
	}
};

class cActionEditSelectAll : public cActionBase{
public:
	cActionEditSelectAll( reWindowMain &window ) : cActionBase( window,
		"Select All", NULL, "Selects all elements",
		deInputEvent::esmControl, deInputEvent::ekcA, deInputEvent::ekcA ){}
	
	virtual igdeUndo *OnAction( reRig *rig ){
		switch( rig->GetElementMode() ){
		case reRig::eemBone:
			SelectAllBones( *pWindow.GetRig() );
			break;
			
		case reRig::eemShape:
			SelectAllShapes( *pWindow.GetRig() );
			break;
			
		case reRig::eemConstraint:
			SelectAllConstraints( *pWindow.GetRig() );
			break;
			
		case reRig::eemPush:
			SelectAllPushes( *pWindow.GetRig() );
			break;
		}
		return NULL;
	}
	
	void SelectAllBones( reRig &rig ){
		reSelectionBones &selection = *rig.GetSelectionBones();
		const int count = rig.GetBoneCount();
		int i;
		
		for( i=0; i<count; i++ ){
			reRigBone * const bone = rig.GetBoneAt( i );
			if( ! bone->GetSelected() ){
				selection.AddBone( bone );
			}
		}
	}
	
	void SelectAllShapes( reRig &rig ){
		reSelectionShapes &selection = *rig.GetSelectionShapes();
		const int boneCount = rig.GetBoneCount();
		int i, j;
		
		for( i=0; i<boneCount; i++ ){
			const reRigBone &bone = *rig.GetBoneAt( i );
			const int shapeCount = bone.GetShapeCount();
			
			for( j=0; j<shapeCount; j++ ){
				reRigShape * const shape = bone.GetShapeAt( j );
				if( ! shape->GetSelected() && shape->IsVisible() ){
					selection.AddShape( shape );
				}
			}
		}
		
		const int shapeCount = rig.GetShapeCount();
		for( i=0; i<shapeCount; i++ ){
			reRigShape * const shape = rig.GetShapeAt( i );
			if( ! shape->GetSelected() && shape->IsVisible() ){
				selection.AddShape( shape );
			}
		}
	}
	
	void SelectAllConstraints( reRig &rig ){
		reSelectionConstraints &selection = *rig.GetSelectionConstraints();
		const int boneCount = rig.GetBoneCount();
		int i;
		
		for( i=0; i<boneCount; i++ ){
			const reRigBone &bone = *rig.GetBoneAt( i );
			const int constraintCount = bone.GetConstraintCount();
			int j;
			
			for( j=0; j<constraintCount; j++ ){
				reRigConstraint * const constraint = bone.GetConstraintAt( j );
				if( ! constraint->GetSelected() && constraint->IsVisible() ){
					selection.AddConstraint( constraint );
				}
			}
		}
		
		const int constraintCount = rig.GetConstraintCount();
		for( i=0; i<constraintCount; i++ ){
			reRigConstraint * const constraint = rig.GetConstraintAt( i );
			if( ! constraint->GetSelected() && constraint->IsVisible() ){
				selection.AddConstraint( constraint );
			}
		}
	}
	
	void SelectAllPushes( reRig &rig ){
		reSelectionPushes &selection = *rig.GetSelectionPushes();
		const int count = rig.GetPushCount();
		int i;
		
		for( i=0; i<count; i++ ){
			reRigPush * const push = rig.GetPushAt( i );
			if( ! push->GetSelected() && push->IsVisible() ){
				selection.AddPush( push );
			}
		}
	}
};

class cActionEditSelectAllWithShapes : public cActionBase{
public:
	cActionEditSelectAllWithShapes( reWindowMain &window ) : cActionBase( window,
		"Select All With Shapes", NULL, "Selects all bones with shapes",
		deInputEvent::esmNone, deInputEvent::ekcUndefined, deInputEvent::ekcS ){}
	
	virtual igdeUndo *OnAction( reRig *rig ){
		if( rig->GetElementMode() != reRig::eemBone ){
			return NULL;
		}
		
		reSelectionBones &selection = *rig->GetSelectionBones();
		const int count = rig->GetBoneCount();
		int i;
		
		for( i=0; i<count; i++ ){
			reRigBone * const bone = rig->GetBoneAt( i );
			if( ! bone->GetSelected() && bone->GetShapeCount() > 0 ){
				selection.AddBone( bone );
			}
		}
		return NULL;
	}
	
	virtual void Update( const reRig &rig ){
		SetEnabled( rig.GetElementMode() == reRig::eemBone );
	}
};

class cActionEditSelectNone : public cActionBase{
public:
	cActionEditSelectNone( reWindowMain &window ) : cActionBase( window,
		"Select None", NULL, "Unselects all elements",
		deInputEvent::esmControl | deInputEvent::esmShift, deInputEvent::ekcA, deInputEvent::ekcN ){}
	
	virtual igdeUndo *OnAction( reRig *rig ){
		switch( rig->GetElementMode() ){
		case reRig::eemBone:
			rig->GetSelectionBones()->RemoveAllBones();
			break;
			
		case reRig::eemShape:
			rig->GetSelectionShapes()->RemoveAllShapes();
			break;
			
		case reRig::eemConstraint:
			rig->GetSelectionConstraints()->RemoveAllConstraints();
			break;
			
		case reRig::eemPush:
			rig->GetSelectionPushes()->RemoveAllPushes();
			break;
		}
		return NULL;
	}
};

class cActionEditDelete : public cActionBase{
public:
	cActionEditDelete( reWindowMain &window ) : cActionBase( window,
		"Delete Selection", NULL, "Deletes the selected objects",
		deInputEvent::esmNone, deInputEvent::ekcDelete, deInputEvent::ekcD ){}
	
	virtual igdeUndo *OnAction( reRig *rig ){
		switch( rig->GetElementMode() ){
		case reRig::eemBone:
			return DeleteBones( rig );
			
		case reRig::eemShape:
			return DeleteShapes( rig );
			
		case reRig::eemConstraint:
			return DeleteConstranits( rig );
			
		case reRig::eemPush:
			return DeletePushes( rig );
		}
		return NULL;
	}
	
	igdeUndo *DeleteBones( reRig *rig ){
		return NULL;
	}
	
	igdeUndo *DeleteShapes( reRig *rig ){
		reRigShapeList list;
		rig->GetSelectionShapes()->AddVisibleShapesTo( list );
		if( list.GetShapeCount() == 0 ){
			return NULL;
		}
		return new reURemoveShape( list );
	}
	
	igdeUndo *DeleteConstranits( reRig *rig ){
		reRigConstraintList list;
		rig->GetSelectionConstraints()->AddVisibleConstraintsTo( list );
		if( list.GetConstraintCount() == 0 ){
			return NULL;
		}
		return new reURemoveConstraint( list );
	}
	
	igdeUndo *DeletePushes( reRig *rig ){
		reRigPushList list;
		rig->GetSelectionPushes()->AddVisiblePushesTo( list );
		if( list.GetPushCount() == 0 ){
			return NULL;
		}
		return new reURemovePush( list );
	}
	
	virtual void Update( const reRig &rig ){
		bool enabled = false;
		switch( rig.GetElementMode() ){
		case reRig::eemBone:
			break;
			
		case reRig::eemShape:{
			reRigShapeList list;
			rig.GetSelectionShapes()->AddVisibleShapesTo( list );
			enabled = list.GetShapeCount() > 0;
			}break;
			
		case reRig::eemConstraint:{
			reRigConstraintList list;
			rig.GetSelectionConstraints()->AddVisibleConstraintsTo( list );
			enabled = list.GetConstraintCount() > 0;
			}break;
			
		case reRig::eemPush:{
			reRigPushList list;
			rig.GetSelectionPushes()->AddVisiblePushesTo( list );
			enabled = list.GetPushCount() > 0;
			}break;
		}
		SetEnabled( enabled );
	}
};



class cActionRigAddShape : public cActionBase{
public:
	cActionRigAddShape( reWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic ) :
	cActionBase( window, text, icon, description, mnemonic ){}
	
	virtual igdeUndo *OnAction( reRig *rig ){
		deObjectReference shape;
		shape.TakeOver( CreateShape() );
		return new reUAddShape( rig, NULL, ( reRigShape* )shape.operator->() );
	}
	
	virtual reRigShape *CreateShape() = 0;
};

class cActionRigAddSphere : public cActionRigAddShape{
public:
	cActionRigAddSphere( reWindowMain &window ) : cActionRigAddShape( window,
		"Add Sphere Shape", NULL, "Adds a sphere shape", deInputEvent::ekcS ){}
	
	reRigShape *CreateShape(){
		return new reRigShapeSphere( pWindow.GetEngineController().GetEngine() );
	}
};

class cActionRigAddBox : public cActionRigAddShape{
public:
	cActionRigAddBox( reWindowMain &window ) : cActionRigAddShape( window,
		"Add Box Shape", NULL, "Adds a box shape", deInputEvent::ekcB ){}
	
	reRigShape *CreateShape(){
		return new reRigShapeBox( pWindow.GetEngineController().GetEngine() );
	}
};

class cActionRigAddCylinder : public cActionRigAddShape{
public:
	cActionRigAddCylinder( reWindowMain &window ) : cActionRigAddShape( window,
		"Add Cylinder Shape", NULL, "Adds a cylinder shape", deInputEvent::ekcC ){}
	
	reRigShape *CreateShape(){
		return new reRigShapeCylinder( pWindow.GetEngineController().GetEngine() );
	}
};

class cActionRigAddCapsule : public cActionRigAddShape{
public:
	cActionRigAddCapsule( reWindowMain &window ) : cActionRigAddShape( window,
		"Add Capsule Shape", NULL, "Adds a capsule shape", deInputEvent::ekcA ){}
	
	reRigShape *CreateShape(){
		return new reRigShapeCapsule( pWindow.GetEngineController().GetEngine() );
	}
};

class cActionRigAddConstraint : public cActionBase{
public:
	cActionRigAddConstraint( reWindowMain &window ) : cActionBase( window,
		"Add Constraint", NULL, "Add a constraint", deInputEvent::ekcC ){}
	
	virtual igdeUndo *OnAction( reRig *rig ){
		deObjectReference constraint;
		constraint.TakeOver( new reRigConstraint( pWindow.GetEngineController().GetEngine() ) );
		return new reUAddConstraint( rig, NULL, ( reRigConstraint* )constraint.operator->() );
	}
};

class cActionRigAddPush : public cActionBase{
public:
	cActionRigAddPush( reWindowMain &window ) : cActionBase( window,
		"Add Push", NULL, "Add a push", deInputEvent::ekcP ){}
	
	virtual igdeUndo *OnAction( reRig *rig ){
		deObjectReference push;
		push.TakeOver( new reRigPush( pWindow.GetEngineController().GetEngine() ) );
		return new reUAddPush( rig, ( reRigPush* )push.operator->() );
	}
};

class cActionRigShowShapes : public cActionBase{
public:
	cActionRigShowShapes( reWindowMain &window ) : cActionBase( window,
		"Show Rig Shapes", NULL, "Show shapes of the entire rig" ){}
	
	virtual igdeUndo *OnAction( reRig *rig ){
		rig->SetShowRigShapes( ! rig->GetShowRigShapes() );
		return NULL;
	}
	
	virtual void Update( const reRig &rig ){
		SetSelected( rig.GetShowRigShapes() );
	}
};

class cActionRigShowConstraints : public cActionBase{
public:
	cActionRigShowConstraints( reWindowMain &window ) : cActionBase( window,
		"Show Rig Constraints", NULL, "Show constraints of the entire rig" ){}
	
	virtual igdeUndo *OnAction( reRig *rig ){
		rig->SetShowRigConstraints( ! rig->GetShowRigConstraints() );
		return NULL;
	}
	
	virtual void Update( const reRig &rig ){
		SetSelected( rig.GetShowRigConstraints() );
	}
};

class cActionRigShowPushes : public cActionBase{
public:
	cActionRigShowPushes( reWindowMain &window ) : cActionBase( window,
		"Show Rig Pushs", NULL, "Show pushes of the entire rig" ){}
	
	virtual igdeUndo *OnAction( reRig *rig ){
		rig->SetShowRigPushes( ! rig->GetShowRigPushes() );
		return NULL;
	}
	
	virtual void Update( const reRig &rig ){
		SetSelected( rig.GetShowRigPushes() );
	}
};



class cActionBoneAdd : public cActionBase{
public:
	cActionBoneAdd( reWindowMain &window ) : cActionBase( window,
		"Add", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ),
		"Add bone", deInputEvent::ekcA ){}
	
	virtual igdeUndo *OnAction( reRig *rig ){
		return NULL;
	}
};

class cActionBoneAddShape : public cActionBaseBone{
public:
	cActionBoneAddShape( reWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic ) :
	cActionBaseBone( window, text, icon, description, mnemonic ){}
	
	virtual igdeUndo *OnActionBone( reRig *rig, reRigBone *bone ){
		deObjectReference shape;
		shape.TakeOver( CreateShape() );
		return new reUAddShape( NULL, bone, ( reRigShape* )shape.operator->() );
	}
	
	virtual reRigShape *CreateShape() = 0;
};

class cActionBoneAddSphere : public cActionBoneAddShape{
public:
	cActionBoneAddSphere( reWindowMain &window ) : cActionBoneAddShape( window,
		"Add Sphere Shape", NULL, "Adds a sphere shape", deInputEvent::ekcS ){}
	
	reRigShape *CreateShape(){
		return new reRigShapeSphere( pWindow.GetEngineController().GetEngine() );
	}
};

class cActionBoneAddBox : public cActionBoneAddShape{
public:
	cActionBoneAddBox( reWindowMain &window ) : cActionBoneAddShape( window,
		"Add Box Shape", NULL, "Adds a box shape", deInputEvent::ekcB ){}
	
	reRigShape *CreateShape(){
		return new reRigShapeBox( pWindow.GetEngineController().GetEngine() );
	}
};

class cActionBoneAddCylinder : public cActionBoneAddShape{
public:
	cActionBoneAddCylinder( reWindowMain &window ) : cActionBoneAddShape( window,
		"Add Cylinder Shape", NULL, "Adds a cylinder shape", deInputEvent::ekcC ){}
	
	reRigShape *CreateShape(){
		return new reRigShapeCylinder( pWindow.GetEngineController().GetEngine() );
	}
};

class cActionBoneAddCapsule : public cActionBoneAddShape{
public:
	cActionBoneAddCapsule( reWindowMain &window ) : cActionBoneAddShape( window,
		"Add Capsule Shape", NULL, "Adds a capsule shape", deInputEvent::ekcA ){}
	
	reRigShape *CreateShape(){
		return new reRigShapeCapsule( pWindow.GetEngineController().GetEngine() );
	}
};

class cActionBoneAddConstraint : public cActionBaseBone{
public:
	cActionBoneAddConstraint( reWindowMain &window ) : cActionBaseBone( window,
		"Add Constraint", NULL, "Add a constraint", deInputEvent::ekcC ){}
	
	virtual igdeUndo *OnActionBone( reRig *rig, reRigBone *bone ){
		deObjectReference constraint;
		constraint.TakeOver( new reRigConstraint( pWindow.GetEngineController().GetEngine() ) );
		return new reUAddConstraint( NULL, bone, ( reRigConstraint* )constraint.operator->() );
	}
};

class cActionBoneShowBones : public cActionBase{
public:
	cActionBoneShowBones( reWindowMain &window ) : cActionBase( window,
		"Show Bones", NULL, "Show bones" ){}
	
	virtual igdeUndo *OnAction( reRig *rig ){
		rig->SetShowBones( ! rig->GetShowBones() );
		return NULL;
	}
	
	virtual void Update( const reRig &rig ){
		SetSelected( rig.GetShowBones() );
	}
};

class cActionBoneShowShapes : public cActionBase{
public:
	cActionBoneShowShapes( reWindowMain &window ) : cActionBase( window,
		"Show All Bone Shapes", NULL,
		"Show shapes of the bones not just the selected one" ){}
	
	virtual igdeUndo *OnAction( reRig *rig ){
		rig->SetShowAllBoneShapes( ! rig->GetShowAllBoneShapes() );
		return NULL;
	}
	
	virtual void Update( const reRig &rig ){
		SetSelected( rig.GetShowAllBoneShapes() );
	}
};

class cActionBoneShowConstraints : public cActionBase{
public:
	cActionBoneShowConstraints( reWindowMain &window ) : cActionBase( window,
		"Show All Bone Constraints", NULL,
		"Show constraints of the bones not just the selected one" ){}
	
	virtual igdeUndo *OnAction( reRig *rig ){
		rig->SetShowAllBoneConstraints( ! rig->GetShowAllBoneConstraints() );
		return NULL;
	}
	
	virtual void Update( const reRig &rig ){
		SetSelected( rig.GetShowAllBoneConstraints() );
	}
};

class cActionBoneMirror : public cActionBaseBone{
public:
	cActionBoneMirror( reWindowMain &window ) : cActionBaseBone( window,
		"Mirror", NULL, "Mirror the selected bones", deInputEvent::ekcM ){}
	
	virtual igdeUndo *OnActionBone( reRig *rig, reRigBone *bone ){
		return new reUBoneMirror( rig );
	}
};

class cActionBoneImport : public cActionBaseBone{
public:
	cActionBoneImport( reWindowMain &window ) : cActionBaseBone( window,
		"Import", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiOpen ),
		"Import the selected bones from file", deInputEvent::ekcI ){}
	
	virtual igdeUndo *OnActionBone( reRig *rig, reRigBone *bone ){
		igdeDialogReference refDialog;
		refDialog.TakeOver( new reDialogImportBone( pWindow ) );
		
		if( ! refDialog->Run( &pWindow ) ){
			return NULL;
		}
		
		const reDialogImportBone &dialog = ( reDialogImportBone& )( igdeDialog& )refDialog;
		deObjectReference refImportRig;
		refImportRig.TakeOver( pWindow.GetLoadSaveSystem().LoadRig( dialog.GetPath() ) );
		
		igdeUndoReference refUndo;
		refUndo.TakeOver( new reUBoneImportFromFile( rig, ( reRig* )refImportRig.operator->() ) );
		reUBoneImportFromFile &undo = ( reUBoneImportFromFile& )( igdeUndo& )refUndo;
		
		undo.SetScale( dialog.GetScaling() );
		undo.SetImportBoneProperties( dialog.GetImportBoneProperties() );
		undo.SetImportShapes( dialog.GetImportShapes() );
		undo.SetImportConstraints( dialog.GetImportConstraints() );
		
		refUndo->AddReference();
		return refUndo;
	}
};

class cActionBoneScaleMass : public cActionBaseBone{
public:
	cActionBoneScaleMass( reWindowMain &window ) : cActionBaseBone( window,
		"Scale Mass", NULL, "Scale total mass of the selected bones", deInputEvent::ekcS ){}
	
	virtual igdeUndo *OnActionBone( reRig *rig, reRigBone *bone ){
		const reSelectionBones &selection = *rig->GetSelectionBones();
		const int count = selection.GetBoneCount();
		decObjectOrderedSet list;
		float oldMass = 0.0f;
		int i;
		
		for( i=0; i<count; i++ ){
			reRigBone * const bone2 = selection.GetBoneAt( i );
			if( bone2->GetShapeCount() > 0 ){
				oldMass += bone2->GetMass();
				list.Add( bone2 );
			}
		}
		
		if( list.GetCount() == 0 ){
			igdeCommonDialogs::Error( &pWindow, "Scale Mass", "No bones selected with shapes" );
			return NULL;
		}
		
		float newMass = oldMass;
		if( ! igdeCommonDialogs::GetFloat( &pWindow, "Scale Mass", "New total mass:", newMass ) ){
			return NULL;
		}
		if( fabs( newMass - oldMass ) < FLOAT_SAFE_EPSILON ){
			return NULL;
		}
		
		return new reUBoneScaleMass( rig, list, newMass );
	}
};

class cActionBoneMassFromVolume : public cActionBaseBone{
public:
	cActionBoneMassFromVolume( reWindowMain &window ) : cActionBaseBone( window,
		"Mass From Volume", NULL, "Set the mass of the bone from the shape volumes",
		deInputEvent::ekcV ){}
	
	virtual igdeUndo *OnActionBone( reRig *rig, reRigBone *bone ){
		float density = 1.0f;
		if( ! igdeCommonDialogs::GetFloat( &pWindow, "Mass From Volume", "Density:", density ) ){
			return NULL;
		}
		
		const reSelectionBones &selection = *rig->GetSelectionBones();
		const int count = selection.GetBoneCount();
		decObjectOrderedSet list;
		int i;
		
		for( i=0; i<count; i++ ){
			list.Add( selection.GetBoneAt( i ) );
		}
		
		return new reUBoneMassFromVolume( rig, list, density );
	}
};



class cActionViewShapeXRay : public cActionBase{
public:
	cActionViewShapeXRay( reWindowMain &window ) : cActionBase( window,
		"X-Ray Shapes", NULL, "Sets if shapes are visible through geometry" ){}
	
	virtual igdeUndo *OnAction( reRig *rig ){
		rig->SetShapeXRay( ! rig->GetShapeXRay() );
		return NULL;
	}
	
	virtual void Update( const reRig &rig ){
		SetSelected( rig.GetShapeXRay() );
	}
};



class cActionSimRun: public cActionBase{
public:
	cActionSimRun( reWindowMain &window ) : cActionBase( window,
		"Run Simulation", NULL, "Starts and stops the simulation",
		deInputEvent::esmControl, deInputEvent::ekcQ, deInputEvent::ekcS ){}
	
	virtual igdeUndo *OnAction( reRig *rig ){
		rig->SetSimulationRunning( ! rig->GetSimulationRunning() );
		return NULL;
	}
	
	virtual void Update( const reRig &rig ){
		SetSelected( rig.GetSimulationRunning() );
	}
};

};



// Private Functions
//////////////////////

void reWindowMain::pLoadIcons(){
	pIconEditBone.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_bone.png" ) );
	pIconEditShape.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_shape.png" ) );
	pIconEditConstraint.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_constraint.png" ) );
	pIconEditPush.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_push.png" ) );
	pIconEditSelect.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_select.png" ) );
	pIconEditMove.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_move.png" ) );
	pIconEditScale.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_scale.png" ) );
	pIconEditRotate.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_rotate.png" ) );
// 	pIconEdit3DCursor.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_3d_cursor.png" ) );
	pIconEditLockAxisX.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_lock_axis_x.png" ) );
	pIconEditLockAxisY.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_lock_axis_y.png" ) );
	pIconEditLockAxisZ.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_lock_axis_z.png" ) );
	pIconEditLockLocal.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_use_local.png" ) );
}

void reWindowMain::pCreateActions(){
	pActionFileNew.TakeOver( new cActionFileNew( *this ) );
	pActionFileOpen.TakeOver( new cActionFileOpen( *this ) );
	pActionFileSave.TakeOver( new cActionFileSave( *this ) );
	pActionFileSaveAs.TakeOver( new cActionFileSaveAs( *this ) );
	
	pActionEditUndo.TakeOver( new igdeActionUndo( GetEnvironment() ) );
	pActionEditRedo.TakeOver( new igdeActionRedo( GetEnvironment() ) );
	pActionEditCut.TakeOver( new cActionEditCut( *this ) );
	pActionEditCopy.TakeOver( new cActionEditCopy( *this ) );
	pActionEditPaste.TakeOver( new cActionEditPaste( *this ) );
	
	pActionEditBoneMode.TakeOver( new cActionEditElementMode( *this, reRig::eemBone,
		"Bone Mode", pIconEditBone, "Bone mode",
		deInputEvent::esmControl, deInputEvent::ekc1, deInputEvent::ekcB ) );
	
	pActionEditShapeMode.TakeOver( new cActionEditElementMode( *this, reRig::eemShape,
		"Shape Mode", pIconEditShape, "Shape mode",
		deInputEvent::esmControl, deInputEvent::ekc2, deInputEvent::ekcS ) );
	
	pActionEditConstraintMode.TakeOver( new cActionEditElementMode( *this, reRig::eemConstraint,
		"Constraint Mode", pIconEditConstraint, "Constraint mode",
		deInputEvent::esmControl, deInputEvent::ekc3, deInputEvent::ekcC ) );
	
	pActionEditPushMode.TakeOver( new cActionEditElementMode( *this, reRig::eemPush,
		"Push Mode", pIconEditPush, "Push mode",
		deInputEvent::esmControl, deInputEvent::ekc4, deInputEvent::ekcP ) );
	
	pActionEditSelectMode.TakeOver( new cActionEditWorkMode( *this, reRig::ewmSelect,
		"Select Mode", pIconEditSelect, "Select mode",
		deInputEvent::esmNone, deInputEvent::ekc1, deInputEvent::ekcE ) );
	
	pActionEditMoveMode.TakeOver( new cActionEditWorkMode( *this, reRig::ewmMove,
		"Move Mode", pIconEditMove, "Move mode",
		deInputEvent::esmNone, deInputEvent::ekc2, deInputEvent::ekcM ) );
	
	pActionEditScaleMode.TakeOver( new cActionEditWorkMode( *this, reRig::ewmScale,
		"Scale Mode", pIconEditScale, "Scale mode",
		deInputEvent::esmNone, deInputEvent::ekc3, deInputEvent::ekcA ) );
	
	pActionEditRotateMode.TakeOver( new cActionEditWorkMode( *this, reRig::ewmRotate,
		"Rotate Mode", pIconEditRotate, "Rotate mode",
		deInputEvent::esmNone, deInputEvent::ekc4, deInputEvent::ekcR ) );
	
// 	pActionEdit3DCursorMode.TakeOver( new cActionEditWorkMode( *this, reRig::ewm3DCursor,
// 		"3D-Cursor Mode", pIconEdit3DCursor, "3D-Cursor mode",
// 		deInputEvent::esmNone, deInputEvent::ekc5, deInputEvent::ekcU ) );
	
	pActionEditSelectAll.TakeOver( new cActionEditSelectAll( *this ) );
	pActionEditSelectAllWithShapes.TakeOver( new cActionEditSelectAllWithShapes( *this ) );
	pActionEditSelectNone.TakeOver( new cActionEditSelectNone( *this ) );
	pActionEditDelete.TakeOver( new cActionEditDelete( *this ) );
	pActionEditLockAxisX.TakeOver( new cActionEditLockAxisX( *this ) );
	pActionEditLockAxisY.TakeOver( new cActionEditLockAxisY( *this ) );
	pActionEditLockAxisZ.TakeOver( new cActionEditLockAxisZ( *this ) );
	pActionEditLockLocal.TakeOver( new cActionEditLockLocal( *this ) );
	
	pActionRigAddSphere.TakeOver( new cActionRigAddSphere( *this ) );
	pActionRigAddBox.TakeOver( new cActionRigAddBox( *this ) );
	pActionRigAddCylinder.TakeOver( new cActionRigAddCylinder( *this ) );
	pActionRigAddCapsule.TakeOver( new cActionRigAddCapsule( *this ) );
	pActionRigAddConstraint.TakeOver( new cActionRigAddConstraint( *this ) );
	pActionRigAddPush.TakeOver( new cActionRigAddPush( *this ) );
	pActionRigShowShapes.TakeOver( new cActionRigShowShapes( *this ) );
	pActionRigShowConstraints.TakeOver( new cActionRigShowConstraints( *this ) );
	pActionRigShowPushes.TakeOver( new cActionRigShowPushes( *this ) );
	
	pActionBoneAdd.TakeOver( new cActionBoneAdd( *this ) );
	pActionBoneAddSphere.TakeOver( new cActionBoneAddSphere( *this ) );
	pActionBoneAddBox.TakeOver( new cActionBoneAddBox( *this ) );
	pActionBoneAddCylinder.TakeOver( new cActionBoneAddCylinder( *this ) );
	pActionBoneAddCapsule.TakeOver( new cActionBoneAddCapsule( *this ) );
	pActionBoneAddConstraint.TakeOver( new cActionBoneAddConstraint( *this ) );
	pActionBoneShowBones.TakeOver( new cActionBoneShowBones( *this ) );
	pActionBoneShowAllShapes.TakeOver( new cActionBoneShowShapes( *this ) );
	pActionBoneShowAllConstraints.TakeOver( new cActionBoneShowConstraints( *this ) );
	pActionBoneMirror.TakeOver( new cActionBoneMirror( *this ) );
	pActionBoneImport.TakeOver( new cActionBoneImport( *this ) );
	pActionBoneScaleMass.TakeOver( new cActionBoneScaleMass( *this ) );
	pActionBoneMassFromVolume.TakeOver( new cActionBoneMassFromVolume( *this ) );
	
	pActionViewShapeXRay.TakeOver( new cActionViewShapeXRay( *this ) );
	
	pActionSimulationRun.TakeOver( new cActionSimRun( *this ) );
	
	
	// register for updating
	AddUpdateAction( pActionFileNew );
	AddUpdateAction( pActionFileOpen );
	AddUpdateAction( pActionFileSave );
	AddUpdateAction( pActionFileSaveAs );
	
	AddUpdateAction( pActionEditUndo );
	AddUpdateAction( pActionEditRedo );
	AddUpdateAction( pActionEditCut );
	AddUpdateAction( pActionEditCopy );
	AddUpdateAction( pActionEditPaste );
	AddUpdateAction( pActionEditBoneMode );
	AddUpdateAction( pActionEditShapeMode );
	AddUpdateAction( pActionEditConstraintMode );
	AddUpdateAction( pActionEditPushMode );
	AddUpdateAction( pActionEditSelectMode );
	AddUpdateAction( pActionEditMoveMode );
	AddUpdateAction( pActionEditScaleMode );
	AddUpdateAction( pActionEditRotateMode );
// 	AddUpdateAction( pActionEdit3DCursorMode );
	AddUpdateAction( pActionEditSelectAll );
	AddUpdateAction( pActionEditSelectAllWithShapes );
	AddUpdateAction( pActionEditSelectNone );
	AddUpdateAction( pActionEditDelete );
	AddUpdateAction( pActionEditLockAxisX );
	AddUpdateAction( pActionEditLockAxisY );
	AddUpdateAction( pActionEditLockAxisZ );
	AddUpdateAction( pActionEditLockLocal );
	
	AddUpdateAction( pActionRigAddSphere );
	AddUpdateAction( pActionRigAddBox );
	AddUpdateAction( pActionRigAddCylinder );
	AddUpdateAction( pActionRigAddCapsule );
	AddUpdateAction( pActionRigAddConstraint );
	AddUpdateAction( pActionRigAddPush );
	AddUpdateAction( pActionRigShowShapes );
	AddUpdateAction( pActionRigShowConstraints );
	AddUpdateAction( pActionRigShowPushes );
	
	AddUpdateAction( pActionBoneAdd );
	AddUpdateAction( pActionBoneAddSphere );
	AddUpdateAction( pActionBoneAddBox );
	AddUpdateAction( pActionBoneAddCylinder );
	AddUpdateAction( pActionBoneAddCapsule );
	AddUpdateAction( pActionBoneAddConstraint );
	AddUpdateAction( pActionBoneShowBones );
	AddUpdateAction( pActionBoneShowAllShapes );
	AddUpdateAction( pActionBoneShowAllConstraints );
	AddUpdateAction( pActionBoneMirror );
	AddUpdateAction( pActionBoneImport );
	AddUpdateAction( pActionBoneScaleMass );
	AddUpdateAction( pActionBoneMassFromVolume );
	
	AddUpdateAction( pActionViewShapeXRay );
	
	AddUpdateAction( pActionSimulationRun );
}

void reWindowMain::pCreateToolBarFile(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBFile.TakeOver( new igdeToolBar( GetEnvironment() ) );
	
	helper.ToolBarButton( pTBFile, pActionFileNew );
	helper.ToolBarButton( pTBFile, pActionFileOpen );
	helper.ToolBarButton( pTBFile, pActionFileSave );
	
	AddSharedToolBar( pTBFile );
}

void reWindowMain::pCreateToolBarEdit(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBEdit.TakeOver( new igdeToolBar( GetEnvironment() ) );
	
	helper.ToolBarButton( pTBEdit, pActionEditUndo );
	helper.ToolBarButton( pTBEdit, pActionEditRedo );
	
	helper.ToolBarSeparator( pTBEdit );
	helper.ToolBarButton( pTBEdit, pActionEditCut );
	helper.ToolBarButton( pTBEdit, pActionEditCopy );
	helper.ToolBarButton( pTBEdit, pActionEditPaste );
	
	helper.ToolBarSeparator( pTBEdit );
	helper.ToolBarToggleButton( pTBEdit, pActionEditBoneMode );
	helper.ToolBarToggleButton( pTBEdit, pActionEditShapeMode );
	helper.ToolBarToggleButton( pTBEdit, pActionEditConstraintMode );
	helper.ToolBarToggleButton( pTBEdit, pActionEditPushMode );
	
	helper.ToolBarSeparator( pTBEdit );
	helper.ToolBarToggleButton( pTBEdit, pActionEditSelectMode );
	helper.ToolBarToggleButton( pTBEdit, pActionEditMoveMode );
	helper.ToolBarToggleButton( pTBEdit, pActionEditScaleMode );
	helper.ToolBarToggleButton( pTBEdit, pActionEditRotateMode );
// 	helper.ToolBarToggleButton( pTBEdit, pActionEdit3DCursorMode );
	
	helper.ToolBarSeparator( pTBEdit );
	helper.ToolBarToggleButton( pTBEdit, pActionEditLockAxisX );
	helper.ToolBarToggleButton( pTBEdit, pActionEditLockAxisY );
	helper.ToolBarToggleButton( pTBEdit, pActionEditLockAxisZ );
	helper.ToolBarToggleButton( pTBEdit, pActionEditLockLocal );
	
	AddSharedToolBar( pTBEdit );
}

void reWindowMain::pCreateMenu(){
	igdeEnvironment &env = GetEnvironment();
	igdeMenuCascadeReference cascade;
	
	cascade.TakeOver( new igdeMenuCascade( env, "File", deInputEvent::ekcF ) );
	pCreateMenuFile( cascade );
	AddSharedMenu( cascade );
	
	cascade.TakeOver( new igdeMenuCascade( env, "Edit", deInputEvent::ekcE ) );
	pCreateMenuEdit( cascade );
	AddSharedMenu( cascade );
	
	cascade.TakeOver( new igdeMenuCascade( env, "Rig", deInputEvent::ekcR ) );
	pCreateMenuRig( cascade );
	AddSharedMenu( cascade );
	
	cascade.TakeOver( new igdeMenuCascade( env, "Bone", deInputEvent::ekcB ) );
	pCreateMenuBone( cascade );
	AddSharedMenu( cascade );
	
	cascade.TakeOver( new igdeMenuCascade( env, "View", deInputEvent::ekcV ) );
	pCreateMenuView( cascade );
	AddSharedMenu( cascade );
	
	cascade.TakeOver( new igdeMenuCascade( env, "Simulation", deInputEvent::ekcS ) );
	pCreateMenuSimulation( cascade );
	AddSharedMenu( cascade );
}

void reWindowMain::pCreateMenuFile( igdeMenuCascade &menu ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( menu, pActionFileNew );
	helper.MenuCommand( menu, pActionFileOpen );
	helper.MenuRecentFiles( menu, GetRecentFiles() );
	helper.MenuCommand( menu, pActionFileSave );
	helper.MenuCommand( menu, pActionFileSaveAs );
}

void reWindowMain::pCreateMenuEdit( igdeMenuCascade &menu ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( menu, pActionEditUndo );
	helper.MenuCommand( menu, pActionEditRedo );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pActionEditCut );
	helper.MenuCommand( menu, pActionEditCopy );
	helper.MenuCommand( menu, pActionEditPaste );
	
	helper.MenuSeparator( menu );
	helper.MenuOption( menu, pActionEditSelectMode );
	helper.MenuOption( menu, pActionEditMoveMode );
	helper.MenuOption( menu, pActionEditScaleMode );
	helper.MenuOption( menu, pActionEditRotateMode );
// 	helper.MenuOption( menu, pActionEdit3DCursorMode );
	
	helper.MenuSeparator( menu );
	helper.MenuOption( menu, pActionEditBoneMode );
	helper.MenuOption( menu, pActionEditShapeMode );
	helper.MenuOption( menu, pActionEditConstraintMode );
	helper.MenuOption( menu, pActionEditPushMode );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pActionEditSelectAll );
	helper.MenuCommand( menu, pActionEditSelectAllWithShapes );
	helper.MenuCommand( menu, pActionEditSelectNone );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pActionEditDelete );
	
	helper.MenuSeparator( menu );
	helper.MenuCheck( menu, pActionEditLockAxisX );
	helper.MenuCheck( menu, pActionEditLockAxisY );
	helper.MenuCheck( menu, pActionEditLockAxisZ );
	helper.MenuCheck( menu, pActionEditLockLocal );
}

void reWindowMain::pCreateMenuRig( igdeMenuCascade &menu ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( menu, pActionRigAddSphere );
	helper.MenuCommand( menu, pActionRigAddBox );
	helper.MenuCommand( menu, pActionRigAddCylinder );
	helper.MenuCommand( menu, pActionRigAddCapsule );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pActionRigAddConstraint );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pActionRigAddPush );
	
	helper.MenuSeparator( menu );
	helper.MenuCheck( menu, pActionRigShowShapes );
	helper.MenuCheck( menu, pActionRigShowConstraints );
	helper.MenuCheck( menu, pActionRigShowPushes );
}

void reWindowMain::pCreateMenuBone( igdeMenuCascade &menu ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( menu, pActionBoneAdd );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pActionBoneMirror );
	helper.MenuCommand( menu, pActionBoneImport );
	helper.MenuCommand( menu, pActionBoneMassFromVolume );
	helper.MenuCommand( menu, pActionBoneScaleMass );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pActionBoneAddSphere );
	helper.MenuCommand( menu, pActionBoneAddBox );
	helper.MenuCommand( menu, pActionBoneAddCylinder );
	helper.MenuCommand( menu, pActionBoneAddCapsule );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pActionBoneAddConstraint );
	
	helper.MenuSeparator( menu );
	helper.MenuCheck( menu, pActionBoneShowBones );
	helper.MenuCheck( menu, pActionBoneShowAllShapes );
	helper.MenuCheck( menu, pActionBoneShowAllConstraints );
}

void reWindowMain::pCreateMenuView( igdeMenuCascade &menu ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCheck( menu, pActionViewShapeXRay );
}

void reWindowMain::pCreateMenuSimulation( igdeMenuCascade &menu ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCheck( menu, pActionSimulationRun );
}
