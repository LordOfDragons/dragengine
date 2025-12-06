/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _REWPPANELSHAPE_H_
#define _REWPPANELSHAPE_H_

#include "../../../rig/shape/reRigShape.h"

#include <deigde/gui/igdeTextField.h>
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
	
	igdeTextField::Ref pEditBone;
	igdeTextField::Ref pEditProperty;
	
	
	
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
