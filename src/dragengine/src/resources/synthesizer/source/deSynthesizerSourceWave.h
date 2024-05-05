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
