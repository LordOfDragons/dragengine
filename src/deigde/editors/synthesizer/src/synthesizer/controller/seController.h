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

#ifndef _SECONTROLLER_H_
#define _SECONTROLLER_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/curve/decCurveBezier.h>

class seSynthesizer;
class deSynthesizerController;



/**
 * \brief Synthesizer controller.
 */
class seController : public deObject{
private:
	seSynthesizer *pSynthesizer;
	int pEngControllerIndex;
	
	decString pName;
	
	float pMinValue;
	float pMaxValue;
	bool pClamp;
	decCurveBezier pCurve;
	
	float pEditConstantValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create controller. */
	seController( const char *name = "Controller" );
	
	/** \brief Create copy of controller. */
	seController( const seController &copy );
	
	/** \brief Clean up controller. */
	virtual ~seController();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent synthesizer. */
	inline seSynthesizer *GetSynthesizer() const{ return pSynthesizer; }
	
	/** \brief Set parent synthesizer. */
	void SetSynthesizer( seSynthesizer *synthesizer );
	
	
	
	/** \brief Index of engine synthesizer instance controller. */
	inline int GetEngineControllerIndex() const{ return pEngControllerIndex; }
	
	/** \brief Set index of engine synthesizer instance controller. */
	void SetEngineControllerIndex( int index );
	
	
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Minimum value. */
	inline float GetMinimumValue() const{ return pMinValue; }
	
	/** \brief Set minimum value. */
	void SetMinimumValue( float value );
	
	/** \brief Maximum value. */
	inline float GetMaximumValue() const{ return pMaxValue; }
	
	/** \brief Set maximum value. */
	void SetMaximumValue( float value );
	
	/** \brief Value is clamped instead of wraped around. */
	inline bool GetClamp() const{ return pClamp; }
	
	/** \brief Set if value is clamped instead of wraped around. */
	void SetClamp( bool clamp );
	
	/** \brief Value curve. */
	inline const decCurveBezier &GetCurve() const{ return pCurve; }
	
	/** \brief Set value curve. */
	void SetCurve( const decCurveBezier &curve );
	
	
	
	/** \brief Edit constant value. */
	inline float GetEditConstantValue() const{ return pEditConstantValue; }
	
	/** \brief Set edit constant value. */
	void SetEditConstantValue( float value );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set controller. */
	seController &operator=( const seController &controller );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
