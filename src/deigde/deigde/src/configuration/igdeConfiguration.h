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
 * IGDE Configuration.
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
	/** Create configuration. */
	igdeConfiguration(igdeWindowMain &windowMain);
	
	/** Clean up configuration. */
	~igdeConfiguration();
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Main window. */
	inline igdeWindowMain &GetWindowMain() const{return pWindowMain;}
	
	
	/** System config path. */
	inline const decString &GetPathConfigSystem() const{return pPathConfigSystem;}
	void SetPathConfigSystem(const char *path);
	
	/** User config path. */
	inline const decString &GetPathConfigUser() const{return pPathConfigUser;}
	void SetPathConfigUser(const char *path);
	
	/** Shares path. */
	inline const decString &GetPathShares() const{return pPathShares;}
	void SetPathShares(const char *path);
	
	/** Library path. */
	inline const decString &GetPathLib() const{return pPathLib;}
	void SetPathLib(const char *path);
	
	/** Logs path. */
	inline const decString &GetPathLogs() const{return pPathLogs;}
	void SetPathLogs(const char *path);
	
	/** Default path for projects. */
	inline const decString &GetPathProjects() const{return pPathProjects;}
	void SetPathProjects(const char *path);
	
	
	/** Path to igde shared data. */
	inline const decString &GetPathIGDEData() const{return pPathIGDEData;}
	void SetPathIGDEData(const char *path);
	
	/** Path to igde project templates. */
	inline const decString &GetPathIGDETemplates() const{return pPathIGDETemplates;}
	void SetPathIGDETemplates(const char *path);
	
	/** Path to igde shared game definitions. */
	inline const decString &GetPathIGDEGameDefs() const{return pPathIGDEGameDefs;}
	void SetPathIGDEGameDefs(const char *path);
	
	/** Path to igde editors shared data. */
	inline const decString &GetPathIGDEEditorData() const{return pPathIGDEEditorData;}
	void SetPathIGDEEditorData(const char *path);
	
	
	/** List of recently loaded game project file names. */
	inline decStringList &GetRecentProjectList(){return pRecentProjectList;}
	inline const decStringList &GetRecentProjectList() const{return pRecentProjectList;}
	
	/** Maximal size of the recenr project list. */
	inline int GetMaxRecentProjectEntries() const{return pMaxRecentProjectEntries;}
	void SetMaxRecentProjectEntries(int entries);
	
	
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
