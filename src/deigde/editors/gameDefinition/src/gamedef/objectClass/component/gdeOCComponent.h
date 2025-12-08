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

#ifndef _GDEOCCOMPONENT_H_
#define _GDEOCCOMPONENT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/collider/deCollider.h>

#include "gdeOCComponentTextureList.h"



/**
 * \brief Object class component.
 */
class gdeOCComponent : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<gdeOCComponent> Ref;
	
	
	/** \brief Properties. */
	enum eProperties{
		/** \brief Model path. */
		epModel,
		
		/** \brief Skin path. */
		epSkin,
		
		/** \brief Rig path. */
		epRig,
		
		/** \brief Animator path. */
		epAnimator,
		
		/** \brief Playback controller. */
		epPlaybackController,
		
		/** \bief Occlusion mesh path. */
		epOcclusionMesh,
		
		/** \bief Audio model path. */
		epAudioModel,
		
		/** \brief Render in environment map. */
		epRenderEnvMap,
		
		/** \brief Affects audio. */
		epAffectsAudio,
		
		/** \brief Attach position. */
		epAttachPosition,
		
		/** \brief Attach rotation. */
		epAttachRotation,
		
		/** \brief Component is ignore from shadow casting of lights on the same object. */
		epLightShadowIgnore,
		
		/** \brief Animation path. */
		epAnimation,
		
		/** \brief Move. */
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
	decVector pPosition;
	decVector pRotation;
	decString pBoneName;
	
	decString pPropertyNames[epMove + 1];
	
	gdeOCComponentTextureList pTextures;
	gdeOCComponentTexture::Ref pActiveTexture;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create component. */
	gdeOCComponent();
	
	/** \brief Create copy of component. */
	gdeOCComponent(const gdeOCComponent &component);
	
	/** \brief Clean up component. */
	virtual ~gdeOCComponent();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Model path. */
	inline const decString &GetModelPath() const{ return pModelPath; }
	
	/** \brief Set model path. */
	void SetModelPath(const char *path);
	
	/** \brief Skin path. */
	inline const decString &GetSkinPath() const{ return pSkinPath; }
	
	/** \brief Set skin path. */
	void SetSkinPath(const char *path);
	
	/** \brief Rig path. */
	inline const decString &GetRigPath() const{ return pRigPath; }
	
	/** \brief Set rig path. */
	void SetRigPath(const char *path);
	
	/** \brief Animator path. */
	inline const decString &GetAnimatorPath() const{ return pAnimatorPath; }
	
	/** \brief Set animator path. */
	void SetAnimatorPath(const char *path);
	
	/** \brief Animation path. */
	inline const decString &GetAnimationPath() const{ return pAnimationPath; }
	
	/** \brief Set animation path. */
	void SetAnimationPath(const char *path);
	
	/** \brief Move. */
	inline const decString &GetMove() const{ return pMove; }
	
	/** \brief Set move. */
	void SetMove(const char *move);
	
	/** \brief Occlusion mesh path. */
	inline const decString &GetOcclusionMeshPath() const{ return pOccMeshPath; }
	
	/** \brief Set occlusion mesh path. */
	void SetOcclusionMeshPath(const char *path);
	
	/** \brief Audio model path. */
	inline const decString &GetAudioModelPath() const{ return pAudioModelPath; }
	
	/** \brief Set audio model path. */
	void SetAudioModelPath(const char *path);
	
	/** \brief Playback controller name. */
	inline const decString &GetPlaybackController() const{ return pPlaybackController; }
	
	/** \brief Set playback controller name. */
	void SetPlaybackController(const char *controller);
	
	/** \brief Component should not be scaled. */
	inline bool GetDoNotScale() const{ return pDoNotScale; }
	
	/** \brief Set if component should not be scaled. */
	void SetDoNotScale(bool doNotScale);
	
	/** \brief Component is static. */
	inline bool GetStatic() const{ return pStatic; }
	
	/** \brief Set if component is static. */
	void SetStatic(bool bStatic);
	
	/** \brief Component can be partially hidden. */
	inline bool GetPartialHide() const{ return pPartialHide; }
	
	/** \brief Set if component can be partially hidden. */
	void SetPartialHide(bool partiallyHidden);
	
	/** \brief Collider response type to use. */
	inline deCollider::eResponseType GetColliderResponseType() const{ return pColliderResponseType; }
	
	/** \brief Set collider response type to use. */
	void SetColliderResponseType(deCollider::eResponseType responseType);
	
	/** \brief Resource can be attached to component. */
	inline bool GetAttachTarget() const{ return pAttachTarget; }
	
	/** \brief Set if resources can be attached to component. */
	void SetAttachTarget(bool attachTarget);
	
	
	
	/** \brief Component is rendered into environment map probes. */
	inline bool GetRenderEnvMap() const{ return pRenderEnvMap; }
	
	/** \brief Set if component is rendered into environment map probes. */
	void SetRenderEnvMap(bool renderEnvMap);
	
	/** \brief Component affects audio. */
	inline bool GetAffectsAudio() const{ return pAffectsAudio; }
	
	/** \brief Set if component affects audio. */
	void SetAffectsAudio(bool affectsAudio);
	
	/** \brief Component is ignore from shadow casting of lights on the same object. */
	inline bool GetLightShadowIgnore() const{ return pLightShadowIgnore; }
	
	/** \brief Set if component is ignored from shadow casting of lights on the same object. */
	void SetLightShadowIgnore(bool lightShadowIgnore);
	
	
	
	/** \brief Position relative to parent object. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Sets position relative to parent object. */
	void SetPosition(const decVector &position);
	
	/** \brief Rotation relative to parent object. */
	inline const decVector &GetRotation() const{ return pRotation; }
	
	/** \brief Set orientation relative to parent object. */
	void SetRotation(const decVector &orientation);
	
	
	
	/** \brief Name of the bone to attach to or empty string. */
	inline const decString &GetBoneName() const{ return pBoneName; }
	
	/** \brief Set name of the bone to attach to or empty string. */
	void SetBoneName(const char *boneName);
	
	
	
	/** \brief Name of a property is set (not empty string). */
	bool IsPropertySet(eProperties property) const;
	
	/** \brief Name of the given property. */
	const decString &GetPropertyName(eProperties property) const;
	
	/** \brief Set name of the given property. */
	void SetPropertyName(eProperties property, const char *name);
	
	/** \brief One or more properties use a name. */
	bool HasPropertyWithName(const char *name) const;
	
	
	
	/** \brief List of textures. */
	inline gdeOCComponentTextureList &GetTextures(){ return pTextures; }
	inline const gdeOCComponentTextureList &GetTextures() const{ return pTextures; }
	
	/** \brief Active texture or \em NULL if none. */
	inline gdeOCComponentTexture *GetActiveTexture() const{ return pActiveTexture; }
	
	/** \brief Set active texture or \em NULL if none. */
	void SetActiveTexture(gdeOCComponentTexture::Ref texture);
	/*@}*/
};

#endif
