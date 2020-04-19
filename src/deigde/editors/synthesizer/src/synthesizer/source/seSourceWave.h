/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#ifndef _SESOURCEWAVE_H_
#define _SESOURCEWAVE_H_

#include "seSource.h"

#include <dragengine/resources/synthesizer/source/deSynthesizerSourceWave.h>



/**
 * \brief Synthesizer source wave generator.
 */
class seSourceWave : public seSource{
private:
	deSynthesizerSourceWave::eWaveType pWaveType;
	
	float pMinFrequency;
	float pMaxFrequency;
	
	seControllerTarget pTargetFrequency;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create source. */
	seSourceWave();
	
	/** \brief Create copy of source. */
	seSourceWave( const seSourceWave &copy );
	
	/** \brief Clean up source. */
	virtual ~seSourceWave();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Wave type. */
	inline deSynthesizerSourceWave::eWaveType GetWaveType() const{ return pWaveType; }
	
	/** \brief Set wave type. */
	void SetWaveType( deSynthesizerSourceWave::eWaveType type );
	
	
	
	/** \brief Minimum play frequency in Hz. */
	inline float GetMinFrequency() const{ return pMinFrequency; }
	
	/** \brief Set minimum frequency in Hz. */
	void SetMinFrequency( float frequency );
	
	/** \brief Maximum frequency in Hz. */
	inline float GetMaxFrequency() const{ return pMaxFrequency; }
	
	/** \brief Set maximum frequency in Hz. */
	void SetMaxFrequency( float frequency );
	
	
	
	/** \brief Play frequency target. */
	inline seControllerTarget &GetTargetFrequency(){ return pTargetFrequency; }
	inline const seControllerTarget &GetTargetFrequency() const{ return pTargetFrequency; }
	
	
	
	/** \brief Create engine synthesizer source. */
	virtual deSynthesizerSource *CreateEngineSource();
	
	/** \brief Update targets. */
	virtual void UpdateTargets();
	
	/** \brief Retrieve the number of targets using a given link. */
	virtual int CountLinkUsage( seLink *link ) const;
	
	/** \brief Removes a link from all targets using it. */
	virtual void RemoveLinkFromTargets( seLink *link );
	
	/** \brief Removes all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	/** \brief Create a copy of this source. */
	virtual seSource *CreateCopy() const;
	
	/** \brief List all links of all source targets. */
	virtual void ListLinks( seLinkList& list );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another synthesizer difference source to this synthesizer difference source. */
	virtual seSourceWave &operator=( const seSourceWave &copy );
	/*@}*/
};

#endif
