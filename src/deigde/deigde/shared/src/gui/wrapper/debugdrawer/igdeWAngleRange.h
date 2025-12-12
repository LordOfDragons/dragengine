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

#ifndef _IGDEWANGLERANGE_H_
#define _IGDEWANGLERANGE_H_

#include "igdeWDebugDrawerShape.h"



/**
 * \brief Debug drawer shape wrapper angle range.
 * 
 * Draws a circle segment representing a 3d angle using a debug drawe shape.
 * The angle disk is oriented along the Z axis of the wrapper object with the
 * lower and upper angle bounds choosable. The limits are measured as rotations
 * around the Z axis.
 */
class DE_DLL_EXPORT igdeWAngleRange{
private:
	igdeWDebugDrawerShape pDDSDisk;
	
	decVector pPosition;
	decQuaternion pOrientation;
	decColor pColorDiskLine;
	decColor pColorDiskArea;
	
	float pLower;
	float pUpper;
	float pRadius;
	
	bool pVisible;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create wrapper. */
	igdeWAngleRange();
	
	/** \brief Clean up wrapper. */
	~igdeWAngleRange();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Set parent debug drawer or nullptr. */
	void SetParentDebugDrawer(deDebugDrawer *debugDrawer);
	
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition(const decVector &position);
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation(const decQuaternion &orientation);
	
	/** \brief Disk line color. */
	inline const decColor &GetColorDiskLine() const{ return pColorDiskLine; }
	
	/** \brief Set disk line color. */
	void SetDiskLineColor(const decColor &color);
	
	/** \brief Disk area color. */
	inline const decColor &GetColorDiskArea() const{ return pColorDiskArea; }
	
	/** \brief Set disk area color. */
	void SetDiskAreaColor(const decColor &color);
	
	/** \brief Lower angle. */
	inline float GetLower() const{ return pLower; }
	
	/** \brief Upper angle. */
	inline float GetUpper() const{ return pUpper; }
	
	/** \brief Set lower and upper angle. */
	void SetRange(float lower, float upper);
	
	/** \brief Disk radius. */
	inline float GetRadius() const{ return pRadius; }
	
	/** \brief Set disk radius. */
	void SetRadius(float radius);
	
	/** \brief Determines if the debug drawer shape is visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** \brief Sets if the debug drawer shape is visible. */
	void SetVisible(bool visible);
	/*@}*/
	
	
	
private:
	void pRebuildShape();
};

#endif
