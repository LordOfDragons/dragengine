/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLRCANVASPAINT_H_
#define _DEOGLRCANVASPAINT_H_

#include "deoglRCanvas.h"

#include <dragengine/resources/canvas/deCanvasPaint.h>

class deoglSharedVBOBlock;


/**
 * Render canvas paint.
 */
class deoglRCanvasPaint : public deoglRCanvas{
private:
	deCanvasPaint::eShapeTypes pShapeType;
	decColor pLineColor;
	decColor pFillColor;
	float pThickness;
	bool pIsThick;
	float pRoundCornerX;
	float pRoundCornerY;
	float pStartAngle;
	float pEndAngle;
	
	decVector2 *pPoints;
	int pPointCount;
	
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
	/** Create peer. */
	deoglRCanvasPaint( deoglRenderThread &renderThread );
	
	/** Clean up peer. */
	virtual ~deoglRCanvasPaint();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Set size. */
	virtual void SetSize( const decVector2 &size );
	
	/** Shape type. */
	inline deCanvasPaint::eShapeTypes GetShapeType() const{ return pShapeType; }
	
	/**
	 * Set shape type.
	 * \throws deeInvalidParam \em shapeType is not a member of deCanvasPaint::eShapeTypes.
	 */
	void SetShapeType( deCanvasPaint::eShapeTypes shapeType );
	
	/** Line color. */
	inline const decColor &GetLineColor() const{ return pLineColor; }
	
	/** Set line color. */
	void SetLineColor( const decColor &color );
	
	/** Fill color. */
	inline const decColor &GetFillColor() const{ return pFillColor; }
	
	/** Set fill color. */
	void SetFillColor( const decColor &color );
	
	/** Line thickness in units. */
	inline float GetThickness() const{ return pThickness; }
	
	/**
	 * Line thickness in units.
	 * \details \em thickness is clamped to 0 or larger.
	 */
	void SetThickness( float thickness );
	
	/** Requires thick rendering. */
	inline bool IsThick() const{ return pIsThick; }
	
	/**
	 * Round corner in X direction as percentage.
	 * 
	 * Used by estRectangle shape type. Value of 0 indicates no non-round corners while
	 * value of 1 indicates fully round corners (aka ellipse).
	 */
	inline float GetRoundCornerX() const{ return pRoundCornerX; }
	
	/**
	 * Set round corner in X direction as percentage.
	 * 
	 * Used by estRectangle shape type. Value of 0 indicates no non-round corners while
	 * value of 1 indicates fully round corners (aka ellipse).
	 */
	void SetRoundCornerX( float roundCorner );
	
	/**
	 * Round corner in Y direction as percentage.
	 * 
	 * Used by estRectangle shape type. Value of 0 indicates no non-round corners while
	 * value of 1 indicates fully round corners (aka ellipse).
	 */
	inline float GetRoundCornerY() const{ return pRoundCornerY; }
	
	/**
	 * Set round corner in Y direction as percentage.
	 * 
	 * Used by estRectangle shape type. Value of 0 indicates no non-round corners while
	 * value of 1 indicates fully round corners (aka ellipse).
	 */
	void SetRoundCornerY( float roundCorner );
	
	/**
	 * Start angle in degrees from 0 to 360.
	 * 
	 * Used by estEllipse and estPie shape type. Angle is measured clock wise.
	 */
	inline float GetStartAngle() const{ return pStartAngle; }
	
	/**
	 * Set start angle in degrees from 0 to 360.
	 * 
	 * Used by estEllipse and estPie shape type. Angle is measured clock wise.
	 */
	void SetStartAngle( float angle );
	
	/**
	 * End angle in degrees from 0 to 360.
	 * 
	 * Used by estEllipse and estPie shape type. Angle is measured clock wise.
	 */
	inline float GetEndAngle() const{ return pEndAngle; }
	
	/**
	 * Set end angle in degrees from 0 to 360.
	 * 
	 * Used by estEllipse and estPie shape type. Angle is measured clock wise.
	 */
	void SetEndAngle( float angle );
	
	
	
	/** \name Points */
	/*@{*/
	/** Number of points. */
	inline int GetPointCount() const{ return pPointCount; }
	
	/**
	 * Set number of points.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void SetPointCount( int count );
	
	/** Points. */
	inline decVector2 *GetPoints() const{ return pPoints; }
	
	
	
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
	
	
	
	/** Prepare for rendering. */
	virtual void PrepareForRender( const deoglRenderPlanMasked *renderPlanMask );
	
	/** Render. */
	virtual void Render( const deoglRenderCanvasContext &context );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	int pRequiredPointCount();
	void pPrepareVBOBlock();
	void pWriteVBOData();
	void pCalcArc( decVector2 *outPoints, const decVector2 &center, const decVector2 &size,
		float startAngle, float stopAngle, int stepCount );
};

#endif
