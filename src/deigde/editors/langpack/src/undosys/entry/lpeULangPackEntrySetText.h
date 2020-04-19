/* 
 * Drag[en]gine IGDE Language Pack Editor
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

#ifndef _LPEULANGPACKENTRYSETTEXT_H_
#define _LPEULANGPACKENTRYSETTEXT_H_

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/string/unicode/decUnicodeString.h>

class lpeLangPackEntry;



/**
 * \brief Undo action set language pack entry text.
 */
class lpeULangPackEntrySetText : public igdeUndo{
private:
	lpeLangPackEntry *pEntry;
	
	decUnicodeString pOldText;
	decUnicodeString pNewText;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	lpeULangPackEntrySetText( lpeLangPackEntry *entry, const decUnicodeString &newText );
	
protected:
	/** \brief Clean up undo. */
	virtual ~lpeULangPackEntrySetText();
	/*@}*/
	
	
	
public:
	/** \description Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
