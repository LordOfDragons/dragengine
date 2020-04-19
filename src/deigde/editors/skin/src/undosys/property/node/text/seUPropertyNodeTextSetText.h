/* 
 * Drag[en]gine IGDE Skin Editor
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

#ifndef _SEUPROPERTYNODETEXTSETTEXT_H_
#define _SEUPROPERTYNODETEXTSETTEXT_H_

#include <deigde/undo/igdeUndo.h>

class sePropertyNodeText;



/**
 * \brief Undo action property node text set text.
 */
class seUPropertyNodeTextSetText : public igdeUndo{
private:
	sePropertyNodeText *pNode;
	
	decString pOldValue;
	decString pNewValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	seUPropertyNodeTextSetText( sePropertyNodeText *node, const char *newValue );
	
protected:
	/** \brief Clean up undo. */
	virtual ~seUPropertyNodeTextSetText();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Set text. */
	void SetNewValue( const char *text );
	
	/** \brief Undo action. */
	virtual void Undo();
	
	/** \brief Redo action. */
	virtual void Redo();
	/*@}*/
};

#endif
