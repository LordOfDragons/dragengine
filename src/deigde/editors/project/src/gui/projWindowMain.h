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

#ifndef _PROJWINDOWMAIN_H_
#define _PROJWINDOWMAIN_H_

#include "../loadsave/projLoadSaveSystem.h"

#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/gui/igdeEditorWindow.h>
#include <deigde/gui/igdeToolBarReference.h>
#include <deigde/gui/igdeTabBookReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/event/igdeActionUndoReference.h>
#include <deigde/gui/event/igdeActionRedoReference.h>
#include <deigde/gui/event/igdeActionExternOpenReference.h>
#include <deigde/gui/resources/igdeFont.h>
#include <deigde/gui/resources/igdeIconReference.h>

class projIGDEModule;
class projProject;
class projPanelProfiles;
class projPanelTestRun;
class projPanelUndoHistory;
class projWindowMainListener;
class decUnicodeStringList;
class projConfiguration;

class decStringList;



/**
 * \brief Editor window.
 */
class projWindowMain : public igdeEditorWindow{
private:
	projWindowMainListener *pListener;
	
	igdeActionReference pActionDistSave;
	
	igdeActionUndoReference pActionEditUndo;
	igdeActionRedoReference pActionEditRedo;
	igdeActionReference pActionEditCut;
	igdeActionReference pActionEditCopy;
	igdeActionReference pActionEditPaste;
	
	igdeActionReference pActionProfileAdd;
	igdeActionReference pActionProfileRemove;
	igdeActionReference pActionProfileDuplicate;
	igdeActionReference pActionProfileDistribute;
	igdeActionReference pActionProfileTestRun;
	
	igdeActionExternOpenReference pActionShowDelga;
	igdeActionExternOpenReference pActionShowContent;
	igdeActionExternOpenReference pActionShowConfig;
	igdeActionExternOpenReference pActionShowOverlay;
	igdeActionExternOpenReference pActionShowCapture;
	igdeActionExternOpenReference pActionShowLogs;
	
	igdeIconReference pIconStart;
	igdeIconReference pIconStop;
	igdeIconReference pIconKill;
	igdeIconReference pIconDelga;
	
	igdeToolBarReference pTBDistribute;
	igdeToolBarReference pTBEdit;
	
	projConfiguration *pConfiguration;
	igdeClipboard pClipboard;
	projLoadSaveSystem pLoadSaveSystem;
	
	projProject *pProject;
	
	projPanelProfiles *pPanelProfiles;
	projPanelTestRun *pPanelTestRun;
	projPanelUndoHistory *pPanelUndoHistory;
	
	igdeTabBookReference pTabPanels;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create window. */
	projWindowMain( igdeEditorModule &module );
	
	/** \brief Clean up window. */
	virtual ~projWindowMain();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Actions. */
	inline igdeAction *GetActionDistSave() const{ return pActionDistSave; }
	
	inline igdeActionUndo *GetActionEditUndo() const{ return pActionEditUndo; }
	inline igdeActionRedo *GetActionEditRedo() const{ return pActionEditRedo; }
	inline igdeAction *GetActionEditCut() const{ return pActionEditCut; }
	inline igdeAction *GetActionEditCopy() const{ return pActionEditCopy; }
	inline igdeAction *GetActionEditPaste() const{ return pActionEditPaste; }
	
	inline igdeAction *GetActionProfileAdd() const{ return pActionProfileAdd; }
	inline igdeAction *GetActionProfileRemove() const{ return pActionProfileRemove; }
	inline igdeAction *GetActionProfileDuplicate() const{ return pActionProfileDuplicate; }
	inline igdeAction *GetActionProfileDistribute() const{ return pActionProfileDistribute; }
	inline igdeAction *GetActionProfileTestRun() const{ return pActionProfileTestRun; }
	
	inline igdeActionExternOpen *GetActionShowDistribute() const{ return pActionShowDelga; }
	inline igdeActionExternOpen *GetActionShowContent() const{ return pActionShowContent; }
	inline igdeActionExternOpen *GetActionShowConfig() const{ return pActionShowConfig; }
	inline igdeActionExternOpen *GetActionShowOverlay() const{ return pActionShowOverlay; }
	inline igdeActionExternOpen *GetActionShowCapture() const{ return pActionShowCapture; }
	inline igdeActionExternOpen *GetActionShowLogs() const{ return pActionShowLogs; }
	
	inline igdeIcon *GetIconStart() const{ return pIconStart; }
	inline igdeIcon *GetIconStop() const{ return pIconStop; }
	inline igdeIcon *GetIconKill() const{ return pIconKill; }
	inline igdeIcon *GetIconDelga() const{ return pIconDelga; }
	
	
	
	/** \brief Asks the user if it is okay to quit the application. */
	bool QuitRequest();
	
	
	
	/** Configuration. */
	inline projConfiguration &GetConfiguration() const{ return *pConfiguration; }
	
	/** \brief Clipboard. */
	inline igdeClipboard &GetClipboard(){ return pClipboard; }
	inline const igdeClipboard &GetClipboard() const{ return pClipboard; }
	
	/** \brief Load save system. */
	inline projLoadSaveSystem &GetLoadSaveSystem(){ return pLoadSaveSystem; }
	inline const projLoadSaveSystem &GetLoadSaveSystem() const{ return pLoadSaveSystem; }
	
	/** \brief Test-Run panel. */
	inline projPanelTestRun &GetPanelTestRun() const{ return *pPanelTestRun; }
	
	/** \brief Activate test-run panel. */
	void ActivatePanelTestRun();
	
	
	
	/** \brief Project. */
	inline projProject *GetProject() const{ return pProject; }
	
	/** \brief Set project. */
	void SetProject( projProject *project );
	
	
	
	/** \brief Load project. */
	void LoadProject();
	
	/** \brief Load project local. */
	void LoadProjectLocal( projProject &project );
	
	/** \brief Save project. */
	void SaveProject();
	
	/** \brief Save project local. */
	void SaveProjectLocal();
	
	
	
	/** \brief Game like frame update. */
	void OnFrameUpdate( float elapsed );
	
	/** \brief Retrieve a list of unsaved open documents. */
	void GetChangedDocuments( decStringList &list );
	
	/** \brief Requests a document to be saved. */
	bool SaveDocument( const char *filename );
	
	/** \brief The game project has changed. */
	void OnGameProjectChanged();
	
	/** \brief Update show action path. */
	void UpdateShowActionPath();
	
	/**
	 * \brief Process command line arguments.
	 * 
	 * This method can be called multiple times. Module has to process arguments from the
	 * first argument in the list and has to stop at the first unsupported argument. The
	 * consumed arguments have to be removed from the beginning of the list. If the module
	 * wishes to close the application it has to return false. Return true to continue
	 * processing command line arguments.
	 */
	bool ProcessCommandLine( decUnicodeStringList &arguments );
	/*@}*/
	
	
	
private:
	void pLoadIcons();
	void pCreateActions();
	void pCreateToolBarDistribute();
	void pCreateToolBarEdit();
	void pCreateMenu();
	void pCreateMenuDistribute( igdeMenuCascade &menu );
	void pCreateMenuEdit( igdeMenuCascade &menu );
	void pCreateMenuProfile( igdeMenuCascade &menu );
	bool pCmdLineProfileDistribute( decUnicodeStringList &arguments );
	bool pCmdLineProfileDistributeFile( decUnicodeStringList &arguments );
	bool pCmdLineProfileList( decUnicodeStringList &arguments );
	void pCmdLineHelp();
};

#endif
