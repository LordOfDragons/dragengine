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

#ifndef _DEOGLRRENDERTHREAD_H_
#define _DEOGLRRENDERTHREAD_H_

#include "deoglRTLeakTracker.h"
#include "../deoglBasics.h"
#include "../configuration/deoglConfiguration.h"
#include "../debug/deoglDebugInformation.h"
#include "../memory/deoglMemoryManager.h"
#include "../pipeline/deoglPipelineManager.h"

#ifdef BACKEND_OPENGL
	#include <deSharedVulkan.h>
	#include <devkDevice.h>
#endif

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/common/utils/decTimeHistory.h>
#include <dragengine/threading/deMutex.h>
#include <dragengine/threading/deBarrier.h>
#include <dragengine/threading/deSemaphore.h>
#include <dragengine/threading/deThread.h>

class deGraphicOpenGl;
class deoglCapabilities;
class deoglConfiguration;
class deoglDeferredRendering;
class deoglDelayedOperations;
class deoglEnvMapSlotManager;
class deoglExtensions;
class deoglGI;
class deoglLightBoundaryMap;
class deoglOcclusionQueryManager;
#include "../canvas/render/deoglRCanvasView.h"
class deoglRRenderWindow;
class deoglShadowMapper;
class deoglTriangleSorter;
class deoglPersistentRenderTaskPool;
class deoglRenderTaskSharedPool;
class deoglRTUniqueKey;
class deoglOcclusionTestPool;
class deoglRCamera;

class deoglRTBufferObject;
class deoglRTContext;
class deoglRTDebug;
class deoglRTDefaultTextures;
class deoglRTFramebuffer;
class deoglRTLogger;
class deoglRTRenderers;
class deoglRTShader;
class deoglRTTexture;
class deoglRTChoices;
class deoglLoaderThread;

class deRenderWindow;

// deprecated
class deoglOptimizerManager;
class deoglQuickSorter;



/**
 * Render thread.
 */
class deoglRenderThread : public deThread{
private:
	enum eThreadStates{
		etsStopped,
		etsInitialize,
		etsSynchronize,
		etsRendering,
		etsFinishedRendering,
		etsCleaningUp,
		etsFrozen,
		#ifdef WITH_OPENGLES
		etsWindowTerminate,
		etsWindowDown,
		etsWindowInit
		#endif
	};
	
	deGraphicOpenGl &pOgl;
	
	bool pAsyncRendering;
	bool pConfigChanged;
	uint32_t pFrameCounter;
	deoglRCamera *pVRCamera;
	decDMatrix pVRDebugPanelMatrix;
	
	deoglConfiguration pConfiguration;
	deoglRTLeakTracker pLeakTracker;
	deoglMemoryManager pMemoryManager;
	decObjectOrderedSet pRRenderWindowList;
	decObjectOrderedSet pRCaptureCanvasList;
	deoglRCanvasView::Ref pCanvasInputOverlay;
	deoglRCanvasView::Ref pCanvasDebugOverlay;
	deoglRCanvasView::Ref pCanvasOverlay;
	
	deoglRTChoices *pChoices;
	deoglRTBufferObject *pBufferObject;
	deoglRTContext *pContext;
	deoglLoaderThread *pLoaderThread;
	deoglRTDebug *pDebug;
	deoglRTDefaultTextures *pDefaultTextures;
	deoglRTFramebuffer *pFramebuffer;
	deoglRTLogger *pLogger;
	deoglRTRenderers *pRenderers;
	deoglRTShader *pShader;
	deoglRTTexture *pTexture;
	deoglPipelineManager::Ref pPipelineManager;
	
	deoglCapabilities *pCapabilities;
	deoglDeferredRendering *pDeferredRendering;
	deoglDelayedOperations *pDelayedOperations;
	deoglEnvMapSlotManager *pEnvMapSlotManager;
	deoglExtensions *pExtensions;
	deoglLightBoundaryMap *pLightBoundarybox;
	deoglOcclusionQueryManager *pOccQueryMgr;
	deoglGI *pGI;
	deoglShadowMapper *pShadowMapper;
	deoglTriangleSorter *pTriangleSorter;
	deoglPersistentRenderTaskPool *pPersistentRenderTaskPool;
	deoglRenderTaskSharedPool *pRenderTaskSharedPool;
	deoglRTUniqueKey *pUniqueKey;
	deoglOcclusionTestPool *pOcclusionTestPool;
	
#ifdef BACKEND_OPENGL
	deSharedVulkan::Ref pVulkan;
	devkDevice::Ref pVulkanDevice;
#endif
	
