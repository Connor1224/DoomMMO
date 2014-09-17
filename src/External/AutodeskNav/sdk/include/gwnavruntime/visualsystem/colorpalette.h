/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/




// primary contact: MAMU - secondary contact: 
#ifndef Navigation_ColorPalette_H
#define Navigation_ColorPalette_H

#include "gwnavruntime/base/memory.h"
#include "gwnavruntime/visualsystem/visualcolor.h"
#include "gwnavruntime/containers/kyarray.h"

namespace Kaim
{


/// Represents a palette of colors.
/// Each instance of this class stores a configurable array of VisualColor objects, and provides access to those
/// colors from their indices within the array.
/// Use of this class is entirely optional. It is provided for your convenience in writing your own visual
/// representations for Gameware Navigation data.
class ColorPalette
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_VisualRep)

public:
	/// Constructs a new ColorPalette.
	/// \param maxColors			Sets the initial size of the #m_colors array.
	/// \param defaultColor			Sets the value of #m_defaultColor. 
	ColorPalette(KyUInt32 maxColors, VisualColor defaultColor = VisualColor::White);

	/// Clears all information maintained by this object. 
	void Clear();

	/// Clears all information maintained by this object.
	/// \param maxColors			Sets the initial size of the #m_colors array.
	/// \param defaultColor			Sets the value of #m_defaultColor.
	/// \param isLightingEnabled	Sets the value of #m_isLightingEnabled. 
	void Init(KyUInt32 maxColors, VisualColor defaultColor = VisualColor::White, bool isLightingEnabled = false);

	/// Returns the color at the specified index within #m_colors.
	/// If the index is out of bounds, the default color is returned. 
	VisualColor GetColor(KyUInt32 index) const;

	/// Sets the color at the specified index within #m_colors. Returns #KY_ERROR if the specified index 
	/// is out of bounds. 
	KyResult SetColor(KyUInt32 index, VisualColor color);

	/// Read accessor for #m_isLightingEnabled. 
	bool IsLightingEnabled() const { return m_isLightingEnabled; }

public:
	KyArray<VisualColor> m_colors;      ///< Stores the list of colors in this palette. 
	VisualColor m_defaultColor;       ///< Stores the default color for this palette, returned by GetColor() if there is an error or if no color is registered for the specified index. 
	bool m_isLightingEnabled;         ///< If true, normals will be generated for this visual representation. 
};


}

#endif
