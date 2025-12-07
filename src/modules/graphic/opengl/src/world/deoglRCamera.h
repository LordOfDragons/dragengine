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

#ifndef _DEOGLRCAMERA_H_
#define _DEOGLRCAMERA_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/curve/decCurveBezier.h>

class deoglREffect;
class deoglRenderPlan;
class deoglRenderThread;
class deoglRWorld;
class deoglTexture;
class deoglVR;


/**
 * Render camera.
 */
class deoglRCamera : public deObject{
private:
	deoglRenderThread &pRenderThread;
	deoglRWorld *pParentWorld;
	
	decDVector pPosition;
	decDMatrix pInverseCameraMatrix;
	decDMatrix pCameraMatrix;
	
	deoglTexture *pTextureToneMapParams;
	float pElapsedToneMapAdaption;
	bool pForceToneMapAdaption;
	
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
	int pToneMapCurveResolution;
	deoglTexture *pTextureToneMapCurve;
	bool pDirtyToneMapCurve;
	
	deoglRenderPlan *pPlan;
	
	decObjectList pEffects;
	
	bool pInitTexture;
	
	float pLastAverageLuminance;
	bool pDirtyLastAverageLuminance;
	
	deoglVR *pVR;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglRCamera> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render camera. */
	deoglRCamera(deoglRenderThread &renderThread);
	
	/** Clean up render camera. */
	virtual ~deoglRCamera();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Parent world or \em NULL if not set. */
	inline deoglRWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** Set parent world or \em NULL if not set. */
	void SetParentWorld(deoglRWorld *parentWorld);
	
	
	
	/** Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** Set position. */
	void SetPosition(const decDVector &position);
	
	/** Camera matrix. */
	inline const decDMatrix &GetCameraMatrix() const{ return pCameraMatrix; }
	
	/** Inverse camera matrix. */
	inline const decDMatrix &GetInverseCameraMatrix() const{ return pInverseCameraMatrix; }
	
	/** Set camera matrix and calculate calvulate inverse camera matrix. */
	void SetCameraMatrices(const decDMatrix &matrix);
	
	
	
	/** Render plan. */
	inline deoglRenderPlan &GetPlan() const{ return *pPlan; }
	
	
	
	/** Tone mapping parameters texture. */
	inline deoglTexture *GetToneMapParamsTexture() const{ return pTextureToneMapParams; }
	
	/** Set tone mapping parameters texture. */
	void SetToneMapParamsTexture(deoglTexture *texture);
	
	/** Elapsed time since the last adaption of the tone mapping parameters. */
	inline float GetElapsedToneMapAdaption() const{ return pElapsedToneMapAdaption; }
	
	/** Set elapsed time since the last adaption of the tone mapping parameters. */
	void SetElapsedToneMapAdaption(float elapsed);
	
	/** Force full tone mapping adaption for the next rendering. */
	inline bool GetForceToneMapAdaption() const{ return pForceToneMapAdaption; }
	
	/** Set if a full tone mapping adaption is forced for the next rendering. */
	void SetForceToneMapAdaption(bool forceAdaption);
	
	/** Reset elapsed tone mapping adaption time. */
	void ResetElapsedToneMapAdaption();
	
	/** Tone mapping curve texture or nullptr. */
	inline deoglTexture *GetTextureToneMapCurve() const{ return pTextureToneMapCurve; }
	
	/** Use custom tone map curve. */
	bool UseCustomToneMapCurve() const;
	
	
	
	/** Enable HDRR. */
	inline bool GetEnableHDRR() const{ return pEnableHDRR; }
	
	/** Set enable HDRR. */
	void SetEnableHDRR(bool enable);
	
	/** Exposure. */
	inline float GetExposure() const{ return pExposure; }
	
	/** Set exposure. */
	void SetExposure(float exposure);
	
	/** Lowest intensity the eye can adapt to. */
	inline float GetLowestIntensity() const{ return pLowestIntensity; }
	
	/** Set lowest intensity the eye can adapt to. */
	void SetLowestIntensity(float lowestIntensity);
	
	/** Highest intensity the eye can adapt to. */
	inline float GetHighestIntensity() const{ return pHighestIntensity; }
	
	/** Set highest intensity the eye can adapt to. */
	void SetHighestIntensity(float highestIntensity);
	
	/** Adaption time of the eye in seconds. */
	inline float GetAdaptionTime() const{ return pAdaptionTime; }
	
	/** Set adaption time of the eye in seconds. */
	void SetAdaptionTime(float adaptionTime);
	
	
	
	/** Enable GI. */
	inline bool GetEnableGI() const{ return pEnableGI; }
	
	/** Set enable GI. */
	void SetEnableGI(bool enable);
	
	
	
	/** VR or nullptr. */
	inline deoglVR *GetVR() const{ return pVR; }
	
	/** Enable/Disable VR. */
	void EnableVR(bool enable);
	
	
	
	/** White intensity multiplier. */
	inline float GetWhiteIntensity() const{ return pWhiteIntensity; }
	void SetWhiteIntensity(float intensity);
	
	/** Bloom intensity multiplier. */
	inline float GetBloomIntensity() const{ return pBloomIntensity; }
	void SetBloomIntensity(float intensity);
	
	/** Bloom strength.*/
	inline float GetBloomStrength() const{ return pBloomStrength; }
	void SetBloomStrength(float strength);
	
	/** Bloom blend.*/
	inline float GetBloomBlend() const{ return pBloomBlend; }
	void SetBloomBlend(float blend);
	
	/** Bloom size as percentage of screen width. */
	inline float GetBloomSize() const{ return pBloomSize; }
	void SetBloomSize(float size);
	
	/** Custom tone mapping curve or empty curve to disable. */
	inline const decCurveBezier &GetToneMapCurve() const{ return pToneMapCurve; }
	void SetToneMapCurve(const decCurveBezier &curve);
	
	
	
	/**
	 * Last average scene luminance.
	 * 
	 * If dirty reads back the adaption parameters from the GPU and stores the last used
	 * average scene luminance. This has a slight performance impact so this method should
	 * be called only at the beginning of rendering where the GPU data has been written
	 * back in time the longest and thus stalling is not to be expected.
	 * 
	 * Time measuring shows ~0.1-0.2ms for this call to return.
	 * 
	 * If no adaption parameters are present the minimum camera intensity times the
	 * scene key constant is used.
	 * 
	 * Once the average luminance is stored the dirty flag is cleared. Thus retrieving the
	 * data from the GPU happens only the first time after the camera updated the adaption
	 * parameters on the GPU.
	 */
	float GetLastAverageLuminance();
	
	
	
	/** Number of effects. */
	int GetEffectCount() const;
	
	/** Effect at index. */
	deoglREffect &GetEffectAt(int index) const;
	
	/** Add effect. */
	void AddEffect(deoglREffect *effect);
	
	/** Remove all effects. */
	void RemoveAllEffects();
	
	
	
	/** Update. */
	void Update(float elapsed);
	
	/** Prepare for rendering. */
	void PrepareForRender();
	/*@}*/
	
private:
	void pCleanUp();
	void pPrepareToneMapCurveTexture();
};

#endif
