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

#ifndef _RERIGSHAPEHULL_H_
#define _RERIGSHAPEHULL_H_

#include "reRigShape.h"


/**
 * \brief Rig Hull Shape.
 * 
 * Working object for rig box shape.
 */
class reRigShapeHull : public reRigShape{
private:
	decVector *pPoints;
	int pPointCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create rig hull shape. */
	reRigShapeHull( deEngine *engine );
	
protected:
	/** \brief Clean up rig shape shape. */
	virtual ~reRigShapeHull();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Point arrays. */
	inline decVector *GetPoints() const{ return pPoints; }
	
	/** \brief Number of points. */
	inline int GetPointCount() const{ return pPointCount; }
	
	/** \brief Point at index. */
	const decVector &GetPointAt( int index ) const;
	
	/** \brief Add point. */
	void AddPoint( const decVector &point );
	
	/** \brief Add point. */
	void InsertPoint( const decVector &point, int index );
	
	/** \brief Set point at index. */
	void SetPointAt( int index, const decVector &point );
	
	/** \brief Remove point. */
	void RemovePoint( int index );
	
	
	
	/** \brief Create copy of shape. */
	virtual reRigShape *Duplicate() const;
	
	/** \brief Uniformly scale shape. */
	virtual void Scale( float scale );
	
	/** \brief Create shape. */
	virtual decShape *CreateShape();
	/*@}*/
};

#endif
