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

#ifndef _IGDECAMERA_H_
#define _IGDECAMERA_H_

#include "../meta/igdeMetaContext.h"
#include "../meta/property/igdeMetaPropertyBoolean.h"
#include "../meta/property/igdeMetaPropertyFloat.h"
#include "../meta/property/igdeMetaPropertyDVector.h"
#include "../meta/property/igdeMetaPropertyVector.h"
#include "../meta/property/igdeMetaPropertyString.h"
#include "../meta/property/igdeMetaPropertyCurveBezier.h"
#include "../meta/property/igdeMetaPropertyGroup.h"
#include "../gui/filedialog/igdeFilePattern.h"
#include "../undo/igdeUndoSystem.h"

#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/common/curve/decCurveBezier.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>

class deCamera;
class deEngine;
class deWorld;
class igdeEnvironment;



/**
 * \brief Camera for rendering a world.
 *
 * Manages a camera including the wrapped engine camera. Provides also
 * some simple unproject methods to determine a ray shot into the world
 * as seen by this camera. The camera is managed in a lazy way. Hence
 * the engine is not create or updated until it is acquired by the user.
 */
class DE_DLL_EXPORT igdeCamera : public deObject{
public:
	/** \brief Reference. */
	using Ref = deTObjectReference<igdeCamera>;
	
	
	/** \brief Meta context. */
	class DE_DLL_EXPORT MetaContext : public igdeMetaContext{
	private:
		igdeCamera *pCamera;
		igdeCamera::Ref pGuard;
		
	public:
		using Ref = deTObjectReference<MetaContext>;
		MetaContext(igdeCamera *camera);
		
		inline igdeCamera *GetCamera() const{ return pCamera; }
		igdeCamera &GetCameraRef() const;
		Ref Capture() const;
		igdeEnvironment &GetEnvironment() const override;
		igdeUndoSystem *GetUndoSystem() const override;
		igdeClipboard *GetClipboard() const override;
		
	protected:
		virtual ~MetaContext() override;
	};
	
	/** \brief Meta properties. */
	class DE_DLL_EXPORT MetaProperties{
	public:
		template <typename T> class TBase : public T{
		public:
			template <typename... A> TBase(A&&... args) : T(std::forward<A>(args)...) {}
			
			igdeMetaContext::Ref Capture(const igdeMetaContext::Ref &context) const override{
				return context.DynamicCast<MetaContext>()->Capture();
			}
			
			bool IsValid(const igdeMetaContext::Ref &context) const override{
				const auto c = context.DynamicCast<MetaContext>();
				return c && !c->IsDisposed();
			}
			
			inline igdeCamera &Camera(const igdeMetaContext::Ref &context) const{
				return context.DynamicCast<MetaContext>()->GetCameraRef();
			}
			
			void AddContextMenuEntries(igdeMenuCascade &contextMenu,
			const igdeMetaContext::Ref &context, igdeWidget &owner) override{
				Camera(context).AddContextMenuEntries(contextMenu, owner);
			}
			
		protected:
			virtual ~TBase() override{}
		};
		
		class DE_DLL_EXPORT Position : public TBase<igdeMetaPropertyDVectorStorage>{
		public:
			Position();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			
		protected:
			~Position() override;
		};
		
		class DE_DLL_EXPORT Rotation : public TBase<igdeMetaPropertyVectorStorageQuaternion>{
		public:
			Rotation();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			
		protected:
			~Rotation() override;
		};
		
		class DE_DLL_EXPORT Distance : public TBase<igdeMetaPropertyFloatStorage>{
		public:
			Distance();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			
		protected:
			~Distance() override;
		};
		
		class DE_DLL_EXPORT Fov : public TBase<igdeMetaPropertyFloatStorage>{
		public:
			Fov();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			
		protected:
			~Fov() override;
		};
		
		class DE_DLL_EXPORT FovRatio : public TBase<igdeMetaPropertyFloatStorage>{
		public:
			FovRatio();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			
		protected:
			~FovRatio() override;
		};
		
		class DE_DLL_EXPORT ImageDistance : public TBase<igdeMetaPropertyFloatStorage>{
		public:
			ImageDistance();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			
		protected:
			~ImageDistance() override;
		};
		
