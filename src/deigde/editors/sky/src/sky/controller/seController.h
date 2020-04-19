/* 
 * Drag[en]gine IGDE Sky Editor
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

class seSky;



/**
 * \brief Controller.
 */
class seController : public deObject{
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
