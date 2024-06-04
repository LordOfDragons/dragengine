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

#ifndef _DEOALCAPABILITIES_H_
#define _DEOALCAPABILITIES_H_

#include "../deoalBasics.h"

#include <dragengine/common/string/decStringList.h>

class deoalAudioThread;



/**
 * \brief OpenAL capabilities.
 */
class deoalCapabilities{
private:
	deoalAudioThread &pAudioThread;
	
	int pMaxMonoSources;
	int pMaxStereoSources;
	
	int pEfxMaxAuxSend;
	
	decStringList pHRTFSpecifiers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create capabilities object. */
	deoalCapabilities( deoalAudioThread &renderThread );
	
	/** \brief Clean up capabilities object. */
	~deoalCapabilities();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deoalAudioThread &GetModule() const{ return pAudioThread; }
	
	/** \brief Report capabilities. */
	void ReportCapabilities();
	
	
	
	/** \brief Maximum mono sources. */
	inline int GetMaxMonoSources() const{ return pMaxMonoSources; }
	
	/** \brief Maximum stereo sources. */
	inline int GetMaxStereoSources() const{ return pMaxStereoSources; }
	
	
	
	/** \brief EFX maximum auxiliary send. */
	inline int GetEfxMaxAuxSend() const{ return pEfxMaxAuxSend; }
	
	
	
	/** \brief HRTF specifiers. */
	inline const decStringList &GetHRTFSpecifiers() const{ return pHRTFSpecifiers; }
	/*@}*/
	
	
	
private:
	void pDetectCapabilities();
};

#endif