	decTimeHistory pTimeHistoryMain; // main thread
	decTimeHistory pTimeHistoryRender; // render thread
	decTimeHistory pTimeHistoryFrame; // shared (main, render)
	decTimer pTimerMain; // main thread
	decTimer pTimerRender; // render thread
	decTimer pTimerFrameUpdate; // render thread
	decTimer pTimerVRFrameUpdate; // render thread
	float pLastFrameTime; // render thread
	float pEstimatedRenderTime; // shared (main, render)
	float pAccumulatedMainTime; // main thread
	float pFrameTimeLimit; // render thread
	int pFPSRate; // main thread
	deMutex pMutexShared;
	
	// debug information
	deoglDebugInformation::Ref pDebugInfoModule;
	
	deoglDebugInformation::Ref pDebugInfoThreadMain;
	deoglDebugInformation::Ref pDebugInfoThreadMainWaitFinish;
	deoglDebugInformation::Ref pDebugInfoThreadMainSynchronize;
	
	deoglDebugInformation::Ref pDebugInfoThreadRender;
	deoglDebugInformation::Ref pDebugInfoThreadRenderSyncGpu;
	deoglDebugInformation::Ref pDebugInfoThreadRenderBegin;
	deoglDebugInformation::Ref pDebugInfoThreadRenderWindows;
	deoglDebugInformation::Ref pDebugInfoThreadRenderWindowsPrepare;
	deoglDebugInformation::Ref pDebugInfoThreadRenderWindowsRender;
	deoglDebugInformation::Ref pDebugInfoThreadRenderCapture;
	deoglDebugInformation::Ref pDebugInfoThreadRenderEnd;
	deoglDebugInformation::Ref pDebugInfoThreadRenderSwap;
	deoglDebugInformation::Ref pDebugInfoVRRender;
	
	deoglDebugInformation::Ref pDebugInfoFrameLimiter;
	deoglDebugInformation::Ref pDebugInfoFLEstimMain;
	deoglDebugInformation::Ref pDebugInfoFLEstimRender;
	deoglDebugInformation::Ref pDebugInfoFLFrameRateMain;
	deoglDebugInformation::Ref pDebugInfoFLFrameRateRender;
	
	decTimer pDebugTimerMainThread1;
	decTimer pDebugTimerMainThread2;
	
	decTimer pDebugTimerRenderThread1;
	decTimer pDebugTimerRenderThread2;
	decTimer pDebugTimerRenderThread3;
	
	bool pMainThreadShowDebugInfoModule;
	
	float pDebugTimeThreadMainWaitFinish;
	
	float pDebugTimeThreadRenderSyncGpu;
	float pDebugTimeThreadRenderBegin;
	float pDebugTimeThreadRenderWindows;
	float pDebugTimeThreadRenderWindowsPrepare;
	float pDebugTimeThreadRenderWindowsRender;
	float pDebugTimeThreadRenderCapture;
	float pDebugTimeThreadRenderSwap;
	float pDebugTimeVRRender;
	int pDebugCountThreadWindows;
	
	// deprecated
	deoglQuickSorter *pQuickSorter;
	deoglOptimizerManager *pOptimizerManager;
	
	// thread control
	deRenderWindow *pInitialRenderWindow; // temporary variable for thread initialization
	eThreadStates pThreadState;
	bool pThreadFailure;
	deBarrier pBarrierSyncIn;
	deBarrier pBarrierSyncOut;
	deSemaphore pSemaphoreSyncVR;
	bool pSignalSemaphoreSyncVR;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render thread. */
	deoglRenderThread(deGraphicOpenGl &ogl);
	
	/** Clean up render thread. */
	virtual ~deoglRenderThread();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** OpenGL module. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	
	
	/** Asynchronous rendering. */
	inline bool GetAsyncRendering() const{ return pAsyncRendering; }
	
	
	
	/** Frame counter. */
	inline uint32_t GetFrameCounter() const{ return pFrameCounter; }
	
	/** VR camera or nullptr. */
	inline deoglRCamera *GetVRCamera() const{ return pVRCamera; }
	
	/** Set VR camera or nullptr. */
	void SetVRCamera(deoglRCamera *camera);
	
	/** VR debug panel matrix. */
	inline const decDMatrix &GetVRDebugPanelMatrix() const{ return pVRDebugPanelMatrix; }
	
	/** Set VR debug panel matrix. */
	void SetVRDebugPanelMatrix(const decDMatrix &matrix);
	
	/** Configuration. */
	inline deoglConfiguration &GetConfiguration(){ return pConfiguration; }
	
	/** Render render window list. */
	inline decObjectOrderedSet &GetRRenderWindowList(){ return pRRenderWindowList; }
	
	/** Rendr capture canvas list. */
	inline decObjectOrderedSet &GetRCaptureCanvasList(){ return pRCaptureCanvasList; }
	
	/** Input overlay canvas view or nullptr. */
	inline const deoglRCanvasView::Ref &GetCanvasInputOverlay() const{ return pCanvasInputOverlay; }
	
