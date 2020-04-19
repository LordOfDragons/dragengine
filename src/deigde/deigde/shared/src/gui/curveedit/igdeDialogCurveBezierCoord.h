/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEDIALOGCURVEBEZIERCOORD_H_
#define _IGDEDIALOGCURVEBEZIERCOORD_H_

#include "../dialog/igdeDialog.h"
#include "../composed/igdeEditVector2Reference.h"

#include <dragengine/common/curve/decCurveBezierPoint.h>



/**
 * \brief Dialog bezier curve coordinates.
 */
class igdeDialogCurveBezierCoord : public igdeDialog{
private:
	decCurveBezierPoint pPoint;
	igdeEditVector2Reference pEditPoint;
	igdeEditVector2Reference pEditHandle1;
	igdeEditVector2Reference pEditHandle2;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	igdeDialogCurveBezierCoord( igdeEnvironment &environment );
	
	/** \brief Create dialog. */
	igdeDialogCurveBezierCoord( igdeEnvironment &environment, const decCurveBezierPoint &point );
	
	
	
protected:
	/** \brief Clean up dialog. */
	virtual ~igdeDialogCurveBezierCoord();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Point. */
	inline const decCurveBezierPoint &GetPoint() const{ return pPoint; }
	
	/** \brief Set point. */
	void SetPoint( const decCurveBezierPoint &point );
	/*@}*/
	
	
	
private:
	void pCreateContent( igdeEnvironment &environment );
};

#endif
