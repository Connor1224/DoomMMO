// This code contains NVIDIA Confidential Information and is disclosed to you
// under a form of NVIDIA software license agreement provided separately to you.
//
// Notice
// NVIDIA Corporation and its licensors retain all intellectual property and
// proprietary rights in and to this software and related documentation and
// any modifications thereto. Any use, reproduction, disclosure, or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA Corporation is strictly prohibited.
//
// ALL NVIDIA DESIGN SPECIFICATIONS, CODE ARE PROVIDED "AS IS.". NVIDIA MAKES
// NO WARRANTIES, EXPRESSED, IMPLIED, STATUTORY, OR OTHERWISE WITH RESPECT TO
// THE MATERIALS, AND EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES OF NONINFRINGEMENT,
// MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Information and code furnished is believed to be accurate and reliable.
// However, NVIDIA Corporation assumes no responsibility for the consequences of use of such
// information or for any infringement of patents or other rights of third parties that may
// result from its use. No license is granted by implication or otherwise under any patent
// or patent rights of NVIDIA Corporation. Details are subject to change without notice.
// This code supersedes and replaces all information previously supplied.
// NVIDIA Corporation products are not authorized for use as critical
// components in life support devices or systems without express written approval of
// NVIDIA Corporation.
//
// Copyright (c) 2008-2013 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  

#include "PxVehicleComponents.h"
#include "PxVehicleDrive.h"
#include "PxVehicleNoDrive.h"
#include "PxVehicleDrive4W.h"
#include "PxVehicleDriveNW.h"
#include "PxVehicleDriveTank.h"
#include "PxVehicleSuspWheelTire4.h"
#include "PxVehicleSuspLimitConstraintShader.h"

#include "PsFoundation.h"
#include "PsUtilities.h"
#include "CmPhysXCommon.h"
#include "PxSerialUtils.h"
#include "PxMetaData.h"

using namespace physx;

static void getBinaryMetaData_PxFixedSizeLookupTable(PxOutputStream& stream)
{
	typedef PxFixedSizeLookupTable<PxVehicleEngineData::eMAX_NB_ENGINE_TORQUE_CURVE_ENTRIES> PxVehicleEngineTable;
    class ShadowLookupTable : public PxVehicleEngineTable
	{
	public:
		static void getBinaryMetaData(PxOutputStream& stream)
		{
			PX_DEF_BIN_METADATA_CLASS(ShadowLookupTable)
			PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleEngineTable,	PxReal, mDataPairs,		0)	
			PX_DEF_BIN_METADATA_ITEM(PxVehicleEngineTable,		        PxU32,	mNbDataPairs,	0)	
			PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleEngineTable,	PxU32,	mPad,			PxMetaDataFlag::ePADDING)
		}
	};

	ShadowLookupTable::getBinaryMetaData(stream);
    PX_DEF_BIN_METADATA_TYPEDEF(PxFixedSizeLookupTable<PxVehicleEngineData::eMAX_NB_ENGINE_TORQUE_CURVE_ENTRIES>, ShadowLookupTable)	
}

