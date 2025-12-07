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

#ifndef _DECAMERA_H_
#define _DECAMERA_H_

#include "../deResource.h"
#include "../../common/math/decMath.h"
#include "../../common/utils/decLayerMask.h"
#include "../../common/curve/decCurveBezier.h"

class deWorld;
class deEffect;
class deEffectChain;
class deCameraManager;
class deBaseGraphicCamera;


/**
 * \brief World camera.
 *
 * A world camera defines the camera parameters used for rendering a world. Cameras have
 * various parameters useful to alter the way the world is rendered.
 * 
 * The lower and upper intensity parameter indicates the range of intensity the camera can
 * automatically adapt to. The chosen intensity level for a camera is the "scene intensity"
 * (or "average intensity"). This intensity is mapped to a reasonable middle gray value in
 * the final image.
 * 
 * The exposure is used to increase or decrease the overall luminance of the rendered world.
 * This alters the scene intensity mapping to the middle gray value. Use values lower than 1
 * to make the image darker and values higher than 1 to brighten up the image. Reducing the
 * exposure usually decreases also the contrast which can make images more pleasant.
 *
 * Using the "scene intensity" a "maximum intensity" is determined for the image. The maximum
 * intensity maps to pure white in the final image. Using the white intensity parameter this
 * intensity can be shifted to avoid bright parts of the image to wash out to white. This can
 * happen when the camera is located inside a room while looking out into the sun light.
 * The behavior of the white intensity parameter depends on the presence of custom tone
 * mapping. If custom tone mapping curve is not used the white intensity parameter alters
 * the tone mapping parameters used by the graphic module to reduce the washing out to white.
 * If custom tone mapping curve is used the white intensity parameter indicates the upper
 * bound of the input send through the curve. Pixel intensities above the white intensity
 * are thus clamped to the white intensity. The default value is 2.
 * 
 * In games overbrighting is often used as a gameplay element to simulate very bright pixel
 * for example an energy beam or entering a room with glaring light while coming out of night
 * light conditions. Overbrighting can be done in two ways. For overbrighting affecting the
 * entire screen like for example flashbangs using a post processing color matrix effect is
 * the best choice. For overbrighting on a per pixel basis blooming is typically used. For
 * this intensities of pixels are clamped against a "bloom intensity threshold". Intensites
 * above this threshold are used to create halo light effects similar to oversatured rods
 * in the eye. The effect is controlled by the bloom intensity, bloom strength, bloom blend
 * and bloom size parameter.
 * 
 * The bloom intensity is a multiplier applied to the current "maximum intensity" of the camera.
 * The default value is 1. To produce an overbright effect pixel requires an intensity 1 times
 * as bright than the current camera maximum intensity.
 * 
 * The bloom strength is a multiplier applied to the overbright intensity beyond the bloom intensity
 * threshold. For example if the threshold is 1 and the pixel intensity is 2 then the overbright
 * intensity is 1 and is multiplied with the strength factor. This allows to modify the amount of
 * intensity required to result in a strong glare. The default value is 0.25
 * 
 * The bloom blend is a multiplier applied to the overbright calculated for a pixel.
 * This is typically used to blend between using bloom (1) and not using bloom (0).
 * 
 * Blooming usually spreads due to rods oversaturating which is a chemical process in the eye.
 * The bloom size indicates the broadness of the blur as percentage of the screen width.
 * 
 * Optionally a custom tone mapping curve can be defined. The custom tone mapping curve is
 * used if it has at least one curve point. The default custom tone mapping curve is empty.
 * 
 * \todo Add option to define render mode:
 * - ermPerspective: Regular perspective rendering (default)
 * - ermOrthogonal: Orthogonal rendering. fov and fov ratio are used to define the render
 * size. or add new parameters
 * - ermOmniDirection: Render 360 view. output is a cube-map. for dynamic skins no addiotnal
 * work has to be done. for canvas render world the canvas has to have
 * a depth of 6. if the depth 1 one the output is converted into a
 * single image mercartor map. this is though not the default use case.
 * => another option is outputing an equirectangular image. this works well with various
 *    texture properties and especially works well with canvas systems since it is an
 *    image with depth 1 as most other images are
 */
class DE_DLL_EXPORT deCamera : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deCamera> Ref;
	
	
private:
	decDVector pPosition;
	decQuaternion pOrientation;
	
	float pFov;
	float pFovRatio;
	float pImageDistance;
	float pViewDistance;
	
	bool pEnableHDRR;
	float pExposure;
	float pLowestIntensity;
	float pHighestIntensity;
	float pAdaptionTime;
	
	bool pEnableGI;
	
	float pWhiteIntensity;
	float pBloomIntensity;
	float pBloomStrength;
	float pBloomBlend;
	float pBloomSize;
	
	decCurveBezier pToneMapCurve;
	
	deEffectChain *pEffects;
	
	decLayerMask pLayerMask;
	
	deBaseGraphicCamera *pPeerGraphic;
	
	deWorld *pParentWorld;
	deCamera *pLLWorldPrev;
	deCamera *pLLWorldNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new camera object with the given resource manager. */
	deCamera(deCameraManager *manager);
	
