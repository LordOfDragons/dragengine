/* 
 * Drag[en]gine IGDE Speech Animation Editor
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

#ifndef _SAEUWORDADDLIST_H_
#define _SAEUWORDADDLIST_H_

#include "../../../sanimation/dictionary/saeWordList.h"

#include <deigde/undo/igdeUndo.h>


class saeSAnimation;



/**
 * Undo action add word list.
 */
class saeUWordAddList : public igdeUndo{
private:
	saeSAnimation *pSAnimation;
	saeWordList pWords;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create undo. */
	saeUWordAddList( saeSAnimation *sanimation, const saeWordList &words );
	
protected:
	/** Clean up undo. */
	virtual ~saeUWordAddList();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Undo. */
	virtual void Undo();
	
	/** Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
