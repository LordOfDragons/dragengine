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

#ifndef _SAEPHONEME_H_
#define _SAEPHONEME_H_

#include <dragengine/deObject.h>

#include <dragengine/common/string/decString.h>

class saeSAnimation;


/**
 * \brief Speech Animation Phoneme.
 */
class saePhoneme : public deObject{
private:
	saeSAnimation *pSAnimation;
	int pEngController;
	
	int pIPA;
	decString pSampleText;
	decString pMoveName;
	float pLength;
	bool pActive;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create phoneme. */
	saePhoneme();
	
	/** \brief Create phoneme. */
	saePhoneme( int ipa );
	
	/** Cleans up the phoneme. */
	virtual ~saePhoneme();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the parent speech animation. */
	inline saeSAnimation *GetSAnimation() const{ return pSAnimation; }
	/** Sets the parent speech animation. */
	void SetSAnimation( saeSAnimation *sanimation );
	/** Retrieves the engine controller or -1 if not used. */
	inline int GetEngineController() const{ return pEngController; }
	/** Sets the engine controller or -1 if not used. */
	void SetEngineController( int controller );
	
	/** Retrieves the IPA symbol. */
	inline int GetIPA() const{ return pIPA; }
	/** Sets the IPA symbol. */
	void SetIPA( int ipa );
	/** Retrieves the length of the phoneme. */
	inline float GetLength() const{ return pLength; }
	/** Sets the length of the phoneme. */
	void SetLength( float length );
	/** Retrieves the sample tzetext. */
	inline const decString &GetSampleText() const{ return pSampleText; }
	/** Sets the sample text. */
	void SetSampleText( const char *sampleText );
	/** Retrieves the move name. */
	inline const decString &GetMoveName() const{ return pMoveName; }
	/** Sets the move name. */
	void SetMoveName( const char *name );
	/** Determines if this is the active phoneme. */
	inline bool GetActive() const{ return pActive; }
	/** Sets if this is the active phoneme. */
	void SetActive( bool active );
	/*@}*/
};

#endif
