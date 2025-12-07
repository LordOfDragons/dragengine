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

#ifndef _DEGRAPHICNULL_H_
#define _DEGRAPHICNULL_H_

#include <dragengine/systems/modules/graphic/deBaseGraphicModule.h>



/**
 */
class deGraphicNull : public deBaseGraphicModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deGraphicNull(deLoadableModule &loadableModule);
	
	/** \brief Clean up module. */
	virtual ~deGraphicNull();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Init module. */
	virtual bool Init(deRenderWindow *renderWindow);
	
	/** \brief Clean up module. */
	virtual void CleanUp();
	
	/** \brief Set render window. */
	virtual void SetRenderWindow(deRenderWindow *renderWindow);
	
	#ifdef OS_ANDROID
	/** \brief Application window has been created. */
	virtual void InitAppWindow();
	
	/** \brief Application window has been closed. */
	virtual void TerminateAppWindow();
	#endif
	/*@}*/
	
	/** \brief Render windows. */
	virtual void RenderWindows();
	
	virtual void InputOverlayCanvasChanged();
	
	
	virtual deBaseGraphicCanvas *CreateCanvas(deCanvas*);
	virtual deBaseGraphicCaptureCanvas *CreateCaptureCanvas(deCaptureCanvas*);
	virtual deBaseGraphicDynamicSkin *CreateDynamicSkin(deDynamicSkin*);
	virtual deBaseGraphicEnvMapProbe *CreateEnvMapProbe(deEnvMapProbe*);
	virtual deBaseGraphicOcclusionMesh *CreateOcclusionMesh(deOcclusionMesh*);
	virtual deBaseGraphicModel *CreateModel(deModel*);
	virtual deBaseGraphicSkin *CreateSkin(deSkin*);
	virtual deBaseGraphicImage *CreateImage(deImage*);
	virtual deBaseGraphicComponent *CreateComponent(deComponent*);
	virtual deBaseGraphicLight *CreateLight(deLight*);
	virtual deBaseGraphicFont *CreateFont(deFont*);
	virtual deBaseGraphicSky *CreateSky(deSky*);
	virtual deBaseGraphicSkyInstance *CreateSkyInstance(deSkyInstance*);
	virtual deBaseGraphicWorld *CreateWorld(deWorld*);
	virtual deBaseGraphicEffect *CreateEffect(deEffect*);
	virtual deBaseGraphicDebugDrawer *CreateDebugDrawer(deDebugDrawer*);
	virtual deBaseGraphicRenderWindow *CreateRenderWindow(deRenderWindow*);
	virtual deBaseGraphicBillboard *CreateBillboard(deBillboard*);
	virtual deBaseGraphicDecal *CreateDecal(deDecal*);
	virtual deBaseGraphicCamera *CreateCamera(deCamera*);
	virtual deBaseGraphicLumimeter *CreateLumimeter(deLumimeter*);
	virtual deBaseGraphicHeightTerrain *CreateHeightTerrain(deHeightTerrain*);
	virtual deBaseGraphicPropField *CreatePropField(dePropField*);
	virtual deBaseGraphicParticleEmitter *CreateParticleEmitter(deParticleEmitter*);
	virtual deBaseGraphicParticleEmitterInstance *CreateParticleEmitterInstance(deParticleEmitterInstance*);
	virtual deBaseGraphicSmokeEmitter *CreateSmokeEmitter(deSmokeEmitter*);
	virtual deBaseGraphicVideo *CreateVideo(deVideo*);
	virtual deBaseGraphicVideoPlayer *CreateVideoPlayer(deVideoPlayer*);
	/*@}*/
};

#endif
