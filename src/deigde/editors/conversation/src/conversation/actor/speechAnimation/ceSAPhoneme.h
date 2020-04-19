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

#ifndef _CESAPHONEME_H_
#define _CESAPHONEME_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief Speech Animation Phoneme.
 */
class ceSAPhoneme : public deObject{
private:
	int pIPA;
	float pLength;
	decString pMoveName;
	decString pSampleText;
	int pEngController;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new phoneme. */
	ceSAPhoneme( int ipa );
	/** Cleans up the phoneme. */
	virtual ~ceSAPhoneme();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the IPA symbol. */
	inline int GetIPA() const{ return pIPA; }
	/** Retrieves the length of the phoneme. */
	inline float GetLength() const{ return pLength; }
	/** Sets the length of the phoneme. */
	void SetLength( float length );
	/** Retrieves the move name. */
	inline const decString &GetMoveName() const{ return pMoveName; }
	/** Sets the move name. */
	void SetMoveName( const char *name );
	/** Retrieves the sample text. */
	inline const decString &GetSampleText() const{ return pSampleText; }
	/** Sets the sample text. */
	void SetSampleText( const char *sampleText );
	/** Retrieves the engine controller or -1 if not used. */
	inline int GetEngineController() const{ return pEngController; }
	/** Sets the engine controller or -1 if not used. */
	void SetEngineController( int controller );
	/*@}*/
};

#endif
