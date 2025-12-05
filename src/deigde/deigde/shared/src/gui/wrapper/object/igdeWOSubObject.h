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

#ifndef _IGDEWOSUBOBJECT_H_
#define _IGDEWOSUBOBJECT_H_

#include "../../../triggersystem/igdeTriggerExpression.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class igdeWObject;
class igdeGameDefinition;
class igdeEnvironment;
class deLogger;
class igdeWOSOVisitor;

class deColliderComponent;
class deEngine;
class decLayerMask;


/**
 * \brief Object wrapper sub object.
 */
class DE_DLL_EXPORT igdeWOSubObject : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeWOSubObject> Ref;
	
	
private:
	igdeWObject &pWrapper;
	const decString pPrefix;
	decVector pBoxMinExtends;
	decVector pBoxMaxExtends;
	bool pHasBoxExtends;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object wrapper sub object. */
	igdeWOSubObject( igdeWObject &wrapper, const decString &prefix );
	
	/** \brief Clean up object wrapper sub object. */
	virtual ~igdeWOSubObject();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Wrapper. */
	inline igdeWObject &GetWrapper() const{ return pWrapper; }
	
	/** \brief Property prefix. */
	inline const decString &GetPrefix(){ return pPrefix; }
	
	/** \brief Box minimum extends. */
	inline const decVector GetBoxMinExtends() const{ return pBoxMinExtends; }
	
	/** \brief Box maximum extends. */
	inline const decVector &GetBoxMaxExtends() const{ return pBoxMaxExtends; }
	
	/** \brief Has box extends. */
	inline bool HasBoxExtends() const{ return pHasBoxExtends; }
	
	/** \brief Set extends. */
	void SetBoxExtends( const decVector &boxMinExtends, const decVector &boxMaxExtends );
	
	/** \brief Clear extends. */
	void ClearBoxExtends();
	
	/** \brief Reattach to colliders. */
	void ReattachToColliders();
	
	/** \brief Update parameters. */
	virtual void UpdateParameters() = 0;
	
	/** \brief Init triggers. */
	virtual void InitTriggers();
	
	/** \brief Update trigger. */
	virtual void UpdateTriggers();
	
	/** \brief Update visibility. */
	virtual void UpdateVisibility();
	
	/** \brief Layer masks changed. */
	virtual void UpdateLayerMasks();
	
	/** \brief Collision filter changed. */
	virtual void UpdateCollisionFilter();
	
	/** \brief Geometry changed. */
	virtual void UpdateGeometry();
	
	/** \brief Update collider response type. */
	virtual void UpdateColliderResponseType();
	
	/** \brief Collider user pointer changed. */
	virtual void ColliderUserPointerChanged();
	
	/** \brief Frame update. */
	virtual void Update( float elapsed );
	
	/** \brief Reset physics. */
	virtual void ResetPhysics();
	
	/** \brief Reset component textures. */
	virtual void ResetComponentTextures();
	
	/** \brief Camera changed. */
	virtual void CameraChanged();
	
	/** \brief Outline skin changed. */
	virtual void OutlineSkinChanged();
	
	/** \brief All sub components finished loading. */
	virtual void OnAllSubObjectsFinishedLoading() = 0;
	
	/** \brief Visit. */
	virtual void Visit( igdeWOSOVisitor &visitor ) = 0;
	
	/**
	 * \brief Sub object is visible.
	 * 
	 * Returns true if wrapped resources is visible. For example a component resource without
	 * valid model or with no texture containing a valid skin is invisible. This check does not
	 * include the visibility state of the object. Hence invisible content is still considered
	 * visible if it would be visible otherwise.
	 */
	virtual bool IsContentVisible();
	
	/**
	 * \brief Determine if all sub objects (direct or indirect) finished loading.
	 * 
	 * Required if world type sub objects are present. These report finished loading
	 * as soon as the world has been loaded to avoid waiting. Use this function to
	 * check if all sub objects inside this object fully finished loading.
	 */
	virtual bool AllSubObjectsFinishedLoading() const;
	/*@}*/
	
	
	
protected:
	virtual void AttachToCollider() = 0;
	virtual void DetachFromCollider() = 0;
	
	igdeEnvironment &GetEnvironment() const;
	igdeGameDefinition &GetGameDefinition() const;
	deLogger &GetLogger() const;
	deEngine &GetEngine() const;
	
	bool GetPropertyValue( const decString &name, decString &value ) const;
	decString GetStringProperty( const decString &name, const decString &defaultValue ) const;
	decVector GetVectorProperty( const decString &name, const decVector &defaultValue ) const;
	decVector2 GetVector2Property( const decString &name, const decVector2 &defaultValue ) const;
	decQuaternion GetRotationProperty( const decString &name, const decQuaternion &defaultValue ) const;
	decColor GetColor3Property( const decString &name, const decColor &defaultValue ) const;
	decColor GetColor4Property( const decString &name, const decColor &defaultValue ) const;
	bool GetBoolProperty( const decString &name, bool defaultValue ) const;
	float GetFloatProperty( const decString &name, float defaultValue ) const;
	int GetIntProperty( const decString &name, int defaultValue ) const;
	
	void pInitTrigger( igdeTriggerExpression::Ref &trigger, const decString &propertyName );
	void pClearTrigger( igdeTriggerExpression::Ref &trigger );
	
	deColliderComponent *GetAttachableColliderComponent() const;
	decDMatrix GetBoneMatrix( const decString &name ) const;
	decLayerMask LayerMaskFromInt( int mask ) const;
};

#endif
