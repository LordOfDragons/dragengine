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
	DEF_NATFUNC(nfGetButtonNear);
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
