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

#ifndef _IGDEEDITORMODULE_H_
#define _IGDEEDITORMODULE_H_

#include <dragengine/common/string/decString.h>

class decStringList;
class igdeEnvironment;
class igdeEditorWindow;
class igdeStepableTask;
class deEngine;
class deLogger;
class deException;
class decUnicodeStringList;


/**
 * \brief Interface for editor modules.
 * 
 * The definition of editor module is stored in an XML file not in this class.
 * This is just the editor module itself.
 */
class DE_DLL_EXPORT igdeEditorModule{
private:
	igdeEnvironment &pEnvironment;
	decString pLoggingName;
	decString pEditorDirectory;
	decString pEditorPathLib;
	igdeEditorWindow::Ref pEditorWindow;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create editor module. */
	igdeEditorModule(igdeEnvironment &environment);
	
	/** \brief Clean up editor module. */
	virtual ~igdeEditorModule();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment &GetEnvironment() const{ return pEnvironment; }
	
	/** \brief Game engine. */
	deEngine *GetEngine() const;
	
	/** \brief Editor directory used to located editor specific data in virtual file systems. */
	inline const decString &GetEditorDirectory() const{ return pEditorDirectory; }
	
	/**
	 * \brief Set editor directory used to located editor specific data in virtual file systems.
	 * 
	 * To be used only by IGDE. Do not call from editor modules.
	 */
	void SetEditorDirectory(const char *directory);
	
	/** \brief Editor module libraries directory path. */
	inline const decString &GetEditorPathLib() const{ return pEditorPathLib; }
	
	/**
	 * \brief Set editor module libraries directory path.
	 * 
	 * To be used only by IGDE. Do not call from editor modules.
	 */
	void SetEditorPathLib(const char *path);
	
	/** \brief Editor window used by module. */
	inline const igdeEditorWindow::Ref &GetEditorWindow() const{ return pEditorWindow; }
	
	/**
	 * \brief Set editor window used by module.
	 * 
	 * Takes over the reference. Use only like this: SetEditorWindow(new MyEditorWindow());
	 */
	void SetEditorWindow(igdeEditorWindow *editorWindow);
	
	
	
	/** \brief Start module. */
	virtual void Start();
	
	/** \brief Stop module. */
	virtual void Stop();
	
	/** \brief Game engine is about to be started. */
	virtual void OnBeforeEngineStart();
	
	/** \brief Game engine has been started. */
	virtual void OnAfterEngineStart();
	
	/** \brief Game engine is about to be stopped. */
	virtual void OnBeforeEngineStop();
	
	/** \brief Game engine has been stopped. */
	virtual void OnAfterEngineStop();
	
	/** \brief Module is has been activated. */
	virtual void OnActivate();
	
	/** \brief Module has been deactivated. */
	virtual void OnDeactivate();
	
	/**
	 * \brief Game like frame update.
	 * 
	 * Modules can use this to simulate game conditions playing animations or conducting
	 * simulations in realtime. The elapsed time since the last call of this method is
	 * passed as argument. The elapsed time is measured relative to the host application
	 * and might not be as accurate as the game timer provided by the game engine in a
	 * real game situation.
	 */
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
	
	/**
	 * \brief Process command line arguments.
	 * 
	 * This method can be called multiple times. Module has to process arguments from the
	 * first argument in the list and has to stop at the first unsupported argument. The
	 * consumed arguments have to be removed from the beginning of the list. If the module
	 * wishes to close the application it has to return false. Return true to continue
	 * processing command line arguments.
	 */
	virtual bool ProcessCommandLine(decUnicodeStringList &arguments);
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** \brief Logger. */
	deLogger *GetLogger() const;
	
	/** \brief Name used as source logging. */
	const decString &GetLoggingName() const;
	
	/** \brief Set name used as source for logging. */
	void SetLoggingName(const char *name);
	
	/** \brief Output information message on console. */
	void LogInfo(const char *message) const;
	
	/** \brief Output formated information message on console. */
	void LogInfoFormat(const char *message, ...) const;
	
	/** \brief Output formated information message on console. */
	void LogInfoFormatUsing(const char *message, va_list args) const;
	
	/** \brief Output warning message on console. */
	void LogWarn(const char *message) const;
	
	/** \brief Output formated warning message on console. */
	void LogWarnFormat(const char *message, ...) const;
	
	/** \brief Output formated warning message on console. */
	void LogWarnFormatUsing(const char *message, va_list args) const;
	
	/** \brief Output error message on console. */
	void LogError(const char *message) const;
	
	/** \brief Output formated error message on console. */
	void LogErrorFormat(const char *message, ...) const;
	
	/** \brief Output formated error message on console. */
	void LogErrorFormatUsing(const char *message, va_list args) const;
	
	/** \brief Output exception as error message on console. */
	void LogException(const deException &exception) const;
	/*@}*/
};

#endif
