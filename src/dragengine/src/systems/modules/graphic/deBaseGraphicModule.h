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

#ifndef _DEBASEGRAPHICMODULE_H_
#define _DEBASEGRAPHICMODULE_H_

#include <stdint.h>

#include "../deBaseModule.h"
#include "../../../dragengine_configuration.h"
#include "../../../common/math/decMath.h"

class deBaseGraphicBillboard;
class deBaseGraphicCamera;
class deBaseGraphicCanvas;
class deBaseGraphicCaptureCanvas;
class deBaseGraphicComponent;
class deBaseGraphicDebugDrawer;
class deBaseGraphicDecal;
class deBaseGraphicDynamicSkin;
class deBaseGraphicEffect;
class deBaseGraphicEnvMapProbe;
class deBaseGraphicFont;
class deBaseGraphicHeightTerrain;
class deBaseGraphicImage;
class deBaseGraphicLight;
class deBaseGraphicLumimeter;
class deBaseGraphicModel;
class deBaseGraphicOcclusionMesh;
class deBaseGraphicParticleEmitter;
class deBaseGraphicParticleEmitterInstance;
class deBaseGraphicPortalSystem;
class deBaseGraphicPropField;
class deBaseGraphicRenderWindow;
class deBaseGraphicSkin;
class deBaseGraphicSky;
class deBaseGraphicSkyInstance;
class deBaseGraphicSmokeEmitter;
class deBaseGraphicVideo;
class deBaseGraphicVideoPlayer;
class deBaseGraphicWorld;

class deBillboard;
class deCamera;
class deCanvas;
class deCaptureCanvas;
class deComponent;
class deDebugDrawer;
class deDecal;
class deDynamicSkin;
class deEffect;
class deEnvMapProbe;
class deFont;
class deHeightTerrain;
class deImage;
class deLight;
class deLumimeter;
class deModel;
class deOcclusionMesh;
class deParticleEmitter;
class deParticleEmitterInstance;
class dePortalSystem;
class dePropField;
class deRenderWindow;
class deSkin;
class deSky;
class deSkyInstance;
class deSmokeEmitter;
class deVideo;
class deVideoPlayer;
class deWorld;


/**
 * \brief Base Graphic Module.
 *
 */