void PxVehicleDriveSimData::getBinaryMetaData(PxOutputStream& stream)
{
	getBinaryMetaData_PxFixedSizeLookupTable(stream);
	PX_DEF_BIN_METADATA_TYPEDEF(PxVehicleClutchAccuracyMode::Enum, PxU32)

	//PxVehicleEngineData
	PX_DEF_BIN_METADATA_CLASS(PxVehicleEngineData)
	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleEngineData, 		ShadowLookupTable,	mTorqueCurve,								0)
	PX_DEF_BIN_METADATA_ITEM(PxVehicleEngineData, 		PxReal,				mMOI,										0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleEngineData, 		PxReal,				mPeakTorque,								0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleEngineData, 		PxReal,				mMaxOmega,									0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleEngineData, 		PxReal,				mDampingRateFullThrottle,					0)
	PX_DEF_BIN_METADATA_ITEM(PxVehicleEngineData, 		PxReal,				mDampingRateZeroThrottleClutchEngaged,		0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleEngineData, 		PxReal,				mDampingRateZeroThrottleClutchDisengaged,	0)
	PX_DEF_BIN_METADATA_ITEM(PxVehicleEngineData, 		PxReal,				mRecipMOI,									0)
	PX_DEF_BIN_METADATA_ITEM(PxVehicleEngineData, 		PxReal,				mRecipMaxOmega,								0)


	//PxVehicleGearsData
	PX_DEF_BIN_METADATA_CLASS(PxVehicleGearsData)
	
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleGearsData,	PxReal,		mRatios,		0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleGearsData,	PxReal,		mFinalRatio,	0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleGearsData,	PxU32,		mNbRatios,		0)
	PX_DEF_BIN_METADATA_ITEM(PxVehicleGearsData,	PxReal,		mSwitchTime,	0)
	PX_DEF_BIN_METADATA_ITEM(PxVehicleGearsData,	PxReal,		mPad,			PxMetaDataFlag::ePADDING)	

	//PxVehicleClutchData
	PX_DEF_BIN_METADATA_CLASS(PxVehicleClutchData)	
	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleClutchData,		        PxReal,                            mStrength,		   0)	

	PX_DEF_BIN_METADATA_ITEM(PxVehicleClutchData,		        PxVehicleClutchAccuracyMode::Enum, mAccuracyMode,	   0)

	PX_DEF_BIN_METADATA_ITEM(PxVehicleClutchData,		        PxU32,	                           mEstimateIterations,0)	

	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleClutchData,	PxU8,	                           mPad,		       PxMetaDataFlag::ePADDING)

	//PxVehicleAutoBoxData
	PX_DEF_BIN_METADATA_CLASS(PxVehicleAutoBoxData)	
	
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleAutoBoxData, PxReal, mUpRatios,	0)	
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleAutoBoxData, PxReal, mDownRatios,	0)	

	//PxVehicleDriveSimData
	PX_DEF_BIN_METADATA_CLASS(PxVehicleDriveSimData)
	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleDriveSimData, PxVehicleEngineData,	mEngine,	0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleDriveSimData, PxVehicleGearsData,	    mGears,		0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleDriveSimData, PxVehicleClutchData,	mClutch,	0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleDriveSimData, PxVehicleAutoBoxData,	mAutoBox,	0)	
}

void PxVehicleDrive::getBinaryMetaData(PxOutputStream& stream)
{
	//PxVehicleDriveDynData
	PX_DEF_BIN_METADATA_CLASS(PxVehicleDriveDynData)	

	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleDriveDynData,		PxReal, mControlAnalogVals,		0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleDriveDynData, 		        bool,	mUseAutoGears,			0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleDriveDynData, 		        bool,	mGearUpPressed,			0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleDriveDynData, 		        bool,	mGearDownPressed,		0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleDriveDynData, 		        PxU32,	mCurrentGear,			0)
	PX_DEF_BIN_METADATA_ITEM(PxVehicleDriveDynData, 		        PxU32,	mTargetGear,			0)
	PX_DEF_BIN_METADATA_ITEM(PxVehicleDriveDynData, 		        PxReal, mEnginespeed,			0)
	PX_DEF_BIN_METADATA_ITEM(PxVehicleDriveDynData, 		        PxReal, mGearSwitchTime,		0)
	PX_DEF_BIN_METADATA_ITEM(PxVehicleDriveDynData, 		        PxReal, mAutoBoxSwitchTime,		0)

	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleDriveDynData,		PxU32,	mPad,					PxMetaDataFlag::ePADDING)	

	//PxVehicleDrive
	PX_DEF_BIN_METADATA_VCLASS(PxVehicleDrive)
	PX_DEF_BIN_METADATA_BASE_CLASS(PxVehicleDrive, PxVehicleWheels)	

	PX_DEF_BIN_METADATA_ITEM(PxVehicleDrive,				        PxVehicleDriveDynData, mDriveDynData, 0)	
}

