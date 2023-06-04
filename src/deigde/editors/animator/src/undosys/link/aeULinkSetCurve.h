/* 
 * Drag[en]gine IGDE Animator Editor
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

#ifndef _AEULINKSETCURVE_H_
#define _AEULINKSETCURVE_H_

#include <deigde/undo/igdeUndo.h>

#include <dragengine/deObjectReference.h>
#include <dragengine/common/curve/decCurveBezier.h>


class aeLink;


/**
 * Undo link set curve.
 */
class aeULinkSetCurve : public igdeUndo{
private:
	deObjectReference pLink;
	decCurveBezier pOldCurve;
	decCurveBezier pNewCurve;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create undo. */
	aeULinkSetCurve( aeLink *link, const decCurveBezier &newCurve );
	
protected:
	/** Clean up undo. */
	virtual ~aeULinkSetCurve();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Set new curve. */
	void SetNewCurve( const decCurveBezier &curve );
	
	/** Undo. */
	virtual void Undo();
	
	/** Redo. */
	virtual void Redo();
	
	/** Progressive redo action. */
	void ProgressiveRedo();
	/*@}*/
};

#endif
