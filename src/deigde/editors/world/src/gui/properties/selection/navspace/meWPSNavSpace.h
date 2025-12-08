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

#ifndef _MEWPSNAVSPACE_H_
#define _MEWPSNAVSPACE_H_

#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditDVector.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class meWPSNavSpaceListener;
class meWPSelection;
class meWorld;
class meNavigationSpace;



/**
 * \brief Navigation space selection panel.
 */
class meWPSNavSpace : public igdeContainerScroll{
private:
	meWPSelection &pWPSelection;
	meWPSNavSpaceListener::Ref pListener;
	
	meWorld::Ref pWorld;
	
	igdeSpinTextField::Ref pSpinActive;
	igdeTextField::Ref pEditSelCount;
	igdeEditDVector::Ref pEditPositon;
	igdeEditVector::Ref pEditOrientation;
	igdeEditPath::Ref pEditPath;
	igdeListBox::Ref pListUsedCostTypes;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	meWPSNavSpace(meWPSelection &wpselection);
	
protected:
	/** \brief Clean up panel. */
	virtual ~meWPSNavSpace();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Selection panel. */
	inline meWPSelection &GetWPSelection() const{ return pWPSelection; }
	
	/** \brief World. */
	inline meWorld *GetWorld() const{ return pWorld; }
	
	/** \brief Set world. */
	void SetWorld(meWorld *world);
	
	/** \brief Active navigation space. */
	meNavigationSpace *GetNavigationSpace() const;
	
	/** \brief Update selection. */
	void UpdateSelection();
	
	/** \brief Update enabled state of all controls. */
	void UpdateEnabled();
	
	/** \brief Updates the navigation space. */
	void UpdateNavSpace();
	
	/** \brief Updates the navigation space geometry. */
	void UpdateGeometry();
	
	/** \brief Updates used cost types list. */
	void UpdateUsedCostTypes();
	/*@}*/
};

#endif
