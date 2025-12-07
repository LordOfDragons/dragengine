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

#ifndef _IGDEEDITORWINDOW_H_
#define _IGDEEDITORWINDOW_H_

#include "layout/igdeContainerBox.h"
#include "../utils/igdeRecentFiles.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decStringList.h>


class igdeAction;
class igdeEditorModule;
class igdeMenuCascade;
class igdeToolBar;
class igdeStepableTask;

class deException;


/**
 * \brief Editor window.
 */
class DE_DLL_EXPORT igdeEditorWindow : public igdeContainerBox{
private:
	igdeEditorModule &pEditorModule;
	bool pActiveModule;
	decObjectOrderedSet pSharedMenus;
	decObjectOrderedSet pSharedToolBars;
	decObjectOrderedSet pUpdateActions;
	igdeRecentFiles pRecentFiles;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	igdeEditorWindow(igdeEditorModule &editorModule);
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeEditorWindow();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Editor module. */
	inline igdeEditorModule &GetEditorModule() const{ return pEditorModule; }
	
	/** \brief Parent editor module is the active module. */
	inline bool GetActiveModule() const{ return pActiveModule; }
	
	/** \brief Recent files. */
	inline igdeRecentFiles &GetRecentFiles(){ return pRecentFiles; }
	inline const igdeRecentFiles &GetRecentFiles() const{ return pRecentFiles; }
	
	/** \brief Game engine is about to be started. */
	virtual void OnBeforeEngineStart();
	
	/** \brief Game engine has been started. */
	virtual void OnAfterEngineStart();
	
	/** \brief Game engine is about to be stopped. */
	virtual void OnBeforeEngineStop();
	
	/** \brief Game engine has been stopped. */
	virtual void OnAfterEngineStop();
	
	/** \brief Module has been activated. */
	virtual void OnActivate();
	
	/** \brief Module has been deactivated. */
	virtual void OnDeactivate();
	
#ifdef OS_W32
	/** \brief Drop UI native widgets to save on resource handlers. */
	virtual void SaveResourceHandlers();
#endif
	
	/** \brief Game like frame update. */
	virtual void OnFrameUpdate(float elapsed);
	
	/**
	 * \brief Retrieves a list of changed documents.
	 * 
	 * This list is requested by the IGDE if a game project is closed due to creating or
	 * loading a new one or because the application is about to be closed. Editors modules
	 * have to add to the list the filenames of open documents that have changed and need
	 * saving. The filename is later used in calls to \ref SaveDocument to save the file
	 * if requested by the user. All other files are discarded.
	 */
	virtual void GetChangedDocuments(decStringList &list);
	
	/**
	 * \brief Requests a document to be loaded.
	 */
	virtual void LoadDocument(const char *filename);
	
	/**
	 * \brief Requests a document to be saved.
	 * 
	 * The document has to be saved if changed. If not changed this call can be ignored.
	 * This call is usually made after a previous call to \ref GetUnsavedDocuments.
	 * 
	 * \returns True if the saving has been successful or false otherwise.
	 */
	virtual bool SaveDocument(const char *filename);
	
	/**
	 * \brief Recent files changed.
	 */
	virtual void RecentFilesChanged();
	
	/**
	 * \brief The game project has changed.
	 * 
	 * Notification send to the editor modules after a new game project has been set.
	 * The editor module has to discard all open documents and all references held of
	 * objects related to the previous game project. The old game project is kept
	 * alive until all editor modules have processed the OnGameProjectChanged().
	 */
	virtual void OnGameProjectChanged();
	
	/**
	 * \brief Project game definition changed.
	 * 
	 * Called after an editor changed the game definition. The old game definition used so
	 * far is replaced by a new game definition. The module has to update everything
	 * using the old game definition. This process can be potentially lengthy. For this
	 * reason the module has to return a steppable task to do the processing. If the module
	 * does not need any update NULL can be returned. The caller delets the task once
	 * finished processing.
	 * 
	 * The default implementation returns NULL.
	 */
	virtual igdeStepableTask *OnGameDefinitionChanged();
	
	/** \brief Display exception error in a message dialog. */
	void DisplayException(const deException &exception);
	/*@}*/
	
	
	
	/**
	 * \name Shared Menus.
	 * 
	 * Shared menus are shown in the main IGDE window when the editor module is active.
	 */
	/*@{*/
	/** \brief Number of shared menus. */
	int GetSharedMenuCount() const;
	
	/** \brief Shared menu at index. */
	igdeMenuCascade *GetSharedMenuAt(int index) const;
	
	/** \brief Add shared menu. */
	void AddSharedMenu(igdeMenuCascade *menu);
	
	/** \brief Remove shared menu. */
	void RemoveSharedMenu(igdeMenuCascade *menu);
	
	/** \brief Remove all shared menus. */
	void RemoveAllSharedMenus();
	/*@}*/
	
	
	
	/**
	 * \name Shared Tool Bars.
	 * 
	 * Shared toolbars are shown in the main IGDE window when the editor module is active.
	 */
	/*@{*/
	/** \brief Number of shared toolbars. */
	int GetSharedToolBarCount() const;
	
	/** \brief Shared toolbar at index. */
	igdeToolBar *GetSharedToolBarAt(int index) const;
	
	/** \brief Add shared tool bar. */
	void AddSharedToolBar(igdeToolBar *toolbar);
	
	/** \brief Remove shared tool bar. */
	void RemoveSharedToolBar(igdeToolBar *toolbar);
	
	/** \brief Remove all shared tool bars. */
	void RemoveAllSharedToolBars();
	/*@}*/
	
	
	
	/**
	 * \name Update Actions.
	 * 
	 * Registering actions for updating is a convenience functionality of the editor window.
	 */
	/*@{*/
	/** \brief Add action to be updated when UpdateAllActions() is called. */
	void AddUpdateAction(igdeAction *action);
	
	/** \brief Remove action to be updated when UpdateAllActions() is called. */
	void RemoveUpdateAction(igdeAction *action);
	
	/** \brief Remove all update actions. */
	void RemoveAllUpdateActions();
	
	/** \brief Update all registered actions. */
	void UpdateAllActions();
	/*@}*/
	
	
	
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/**
	 * \brief Create native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void CreateNativeWidget();
	/*@}*/
};

#endif
