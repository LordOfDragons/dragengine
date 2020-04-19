/* 
 * Drag[en]gine IGDE Rig Editor
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

#ifndef _REWPPANELSHAPE_H_
#define _REWPPANELSHAPE_H_

#include "../../../rig/shape/reRigShape.h"

#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>

class reRig;
class reWPShape;
class igdeUndo;



/**
 * \brief Base shape panel.
 */
class reWPPanelShape : public igdeContainerFlow{
protected:
	class cEditPosition : public igdeEditVectorListener{
		reWPPanelShape &pPanel;
	public:
		cEditPosition( reWPPanelShape &panel );
		virtual void OnVectorChanged( igdeEditVector *editVector );
	};
	
	class cEditRotation : public igdeEditVectorListener{
		reWPPanelShape &pPanel;
	public:
		cEditRotation( reWPPanelShape &panel );
		virtual void OnVectorChanged( igdeEditVector *editVector );
	};
	
	
	
private:
	reWPShape &pWPShape;
	const reRigShape::eShapeTypes pRequiredShapeType;
	
	reRig *pRig;
	reRigShape *pShape;
	
	igdeTextFieldReference pEditBone;
	igdeTextFieldReference pEditProperty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	reWPPanelShape( reWPShape &wpShape, reRigShape::eShapeTypes requiredShapeType );
	
protected:
	/** \brief Clean up panel. */
	virtual ~reWPPanelShape();
	/*@}*/
	
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Parent window. */
	inline reWPShape &GetWPShares() const{ return pWPShape; }
	
	/** \brief Required shape type. */
	inline reRigShape::eShapeTypes GetRequiredShapeType() const{ return pRequiredShapeType; }
	
	/** \brief Rig. */
	inline reRig *GetRig() const{ return pRig; }
	
	/** \brief Shape. */
	inline reRigShape *GetShape() const{ return pShape; }
	
	/** \brief Set shape. */
	void SetShape( reRig *rig, reRigShape *shape );
	
	
	
	/** \brief Update shape. */
	virtual void UpdateShape();
	/*@}*/
};

#endif
