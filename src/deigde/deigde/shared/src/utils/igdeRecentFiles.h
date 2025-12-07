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

#ifndef _IGDERECENTFILES_H_
#define _IGDERECENTFILES_H_

#include "../gui/menu/igdeMenuCascade.h"

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
	igdeMenuCascade::Ref pMenu;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create recent files. */
	igdeRecentFiles(igdeEnvironment &environment, bool unixPath, int size);
	
	/** \brief Create recent files. */
	igdeRecentFiles(igdeEditorWindow &editorWindow, bool unixPath, int size);
	
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
	void SetSize(int size);
	
	/** \brief Use unix path. */
	inline bool GetUnixPath() const{ return pUnixPath; }
	
	/** \brief Recent files. */
	inline const decStringList &GetFiles() const{ return pFiles; }
	
	/** \brief Set recent files. */
	void SetFiles(const decStringList &files);
	
	/** \brief Menu to synchronize or NULL if not used. */
	inline igdeMenuCascade *GetMenu() const{ return pMenu; }
	
	/** \brief Set menu to synchronize or NULL if not used. */
	void SetMenu(igdeMenuCascade *menu);
	
	/** \brief Add file in first position. */
	void AddFile(const char *filename);
	
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
	void ReadFromXml(const decXmlElementTag &root);
	
	/**
	 * \brief Save to XML.
	 * 
	 * Writes tag with name \em tagName containing one child tag for each file entry.
	 * The child tags have the name "filename" and contain as CDATA the filename.
	 */
	void WriteToXml(decXmlWriter &writer, const char *tagName = "recentFiles") const;
	
	
	
	/**
	 * \brief Open file.
	 * 
	 * Default implementation calls igdeEditorWindow::LoadDocument() if editor window is present.
	 */
	virtual void OpenFile(const char *filename);
	
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
