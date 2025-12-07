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

#ifndef _REWPRIG_H_
#define _REWPRIG_H_

#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class reRig;
class reWindowProperties;
class reWPRigListener;



/**
 * \brief Rig panel.
 */
class reWPRig : public igdeContainerScroll{
private:
	reWindowProperties &pWindowProperties;
	reRig *pRig;
	reWPRigListener *pListener;
	
	igdeComboBox::Ref pCBRootBone;
	igdeCheckBox::Ref pChkDynamic;
	igdeCheckBox::Ref pChkModelCollision;
	igdeEditVector::Ref pEditCentralMassPoint;
	igdeTextField::Ref pEditMass;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	reWPRig(reWindowProperties &windowProperties);
	
protected:
	/** \brief Clean up panel. */
	virtual ~reWPRig();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Rig. */
	inline reRig *GetRig() const{return pRig;}
	
	/** \brief Set rig. */
	void SetRig(reRig *rig);
	
	/** \brief Update root bone list. */
	void UpdateRootBoneList();
	
	/** \brief Update rig. */
	void UpdateRig();
	/*@}*/
};

#endif
