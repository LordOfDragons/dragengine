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

#ifndef _DEDEBUGDRAWERSHAPE_H_
#define _DEDEBUGDRAWERSHAPE_H_

#include "../../common/math/decMath.h"
#include "../../common/shape/decShapeList.h"
#include "../../common/collection/decPointerList.h"

class deDebugDrawerShapeFace;


/**
 * \brief Debug drawer shape.
 * 
 * Describes the a shape to be drawn. There are a couple of parameters
 * that can be set to define the appearance. The edge and fill color flag
 * determine the color and transparency of the edges and the faces of
 * the shape. By default the edge is set to white fully opaque and the
 * fill is set to white fully transparent. Both shapes and faces can be
 * used at the same time. If none of them are present nothing is drawn.
 */
class DE_DLL_EXPORT deDebugDrawerShape{
private:
	decVector pPosition;
	decQuaternion pOrientation;
	decVector pScale;
	
	decShapeList pShapeList;
	
	decPointerList pFaces;
	
	decColor pEdgeColor;
	decColor pFillColor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create debug drawer shape. */
	deDebugDrawerShape();
	
	/** \brief Clean up debug drawer shape. */
	~deDebugDrawerShape();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition(const decVector &position);
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation(const decQuaternion &orientation);
	
	/** \brief Scale. */
	inline const decVector &GetScale() const{ return pScale; }
	
	/** \brief Set scale. */
	void SetScale(const decVector &scale);
	
	
	
	/** \brief Edge color. */
	inline const decColor &GetEdgeColor() const{ return pEdgeColor; }
	
	/** \brief Set edge color. */
	void SetEdgeColor(const decColor &color);
	
	/** \brief Fill color. */
	inline const decColor &GetFillColor() const{ return pFillColor; }
	
	/** \brief Set fill color. */
	void SetFillColor(const decColor &color);
	
	/** \brief Shape list. */
	inline decShapeList &GetShapeList(){ return pShapeList; }
	inline const decShapeList &GetShapeList() const{ return pShapeList; }
	/*@}*/
	
	
	
	/** \name Volume Faces */
	/*@{*/
	/** \brief Number of volume faces. */
	int GetFaceCount() const;
	
	/** \brief Face at index. */
	deDebugDrawerShapeFace *GetFaceAt(int index) const;
	
	/** \brief Add face. */
	void AddFace(deDebugDrawerShapeFace *face);
	
	/** \brief Remove all faces. */
	void RemoveAllFaces();
	/*@}*/
};

#endif
