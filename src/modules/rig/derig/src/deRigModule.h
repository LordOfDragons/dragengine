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
