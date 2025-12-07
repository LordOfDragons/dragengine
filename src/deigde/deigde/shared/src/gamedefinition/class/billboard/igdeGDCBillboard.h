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

#ifndef _IGDEGDCBILLBOARD_H_
#define _IGDEGDCBILLBOARD_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>



/**
 * \brief Game Definition Billboard.
 */
class DE_DLL_EXPORT igdeGDCBillboard : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeGDCBillboard> Ref;


public:
	/** \brief Properties. */
	enum eProperties{
		epSkin,
		epAxis,
		epOffset,
		epLocked,
		epSpherical,
		epRenderEnvMap,
		epAttachPosition
	};
	
	
	
private:
	decString pSkinPath;
	decVector pAxis;
	decVector2 pSize;
	decVector2 pOffset;
	bool pLocked;
	bool pSpherical;
	bool pSizeFixedToScreen;
	
	bool pDoNotScale;
	bool pPartialHide;
	bool pRenderEnvMap;
	decDVector pPosition;
	decString pBoneName;
	
	decString pPropertyNames[epAttachPosition + 1];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game definition billboard. */
	igdeGDCBillboard();
	
	/** \brief Create game definition billboard as a copy of another game definition billboard. */
	igdeGDCBillboard(const igdeGDCBillboard &billboard);
	
	/** \brief Clean up game definition billboard. */
	virtual ~igdeGDCBillboard();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Skin path. */
	inline const decString &GetSkinPath() const{return pSkinPath;}
	
	/** \brief Set skin path. */
	void SetSkinPath(const char *path);
	
	/** \brief Axis. */
	inline const decVector &GetAxis() const{return pAxis;}
	
	/** \brief Set axis. */
	void SetAxis(const decVector &axis);
	
	/** \brief Size. */
	inline const decVector2 &GetSize() const{return pSize;}
	
	/** \brief Set size. */
	void SetSize(const decVector2 &size);
	
	/** \brief Offset. */
	inline const decVector2 &GetOffset() const{return pOffset;}
	
	/** \brief Set offset. */
	void SetOffset(const decVector2 &offset);
	
	/** \brief Determines if the billboard is locked. */
	inline bool GetLocked() const{return pLocked;}
	
	/** \brief Set billboard is locked. */
	void SetLocked(bool locked);
	
	/** \brief Determines if the billboard is spherical. */
	inline bool GetSpherical() const{return pSpherical;}
	
	/** \brief Sets if the billboard is spherical. */
	void SetSpherical(bool spherical);
	
	/** \brief Determines if the billboard size is fixed to the screen size. */
	inline bool GetSizeFixedToScreen() const{return pSizeFixedToScreen;}
	
	/** \brief Sets if the billboard size is fixed to the screen size. */
	void SetSizeFixedToScreen(bool sizeFixedToScreen);
	
	
	
	/** \brief Determines if this billboard should not be scaled. */
	inline bool GetDoNotScale() const{return pDoNotScale;}
	
	/** \brief Sets if this billboard should not be scaled. */
	void SetDoNotScale(bool doNotScale);
	
	/** \brief Determines if the billboard can be partially hidden. */
	inline bool GetPartialHide() const{return pPartialHide;}
	
	/** \brief Sets if the billboard can be partially hidden. */
	void SetPartialHide(bool partiallyHidden);
	
	/** \brief Determines if the billboard is rendered into environment map probes. */
	inline bool GetRenderEnvMap() const{return pRenderEnvMap;}
	
	/** \brief Sets if the billboard is rendered into environment map probes. */
	void SetRenderEnvMap(bool renderEnvMap);
	
	/** \brief Position. */
	inline const decDVector &GetPosition() const{return pPosition;}
	
	/** \brief Set position. */
	void SetPosition(const decDVector &position);
	
	/** \brief Name of the bone to attach to or empty string. */
	inline const decString &GetBoneName() const{return pBoneName;}
	
	/** \brief Set name of bone to attach to or empty string. */
	void SetBoneName(const char *boneName);
	
	
	
	
	/** \brief Determines if the name of a property is set or not (empty string). */
	bool IsPropertySet(eProperties property) const;
	
	/** \brief Name of the given property. */
	const decString &GetPropertyName(eProperties property) const;
	
	/** \brief Set name of the given property. */
	void SetPropertyName(eProperties property, const char *name);
	
	/** \brief Determines if one or more properties use a name. */
	bool HasPropertyWithName(const char *name) const;
	/*@}*/
};

#endif
