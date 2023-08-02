/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
