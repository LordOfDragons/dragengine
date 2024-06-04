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

#ifndef _PEECONTROLLER_H_
#define _PEECONTROLLER_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>

#include <dragengine/common/string/decString.h>

class peeEmitter;
class deParticleEmitterController;



/**
 * \brief Particle Emitter Controller.
 */
class peeController : public deObject{
private:
	peeEmitter *pEmitter;
	int pEngControllerIndex;
	
	decString pName;
	
	float pLower;
	float pUpper;
	float pValue;
	bool pClamp;
	bool pFrozen;
	bool pLinkToTime;
	
	bool pActive;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new controller. */
	peeController();
	/** \brief Clean up the controller. */
	virtual ~peeController();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the parent emitter. */
	inline peeEmitter *GetEmitter() const{ return pEmitter; }
	/** Sets the parent emitter. */
	void SetEmitter( peeEmitter *emitter );
	
	/** Retrieves the engine particle emitter controller index. */
	inline int GetEngineControllerIndex() const{ return pEngControllerIndex; }
	/** Sets the index of the engine controller. */
	void SetEngineControllerIndex( int index );
	/** Update engine controller if existing. */
	void UpdateEngineController( deParticleEmitterController &controller );
	
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the name. */
	void SetName( const char *name );
	/** Updates the controller if required. */
	void UpdateValue( float elapsed );
	
	/** \brief Lower value. */
	inline float GetLower() const{ return pLower; }
	
	/** \brief Set lower value. */
	void SetLower( float value );
	
	/** \brief Upper value. */
	inline float GetUpper() const{ return pUpper; }
	
	/** \brief Set upper values. */
	void SetUpper( float value );
	
	/** Retrieves the current value. */
	inline float GetValue() const{ return pValue; }
	/** Sets the current value. */
	void SetValue( float value );
	/** Increments the current value. */
	void IncrementValue( float amount );
	/** Determines if the controller is frozen. */
	inline bool GetFrozen() const{ return pFrozen; }
	/** Sets if the controller is frozen. */
	void SetFrozen( bool frozen );
	/** Determines if values passed the range are clamped or wrapped around. */
	inline bool GetClamp() const{ return pClamp; }
	/** Sets if values passed the range are clamped or wrapped around. */
	void SetClamp( bool clamp );
	/** Determines if the controller is advanced by elapsed time. */
	inline bool GetLinkToTime() const{ return pLinkToTime; }
	/** Sets if the controller is advanced by elapsed time. */
	void SetLinkToTime( bool linkToTime );
	
	/** Determines if this is the active controller. */
	inline bool GetActive() const{ return pActive; }
	/** Sets if this is the active controller. */
	void SetActive( bool active );
	/*@}*/
	
private:
	float pCheckValue( float value );
	void pUpdateRange();
};

#endif
