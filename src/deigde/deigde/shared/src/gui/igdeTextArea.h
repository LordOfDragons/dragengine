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

#ifndef _IGDETEXTAREA_H_
#define _IGDETEXTAREA_H_

#include <stdlib.h>

#include "igdeWidget.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>


class igdeTextAreaListener;
class igdeTextStyle;
class igdeTextSegment;
class igdeAction;



/**
 * \brief IGDE UI TextArea.
 * 
 * Text area support adding text segments with individual style and optional click action.
 * To use segments add first the text styles to use. Then add text using named styles and
 * optionally an action. If SetText() is called all segment information is discarded.
 * AppendText() is safe to be used.
 */
class DE_DLL_EXPORT igdeTextArea : public igdeWidget{
private:
	bool pEnabled;
	decString pText;
	int pColumns;
	int pRows;
	bool pEditable;
	decString pDescription;
	
	decObjectOrderedSet pStyles;
	decObjectOrderedSet pSegments;
	
	decObjectOrderedSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create textarea. */
	igdeTextArea( igdeEnvironment &environment, int columns, int rows,
		const char *description = "" );
	
	igdeTextArea( igdeEnvironment &environment, int columns, int rows,
		bool editable, const char *description = "" );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeTextArea();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief TextArea is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if button is enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Visible columns in text area. */
	inline int GetColumns() const{ return pColumns; }
	
	/** \brief Set visible columns in text area. */
	void SetColumns( int columns );
	
	/** \brief Visible rows in text area. */
	inline int GetRows() const{ return pRows; }
	
	/** \brief Set visible rows in text area. */
	void SetRows( int rows );
	
	/** \brief Text is editable. */
	inline bool GetEditable() const{ return pEditable; }
	
	/** \brief Set if text is editable. */
	void SetEditable( bool editable );
	
	/** \brief Description shown in tool tips. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description shown in tool tips. */
	void SetDescription( const char *description );
	
	
	
	/** \brief Number of styles. */
	int GetStyleCount() const;
	
	/** \brief Get style at index. */
	igdeTextStyle *GetStyleAt( int index ) const;
	
	/** \brief Get named style or NULL if absent. */
	igdeTextStyle *GetStyleNamed( const char *name ) const;
	
	/** \brief Index of named style or -1 if absent. */
	int IndexOfStyleNamed( const char *name ) const;
	
	/** \brief Add style. */
	void AddStyle( igdeTextStyle *style );
	
	/** \brief Remove all styles. */
	void RemoveAllStyles();
	
	/** \brief Styles changed. */
	void StylesChanged();
	
	
	
	/** \brief Text. */
	inline const decString &GetText() const{ return pText; }
	
	/**
	 * \brief Set text.
	 * \note Removes all text segment information.
	 */
	void SetText( const char *text );
	
	/** \brief Append text. */
	void AppendText( const char *text );
	
	/** \brief Append text with named style. */
	void AppendText( const char *text, const char *style );
	
	/** \brief Append text with named style and action. */
	void AppendText( const char *text, const char *style, igdeAction *action );
	
	/**
	 * \brief Delete text.
	 * \param[in] begin Offset to first character to delete.
	 * \param[in] end Offset to character after the last character to delete.
	 */
	void DeleteText( int begin, int end );
	
	/**
	 * \brief Clear text.
	 * \note Removes all text segment information.
	 */
	void ClearText();
	
	
	
	/** \brief Cursor position as offset from start of text. */
	int GetCursorPosition() const;
	
	/** \brief Set cursor position as offset from start of text. */
	void SetCursorPosition( int position );
	
	/** \brief Cursor coordinate as point with column and line. */
	decPoint GetCursorCoordinate() const;
	
	/** \brief Set cursor coordinate as point with column and line. */
	void SetCursorCoordinate( const decPoint &coordinate );
	
	/** \brief Top line. */
	int GetTopLine() const;
	
	/** \brief Set top line. */
	void SetTopLine( int line );
	
	/** \brief Bottom line. */
	int GetBottomLine() const;
	
	/** \brief Set bottom line. */
	void SetBottomLine( int line );
	
	/** \brief Number of lines. */
	int GetLineCount() const;
	
	/** \brief Focus widget. */
	void Focus();
	
	
	
	/** \brief Number of segments. */
	int GetSegmentCount() const;
	
	/** \brief Get segment at index. */
	const igdeTextSegment &GetSegmentAt( int index ) const;
	
	/** \brief Segment containing position or NULL if not found. */
	const igdeTextSegment *GetSegmentWith( int offset ) const;
	
	/**
	 * \brief Set style of text segment.
	 * 
	 * Segments fully contained inside this segment at removed. Segments overlapping
	 * this segment are truncated.
	 * 
	 * \param[in] begin Offset to first character.
	 * \param[in] end Offset to last character.
	 * \param[in] style Name of style.
	 * \param[in] action Action to use on clicking or NULL.
	 */
	void SetTextSegment( int begin, int end, const char *style, igdeAction *action );
	
	/**
	 * \brief Clear style of text segment.
	 * 
	 * Segments fully contained inside this segment at removed. Segments overlapping
	 * this segment are truncated.
	 * 
	 * \param[in] begin Offset to first character.
	 * \param[in] end Offset to last character.
	 */
	void ClearTextSegment( int begin, int end );
	
	
	
	/** \brief Add listener. */
	void AddListener( igdeTextAreaListener *listener );
	
	/** \brief Remove listener. */
	void RemoveListener( igdeTextAreaListener *listener );
	
	/** \brief Notify listeners text changed. */
	virtual void NotifyTextChanged();
	
	/** \brief Notify listeners text is changing. */
	virtual void NotifyTextChanging();
	/*@}*/
	
	
	
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/**
	 * \brief Create native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void CreateNativeWidget();
	
	/**
	 * \brief Destroy native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void DestroyNativeWidget();
	
	
	
protected:
	/** \brief Text changed. */
	virtual void OnTextChanged();
	
	/** \brief Columns changed. */
	virtual void OnColumnsChanged();
	
	/** \brief Rows changed. */
	virtual void OnRowsChanged();
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	
	/** \brief Editable changed. */
	virtual void OnEditableChanged();
	
	/** \brief Description changed. */
	virtual void OnDescriptionChanged();
	
	/** \brief Styles changed. */
	virtual void OnStylesChanged();
	
	/**
	 * \brief Clear segment without sending notifications.
	 * \returns \em true if anything change otherwise \em false.
	 */
	bool pClearSegment( int begin, int end );
	/*@}*/
};

#endif
