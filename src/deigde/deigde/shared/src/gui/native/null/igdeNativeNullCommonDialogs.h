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

#ifndef _IGDENATIVENULLCOMMONDIALOGS_H_
#define _IGDENATIVENULLCOMMONDIALOGS_H_

#include "../../igdeCommonDialogs.h"

#include <dragengine/common/string/decStringList.h>


class igdeFilePatternList;

class decString;
class deException;
class deVirtualFileSystem;


/**
 * \brief Null Common Dialogs.
 */
class igdeNativeNullCommonDialogs{
public:
	/** \name Management */
	/*@{*/
	/**
	 * \brief Shows message dialog.
	 */
	static igdeCommonDialogs::eButton Message( igdeWidget *owner,
		igdeCommonDialogs::eButtonSet buttons, igdeCommonDialogs::eIcon icon,
		const char *title, const char *text );
	
	
	
	/**
	 * \brief Request integer input from user.
	 * 
	 * Initial value is located in \em value. If the user accepts the input \em value
	 * is updated with the user input and \em true returned. If the user cancelled
	 * the input \em false is returned.
	 */
	static bool GetInteger( igdeWidget *owner, const char *title, const char *text, int &value );
	
	/**
	 * \brief Request decimal input from user.
	 * 
	 * Initial value is located in \em value. If the user accepts the input \em value
	 * is updated with the user input and \em true returned. If the user cancelled
	 * the input \em false is returned.
	 */
	static bool GetFloat( igdeWidget *owner, const char *title, const char *text, float &value );
	
	/**
	 * \brief Request string input from user.
	 * 
	 * Initial value is located in \em value. If the user accepts the input \em value
	 * is updated with the user input and \em true returned. If the user cancelled
	 * the input \em false is returned.
	 */
	static bool GetString( igdeWidget *owner, const char *title, const char *text,
		decString &value );
	
	/**
	 * \brief Request string selection input from user.
	 * 
	 * Initial selection is located in \em selection showing entries from \em list.
	 * If the user accepts the input \em selection is updated with the user selection
	 * and \em true returned. If the user cancelled the input \em false is returned.
	 */
	static bool SelectString( igdeWidget *owner, const char *title, const char *text,
		const decStringList &list, int &selection );
	
	
	
	/**
	 * \brief Request open file from user.
	 * 
	 * Initial value is located in \em value. If the user accepts the input \em value
	 * is updated with the user input and \em true returned. If the user cancelled
	 * the input \em false is returned.
	 */
	static bool GetFileOpen( igdeWidget *owner, const char *title,
		const igdeFilePatternList &filePatterns, decString &filename );
	
	/**
	 * \brief Request open file from user.
	 * 
	 * Initial value is located in \em value. If the user accepts the input \em value
	 * is updated with the user input and \em true returned. If the user cancelled
	 * the input \em false is returned.
	 */
	static bool GetFileOpen( igdeWidget *owner, const char *title, deVirtualFileSystem &vfs,
		const igdeFilePatternList &filePatterns, decString &filename );
	
	/**
	 * \brief Request save file from user.
	 * 
	 * Initial value is located in \em value. If the user accepts the input \em value
	 * is updated with the user input and \em true returned. If the user cancelled
	 * the input \em false is returned.
	 */
	static bool GetFileSave( igdeWidget *owner, const char *title,
		const igdeFilePatternList &filePatterns, decString &filename );
	
	/**
	 * \brief Request save file from user.
	 * 
	 * Initial value is located in \em value. If the user accepts the input \em value
	 * is updated with the user input and \em true returned. If the user cancelled
	 * the input \em false is returned.
	 */
	static bool GetFileSave( igdeWidget *owner, const char *title, deVirtualFileSystem &vfs,
		const igdeFilePatternList &filePatterns, decString &filename );
	
	/**
	 * \brief Request directory from user.
	 * 
	 * Initial value is located in \em value. If the user accepts the input \em value
	 * is updated with the user input and \em true returned. If the user cancelled
	 * the input \em false is returned.
	 */
	static bool GetDirectory( igdeWidget *owner, const char *title, decString &dirname );
	
	/**
	 * \brief Request directory from user.
	 * 
	 * Initial value is located in \em value. If the user accepts the input \em value
	 * is updated with the user input and \em true returned. If the user cancelled
	 * the input \em false is returned.
	 */
	static bool GetDirectory( igdeWidget *owner, const char *title, deVirtualFileSystem &vfs,
		decString &dirname );
	
	/**
	 * \brief Select system font.
	 * 
	 * Initial value is located in \em config. If the user accepts the input \em config
	 * is updated with the user input and \em true returned. If the user cancelled
	 * the input \em false is returned.
	 */
	static bool SelectSystemFont( igdeWidget *owner, const char *title,
		igdeFont::sConfiguration &config );
	/*@}*/
};

typedef igdeNativeNullCommonDialogs igdeNativeCommonDialogs;

#endif
