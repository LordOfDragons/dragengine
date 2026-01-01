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

#ifndef _IGDEGDCNAVIGATIONSPACE_H_
#define _IGDEGDCNAVIGATIONSPACE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>



/**
 * \brief Game definition class navigation space.
 */
class DE_DLL_EXPORT igdeGDCNavigationSpace : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeGDCNavigationSpace> Ref;
	
	/** \brief Type holding list of objects. */
	typedef decTObjectOrderedSet<igdeGDCNavigationSpace> List;
	
	
	/** \brief Properties. */
	enum eProperties{
		epPath,
		epBlockerShape,
		epLayer,
		epBlockingPriority,
		epSnapDistance,
		epSnapAngle,
		epAttachPosition,
		epAttachRotation
	};
	
	
	
private:
	decString pPath;
	decVector pPosition;
	decQuaternion pOrientation;
	decString pBoneName;
	int pLayer;
	float pSnapDistance;
	float pSnapAngle;
	deNavigationSpace::eSpaceTypes pType;
	
	int pBlockingPriority;
	decShapeList pBlockerShapeList;
	
	decString pPropertyNames[epAttachRotation + 1];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class navigation space. */
	igdeGDCNavigationSpace();
	
	/** \brief Create copy of a game definition class navigation space. */
	igdeGDCNavigationSpace(const igdeGDCNavigationSpace &space);
	
protected:
	/** \brief Clean up class navigation space. */
	virtual ~igdeGDCNavigationSpace();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Path to use. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Set path to use. */
	void SetPath(const char *path);
	
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition(const decVector &position);
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation(const decQuaternion &orientation);
	
	/** \brief Name of the bone to attach to or empty string. */
	inline const decString &GetBoneName() const{ return pBoneName; }
	
	/** \brief Set name of bone to attach to or empty string. */
	void SetBoneName(const char *boneName);
	
	/** \brief Layer number. */
	inline int GetLayer() const{ return pLayer; }
	
	/** \brief Set layer number. */
	void SetLayer(int layer);
	
	/** \brief Space type. */
	inline deNavigationSpace::eSpaceTypes GetType() const{ return pType; }
	
	/** \brief Set space type. */
	void SetType(deNavigationSpace::eSpaceTypes type);
	
	/**
	 * \brief Blocking priority.
	 * 
	 * Navigation meshes is blocked by all blockers with an equal or larger blocking priority.
	 * If a blocker shape exists it blocks all navigation meshes with equal or less blocking priority.
	 */
	inline int GetBlockingPriority() const{ return pBlockingPriority; }
	
	/**
	 * \brief Set blocking priority.
	 * 
	 * Navigation meshes is blocked by all blockers with an equal or larger blocking priority.
	 * If a blocker shape exists it blocks all navigation meshes with equal or less blocking priority.
	 */
	void SetBlockingPriority(int priority);
	
	/** \brief Blocker shape list. */
	inline const decShapeList &GetBlockerShapeList() const{ return pBlockerShapeList; }
	
	/** \brief Set blocker shape list. */
	void SetBlockerShapeList(const decShapeList &shapeList);
	
	/** \brief Snap distance. */
	inline float GetSnapDistance() const{ return pSnapDistance; }
	
	/** \brief Set snap distance. */
	void SetSnapDistance(float distance);
	
	/** \brief Snap angle. */
	inline float GetSnapAngle() const{ return pSnapAngle; }
	
	/** \brief Set snap angle. */
	void SetSnapAngle(float angle);
	
	/** \brief Determines if a property name is set or not (empty string). */
	bool IsPropertySet(eProperties property) const;
	
	/** \brief Name of the given property. */
	const decString &GetPropertyName(eProperties property) const;
	
	/** \brief Set name of the given property. */
	void SetPropertyName(eProperties property, const char *name);
	
	/** \brief Determines if one or more properties use a name. */
	bool HasPropertyWithName(const char *name) const;
	/*@}*/
};

#endif
