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

#ifndef _DESYNTHESIZERINSTANCE_H_
#define _DESYNTHESIZERINSTANCE_H_

#include "deSynthesizer.h"
#include "source/deSynthesizerSource.h"
#include "../deResource.h"
#include "../../common/collection/decObjectOrderedSet.h"

class deSynthesizerInstanceManager;
class deSynthesizerController;
class deBaseSynthesizerSynthesizerInstance;
class deBaseAudioSynthesizerInstance;


/**
 * \brief Synthesizer instance producing source using a synthesizer.
 * 
 * Every synthesizer contains a set of sources and controllers indicating how the final sound
 * is produced. While synthesizers define this production process the synthesizer instance
 * produces the actual sound using per instance parameters.
 * 
 * \par Module interaction
 * 
 * Synthesizer instance has module interaction between synthesizer and audio module.
 * 
 * The audio module calls GenerateSound() to obtain continuous sound to play. The synthesizer
 * module will update internal states if required whenever GenerateSound() is called.
 * 
 * Because this requires the audio module and the synthesizer module to use the same sound
 * data format it is not allowed for the user to change the synthesizer object while it is
 * in use. It is thus correct for the audio module to assume the synthesizer format parameters
 * (bytes per sample, channel count and sample rate) to stay constant as long as the synthesizer
 * is assigned. Inconsistencies created by the user incorrectly manipulating the synthesizer
 * object while it is in use has to be catched by the synthesizer module throwing an exception.
 * 
 * The audio module can call Reset() on the synthesizer instance any time it wants to reset
 * playback.
 * 
 * To allow performant synthesizing of sound the audio and synthesizer module are allowed to
 * run their work asynchronously. In particular this means GenerateSound() is allowed to be
 * called asynchronously while any manipulation of the synthesizer instance states (like
 * changing sample count or controller curves) is allowed only by the main thread.
 * The synthesizer module has to be prepared to deal with this asynchronous handling safely.
 * 
 * Synthesizers can not be shared. If done so problems will most likely happen.
 */
class DE_DLL_EXPORT deSynthesizerInstance : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deSynthesizerInstance> Ref;
	
	
private:
	deSynthesizer::Ref pSynthesizer;
	
	decObjectOrderedSet pControllers;
	
	int pSampleCount;
	
	deBaseSynthesizerSynthesizerInstance *pPeerSynthesizer;
	deBaseAudioSynthesizerInstance *pPeerAudio;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer instance. */
	deSynthesizerInstance(deSynthesizerInstanceManager *manager);
	
protected:
	/**
	 * \brief Clean up the synthesizer instance.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deSynthesizerInstance();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Synthesizer or NULL if not set. */
	inline deSynthesizer *GetSynthesizer() const{return pSynthesizer;}
	
	/** \brief Set synthesizer or NULL to clear. */
	void SetSynthesizer(deSynthesizer *synthesizer);
	
	
	
	/** \brief Number of samples to create. */
	inline int GetSampleCount() const{return pSampleCount;}
	
	/** \brief Set number of samples to create. */
	void SetSampleCount(int sampleCount);
	
	
	
	/** \brief Number of controllers. */
	int GetControllerCount() const;
	
	/**
	 * \brief Controller at index.
	 * \throws deeInvalidParam \em index is less than 0 or greater or equal than GetControllerCount().
	 */
	deSynthesizerController *GetControllerAt(int index) const;
	
	/** \brief Index of controller or -1 if absent. */
	int IndexOfControllerNamed(const char *name) const;
	
	/** \brief Notify peer controller changed. */
	void NotifyControllerChangedAt(int index);
	/*@}*/
	
	
	
	/** \name Generate sound */
	/*@{*/
	/**
	 * \brief Reset synthesizer playback.
	 * 
	 * \warning Audio module is allowed to call this asynchronously.
	 */
	void Reset();
	
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
	void GenerateSound(void *buffer, int bufferSize, int offset, int samples);
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Synthesizer peer or NULL if not set. */
	inline deBaseSynthesizerSynthesizerInstance *GetPeerSynthesizer() const{return pPeerSynthesizer;}
	
	/** \brief Set synthesizer peer or NULL if not set. */
	void SetPeerSynthesizer(deBaseSynthesizerSynthesizerInstance *peer);
	
	/** \brief Audio peer or NULL if not set. */
	inline deBaseAudioSynthesizerInstance *GetPeerAudio() const{return pPeerAudio;}
	
	/** \brief Set audio peer or NULL if not set. */
	void SetPeerAudio(deBaseAudioSynthesizerInstance *peer);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pUpdateControllers();
};

#endif
