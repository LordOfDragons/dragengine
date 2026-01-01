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

#ifndef _GDEOCSNAPPOINT_H_
#define _GDEOCSNAPPOINT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief Object class snap point.
 */
class gdeOCSnapPoint : public deObject{
private:
	decString pName;
	
	decDVector pPosition;
	decVector pRotation;
	
	float pSnapDistance;
	bool pSnapToRotation;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<gdeOCSnapPoint> Ref;

	/** \brief Type holding list. */
	typedef decTObjectOrderedSet<gdeOCSnapPoint> List;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create snap point. */
	gdeOCSnapPoint();
	
	/** \brief Create copy of snap point. */
	gdeOCSnapPoint(const gdeOCSnapPoint &snapPoint);
	
	/** \brief Clean up snap point. */
protected:
	virtual ~gdeOCSnapPoint();
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Display name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set display name. */
	void SetName(const char *name);
	
	
	
	/** \brief Snap position relative to parent object. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set snap position relative to parent object. */
	void SetPosition(const decDVector &position);
	
	/** \brief Snap orientation relative to parent object. */
	inline const decVector &GetRotation() const{ return pRotation; }
	
	/** \brief Set snap orientation relative to parent object. */
	void SetRotation(const decVector &orientation);
	
	
	
	/** \brief Snap distance in meters. */
	inline float GetSnapDistance() const{ return pSnapDistance; }
	
	/** \brief Set snap distance in meters. */
	void SetSnapDistance(float distance);
	
	/** \brief Snap to orientation. */
	inline bool GetSnapToRotation() const{ return pSnapToRotation; }
	
	/** \brief Set snap to orientation. */
	void SetSnapToRotation(bool snapToRotation);
	/*@}*/
};

#endif
