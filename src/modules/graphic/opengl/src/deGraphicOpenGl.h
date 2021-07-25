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

#ifndef _DEGRAPHICOPENGL_H_
#define _DEGRAPHICOPENGL_H_

#include "window/deoglRenderWindowList.h"
#include "canvas/capture/deoglCaptureCanvasList.h"
#include "configuration/deoglConfiguration.h"
#include "commands/deoglCommandExecuter.h"
#include "parameters/deoglParameterList.h"
#include "debug/deoglDebugOverlay.h"

#include <dragengine/systems/modules/graphic/deBaseGraphicModule.h>

class deoglCaches;
class deoglRenderThread;



/**
 * \brief OpenGl Graphic Module.
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
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create graphic module. */
	deGraphicOpenGl( deLoadableModule &loadableModule );
	
	/** \brief Clean up graphic module. */
	virtual ~deGraphicOpenGl();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/**
	 * Called to init the module and prepare it for rendering. Returns true on success.
	 * To access the os of the engine use the GetOS function.
	 */
	virtual bool Init( deRenderWindow *renderWindow );
	
	/**
	 * Called to cleanup the module. All resources have to be freed and running threads
	 * stopped or killed if needed.
	 */
	virtual void CleanUp();
	
	/** \brief Input system overlay canvas view changed. */
	virtual void InputOverlayCanvasChanged();
	
	#ifdef ANDROID
	/** \brief Application window has been created. */
	virtual void InitAppWindow();
	
	/** \brief Application window has been closed. */
	virtual void TerminateAppWindow();
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
	virtual void RenderWindows();
	
	/**
	 * \brief Frame-per-second rate averaged over the last couple of frames.
	 * \version 1.6
	 * 
	 * Returns 0 if module is not using a separate thread.
	 */
	virtual int GetFPSRate();
	/*@}*/
	
	
	
	/** Create a peer for the given billboard. */
	virtual deBaseGraphicBillboard *CreateBillboard( deBillboard *billboard );
	
	/** \brief Create a peer for the given canvas. */
	virtual deBaseGraphicCanvas *CreateCanvas( deCanvas *canvas );
	
	/** \brief Create peer for capture canvas. */
	virtual deBaseGraphicCaptureCanvas *CreateCaptureCanvas( deCaptureCanvas *captureCanvas );
	
	/** Create a peer for the given camera. */
	virtual deBaseGraphicCamera *CreateCamera( deCamera *camera );
	
	/** Create a peer for the given component. */
	virtual deBaseGraphicComponent *CreateComponent( deComponent *comp );
	
	/** Create a peer for the given debug drawer. */
	virtual deBaseGraphicDebugDrawer *CreateDebugDrawer( deDebugDrawer *debugDrawer );
	
	/** Create a peer for the given decal. */
	virtual deBaseGraphicDecal *CreateDecal( deDecal *decal );
	
	/** Create a peer for the given dynamic skin. */
	virtual deBaseGraphicDynamicSkin *CreateDynamicSkin( deDynamicSkin *dynamicSkin );
	
	/** Create a peer for the given effect. */
	virtual deBaseGraphicEffect *CreateEffect( deEffect *effect );
	
	/** Create a peer for the given environment map probe. */
	virtual deBaseGraphicEnvMapProbe *CreateEnvMapProbe( deEnvMapProbe *envMapProbe );
	
	/** Create a peer for the given font. */
	virtual deBaseGraphicFont *CreateFont( deFont *font );
	
	/** Create a peer for the given height terrain. */
	virtual deBaseGraphicHeightTerrain *CreateHeightTerrain( deHeightTerrain *heightTerrain );
	
	/** Create a peer for the given image. */
	virtual deBaseGraphicImage *CreateImage( deImage *image );
	
	/** Create a peer for the given light. */
	virtual deBaseGraphicLight *CreateLight( deLight *light );
	
	/** Create a peer for the given lumimeter. */
	virtual deBaseGraphicLumimeter *CreateLumimeter( deLumimeter *lumimeter );
	
	/** Ccreat a peer for the given model. */
	virtual deBaseGraphicModel *CreateModel( deModel *model );
	
	/** Create a peer for the given occlusion mesh. */
	virtual deBaseGraphicOcclusionMesh *CreateOcclusionMesh( deOcclusionMesh *occmesh );
	
	/** Create a peer for the given particle emitter. */
	virtual deBaseGraphicParticleEmitter *CreateParticleEmitter( deParticleEmitter *emitter );
	
	/** Create a peer for the given particle emitter instance. */
	virtual deBaseGraphicParticleEmitterInstance *CreateParticleEmitterInstance( deParticleEmitterInstance *instance );
	
	/** Create a peer for the given prop field. */
	virtual deBaseGraphicPropField *CreatePropField( dePropField *propField );
	
	/** Create a peer for the given render window. */
	virtual deBaseGraphicRenderWindow *CreateRenderWindow( deRenderWindow *renderWindow );
	
	/** Create a peer for the given skin. */
	virtual deBaseGraphicSkin *CreateSkin( deSkin *skin );
	
	/** \brief Create peer sky. */
	virtual deBaseGraphicSky *CreateSky( deSky *sky );
	
	/** \brief Create peer sky instance. */
	virtual deBaseGraphicSkyInstance *CreateSkyInstance( deSkyInstance *instance );
	
	/** Create a peer for the given smoke emitter. */
	virtual deBaseGraphicSmokeEmitter *CreateSmokeEmitter( deSmokeEmitter *smokeEmitter );
	
	/** Create a peer for the given video. */
	virtual deBaseGraphicVideo *CreateVideo( deVideo *video );
	
	/** Create a peer for the given video player. */
	virtual deBaseGraphicVideoPlayer *CreateVideoPlayer( deVideoPlayer *videoPlayer );
	
	/** Create a peer for the given world. */
	virtual deBaseGraphicWorld *CreateWorld( deWorld *world );
	/*@}*/
	
	
	
	/** \name Parameters */
	/*@{*/
	/** \brief Number of parameters. */
	virtual int GetParameterCount() const;
	
	/**
	 * \brief Get information about parameter.
	 * \param[in] index Index of the parameter
	 * \param[in] parameter Object to fill with information about the parameter
	 */
	virtual void GetParameterInfo( int index, deModuleParameter &parameter ) const;
	
	/** \brief Index of named parameter or -1 if not found. */
	virtual int IndexOfParameterNamed( const char *name ) const;
	
	/** \brief Value of named parameter. */
	virtual decString GetParameterValue( const char *name ) const;
	
	/** \brief Set value of named parameter. */
	virtual void SetParameterValue( const char *name, const char *value );
	/*@}*/
	
	
	
	/** \name Parallel Processing */
	/*@{*/
	/**
	 * \brief Parallel processing is pausing and threads are still running.
	 * 
	 * If module has special processing which potentially blocks threads from finishing
	 * running it is given the chances to finish these processings now. This method is
	 * called for all modules repeatedly until all threads successfully finished.
	 * 
	 * The default implementation does nothing.
	 */
	virtual void PauseParallelProcessingUpdate();
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
	virtual void SendCommand( const decUnicodeArgumentList &command, decUnicodeString &answer );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return *pRenderThread; }
	
	/** \brief Render thread is not NULL. */
	bool HasRenderThread() const;
	
	/** \brief Capture canvas list. */
	inline deoglCaptureCanvasList &GetCaptureCanvasList(){ return pCaptureCanvasList; }
	
	/** \brief Caches. */
	inline deoglCaches &GetCaches() const{ return *pCaches; }
	
	/** \brief Debug overlay manager. */
	inline deoglDebugOverlay &GetDebugOverlay(){ return pDebugOverlay; }
	
	/** \brief Configuration. */
	inline deoglConfiguration &GetConfiguration(){ return pConfiguration; }
	
	/** \brief List of render windows. */
	inline deoglRenderWindowList &GetRenderWindowList(){ return pRenderWindowList; }
	inline const deoglRenderWindowList &GetRenderWindowList() const{ return pRenderWindowList; }
	/*@}*/
	
private:
	void pLoadConfig();
	void pCreateParameters();
};

#endif