		class DE_DLL_EXPORT ViewDistance : public TBase<igdeMetaPropertyFloatStorage>{
		public:
			ViewDistance();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			
		protected:
			~ViewDistance() override;
		};
		
		class DE_DLL_EXPORT EnableHDRR : public TBase<igdeMetaPropertyBooleanStorage>{
		public:
			EnableHDRR();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			
		protected:
			~EnableHDRR() override;
		};
		
		class DE_DLL_EXPORT EnableGI : public TBase<igdeMetaPropertyBooleanStorage>{
		public:
			EnableGI();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			
		protected:
			~EnableGI() override;
		};
		
		class DE_DLL_EXPORT Exposure : public TBase<igdeMetaPropertyFloatStorage>{
		public:
			Exposure();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			
		protected:
			~Exposure() override;
		};
		
		class DE_DLL_EXPORT LowestIntensity : public TBase<igdeMetaPropertyFloatStorage>{
		public:
			LowestIntensity();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			
		protected:
			~LowestIntensity() override;
		};
		
		class DE_DLL_EXPORT HighestIntensity : public TBase<igdeMetaPropertyFloatStorage>{
		public:
			HighestIntensity();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			
		protected:
			~HighestIntensity() override;
		};
		
		class DE_DLL_EXPORT AdaptionTime : public TBase<igdeMetaPropertyFloatStorage>{
		public:
			AdaptionTime();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			
		protected:
			~AdaptionTime() override;
		};
		
		class DE_DLL_EXPORT WhiteIntensity : public TBase<igdeMetaPropertyFloatStorage>{
		public:
			WhiteIntensity();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			
		protected:
			~WhiteIntensity() override;
		};
		
		class DE_DLL_EXPORT BloomIntensity : public TBase<igdeMetaPropertyFloatStorage>{
		public:
			BloomIntensity();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			
		protected:
			~BloomIntensity() override;
		};
		
		class DE_DLL_EXPORT BloomStrength : public TBase<igdeMetaPropertyFloatStorage>{
		public:
			BloomStrength();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			
		protected:
			~BloomStrength() override;
		};
		
		class DE_DLL_EXPORT BloomBlend : public TBase<igdeMetaPropertyFloatStorage>{
		public:
			BloomBlend();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			
		protected:
			~BloomBlend() override;
		};
		
		class DE_DLL_EXPORT BloomSize : public TBase<igdeMetaPropertyFloatStorage>{
		public:
			BloomSize();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			
		protected:
			~BloomSize() override;
		};
		
		class DE_DLL_EXPORT ToneMapCurve : public TBase<igdeMetaPropertyCurveBezierStorage>{
		public:
			ToneMapCurve();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			
		protected:
			~ToneMapCurve() override;
		};
		
	private:
		MetaProperties();
		
	public:
		deTObjectReference<Position> position;
		deTObjectReference<Rotation> rotation;
		deTObjectReference<Distance> distance;
		deTObjectReference<EnableHDRR> enableHDRR;
		deTObjectReference<EnableGI> enableGI;
		
		deTObjectReference<Fov> fov;
		deTObjectReference<FovRatio> fovRatio;
		deTObjectReference<ImageDistance> imageDistance;
		deTObjectReference<ViewDistance> viewDistance;
		deTObjectReference<igdeMetaPropertyGroup> groupInternal;
		
		deTObjectReference<Exposure> exposure;
		deTObjectReference<LowestIntensity> lowestIntensity;
		deTObjectReference<HighestIntensity> highestIntensity;
		deTObjectReference<AdaptionTime> adaptionTime;
		deTObjectReference<igdeMetaPropertyGroup> groupExposure;
		
		deTObjectReference<WhiteIntensity> whiteIntensity;
		deTObjectReference<ToneMapCurve> toneMapCurve;
		deTObjectReference<igdeMetaPropertyGroup> groupToneMap;
		
		deTObjectReference<BloomIntensity> bloomIntensity;
		deTObjectReference<BloomStrength> bloomStrength;
		deTObjectReference<BloomBlend> bloomBlend;
		deTObjectReference<BloomSize> bloomSize;
		deTObjectReference<igdeMetaPropertyGroup> groupBloom;
		
		const igdeMetaContext::PropertyList::Ref properties;
		
