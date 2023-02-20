/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEWOSUBOBJECT_H_
#define _IGDEWOSUBOBJECT_H_

#include "../../../triggersystem/igdeTriggerExpressionReference.h"

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
	
	void pInitTrigger( igdeTriggerExpressionReference &trigger, const decString &propertyName );
	void pClearTrigger( igdeTriggerExpressionReference &trigger );
	
	deColliderComponent *GetAttachableColliderComponent() const;
	decDMatrix GetBoneMatrix( const decString &name ) const;
	decLayerMask LayerMaskFromInt( int mask ) const;
};

#endif
