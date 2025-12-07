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

#ifndef _REWPSHAPE_H_
#define _REWPSHAPE_H_

#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class reRig;
class reRigShape;
class reWPShapeListener;
class reWPPanelShape;
class reWPPanelShapeSphere;
class reWPPanelShapeBox;
class reWPPanelShapeCylinder;
class reWPPanelShapeCapsule;
class reWPPanelShapeHull;
class reWindowProperties;



/**
 * \brief Shapes panel.
 */
class reWPShape : public igdeContainerScroll{
private:
	reWindowProperties &pWindowProperties;
	reRigShape *pShape;
	reRig *pRig;
	reWPShapeListener *pListener;
	
	igdeSwitcher::Ref pSwitcher;
	reWPPanelShapeSphere *pPanelSphere;
	reWPPanelShapeBox *pPanelBox;
	reWPPanelShapeCylinder *pPanelCylinder;
	reWPPanelShapeCapsule *pPanelCapsule;
	reWPPanelShapeHull *pPanelHull;
	reWPPanelShape *pActivePanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	reWPShape(reWindowProperties &windowProperties);
	
protected:
	/** \brief Clean up panel. */
	virtual ~reWPShape();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Window properties. */
	inline reWindowProperties &GetWindowProperties() const{return pWindowProperties;}
	
	/** \brief Rig. */
	inline reRig *GetRig() const{return pRig;}
	
	/** \brief Set rig. */
	void SetRig(reRig *rig);
	
	/** \brief Shape. */
	inline reRigShape *GetShape() const{return pShape;}
	
	/** \brief Set shape. */
	void SetShape(reRigShape *shape);
	
	/** \brief Update shape. */
	void UpdateShape();
	/*@}*/
};

#endif
