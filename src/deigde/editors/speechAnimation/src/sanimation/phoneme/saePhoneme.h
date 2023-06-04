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
 * Speech Animation Phoneme.
 */
class saePhoneme : public deObject{
private:
	saeSAnimation *pSAnimation;
	int pEngController;
	
	int pIPA;
	decString pSampleText;
	decString pMoveName;
	decString pVertexPositionSet;
	float pLength;
	bool pActive;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create phoneme. */
	saePhoneme();
	
	/** Create phoneme. */
	saePhoneme( int ipa );
	
protected:
	/** Clean up phoneme. */
	virtual ~saePhoneme();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Parent speech animation. */
	inline saeSAnimation *GetSAnimation() const{ return pSAnimation; }
	
	/** Set parent speech animation. */
	void SetSAnimation( saeSAnimation *sanimation );
	
	/** Engine controller or -1 if not used. */
	inline int GetEngineController() const{ return pEngController; }
	
	/** Set engine controller or -1 if not used. */
	void SetEngineController( int controller );
	
	/** IPA symbol. */
	inline int GetIPA() const{ return pIPA; }
	
	/** Set IPA symbol. */
	void SetIPA( int ipa );
	
	/** Length of phoneme. */
	inline float GetLength() const{ return pLength; }
	
	/** Set length of phoneme. */
	void SetLength( float length );
	
	/** Sample text. */
	inline const decString &GetSampleText() const{ return pSampleText; }
	
	/** Set sample text. */
	void SetSampleText( const char *sampleText );
	
	/** Move name. */
	inline const decString &GetMoveName() const{ return pMoveName; }
	
	/** Set move name. */
	void SetMoveName( const char *name );
	
	/** Vertex position set. */
	inline const decString &GetVertexPositionSet() const{ return pVertexPositionSet; }
	
	/** Set vertex position set. */
	void SetVertexPositionSet( const char *name );
	
	/** Active phoneme. */
	inline bool GetActive() const{ return pActive; }
	
	/** Set if active phoneme. */
	void SetActive( bool active );
	/*@}*/
};

#endif
