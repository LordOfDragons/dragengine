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

#ifndef _IGDEGDCCOMPONENT_H_
#define _IGDEGDCCOMPONENT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/collider/deCollider.h>

#include "igdeGDCCTextureList.h"



/**
 * \brief Game Definition Component.
 * 
 * Contains information about a component used to display the element
 * inside the editor.
 */
class DE_DLL_EXPORT igdeGDCComponent : public deObject{
public:
	/** \brief Properties. */
	enum eProperties{
		epModel,
		epSkin,
		epRig,
		epAnimator,
		epPlaybackController,
		epOcclusionMesh,
		epAudioModel,
		epRenderEnvMap,
		epAffectsAudio,
		epAttachPosition,
		epAttachRotation,
		epLightShadowIgnore,
		epAnimation,
		epMove
	};
	
	
	
private:
	decString pModelPath;
	decString pSkinPath;
	decString pRigPath;
	decString pAnimatorPath;
	decString pOccMeshPath;
	decString pAudioModelPath;
	decString pPlaybackController;
	decString pAnimationPath;
	decString pMove;
	bool pDoNotScale;
	bool pStatic;
	bool pRenderEnvMap;
	bool pAffectsAudio;
	bool pPartialHide;
	bool pAttachTarget;
	bool pLightShadowIgnore;
	deCollider::eResponseType pColliderResponseType;
	decDVector pPosition;
	decQuaternion pOrientation;
	decString pBoneName;
	
	decString pPropertyNames[ epMove + 1 ];
	
	igdeGDCCTextureList pTextureList;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game definition component. */
	igdeGDCComponent();
	
	/** \brief Create copy of a game definition component. */
	igdeGDCComponent( const igdeGDCComponent &component );
	
	/** \brief Clean up game definition component. */
	virtual ~igdeGDCComponent();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Model path. */
	inline const decString &GetModelPath() const{ return pModelPath; }
	
	/** \brief Set model path. */
	void SetModelPath( const char *path );
	
	/** \brief Skin path. */
	inline const decString &GetSkinPath() const{ return pSkinPath; }
	
	/** \brief Set skin path. */
	void SetSkinPath( const char *path );
	
	/** \brief Rig path. */
	inline const decString &GetRigPath() const{ return pRigPath; }
	
	/** \brief Set rig path. */
	void SetRigPath( const char *path );
	
	/** \brief Animator path. */
	inline const decString &GetAnimatorPath() const{ return pAnimatorPath; }
	
	/** \brief Set animator path. */
	void SetAnimatorPath( const char *path );
	
	/** \brief Animation path. */
	inline const decString &GetAnimationPath() const{ return pAnimationPath; }
	
	/** \brief Set animation path. */
	void SetAnimationPath( const char *path );
	
	/** \brief Move. */
	inline const decString &GetMove() const{ return pMove; }
	
	/** \brief Set move. */
	void SetMove( const char *move );
	
	/** \brief Occlusion mesh path. */
	inline const decString &GetOcclusionMeshPath() const{ return pOccMeshPath; }
	
	/** \brief Set occlusion mesh path. */
	void SetOcclusionMeshPath( const char *path );
	
	/** \brief Audio model path. */
	inline const decString &GetAudioModelPath() const{ return pAudioModelPath; }
	
	/** \brief Set audio model path. */
	void SetAudioModelPath( const char *path );
	
	/** \brief Playback controller name. */
	inline const decString &GetPlaybackController() const{ return pPlaybackController; }
	
	/** \brief Set playback controller name. */
	void SetPlaybackController( const char *controller );
	
	/** \brief Determines if this component should not be scaled. */
	inline bool GetDoNotScale() const{ return pDoNotScale; }
	
	/** \brief Sets if this component should not be scaled. */
	void SetDoNotScale( bool doNotScale );
	
	/** \brief Determines if the component is static. */
	inline bool GetStatic() const{ return pStatic; }
	
	/** \brief Sets if the component is static. */
	void SetStatic( bool bStatic );
	
	/** \brief Determines if the component can be partially hidden. */
	inline bool GetPartialHide() const{ return pPartialHide; }
	
	/** \brief Sets if the component can be partially hidden. */
	void SetPartialHide( bool partiallyHidden );
	
	/** \brief Resourcec can be attached to component. */
	inline bool GetAttachTarget() const{ return pAttachTarget; }
	
	/** \brief Set if resources can be attached to component. */
	void SetAttachTarget( bool attachTarget );
	
	/** \brief Collider response type to use. */
	inline deCollider::eResponseType GetColliderResponseType() const{ return pColliderResponseType; }
	
	/** \brief Set collider response type to use. */
	void SetColliderResponseType( deCollider::eResponseType responseType );
	
	/** \brief Determines if the component is rendered into environment map probes. */
	inline bool GetRenderEnvMap() const{ return pRenderEnvMap; }
	
	/** \brief Sets if the component is rendered into environment map probes. */
	void SetRenderEnvMap( bool renderEnvMap );
	
	/** \brief Component affects audio. */
	inline bool GetAffectsAudio() const{ return pAffectsAudio; }
	
	/** \brief Ses if component affects audio. */
	void SetAffectsAudio( bool affectsAudio );
	
	/** \brief Component is ignored from shadows cast by lights present in the same object. */
	inline bool GetLightShadowIgnore() const{ return pLightShadowIgnore; }
	
	/** \brief Ses if component is ignored from shadows cast by lights present in the same object. */
	void SetLightShadowIgnore( bool lightShadowIgnore );
	
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decDVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Name of the bone to attach to or empty string. */
	inline const decString &GetBoneName() const{ return pBoneName; }
	
	/** \brief Set name of bone to attach to or empty string. */
	void SetBoneName( const char *boneName );
	
	/** \brief Determines if the name of a property is set or not (empty string). */
	bool IsPropertySet( int property ) const;
	
	/** \brief Name of the given property. */
	const decString &GetPropertyName( int property ) const;
	
	/** \brief Set name of the given property. */
	void SetPropertyName( int property, const char *name );
	
	/** \brief Determines if one or more properties use a name. */
	bool HasPropertyWithName( const char *name ) const;
	
	/** \brief List of textures. */
	inline igdeGDCCTextureList &GetTextureList(){ return pTextureList; }
	inline const igdeGDCCTextureList &GetTextureList() const{ return pTextureList; }
	/*@}*/
};

#endif
