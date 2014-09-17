/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: LAPA - secondary contact: NOBODY
#ifndef Navigation_TargetOnPathConfig_H
#define Navigation_TargetOnPathConfig_H

#include "gwnavruntime/base/memory.h"

namespace Kaim
{

/// Set of parameters used by TargetOnPathComputer class to update Bot target on path position.
/// This class is aggregated into PathFollowerConfig; you don't need to directly instantiate
/// one for your Bot.
class TargetOnPathConfig
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_Bot)

public:
	// ---------------------------------- Public Member Functions ----------------------------------

	TargetOnPathConfig() { SetDefaults(); }

	/// Sets all members to their default value.
	void SetDefaults()
	{
		m_maxDistanceFromBot = 10.0f;
		m_samplingDistance = 1.0f;

		m_shortcutAttemptDistanceRatio = 0.7f;
		m_shortcutForceAttemptDistance = 0.5f;

		m_backwardSamplingDistance = 1.0f;
		m_backwardMaxDistance = 10.0f;
	}


	// ---------------------------------- Forward Shortcut Parameters ----------------------------------
	/// These parameters control the way shortcut candidate position are computed.

	/// The shortcut candidates are generated up to reach this distance.
	/// If the Bot has moved backward, it may happen the target on path stays farther from the Bot than this distance.
	KyFloat32 m_maxDistanceFromBot;

	/// The maximal distance between two consecutive target candidate position (Path nodes are always checked).
	KyFloat32 m_samplingDistance;


	// ---------------------------------- Forward Shortcut Control Parameters ----------------------------------
	/// These parameters control the shortcut attempt adaptive frequency.
	/// When the bot is far from its current target on path, it will try to shortcut
	/// only when it moved enough since last attempt. When close to its target on path
	/// it will attempt every frame.

	KyFloat32 m_shortcutAttemptDistanceRatio; ///< Triggers new attempt when the bot get closer to TOP than #m_shortcutAttemptDistanceRatio * {distance to TOP at last attempt}.
	KyFloat32 m_shortcutForceAttemptDistance; ///< Always try to shortcut when the bot is closer to TOP than #m_shortcutForceAttemptDistance.


	// ---------------------------------- Backward Sampling Parameters  ----------------------------------
	/// These parameters control the backward search when current target on path is no more
	/// visible. In than case, we sample PositionOnPaths backward from current target on path
	/// until we either find a visible position or reach the previous PathEvent.

	KyFloat32 m_backwardSamplingDistance; ///< The backward sampling precision.
	KyFloat32 m_backwardMaxDistance; ///< The maximal distance along Path on which backward sampling is done.
};


} // namespace Kaim

#endif // Navigation_TargetOnPathConfig_H
