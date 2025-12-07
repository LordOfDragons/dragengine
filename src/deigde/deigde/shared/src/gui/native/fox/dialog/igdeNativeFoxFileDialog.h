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

#ifndef _IGDENATIVEFOXFILEDIALOG_H_
#define _IGDENATIVEFOXFILEDIALOG_H_

#include "../foxtoolkit.h"


class igdeEnvironment;
class deVirtualFileSystem;
class igdeNativeFoxVFSList;
class igdeNativeFoxVFSDirectoryBox;
class igdeFilePatternList;
class dePatternList;
class decPath;
class igdeWidget;



/**
 * 
 * Provides a very simple file dialog geared to work with the virtual file
 * system instead of the true file system. The dialog uses the file patterns
 * found in the assigned file pattern list. The dialog only uses the list
 * but does not allocate or free the list. This is the responsability of the
 * calling application. This way a list can be build and reused.
 */
class igdeNativeFoxFileDialog : public FXDialogBox{
	FXDECLARE(igdeNativeFoxFileDialog)
	
protected:
	igdeNativeFoxFileDialog();
	
public:
	enum eFoxIDs{
		ID_CANVAS = FXDialogBox::ID_LAST,
		ID_CBDIRECTORY,
		ID_BTNUPDIR,
		ID_BTNGOROOT,
		ID_BTNMKDIR,
		ID_BTNLISTMODE,
		ID_BTNICONMODE,
		ID_BTNDETAILMODE,
		ID_FILELIST,
		ID_CBFILTER,
		ID_LAST
	};
private:
	igdeWidget *pOwner;
	deVirtualFileSystem *pVFS;
	
	FXIcon *pIconUpDir;
	FXIcon *pIconListMode;
	FXIcon *pIconDetailMode;
	FXIcon *pIconIconMode;
	FXIcon *pIconGoRoot;
	FXIcon *pIconMkdir;
	FXIcon *pIconDelete;
	FXIcon *pIconMove;
	FXIcon *pIconCopy;
	FXIcon *pIconLink;
	
	igdeNativeFoxVFSList *pList;
	FXTextField *pEditFilename;
	igdeNativeFoxVFSDirectoryBox *pCBDirectory;
	FXComboBox *pCBFilter;
	FXButton *pBtnAccept;
	
	const igdeFilePatternList *pFilePatternList;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create file dialog using the game engine virtual file system.
	 * \details If you need the IGDE virtual file system use the constructor call
	 *          where you can specify the virtual file system on your own.
	 */
	igdeNativeFoxFileDialog(igdeWidget &owner, FXComposite *parent, const FXString& name,
		FXuint opts=0, FXint x=0, FXint y=0, FXint w=500, FXint h=300);
	
	/** \brief Create file dialog with specific virtual file system. */
	igdeNativeFoxFileDialog(igdeWidget &owner, deVirtualFileSystem *vfs, FXComposite *parent,
		const FXString& name, FXuint opts=0, FXint x=0, FXint y=0, FXint w=500, FXint h=300);
	
	/** Cleans up the file fialog. */
	virtual ~igdeNativeFoxFileDialog();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the virtual file system. */
	inline deVirtualFileSystem *GetVirtualFileSystem() const{ return pVFS; }
	
	/** Retrieves the filename. */
	FXString GetFilename() const;
	
	/**
	 * \brief Set filename.
	 * \note Filename has to be an absolute path.
	 */
	void SetFilename(const char *filename);
	
	/**
	 * \brief Set filename with base path.
	 * 
	 * If filename is absolute it is set directly. Otherwise the filename is appended to
	 * the base path to form an absolute path. Helper method for editors using relative
	 * file path since virtual file systems do not have a current working directory.
	 */
	void SetFilename(const char *filename, const char *basePath);
	
	/** Retrieves the assigned file pattern list.*/
	inline const igdeFilePatternList *GetFilePatternList() const{ return pFilePatternList; }
	/** Sets the assigned file pattern list. */
	void SetFilePatternList(const igdeFilePatternList *filePatternList);
	
	/** Updates the file type combo box. */
	void UpdateFileTypeBox();
	
	/** Hide the window. */
	virtual void hide();
	/*@}*/
	
	/** \name Events */
	/*@{*/
	long onCmdAccept(FXObject*, FXSelector, void*);
	
	long onBtnUpDir(FXObject*, FXSelector, void*);
	long onBtnGoRoot(FXObject*, FXSelector, void*);
	long onBtnMkDir(FXObject*, FXSelector, void*);
	long onBtnListMode(FXObject*, FXSelector, void*);
	long onBtnIconMode(FXObject*, FXSelector, void*);
	long onBtnDetailMode(FXObject*, FXSelector, void*);
	long updateBtnUpDir(FXObject*, FXSelector, void*);
	long updateBtnGoRoot(FXObject*, FXSelector, void*);
	long updateBtnMkDir(FXObject*, FXSelector, void*);
	long updateBtnListMode(FXObject*, FXSelector, void*);
	long updateBtnIconMode(FXObject*, FXSelector, void*);
	long updateBtnDetailMode(FXObject*, FXSelector, void*);
	
	long onCBDirectoryChanged(FXObject*, FXSelector, void*);
	long onCBFilterChanged(FXObject*, FXSelector, void*);
	
	long onFLItemDoubleClicked(FXObject*, FXSelector, void*);
	long onFLRightMouseUp(FXObject*, FXSelector, void*);
	long onFLItemSelected(FXObject*, FXSelector, void*);
	long onFLItemDeselected(FXObject*, FXSelector, void*);
	/*@}*/
	
private:
	void pCreateDialog();
	void pFindExistingDirectory(decPath &path, decPath &directory);
	void pSetFilenameAndAppendExtension(const char *filename);
};

#endif
