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

#ifndef _MEWVNODECONSTANT_H_
#define _MEWVNODECONSTANT_H_

#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/composed/igdeEditVector.h>

#include "meWVNode.h"

class meHTVRuleConstant;



/**
 * \brief Vegetation Editing Window Node Constant.
 */
class meWVNodeConstant : public meWVNode{
private:
	meHTVRuleConstant *pRuleConstant;
	
	igdeContainer::Ref pFraParameters;
	igdeEditVector::Ref pEditVector;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meWVNodeConstant(meWindowVegetation &windowVegetation, meHTVRuleConstant *rule);
	
protected:
	/** \brief Clean up object. */
	~meWVNodeConstant() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Rule. */
	inline meHTVRuleConstant *GetRuleConstant() const{ return pRuleConstant; }
	
	/** \brief Update node. */
	void Update() override;
	/*@}*/
};

#endif
