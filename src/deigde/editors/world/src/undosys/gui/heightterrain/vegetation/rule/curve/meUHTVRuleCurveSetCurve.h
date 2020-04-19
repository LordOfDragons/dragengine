/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEUHTVRULECURVESETCURVE_H_
#define _MEUHTVRULECURVESETCURVE_H_

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/curve/decCurveBezier.h>


class meHTVegetationLayer;
class meHTVRuleCurve;



/**
 * \brief Undo Height Terrain Vegetation Rule Curve Set Curve.
 */
class meUHTVRuleCurveSetCurve : public igdeUndo{
private:
	meHTVegetationLayer *pVLayer;
	meHTVRuleCurve *pRule;
	
	decCurveBezier pOldCurve;
	decCurveBezier pNewCurve;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meUHTVRuleCurveSetCurve( meHTVegetationLayer *vlayer, meHTVRuleCurve *rule, const decCurveBezier &newCurve );
	
protected:
	/** \brief Clean up object. */
	virtual ~meUHTVRuleCurveSetCurve();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Set new curve. */
	void SetNewCurve( const decCurveBezier &curve );
	
	/** \brief Undo. */
	virtual void Undo();
	
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
