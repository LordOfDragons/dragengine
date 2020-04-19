/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CETEXTBOXTEXT_H_
#define _CETEXTBOXTEXT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

class deCanvasView;
class ceTextBox;
class deGraphicContext;



/**
 * \brief Text Box Text.
 */
class ceTextBoxText : public deObject{
private:
	decUnicodeString pName;
	decUnicodeString pText;
	
	deCanvasView *pCanvasView;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create text box text. */
	ceTextBoxText();
	
	/** \brief Clean up text box text. */
	virtual ~ceTextBoxText();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decUnicodeString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const decUnicodeString &name );
	
	/** \brief Text. */
	inline const decUnicodeString &GetText() const{ return pText; }
	
	/** \brief Set text. */
	void SetText( const decUnicodeString &text );
	
	/** \brief Canvas view. */
	inline deCanvasView *GetCanvasView() const{ return pCanvasView; }
	
	/** \brief Layout text. Create canvas if not existing. */
	void Layout( const ceTextBox &textBox );
	/*@}*/
};

#endif
