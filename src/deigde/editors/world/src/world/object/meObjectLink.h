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

#ifndef _MEOBJECTLINK_H_
#define _MEOBJECTLINK_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/debug/deDebugDrawer.h>

class meWorld;
class meObject;

class igdeEnvironment;
class igdeWConnection;

class deEngine;
class deDebugDrawerShape;



/**
 * \brief Draws a link from one object to another.
 */
class meObjectLink : public deObject{
public:
	typedef deTObjectReference<meObjectLink> Ref;
	typedef decTObjectOrderedSet<meObjectLink> List;
	
	
private:
	igdeEnvironment *pEnvironment;
	meWorld *pWorld;
	
	deDebugDrawer::Ref pDebugDrawer;
	igdeWConnection *pDDSConnection;
	
	deTObjectReference<meObject> pAnchor, pTarget;
	decString pAnchorProperty, pTargetProperty;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new object link. */
	meObjectLink(igdeEnvironment *environment, meObject *anchor, meObject *target);
	
protected:
	/** \brief Cleans up an object link. */
	virtual ~meObjectLink();
	
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the environment. */
	inline igdeEnvironment *GetEnvironment() const{ return pEnvironment; }
	
	/** \brief Retrieves the anchor object. */
	inline const deTObjectReference<meObject> &GetAnchor() const{ return pAnchor; }
	
	/** \brief Retrieves the target object. */
	inline const deTObjectReference<meObject> &GetTarget() const{ return pTarget; }
	
	/** \brief Retrieves the anchor property name. */
	inline const decString &GetAnchorProperty() const{ return pAnchorProperty; }
	/** \brief Sets the anchor property name. */
	void SetAnchorProperty(const char *property);
	/** \brief Retrieves the target property name. */
	inline const decString &GetTargetProperty() const{ return pTargetProperty; }
	/** \brief Sets the target property name. */
	void SetTargetProperty(const char *property);
	
	/** \brief Retrieves the world this link resides in. */
	inline meWorld *GetWorld() const{ return pWorld; }
	/** \brief Sets the world this link resides in. */
	void SetWorld(meWorld *world);
	
	/** \brief Notifies that the anchor or target moved. */
	void ObjectsMoved();
	
	/** \brief Update link color based on anchor active state. */
	void UpdateColor();
	/*@}*/
	
private:
	void pCleanUp();
	void pUpdateConnection();
};

#endif
