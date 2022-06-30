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

#ifndef _DECLASSNETWORKSTATE_H_
#define _DECLASSNETWORKSTATE_H_

#include <libdscript/libdscript.h>

class deNetworkState;
class deScriptingDragonScript;


/**
 * \brief Network state native script class.
 */
class deClassNetworkState : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsNetworkStateValueType;
	dsClass *pClsNetworkStateValueFormat;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new native script class. */
	deClassNetworkState( deScriptingDragonScript &ds );
	
	/** \brief Clean up native script class. */
	virtual ~deClassNetworkState();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Script module. */
	inline const deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Network state from object. */
	deNetworkState *GetNetworkState( dsRealObject *myself ) const;
	
	/** \brief Push network state. */
	void PushNetworkState( dsRunTime *rt, deNetworkState *state );
	
	inline dsClass *GetClassNetworkStateValueType() const{ return pClsNetworkStateValueType; }
	inline dsClass *GetClassNetworkStateValueFormat() const{ return pClsNetworkStateValueFormat; }
	/*@}*/
	
	
	
	
private:
	struct sInitData{
		dsClass *clsNS, *clsVoid, *clsInt, *clsFlt, *clsBool, *clsStr;
		dsClass *clsPt, *clsPt3, *clsVec, *clsQuat, *clsObj, *clsNSL;
		dsClass *clsDVec;
		dsClass *clsVector2;
		dsClass *clsNetworkStateValueType;
		dsClass *clsNetworkStateValueFormat;
		dsClass *clsMemoryFile;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetReadOnly );
	
	DEF_NATFUNC( nfGetValueCount );
	DEF_NATFUNC( nfAddValue );
	DEF_NATFUNC( nfGetValueFormatAt );
	DEF_NATFUNC( nfGetValueIntAt );
	DEF_NATFUNC( nfGetValueFloatAt );
	DEF_NATFUNC( nfGetValueStringAt );
	DEF_NATFUNC( nfGetValuePoint2At );
	DEF_NATFUNC( nfGetValuePoint3At );
	DEF_NATFUNC( nfGetValueVector2At );
	DEF_NATFUNC( nfGetValueVector3At );
	DEF_NATFUNC( nfGetValueDVector3At );
	DEF_NATFUNC( nfGetValueQuaternionAt );
	DEF_NATFUNC( nfSetValueIntAt );
	DEF_NATFUNC( nfSetValueFloatAt );
	DEF_NATFUNC( nfSetValueStringAt );
	DEF_NATFUNC( nfSetValuePoint2At );
	DEF_NATFUNC( nfSetValuePoint3At );
	DEF_NATFUNC( nfSetValueVector2At );
	DEF_NATFUNC( nfSetValueVector3At );
	DEF_NATFUNC( nfSetValueDVector3At );
	DEF_NATFUNC( nfSetValueQuaternionAt );
	DEF_NATFUNC( nfGetValuePrecisionAt );
	DEF_NATFUNC( nfSetValuePrecisionAt );
	DEF_NATFUNC( nfGetValueDataAt );
	DEF_NATFUNC( nfSetValueDataAt );
	
	DEF_NATFUNC( nfGetStateListener );
	DEF_NATFUNC( nfSetStateListener );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
