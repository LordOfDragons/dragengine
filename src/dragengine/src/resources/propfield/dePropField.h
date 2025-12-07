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

#ifndef _DEPROPFIELD_H_
#define _DEPROPFIELD_H_

#include "../deResource.h"
#include "../../common/math/decMath.h"

class dePropFieldType;
class dePropFieldManager;
class dePropFieldGround;

class deBaseGraphicPropField;
class deBasePhysicsPropField;
class deBaseScriptingPropField;
class deWorld;


/**
 * \brief Prop Field.
 *
 * Prop fields define a large number of inexpensive models used to populate
 * with vegetation or props in general for improved visuals. These
 * props are not required to be rendered or simulated in a physical way for
 * the game to work. Prop fields have a couple of restrictions on how these
 * props are defined to create large numbers of props with little definition,
 * processing and memory overhead. Each prop field contains a list of types.
 */
class DE_DLL_EXPORT dePropField : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<dePropField> Ref;
	
	
private:
	decDVector pPosition;
	
	dePropFieldType **pTypes;
	int pTypeCount;
	int pTypeSize;
	
	deBaseGraphicPropField *pPeerGraphic;
	deBasePhysicsPropField *pPeerPhysics;
	deBaseScriptingPropField *pPeerScripting;
	
	deWorld *pParentWorld;
	dePropField *pLLWorldPrev;
	dePropField *pLLWorldNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new prop field. */
	dePropField(dePropFieldManager *manager);
	
protected:
	/**
	 * \brief Clean up prop field.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~dePropField();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition(const decDVector &position);
	
	/** \brief Number of types. */
	inline int GetTypeCount() const{ return pTypeCount; }
	
	/** \brief Type at the given index. */
	dePropFieldType *GetTypeAt(int index) const;
	
	/** \brief Index of the given type of -1 if not found. */
	int IndexOfType(dePropFieldType *type) const;
	
	/** \brief Adds a type. */
	void AddType(dePropFieldType *type);
	
	/** \brief Removes the given type. */
	void RemoveType(dePropFieldType *type);
	
	/** \brief Removes all types. */
	void RemoveAllTypes();
	
	/** \brief Notifies the peers that the given type changed. */
	void NotifyTypeChanged(int type);
	
	/** \brief Notifies the peers that the given type instances changed. */
	void NotifyInstancesChanged(int type);
	
	/** \brief Notifies the peers that the given type instances to bend states assignments changed. */
	void NotifyAssignmentsChanged(int type);
	
	/** \brief Notifies the peers that the given type bend states changed. */
	void NotifyBendStatesChanged(int type);
	
	/** \brief Notifies the graphic module that the ground changed. */
	void NotifyGroundChanged();
	
	/** \brief Requests the scripting module to create instances using the given density. */
	void NotifyCreateInstances(float density);
	
	/** \brief Requests the physics module to project all instances to the given ground. */
	void NotifyProjectInstances(const dePropFieldGround &ground, const decVector &direction);
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer object. */
	inline deBaseGraphicPropField *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer object. */
	void SetPeerGraphic(deBaseGraphicPropField *peer);
	
	/** \brief Physics system peer object. */
	inline deBasePhysicsPropField *GetPeerPhysics() const{ return pPeerPhysics; }
	
	/** \brief Set physics system peer object. */
	void SetPeerPhysics(deBasePhysicsPropField *peer);
	
	/** \brief Scripting system peer object. */
	inline deBaseScriptingPropField *GetPeerScripting() const{ return pPeerScripting; }
	
	/** \brief Set scripting system peer object. */
	void SetPeerScripting(deBaseScriptingPropField *peer);
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Parent world or NULL. */
	inline deWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld(deWorld *world);
	
	/** \brief Previous prop field in the parent world linked list. */
	inline dePropField *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set next prop field in the parent world linked list. */
	void SetLLWorldPrev(dePropField *propField);
	
	/** \brief Next prop field in the parent world linked list. */
	inline dePropField *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set next prop field in the parent world linked list. */
	void SetLLWorldNext(dePropField *propField);
	/*@}*/
};

#endif
