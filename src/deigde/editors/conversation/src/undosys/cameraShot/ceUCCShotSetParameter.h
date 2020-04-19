/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CEUCCSHOTSETPARAMETER_H_
#define _CEUCCSHOTSETPARAMETER_H_

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/curve/decCurveBezier.h>

class ceCameraShot;



/**
 * \brief Undo Action Camera Shot Set Parameter.
 */
class ceUCCShotSetParameter : public igdeUndo{
private:
	ceCameraShot *pCameraShot;
	int pParameter;
	
	decCurveBezier pOldCurve;
	decCurveBezier pNewCurve;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	ceUCCShotSetParameter( ceCameraShot *cameraShot, int parameter );
protected:
	/** \brief Clean up undo. */
	virtual ~ceUCCShotSetParameter();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Sets the new curve. */
	void SetNewCurve( const decCurveBezier &curve );
	
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
