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

#ifndef _SAEWORD_H_
#define _SAEWORD_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

class saeSAnimation;



/**
 * Speech Animation Word.
 */
class saeWord : public deObject{
private:
	saeSAnimation *pSAnimation;
	
	decString pName;
	decUnicodeString pPhonetics;
	
	bool pActive;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create word. */
	saeWord();
	
	/** Create word. */
	saeWord( const char *name );
	
	/** Create word. */
	saeWord( const char *name, const decUnicodeString &phonetics );
	
	/** Cleans up the word. */
	virtual ~saeWord();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the parent speech animation. */
	inline saeSAnimation *GetSAnimation() const{ return pSAnimation; }
	/** Sets the parent speech animation. */
	void SetSAnimation( saeSAnimation *sanimation );
	
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the name. */
	void SetName( const char *name );
	/** Retrieves the phonetics. */
	inline const decUnicodeString &GetPhonetics() const{ return pPhonetics; }
	/** Sets the phonetics. */
	void SetPhonetics( const decUnicodeString &phonetics );
	/** Determines if this is the active word. */
	inline bool GetActive() const{ return pActive; }
	/** Sets if this is the active word. */
	void SetActive( bool active );
	/*@}*/
};

#endif
