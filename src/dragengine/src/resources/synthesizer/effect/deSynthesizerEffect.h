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

#ifndef _DESYNTHESIZEREFFECT_H_
#define _DESYNTHESIZEREFFECT_H_

#include "../deSynthesizerControllerTarget.h"
#include "../../../deObject.h"

class deSynthesizer;
class deSynthesizerEffectVisitor;


/**
 * \brief Synthesizer effect.
 */
class DE_DLL_EXPORT deSynthesizerEffect : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deSynthesizerEffect> Ref;
	
	
	
private:
	deSynthesizer *pSynthesizer;
	float pStrength;
	bool pEnabled;
	
	deSynthesizerControllerTarget pTargetStrength;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer effect. */
	deSynthesizerEffect();
	
protected:
	/**
	 * \brief Clean up synthesizer effect.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deSynthesizerEffect();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Parent synthesizer. */
	inline deSynthesizer *GetSynthesizer() const{ return pSynthesizer; }
	
	/**
	 * \brief Set parent synthesizer.
	 * 
	 * This function has to be used only by the deSynthesizer class to
	 * assign itself as a parent. Do not call this function directly.
	 */
	void SetSynthesizer( deSynthesizer *synthesizer );
	
	/** \brief Effect is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if effect is enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Strength. */
	inline float GetStrength() const{ return pStrength; }
	
	/** \brief Set strength. */
	void SetStrength( float strength );
	
	
	
	/** \brief Strength target. */
	inline deSynthesizerControllerTarget &GetTargetStrength(){ return pTargetStrength; }
	inline const deSynthesizerControllerTarget &GetTargetStrength() const{ return pTargetStrength; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit effect. */
	virtual void Visit( deSynthesizerEffectVisitor &visitor );
	/*@}*/
};

#endif