class DE_DLL_EXPORT deBaseGraphicModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new module. */
	deBaseGraphicModule(deLoadableModule &loadableModule);
	
	/** \brief Clean up module. */
	virtual ~deBaseGraphicModule();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/**
	 * \brief Called to init the module and prepare it for rendering.
	 * 
	 * Returns true on success. To access the os of the engine use the GetOS function.
	 */
	virtual bool Init(deRenderWindow *renderWindow) = 0;
	
	/**
	 * \brief Called to cleanup the module.
	 * 
	 * All resources have to be freed and running threads stopped or killed if needed.
	 */
	virtual void CleanUp() = 0;
	
	/** \brief Input system overlay canvas view changed. */
	virtual void InputOverlayCanvasChanged() = 0;
	
	#ifdef OS_ANDROID
	/** \brief Application window has been created. */
	virtual void InitAppWindow() = 0;
	
	/** \brief Application window has been closed. */
	virtual void TerminateAppWindow() = 0;
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
	virtual void RenderWindows() = 0;
	
	/**
	 * \brief Frame-per-second rate averaged over the last couple of frames.
	 * \version 1.6
	 * 
	 * Returns 0 if module is not using a separate thread.
	 */
	virtual int GetFPSRate();
	
	/**
	 * \brief Set position and orientation of VR debug panel if graphic module shows one.
	 * \version 1.17
	 * 
	 * The debug panel is usually shown if the user looks at the back of his right hand
	 * like looking at a wrist watch. The position is the center of the debug panel with
	 * the size determined by the graphic module. The matrix defined by the orientation
	 * lines up with the panel like this:
	 * - matrix right vector along panel right direction
	 * - matrix up vector along panel down direction
	 * - matrix forward vector pointing towards the user
	 */
	virtual void SetVRDebugPanelPosition(const decDVector &position, const decQuaternion &orientation);
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Create peer for billboard. */
	virtual deBaseGraphicBillboard *CreateBillboard(deBillboard *billboard) = 0;
	
	/** \brief Create peer for canvas. */
	virtual deBaseGraphicCanvas *CreateCanvas(deCanvas *canvas) = 0;
	
	/** \brief Create peer for capture canvas. */
	virtual deBaseGraphicCaptureCanvas *CreateCaptureCanvas(deCaptureCanvas *captureCanvas) = 0;
	
	/** \brief Create peer for camera. */
	virtual deBaseGraphicCamera *CreateCamera(deCamera *camera) = 0;
	
	/** \brief Create peer for component. */
	virtual deBaseGraphicComponent *CreateComponent(deComponent *comp) = 0;
	
	/** \brief Create peer for debug drawer. */
	virtual deBaseGraphicDebugDrawer *CreateDebugDrawer(deDebugDrawer *debugDrawer) = 0;
	
	/** \brief Create peer for decal. */
	virtual deBaseGraphicDecal *CreateDecal(deDecal *decal) = 0;
	
	/** \brief Create peer for dynamic skin. */
	virtual deBaseGraphicDynamicSkin *CreateDynamicSkin(deDynamicSkin *dynamicSkin) = 0;
	
	/** \brief Create peer for effect. */
	virtual deBaseGraphicEffect *CreateEffect(deEffect *effect) = 0;
	
	/** \brief Create peer for environment map probe. */
	virtual deBaseGraphicEnvMapProbe *CreateEnvMapProbe(deEnvMapProbe *envMapProbe) = 0;
	
	/** \brief Create peer for font. */
	virtual deBaseGraphicFont *CreateFont(deFont *font) = 0;
	
	/** \brief Create peer for height terrain. */
	virtual deBaseGraphicHeightTerrain *CreateHeightTerrain(deHeightTerrain *heightTerrain) = 0;
	
	/** \brief Create peer for image. */
	virtual deBaseGraphicImage *CreateImage(deImage *image) = 0;
	
	/** \brief Create peer for light. */
	virtual deBaseGraphicLight *CreateLight(deLight *light) = 0;
	
	/** \brief Create peer for lumimeter. */
	virtual deBaseGraphicLumimeter *CreateLumimeter(deLumimeter *lumimeter) = 0;
	
	/** \brief Ccreat a peer for the given model. */
	virtual deBaseGraphicModel *CreateModel(deModel *model) = 0;
	
	/** \brief Create peer for occlusion mesh. */
	virtual deBaseGraphicOcclusionMesh *CreateOcclusionMesh(deOcclusionMesh *occmesh) = 0;
	
	/** \brief Create peer for particle emitter. */
	virtual deBaseGraphicParticleEmitter *CreateParticleEmitter(deParticleEmitter *emitter) = 0;
	
	/** \brief Create peer for particle emitter instance. */
	virtual deBaseGraphicParticleEmitterInstance *CreateParticleEmitterInstance(deParticleEmitterInstance *instance) = 0;
	
	/** \brief Create peer for prop field. */
	virtual deBaseGraphicPropField *CreatePropField(dePropField *propField) = 0;
	
	/** \brief Create peer for render window. */
	virtual deBaseGraphicRenderWindow *CreateRenderWindow(deRenderWindow *renderWindow) = 0;
	
	/** \brief Create peer for skin. */
	virtual deBaseGraphicSkin *CreateSkin(deSkin *skin) = 0;
	
	/** \brief Create peer for sky. */
	virtual deBaseGraphicSky *CreateSky(deSky *sky) = 0;
	
	/** \brief Create peer for sky instance. */
	virtual deBaseGraphicSkyInstance *CreateSkyInstance(deSkyInstance *sky) = 0;
	
	/** \brief Create peer for smoke emitter. */
	virtual deBaseGraphicSmokeEmitter *CreateSmokeEmitter(deSmokeEmitter *smokeEmitter) = 0;
	
	/** \brief Create peer for video. */
	virtual deBaseGraphicVideo *CreateVideo(deVideo *video) = 0;
	
	/** \brief Create peer for video player. */
	virtual deBaseGraphicVideoPlayer *CreateVideoPlayer(deVideoPlayer *videoPlayer) = 0;
	
	/** \brief Create peer for world. */
	virtual deBaseGraphicWorld *CreateWorld(deWorld *world) = 0;
	/*@}*/
	
	
	
	/**
	 * \name Inter Module Connection.
	 * \warning For inter-module use only! Do not use if you dont know what you are doing!
	 */
	/*@{*/
	/**
	 * \brief Graphic API connection parameters.
	 * \warning For inter-module use only! Do not use if you dont know what you are doing!
	 */
	struct sGraphicApiConnection{
		// OpenGL
		struct sGraphicApiConnectionOpenGl{
			#ifdef OS_BEOS
			void *dummy; //<!avoid empty struct
			
			#elif defined OS_ANDROID
			void *display; //<!Android: EGLDisplay
			void *config; //<!Android: EGLConfig
			void *context; //<!Android: EGLContext
			
			#elif defined OS_WEBWASM
			void *dummy; //<!avoid empty struct
			
			#elif defined OS_UNIX_X11
			void *display; //<!X11: Display*
			uint32_t visualid; //<!X11: uint32_t
			void *glxFBConfig; //<!GLXFBConfig
			unsigned long glxDrawable; //<!GLXDrawable
			void *glxContext; //<!GLXContext
			
			#elif defined OS_W32
			void *hDC; // Windows: HDC
			void *hGLRC; // Windows: HGLRC
			
			#else
			void *dummy; //<!avoid empty struct
			#endif
		} opengl;
		
		// Vulkan
		struct sGraphicApiConnectionVulkan{
			void *instance; //<!Vulkan: VkInstance
			void *physicalDevice; //<!Vulkan: VkPhysicalDevice
			void *device; //<!Vulkan: VkDevice
			uint32_t queueFamilyIndex; //<!Vulkan: uint32_t
			uint32_t queueIndex; //<!Vulkan: uint32_t
		} vulkan;
	};
	
	/**
	 * \brief Get graphic api connection parameters.
	 * \warning For inter-module use only! Do not call nor implement if you dont know what you are doing!
	 * 
	 * Default implementation sets all parameters to invalid.
	 */
	virtual void GetGraphicApiConnection(sGraphicApiConnection &connection);
	/*@}*/
};

#endif