void PxVehicleDriveSimData4W::getBinaryMetaData(PxOutputStream& stream)
{
	//PxVehicleDifferential4WData
	PX_DEF_BIN_METADATA_CLASS(PxVehicleDifferential4WData)	

	PX_DEF_BIN_METADATA_ITEM(PxVehicleDifferential4WData, 		        PxReal,		mFrontRearSplit,		0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleDifferential4WData, 		        PxReal, 	mFrontLeftRightSplit,	0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleDifferential4WData, 		        PxReal, 	mRearLeftRightSplit,	0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleDifferential4WData, 		        PxReal, 	mCentreBias,			0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleDifferential4WData, 		        PxReal, 	mFrontBias,				0)
	PX_DEF_BIN_METADATA_ITEM(PxVehicleDifferential4WData, 		        PxReal, 	mRearBias,				0)
	PX_DEF_BIN_METADATA_ITEM(PxVehicleDifferential4WData, 		        PxU32,		mType,					0)
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleDifferential4WData,	PxReal, 	mPad,					PxMetaDataFlag::ePADDING)
	
	//PxVehicleAckermannGeometryData
	PX_DEF_BIN_METADATA_CLASS(PxVehicleAckermannGeometryData)	

	PX_DEF_BIN_METADATA_ITEM(PxVehicleAckermannGeometryData, PxReal, 	mAccuracy,			0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleAckermannGeometryData, PxReal, 	mFrontWidth,		0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleAckermannGeometryData, PxReal, 	mRearWidth,			0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleAckermannGeometryData, PxReal, 	mAxleSeparation,	0)	
	
	//PxVehicleDriveSimData4W	
	PX_DEF_BIN_METADATA_CLASS(PxVehicleDriveSimData4W)
	PX_DEF_BIN_METADATA_BASE_CLASS(PxVehicleDriveSimData4W, PxVehicleDriveSimData)	

	PX_DEF_BIN_METADATA_ITEM(PxVehicleDriveSimData4W, PxVehicleDifferential4WData,		mDiff,				0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleDriveSimData4W, PxVehicleAckermannGeometryData,	mAckermannGeometry, 0)
}

void PxVehicleDriveSimDataNW::getBinaryMetaData(PxOutputStream& stream)
{
	//PxVehicleDifferentialWData
	PX_DEF_BIN_METADATA_CLASS(PxVehicleDifferentialNWData)	
	
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleDifferentialNWData, PxU32,	mBitmapBuffer,		0)
	PX_DEF_BIN_METADATA_ITEM(PxVehicleDifferentialNWData,	          PxU32,	mNbDrivenWheels,	0)
	PX_DEF_BIN_METADATA_ITEM(PxVehicleDifferentialNWData,	          PxReal,	mInvNbDrivenWheels,0)
	PX_DEF_BIN_METADATA_ITEM(PxVehicleDifferentialNWData,	          PxU32,	mPad,				0)

	//PxVehicleDriveSimDataNW
	PX_DEF_BIN_METADATA_CLASS(PxVehicleDriveSimDataNW)
	PX_DEF_BIN_METADATA_BASE_CLASS(PxVehicleDriveSimDataNW, PxVehicleDriveSimData)	

	PX_DEF_BIN_METADATA_ITEM(PxVehicleDriveSimDataNW, PxVehicleDifferentialNWData,		mDiff,				0)	
}

