/* 
 * Drag[en]gine Game Engine
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

#ifndef _DESYNTHESIZERSOURCEWAVE_H_
#define _DESYNTHESIZERSOURCEWAVE_H_

#include "deSynthesizerSource.h"


/**
 * \brief Synthesizer wave function source.
 */
class DE_DLL_EXPORT deSynthesizerSourceWave : public deSynthesizerSource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deSynthesizerSourceWave> Ref;
	
	
	
public:
	/** \brief Wave type. */
	enum eWaveType{
		/** \brief Sine wave. */
		ewtSine,
		
		/** \brief Square wave. */
		ewtSquare,
		
		/** \brief Saw-tooth wave. */
		ewtSawTooth,
		
		/** \brief Triangle wave. */
		ewtTriangle
	};
	
	
	
private:
	eWaveType pType;
	
	float pMinFrequency;
	float pMaxFrequency;
	
	deSynthesizerControllerTarget pTargetFrequency;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer wave generator source. */
	deSynthesizerSourceWave();
	
protected:
	/**
	 * \brief Clean up synthesizer wave generator source.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deSynthesizerSourceWave();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Wave type. */
	inline eWaveType GetType() const{ return pType; }
	
	/** \brief Set wave type. */
	void SetType( eWaveType type );
	
	
	
	/** \brief Minimum play frequency in Hz. */
	inline float GetMinFrequency() const{ return pMinFrequency; }
	
	/** \brief Set minimum frequency in Hz. */
	void SetMinFrequency( float frequency );
	
	/** \brief Maximum frequency in Hz. */
	inline float GetMaxFrequency() const{ return pMaxFrequency; }
	
	/** \brief Set maximum frequency in Hz. */
	void SetMaxFrequency( float frequency );
	
	
	
	/** \brief Play frequency target. */
	inline deSynthesizerControllerTarget &GetTargetFrequency(){ return pTargetFrequency; }
	inline const deSynthesizerControllerTarget &GetTargetFrequency() const{ return pTargetFrequency; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit source. */
	virtual void Visit( deSynthesizerSourceVisitor &visitor );
	/*@}*/
};

#endif
