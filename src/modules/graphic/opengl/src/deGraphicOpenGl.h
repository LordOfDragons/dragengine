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

#ifndef _DEGRAPHICOPENGL_H_
#define _DEGRAPHICOPENGL_H_

#include "canvas/capture/deoglCaptureCanvasList.h"
#include "configuration/deoglConfiguration.h"
#include "commands/deoglCommandExecuter.h"
#include "debug/deoglDebugOverlay.h"
#include "parameters/deoglParameterList.h"
#include "shaders/deoglShaderCompilingInfo.h"
#include "window/deoglRenderWindowList.h"

#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicModule.h>

class deoglCaches;
class deoglRenderThread;
class deoglCamera;
class deoglResources;



/**
 * OpenGl Graphic Module.
 */
class deGraphicOpenGl : public deBaseGraphicModule{
private:
	deoglConfiguration pConfiguration;
	deoglCommandExecuter pCommandExecuter;
	deoglParameterList pParameters;
	
	deoglRenderWindowList pRenderWindowList;
	deoglCaptureCanvasList pCaptureCanvasList;
	
	deoglRenderThread *pRenderThread;
	deoglCaches *pCaches;
	deoglDebugOverlay pDebugOverlay;
	deoglResources *pResources;
	
	deCanvasView::Ref pOverlay;
	deoglShaderCompilingInfo::Ref pShaderCompilingInfo;
	
	deoglCamera *pVRCamera;
	decDMatrix pVRDebugPanelMatrix;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create graphic module. */
	deGraphicOpenGl(deLoadableModule &loadableModule);
	
	/** Clean up graphic module. */
	~deGraphicOpenGl() override;
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/**
	 * Called to init the module and prepare it for rendering. Returns true on success.
	 * To access the os of the engine use the GetOS function.
	 */
	bool Init(deRenderWindow *renderWindow) override;
	
	/**
	 * Called to cleanup the module. All resources have to be freed and running threads
	 * stopped or killed if needed.
	 */
	void CleanUp() override;
	
	/** Input system overlay canvas view changed. */
	void InputOverlayCanvasChanged() override;
	
	#ifdef WITH_OPENGLES
	/** Application window has been created. */
	void InitAppWindow() override;
	
	/** Application window has been closed. */
	void TerminateAppWindow() override;
	#endif
	
	
	
	/**
	 * Render windows.
	 * 
	 * Tells the graphic module changes for this frame update are finished
	 * and the existing render windows can be rendered if required. Depending
	 * on the graphic module implementation this can be a synchronous rendering
	 * in the main thread or an asynchronous rendering in a render thread. Most
	 * modules will use a render thread though. This is a synchronization point
	 * and execution will return once the graphic module has finished all
	 * necessary preparation and rendering tasks if required. After the call
	 * returns the operating system window resources are guaranteed to be valid
	 * either with the current frame changes or the image rendered since the
	 * last call to RenderWindows(). It is at the distretion of the graphic
	 * module to decide if it shows the last frame or the current frame for
	 * best performance. For this reason run RenderWindows in regular intervals.
	 */
	void RenderWindows() override;
	
	/**
	 * Frame-per-second rate averaged over the last couple of frames.
	 * \version 1.6
	 * 
	 * Returns 0 if module is not using a separate thread.
	 */
	int GetFPSRate() override;
	
	/** Set position and orientation of VR debug panel if graphic module shows one. */
	void SetVRDebugPanelPosition(const decDVector &position, const decQuaternion &orientation) override;
	/*@}*/
	
	
	
	/** Create a peer for the given billboard. */
	deBaseGraphicBillboard *CreateBillboard(deBillboard *billboard) override;
	
	/** Create a peer for the given canvas. */
	deBaseGraphicCanvas *CreateCanvas(deCanvas *canvas) override;
	
	/** Create peer for capture canvas. */
	deBaseGraphicCaptureCanvas *CreateCaptureCanvas(deCaptureCanvas *captureCanvas) override;
	
	/** Create a peer for the given camera. */
	deBaseGraphicCamera *CreateCamera(deCamera *camera) override;
	
	/** Create a peer for the given component. */
	deBaseGraphicComponent *CreateComponent(deComponent *comp) override;
	
	/** Create a peer for the given debug drawer. */
	deBaseGraphicDebugDrawer *CreateDebugDrawer(deDebugDrawer *debugDrawer) override;
	
	/** Create a peer for the given decal. */
	deBaseGraphicDecal *CreateDecal(deDecal *decal) override;
	
	/** Create a peer for the given dynamic skin. */
	deBaseGraphicDynamicSkin *CreateDynamicSkin(deDynamicSkin *dynamicSkin) override;
	
	/** Create a peer for the given effect. */
	deBaseGraphicEffect *CreateEffect(deEffect *effect) override;
	
	/** Create a peer for the given environment map probe. */
	deBaseGraphicEnvMapProbe *CreateEnvMapProbe(deEnvMapProbe *envMapProbe) override;
	
	/** Create a peer for the given font. */
	deBaseGraphicFont *CreateFont(deFont *font) override;
	
	/** Create a peer for the given height terrain. */
	deBaseGraphicHeightTerrain *CreateHeightTerrain(deHeightTerrain *heightTerrain) override;
	
	/** Create a peer for the given image. */
	deBaseGraphicImage *CreateImage(deImage *image) override;
	