	/** Set input overlay canvas view or nullptr. */
	void SetCanvasInputOverlay(deoglRCanvasView *canvas);
	
	/** Debug overlay canvas view or nullptr. */
	inline const deoglRCanvasView::Ref &GetCanvasDebugOverlay() const{ return pCanvasDebugOverlay; }
	
	/** Set debug overlay canvas view or nullptr. */
	void SetCanvasDebugOverlay(deoglRCanvasView *canvas);
	
	/** Overlay canvas view or nullptr. */
	inline const deoglRCanvasView::Ref &GetCanvasOverlay() const{ return pCanvasOverlay; }
	
	/** Set overlay canvas view or nullptr. */
	void SetCanvasOverlay(deoglRCanvasView *canvas);
	
	
	
	/** Choices. */
	inline const deoglRTChoices &GetChoices() const{ return *pChoices; }
	inline bool HasChoices() const{ return pChoices != nullptr; }
	
	/** Buffer objects. */
	inline deoglRTBufferObject &GetBufferObject() const{ return *pBufferObject; }
	
	/** Has context. */
	bool HasContext() const;
	
	/** Context. */
	inline deoglRTContext &GetContext() const{ return *pContext; }
	
	/** Loader thread. */
	inline deoglLoaderThread &GetLoaderThread() const{ return *pLoaderThread; }
	
	/** Debug. */
	inline deoglRTDebug &GetDebug() const{ return *pDebug; }
	inline bool HasDebug() const{ return pDebug != nullptr; }
	
	/** Default textures. */
	inline deoglRTDefaultTextures &GetDefaultTextures() const{ return *pDefaultTextures; }
	
	/** Framebuffer related. */
	inline deoglRTFramebuffer &GetFramebuffer() const{ return *pFramebuffer; }
	inline bool HasFramebuffer() const{ return pFramebuffer != nullptr; }
	
	/** Logger. */
	inline deoglRTLogger &GetLogger() const{ return *pLogger; }
	
	/** Renderers. */
	inline deoglRTRenderers &GetRenderers() const{ return *pRenderers; }
	
	/** Shader related. */
	inline deoglRTShader &GetShader() const{ return *pShader; }
	
	/** Texture related. */
	inline deoglRTTexture &GetTexture() const{ return *pTexture; }
	
	/** Pipeline manager. */
	inline deoglPipelineManager &GetPipelineManager() const{ return pPipelineManager; }
	
	
	
	/** Memory  manager. */
	inline deoglMemoryManager &GetMemoryManager(){ return pMemoryManager; }
	
	/** Extensions. */
	inline deoglExtensions &GetExtensions() const{ return *pExtensions; }
	
	/** Capabilities. */
	inline deoglCapabilities &GetCapabilities() const{ return *pCapabilities; }
	
	/** Delayed operations manager. */
	inline deoglDelayedOperations &GetDelayedOperations() const{ return *pDelayedOperations; }
	
	/** Shadow mapper. */
	inline deoglShadowMapper &GetShadowMapper() const{ return *pShadowMapper; }
	
	/** Deferred rendering. */
	inline deoglDeferredRendering &GetDeferredRendering() const{ return *pDeferredRendering; }
	
	/** Environment map slot manager. */
	inline deoglEnvMapSlotManager &GetEnvMapSlotManager() const{ return *pEnvMapSlotManager; }
	
	/** Occlusion query manager. */
	inline deoglOcclusionQueryManager &GetOcclusionQueryManager() const{ return *pOccQueryMgr; }
	
	/** Global illumination. */
	inline deoglGI &GetGI() const{ return *pGI; }
	
	/** Light boundary box having at least the given size. */
	deoglLightBoundaryMap &GetLightBoundaryMap(int size);
	
	/** Triangle sorter. */
	deoglTriangleSorter &GetTriangleSorter() const{ return *pTriangleSorter; }
	
	/** Persistent render task pool. */
	inline deoglPersistentRenderTaskPool &GetPersistentRenderTaskPool() const{ return *pPersistentRenderTaskPool; }
	
	/** Render task shared pool. */
	inline deoglRenderTaskSharedPool &GetRenderTaskSharedPool() const{ return *pRenderTaskSharedPool; }
	
	/** Unique key. */
	inline deoglRTUniqueKey &GetUniqueKey() const{ return *pUniqueKey; }
	
	/** Occlusion test pool. */
	inline deoglOcclusionTestPool &GetOcclusionTestPool() const{ return *pOcclusionTestPool; }
	
	
	
#ifdef BACKEND_OPENGL
	/** Vulkan if present. */
	inline const deSharedVulkan::Ref &GetVulkan() const{ return pVulkan; }
	
