/* 
 * Drag[en]gine OpenAL Audio Module
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

#ifndef _DEOALATCONTEXT_H_
#define _DEOALATCONTEXT_H_

#include <AL/al.h>
#include <AL/alc.h>

class deoalAudioThread;


/**
 * \brief Audio context.
 */
class deoalATContext{
private:
	deoalAudioThread &pAudioThread;
	
	ALCdevice *pDevice;
	ALCcontext *pContext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create audio context. */
	deoalATContext( deoalAudioThread &audioThread );
	
	/** \brief Clean up audio context. */
	~deoalATContext();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief OpenAL device. */
	inline ALCdevice *GetDevice() const{ return pDevice; }
	
	/** \brief OpenAL context. */
	inline ALCcontext *GetContext() const{ return pContext; }
	
	
	
	/** \brief Open device. */
	void OpenDevice();
	
	/** \brief Create context. */
	void CreateContext();
	
	/** \brief Log context information. */
	void LogContextInfo();
	
	/** \brief Clean up. */
	void CleanUp();
	/*@}*/
	
	
	
private:
	void pScanForDevices();
};

#endif