void PxVehicleNoDrive::getBinaryMetaData(PxOutputStream& stream)
{
	PX_DEF_BIN_METADATA_VCLASS(PxVehicleNoDrive)
	PX_DEF_BIN_METADATA_BASE_CLASS(PxVehicleNoDrive,	PxVehicleWheels)	

	PX_DEF_BIN_METADATA_ITEM(PxVehicleNoDrive, 	           PxReal,		mSteerAngles,					PxMetaDataFlag::ePTR)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleNoDrive, 	           PxReal,		mDriveTorques,					PxMetaDataFlag::ePTR)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleNoDrive, 	           PxReal,		mBrakeTorques,					PxMetaDataFlag::ePTR)	
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleNoDrive, PxU32,		mPad,							PxMetaDataFlag::ePADDING)

	PX_DEF_BIN_METADATA_EXTRA_ITEMS(PxVehicleNoDrive, PxReal, mSteerAngles,  mWheelsSimData.mNbWheels4, 0, PX_SERIAL_DEFAULT_ALIGN)
	PX_DEF_BIN_METADATA_EXTRA_ITEMS(PxVehicleNoDrive, PxReal, mSteerAngles,  mWheelsSimData.mNbWheels4, 0, 0)
	PX_DEF_BIN_METADATA_EXTRA_ITEMS(PxVehicleNoDrive, PxReal, mSteerAngles,  mWheelsSimData.mNbWheels4, 0, 0)
	PX_DEF_BIN_METADATA_EXTRA_ITEMS(PxVehicleNoDrive, PxReal, mSteerAngles,  mWheelsSimData.mNbWheels4, 0, 0)

	PX_DEF_BIN_METADATA_EXTRA_ITEMS(PxVehicleNoDrive, PxReal, mDriveTorques,  mWheelsSimData.mNbWheels4, 0, 0)
	PX_DEF_BIN_METADATA_EXTRA_ITEMS(PxVehicleNoDrive, PxReal, mDriveTorques,  mWheelsSimData.mNbWheels4, 0, 0)
	PX_DEF_BIN_METADATA_EXTRA_ITEMS(PxVehicleNoDrive, PxReal, mDriveTorques,  mWheelsSimData.mNbWheels4, 0, 0)
	PX_DEF_BIN_METADATA_EXTRA_ITEMS(PxVehicleNoDrive, PxReal, mDriveTorques,  mWheelsSimData.mNbWheels4, 0, 0)

	PX_DEF_BIN_METADATA_EXTRA_ITEMS(PxVehicleNoDrive, PxReal, mBrakeTorques,  mWheelsSimData.mNbWheels4, 0, 0)
	PX_DEF_BIN_METADATA_EXTRA_ITEMS(PxVehicleNoDrive, PxReal, mBrakeTorques,  mWheelsSimData.mNbWheels4, 0, 0)
	PX_DEF_BIN_METADATA_EXTRA_ITEMS(PxVehicleNoDrive, PxReal, mBrakeTorques,  mWheelsSimData.mNbWheels4, 0, 0)
	PX_DEF_BIN_METADATA_EXTRA_ITEMS(PxVehicleNoDrive, PxReal, mBrakeTorques,  mWheelsSimData.mNbWheels4, 0, 0)
}

void PxVehicleNoDrive::exportExtraData(PxSerialStream& stream, void*)
{
	PxVehicleWheels::exportExtraData(stream,NULL);

	PxU32 size = sizeof(PxReal)*4*mWheelsSimData.mNbWheels4;
	stream.align();
	stream.storeBuffer(mSteerAngles, size);
	stream.storeBuffer(mDriveTorques, size);
	stream.storeBuffer(mBrakeTorques, size);
}

char* PxVehicleNoDrive::importExtraData(char* address, PxU32& totalPadding)
{
	address = PxVehicleWheels::importExtraData(address,totalPadding);

	PxU32 size = sizeof(PxReal)*4*mWheelsSimData.mNbWheels4;
	address = PxAlignStream(address, totalPadding, PX_SERIAL_DEFAULT_ALIGN);
	mSteerAngles=(PxReal*)address;
	address+=size;
	mDriveTorques=(PxReal*)address;
	address+=size;
	mBrakeTorques=(PxReal*)address;
	address+=size;
	return address;
}

void PxVehicleDrive4W::getBinaryMetaData(PxOutputStream& stream)
{
	PxVehicleDriveSimData4W::getBinaryMetaData(stream);

	PX_DEF_BIN_METADATA_VCLASS(PxVehicleDrive4W)
	PX_DEF_BIN_METADATA_BASE_CLASS(PxVehicleDrive4W,	PxVehicleDrive)	

	PX_DEF_BIN_METADATA_ITEM(PxVehicleDrive4W,			PxVehicleDriveSimData4W, mDriveSimData, 0)	
}

void PxVehicleDriveNW::getBinaryMetaData(PxOutputStream& stream)
{
	PxVehicleDriveSimDataNW::getBinaryMetaData(stream);

	PX_DEF_BIN_METADATA_VCLASS(PxVehicleDriveNW)
	PX_DEF_BIN_METADATA_BASE_CLASS(PxVehicleDriveNW,	PxVehicleDrive)	

	PX_DEF_BIN_METADATA_ITEM(PxVehicleDriveNW,			PxVehicleDriveSimDataNW, mDriveSimData, 0)	
}

