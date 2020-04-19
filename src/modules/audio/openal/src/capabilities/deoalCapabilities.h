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
