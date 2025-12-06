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

#ifndef _SEUNODEDATA_H_
#define _SEUNODEDATA_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>

class sePropertyNode;



/**
 * \brief Undo action property node data.
 */
class seUPropertyNodeData : public deObject{
private:
	sePropertyNode *pNode;
	decPoint3 pPosition;
	decPoint3 pSize;
	float pRotation;
	float pShearing;
	decTexMatrix2 pMatrix;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<seUPropertyNodeData> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	seUPropertyNodeData( sePropertyNode *node );
	
protected:
	/** \brief Clean up undo. */
	virtual ~seUPropertyNodeData();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Node. */
	inline sePropertyNode *GetNode() const{ return pNode; }
	
	/** \brief Reference position. */
	inline const decPoint3 &GetPosition() const{ return pPosition; }
	
	/** \brief Reference size. */
	inline const decPoint3 &GetSize() const{ return pSize; }
	
	/** \brief Refrence rotation. */
	inline float GetRotation() const{ return pRotation; }
	
	/** \brief Reference shearing. */
	inline float GetShearing() const{ return pShearing; }
	
	/** \brief Matrix. */
	inline const decTexMatrix2 &GetMatrix() const{ return pMatrix; }
	
	/** \brief Create inverse matrix for the parent or mask parent. */
	decTexMatrix2 CreateInverseParentMatrix() const;
	/*@}*/
};

#endif
