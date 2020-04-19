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

#ifndef _DECLASSINPUTEVENT_H_
#define _DECLASSINPUTEVENT_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;

class deInputEvent;



/**
 * \brief Input event script class.
 */
class deClassInputEvent : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsInputEventType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassInputEvent( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassInputEvent();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief InputEvent or \em NULL if myself is \em NULL. */
	const deInputEvent &GetInputEvent( dsRealObject *myself ) const;
	
	/** \brief Push input event which can be \em NULL. */
	void PushInputEvent( dsRunTime *rt, const deInputEvent &event );
	
	inline dsClass *GetClassInputEventType() const{ return pClsInputEventType; }
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsInputEvent;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsString;
		dsClass *clsObject;
		dsClass *clsInputEventType;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetType );
	DEF_NATFUNC( nfGetDevice );
	DEF_NATFUNC( nfGetCode );
	DEF_NATFUNC( nfGetState );
	DEF_NATFUNC( nfGetKeyCode );
	DEF_NATFUNC( nfGetKeyChar );
	DEF_NATFUNC( nfGetX );
	DEF_NATFUNC( nfGetY );
	DEF_NATFUNC( nfGetValue );
	DEF_NATFUNC( nfGetTime );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
