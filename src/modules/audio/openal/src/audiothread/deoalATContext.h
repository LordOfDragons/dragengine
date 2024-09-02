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
