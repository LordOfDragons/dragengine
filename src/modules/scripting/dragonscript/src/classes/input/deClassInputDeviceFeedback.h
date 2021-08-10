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

#ifndef _DEDSCLASSINPUTDEVICEFEEDBACK_H_
#define _DEDSCLASSINPUTDEVICEFEEDBACK_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;
class dedsInputDevice;


/**
 * \brief Input device feedback script class.
 */
class deClassInputDeviceFeedback : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsInputDeviceFeedbackType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassInputDeviceFeedback( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassInputDeviceFeedback();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Push feedback. */
	void PushFeedback( dsRunTime *rt, dedsInputDevice *device, int index );
	
	inline dsClass *GetClassInputDeviceFeedbackType() const{ return pClsInputDeviceFeedbackType; }
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsIDFeedback;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsString;
		dsClass *clsObject;
		
		dsClass *clsInputDevice;
		dsClass *clsInputDeviceFeedbackType;
		dsClass *clsImage;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetInputDevice );
	DEF_NATFUNC( nfGetFeedbackIndex );
	
	DEF_NATFUNC( nfGetID );
	DEF_NATFUNC( nfGetName);
	DEF_NATFUNC( nfGetType );
	DEF_NATFUNC( nfGetDisplayImage );
	DEF_NATFUNC( nfGetDisplayIconCount );
	DEF_NATFUNC( nfGetDisplayIconAt );
	DEF_NATFUNC( nfGetLargestDisplayIconX );
	DEF_NATFUNC( nfGetLargestDisplayIconY );
	DEF_NATFUNC( nfGetDisplayText );
	
	DEF_NATFUNC( nfGetValue );
	DEF_NATFUNC( nfSetValue );
	
	DEF_NATFUNC( nfEquals );
#undef DEF_NATFUNC
};

#endif
