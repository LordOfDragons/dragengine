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
	~deGraphicNull() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Init module. */
	bool Init(deRenderWindow *renderWindow) override;
	
	/** \brief Clean up module. */
	void CleanUp() override;
	
	/** \brief Set render window. */
	virtual void SetRenderWindow(deRenderWindow *renderWindow);
	
	#ifdef OS_ANDROID
	/** \brief Application window has been created. */
	void InitAppWindow() override;
	
	/** \brief Application window has been closed. */
	void TerminateAppWindow() override;
	#endif
	/*@}*/
	
	/** \brief Render windows. */
	void RenderWindows() override;
	
	void InputOverlayCanvasChanged() override;
	
	
	deBaseGraphicCanvas *CreateCanvas(deCanvas*) override;
	deBaseGraphicCaptureCanvas *CreateCaptureCanvas(deCaptureCanvas*) override;
	deBaseGraphicDynamicSkin *CreateDynamicSkin(deDynamicSkin*) override;
	deBaseGraphicEnvMapProbe *CreateEnvMapProbe(deEnvMapProbe*) override;
	deBaseGraphicOcclusionMesh *CreateOcclusionMesh(deOcclusionMesh*) override;
	deBaseGraphicModel *CreateModel(deModel*) override;
	deBaseGraphicSkin *CreateSkin(deSkin*) override;
	deBaseGraphicImage *CreateImage(deImage*) override;
	deBaseGraphicComponent *CreateComponent(deComponent*) override;
	deBaseGraphicLight *CreateLight(deLight*) override;
	deBaseGraphicFont *CreateFont(deFont*) override;
	deBaseGraphicSky *CreateSky(deSky*) override;
	deBaseGraphicSkyInstance *CreateSkyInstance(deSkyInstance*) override;
	deBaseGraphicWorld *CreateWorld(deWorld*) override;
	deBaseGraphicEffect *CreateEffect(deEffect*) override;
	deBaseGraphicDebugDrawer *CreateDebugDrawer(deDebugDrawer*) override;
	deBaseGraphicRenderWindow *CreateRenderWindow(deRenderWindow*) override;
	deBaseGraphicBillboard *CreateBillboard(deBillboard*) override;
	deBaseGraphicDecal *CreateDecal(deDecal*) override;
	deBaseGraphicCamera *CreateCamera(deCamera*) override;
	deBaseGraphicLumimeter *CreateLumimeter(deLumimeter*) override;
	deBaseGraphicHeightTerrain *CreateHeightTerrain(deHeightTerrain*) override;
	deBaseGraphicPropField *CreatePropField(dePropField*) override;
	deBaseGraphicParticleEmitter *CreateParticleEmitter(deParticleEmitter*) override;
	deBaseGraphicParticleEmitterInstance *CreateParticleEmitterInstance(deParticleEmitterInstance*) override;
	deBaseGraphicSmokeEmitter *CreateSmokeEmitter(deSmokeEmitter*) override;
	deBaseGraphicVideo *CreateVideo(deVideo*) override;
	deBaseGraphicVideoPlayer *CreateVideoPlayer(deVideoPlayer*) override;
	/*@}*/
};

#endif
