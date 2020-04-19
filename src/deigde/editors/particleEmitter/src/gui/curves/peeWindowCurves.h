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

#ifndef _PEEWINDOWCURVES_H_
#define _PEEWINDOWCURVES_H_

#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/curveedit/igdeViewCurveBezierReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/layout/igdeContainerSplitted.h>
#include <deigde/gui/resources/igdeIconReference.h>
#include <deigde/undo/igdeUndoReference.h>

class peeEmitter;
class peeParameter;
class peeType;
class peeWindowCurvesListener;
class peeWindowMain;



/**
 * \brief Curves Panel.
 */
class peeWindowCurves : public igdeContainerSplitted{
public:
	/** \brief Curve to edit. */
	enum eCurves{
		ecValue,
		ecSpread,
		ecProgress,
		ecBeam
	};
	
	
	
private:
	peeWindowMain &pWindowMain;
	peeWindowCurvesListener *pListener;
	
	peeEmitter *pEmitter;
	
	igdeIconReference pIconCurveEmpty;
	igdeIconReference pIconCurveUsed;
	
	igdeListBoxReference pListCurves;
	igdeViewCurveBezierReference pEditCurve;
	
	igdeUndoReference pUndoSetCurve;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create window. */
	peeWindowCurves( peeWindowMain &windowMain );
	
protected:
	/** \brief Clean up window. */
	virtual ~peeWindowCurves();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline peeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Active type. */
	peeType *GetType() const;
	
	/** \brief Active parameter. */
	peeParameter *GetParameter() const;
	
	/** \brief Curve to edit. */
	eCurves GetCurve() const;
	
	/** \brief Emitter. */
	inline peeEmitter *GetEmitter() const{ return pEmitter; }
	
	/** \brief Set emitter. */
	void SetEmitter( peeEmitter *emitter );
	
	/** \brief Update curve. */
	void UpdateCurve();
	
	/** \brief Drop progressive undo. */
	void DropProgressiveUndo();
	
	
	
private:
	void pUpdateCurveListIcons();
	/*@}*/
};

#endif
