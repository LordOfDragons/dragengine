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

#ifndef _DEGRAPHICSYSTEM_H_
#define _DEGRAPHICSYSTEM_H_

#include "../dragengine_configuration.h"

#if defined OS_UNIX && defined HAS_LIB_X11
#	include <X11/Xlib.h>
#	include <X11/Xutil.h>
#endif

#ifdef OS_W32
#	include "../app/include_windows.h"
#endif

#ifdef OS_BEOS
	class BWindow;
#endif

#include "deBaseSystem.h"
#include "../common/math/decMath.h"
#include "../resources/canvas/deCanvasViewReference.h"
#include "../resources/rendering/deRenderWindowReference.h"

class deBaseGraphicModule;
class deBillboard;
class deCamera;
class deCanvas;
class deCaptureCanvas;
class deComponent;
class deDebugDrawer;
class deDecal;
class deDynamicSkin;
class deEffect;
class deEngine;
class deEnvMapProbe;
class deFont;
class deHeightTerrain;
class deImage;
class deLight;
class deLoadableModule;
class deLumimeter;
class deModel;
class deOcclusionMesh;
class deParticleEmitter;
class deParticleEmitterInstance;
class dePropField;
class deSkin;
class deSky;
class deSkyInstance;
class deSmokeEmitter;
class deVideo;
class deVideoPlayer;
class deWorld;


/**
 * \brief Graphic System.
 *
 * The graphic system is a single type system providing the output functions
 * of the engine in the graphic direction. Everything drawn to the screen
 * including the windows required for this task are managed by the graphic
 * system. This system provides the most peers into engine objects as it is
 * a system requiring heavy optimizations to provide good speed also on
 * weaker computers. A graphic system provides one special render target
 * the <em>Primary Render Target</em>. This render target can not be created
 * manually and represents the means to render to the main output location.
 * You can create manually render targets for rendering to non visible
 * places if you wish.
 *
 * \note
 * Currently the render system expects a buffer swapping to be done for the
 * output object. This is true in the case of games but for other applications
 * using the engine in headless mode this is overkill. Furthermore for a pure
 * GUI intro menu without 3d graphics used buffer swapping is overkill too.
 * An possible solution is to allow setting a buffer-switching flag. If the
 * flag is not set always the same buffer is used and drawing takes place
 * immediatly the way operating systems handle drawing. With buffer switching
 * activated each time you finish a frame the the buffers are switched. In the
 * later scenario you have to draw the entire screen each time wheras in the
 * former you only have to render if needed.
 *
 * \note
 * Currently the graphic system manages render targets instead of a separate
 * render target manager. This is a bad situation and will be changed in the
 * future. The render target becomes an own resource type and a manager.
 */
class DE_DLL_EXPORT deGraphicSystem : public deBaseSystem{
private:
	deBaseGraphicModule *pActiveModule;
	deRenderWindowReference pRenderWindow;
	
	deCanvasViewReference pInputOverlayCanvas;
	deCanvasViewReference pDebugOverlayCanvas;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new graphic system  linked to the given engine. */
	deGraphicSystem( deEngine *engine );
	
	/** \brief Clean up graphic system. */
	virtual ~deGraphicSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Activated graphic module. */
	inline deBaseGraphicModule *GetActiveModule() const{ return pActiveModule; }
	
	/** \brief Render window. */
	inline deRenderWindow *GetRenderWindow() const{ return pRenderWindow; }
	
	/** \brief Set render window. */
	void SetRenderWindow( deRenderWindow *renderWindow );
	
	/** \brief Creates and sets a new render window. */
	void CreateAndSetRenderWindow( int width, int height, bool fullScreen,
		const char *title, deImage *icon );
	
	/** \brief Creates and sets a new hosted render window. */
	#ifdef ANDROID
	void CreateAndSetHostedRenderWindow( int width, int height, bool fullScreen,
		const char *title, deImage *icon, void *hostWindow );
	#endif
	#ifdef OS_BEOS
	void CreateAndSetHostedRenderWindow( int width, int height, bool fullScreen,
		const char *title, deImage *icon, BWindow *hostWindow );
	#endif
	#if defined OS_UNIX && defined HAS_LIB_X11
	void CreateAndSetHostedRenderWindow( int width, int height, bool fullScreen,
		const char *title, deImage *icon, Window hostWindow );
	#endif
	#ifdef OS_W32
	void CreateAndSetHostedRenderWindow( int width, int height, bool fullScreen,
		const char *title, deImage *icon, HWND hostWindow );
	#endif
	
	/** \brief Input system overlay canvas or NULL if not used. */
	inline deCanvasView *GetInputOverlayCanvas() const{ return pInputOverlayCanvas; }
	
	/** \brief Set input system overlay canvas or NULL if not used. */
	void SetInputOverlayCanvas( deCanvasView *view );
	
