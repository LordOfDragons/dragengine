/* 
 * Drag[en]gine Null Graphic Module
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
	deGraphicNull( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deGraphicNull();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Init module. */
	virtual bool Init( deRenderWindow *renderWindow );
	
	/** \brief Clean up module. */
	virtual void CleanUp();
	
	/** \brief Set render window. */
	virtual void SetRenderWindow( deRenderWindow *renderWindow );
	
	#ifdef ANDROID
	/** \brief Application window has been created. */
	virtual void InitAppWindow();
	
	/** \brief Application window has been closed. */
	virtual void TerminateAppWindow();
	#endif
	/*@}*/
	
	/** \brief Render windows. */
	virtual void RenderWindows();
	
	virtual void InputOverlayCanvasChanged();
	
	
	virtual deBaseGraphicCanvas *CreateCanvas( deCanvas* );
	virtual deBaseGraphicCaptureCanvas *CreateCaptureCanvas( deCaptureCanvas* );
	virtual deBaseGraphicDynamicSkin *CreateDynamicSkin( deDynamicSkin* );
	virtual deBaseGraphicEnvMapProbe *CreateEnvMapProbe( deEnvMapProbe* );
	virtual deBaseGraphicOcclusionMesh *CreateOcclusionMesh( deOcclusionMesh* );
	virtual deBaseGraphicModel *CreateModel( deModel* );
	virtual deBaseGraphicSkin *CreateSkin( deSkin* );
	virtual deBaseGraphicImage *CreateImage( deImage* );
	virtual deBaseGraphicComponent *CreateComponent( deComponent* );
	virtual deBaseGraphicLight *CreateLight( deLight* );
	virtual deBaseGraphicFont *CreateFont( deFont* );
	virtual deBaseGraphicSky *CreateSky( deSky* );
	virtual deBaseGraphicSkyInstance *CreateSkyInstance( deSkyInstance* );
	virtual deBaseGraphicWorld *CreateWorld( deWorld* );
	virtual deBaseGraphicEffect *CreateEffect( deEffect* );
	virtual deBaseGraphicDebugDrawer *CreateDebugDrawer( deDebugDrawer* );
	virtual deBaseGraphicRenderWindow *CreateRenderWindow( deRenderWindow* );
	virtual deBaseGraphicBillboard *CreateBillboard( deBillboard* );
	virtual deBaseGraphicDecal *CreateDecal( deDecal* );
	virtual deBaseGraphicCamera *CreateCamera( deCamera* );
	virtual deBaseGraphicLumimeter *CreateLumimeter( deLumimeter* );
	virtual deBaseGraphicHeightTerrain *CreateHeightTerrain( deHeightTerrain* );
	virtual deBaseGraphicPropField *CreatePropField( dePropField* );
	virtual deBaseGraphicParticleEmitter *CreateParticleEmitter( deParticleEmitter* );
	virtual deBaseGraphicParticleEmitterInstance *CreateParticleEmitterInstance( deParticleEmitterInstance* );
	virtual deBaseGraphicSmokeEmitter *CreateSmokeEmitter( deSmokeEmitter* );
	virtual deBaseGraphicVideo *CreateVideo( deVideo* );
	virtual deBaseGraphicVideoPlayer *CreateVideoPlayer( deVideoPlayer* );
	/*@}*/
};

#endif
