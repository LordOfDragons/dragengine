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

#ifndef _DEOALATDEBUG_H_
#define _DEOALATDEBUG_H_

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/resources/debug/deDebugDrawerReference.h>

class deoalAudioThread;

class deDebugBlockInfo;



/**
 * \brief Audio thread debugging.
 */
class deoalATDebug{
private:
	deoalAudioThread &pAudioThread;
	
	bool pEnabled;
	
	bool pLogCalcEnvProbe;
	bool pShowSpeakerEnvInfo;
	bool pShowSpeakerEnvInfoAt;
	decDVector pShowSpeakerEnvInfoAtPosition;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render thread debugging. */
	deoalATDebug( deoalAudioThread &audioThread );
	
	/** \brief Clean up render thread renderers. */
	~deoalATDebug();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Developer mode enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Log calculation of environment probe calculation. */
	inline bool GetLogCalcEnvProbe() const{ return pLogCalcEnvProbe; }
	
	/** \brief Show speaker environment overlay information. */
	inline bool GetShowSpeakerEnvInfo() const{ return pShowSpeakerEnvInfo; }
	
	/** \brief Show only speaker environment overlay information at position. */
	inline bool GetShowSpeakerEnvInfoAt() const{ return pShowSpeakerEnvInfoAt; }
	
	/** \brief Show environment overlay information position. */
	inline const decDVector &GetShowSpeakerEnvInfoAtPosition() const{ return pShowSpeakerEnvInfoAtPosition; }
	
	
	
	/** \brief Synchronize. */
	void Synchronize();
	/*@}*/
};

#endif
