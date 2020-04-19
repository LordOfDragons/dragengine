/* 
 * Drag[en]gine IGDE Particle Emitter Editor
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

#ifndef _PEEUPARAMETERSETCURVE_H_
#define _PEEUPARAMETERSETCURVE_H_

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/curve/decCurveBezier.h>

class peeType;
class peeParameter;



/**
 * \brief Base class for undo action set parameter curve.
 */
class peeUParameterSetCurve : public igdeUndo{
protected:
	peeType *pType;
	peeParameter *pParameter;
	
	decCurveBezier pOldCurve;
	decCurveBezier pNewCurve;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	peeUParameterSetCurve( peeType *type, peeParameter *parameter, const decCurveBezier &newCurve );
	
protected:
	/** \brief Clean up undo. */
	virtual ~peeUParameterSetCurve();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Set new curve. */
	void SetNewCurve( const decCurveBezier &curve );
	/*@}*/
};

#endif
