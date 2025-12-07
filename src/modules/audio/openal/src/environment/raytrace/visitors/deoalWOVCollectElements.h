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

#ifndef _DEOALWOVCOLLECTELEMENTS_H_
#define _DEOALWOVCOLLECTELEMENTS_H_

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>

#include "../../../world/octree/deoalWorldOctreeVisitor.h"

class deoalAComponent;



/**
 * \brief World octree visitor collecting elements.
 * 
 * Visits octree with box. All elements hit by the box are stored in a list.
 */
class deoalWOVCollectElements : public deoalWorldOctreeVisitor{
private:
	decDVector pBoxMin;
	decDVector pBoxMax;
	decPointerList pComponents;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deoalWOVCollectElements();
	
	/** \brief Clean up visitor. */
	~deoalWOVCollectElements() override;
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Box minimum. */
	inline const decDVector &GetBoxMin() const{ return pBoxMin; }
	
	/** \brief Box maximum. */
	inline const decDVector &GetBoxMax() const{ return pBoxMax; }
	
	/** \brief Set box. */
	void SetBox(const decDVector &minimum, const decDVector &maximum);
	
	/** \brief Set box. */
	void SetBoxRange(const decDVector &position, double range);
	
	/** \brief Reset. */
	void Reset();
	
	
	
	/** \brief Number of components. */
	int GetComponentCount() const;
	
	/** \brief Component at index. */
	deoalAComponent *GetComponentAt(int index) const;
	
	
	
	/** \brief Visit component. */
	void VisitComponent(deoalAComponent *component) override;
	/*@}*/
};

#endif
