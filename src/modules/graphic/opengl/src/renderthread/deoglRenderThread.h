/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLRRENDERTHREAD_H_
#define _DEOGLRRENDERTHREAD_H_

#include "deoglRTLeakTracker.h"
#include "../deoglBasics.h"
#include "../configuration/deoglConfiguration.h"
#include "../memory/deoglMemoryManager.h"

#include <deSharedVulkan.h>
#include <devkDevice.h>

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/common/utils/decTimeHistory.h>
#include <dragengine/threading/deMutex.h>
#include <dragengine/threading/deBarrier.h>
#include <dragengine/threading/deThread.h>

class deGraphicOpenGl;
class deoglCapabilities;
class deoglConfiguration;
class deoglDebugInformation;
class deoglDeferredRendering;
class deoglDelayedOperations;
class deoglEnvMapSlotManager;
class deoglExtensions;
class deoglGI;
class deoglLightBoundaryMap;
class deoglOcclusionQueryManager;
class deoglRCanvas;
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

class deRenderWindow;

// deprecated
class deoglEdgeFinder;
class deoglOptimizerManager;
class deoglPreloader;
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
		#ifdef ANDROID
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
	
	deoglConfiguration pConfiguration;
	deoglRTLeakTracker pLeakTracker;
	deoglMemoryManager pMemoryManager;
	decObjectOrderedSet pRRenderWindowList;
	decObjectOrderedSet pRCaptureCanvasList;
	deoglRCanvas *pCanvasInputOverlay;
	deoglRCanvas *pCanvasDebugOverlay;
	
	deoglRTChoices *pChoices;
	deoglRTBufferObject *pBufferObject;
	deoglRTContext *pContext;
	deoglRTDebug *pDebug;
	deoglRTDefaultTextures *pDefaultTextures;
	deoglRTFramebuffer *pFramebuffer;
	deoglRTLogger *pLogger;
	deoglRTRenderers *pRenderers;
	deoglRTShader *pShader;
	deoglRTTexture *pTexture;
	
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
	
	deSharedVulkan::Ref pVulkan;
	devkDevice::Ref pVulkanDevice;
	
	decTimeHistory pTimeHistoryMain;
	decTimeHistory pTimeHistoryRender;
	decTimeHistory pTimeHistoryFrame;
	decTimer pTimerMain;
	decTimer pTimerRender;
	decTimer pTimerFrameUpdate;
	float pEstimatedRenderTime;
	float pAccumulatedMainTime;
	float pFrameTimeLimit;
	int pFPSRate;
	
	// debug information
	deoglDebugInformation *pDebugInfoModule;
	
	deoglDebugInformation *pDebugInfoThreadMain;
	deoglDebugInformation *pDebugInfoThreadMainWaitFinish;
	deoglDebugInformation *pDebugInfoThreadMainSynchronize;
	
	deoglDebugInformation *pDebugInfoThreadRender;
	deoglDebugInformation *pDebugInfoThreadRenderSwap;
	deoglDebugInformation *pDebugInfoThreadRenderBegin;
	deoglDebugInformation *pDebugInfoThreadRenderWindows;
	deoglDebugInformation *pDebugInfoThreadRenderWindowsPrepare;
	deoglDebugInformation *pDebugInfoThreadRenderWindowsRender;
	deoglDebugInformation *pDebugInfoThreadRenderCapture;
	deoglDebugInformation *pDebugInfoThreadRenderEnd;
	
	deoglDebugInformation *pDebugInfoFrameLimiter;
	deoglDebugInformation *pDebugInfoFLEstimMain;
	deoglDebugInformation *pDebugInfoFLEstimRender;
	deoglDebugInformation *pDebugInfoFLFrameRateMain;
	deoglDebugInformation *pDebugInfoFLFrameRateRender;
	
	decTimer pDebugTimerMainThread1;
	decTimer pDebugTimerMainThread2;
	
	decTimer pDebugTimerRenderThread1;
	decTimer pDebugTimerRenderThread2;
	decTimer pDebugTimerRenderThread3;
	
	bool pMainThreadShowDebugInfoModule;
	
	float pDebugTimeThreadMainWaitFinish;
	
	float pDebugTimeThreadRenderSwap;
	float pDebugTimeThreadRenderBegin;
	float pDebugTimeThreadRenderWindows;
	float pDebugTimeThreadRenderWindowsPrepare;
	float pDebugTimeThreadRenderWindowsRender;
	float pDebugTimeThreadRenderCapture;
	int pDebugCountThreadWindows;
	
	// deprecated
	deoglQuickSorter *pQuickSorter;
	deoglPreloader *pPreloader;
	deoglEdgeFinder *pEdgeFinder;
	deoglOptimizerManager *pOptimizerManager;
	
	// thread control
	deRenderWindow *pInitialRenderWindow; // temporary variable for thread initialization
	eThreadStates pThreadState;
	bool pThreadFailure;
	deBarrier pBarrierSyncIn;
	deBarrier pBarrierSyncOut;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render thread. */
	deoglRenderThread( deGraphicOpenGl &ogl );
	
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
	void SetVRCamera ( deoglRCamera *camera );
	
	/** Configuration. */
	inline deoglConfiguration &GetConfiguration(){ return pConfiguration; }
	
	/** Render render window list. */
	inline decObjectOrderedSet &GetRRenderWindowList(){ return pRRenderWindowList; }
	
	/** Rendr capture canvas list. */
	inline decObjectOrderedSet &GetRCaptureCanvasList(){ return pRCaptureCanvasList; }
	
	/** Input overlay canvas view or \em NULL if not present. */
	inline deoglRCanvas *GetCanvasInputOverlay() const{ return pCanvasInputOverlay; }
	
	/** Set input overlay canvas view or \em NULL if not present. */
	void SetCanvasInputOverlay( deoglRCanvas *canvas );
	
	/** Debug overlay canvas view or \em NULL if not present. */
	inline deoglRCanvas *GetCanvasDebugOverlay() const{ return pCanvasDebugOverlay; }
	
	/** Set debug overlay canvas view or \em NULL if not present. */
	void SetCanvasDebugOverlay( deoglRCanvas *canvas );
	
	
	
	/** Choices. */
	inline const deoglRTChoices &GetChoices() const{ return *pChoices; }
	
	/** Buffer objects. */
	inline deoglRTBufferObject &GetBufferObject() const{ return *pBufferObject; }
	
	/** Has context. */
	bool HasContext() const;
	
	/** Context. */
	inline deoglRTContext &GetContext() const{ return *pContext; }
	
	/** Debug. */
	inline deoglRTDebug &GetDebug() const{ return *pDebug; }
	
	/** Default textures. */
	inline deoglRTDefaultTextures &GetDefaultTextures() const{ return *pDefaultTextures; }
	
	/** Framebuffer related. */
	inline deoglRTFramebuffer &GetFramebuffer() const{ return *pFramebuffer; }
	
	/** Logger. */
	inline deoglRTLogger &GetLogger() const{ return *pLogger; }
	
	/** Renderers. */
	inline deoglRTRenderers &GetRenderers() const{ return *pRenderers; }
	
	/** Shader related. */
	inline deoglRTShader &GetShader() const{ return *pShader; }
	
	/** Texture related. */
	inline deoglRTTexture &GetTexture() const{ return *pTexture; }
	
	
	
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
	deoglLightBoundaryMap &GetLightBoundaryMap( int size );
	
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
	
	
	
	/** Vulkan if present. */
	inline deSharedVulkan *GetVulkan() const{ return pVulkan; }
	
	/** Vulkan device if present. */
	inline devkDevice *GetVulkanDevice() const{ return pVulkanDevice; }
	
	
	
	/** Main thread time history. */
	inline decTimeHistory &GetTimeHistoryMain(){ return pTimeHistoryMain; }
	inline const decTimeHistory &GetTimeHistoryMain() const{ return pTimeHistoryMain; }
	
	/** Render thread time history. */
	inline decTimeHistory &GetTimeHistoryRender(){ return pTimeHistoryRender; }
	inline const decTimeHistory &GetTimeHistoryRender() const{ return pTimeHistoryRender; }
	
	
	
	/** Leak tracker. */
	inline deoglRTLeakTracker &GetLeakTracker(){ return pLeakTracker; }
	
	
	
	/** Initialize. */
	void Init( deRenderWindow *renderWindow );
	
	/** Clean up. */
	void CleanUp();
	
	#ifdef ANDROID
	/** Application window has been created. */
	void InitAppWindow();
	
	/** Application window has been closed. */
	void TerminateAppWindow();
	#endif
	
	
	
	/** Run render thread. */
	virtual void Run();
	
	/** Finalize asynchronously loaded resources. */
	void FinalizeAsyncResLoading();
	
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
	void CreateRenderWindow( deoglRRenderWindow *window );
	
	/** FPS Rate. */
	inline int GetFPSRate() const{ return pFPSRate; }
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** Sample debug timer render thread render windows prepare. */
	void SampleDebugTimerRenderThreadRenderWindowsPrepare();
	
	/** Sample debug timer render thread render windows render. */
	void SampleDebugTimerRenderThreadRenderWindowsRender();
	
	
	
	/** Developer mode debug information changed. */
	void DevModeDebugInfoChanged();
	
	#ifdef OS_ANDROID
	bool DoesDebugMemoryUsage() const;
	void DebugMemoryUsage( const char *prefix );
	void DebugMemoryUsageSmall( const char *prefix );
	#endif
	/*@}*/
	
	
	
	/** \name Deprecated */
	/*@{*/
	/** Quick sorter. */
	inline deoglQuickSorter &GetQuickSorter() const{ return *pQuickSorter; }
	
	/** Preloader. */
	inline deoglPreloader &GetPreloader() const{ return *pPreloader; }
	
	/** Edge finder. */
	inline deoglEdgeFinder &GetEdgeFinder() const{ return *pEdgeFinder; }
	
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
	void pVRSubmit();
	void pVRRender();
	void pCaptureCanvas();
	void pEndFrame();
	void pLimitFrameRate( float elapsed );
	
	void pUpdateConfigFrameLimiter();
	void pCleanUpThread();
	
	void pReportLeaks();
};

#endif