void PxVehicleDriveTank::getBinaryMetaData(PxOutputStream& stream)
{
	PX_DEF_BIN_METADATA_VCLASS(PxVehicleDriveTank)
	PX_DEF_BIN_METADATA_BASE_CLASS(PxVehicleDriveTank, PxVehicleDrive)	

	PX_DEF_BIN_METADATA_ITEM(PxVehicleDriveTank,	         PxVehicleDriveSimData,	mDriveSimData,	0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleDriveTank,	         PxU32,					mDriveModel,	0)		
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleDriveTank, PxU32,					mPad,			PxMetaDataFlag::ePADDING)	
}
void PxVehicleWheelsSimData::getBinaryMetaData(PxOutputStream& stream)
{
	PxVehicleWheels4SimData::getBinaryMetaData(stream);
	//PxVehicleTireLoadFilterData
	PX_DEF_BIN_METADATA_CLASS(PxVehicleTireLoadFilterData)
	PX_DEF_BIN_METADATA_ITEM(PxVehicleTireLoadFilterData,            PxReal, mMinNormalisedLoad,			0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleTireLoadFilterData,            PxReal, mMinFilteredNormalisedLoad,	0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleTireLoadFilterData,            PxReal, mMaxNormalisedLoad,			0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleTireLoadFilterData,            PxReal, mMaxFilteredNormalisedLoad,	0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleTireLoadFilterData,            PxReal, mDenominator,				    0)	
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleTireLoadFilterData,PxU32,  mPad,						    PxMetaDataFlag::ePADDING)

	//PxVehicleWheelsSimData
	PX_DEF_BIN_METADATA_CLASS(PxVehicleWheelsSimData)	

	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheelsSimData, 	PxVehicleTireLoadFilterData,	mNormalisedLoadFilter,				0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheelsSimData, 	PxVehicleWheels4SimData,		mWheels4SimData,					PxMetaDataFlag::ePTR)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheelsSimData, 	PxU32, 							mNbWheels4,							0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheelsSimData, 	PxU32, 							mNbActiveWheels,					0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheelsSimData, 	PxU32, 							mActiveWheelsBitmapBuffer,			0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheelsSimData, 	PxReal, 						mThresholdLongitudinalSpeed,		0)
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheelsSimData, 	PxU32, 							mLowForwardSpeedSubStepCount,		0)
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheelsSimData, 	PxU32, 							mHighForwardSpeedSubStepCount,		0)
    PX_DEF_BIN_METADATA_ITEM(PxVehicleWheelsSimData, 	PxU32, 							mMinLongSlipDenominator,			0)

#ifdef PX_X64
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleWheelsSimData, PxU32,							mPad,					PxMetaDataFlag::ePADDING)
#endif

	PX_DEF_BIN_METADATA_EXTRA_ITEMS(PxVehicleWheelsSimData, PxVehicleWheels4SimData, mWheels4SimData, mNbWheels4, 0, 0)
}

void PxVehicleWheelsDynData::getBinaryMetaData(PxOutputStream& stream)
{
	PxVehicleWheels4DynData::getBinaryMetaData(stream);
	PxVehicleConstraintShader::getBinaryMetaData(stream);

	//PxVehicleTireForceCalculator
	PX_DEF_BIN_METADATA_CLASS(PxVehicleTireForceCalculator)

	PX_DEF_BIN_METADATA_ITEM(PxVehicleTireForceCalculator, 	void*,	mShaderData,	PxMetaDataFlag::ePTR)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleTireForceCalculator, 	PxU32,	mShader,		PxMetaDataFlag::ePTR)		

