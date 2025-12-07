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

#ifndef _DESYNSYNTHESIZER_H_
#define _DESYNSYNTHESIZER_H_

#include "../desynBasics.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/systems/modules/synthesizer/deBaseSynthesizerSynthesizer.h>
#include <dragengine/threading/deMutex.h>

class desynSynthesizerInstance;
class deDESynthesizer;
class deSynthesizer;
class desynSynthesizerSource;
class desynSynthesizerLink;



/**
 * \brief Synthesizer peer.
 */
class desynSynthesizer : public deBaseSynthesizerSynthesizer{
private:
	deDESynthesizer &pModule;
	deSynthesizer &pSynthesizer;
	
	decPointerList pLinks;
	
	desynSynthesizerSource **pSources;
	int pSourceCount;
	
	bool pSilent;
	int pStateDataSize;
	
	bool pDirtyContent;
	
	int pChannelCount;
	int pSampleRate;
	int pBytesPerSample;
	int pSampleCount;
	
	unsigned int pUpdateTracker;
	deMutex pMutex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer peer. */
	desynSynthesizer(deDESynthesizer &module, deSynthesizer &synthesizer);
	
	/** \brief Clean up synthesizer peer. */
	virtual ~desynSynthesizer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deDESynthesizer &GetModule() const{ return pModule; }
	
	/** \brief Synthesizer engine resource. */
	inline deSynthesizer &GetSynthesizer() const{ return pSynthesizer; }
	
	/** \brief Synthesizer is silent. */
	inline bool GetSilent() const{ return pSilent; }
	
	/** \brief Set if synthesizer is silent. */
	void SetSilent(bool silent);
	
	/** \brief Size of state data in bytes. */
	inline int GetStateDataSize() const{ return pStateDataSize; }
	
	/** \brief Set size of state data in bytes. */
	void SetStateDataSize(int size);
	
	/** \brief Current update tracker value. */
	inline unsigned int GetUpdateTracker() const{ return pUpdateTracker; }
	
	/** \brief Channel count. */
	inline int GetChannelCount() const{ return pChannelCount; }
	
	/** \brief Sample rate. */
	inline int GetSampleRate() const{ return pSampleRate; }
	
	/** \brief Bytes per sample. */
	inline int GetBytesPerSample() const{ return pBytesPerSample; }
	
	/** \brief Sample count. */
	inline int GetSampleCount() const{ return pSampleCount; }
	
	/** \brief Mutex. */
	inline deMutex &GetMutex(){ return pMutex; }
	
	
	
	/** \brief Number of links. */
	int GetLinkCount() const;
	
	/** \brief Link at index. */
	const desynSynthesizerLink &GetLinkAt(int index) const;
	
	/** \brief Add link. */
	void AddLink(desynSynthesizerLink *link);
	
	
	
	/** \brief Number of sources. */
	inline int GetSourceCount() const{ return pSourceCount; }
	
	/** \brief Source at index. */
	const desynSynthesizerSource &GetSourceAt(int index) const;
	
	
	
	/** \brief Prepare if required. */
	void Prepare();
	
	/** \brief Init state data. */
	void InitStateData(char *stateData);
	
	/** \brief Clean up state data. */
	void CleanUpStateData(char *stateData);
	
	/**
	 * \brief Generate sound.
	 * \param[in,out] stateData State at start of buffer. Update with state at start of next buffer.
	 * \param[out] buffer Buffer to store samples in.
	 * \param[in] offset Offset in samples to start producing sound at.
	 * \param[in] samples Number of samples to produce.
	 */
	void GenerateSound(const desynSynthesizerInstance &instance, char *stateData, float *buffer, int samples);
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Play time changed. */
	virtual void ParametersChanged();
	
	/** \brief Controllers changed. */
	virtual void ControllersChanged();
	
	/** \brief Links changed. */
	virtual void LinksChanged();
	
	/** \brief Sources changed. */
	virtual void SourcesChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pClearLinks();
	void pCreateLinks();
	void pCreateSources();
	void pClearSources();
};

#endif
