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

#ifndef _IGDEEDITORMODULEMANAGER_H_
#define _IGDEEDITORMODULEMANAGER_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/string/decString.h>

class igdeEditorModuleDefinition;
class igdeWindowMain;


/**
 * \brief Manages editor modules.
 *
 * Stores a list of editor modules providing support for scanning
 * the module directory for valid modules and loading them.
 */
class igdeEditorModuleManager{
private:
	decString pPathModules;
	igdeWindowMain &pWindowMain;
	decObjectOrderedSet pModules;
	igdeEditorModuleDefinition *pActiveModule;
	decPointerList pRecentlyUsed;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new editor module manager. */
	igdeEditorModuleManager(igdeWindowMain &windowMain);
	
	/** \brief Clean up editor module manager. */
	~igdeEditorModuleManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline igdeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Module path. */
	inline const decString &GetPathModules() const{ return pPathModules; }
	
	/** \brief Activate module or nullptr if none is active. */
	inline igdeEditorModuleDefinition *GetActiveModule() const{ return pActiveModule; }
	
	/** \brief Set active module. */
	void SetActiveModule(igdeEditorModuleDefinition *module);
	
	/** \brief Activate first possible module. */
	void ActivateMostRecentModule();
	
	/** \brief Activate project manager if present. */
	void ActivateProjectManager();
	/*@}*/
	
	
	
	/** \name Modules */
	/*@{*/
	/** \brief Number of modules. */
	int GetModuleCount() const;
	
	/** \brief Module at index. */
	igdeEditorModuleDefinition *GetModuleAt(int index) const;
	
	/** \brief Module with ID or nullptr if not found. */
	igdeEditorModuleDefinition *GetModuleWithID (const char *id) const;
	
	/** \brief Remove all modules. */
	void RemoveAllModules();
	
	/** \brief Start all modules. */
	void StartModules();
	
	/** \brief Stops all modules. */
	void StopModules();
	
	/** \brief Recent module. */
	igdeEditorModuleDefinition *GetRecentModuleAt(int index) const;
	
	/** \brief Change recent used position of module. */
	void ChangeModuleRecentUsedPosition(igdeEditorModuleDefinition *module, int position);
	
	/** \brief Reset recently used position of modules. */
	void ResetRecentUsedPosition();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pScanForModules();
};

#endif
