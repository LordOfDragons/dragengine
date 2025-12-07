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

#ifndef _IGDEWCOORDSYSARROWS_H_
#define _IGDEWCOORDSYSARROWS_H_

#include "igdeWDebugDrawerShape.h"



/**
 * \brief Coordinate system arrows wrapper.
 * 
 * Draws a coordinate system using debug drawer shapes. Each arrow has an own
 * color to indicate the coordinate axis it refers to. The coordinate system
 * is given by a position and orientation.
 */
class DE_DLL_EXPORT igdeWCoordSysArrows{
private:
	igdeWDebugDrawerShape pDDSAxisX;
	igdeWDebugDrawerShape pDDSAxisY;
	igdeWDebugDrawerShape pDDSAxisZ;
	
	decVector pPosition;
	decQuaternion pOrientation;
	decVector pScale;
	
	decColor pColorAxisX;
	decColor pColorAxisY;
	decColor pColorAxisZ;
	
	float pArrowLength;
	float pArrowSize;
	
	bool pVisible;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create wrapper. */
	igdeWCoordSysArrows();
	
	/** \brief Clean up wrapper. */
	~igdeWCoordSysArrows();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Set parent debug drawer or NULL. */
	void SetParentDebugDrawer(deDebugDrawer *debugDrawer);
	
	/** \brief Position. */
	inline const decVector &GetPosition() const{return pPosition;}
	
	/** \brief Set position. */
	void SetPosition(const decVector &position);
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{return pOrientation;}
	
	/** \brief Set orientation. */
	void SetOrientation(const decQuaternion &orientation);
	
	/** \brief Scale. */
	inline const decVector &GetScale() const{return pScale;}
	
	/** \brief Set scale. */
	void SetScale(const decVector &scale);
	
	/** \brief X axis color. */
	inline const decColor &GetColorAxisX() const{return pColorAxisX;}
	
	/** \brief Set x axis color. */
	void SetAxisXColor(const decColor &color);
	
	/** \brief Y axis color. */
	inline const decColor &GetColorAxisY() const{return pColorAxisY;}
	
	/** \brief Set y axis color. */
	void SetAxisYColor(const decColor &color);
	
	/** \brief Z axis color. */
	inline const decColor &GetColorAxisZ() const{return pColorAxisZ;}
	
	/** \brief Set z axis color. */
	void SetAxisZColor(const decColor &color);
	
	/** \brief Length of the arrows. */
	inline float GetArrowLength() const{return pArrowLength;}
	
	/** \brief Set length of the arrows. */
	void SetArrowLength(float length);
	
	/** \brief Size of the arrow heads. */
	inline float GetArrowSize() const{return pArrowSize;}
	
	/** \brief Set size of the arrow heads. */
	void SetArrowSize(float size);
	
	/** \brief Determines if the debug drawer shape is visible. */
	inline bool GetVisible() const{return pVisible;}
	
	/** \brief Sets if the debug drawer shape is visible. */
	void SetVisible(bool visible);
	/*@}*/
	
	
	
private:
	void pRebuildArrows();
};

#endif
