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

#ifndef _CEPCBOPTION_H_
#define _CEPCBOPTION_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

class deCanvasView;
class cePlayerChoiceBox;
class deGraphicContext;
class ceCAPlayerChoice;
class ceCAPlayerChoiceOption;



/**
 * \brief Player Choice Box Option.
 */
class cePCBOption : public deObject{
private:
	decUnicodeString pText;
	ceCAPlayerChoice *pAction;
	ceCAPlayerChoiceOption *pActionOption;
	
	deCanvasView *pCanvasView;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create player choice box option. */
	cePCBOption();
	
	/** \brief Clean up player choice box option. */
	virtual ~cePCBOption();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Text. */
	inline const decUnicodeString &GetText() const{ return pText; }
	
	/** \brief Set text. */
	void SetText( const decUnicodeString &text );
	
	/** \brief Action. */
	inline ceCAPlayerChoice *GetAction() const{ return pAction; }
	
	/** \brief Action option. */
	inline ceCAPlayerChoiceOption *GetActionOption() const{ return pActionOption; }
	
	/** \brief Set action option. */
	void SetActionOption( ceCAPlayerChoice *action, ceCAPlayerChoiceOption *actionOption );
	
	/** \brief Canvas view. */
	inline deCanvasView *GetCanvasView() const{ return pCanvasView; }
	
	/** \brief Layout text. Create canvas if not existing. */
	void Layout( const cePlayerChoiceBox &pcbox, bool selected );
	/*@}*/
};

#endif
