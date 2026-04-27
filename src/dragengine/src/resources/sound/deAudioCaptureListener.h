/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEAUDIOCAPTURELISTENER_H_
#define _DEAUDIOCAPTURELISTENER_H_

#include "../../deObject.h"


/**
 * \brief Audio capture listener.
 * \version 1.32
 */
class DE_DLL_EXPORT deAudioCaptureListener : public deObject{
public:
	/** \brief Type holding strong reference. */
	using Ref = deTObjectReference<deAudioCaptureListener>;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create audio capture listener. */
	deAudioCaptureListener();
	
protected:
	/**
	 * \brief Clean up audio capture listener.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deAudioCaptureListener() override;
	/*@}*/
	
	
	
public:
	/** \name Notifications */
	/*@{*/
	/**
	 * \brief Audio capture state changed.
	 *
	 * Send after Start() or Stop() has been called. If deAudioSystem::IsCapturingAudio()
	 * returns true retrieve capture format using deAudioSystem::GetAudioCaptureFormat().
	 * After all listeners have been notified SamplesCaptured() can be called with the
	 * format returned by deAudioSystem::GetAudioCaptureFormat().
	 */
	virtual void StateChanged();
	
	/**
	 * \brief Samples captured.
	 *
	 * Send after new samples have arrived. \em count is the count of samples in the buffer.
	 * Buffer \em samples contains "count * sampleRate * (bitRate / 8)" bytes.
	 * 
	 * Listeners must not block inside this function call to do processing to retain high
	 * performance. Instead listeners must copy the data aside. Furthermore the buffer is
	 * only valid during the function call.
	 * 
	 * \warning Potentially called asynchronously.
	 */
	virtual void SamplesCaptured(const void *samples, int count);
	/*@}*/
};

#endif