		static MetaProperties global;
	};
	
	
private:
	igdeEnvironment &pEnvironment;
	igdeUndoSystem *pUndoSystem;
	
	MetaContext::Ref pMetaContext;
	
	decString pName;
	deEngine *pEngine;
	deCamera::Ref pEngCamera;
	deWorld::Ref pEngWorld;
	
	decDMatrix pViewMatrix;
	
	igdeMetaPropertyDVectorStorage::Storage pMPPosition;
	igdeMetaPropertyVectorStorageQuaternion::Storage pMPRotation;
	igdeMetaPropertyFloatStorage::Storage pMPDistance;
	igdeMetaPropertyFloatStorage::Storage pMPFov;
	igdeMetaPropertyFloatStorage::Storage pMPFovRatio;
	igdeMetaPropertyFloatStorage::Storage pMPImageDistance;
	igdeMetaPropertyFloatStorage::Storage pMPViewDistance;
	igdeMetaPropertyBooleanStorage::Storage pMPEnableHDRR;
	igdeMetaPropertyBooleanStorage::Storage pMPEnableGI;
	igdeMetaPropertyFloatStorage::Storage pMPExposure;
	igdeMetaPropertyFloatStorage::Storage pMPLowestIntensity;
	igdeMetaPropertyFloatStorage::Storage pMPHighestIntensity;
	igdeMetaPropertyFloatStorage::Storage pMPAdaptionTime;
	igdeMetaPropertyFloatStorage::Storage pMPWhiteIntensity;
	igdeMetaPropertyFloatStorage::Storage pMPBloomIntensity;
	igdeMetaPropertyFloatStorage::Storage pMPBloomStrength;
	igdeMetaPropertyFloatStorage::Storage pMPBloomBlend;
	igdeMetaPropertyFloatStorage::Storage pMPBloomSize;
	igdeMetaPropertyCurveBezierStorage::Storage pMPToneMapCurve;
	
public:
	/** \brief Camera file pattern list. */
	static const igdeFilePattern::List patternCamera;
	
	/** \brief Last used camera file. */
	static decString lastCameraFile;
	
	/** \brief Camera changed event. */
	igdeTEvent<> onChanged;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create camera. */
	igdeCamera(igdeEnvironment &environment, deEngine *engine);
	
	igdeCamera(const igdeCamera &) = delete;
	igdeCamera(igdeCamera &&) = delete;
	igdeCamera &operator=(const igdeCamera &) = delete;
	igdeCamera &operator=(igdeCamera &&) = delete;
	
protected:
	/** \brief Clean up camera object. */
	~igdeCamera() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Meta property position. */
	inline igdeMetaPropertyDVectorStorage::Storage &GetMPPosition(){ return pMPPosition; }
	
	/** \brief Meta property rotation. */
	inline igdeMetaPropertyVectorStorageQuaternion::Storage &GetMPRotation(){ return pMPRotation; }
	
	/** \brief Meta property distance. */
	inline igdeMetaPropertyFloatStorage::Storage &GetMPDistance(){ return pMPDistance; }
	
	/** \brief Meta property field of view. */
	inline igdeMetaPropertyFloatStorage::Storage &GetMPFov(){ return pMPFov; }
	
	/** \brief Meta property field of view ratio. */
	inline igdeMetaPropertyFloatStorage::Storage &GetMPFovRatio(){ return pMPFovRatio; }
	
	/** \brief Meta property image distance. */
	inline igdeMetaPropertyFloatStorage::Storage &GetMPImageDistance(){ return pMPImageDistance; }
	
	/** \brief Meta property view distance. */
	inline igdeMetaPropertyFloatStorage::Storage &GetMPViewDistance(){ return pMPViewDistance; }
	
	/** \brief Meta property enable HDRR. */
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPEnableHDRR(){ return pMPEnableHDRR; }
	
	/** \brief Meta property enable GI. */
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPEnableGI(){ return pMPEnableGI; }
	
	/** \brief Meta property exposure. */
	inline igdeMetaPropertyFloatStorage::Storage &GetMPExposure(){ return pMPExposure; }
	
	/** \brief Meta property lowest intensity. */
	inline igdeMetaPropertyFloatStorage::Storage &GetMPLowestIntensity(){ return pMPLowestIntensity; }
	
