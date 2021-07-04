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

#ifndef _IGDECOMMONDIALOGS_H_
#define _IGDECOMMONDIALOGS_H_


#include "resources/igdeFont.h"

#include <dragengine/common/string/decStringList.h>


class igdeEditorModule;
class igdeWidget;
class igdeFilePatternList;

class decString;
class deException;
class deVirtualFileSystem;


/**
 * \brief IGDE UI Common Dialogs.
 * 
 * Provides support to create common dialogs for requesting input from the user.
 */
class igdeCommonDialogs{
public:
	/** \brief Buttons to show. */
	enum eButtonSet{
		/** \brief OK. */
		ebsOk,
		
		/** \brief OK, Cancel. */
		ebsOkCancel,
		
		/** \brief Yes, No. */
		ebsYesNo,
		
		/** \brief Yes, No, Cancel. */
		ebsYesNoCancel
	};
	
	/** \brief Button. */
	enum eButton{
		/** \brief OK. */
		ebOk,
		
		/** \brief Cancel. */
		ebCancel,
		
		/** \brief Yes. */
		ebYes,
		
		/** \brief No. */
		ebNo
	};
	
	/** \brief Icon to show. */
	enum eIcon {
		/** \brief No icon. */
		eiNone,
		
		/** \brief Information. */
		eiInfo,
		
		/** \brief Question. */
		eiQuestion,
		
		/** \brief Warning. */
		eiWarning,
		
		/** \brief Error. */
		eiError
	};
	
	
	
private:
	igdeEditorModule &pEditorModule;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create common dialog builder. */
	igdeCommonDialogs( igdeEditorModule &editorModule );
	
	/** \brief Clean up common dialog builder. */
	~igdeCommonDialogs();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Editor module. */
	inline igdeEditorModule &GetEditorModule() const{ return pEditorModule; }
	
	
	
	/**
	 * \brief Shows information dialog with OK button.
	 */
	static void Information( igdeWidget *owner, const char *title, const char *text );
	
	/**
	 * \brief Shows information dialog with OK button and formatted text.
	 */
	static void InformationFormat( igdeWidget *owner, const char *title,
		const char *textFormat, ... )
		#ifdef __GNUC__
		__attribute__ ((format (printf, 3, 4)))
		#endif
		;
	
	/**
	 * \brief Shows question dialog.
	 */
	static eButton Question( igdeWidget *owner, eButtonSet buttons,
		const char *title, const char *text );
	
	/**
	 * \brief Shows question dialog with formatted text.
	 */
	static eButton QuestionFormat( igdeWidget *owner, eButtonSet buttons,
		const char *title, const char *textFormat, ... )
		#ifdef __GNUC__
		__attribute__ ((format (printf, 4, 5)))
		#endif
		;
	
	/**
	 * \brief Shows warning dialog with OK button.
	 */
	static void Warning( igdeWidget *owner, const char *title, const char *text );
	
	/**
	 * \brief Shows warning dialog with OK button and formatted text.
	 */
	static void WarningFormat( igdeWidget *owner, const char *title, const char *textFormat, ... )
		#ifdef __GNUC__
		__attribute__ ((format (printf, 3, 4)))
		#endif
		;
	
	/**
	 * \brief Shows error dialog with OK button.
	 */
	static void Error( igdeWidget *owner, const char *title, const char *text );
	
	/**
	 * \brief Shows error dialog with OK button and formatted text.
	 */
	static void ErrorFormat( igdeWidget *owner, const char *title, const char *textFormat, ... )
		#ifdef __GNUC__
		__attribute__ ((format (printf, 3, 4)))
		#endif
		;
	
	
	
	/**
	 * \brief Shows message dialog.
	 */
	static eButton Message( igdeWidget *owner, eButtonSet buttons, eIcon icon,
		const char *title, const char *text );
	
	/**
	 * \brief Shows message dialog with formatted text.
	 */
	static eButton MessageFormat( igdeWidget *owner, eButtonSet buttons, eIcon icon,
		const char *title, const char *textFormat, ... )
		#ifdef __GNUC__
		__attribute__ ((format (printf, 5, 6)))
		#endif
		;
	
	
	
	/** \brief Format exception for display or logging purpose. */
	static decString FormatException( const deException &exception );
	
	/** \brief Display exception to user. */
	static void Exception( igdeWidget *owner, const deException &exception );
	
	
	
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
	 * \brief Request multiline string input from user.
	 * 
	 * Initial value is located in \em value. If the user accepts the input \em value
	 * is updated with the user input and \em true returned. If the user cancelled
	 * the input \em false is returned.
	 */
	static bool GetMultilineString( igdeWidget *owner, const char *title, const char *text,
		decString &value );
	
	/**
	 * \brief Request string input from user with proposed values.
	 * 
	 * Initial value is located in \em value. If the user accepts the input \em value
	 * is updated with the user input and \em true returned. If the user cancelled
	 * the input \em false is returned.
	 */
	static bool GetString( igdeWidget *owner, const char *title, const char *text,
		decString &value, const decStringList &proposals );
	
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
	static bool SelectSystemFont( igdeWidget *owner, const char *title, igdeFont::sConfiguration &config );
	/*@}*/
};

#endif
