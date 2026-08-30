/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDENATIVEBEOSFILEDIALOG_H_
#define _IGDENATIVEBEOSFILEDIALOG_H_

#include "../../../filedialog/igdeFilePattern.h"

#include <interface/Window.h>
#include <interface/View.h>
#include <interface/TextControl.h>
#include <interface/Button.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <Menu.h>

#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decString.h>

class igdeWidget;
class deVirtualFileSystem;
class igdeNativeBeosVFSList;
class igdeNativeBeosVFSDirectoryBox;

/**
 * \brief File dialog for virtual file system selection.
 */
class igdeNativeBeosFileDialog : public BWindow {
private:
	enum eMessageIDs {
		ID_BTN_OK = 1001,
		ID_BTN_CANCEL = 1002,
		ID_TEXT_FILENAME = 1003,
		ID_CB_FILTER = 1004,
		ID_LIST_DOUBLE_CLICK = 1005
	};
	
	igdeWidget *pOwner;
	deVirtualFileSystem *pVFS;
	
	igdeNativeBeosVFSDirectoryBox *pDirectoryBox;
	igdeNativeBeosVFSList *pFileList;
	BTextControl *pEditFilename;
	BMenuField *pCBFilter;
	BButton *pBtnOK;
	BButton *pBtnCancel;
	
	igdeFilePattern::List pFilePatternList;
	decString pSelectedFile;
	int32 pResult;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new file dialog. */
	igdeNativeBeosFileDialog(igdeWidget &owner, deVirtualFileSystem *vfs, const char *name);
	
	/** \brief Cleans up the file dialog. */
	~igdeNativeBeosFileDialog() override;
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Get the virtual file system. */
	inline deVirtualFileSystem *GetVirtualFileSystem() const{ return pVFS; }
	
	/** \brief Get the file list. */
	inline igdeNativeBeosVFSList *GetFileList() const{ return pFileList; }
	
	/** \brief Get the directory box. */
	inline igdeNativeBeosVFSDirectoryBox *GetDirectoryBox() const{ return pDirectoryBox; }
	
	/** \brief Get the filename. */
	decString GetFilename() const;
	
	/** \brief Set the filename. */
	void SetFilename(const char *filename);
	
	/** \brief Set the filename with base path. */
	void SetFilename(const char *filename, const char *basePath);
	
	/** \brief Set file patterns. */
	void SetFilePatternList(const igdeFilePattern::List &list);
	
	/** \brief Update file type filter box. */
	void UpdateFileTypeBox();
	
	/** \brief Get result (B_OK or B_CANCEL). */
	inline int32 GetResult() const{ return pResult; }
	
	/** \brief Show modal dialog. */
	int32 ShowModal();
	
	/** \brief BWindow overrides */
	bool QuitRequested() override;
	void MessageReceived(BMessage *message) override;
	/*@}*/
	
private:
	void pCreateUI();
	void pUpdateFromDirectory();
	void pHandleOK();
	void pHandleCancel();
	void pHandleFileListDoubleClick();
	void pFindExistingDirectory(const decPath &path, decPath &directory);
};

#endif