	/** \brief Meta property highest intensity. */
	inline igdeMetaPropertyFloatStorage::Storage &GetMPHighestIntensity(){ return pMPHighestIntensity; }
	
	/** \brief Meta property adaption time. */
	inline igdeMetaPropertyFloatStorage::Storage &GetMPAdaptionTime(){ return pMPAdaptionTime; }
	
	/** \brief Meta property white intensity. */
	inline igdeMetaPropertyFloatStorage::Storage &GetMPWhiteIntensity(){ return pMPWhiteIntensity; }
	
	/** \brief Meta property bloom intensity. */
	inline igdeMetaPropertyFloatStorage::Storage &GetMPBloomIntensity(){ return pMPBloomIntensity; }
	
	/** \brief Meta property bloom strength. */
	inline igdeMetaPropertyFloatStorage::Storage &GetMPBloomStrength(){ return pMPBloomStrength; }
	
	/** \brief Meta property bloom blend. */
	inline igdeMetaPropertyFloatStorage::Storage &GetMPBloomBlend(){ return pMPBloomBlend; }
	
	/** \brief Meta property bloom size. */
	inline igdeMetaPropertyFloatStorage::Storage &GetMPBloomSize(){ return pMPBloomSize; }
	
	/** \brief Meta property tone map curve. */
	inline igdeMetaPropertyCurveBezierStorage::Storage &GetMPToneMapCurve(){ return pMPToneMapCurve; }
	
	
	/** \brief Environment. */
	inline igdeEnvironment &GetEnvironment() const{ return pEnvironment; }
	
	/** \brief Undo system or nullptr. */
	inline igdeUndoSystem *GetUndoSystem() const{ return pUndoSystem; }
	
	/** \brief Set undo system or nullptr. */
	void SetUndoSystem(igdeUndoSystem *undoSystem);
	
	/** \brief Meta context. */
	inline const MetaContext::Ref &GetMetaContext() const{ return pMetaContext; }
	
	/** \brief Game engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	
	/** \brief Engine camera. */
	inline const deCamera::Ref &GetEngineCamera() const{ return pEngCamera; }
	
	/** \brief World the camera is attached to. */
	inline const deWorld::Ref &GetEngineWorld() const{ return pEngWorld; }
	
