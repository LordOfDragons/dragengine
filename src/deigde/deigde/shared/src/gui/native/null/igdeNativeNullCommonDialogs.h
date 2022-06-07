/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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
