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

#ifndef _DEBASESYNTHESIZERSYNTHESIZERINSTANCE_H_
#define _DEBASESYNTHESIZERSYNTHESIZERINSTANCE_H_

#include "../../../dragengine_export.h"


/**
 * \brief Synthesizer module synthesizer instance peer.
 */
class DE_DLL_EXPORT deBaseSynthesizerSynthesizerInstance{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deBaseSynthesizerSynthesizerInstance();
	
	/** \brief Clean up peer. */
	virtual ~deBaseSynthesizerSynthesizerInstance();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Synthesizer changed. */
	virtual void SynthesizerChanged();
	
	/** \brief Controller changed. */
	virtual void ControllerChanged( int index );
	
	/** \brief Play time changed. */
	virtual void PlayTimeChanged();
	/*@}*/
	
	
	
	/** \name Generate sound */
	/*@{*/
	/**
	 * \brief Reset synthesizer playback.
	 * 
	 * \warning Audio module is allowed to call this asynchronously.
	 */
	virtual void Reset() = 0;
	
	/**
	 * \brief Generate sound.
	 * 
	 * \warning Audio module is allowed to call this asynchronously.
	 * 
	 * \param[out] buffer Buffer to store samples in.
	 * \param[in] bufferSize Size of buffer in bytes to store samples in. Has to match format.
	 * \param[in] offset Offset in samples to produce sound at.
	 * \param[in] samples Number of samples to produce.
	 * 
	 * \throws EInvalidParam \em bufferSize does not match format.
	 * \throws EInvalidParam \em buffer is NULL.
	 * \throws EInvalidParam Assigned synthesizer object changed while in use.
	 */
	virtual void GenerateSound( void *buffer, int bufferSize, int offset, int samples ) = 0;
	/*@}*/
};

#endif
