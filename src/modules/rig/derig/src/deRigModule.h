/* 
 * Drag[en]gine Rig Module
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

#ifndef _DERIGMODULE_H_
#define _DERIGMODULE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/collider/deColliderConstraint.h>
#include <dragengine/systems/modules/rig/deBaseRigModule.h>

class decShapeList;
class decXmlElementTag;
class decXmlAttValue;
class deRigBone;
class deRigConstraint;
class dermNameList;
class decXmlWriter;



/**
 * \brief Drag[en]gine Rig Module.
 *
 * Rig module for loading and saving rig files in the Drag[en]gine
 * XML Rig File Format.
 */
class deRigModule : public deBaseRigModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deRigModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deRigModule();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Load rig. */
	virtual void LoadRig( decBaseFileReader &reader, deRig &rig );
	
	/** \brief Save rig. */
	virtual void SaveRig( decBaseFileWriter &writer, const deRig &rig );
	/*@}*/
	
private:
	decXmlElementTag *pGetTagAt( decXmlElementTag *tag, int index );
	decXmlElementTag *pGetTagAt( const decXmlElementTag &tag, int index );
	decXmlAttValue *pFindAttribute( decXmlElementTag *tag, const char *name );
	const char *pGetAttributeString( decXmlElementTag *tag, const char *name );
	int pGetAttributeInt( decXmlElementTag *tag, const char *name );
	float pGetAttributeFloat( decXmlElementTag *tag, const char *name );
	
	void pParseRig( decXmlElementTag *root, deRig &rig );
	void pParseBone( decXmlElementTag *root, deRig &rig, dermNameList &boneNameList );
	void pParseBoneIK( decXmlElementTag *root, float &lower, float &upper, float &resistance, bool &locked );
	void pParseSphere( decXmlElementTag *root, decShapeList &shapes, decStringList &shapeProperties );
	void pParseCylinder( decXmlElementTag *root, decShapeList &shapes, decStringList &shapeProperties );
	void pParseCapsule( decXmlElementTag *root, decShapeList &shapes, decStringList &shapeProperties );
	void pParseBox( decXmlElementTag *root, decShapeList &shapes, decStringList &shapeProperties );
	void pParseHull( decXmlElementTag *root, decShapeList &shapes, decStringList &shapeProperties );
	void pParseVector( decXmlElementTag *root, decVector &vector );
	void pParseConstraint( decXmlElementTag *root, deRig &rig, deRigBone *bone, dermNameList &boneNameList );
	void pParseConstraintDof( const decXmlElementTag &root, deColliderConstraintDof &dof, bool linearConstraint );
	void pParseConstraintLimits( decXmlElementTag *root, deRigConstraint *constraint );
	void pParseConstraintLimitsLinear( decXmlElementTag *root, deRigConstraint *constraint );
	void pParseConstraintLimitsAngular( decXmlElementTag *root, deRigConstraint *constraint );
	void pParseConstraintSpringStiffness( decXmlElementTag *root, deRigConstraint *constraint );
	void pParseConstraintDamping( decXmlElementTag *root, deRigConstraint *constraint );
	
	void pWriteRig( decXmlWriter &writer, const deRig &rig );
	void pWriteBone( decXmlWriter &writer, const deRig &rig, const deRigBone &bone );
	void pWriteConstraint( decXmlWriter &writer, const deRig &rig, const deRigConstraint &constraint );
	void pWriteConstraintDof( decXmlWriter &writer, const deColliderConstraintDof &dof,
		const char *tagName, bool linearConstraint );
};

#endif
