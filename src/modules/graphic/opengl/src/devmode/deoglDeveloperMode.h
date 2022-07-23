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

#ifndef _DEOGLDEVELOPERMODE_H_
#define _DEOGLDEVELOPERMODE_H_

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class decUnicodeArgumentList;
class decUnicodeString;
class deoglTexture;
class deoglFramebuffer;



/**
 * OpenGL developer mode..
 */
class deoglDeveloperMode{
public:
	static const int edimModule = 0x1;
	static const int edimPlanPrepare = 0x2;
	static const int edimCanvas = 0x4;
	static const int edimWorld = 0x8;
	static const int edimSolidGeometry = 0x10;
	static const int edimTransparency = 0x20;
	static const int edimLight = 0x40;
	static const int edimLightSky = 0x80;
	static const int edimLightPoint = 0x100;
	static const int edimLightSpot = 0x200;
	static const int edimFrameLimiter = 0x400;
	static const int edimGI = 0x800;
	
	
	
private:
	deoglRenderThread &pRenderThread;
	bool pEnabled;
	
	bool pShowVisComponent;
	bool pShowVisLight;
	bool pShowComponentLODLevels;
	
	bool pShowHeightTerrain;
	bool pShowPropFieldBox;
	int pShowPropFieldClusters;
	
	bool pShowLightFullBox;
	bool pShowLightBox;
	bool pShowLightVolume;
	bool pShowLightRooms;
	int pShowLightVisualInfo;
	
	bool pShowTranspLevelCount;
	bool pHighlightTransparentObjects;
	
	bool pDebugRenderPlan;
	bool pShowMemoryInfo;
	bool pLogMemoryConsumption;
	
	int pShowOccMapLevel;
	
	bool pShowEnvMaps;
	
	bool pShowEnvMapHull;
	
	bool pDebugEnableLightDepthStencil;
	
	bool pShowSSAO;
	
	bool pShowDebugInfo;
	bool pDebugInfoSync;
	bool pDebugInfoLog;
	int pDebugInfoDetails;
	
	bool pGIShowProbes;
	bool pGIShowProbeOffsets;
	bool pGIShowProbeUpdate;
	int pGIShowCascade;
	
	deoglTexture *pTextureDebugImage;
	deoglFramebuffer *pFBODebugImage;
	bool pDebugImageUsed;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new developer mode object. */
	deoglDeveloperMode( deoglRenderThread &renderThread );
	/** Cleans up the developer mode object. */
	~deoglDeveloperMode();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Determines if the developer mode is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** Determines if the vis mesh has to be shown. */
	/** Determines if the visibility of components is shown. */
	inline bool GetShowVisComponent() const{ return pShowVisComponent; }
	/** Determines if the visibility of lights is shown. */
	inline bool GetShowVisLight() const{ return pShowVisLight; }
	/** Determines if the component lod levels are shown. */
	inline bool GetShowComponentLODLevels() const{ return pShowComponentLODLevels; }
	
	/** Determines if height terrain information are shown. */
	inline bool GetShowHeightTerrain() const{ return pShowHeightTerrain; }
	/** Determines if prop field boundaries are shown as boxes. */
	inline bool GetShowPropFieldBox() const{ return pShowPropFieldBox; }
	/** Retrieves the index of the prop field type to show clusters for. */
	inline int GetShowPropFieldClusters() const{ return pShowPropFieldClusters; }
	
	/** Determines if the light full box is shown. */
	inline bool GetShowLightFullBox() const{ return pShowLightFullBox; }
	/** Determines if the light box is shown. */
	inline bool GetShowLightBox() const{ return pShowLightBox; }
	/** Determines if the light volumes are shown. */
	inline bool GetShowLightVolume() const{ return pShowLightVolume; }
	/** Determines if the light rooms are shown. */
	inline bool GetShowLightRooms() const{ return pShowLightRooms; }
	/** Retrieves the index of the light to show visual information for or -1 to disable. */
	inline int GetShowLightVisualInfo() const{ return pShowLightVisualInfo; }
	
	/** Determines if the number of transparency levels are shown. */
	inline bool GetShowTranspLevelCount() const{ return pShowTranspLevelCount; }
	/** Determines if transparent objects are highlighted. */
	inline bool GetHighlightTransparentObjects() const{ return pHighlightTransparentObjects; }
	
