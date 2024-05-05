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

#ifndef _DEOGLSKINSTATECNSHAPE_H_
#define _DEOGLSKINSTATECNSHAPE_H_

#include "../../../deoglGL.h"
#include "deoglSkinStateConstructedNode.h"

#include <dragengine/resources/skin/property/node/deSkinPropertyNodeShape.h>

class deoglSharedVBOBlock;


/**
 * Skin state constructed node.
 */
class deoglSkinStateCNShape : public deoglSkinStateConstructedNode{
public:
	typedef deTObjectReference<deoglSkinStateCNShape> Ref;
	
	
	
private:
	const deSkinPropertyNodeShape::eShapeTypes pShapeType;
	decColor pFillColor;
	decColor pLineColor;
	float pThickness;
	
	int pShapeMapped[ deSkinPropertyNodeShape::ShapeMappedCount ];
	
	bool pIsThick;
	
	deoglSharedVBOBlock *pVBOBlock;
	bool pDirtyVBOBlock;
	int pVBOBlockPointCount;
	
	GLenum pDrawModeFill;
	int pDrawOffsetFill;
	int pDrawCountFill;
	
	GLenum pDrawModeLine;
	int pDrawOffsetLine;
	int pDrawCountLine;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin state constructed node. */
	deoglSkinStateCNShape( deSkinPropertyNodeShape &node );
	
	/** Create skin state constructed node. */
	deoglSkinStateCNShape( const deoglSkinStateCNShape &node );
	
protected:
	/** Clean up skin state mapped. */
	virtual ~deoglSkinStateCNShape() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Shape type. */
	inline deSkinPropertyNodeShape::eShapeTypes GetShapeType() const{ return pShapeType; }
	
	/** Fill color. */
	inline const decColor &GetFillColor() const{ return pFillColor; }
	
	/** Line color. */
	inline const decColor &GetLineColor() const{ return pLineColor; }
	
	/** Thickness. */
	inline float GetThickness() const{ return pThickness; }
	
	/** Shape mapped for type. */
	int GetShapeMappedFor( deSkinPropertyNodeShape::eShapeMapped type ) const;
	
	
	
	/**
	 * Update.
	 * \warning Called from main thread.
	 */
	virtual void Update( deoglSkinState &state ) override;
	
	/** Prepare for render. */
	virtual void PrepareForRender( deoglSkinState &state ) override;
	
	/** Render. */
	virtual void Render( deoglSkinState &state, const deoglRenderCanvasContext &context ) override;
	
	/** Create copy. */
	virtual deoglSkinStateConstructedNode::Ref Copy() const override;
	
	
	
	/** \name Points */
	/*@{*/
	/** VBO block. */
	inline deoglSharedVBOBlock *GetVBOBlock() const{ return pVBOBlock; }
	
	
	
	/** Fill draw mode. */
	inline GLenum GetDrawModeFill() const{ return pDrawModeFill; }
	
	/** VBO fill offset. */
	inline int GetDrawOffsetFill() const{ return pDrawOffsetFill; }
	
	/** VBO fill count. */
	inline int GetDrawCountFill() const{ return pDrawCountFill; }
	
	
	
	/** Line draw mode. */
	inline GLenum GetDrawModeLine() const{ return pDrawModeLine; }
	
	/** VBO line offset. */
	inline int GetDrawOffsetLine() const{ return pDrawOffsetLine; }
	
	/** VBO line count. */
	inline int GetDrawCountLine() const{ return pDrawCountLine; }
	/*@}*/
	
	
	
private:
	int pRequiredPointCount();
	void pPrepareVBOBlock( deoglSkinState &state );
	void pWriteVBOData( deoglSkinState &state );
	void pCalcArc( decVector2 *outPoints, const decVector2 &center, const decVector2 &size,
		float startAngle, float stopAngle, int stepCount );
};

#endif