#ifndef PX_X64
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleTireForceCalculator,	PxU32,						mPad,			PxMetaDataFlag::ePADDING)
#endif

	//PxVehicleWheelsDynData
	PX_DEF_BIN_METADATA_CLASS(PxVehicleWheelsDynData)	

	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheelsDynData,             PxVehicleWheels4DynData,	   mWheels4DynData,		    PxMetaDataFlag::ePTR)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheelsDynData,             PxVehicleTireForceCalculator, mTireForceCalculators,	PxMetaDataFlag::ePTR)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheelsDynData,             PxU32,						   mUserDatas,				PxMetaDataFlag::ePTR)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheelsDynData,             PxU32,						   mNbWheels4,				0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheelsDynData,             PxU32,						   mNbActiveWheels,		    0)	
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleWheelsDynData, PxU32,			               mPad,					PxMetaDataFlag::ePADDING)

	PX_DEF_BIN_METADATA_EXTRA_ITEMS(PxVehicleWheelsDynData,	PxVehicleWheels4DynData,		mWheels4DynData,		mNbWheels4, 0, 0)
	PX_DEF_BIN_METADATA_EXTRA_ITEM(PxVehicleWheelsDynData,	PxVehicleTireForceCalculator,	mTireForceCalculators,	0)

	PX_DEF_BIN_METADATA_EXTRA_ITEMS(PxVehicleWheelsDynData, void, mTireForceCalculators, mNbWheels4, PxMetaDataFlag::ePTR, 0)
	PX_DEF_BIN_METADATA_EXTRA_ITEMS(PxVehicleWheelsDynData, void, mTireForceCalculators, mNbWheels4, PxMetaDataFlag::ePTR, 0)
	PX_DEF_BIN_METADATA_EXTRA_ITEMS(PxVehicleWheelsDynData, void, mTireForceCalculators, mNbWheels4, PxMetaDataFlag::ePTR, 0)
	PX_DEF_BIN_METADATA_EXTRA_ITEMS(PxVehicleWheelsDynData, void, mTireForceCalculators, mNbWheels4, PxMetaDataFlag::ePTR, 0)

	PX_DEF_BIN_METADATA_EXTRA_ITEMS(PxVehicleWheelsDynData, void, mUserDatas, mNbWheels4, PxMetaDataFlag::ePTR, 0)
	PX_DEF_BIN_METADATA_EXTRA_ITEMS(PxVehicleWheelsDynData, void, mUserDatas, mNbWheels4, PxMetaDataFlag::ePTR, 0)
	PX_DEF_BIN_METADATA_EXTRA_ITEMS(PxVehicleWheelsDynData, void, mUserDatas, mNbWheels4, PxMetaDataFlag::ePTR, 0)
	PX_DEF_BIN_METADATA_EXTRA_ITEMS(PxVehicleWheelsDynData, void, mUserDatas, mNbWheels4, PxMetaDataFlag::ePTR, 0)

	PX_DEF_BIN_METADATA_EXTRA_ITEMS(PxVehicleWheelsDynData, PxVehicleConstraintShader,	mWheels4DynData, mNbWheels4, 0, 0)
}

void PxVehicleWheels::exportExtraData(PxSerialStream& stream, void*)
{	
	PxU32 size = computeByteSize(mWheelsSimData.mNbWheels4);
	stream.align();
	stream.storeBuffer(mWheelsSimData.mWheels4SimData, size);
}

char* PxVehicleWheels::importExtraData(char* address, PxU32& totalPadding)
{
	address = PxAlignStream(address, totalPadding, PX_SERIAL_DEFAULT_ALIGN);
	address = (char*)patchupPointers(this, (PxU8*)address, mWheelsSimData.mNbWheels4, mWheelsSimData.mNbActiveWheels, false);
	return address;
}

void PxVehicleWheels::getBinaryMetaData(PxOutputStream& stream)
{
	PxVehicleWheelsSimData::getBinaryMetaData(stream);
	PxVehicleWheelsDynData::getBinaryMetaData(stream);

	//PxVehicleWheels
	PX_DEF_BIN_METADATA_VCLASS(PxVehicleWheels)
	PX_DEF_BIN_METADATA_BASE_CLASS(PxVehicleWheels, PxBase)	

	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheels, 		        PxVehicleWheelsSimData, mWheelsSimData,					0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheels, 		        PxVehicleWheelsDynData, mWheelsDynData,					0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheels, 		        PxRigidDynamic,			mActor,							PxMetaDataFlag::ePTR)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheels,		        PxU32,					mNbNonDrivenWheels,				0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheels,		        PxU8,					mOnConstraintReleaseCounter,	0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheels,		        PxU8,					mType,							0)	
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleWheels,	PxU8,					mPad,							PxMetaDataFlag::ePADDING)
}

