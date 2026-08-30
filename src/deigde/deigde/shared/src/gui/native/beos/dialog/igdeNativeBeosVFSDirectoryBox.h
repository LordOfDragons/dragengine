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

#ifndef _IGDENATIVEBEOSVFSDIRECTORYBOX_H_
#define _IGDENATIVEBEOSVFSDIRECTORYBOX_H_

#include <interface/View.h>
#include <interface/TextControl.h>
#include <interface/Button.h>
#include <MenuField.h>

#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decString.h>

class deVirtualFileSystem;


/**
 * \brief Directory path editor with browse functionality.
 */
class igdeNativeBeosVFSDirectoryBox : public BView{
private:
	deVirtualFileSystem *pVFS;
	BTextControl *pTextControl;
	BButton *pBrowseButton;
	decPath pPath;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new VFS directory box. */
	igdeNativeBeosVFSDirectoryBox(deVirtualFileSystem *vfs, BRect frame, const char *name);
	
	/** \brief Cleans up the VFS directory box. */
	~igdeNativeBeosVFSDirectoryBox() override;
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Get the virtual file system. */
	inline deVirtualFileSystem *GetVirtualFileSystem() const{ return pVFS; }
	
	/** \brief Get the text control. */
	inline BTextControl *GetTextControl() const{ return pTextControl; }
	
	/** \brief Get the browse button. */
	inline BButton *GetBrowseButton() const{ return pBrowseButton; }
	
	/** \brief Get the current path. */
	inline const decPath &GetPath() const{ return pPath; }
	
	/** \brief Set the path. */
	void SetPath(const decPath &path);
	
	/** \brief Get the text representation of the path. */
	decString GetPathText() const;
	
	/** \brief Set path from text. */
	void SetPathFromText();
	
	/** \brief BView overrides */
	void AttachedToWindow() override;
	void MessageReceived(BMessage *message) override;
	void ResizeToPreferred() override;
	/*@}*/
};

#endif
