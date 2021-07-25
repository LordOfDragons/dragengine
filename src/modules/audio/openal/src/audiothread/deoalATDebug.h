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
