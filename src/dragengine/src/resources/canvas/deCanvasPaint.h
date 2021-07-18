/* 
 * Drag[en]gine Game Engine
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

#ifndef _DECANVASPAINT_H_
#define _DECANVASPAINT_H_

#include "deCanvas.h"


/**
 * \brief Canvas rendering geometric shapes.
 */
class deCanvasPaint : public deCanvas{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deCanvasPaint> Ref;
	
	
	
public:
	/** \brief Shape types. */
	enum eShapeTypes{
		/**
		 * \brief Points of radius \em thickness.
		 * 
		 * Add N points to render N points.
		 */
		estPoints,
		
		/**
		 * \brief Disconnected line segments.
		 * 
		 * Add N*2 points to render N disconnected line segments.
		 */
		estLines,
		
		/**
		 * \brief Filled or empty rectangle.
		 * 
		 * The width and height of the rectangle matches the canvas size rendering
		 * a rectangle filling the entire canvas. The round corners parameters
		 * define the percentag of rounding with 0 no rounding and 1 full rounding
		 * upp the way to the center axis.
		 */
		estRectangle,
		
		/**
		 * \brief Filled or empty ellipse.
		 * 
		 * The radi of the ellipse matches the size of the canvas to fill entire
		 * canvas. If the start and end angle are equal the ellipse is closed
		 * otherwise it is open with the end points connected by a straight line.
		 * If ellipse is open and \em fillColor is not transparent the ellipse is
		 * filled up to the connection line between the two end points.
		 */
		estEllipse,
		
		/**
		 * \brief Filled or empty pie.
		 * 
		 * Same definition as deCanvasPaint::estEllipse with the difference that in
		 * the open case a connection line is drawn from one end point to the other
		 * across the center point.
		 */
		estPie,
		
		/**
		 * \brief Open or closed filled or empty polygon.
		 * 
		 * Add N points to render a polygon with N corners. At least 2 points are
		 * required for the outline of the polygon to be rendered. At least 3 points
		 * are required for the polygon to be filled if \em fillColor is not
		 * transparent. The points have to be oriented clockwise. The polygon is not
		 * required to be convex. If the last point is equal to the first point the
		 * polygon is closed otherwise open. If the polygon is open it is filled up
		 * to the straight connection line between the two end points.
		 */
		estPolygon
	};
	
private:
	eShapeTypes pShapeType;
	decColor pLineColor;
	decColor pFillColor;
	float pThickness;
	float pRoundCornerX;
	float pRoundCornerY;
	float pStartAngle;
	float pEndAngle;
	
	decPoint *pPoints;
	int pPointCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create canvas.
	 * 
	 * The default values are:
	 * - Shape type is deCanvasPaint::estPolygon.
	 * - Thickness is 1 units.
	 * - Line color is black.
	 * - Fill color is transparent.
	 */
	deCanvasPaint( deCanvasManager *manager );
	
protected:
	/**
	 * \brief Clean up canvas.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deCanvasPaint();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Shape type. */
	inline eShapeTypes GetShapeType() const{ return pShapeType; }
	
	/**
	 * \brief Set shape type.
	 * \throws deeInvalidParam \em shapeType is not a member of deCanvasPaint::eShapeTypes.
	 */
	void SetShapeType( eShapeTypes shapeType );
	
	/** \brief Line color. */
	inline const decColor &GetLineColor() const{ return pLineColor; }
	
	/** \brief Set line color. */
	void SetLineColor( const decColor &color );
	
	/** \brief Fill color. */
	inline const decColor &GetFillColor() const{ return pFillColor; }
	
	/** \brief Set fill color. */
	void SetFillColor( const decColor &color );
	
	/** \brief Line thickness in units. */
	inline float GetThickness() const{ return pThickness; }
	
	/**
	 * \brief Line thickness in units.
	 * 
	 * \em thickness is clamped to 0 or larger.
	 */
	void SetThickness( float thickness );
	
	/**
	 * \brief Round corner in X direction as percentage.
	 * 
	 * Used by estRectangle shape type. Value of 0 indicates no non-round corners while
	 * value of 1 indicates fully round corners (aka ellipse).
	 */
	inline float GetRoundCornerX() const{ return pRoundCornerX; }
	
	/**
	 * \brief Set round corner in X direction as percentage.
	 * 
	 * Used by estRectangle shape type. Value of 0 indicates no non-round corners while
	 * value of 1 indicates fully round corners (aka ellipse).
	 */
	void SetRoundCornerX( float roundCorner );
	
	/**
	 * \brief Round corner in Y direction as percentage.
	 * 
	 * Used by estRectangle shape type. Value of 0 indicates no non-round corners while
	 * value of 1 indicates fully round corners (aka ellipse).
	 */
	inline float GetRoundCornerY() const{ return pRoundCornerY; }
	
	/**
	 * \brief Set round corner in Y direction as percentage.
	 * 
	 * Used by estRectangle shape type. Value of 0 indicates no non-round corners while
	 * value of 1 indicates fully round corners (aka ellipse).
	 */
	void SetRoundCornerY( float roundCorner );
	
	/**
	 * \brief Start angle in degrees radians from 0 to 2PI.
	 * 
	 * Used by estEllipse and estPie shape type. Angle is measured clock wise.
	 */
	inline float GetStartAngle() const{ return pStartAngle; }
	
	/**
	 * \brief Set start angle in radians from 0 to 2PI.
	 * 
	 * Used by estEllipse and estPie shape type. Angle is measured clock wise.
	 */
	void SetStartAngle( float angle );
	
	/**
	 * \brief End angle in radians from 0 to 2PI.
	 * 
	 * Used by estEllipse and estPie shape type. Angle is measured clock wise.
	 */
	inline float GetEndAngle() const{ return pEndAngle; }
	
	/**
	 * \brief Set end angle in radians from 0 to 2PI.
	 * 
	 * Used by estEllipse and estPie shape type. Angle is measured clock wise.
	 */
	void SetEndAngle( float angle );
	/*@}*/
	
	
	
	/** \name Points */
	/*@{*/
	/** \brief Number of points. */
	inline int GetPointCount() const{ return pPointCount; }
	
	/**
	 * \brief Point at position.
	 * \throws deeInvalidParam \em position is less than 0.
	 * \throws deeInvalidParam \em position is larger or equal to GetPointCount().
	 */
	const decPoint &GetPointAt( int position ) const;
	
	/** \brief Add a point. */
	void AddPoint( const decPoint &point );
	
	/**
	 * \brief Remove a point.
	 * \throws deeInvalidParam \em position is less than 0.
	 * \throws deeInvalidParam \em position is larger or equal to GetPointCount().
	 */
	void RemovePointFrom( int position );
	
	/** \brief Remove all points. */
	void RemoveAllPoints();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit canvas. */
	virtual void Visit( deCanvasVisitor &visitor );
	/*@}*/
};

#endif
