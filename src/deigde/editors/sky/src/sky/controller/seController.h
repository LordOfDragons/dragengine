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

#ifndef _SECONTROLLER_H_
#define _SECONTROLLER_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class seSky;



/**
 * \brief Controller.
 */
class seController : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<seController> Ref;


private:
	seSky *pSky;
	int pIndex;
	
	decString pName;
	
	float pMinValue;
	float pMaxValue;
	float pCurValue;
	bool pClamp;
	bool pFrozen;
	
	bool pSelected;
	bool pActive;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sky controller. */
	seController();
	
	/** \brief Clean up sky controller. */
	virtual ~seController();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent sky. */
	inline seSky *GetSky() const{ return pSky; }
	
	/** \brief Set parent sky. */
	void SetSky( seSky *sky );
	
	/** \brief Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** \brief Set index. */
	void SetIndex( int index );
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set world filename. */
	void SetName( const char *filename );
	
	/** \brief Minimum value. */
	inline float GetMinimumValue() const{ return pMinValue; }
	
	/** \brief Set minimum value. */
	void SetMinimumValue( float value );
	
	/** \brief Maximum value. */
	inline float GetMaximumValue() const{ return pMaxValue; }
	
	/** \brief Set maximum value. */
	void SetMaximumValue( float value );
	
	/** \brief Current value. */
	inline float GetCurrentValue() const{ return pCurValue; }
	
	/** \brief Set current value. */
	void SetCurrentValue( float value );
	
	/** \brief Controller is frozen. */
	inline bool GetFrozen() const{ return pFrozen; }
	
	/** \brief Set if controller is frozen. */
	void SetFrozen( bool frozen );
	
	/** \brief Values are clamped instead of wrapping around. */
	inline bool GetClamp() const{ return pClamp; }
	
	/** \brief Set if values are clamped instead of wrapping around. */
	void SetClamp( bool clamp );
	
	/** \brief Controller is the active one. */
	inline bool GetActive() const{ return pActive; }
	
	/** \brief Set if controller is the active one. */
	void SetActive( bool active );
	
	/** \brief Controller is selected. */
	inline bool GetSelected() const{ return pSelected; }
	
	/** \brief Set if controller is selected. */
	void SetSelected( bool selected );
	/*@}*/
	
	
	
private:
	float pCheckValue( float value );
};

#endif
