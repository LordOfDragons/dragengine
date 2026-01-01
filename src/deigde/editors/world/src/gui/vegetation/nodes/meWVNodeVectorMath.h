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

#ifndef _MEWVNODEVECTORMATH_H_
#define _MEWVNODEVECTORMATH_H_

#include "meWVNode.h"

#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditVector.h>

class meHTVRuleVectorMath;



/**
 * \brief Vegetation Editing Window Node Vector Math.
 */
class meWVNodeVectorMath : public meWVNode{
public:
	typedef deTObjectReference<meWVNodeVectorMath> Ref;
	
private:
	meHTVRuleVectorMath *pRuleVectorMath;
	
	igdeContainer::Ref pFraParameters;
	igdeComboBox::Ref pCBOperator;
	igdeEditVector::Ref pEditVectorA;
	igdeEditVector::Ref pEditVectorB;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meWVNodeVectorMath(meWindowVegetation &windowVegetation, meHTVRuleVectorMath *rule);
	
protected:
	/** \brief Clean up object. */
	~meWVNodeVectorMath() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Rule. */
	inline meHTVRuleVectorMath *GetRuleVectorMath() const{ return pRuleVectorMath; }
	
	/** \brief Update node. */
	void Update() override;
	/*@}*/
};

#endif
