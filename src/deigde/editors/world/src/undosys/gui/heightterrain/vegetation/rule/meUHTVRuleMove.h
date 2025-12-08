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
#ifndef _MEUHTVRULEMOVE_H_
#define _MEUHTVRULEMOVE_H_

// includes
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

// predefinitions
class meHTVegetationLayer;
class meHTVRule;



/**
 * \brief Undo Move Height Terrain Vegetation Rule.
 *
 * Undo action for moving a height terrain vegetation rule.
 */
class meUHTVRuleMove : public igdeUndo{
private:
	meHTVegetationLayer::Ref pVLayer;
	meHTVRule::Ref pRule;
	
	decVector2 pOldPosition;
	decVector2 pNewPosition;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meUHTVRuleMove> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meUHTVRuleMove(meHTVegetationLayer::Ref vlayer, meHTVRule::Ref rule);
	/** \brief Clean up object. */
	virtual ~meUHTVRuleMove();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the new position. */
	inline const decVector2 &GetNewPosition() const{ return pNewPosition; }
	/** Sets the new position triggering. */
	void SetNewPosition(const decVector2 &position);
	
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

// end of include only once
#endif
