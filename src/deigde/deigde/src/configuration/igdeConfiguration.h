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

#ifndef _IGDECONFIGURATION_H_
#define _IGDECONFIGURATION_H_

#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/math/decMath.h>

class igdeWindowMain;



/**
 * \brief IGDE Configuration.
 */
class igdeConfiguration{
private:
	igdeWindowMain &pWindowMain;
	
	decString pPathConfigSystem;
	decString pPathConfigUser;
	decString pPathShares;
	decString pPathLib;
	decString pPathLogs;
	decString pPathProjects;
	
	decString pPathIGDEData;
	decString pPathIGDEGameDefs;
	decString pPathIGDETemplates;
	decString pPathIGDEEditorData;
	
	decStringList pRecentProjectList;
	int pMaxRecentProjectEntries;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new configuration. */
	igdeConfiguration( igdeWindowMain &windowMain );
	/** Cleans up the configuration. */
	~igdeConfiguration();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the main window. */
	inline igdeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** Retrieves the system config path. */
	inline const decString &GetPathConfigSystem() const{ return pPathConfigSystem; }
	/** Sets the system config path. */
	void SetPathConfigSystem( const char *path );
	/** Retrieves the user config path. */
	inline const decString &GetPathConfigUser() const{ return pPathConfigUser; }
	/** Sets the user config path. */
	void SetPathConfigUser( const char *path );
	/** Retrieves the shares path. */
	inline const decString &GetPathShares() const{ return pPathShares; }
	/** Sets the shares path. */
	void SetPathShares( const char *path );
	/** Retrieves the library path. */
	inline const decString &GetPathLib() const{ return pPathLib; }
	/** Sets the library path. */
	void SetPathLib( const char *path );
	/** Retrieves the logs path. */
	inline const decString &GetPathLogs() const{ return pPathLogs; }
	/** Sets the logs path. */
	void SetPathLogs( const char *path );
	/** Retrieves the default path for projects. */
	inline const decString &GetPathProjects() const{ return pPathProjects; }
	/** Sets the default path for projects. */
	void SetPathProjects( const char *path );
	
	
	
	/** \brief Path to the igde shared data. */
	inline const decString &GetPathIGDEData() const{ return pPathIGDEData; }
	
	/** \brief Set path to the igde shared  data. */
	void SetPathIGDEData( const char *path );
	
	/** \brief Path to the igde project templates. */
	inline const decString &GetPathIGDETemplates() const{ return pPathIGDETemplates; }
	
	/** \brief Set path to the igde project templates. */
	void SetPathIGDETemplates( const char *path );
	
	/** \brief Path to the igde shared game definitions. */
	inline const decString &GetPathIGDEGameDefs() const{ return pPathIGDEGameDefs; }
	
	/** \brief Set path to the igde shared game definitions. */
	void SetPathIGDEGameDefs( const char *path );
	
	/** \brief Path to the igde editors shared data. */
	inline const decString &GetPathIGDEEditorData() const{ return pPathIGDEEditorData; }
	
	/** \brief Set path to the igde editors shared data. */
	void SetPathIGDEEditorData( const char *path );
	
	
	
	/** \brief Retrieves the list of recently loaded game project file names. */
	inline decStringList &GetRecentProjectList(){ return pRecentProjectList; }
	inline const decStringList &GetRecentProjectList() const{ return pRecentProjectList; }
	/** \brief Retrieves the maximal size of the recenr project list. */
	inline int GetMaxRecentProjectEntries() const{ return pMaxRecentProjectEntries; }
	/** \brief Sets the maximal size of the recenr project list. */
	void SetMaxRecentProjectEntries( int entries );
	
	/** Locate path. */
	void LocatePath();
	/** Initialize virtual file system. */
	void InitVirtualFileSystem();
	/** Log some important value. */
	void LogImportantValues();
	
	/**
	 * Load the configuration. Tries to locate the configuration file in
	 * the known places reading first the system wide configuration and
	 * then the user configuration if found. For all not found configuration
	 * values the default value is used.
	 */
	void LoadConfiguration();
	/**
	 * Saves the configuration to the user configuration directory. If the
	 * directory or files do not exist they are created.
	 */
	void SaveConfiguration();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
