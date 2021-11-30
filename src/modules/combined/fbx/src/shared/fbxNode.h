/* 
 * FBX Modules
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

#ifndef _FBXNODE_H_
#define _FBXNODE_H_


#include <stdint.h>

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/collection/decPointerList.h>

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>


class fbxProperty;

class decBaseFileReader;
class decBaseFileWriter;
class deBaseModule;
class decStringSet;


/**
 * \brief FBX node.
 */
class fbxNode : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<fbxNode> Ref;
	
	
	
private:
	decString pName;
	decObjectOrderedSet pProperties;
	decObjectOrderedSet pNodes;
	fbxNode *pNodeProperties70;
	int64_t pID;
	
	decVector pLocalTranslation;
	decVector pLocalRotation;
	decVector pLocalScaling;
	decMatrix pTransformation;
	decMatrix pInverseTransformation;
	bool pDirtyTransformation;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create node. */
	fbxNode();
	
	/** \brief Load node. */
	fbxNode( decBaseFileReader &reader );
	
	/** \brief Load node with already read end offset. */
	fbxNode( decBaseFileReader &reader, int endOffset );
	
protected:
	/** \brief Clean up node. */
	virtual ~fbxNode();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	
	
	/** \brief Count of properties. */
	int GetPropertyCount() const;
	
	/** \brief Property at index. */
	fbxProperty *GetPropertyAt( int index ) const;
	
	/** \brief Get named object property value. */
	bool GetPropertyBoolIf( const char *name, bool &value ) const;
	bool GetPropertyIntIf( const char *name, int &value ) const;
	bool GetPropertyLongIf( const char *name, int64_t &value ) const;
	bool GetPropertyFloatIf( const char *name, float &value ) const;
	bool GetPropertyDoubleIf( const char *name, double &value ) const;
	bool GetPropertyStringIf( const char *name, decString &value ) const;
	bool GetPropertyVectorIf( const char *name, decVector &value ) const;
	bool GetPropertyColorIf( const char *name, decColor &value ) const;
	
	bool GetPropertyBool( const char *name ) const;
	int GetPropertyInt( const char *name ) const;
	int64_t GetPropertyLong( const char *name ) const;
	float GetPropertyFloat( const char *name ) const;
	double GetPropertyDouble( const char *name ) const;
	const decString &GetPropertyString( const char *name ) const;
	decVector GetPropertyVector( const char *name ) const;
	decColor GetPropertyColor( const char *name ) const;
	
	bool GetPropertyBool( const char *name, bool defaultValue ) const;
	int GetPropertyInt( const char *name, int defaultValue ) const;
	int64_t GetPropertyLong( const char *name, int64_t defaultValue ) const;
	float GetPropertyFloat( const char *name, float defaultValue ) const;
	double GetPropertyDouble( const char *name, double defaultValue ) const;
	const decString &GetPropertyString( const char *name, const decString &defaultValue ) const;
	decVector GetPropertyVector( const char *name, const decVector &defaultValue ) const;
	decColor GetPropertyColor( const char *name, const decColor &defaultValue ) const;
	
	/** \brief Transformation. */
	const decVector &GetLocalTranslation();
	const decVector &GetLocalRotation();
	const decVector &GetLocalScaling();
	const decMatrix &GetTransformation();
	const decMatrix &GetInverseTransformation();
	
	/** \brief Object ID or 0 if this is not an object node. */
	inline int64_t GetID() const{ return pID; }
	
	/** \brief Set object ID or 0 if this is not an object node. */
	void SetID( int64_t id );
	
	
	
	/** \brief Count of nodes. */
	int GetNodeCount() const;
	
	/** \brief Node at index. */
	fbxNode *GetNodeAt( int index ) const;
	
	/** \brief Add node. */
	void AddNode( fbxNode *node );
	
	/** \brief First node matching name or NULL if absent. */
	fbxNode *FirstNodeNamed( const char *name ) const;
	
	/** \brief First node matching name or NULL if absent. */
	fbxNode *FirstNodeNamedOrNull( const char *name ) const;
	
	/** \brief Find all nodes matching name. */
	void FindNodesNamed( decPointerList &list, const char *name ) const;
	
	/** \brief Names of all nodes. */
	void GetNodeNames( decStringSet &list ) const;
	
	
	
	/** \brief Prepare after reading. */
	void Prepare( deBaseModule &module );
	
	
	
	/** \brief Save to file. */
	void Save( decBaseFileWriter &writer );
	
	/** \brief Debug print node structure. */
	void DebugPrintStructure( deBaseModule &module, const decString &prefix, bool verbose = false ) const;
	/*@}*/
	
	
	
private:
	void pRead( decBaseFileReader &reader, int endOffset );
	fbxNode *pProp70Named( const char *name ) const;
	void pInitID();
	void pPrepareTransformation();
};

#endif
