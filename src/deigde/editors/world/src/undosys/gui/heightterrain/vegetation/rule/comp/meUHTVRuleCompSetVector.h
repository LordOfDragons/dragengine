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

// include only once
#ifndef _MEUHTVRULECOMPSETVECTOR_H_
#define _MEUHTVRULECOMPSETVECTOR_H_

// includes
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

// predefinitions
class meHTVegetationLayer;
class meHTVRuleComponents;



/**
 * \brief Undo Height Terrain Vegetation Rule Components Set Vector.
 *
 * Undo action to set the vector of a height terrain vegetation rule components.
 */
class meUHTVRuleCompSetVector : public igdeUndo{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meUHTVRuleCompSetVector> Ref;
	
	
private:
	meHTVegetationLayer::Ref pVLayer;
	meHTVRuleComponents::Ref pRule;
	
	decVector pOldVector;
	decVector pNewVector;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meUHTVRuleCompSetVector(meHTVegetationLayer *vlayer, meHTVRuleComponents *rule, const decVector &nvector);
	/** \brief Clean up object. */
	virtual ~meUHTVRuleCompSetVector();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

// end of include only once
#endif