	/** \brief Attach camera to world. */
	void SetEngineWorld(deWorld *world);
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName(const char *name);
	
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pMPPosition; }
	
	/** \brief Set position. */
	void SetPosition(const decDVector &position);
	
	/** \brief Orientation of the camera. */
	inline const decVector &GetOrientation() const{ return pMPRotation; }
	
	/** \brief Set orientation of the camera. */
	void SetOrientation(const decVector &orientation);
	
	/** \brief Field of view in radians. */
	inline float GetFov() const{ return pMPFov; }
	
	/** \brief Set field of view in radians. */
	void SetFov(float fov);
	
	/** \brief Aspect ratio of the horizontal field of view to the vertical field of view. */
	inline float GetFovRatio() const{ return pMPFovRatio; }
	
	/** \brief Set aspect ratio of the horizonral field of view to the vertical field of view. */
	void SetFovRatio(float ratio);
	
	/** \brief Distance to the image plane. */
	inline float GetImageDistance() const{ return pMPImageDistance; }
	
	/** \brief Set distance to the image plane. */
	void SetImageDistance(float distance);
	
	/** \brief View distance. */
	inline float GetViewDistance() const{ return pMPViewDistance; }
	
	/** \brief Set view distance. */
	void SetViewDistance(float viewDistance);
	
	
	
	/** \brief Enable high definition range rendering (HDRR) if supported. */
	inline bool GetEnableHDRR() const{ return pMPEnableHDRR; }
	
	/** \brief Set to enable high definition range rendering (HDRR) if supported. */
	void SetEnableHDRR(bool enable);
	
	/** \brief Exposure. */
	inline float GetExposure() const{ return pMPExposure; }
	
	/** \brief Set exposure. */
	void SetExposure(float exposure);
	
	/** \brief Lowest intensity the eye can adapt to. */
	inline float GetLowestIntensity() const{ return pMPLowestIntensity; }
	
	/** \brief Set lowest intensity the eye can adapt to. */
	void SetLowestIntensity(float lowestIntensity);
	
	/** \brief Highest intensity the eye can adapt to. */
	inline float GetHighestIntensity() const{ return pMPHighestIntensity; }
	
	/** \brief Set highest intensity the eye can adapt to. */
	void SetHighestIntensity(float highestIntensity);
	
	/** \brief Adaption time of the eye in seconds. */
	inline float GetAdaptionTime() const{ return pMPAdaptionTime; }
	
	/** \brief Set adaption time of the eye in seconds. */
	void SetAdaptionTime(float adaptionTime);
	
	/** \brief Distance of camera to the center point along the view direction. */
	inline float GetDistance() const{ return pMPDistance; }
	
	/** \brief Set distance of camera to the center point along the view direction. */
	void SetDistance(float distance);
	
	
	
	/** \brief Enable global illumination (GI) if supported. */
	inline bool GetEnableGI() const{ return pMPEnableGI; }
	
	/** \brief Set to enable global illumination (GI) if supported. */
	void SetEnableGI(bool enable);
	
	
	
	/**
	 * \brief White intensity multiplier.
	 * \version 1.21
	 */
	inline float GetWhiteIntensity() const{ return pMPWhiteIntensity; }
	
	/**
	 * \brief Set white intensity multiplier.
	 * \version 1.21
	 */
	void SetWhiteIntensity(float intensity);
	
	/**
	 * \brief Bloom intensity multiplier.
	 * \version 1.21
	 */
	inline float GetBloomIntensity() const{ return pMPBloomIntensity; }
	
	/**
	 * \brief Set bloom intensity multiplier.
	 * \version 1.21
	 */
	void SetBloomIntensity(float intensity);
	
	/**
	 * \brief Bloom strength as multiplier of intensity beyond bloom intensity.
	 * \version 1.21
	 */
	inline float GetBloomStrength() const{ return pMPBloomStrength; }
	
	/**
	 * \brief Set bloom strength as multiplier of intensity beyond bloom intensity.
	 * \version 1.21
	 */
	void SetBloomStrength(float strength);
	
	/**
	 * \brief Bloom blend as multiplier of intensity beyond bloom intensity.
	 * \version 1.21
	 */
	inline float GetBloomBlend() const{ return pMPBloomBlend; }
	
	/**
	 * \brief Set bloom blend as multiplier of intensity beyond bloom intensity.
	 * \version 1.21
	 */
	void SetBloomBlend(float blend);
	
	/**
	 * \brief Bloom size as percentage of screen width.
	 * \version 1.21
	 */
	inline float GetBloomSize() const{ return pMPBloomSize; }
	
	/**
	 * \brief Bloom size as percentage of screen width.
	 * \version 1.21
	 */
	void SetBloomSize(float size);
	
	
	
	/**
	 * \brief Custom tone mapping curve or empty curve to disable.
	 * \version 1.21
	 */
	inline const decCurveBezier &GetToneMapCurve() const{ return pMPToneMapCurve; }
	
	/**
	 * \brief Set custom tone mapping curve or empty curve to disable.
	 * \version 1.21
	 * \note If enabled make sure to set the matching white intensity as it defaults to 4.
	 */
	void SetToneMapCurve(const decCurveBezier &curve);
	
	
	
	/** \brief View matrix. */
	inline const decDMatrix &GetViewMatrix() const{ return pViewMatrix; }
	
	/**
	 * \brief Direction of ray shot from camera position through point on image plane.
	 * 
	 * The width and height are the size of the screen.
	 */
	decVector GetDirectionFor(int width, int height, int x, int y) const;
	
	/**
	 * \brief Set default parameters.
	 * \version 1.21
	 */
	void SetDefaultParameters(float lowestIntensity, float highestIntensity, float adaptionTime);
	
	
	/** \brief Add shared camera context menu entries. */
	void AddContextMenuEntries(igdeMenuCascade &menu, igdeWidget &owner);
	
	
	/** \brief Reset camera. */
	virtual void Reset();
	
	/** \brief World changed. */
	virtual void WorldChanged();
	
	/** \brief Geometry changed. */
	virtual void GeometryChanged();
	
	/** \brief Parameter changed. */
	virtual void ParameterChanged();
	
	/** \brief Adaption changed. */
	virtual void AdaptionChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pUpdateCameraPosition();
	void pUpdateViewMatrix();
};

#endif