protected:
	/**
	 * \brief Clean up camera.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deCamera() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition(const decDVector &position);
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation(const decQuaternion &orientation);
	
	/** \brief Vertical field  of view in radians for the entire view. */
	inline float GetFov() const{ return pFov; }
	
	/** \brief Set vertical field of view in radians of the entire view. */
	void SetFov(float fov);
	
	/** \brief Aspect ratio of the horizontal field of view to the vertical field of view. */
	inline float GetFovRatio() const{ return pFovRatio; }
	
	/** \brief Set aspect ratio of the horizonral field of view to the vertical field of view. */
	void SetFovRatio(float ratio);
	
	/** \brief Distance to the image plane. */
	inline float GetImageDistance() const{ return pImageDistance; }
	
	/** \brief Set distance to the image plane. */
	void SetImageDistance(float distance);
	
	/** \brief Viewing distance up to which world geometry is rendered. */
	inline float GetViewDistance() const{ return pViewDistance; }
	
	/** \brief Set view distance up to which world geometry is rendered. */
	void SetViewDistance(float distance);
	
	
	
	/** \brief Enable high definition range rendering (HDRR) if supported. */
	inline bool GetEnableHDRR() const{ return pEnableHDRR; }
	
	/** \brief Set to enable high definition range rendering (HDRR) if supported. */
	void SetEnableHDRR(bool enable);
	
	/** \brief Exposure. */
	inline float GetExposure() const{ return pExposure; }
	
	/** \brief Set exposure. */
	void SetExposure(float exposure);
	
	/** \brief Lowest intensity the eye can adapt to. */
	inline float GetLowestIntensity() const{ return pLowestIntensity; }
	
	/** \brief Set lowest intensity the eye can adapt to. */
	void SetLowestIntensity(float lowestIntensity);
	
	/** \brief Highest intensity the eye can adapt to. */
	inline float GetHighestIntensity() const{ return pHighestIntensity; }
	
	/** \brief Set highest intensity the eye can adapt to. */
	void SetHighestIntensity(float highestIntensity);
	
	/** \brief Adaption time of the eye in seconds. */
	inline float GetAdaptionTime() const{ return pAdaptionTime; }
	
	/** \brief Set adaption time of the eye in seconds. */
	void SetAdaptionTime(float adaptionTime);
	
	
	
	/** \brief Enable global illumination (GI) if supported. */
	inline bool GetEnableGI() const{ return pEnableGI; }
	
	/** \brief Set to enable global illumination (GI) if supported. */
	void SetEnableGI(bool enable);
	
	
	
	/** \brief Layer mask. Call NotifyLayerMaskChanged afterwards. */
	decLayerMask &GetLayerMask(){return pLayerMask;}
	const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Notifies the peers that the layer mask changed. */
	void NotifyLayerMaskChanged();
	
	/** \brief Request graphic module to reset adapted intensity to optimal value. */
	void ResetAdaptedIntensity();
	
	
	
	/**
	 * \brief White intensity multiplier.
	 * \version 1.21
	 */
	inline float GetWhiteIntensity() const{ return pWhiteIntensity; }
	
	/**
	 * \brief Set white intensity multiplier.
	 * \version 1.21
	 */
	void SetWhiteIntensity(float intensity);
	
	/**
	 * \brief Bloom intensity multiplier.
	 * \version 1.21
	 */
	inline float GetBloomIntensity() const{ return pBloomIntensity; }
	
	/**
	 * \brief Set bloom intensity multiplier.
	 * \version 1.21
	 */
	void SetBloomIntensity(float intensity);
	
	/**
	 * \brief Bloom strength as multiplier of intensity beyond bloom intensity.
	 * \version 1.21
	 */
	inline float GetBloomStrength() const{ return pBloomStrength; }
	
	/**
	 * \brief Set bloom strength as multiplier of intensity beyond bloom intensity.
	 * \version 1.21
	 */
	void SetBloomStrength(float strength);
	
	/**
	 * \brief Bloom blend as multiplier of intensity beyond bloom intensity.
	 * \version 1.21
	 */
	inline float GetBloomBlend() const{ return pBloomBlend; }
	
	/**
	 * \brief Set bloom blend as multiplier of intensity beyond bloom intensity.
	 * \version 1.21
	 */
	void SetBloomBlend(float blend);
	
	/**
	 * \brief Bloom size as percentage of screen width.
	 * \version 1.21
	 */
	inline float GetBloomSize() const{ return pBloomSize; }
	
	/**
	 * \brief Bloom size as percentage of screen width.
	 * \version 1.21
	 */
	void SetBloomSize(float size);
	
	
	
	/**
	 * \brief Custom tone mapping curve or empty curve to disable.
	 * \version 1.21
	 */
	inline const decCurveBezier &GetToneMapCurve() const{ return pToneMapCurve; }
	
	/**
	 * \brief Set custom tone mapping curve or empty curve to disable.
	 * \version 1.21
	 * \note If enabled make sure to match the curve range to the white intensity.
	 */
	void SetToneMapCurve(const decCurveBezier &curve);
	/*@}*/
	
	
	
	/** \name Effects */
	/*@{*/
	/** \brief Count of effects. */
	int GetEffectCount() const;
	
	/** \brief Effect at the given index. */
	deEffect *GetEffectAt(int index) const;
	
	/** \brief Adds an effect. */
	void AddEffect(deEffect *effect);
	
	/** \brief Removes an effect. */
	void RemoveEffect(deEffect *effect);
	
	/** \brief Removes all effects. */
	void RemoveAllEffects();
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system camera peer. */
	inline deBaseGraphicCamera *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system camera peer. */
	void SetPeerGraphic(deBaseGraphicCamera *peer);
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Parent world or NULL. */
	inline deWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld(deWorld *world);
	
	/** \brief Previous camera in the parent world linked list. */
	inline deCamera *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set next camera in the parent world linked list. */
	void SetLLWorldPrev(deCamera *camera);
	
	/** \brief Next camera in the parent world linked list. */
	inline deCamera *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set next camera in the parent world linked list. */
	void SetLLWorldNext(deCamera *camera);
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
