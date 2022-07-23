/* 
 * Drag[en]gine DragonScript Script Module
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

#ifndef _DECLASSINPUTDEVICE_H_
#define _DECLASSINPUTDEVICE_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;

class dedsInputDevice;



/**
 * \brief Input device script class.
 */
class deClassInputDevice : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsInputDeviceType;
	dsClass *pClsInputDeviceBoneConfiguration;
	dsClass *pClsInputEventSource;
	dsClass *pClsInputEventKeyLocation;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassInputDevice( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassInputDevice();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief InputDevice or \em NULL if myself is \em NULL. */
	dedsInputDevice *GetInputDevice( dsRealObject *myself ) const;
	
	/** \brief Push input device which can be \em NULL. */
	void PushInputDevice( dsRunTime *rt, dedsInputDevice *device );
	
	inline dsClass *GetClassInputDeviceType() const{ return pClsInputDeviceType; }
	inline dsClass *GetClassInputDeviceBoneConfiguration() const{ return pClsInputDeviceBoneConfiguration; }
	inline dsClass *GetClassInputEventSource() const{ return pClsInputEventSource; }
	inline dsClass *GetClassInputEventKeyLocation() const{ return pClsInputEventKeyLocation; }
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsInputDevice;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsString;
		dsClass *clsObject;
		
		dsClass *clsImage;
		dsClass *clsModel;
		dsClass *clsSkin;
		dsClass *clsIDAxis;
		dsClass *clsIDButton;
		dsClass *clsIDFeedback;
		dsClass *clsIDComponent;
		dsClass *clsInputDeviceType;
		dsClass *clsInputDeviceBoneConfiguration;
		dsClass *clsInputEventSource;
		dsClass *clsInputEventKeyLocation;
		dsClass *clsVector;
		dsClass *clsQuaternion;
		dsClass *clsMatrix;
		dsClass *clsRig;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetSource );
	DEF_NATFUNC( nfGetIndex );
	DEF_NATFUNC( nfGetID );
	DEF_NATFUNC( nfGetName );
	DEF_NATFUNC( nfGetType );
	DEF_NATFUNC( nfGetDisplayModel );
	DEF_NATFUNC( nfGetDisplaySkin );
	DEF_NATFUNC( nfGetDisplayImage );
	DEF_NATFUNC( nfGetDisplayIconCount );
	DEF_NATFUNC( nfGetDisplayIconAt );
	DEF_NATFUNC( nfGetLargestDisplayIconX );
	DEF_NATFUNC( nfGetLargestDisplayIconY );
	DEF_NATFUNC( nfGetDisplayText );
	
	DEF_NATFUNC( nfGetAxisCount );
	DEF_NATFUNC( nfGetAxisAt );
	
	DEF_NATFUNC( nfGetButtonCount );
	DEF_NATFUNC( nfGetButtonAt );
	
	DEF_NATFUNC( nfGetFeedbackCount );
	DEF_NATFUNC( nfGetFeedbackAt );
	
	DEF_NATFUNC( nfGetComponentCount );
	DEF_NATFUNC( nfGetComponentAt );
	
	DEF_NATFUNC( nfButtonMatchingKeyCode );
	DEF_NATFUNC( nfButtonMatchingKeyChar );
	DEF_NATFUNC( nfButtonMatchingKeyCode2 );
	DEF_NATFUNC( nfButtonMatchingKeyChar2 );
	
	DEF_NATFUNC( nfGetBoneConfiguration );
	DEF_NATFUNC( nfGetFingerTipOffset );
	DEF_NATFUNC( nfGetSupportsFaceEyeExpressions );
	DEF_NATFUNC( nfGetSupportsFaceMouthExpressions );
	DEF_NATFUNC( nfGetHandRig );
	DEF_NATFUNC( nfGetVRModel );
	DEF_NATFUNC( nfGetVRSkin );
	
	DEF_NATFUNC( nfIsPresent );
	
	DEF_NATFUNC( nfGetDevicePosePosition );
	DEF_NATFUNC( nfGetDevicePoseOrientation );
	DEF_NATFUNC( nfGetDevicePoseLinearVelocity );
	DEF_NATFUNC( nfGetDevicePoseAngularVelocity );
	DEF_NATFUNC( nfGetDevicePoseMatrix );
	
	DEF_NATFUNC( nfGetDeviceBonePoseCount );
	DEF_NATFUNC( nfGetDeviceBonePosePosition );
	DEF_NATFUNC( nfGetDeviceBonePoseOrientation );
	DEF_NATFUNC( nfGetDeviceBonePoseMatrix );
	
	DEF_NATFUNC( nfGetDeviceFaceExpressionCount );
	DEF_NATFUNC( nfGetDeviceFaceExpressionAt );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
