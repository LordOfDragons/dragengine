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

#ifndef _DECLASSNETWORKMESSAGE_H_
#define _DECLASSNETWORKMESSAGE_H_

#include <libdscript/libdscript.h>

class deNetworkMessage;
class deScriptingDragonScript;



/**
 * \brief Network message script class.
 */
class deClassNetworkMessage : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassNetworkMessage( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassNetworkMessage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module owning the script class. */
	inline deScriptingDragonScript &GetDS(){ return pDS; }
	inline const deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create script class members. */
	virtual void CreateClassMembers( dsEngine *engine );
	
	
	/**
	 * \brief Network message from script object.
	 * \details Returns NULL if \em myself is NULL.
	 */
	deNetworkMessage *GetNetworkMessage( dsRealObject *myself ) const;
	
	/**
	 * \brief Push network message onto stack.
	 * \details Pushes a \em null object to the stack if \em collider is NULL.
	 */
	void PushNetworkMessage( dsRunTime *rt, deNetworkMessage *message );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsNetworkMessage;
		dsClass *clsVoid;
		dsClass *clsInteger;
		dsClass *clsBool;
		dsClass *clsString;
		dsClass *clsObject;
		dsClass *clsFileReader;
		dsClass *clsFileWrite;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetDataLength );
	DEF_NATFUNC( nfClear );
	
	DEF_NATFUNC( nfGetReader );
	DEF_NATFUNC( nfGetWriter );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
