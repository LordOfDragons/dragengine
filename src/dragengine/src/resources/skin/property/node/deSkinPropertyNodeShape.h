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

#ifndef _DESKINPROPERTYNODESHAPE_H_
#define _DESKINPROPERTYNODESHAPE_H_

#include "deSkinPropertyNode.h"


/**
 * \brief Skin property shape node for constructed texture property.
 */
class DE_DLL_EXPORT deSkinPropertyNodeShape : public deSkinPropertyNode{
public:
	/** \brief Shape types. */
	enum eShapeTypes{
		/** \brief Rectangle/box. */
		estRectangle,
		
		/** \brief Ellipse/Ellipsoid. */
		estEllipse
	};
	
	/** \brief Mapped. */
	enum eShapeMapped{
		esmFillColorRed, //<!Fill color red component
		esmFillColorGreen, //<!Fill color green component
		esmFillColorBlue, //<!Fill color blue component
		esmFillColorAlpha, //<!Fill color alpha component
		esmLineColorRed, //<!Line color red component
		esmLineColorGreen, //<!Line color green component
		esmLineColorBlue, //<!Line color blue component
		esmLineColorAlpha, //<!Line color alpha component
		esmThickness //<!Thickness
	};
	
	static const int ShapeMappedCount = esmThickness + 1;
	
	
	
private:
	eShapeTypes pShapeType;
	decColor pFillColor;
	decColor pLineColor;
	float pThickness;
	
	int pShapeMapped[ShapeMappedCount];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create constructed property node. */
	deSkinPropertyNodeShape();
	
	/** \brief Clean up constructed property node. */
	virtual ~deSkinPropertyNodeShape();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Shape type. */
	inline eShapeTypes GetShapeType() const{ return pShapeType; }
	
	/** \brief Set shape type. */
	void SetShapeType(eShapeTypes type);
	
	/** \brief Fill color. */
	inline const decColor &GetFillColor() const{ return pFillColor; }
	
	/** \brief Set fill color. */
	void SetFillColor(const decColor &color);
	
	/** \brief Line color. */
	inline const decColor &GetLineColor() const{ return pLineColor; }
	
	/** \brief Set line color. */
	void SetLineColor(const decColor &color);
	
	/** \brief Line thickness in pixels. */
	inline float GetThickness() const{ return pThickness; }
	
	/** \brief Set thicknss in pixels. */
	void SetThickness(float thickness);
	
	
	
	/** \brief Index of mapped value or -1 to use static value. */
	int GetShapeMappedFor(eShapeMapped mapped) const;
	
	/** \brief Set index of mapped value or -1 to use static value. */
	void SetShapeMappedFor(eShapeMapped mapped, int index);
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits node. */
	virtual void Visit(deSkinPropertyNodeVisitor &visitor);
	/*@}*/
};

#endif
