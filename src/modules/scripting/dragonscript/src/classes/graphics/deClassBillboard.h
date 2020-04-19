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

#ifndef _DECLASSBILLBOARD_H_
#define _DECLASSBILLBOARD_H_

#include <libdscript/libdscript.h>

class deEngine;
class deBillboard;
class deScriptingDragonScript;



/**
 * \brief Billboard Script Class.
 */
class deClassBillboard : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassBillboard( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassBillboard();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Billboard from object. */
	deBillboard *GetBillboard( dsRealObject *myself ) const;
	
	/** \brief Pushe billboard. */
	void PushBillboard( dsRunTime *rt, deBillboard *billboard );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsBillboard;
		dsClass *clsVoid;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsString;
		dsClass *clsBool;
		dsClass *clsObject;
		
		dsClass *clsVector;
		dsClass *clsVector2;
		dsClass *clsDVector;
		dsClass *clsSkin;
		dsClass *clsDynamicSkin;
		dsClass *clsLayerMask;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetPosition );
	DEF_NATFUNC( nfSetPosition );
	DEF_NATFUNC( nfGetAxis );
	DEF_NATFUNC( nfSetAxis );
	DEF_NATFUNC( nfGetSize );
	DEF_NATFUNC( nfSetSize );
	DEF_NATFUNC( nfGetOffset );
	DEF_NATFUNC( nfSetOffset );
	DEF_NATFUNC( nfGetSkin );
	DEF_NATFUNC( nfSetSkin );
	DEF_NATFUNC( nfGetDynamicSkin );
	DEF_NATFUNC( nfSetDynamicSkin );
	DEF_NATFUNC( nfGetLocked );
	DEF_NATFUNC( nfSetLocked );
	DEF_NATFUNC( nfGetSpherical );
	DEF_NATFUNC( nfSetSpherical );
	DEF_NATFUNC( nfGetSizeFixedToScreen );
	DEF_NATFUNC( nfSetSizeFixedToScreen );
	DEF_NATFUNC( nfGetVisible );
	DEF_NATFUNC( nfSetVisible );
	DEF_NATFUNC( nfGetLayerMask );
	DEF_NATFUNC( nfSetLayerMask );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