	/** Create a peer for the given light. */
	deBaseGraphicLight *CreateLight(deLight *light) override;
	
	/** Create a peer for the given lumimeter. */
	deBaseGraphicLumimeter *CreateLumimeter(deLumimeter *lumimeter) override;
	
	/** Ccreat a peer for the given model. */
	deBaseGraphicModel *CreateModel(deModel *model) override;
	
	/** Create a peer for the given occlusion mesh. */
	deBaseGraphicOcclusionMesh *CreateOcclusionMesh(deOcclusionMesh *occmesh) override;
	
	/** Create a peer for the given particle emitter. */
	deBaseGraphicParticleEmitter *CreateParticleEmitter(deParticleEmitter *emitter) override;
	
	/** Create a peer for the given particle emitter instance. */
	deBaseGraphicParticleEmitterInstance *CreateParticleEmitterInstance(deParticleEmitterInstance *instance) override;
	
	/** Create a peer for the given prop field. */
	deBaseGraphicPropField *CreatePropField(dePropField *propField) override;
	
	/** Create a peer for the given render window. */
	deBaseGraphicRenderWindow *CreateRenderWindow(deRenderWindow *renderWindow) override;
	
	/** Create a peer for the given skin. */
	deBaseGraphicSkin *CreateSkin(deSkin *skin) override;
	
	/** Create peer sky. */
	deBaseGraphicSky *CreateSky(deSky *sky) override;
	
	/** Create peer sky instance. */
	deBaseGraphicSkyInstance *CreateSkyInstance(deSkyInstance *instance) override;
	
	/** Create a peer for the given smoke emitter. */
	deBaseGraphicSmokeEmitter *CreateSmokeEmitter(deSmokeEmitter *smokeEmitter) override;
	
	/** Create a peer for the given video. */
	deBaseGraphicVideo *CreateVideo(deVideo *video) override;
	
	/** Create a peer for the given video player. */
	deBaseGraphicVideoPlayer *CreateVideoPlayer(deVideoPlayer *videoPlayer) override;
	
	/** Create a peer for the given world. */
	deBaseGraphicWorld *CreateWorld(deWorld *world) override;
	
	/** Get graphic api connection parameters. */
	void GetGraphicApiConnection(sGraphicApiConnection &connection) override;
	/*@}*/
	
	
	
	/** \name Parameters */
	/*@{*/
	/** Number of parameters. */
	int GetParameterCount() const override;
	
	/**
	 * Get information about parameter.
	 * \param[in] index Index of the parameter
	 * \param[in] parameter Object to fill with information about the parameter
	 */
	void GetParameterInfo(int index, deModuleParameter &parameter) const override;
	
	/** Index of named parameter or -1 if not found. */
	int IndexOfParameterNamed(const char *name) const override;
	
	/** Value of named parameter. */
	decString GetParameterValue(const char *name) const override;
	
	/** Set value of named parameter. */
	void SetParameterValue(const char *name, const char *value) override;
	/*@}*/
	
	
	
	/** \name Parallel Processing */
	/*@{*/
	/**
	 * Parallel processing is pausing and threads are still running.
	 * 
	 * If module has special processing which potentially blocks threads from finishing
	 * running it is given the chances to finish these processings now. This method is
	 * called for all modules repeatedly until all threads successfully finished.
	 * 
	 * The default implementation does nothing.
	 */
	void PauseParallelProcessingUpdate() override;
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/**
	 * Sends a command to the module and retrieves an answer from it.
	 * At least the 'help' command has to be understood answering a
	 * list of possible commands. The command is provided in the 'command'
	 * parameter and the answer has to be written into 'answer'. The default
	 * implementation simply answers only to help with itself.
	 */
	void SendCommand(const decUnicodeArgumentList &command, decUnicodeString &answer) override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return *pRenderThread; }
	
	/** Render thread is not NULL. */
	bool HasRenderThread() const;
	
	/** Capture canvas list. */
	inline deoglCaptureCanvasList &GetCaptureCanvasList(){ return pCaptureCanvasList; }
	
	/** Caches. */
	inline deoglCaches &GetCaches() const{ return *pCaches; }
	
	/** Debug overlay manager. */
	inline deoglDebugOverlay &GetDebugOverlay(){ return pDebugOverlay; }
	
	/** Resources. */
	inline deoglResources &GetResources() const{ return *pResources; }
	
	/** Overlay canvas view. */
	inline const deCanvasView::Ref &GetOverlay() const{ return pOverlay; }
	
	/** Shader compiling information. */
	inline const deoglShaderCompilingInfo::Ref &GetShaderCompilingInfo() const{ return pShaderCompilingInfo; }
	
	/** Configuration. */
	inline deoglConfiguration &GetConfiguration(){ return pConfiguration; }
	
	/** List of render windows. */
	inline deoglRenderWindowList &GetRenderWindowList(){ return pRenderWindowList; }
	inline const deoglRenderWindowList &GetRenderWindowList() const{ return pRenderWindowList; }
	
	/** VR camera or null. */
	inline deoglCamera *GetVRCamera() const{ return pVRCamera; }
	
	/** Set VR camera or null. */
	void SetVRCamera(deoglCamera *camera);
	
	/** VR debug panel matrix. */
	inline const decDMatrix &GetVRDebugPanelMatrix() const{ return pVRDebugPanelMatrix; }
	/*@}*/
	
private:
	void pLoadConfig();
	void pCreateParameters();
};

#endif
