/* 
 * Drag[en]gine DragonScript Script Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DECLASSVRSYSTEM_H_
#define _DECLASSVRSYSTEM_H_

#include <dragengine/common/collection/decObjectList.h>

#include <libdscript/libdscript.h>

class deScriptingDragonScript;
class dedsInputDevice;


/**
 * \brief Input system script class.
 */
class deClassVRSystem : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	dsClass *pClsVRFeatureSupportLevel;
	
	decObjectList pCachedDevices;
	bool pCacheDirty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassVRSystem( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassVRSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create class members. */
	virtual void CreateClassMembers( dsEngine *engine );
	
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Cached input device count. */
	int GetCachedDeviceCount();
	
	/** \brief Cached device. */
	dedsInputDevice *GetCachedDeviceAt( int index );
	
	/** \brief Invalidate cached devices. */
	void InvalidCachedDevices();
	
	/** Frame update. */
	void OnFrameUpdate();
	
	inline dsClass *GetClassVRFeatureSupportLevel() const{ return pClsVRFeatureSupportLevel; }
	/*@}*/
	
	
	
private:
	void pUpdateCachedDevices();
	
	
	struct sInitData{
		dsClass *clsVRSystem;
		dsClass *clsVoid;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsBool;
		dsClass *clsString;
		
		dsClass *clsModPar;
		dsClass *clsInputDevice;
		dsClass *clsCamera;
		dsClass *clsVRFeatureSupportLevel;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	
	DEF_NATFUNC( nfRuntimeUsable );
	DEF_NATFUNC( nfRequestFeatureEyeGazeTracking );
	DEF_NATFUNC( nfRequestFeatureFacialTracking );
	DEF_NATFUNC( nfStartRuntime );
	DEF_NATFUNC( nfStopRuntime );
	DEF_NATFUNC( nfIsRuntimeRunning );
	
	DEF_NATFUNC( nfGetCamera );
	DEF_NATFUNC( nfSetCamera );
	DEF_NATFUNC( nfSupportsPassthrough );
	DEF_NATFUNC( nfGetEnablePassthrough );
	DEF_NATFUNC( nfSetEnablePassthrough );
	DEF_NATFUNC( nfGetPassthroughTransparency );
	DEF_NATFUNC( nfSetPassthroughTransparency );
	
	DEF_NATFUNC( nfGetDeviceCount );
	DEF_NATFUNC( nfGetDeviceAt );
	DEF_NATFUNC( nfIndexOfDeviceWithID );
	DEF_NATFUNC( nfIndexOfButtonWithID );
	DEF_NATFUNC( nfIndexOfAxisWithID );
	DEF_NATFUNC( nfIndexOfFeedbackWithID );
	DEF_NATFUNC( nfGetButtonPressed );
	DEF_NATFUNC( nfGetButtonTouched );
	DEF_NATFUNC( nfGetAxisValue );
	DEF_NATFUNC( nfGetFeedbackValue );
	DEF_NATFUNC( nfSetFeedbackValue );
	
	DEF_NATFUNC( nfGetParameterCount );
	DEF_NATFUNC( nfGetParameterInfo );
	DEF_NATFUNC( nfGetParameterInfo2 );
	DEF_NATFUNC( nfGetParameterValue );
	DEF_NATFUNC( nfSetParameterValue );
	DEF_NATFUNC( nfSendCommand );
#undef DEF_NATFUNC
};

#endif
