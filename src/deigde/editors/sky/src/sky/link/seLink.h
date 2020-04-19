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

#ifndef _SELINK_H_
#define _SELINK_H_

#include <dragengine/deObject.h>
#include <dragengine/common/curve/decCurveBezier.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class seSky;
class seController;



/**
 * \brief Sky link.
 */
class seLink : public deObject{
private:
	seSky *pSky;
	int pIndex;
	
	decString pName;
	
	seController *pController;
	decCurveBezier pCurve;
	int pRepeat;
	
	bool pSelected;
	bool pActive;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create link. */
	seLink();
	
	/** \brief Clean up link. */
	virtual ~seLink();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent sky or \em NULL. */
	inline seSky *GetSky() const{ return pSky; }
	
	/** \brief Set parent sky or \em NULL. */
	void SetSky( seSky *sky );
	
	/** \brief Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** \brief Set index. */
	void SetIndex( int index );
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Controller or \em NULL. */
	inline seController *GetController() const{ return pController; }
	
	/** \brief Set controller or \em NULL. */
	void SetController( seController *controller );
	
	/** \brief Curve. */
	inline decCurveBezier &GetCurve(){ return pCurve; }
	inline const decCurveBezier &GetCurve() const{ return pCurve; }
	
	/** \brief Notify curve changed. */
	void NotifyCurveChanged();
	
	/** \brief Repeat count of input value. */
	inline int GetRepeat() const{ return pRepeat; }
	
	/**
	 * \brief Set repeat count of input value.
	 * \throws deeInvalidParam \em repeat is less than 1.
	 */
	void SetRepeat( int repeat );
	
	/** \brief Link is active one. */
	inline bool GetActive() const{ return pActive; }
	
	/** \brief Set if link is active one. */
	void SetActive( bool active );
	
	/** \brief Link is selected. */
	inline bool GetSelected() const{ return pSelected; }
	
	/** \brief Set if link is selected. */
	void SetSelected( bool selected );
	/*@}*/
};

#endif