void PxVehicleConstraintShader::getBinaryMetaData(PxOutputStream& stream)
{
	//SuspLimitConstraintData
	PX_DEF_BIN_METADATA_CLASS(SuspLimitConstraintData)
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(SuspLimitConstraintData, PxVec3,	mCMOffsets, 	0)	
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(SuspLimitConstraintData, PxVec3,	mDirs,			0)	
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(SuspLimitConstraintData, PxReal,	mErrors,		0)
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(SuspLimitConstraintData, bool,		mActiveFlags,	0)

	//StickyTireConstraintData
	PX_DEF_BIN_METADATA_CLASS(StickyTireConstraintData)
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(StickyTireConstraintData, PxVec3, 	mCMOffsets,		0)	
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(StickyTireConstraintData, PxVec3, 	mDirs,			0)	
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(StickyTireConstraintData, PxReal, 	mTargetSpeeds,	0)
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(StickyTireConstraintData, bool,		mActiveFlags,	0)

	//VehicleConstraintData
	PX_DEF_BIN_METADATA_CLASS(VehicleConstraintData)
	PX_DEF_BIN_METADATA_ITEM(VehicleConstraintData, SuspLimitConstraintData,	mSuspLimitData,  0)	
	PX_DEF_BIN_METADATA_ITEM(VehicleConstraintData, StickyTireConstraintData,	mStickyTireForwardData, 0)	
	PX_DEF_BIN_METADATA_ITEM(VehicleConstraintData, StickyTireConstraintData,	mStickyTireSideData, 0)	

	//PxVehicleConstraintShader
	PX_DEF_BIN_METADATA_VCLASS(PxVehicleConstraintShader)
	PX_DEF_BIN_METADATA_BASE_CLASS(PxVehicleConstraintShader, PxConstraintConnector)
	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleConstraintShader, 	        VehicleConstraintData,	mData,			0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleConstraintShader, 	        PxConstraint,			mConstraint,	PxMetaDataFlag::ePTR)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleConstraintShader, 	        PxVehicleWheels,		mVehicle,		PxMetaDataFlag::ePTR)	
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleConstraintShader,	PxU32,					mPad,			PxMetaDataFlag::ePADDING)
}