	/** Determines if the render plan debug mode is enabled. */
	inline bool GetDebugRenderPlan() const{ return pDebugRenderPlan; }
	/** Determines if the memory information are displayed. */
	inline bool GetShowMemoryInfo() const{ return pShowMemoryInfo; }
	
	/** Log memory consumption. */
	inline bool GetLogMemoryConsumption() const{ return pLogMemoryConsumption; }
	
	/** Retrieves the occlusion map level to show or -1 to disable. */
	inline int GetShowOccMapLevel() const{ return pShowOccMapLevel; }
	
	/** Environment maps are shown. */
	inline bool GetShowEnvMaps() const{ return pShowEnvMaps; }
	
	/** Determines if environment map hulls are shown. */
	inline bool GetShowEnvMapHull() const{ return pShowEnvMapHull; }
	
	/** Determines if depth and stencil tests using a depth copy is enabled for lighting passes. */
	inline bool GetDebugEnableLightDepthStencil() const{ return pDebugEnableLightDepthStencil; }
	
	/** Determines if the SSAO image is shown. */
	inline bool GetShowSSAO() const{ return pShowSSAO; }
	
	
	
	/** Show debug information which also enables debug timing measurements. */
	inline bool GetShowDebugInfo() const{ return pShowDebugInfo; }
	
	/** Call glFinish before each debug timing measurement for true GPU time measuring. */
	inline bool GetDebugInfoSync() const{ return pDebugInfoSync; }
	
	/** Log debug timing measurement result per frame. */
	inline bool GetDebugInfoLog() const{ return pDebugInfoLog; }
	
	/** Debug information details to show. */
	inline int GetDebugInfoDetails() const{ return pDebugInfoDetails; }
	
	
	
	inline bool GetGIShowProbes() const{ return pGIShowProbes; }
	inline bool GetGIShowProbeOffsets() const{ return pGIShowProbeOffsets; }
	inline bool GetGIShowProbeUpdate() const{ return pGIShowProbeUpdate; }
	inline int GetGIShowCascade() const{ return pGIShowCascade; }
	
	
	
	/** Retrieves the debug image texture or NULL if not existing. */
	inline deoglTexture *GetTextureDebugImage() const{ return pTextureDebugImage; }
	/**
	 * Retrieves the debug image framebuffer for the given size. Attached the debug image texture to the
	 * framebuffer. If the texture does not exist or the size does not match it is created first.
	 */
	deoglFramebuffer *GetFBODebugImageWith( int width, int height );
	/** Check if the debug image has been used since the last time deleting it if so. */
	void CheckDebugImageUse();
	
	/**
	 * Executes a command. If the command is recognized true is returned
	 * otherwise false.
	 */
	bool ExecuteCommand( const decUnicodeArgumentList &command, decUnicodeString &answer );
	/*@}*/
	
private:
	void pCmdHelp( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdEnable( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	void pCmdShowVisComponent( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdShowVisLight( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdShowComponentLodLevels( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	void pCmdShowHeightTerrain( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdShowPropFieldBox( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdShowPropFieldClusters( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	void pCmdQuickTest( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	void pCmdOpenGLCaps( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	void pCmdStats( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	void pCmdCapabilities( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdMemoryInfo( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	void pCmdDebugSnapshot( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	void pCmdShowLightFullBox( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdShowLightBox( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdShowLightVolume( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdShowLightRooms( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdShowLightVisualInfo( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	void pCmdShowTranspLayerCount( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdHighlightTransparentObjects( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	void pCmdTests( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	void pCmdDebugRenderPlan( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdShowMemoryInfo( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdLogMemoryConsumption( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	void pCmdShowOccMapLevel( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	void pCmdShowEnvMaps( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	void pCmdShowEnvMapHull( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	void pCmdShowSSAO( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	void pCmdTestGenerateShader( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	void pCmdDebugEnableLightDepthStencil( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	void pCmdShowDebugInfo( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdDebugInfoSync( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdDebugInfoLog( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdDebugInfoDetails( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	void pCmdGIShowProbes( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdGIShowProbeOffsets( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdGIShowProbeUpdate( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdGIShowCascade( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	bool pBaseCmdBool( const decUnicodeArgumentList &command, decUnicodeString &answer,
		bool &variable, const char *commandName );
	bool pBaseCmdInt( const decUnicodeArgumentList &command, decUnicodeString &answer,
		int &variable, const char *commandName );
};

#endif
