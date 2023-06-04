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

#ifndef _IGDERECENTFILES_H_
#define _IGDERECENTFILES_H_

#include "../gui/menu/igdeMenuCascadeReference.h"

#include <dragengine/common/string/decStringList.h>


class igdeEnvironment;
class igdeEditorWindow;
class decXmlElementTag;
class decXmlWriter;


/**
 * \brief Recent files with support to synchronize an igdeMenuCascade.
 */
class DE_DLL_EXPORT igdeRecentFiles{
private:
	igdeEnvironment &pEnvironment;
	igdeEditorWindow *pEditorWindow;
	int pSize;
	bool pUnixPath;
	decStringList pFiles;
	igdeMenuCascadeReference pMenu;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create recent files. */
	igdeRecentFiles( igdeEnvironment &environment, bool unixPath, int size );
	
	/** \brief Create recent files. */
	igdeRecentFiles( igdeEditorWindow &editorWindow, bool unixPath, int size );
	
	/** \brief Clean up recent files. */
	virtual ~igdeRecentFiles();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment &GetEnvironment() const{ return pEnvironment; }
	
	/** \brief Editor window or NULL. */
	inline igdeEditorWindow *GetEditorWindow() const{ return pEditorWindow; }
	
	/** \brief Maximum count of files to remember. */
	inline int GetSize() const{ return pSize; }
	
	/** \brief Set maximum count of files to remember. */
	void SetSize( int size );
	
	/** \brief Use unix path. */
	inline bool GetUnixPath() const{ return pUnixPath; }
	
	/** \brief Recent files. */
	inline const decStringList &GetFiles() const{ return pFiles; }
	
	/** \brief Set recent files. */
	void SetFiles( const decStringList &files );
	
	/** \brief Menu to synchronize or NULL if not used. */
	inline igdeMenuCascade *GetMenu() const{ return pMenu; }
	
	/** \brief Set menu to synchronize or NULL if not used. */
	void SetMenu( igdeMenuCascade *menu );
	
	/** \brief Add file in first position. */
	void AddFile( const char *filename );
	
	/** \brief Remove all files. */
	void RemoveAllFiles();
	
	/**
	 * \brief Load from XML.
	 * 
	 * Root element has to contain 0 or more tags of the name "filename" containing as CDATA
	 * the filename to add to the list. Typically the root element is called "recentFiles"
	 * but this is not mandatory.
	 * 
	 * This call directly manipulates files list and does not call FilesChanged().
	 */
	void ReadFromXml( const decXmlElementTag &root );
	
	/**
	 * \brief Save to XML.
	 * 
	 * Writes tag with name \em tagName containing one child tag for each file entry.
	 * The child tags have the name "filename" and contain as CDATA the filename.
	 */
	void WriteToXml( decXmlWriter &writer, const char *tagName = "recentFiles" ) const;
	
	
	
	/**
	 * \brief Open file.
	 * 
	 * Default implementation calls igdeEditorWindow::LoadDocument() if editor window is present.
	 */
	virtual void OpenFile( const char *filename );
	
	/** \brief Update menu if present. */
	virtual void UpdateMenu();
	
	/**
	 * \brief Recent files changed.
	 * 
	 * Default implementation calls igdeEditorWindow::RecentFilesChanged() if editor window is present.
	 */
	virtual void FilesChanged();
	
	
	
protected:
	/** \brief Ensure recent files count is at most size. */
	void EnsureSize();
	/*@}*/
};

#endif
