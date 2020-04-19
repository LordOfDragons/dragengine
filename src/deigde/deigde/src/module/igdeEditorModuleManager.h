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
	igdeEditorModuleManager( igdeWindowMain &windowMain );
	
	/** \brief Clean up editor module manager. */
	~igdeEditorModuleManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline igdeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Module path. */
	inline const decString &GetPathModules() const{ return pPathModules; }
	
	/** \brief Activate module or NULL if none is active. */
	inline igdeEditorModuleDefinition *GetActiveModule() const{ return pActiveModule; }
	
	/** \brief Set active module. */
	void SetActiveModule( igdeEditorModuleDefinition *module );
	
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
	igdeEditorModuleDefinition *GetModuleAt( int index ) const;
	
	/** \brief Module with ID or NULL if not found. */
	igdeEditorModuleDefinition *GetModuleWithID ( const char *id ) const;
	
	/** \brief Remove all modules. */
	void RemoveAllModules();
	
	/** \brief Start all modules. */
	void StartModules();
	
	/** \brief Stops all modules. */
	void StopModules();
	
	/** \brief Recent module. */
	igdeEditorModuleDefinition *GetRecentModuleAt( int index ) const;
	
	/** \brief Change recent used position of module. */
	void ChangeModuleRecentUsedPosition( igdeEditorModuleDefinition *module, int position );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pScanForModules();
};

#endif