void PxVehicleWheels4SimData::getBinaryMetaData(PxOutputStream& stream)
{
	//PxVehicleSuspensionData
	PX_DEF_BIN_METADATA_CLASS(PxVehicleSuspensionData)
	PX_DEF_BIN_METADATA_ITEM(PxVehicleSuspensionData,		        PxReal, mSpringStrength,		0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleSuspensionData,		        PxReal, mSpringDamperRate,		0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleSuspensionData,		        PxReal, mMaxCompression,		0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleSuspensionData,		        PxReal, mMaxDroop,				0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleSuspensionData,		        PxReal, mSprungMass,			0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleSuspensionData,		        PxReal, mCamberAtRest,		    0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleSuspensionData,		        PxReal, mCamberAtMaxCompression,0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleSuspensionData,		        PxReal, mCamberAtMaxDroop,		0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleSuspensionData,		        PxReal, mRecipMaxCompression,	0)
	PX_DEF_BIN_METADATA_ITEM(PxVehicleSuspensionData,		        PxReal, mRecipMaxDroop,			0)
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleSuspensionData,	PxU32,	mPad,					PxMetaDataFlag::ePADDING)	

	//PxVehicleWheelData
	PX_DEF_BIN_METADATA_CLASS(PxVehicleWheelData)
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheelData,		            PxReal,		mRadius,				0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheelData,		            PxReal, 	mWidth,					0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheelData,		            PxReal, 	mMass,					0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheelData,		            PxReal, 	mMOI,					0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheelData,		            PxReal, 	mDampingRate,			0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheelData,		            PxReal, 	mMaxBrakeTorque,		0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheelData,		            PxReal, 	mMaxHandBrakeTorque,	0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheelData,		            PxReal, 	mMaxSteer,				0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheelData,		            PxReal, 	mToeAngle,				0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheelData,		            PxReal, 	mRecipRadius,			0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheelData,		            PxReal, 	mRecipMOI,				0)	
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleWheelData,		PxReal, 	mPad,					PxMetaDataFlag::ePADDING)	

	//PxVehicleTireData
	PX_DEF_BIN_METADATA_CLASS(PxVehicleTireData)
	PX_DEF_BIN_METADATA_ITEM(PxVehicleTireData,		                PxReal, 	mLatStiffX,						            0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleTireData, 		            PxReal, 	mLatStiffY,									0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleTireData, 		            PxReal, 	mLongitudinalStiffnessPerUnitGravity,		0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleTireData, 		            PxReal, 	mCamberStiffnessPerUnitGravity,				0)	
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleTireData,		    PxReal, 	mFrictionVsSlipGraph,						0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleTireData, 		            PxU32,		mType,										0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleTireData, 		            PxReal, 	mRecipLongitudinalStiffnessPerUnitGravity,	0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleTireData, 		            PxReal, 	mFrictionVsSlipGraphRecipx1Minusx0, 		0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleTireData, 		            PxReal, 	mFrictionVsSlipGraphRecipx2Minusx1, 		0)	
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleTireData,		    PxReal, 	mPad,										PxMetaDataFlag::ePADDING)	

	//PxVehicleWheels4SimData
	PX_DEF_BIN_METADATA_CLASS(PxVehicleWheels4SimData)
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleWheels4SimData, 	PxVehicleSuspensionData,	mSuspensions,					0)	
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleWheels4SimData, 	PxVehicleWheelData,			mWheels,						0)	
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleWheels4SimData, 	PxVehicleTireData,			mTires,							0)	
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleWheels4SimData, 	PxVec3, 					mSuspDownwardTravelDirections,	0)	
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleWheels4SimData, 	PxVec3, 					mSuspForceAppPointOffsets,		0)	
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleWheels4SimData, 	PxVec3, 					mTireForceAppPointOffsets,		0)	
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleWheels4SimData, 	PxVec3, 					mWheelCentreOffsets,			0)	
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleWheels4SimData, 	PxReal, 					mTireRestLoads,					0)
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleWheels4SimData, 	PxReal, 					mRecipTireRestLoads,			0)
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleWheels4SimData, 	PxFilterData, 				mSqFilterData,					0)
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleWheels4SimData, 	PxU8, 						mWheelShapeMap,					0)
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleWheels4SimData, 	PxU32, 						mPad,							PxMetaDataFlag::ePADDING)
}

void PxVehicleWheels4DynData::getBinaryMetaData(PxOutputStream& stream)
{
	PX_DEF_BIN_METADATA_CLASS(PxVehicleWheels4DynData)
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleWheels4DynData,		PxReal,						mWheelSpeeds,				0)	
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleWheels4DynData, 		PxReal, 					mCorrectedWheelSpeeds,		0)
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleWheels4DynData, 		PxReal, 					mWheelRotationAngles,		0)	
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleWheels4DynData,		PxReal, 					mTireLowForwardSpeedTimers, 0)
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleWheels4DynData,		PxReal, 					mTireLowSideSpeedTimers,	0)
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleWheels4DynData, 		PxU8, 						mRaycastsOrCachedHitResults,0)	
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheels4DynData,			        PxVehicleConstraintShader,	mVehicleConstraints,		PxMetaDataFlag::ePTR)
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheels4DynData,			        PxRaycastQueryResult,		mSqResults,					PxMetaDataFlag::ePTR)
	PX_DEF_BIN_METADATA_ITEM(PxVehicleWheels4DynData,			        bool,						mHasCachedRaycastHitPlane,	0)
	PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(PxVehicleWheels4DynData,		PxU32,						mPad,						PxMetaDataFlag::ePADDING)
}
