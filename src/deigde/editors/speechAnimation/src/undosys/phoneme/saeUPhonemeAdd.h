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

#ifndef _SAEUPHONEMEADD_H_
#define _SAEUPHONEMEADD_H_

#include <deigde/undo/igdeUndo.h>

class saePhoneme;
class saeSAnimation;



/**
 * Undo Action Add Phoneme.
 */
class saeUPhonemeAdd : public igdeUndo{
private:
	saeSAnimation *pSAnimation;
	saePhoneme *pPhoneme;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create undo. */
	saeUPhonemeAdd( saeSAnimation *sanimation, saePhoneme *phoneme );
	
protected:
	/** Clean up undo. */
	virtual ~saeUPhonemeAdd();
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
