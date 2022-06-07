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

#ifndef _IGDENATIVENULLTEXTAREA_H_
#define _IGDENATIVENULLTEXTAREA_H_

class igdeTextArea;


/**
 * Null Text Area.
 */
class igdeNativeNullTextArea{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create text widget. */
	igdeNativeNullTextArea();
	
	/** \brief Clean up text widget. */
	virtual ~igdeNativeNullTextArea();
	
	/** \brief Create native widget. */
	static igdeNativeNullTextArea* CreateNativeWidget( igdeTextArea &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	void UpdateStyles();
	void ApplyStyles();
	virtual void UpdateText();
	virtual void UpdateEnabled();
	virtual void UpdateDescription();
	virtual void UpdateEditable();
	virtual void Focus();
	virtual int GetCursorPosition() const;
	virtual void SetCursorPosition( int position );
	virtual int GetCursorColumn() const;
	virtual int GetCursorRow() const;
	virtual void SetCursorColumn( int column );
	virtual void SetCursorRow( int row );
	virtual int GetTopLine() const;
	virtual void SetTopLine( int line );
	virtual int GetBottomLine() const;
	virtual void SetBottomLine( int line );
	virtual int GetLineCount() const;
	virtual void UpdateColumns();
	virtual void UpdateRows();
	/*@}*/
};

typedef igdeNativeNullTextArea igdeNativeTextArea;

#endif