	/** Vulkan device if present. */
	inline const devkDevice::Ref &GetVulkanDevice() const{ return pVulkanDevice; }
#endif
	
	
	
	/** Main thread time history. */
	inline decTimeHistory &GetTimeHistoryMain(){ return pTimeHistoryMain; }
	inline const decTimeHistory &GetTimeHistoryMain() const{ return pTimeHistoryMain; }
	
	/** Render thread time history. */
	inline decTimeHistory &GetTimeHistoryRender(){ return pTimeHistoryRender; }
	inline const decTimeHistory &GetTimeHistoryRender() const{ return pTimeHistoryRender; }
	
	
	
	/** Leak tracker. */
	inline deoglRTLeakTracker &GetLeakTracker(){ return pLeakTracker; }
	
	
	
	/** Initialize. */
	void Init(deRenderWindow *renderWindow);
	
	/** Clean up. */
	void CleanUp();
	
	#ifdef WITH_OPENGLES
	/** Application window has been created. */
	void InitAppWindow();
	
	/** Application window has been closed. */
	void TerminateAppWindow();
	#endif
	
	
	
	/** Run render thread. */
	virtual void Run();
	
	/** Finalize asynchronously loaded resources. */
	void FinalizeAsyncResLoading();
	
	/** Recreate failed resources. */
	void RecreatedRes();
	
	/** Synchronize to render next frame. */
	void Synchronize();
	
	/** Wait for render thread to finish rendering. */
	void WaitFinishRendering();
	
	/**
	 * Main thread wait for render thread to finish rendering.
	 * 
	 * Called only by main thread. Wraps WaitFinishRendering with debug time measuring
	 * if enabled and waiting optimization. If rendering is done asynchronously time history
	 * is used to judge if rendering is finished soon enough to wait for this event or to
	 * skip synchronization and running another game frame update.
	 * 
	 * \retval true Waited for rendering to finish. Caller has to do synchronization and
	 *              trigger a new frame rendering. Returned always for synchronous rendering.
	 *              Returned for asynchronous rendering if judging by the time history there
	 *              is not enough time left to do another full game frame update.
	 * \retval false Not waiting for rendering to finish. Caller has to do another game frame
	 *               update before trying again. Never returned for synchronous rendering.
	 *               Returned for asynchronous rendering if judging by the time history there
	 *               is enough time left to do another full game frame update.
	 */
	bool MainThreadWaitFinishRendering();
	
	/**
	 * Freeze render thread.
	 * \details Waits until the render thread finished rendering and no parallel
	 *          task is running. Upon returning from this function data members
	 *          of the render thread can be accessed directly for example for
	 *          configuration or debugging purpose.
	 */
	void Freeze();
	
	/** Unfreeze render thread. */
	void Unfreeze();
	
	/** Create a render window thread safe. */
	void CreateRenderWindow(deoglRRenderWindow *window);
	
	/** FPS Rate. */
	inline int GetFPSRate() const{ return pFPSRate; }
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** Sample debug timer render thread render windows prepare. */
	void SampleDebugTimerRenderThreadRenderWindowsPrepare();
	
	/** Sample debug timer render thread render windows render. */
	void SampleDebugTimerRenderThreadRenderWindowsRender();
	
	/** Sample debug timer VR render. */
	void SampleDebugTimerVRRender();
	
	
	
	/** Developer mode debug information changed. */
	void DevModeDebugInfoChanged();
	
	#ifdef WITH_OPENGLES
	bool DoesDebugMemoryUsage() const;
	void DebugMemoryUsage(const char *prefix);
	void DebugMemoryUsageSmall(const char *prefix);
	#endif
	/*@}*/
	
	
	
	/** \name Deprecated */
	/*@{*/
	/** Quick sorter. */
	inline deoglQuickSorter &GetQuickSorter() const{ return *pQuickSorter; }
	
	/** Optimizer. */
	inline deoglOptimizerManager &GetOptimizerManager() const{ return *pOptimizerManager; }
	/*@}*/
	
private:
	void pCleanUp();
	
	void pInitThreadPhase1();
	void pInitThreadPhase2();
	void pInitThreadPhase3();
	void pInitThreadPhase4();
	void pInitExtensions();
	void pInitCapabilities();
	
	void pRenderSingleFrame();
	void pSyncConfiguration();
	void pSwapBuffers();
	void pBeginFrame();
	void pRenderWindows();
	void pVRStartBeginFrame();
	void pVRWaitBeginFrameFinished();
	void pVRRender();
	void pVRSubmit();
	void pVREndFrame();
	void pCaptureCanvas();
	void pEndFrame();
	void pLimitFrameRate(float elapsed);
	
	void pUpdateConfigFrameLimiter();
	void pCleanUpThread();
	
	void pReportLeaks();
};

#endif
