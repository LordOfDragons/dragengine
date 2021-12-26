/* 
 * Drag[en]gine Game Engine
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
