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

#ifndef _IGDENATIVEBEOSCOMMONDIALOGS_H_
#define _IGDENATIVEBEOSCOMMONDIALOGS_H_

#include "../../igdeCommonDialogs.h"
#include "../../../gui/filedialog/igdeFilePattern.h"

class igdeWidget;
class igdeFont;

class decString;
class decStringList;
class deException;
class deVirtualFileSystem;

/**
 * BeOS/Haiku common dialogs.
 */
class igdeNativeBeosCommonDialogs{
public:
	igdeNativeBeosCommonDialogs();
	virtual ~igdeNativeBeosCommonDialogs();
	
	static void *CreateNativeWidget(igdeCommonDialogs &owner);
	
	// Message dialogs
	static igdeCommonDialogs::eButton Message(igdeWidget &owner,
		igdeCommonDialogs::eButtonSet buttons, igdeCommonDialogs::eIcon icon,
		const char *title, const char *text);
	
	static void FatalError(const char *title, const char *text);
	
	// Input dialogs
	static bool GetInteger(igdeWidget &owner, const char *title,
		const char *text, int &value);
	
	static bool GetFloat(igdeWidget &owner, const char *title,
		const char *text, float &value);
	
	static bool GetString(igdeWidget &owner, const char *title,
		const char *text, decString &value);
	
	static bool GetString(igdeWidget &owner, const char *title,
		const char *text, decString &value, const decStringList &proposals);
	
	static bool GetMultilineString(igdeWidget &owner, const char *title,
		const char *text, decString &value);
	
	static bool SelectString(igdeWidget &owner, const char *title,
		const char *text, const decStringList &list, int &selection);
	
	// File dialogs
	static bool GetFileOpen(igdeWidget &owner, const char *title,
		const igdeFilePattern::List &filePatterns, decString &filename);
	
	static bool GetFileOpen(igdeWidget &owner, const char *title,
		deVirtualFileSystem &vfs, const igdeFilePattern::List &filePatterns, decString &filename);
	
	static bool GetFileSave(igdeWidget &owner, const char *title,
		const igdeFilePattern::List &filePatterns, decString &filename);
	
	static bool GetFileSave(igdeWidget &owner, const char *title,
		deVirtualFileSystem &vfs, const igdeFilePattern::List &filePatterns, decString &filename);
	
	// Directory dialogs
	static bool GetDirectory(igdeWidget &owner, const char *title, decString &dirname);
	
	static bool GetDirectory(igdeWidget &owner, const char *title,
		deVirtualFileSystem &vfs, decString &dirname);
	
	// Font dialogs
	static bool SelectSystemFont(igdeWidget &owner, const char *title,
		igdeFont::sConfiguration &config);
};

typedef igdeNativeBeosCommonDialogs igdeNativeCommonDialogs;

#endif
