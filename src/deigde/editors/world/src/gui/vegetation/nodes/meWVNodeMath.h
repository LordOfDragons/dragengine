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

#ifndef _MEWVNODEMATH_H_
#define _MEWVNODEMATH_H_

#include "meWVNode.h"

#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeTextFieldReference.h>

class meHTVRuleMath;



/**
 * \brief Vegetation Editing Window Node Math.
 */
class meWVNodeMath : public meWVNode{
private:
	meHTVRuleMath *pRuleMath;
	
	igdeContainerReference pFraParameters;
	igdeComboBoxReference pCBOperator;
	igdeTextFieldReference pEditValueA;
	igdeTextFieldReference pEditValueB;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meWVNodeMath( meWindowVegetation &windowVegetation, meHTVRuleMath *rule );
	
protected:
	/** \brief Clean up object. */
	virtual ~meWVNodeMath();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Rule. */
	inline meHTVRuleMath *GetRuleMath() const{ return pRuleMath; }
	
	/** \brief Update node. */
	virtual void Update();
	/*@}*/
};

#endif