	/**
	 * \brief Debug overlay canvas.
	 * 
	 * This overlay is for use by modules to display debug information. Each canvas added to
	 * this view is considered as one debug data block. The graphic module is at liberty to
	 * arrange the these canvas as it sees fit. For this reason modules should not expect
	 * their debug canvas to stay in a particular order. Best use a deCanvasView for each
	 * individual debug data block. Helper classes can be found in the debug directory. They
	 * provide ready made debug data blocks that can be filled with information easily.
	 */
	inline deCanvasView *GetDebugOverlayCanvas() const{ return pDebugOverlayCanvas; }
	
	#ifdef ANDROID
	/** \brief Application window has been created. */
	void InitAppWindow();
	
	/** \brief Application window has been closed. */
	void TerminateAppWindow();
	#endif
	
	/**
	 * \brief Render windows.
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
	void RenderWindows();
	
	/** \brief Create global resources. */
	void CreateGlobalResources();
	
	
	
	/** \brief Create peer object for the given billboard using the active module and assigns it. */
	void LoadBillboard( deBillboard *billboard );
	
	/** \brief Create peer object for the given canvas using the active module and assigns it. */
	void LoadCanvas( deCanvas *canvas );
	
	/** \brief Create peer object for the given camera using the active module and assigns it. */
	void LoadCamera( deCamera *camera );
	
	/** \brief Create peer for capture canvas using active module and assigns it. */
	void LoadCaptureCanvas( deCaptureCanvas *captureCanvas );
	
	/** \brief Create peer object for the given component using the active module and assigns it. */
	void LoadComponent( deComponent *comp );
	
	/** \brief Create peer object for the given debug drawer using the active module and assigns it. */
	void LoadDebugDrawer( deDebugDrawer *debugDrawer );
	
	/** \brief Create peer object for the given decal using the active module and assigns it. */
	void LoadDecal( deDecal *decal );
	
	/** \brief Create peer for the dynamic skin using the active module and assigns it. */
	void LoadDynamicSkin( deDynamicSkin *dynamicSkin );
	
	/** \brief Create peer object for the given effect using the active module and assigns it. */
	void LoadEffect( deEffect *effect );
	
	/** \brief Create peer object for the given environment mapü probe using the active module and assigns it. */
	void LoadEnvMapProbe( deEnvMapProbe *envMapProbe );
	
	/** \brief Create peer object for the given font using the active module and assigns it. */
	void LoadFont( deFont *font );
	
	/** \brief Create peer object for the given height terrain using the active module and assigns it. */
	void LoadHeightTerrain( deHeightTerrain *heightTerrain );
	
	/** \brief Create peer object for the given image using the active module and assigns it. */
	void LoadImage( deImage *image );
	
	/** \brief Create peer object for the given light using the active module and assigns it. */
	void LoadLight( deLight *light );
	
	/** \brief Create peer object for the given lumimeter using the active module and assigns it. */
	void LoadLumimeter( deLumimeter *lumimeter );
	
	/** \brief Create peer object for the given model using the active module and assigns it. */
	void LoadModel( deModel *model );
	
	/** \brief Create peer for the occlusion mesh using the active module and assigns it. */
	void LoadOcclusionMesh( deOcclusionMesh *occmesh );
	
	/** \brief Create peer for the particle emitter using the active module and assigns it. */
	void LoadParticleEmitter( deParticleEmitter *emitter );
	
	/** \brief Create peer for the particle emitter instance using the active module and assigns it. */
	void LoadParticleEmitterInstance( deParticleEmitterInstance *instance );
	
	/** \brief Create peer object for the given prop field using the active module and assigns it. */
	void LoadPropField( dePropField *propField );
	
	/** \brief Create peer object for the given render window using the active module and assigns it. */
	void LoadRenderWindow( deRenderWindow *renderWindow );
	
	/** \brief Create peer object for the given skin using the active module and assigns it. */
	void LoadSkin( deSkin *skin );
	
	/** \brief Create and assign peer for sky using active module. */
	void LoadSky( deSky *sky );
	
	/** \brief Create and assign peer for sky instance using active module. */
	void LoadSkyInstance( deSkyInstance *sky );
	
	/** \brief Create peer for the smoke emitter using the active module and assigns it. */
	void LoadSmokeEmitter( deSmokeEmitter *smokeEmitter );
	
	/** \brief Create peer for the video using the active module and assigns it. */
	void LoadVideo( deVideo *video );
	
	/** \brief Create peer for the video player using the active module and assigns it. */
	void LoadVideoPlayer( deVideoPlayer *videoPlayer );
	
	/** \brief Create peer object for the given world using the active module and assigns it. */
	void LoadWorld( deWorld *world );
	/*@}*/
	
	
	
	/** \name Overloadables */
	/*@{*/
	/**
	 * \brief Set active loadable module.
	 * 
	 * Do not forget to call the super function.
	 */
	virtual void SetActiveModule( deLoadableModule *module );
	
	/**
	 * \brief Clearcross references and links that could lead to memory leaks.
	 * 
	 * Do not forget to call the super function.
	 */
	virtual void ClearPermanents();
	
	/** \brief Carry out here actions right after the system started up. */
	virtual void PostStart();
	
	/** \brief Carry out here actions right before the system shuts down. */
	virtual void PreStop();
	/*@}*/
};

#endif
