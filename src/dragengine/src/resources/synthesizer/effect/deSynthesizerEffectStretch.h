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

#ifndef _DESYNTHESIZEREFFECSTRETCH_H_
#define _DESYNTHESIZEREFFECSTRETCH_H_

#include "deSynthesizerEffect.h"


/**
 * \brief Synthesizer effect stretch time and pitch.
 */
class DE_DLL_EXPORT deSynthesizerEffectStretch : public deSynthesizerEffect{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deSynthesizerEffectStretch> Ref;
	
	
private:
	float pMinTime;
	float pMaxTime;
	float pMinPitch;
	float pMaxPitch;
	
	deSynthesizerControllerTarget pTargetTime;
	deSynthesizerControllerTarget pTargetPitch;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer stretch effect. */
	deSynthesizerEffectStretch();
	
protected:
	/**
	 * \brief Clean up synthesizer stretch effect.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deSynthesizerEffectStretch();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Minimum time stretch factor. */
	inline float GetMinTime() const{ return pMinTime; }
	
	/** \brief Set minimum time stretch factor. */
	void SetMinTime( float stretch );
	
	/** \brief Maximum time stretch factor. */
	inline float GetMaxTime() const{ return pMaxTime; }
	
	/** \brief Set maximum time stretch factor. */
	void SetMaxTime( float stretch );
	
	/** \brief Minimum pitch shift factor. */
	inline float GetMinPitch() const{ return pMinPitch; }
	
	/** \brief Set minimum pitch shift factor. */
	void SetMinPitch( float shift );
	
	/** \brief Maximum pitch shift factor. */
	inline float GetMaxPitch() const{ return pMaxPitch; }
	
	/** \brief Set maximum pitch shift factor. */
	void SetMaxPitch( float shift );
	
	
	
	/** \brief Time stretch target. */
	inline deSynthesizerControllerTarget &GetTargetTime(){ return pTargetTime; }
	inline const deSynthesizerControllerTarget &GetTargetTime() const{ return pTargetTime; }
	
	/** \brief Pitch stretch target. */
	inline deSynthesizerControllerTarget &GetTargetPitch(){ return pTargetPitch; }
	inline const deSynthesizerControllerTarget &GetTargetPitch() const{ return pTargetPitch; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit effect. */
	virtual void Visit( deSynthesizerEffectVisitor &visitor );
	/*@}*/
};

#endif
