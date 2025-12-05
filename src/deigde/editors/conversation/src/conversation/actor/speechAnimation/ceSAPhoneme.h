/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceSAPhoneme> Ref;


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
