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
 * Speech Animation Phoneme.
 */
class ceSAPhoneme : public deObject{
private:
	int pIPA;
	float pLength;
	decString pMoveName;
	decString pVertexPositionSet;
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
	/** IPA symbol. */
	inline int GetIPA() const{ return pIPA; }
	
	/** Length of phoneme. */
	inline float GetLength() const{ return pLength; }
	
	/** Set length of phoneme. */
	void SetLength( float length );
	
	/** Move name. */
	inline const decString &GetMoveName() const{ return pMoveName; }
	
	/** Set move name. */
	void SetMoveName( const char *name );
	
	/** Vertex position set. */
	inline const decString &GetVertexPositionSet() const{ return pVertexPositionSet; }
	
	/** Set vertex position set. */
	void SetVertexPositionSet( const char *vertexPositionSet );
	
	/** Sample text. */
	inline const decString &GetSampleText() const{ return pSampleText; }
	
	/** Set sample text. */
	void SetSampleText( const char *sampleText );
	
	/** Engine controller or -1 if not used. */
	inline int GetEngineController() const{ return pEngController; }
	
	/** Set engine controller or -1 if not used. */
	void SetEngineController( int controller );
	/*@}*/
};

#endif
