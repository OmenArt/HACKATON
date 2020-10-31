#pragma once
#define _USE_MATH_DEFINES

#include <windows.h>
#include <math.h>
#include <memory.h>
#include <complex>
#include "DirectXComputeShaderManagementClass.h"
#include "Direct2DManagementClass.h"
#include "MatrixObjectBuffer.h"
#include <list>
#include <vector>
#include <algorithm>
#include <amp.h>
#include <random>
#include <time.h>
#include <iostream>
#include <functional>
#include "ImageDerivativeClass.h"
#include "ImageFullDerivativesClass.h"

#include "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\D3DCSX.h"

using namespace std;
using namespace concurrency;

const UINT NUM_ELEMENTS = 1024;
const UINT BITONIC_BLOCK_SIZE = 512;
const UINT TRANSPOSE_BLOCK_SIZE = 16;
const UINT MATRIX_WIDTH = BITONIC_BLOCK_SIZE;
const UINT MATRIX_HEIGHT = NUM_ELEMENTS / BITONIC_BLOCK_SIZE;
#define USE_STRUCTURED_BUFFERS

// If defined, then the hardware/driver must report support for double-precision CS 5.0 shaders or the sample fails to run
//#define TEST_DOUBLE

// The number of elements in a buffer to be tested


#if defined(_MSC_VER) && (_MSC_VER<1610) && !defined(_In_reads_)
#define _Outptr_
#define _Outptr_opt_ 
#define _In_reads_(exp)
#define _In_reads_opt_(exp)
#define _Out_writes_(exp)
#endif

#ifndef _Use_decl_annotations_
#define _Use_decl_annotations_
#endif

#define MAX_MATRIX_SIZE 16;




struct MatrixVariableOutputType
{
	int pXPosition;
	int pYPosition;
	float pMatrixCurrentValue;
	float pMatrixTotal;
};


struct PixelObjectGrayScale
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float pGreyLevel;

	PixelObjectGrayScale(int pPixelNumber, float pXVal, float pYVal, float pGreyVal)
	{
		pixelNumber = pPixelNumber;
		xPosition = pXVal;
		yPosition = pYVal;
		pGreyLevel = pGreyVal;
	}
};


struct PixelObjectGrayScaleInt
{
	int pixelNumber;
	int xPosition;
	int yPosition;
	int pGreyLevel;

	PixelObjectGrayScaleInt(int pPixelNumber, int pXVal, int pYVal, int pGreyVal)
	{
		pixelNumber = pPixelNumber;
		xPosition = pXVal;
		yPosition = pYVal;
		pGreyLevel = pGreyVal;
	}
};


struct InputArrayStruct
{
	float pItemIndex;
	float pInputNumberVar;
};



struct InputItemType
{
	int pItemId;
	float pItemValue;
};



struct CStandardDeviationOutputType
{
	int pItemIndex;
	float pInputValue;
	float pOutputValue;
};





struct OutputArrayStruct
{
	float pItemIndex;
	float pInputNumberVar;
	int kindOfNumber;
};


struct OutputArrayFinalStructOddEven
{
	float pItemIndex;
	float pInputNumberVar;
	int kindOfNumber;
	int pCurrentState;
	bool didStateChange;
};


struct CBStandardDeviation
{
	float pAverageValue;
};



struct BData
{
	float pData;
	int pDataIntValue;
	double pDataDoubleValue;
};


struct BData0
{
	int pData;
};

struct InputStructFloat
{
	float pData;
};

struct BDataComplexNumber
{
	_complex pData;
};


struct AmountOfItemsStruct
{
	UINT pArraySize;
};


struct CBBinaryImageConstantBuffer
{
	float pThreshHoldVar;
};


struct BorderOutputStructure
{

	int pixelNumber;
	float xPosition0;
	float yPosition0;
	float xPosition1;
	float yPosition1;
	float pGrayScaleValue0;
	float pGrayScaleValue1;
	float pOutputPixelValue;
	bool pIsBorderValue;
};

struct ConstantBuffer
{
	UINT iLevel;
	UINT iLevelMask;
	UINT iWidth;
	UINT iHeight;
};


struct CBParallelReduction
{
	UINT pAmountOfElements;
	UINT pDispathcThreadX;
};




struct OutputPixelObjectColorDistanceAndAngleFilter
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float rValue;
	float gValue;
	float bValue;
	float pDistanceToPoint;
	float pAngleTangent;
	float pAngleSine;
	float pAngleCosine;
	float pAngleToPixelData;
	bool pIsColorValid;
	bool pIsDistanceValid;
	bool pIsAngleValid;
	bool pIsPixelValid;

};




struct GasDynamicsConstantBuffer
{
	float pGasMass;
	float pRValue;
	float pGammaVar;
};

struct TemperatureStructure
{
	float pTemperatureValue;
};

struct SpeedValue
{
	float pSpeedValue;
};


struct ConstantBufferQuadratic
{
	float pACoefficient;
	float pBCoefficient;
	float pCoefficient;
};

struct CBCosineFunctionBuffer
{
	unsigned int pCosineCoefficient;
	unsigned int pEmptyCoefficient;
};


struct ConstantBufferExponential
{
	unsigned int pCoefficientParam;
	unsigned int pFunctionFreeParameter;
};


struct ComplexNumberNumber
{
	ComplexNumberNumber() :x(0), y(0){}
	ComplexNumberNumber(double r) :x(r), y(0){}
	float x, y;
};

struct CBFastFourierConstantBuffer
{
	float pAmountOfElements;
	float pi_val;
	std::complex<double> imaginayOne;
};

struct BufTypeP
{
	int pIntDataBuffer;
	float pFloatDataBuffer;
	double pDoubleDataBuffer;
};


struct MatrixMultiplyCB
{

	UINT pWidthA;
	UINT pHeightA;
	UINT pWidthB;
	UINT pHeigtB;
	UINT pWidthOut;
	UINT pHeightOut;

};


struct MatrixDeterminantCB
{

	UINT pWidthA;
	UINT pHeightA;
	UINT pWidthOut;
	UINT pHeightOut;

};



//Matrix Data Structure
struct MatrixVariableType
{
	BufType pMatrixValue[16];
};

struct MatrixMultiplyConstantBuffer
{
	int pMatrixSize;
};

struct CBGaussianBlurConstantBuffer
{

	float pi_var;
	float pStandardDeviationVar;
};


struct OddNumberCB
{
	int OddNumber;
	int EvenNumber;
	int Undefined;
	float pDivisorValue;
	float pKindOfNumberOdd;
	float pKindOfNumberEven;
	float pKindOfNumberUnknown;
};


struct InputArrayOddEvenStruct
{
	float pItemIndex;
	float pInputNumberVar;
};


struct OutputArrayOddEventStruct
{
	float pItemIndex;
	float pInputNumberVar;
	float kindOfNumber;
};

struct OutputArrayOddEvenFinalStruct
{
	float pItemIndex;
	float pInputNumberVar;
	int kindOfNumber;
	int pCurrentState;
	int pPreviousState;
	bool didStateChange;
};

struct ImageKernelItem
{
	float pKernelItemId;
	float xPosition;
	float yPosition;
	float pIntensityValue;

	ImageKernelItem(float pItemId,
		float pXPosition,
		float pYPosition,
		float pIntensityVal)
	{
		pKernelItemId = pItemId;
		xPosition = pXPosition;
		yPosition = pYPosition;
		pIntensityValue = pIntensityVal;
	}

};



struct CbConvolultionBuffer
{
	float pImageWidth;
	float pImageHeight;
	float pFilterWidth;
	float pFilterHeight;
	float pOutputWidth;
	float pOutputHeight;
	float pFilterArrayVal[3];
};

struct CStandardDeviationImageOutputType
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float pStandardDeviationVar;
};

struct CbKernelConvolution
{
	float pImageWidth;
	float pImageHeight;
	float pKernelGate[6];
};


struct CannyOutputVar
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float pGaussianFilterValue;
	float pGaussianValueF;
	float pCannyEdgeDetectorX;
	float pCannyEdgeDetectorY;
	float pCannyMagnitude;
	float pCannyDirection;
};


struct GaussianOutputVar
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float pIntensityValue;
	float pGradientValue;
	float pGaussianValueF;
};

struct LaplaciantGaussianOutputVar
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float pIntensityValue;
	float pGradientVarTempF;
	float pOutputFunctionF;
};




struct InputSalesUnit
{
	int pResourceId;
	float pResourceMass;
	float pInitialVolume;
	float pTimeValue;

public:
	InputSalesUnit(int resourceId,
		float resourceMass,
		float initialVolume,
		float pTimeVal)
	{
		pResourceId = resourceId;
		pResourceMass = resourceMass;
		pInitialVolume = initialVolume;
		pTimeValue = pTimeVal;
	}
};


struct OutputSalesUnit
{
	int pResourceId;
	float pInitialVolume;
	float pTimeValue;
	float pCurrentVolumeOfSales;
	float pScaleUnitValue;
	float pMomentOfGrowth;
	float pWeightGrowthMoment;
	float pAccelerationVolume;
	float pFluctuationOfResource;
	float pSumOfFluctuations;

public:
	OutputSalesUnit(int resourceid,
		float initalVolume,
		float timeValue,
		float currentVolumeOfSales,
		float scaleUnitValue,
		float momentOfGrowth,
		float weightGrowthMoment,
		float acceleratedVolume,
		float fluctuationOfResources,
		float sumOfFluctuations)
	{
		pResourceId = resourceid;
		pInitialVolume = initalVolume;
		pCurrentVolumeOfSales = currentVolumeOfSales;
		pScaleUnitValue = scaleUnitValue;
		pMomentOfGrowth = momentOfGrowth;
		pWeightGrowthMoment = weightGrowthMoment;
		pAccelerationVolume = acceleratedVolume;
		pFluctuationOfResource = fluctuationOfResources;
		pSumOfFluctuations = sumOfFluctuations;
	}
};




struct CbImageComparisonConstants
{
	float pImageWidth;
	float pImageHeight;
};



struct CImageDerivativeClassStruct
{
public:
	UINT pDerivativeIndex;
	float pXPosition;
	float pYPosition;
	float pDerivativeValue;

	CImageDerivativeClassStruct(UINT derivativeIndex,
		float xPosition,
		float yPosition,
		float derivativeValue)
	{
		pDerivativeIndex = derivativeIndex;
		pXPosition = xPosition;
		pYPosition = yPosition;
		pDerivativeValue = derivativeValue;
	}
	~CImageDerivativeClassStruct()
	{
		pDerivativeIndex = 0;
		pXPosition = 0.0f;
		pYPosition = 0.0f;
		pDerivativeValue = 0.0f;
	}
};

struct CImageFullDerivativeClassStruct
{
public:
	UINT pDerivativeIndex;
	float pXPosition;
	float pYPosition;
	float xPosition;
	float yPosition;
	float pDerivativeValueX;
	float pDerivativeValueY;
	float pMagnitude;
	float pDirection;

	CImageFullDerivativeClassStruct(UINT derivativeIndex,
		float xPositionVal,
		float yPositionVal,
		float derivativeValueX,
		float derivativeValueY,
		float pMagnitudeVal,
		float pDirectionVal)
	{
		pDerivativeIndex = derivativeIndex;
		pXPosition = xPositionVal;
		pYPosition = yPositionVal;
		pDerivativeValueX = derivativeValueX;
		pDerivativeValueY = derivativeValueY;
		pMagnitude = pMagnitudeVal;
		pDirection = pDirectionVal;

	}
	~CImageFullDerivativeClassStruct()
	{
		pDerivativeIndex = 0;
		pXPosition = 0.0f;
		pYPosition = 0.0f;
		pMagnitude = 0.0f;
		pDirection = 0.0f;
	}
};

struct DivisionOutCome
{
	BufType pDivident;
	BufType pOutcome;
};

struct DivisionRemainder
{
	BufType pDivident;
	BufType pOutcome;
};

struct CBImageParameters
{
	float pXPosition0;
	float pYPosition0;
	float pImageWidth;
	float pImageHeight;
};



struct OutputPixelBuffer
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float grayScaleValue0;
	float grayScaleValue1;
	float outputComparisonValue;
	bool arePixelsIdenticalVal;
	float pAmountOfIdenticalPixels;
	float pAmountOfDifferentPixels;

	OutputPixelBuffer(int pPixelNumber,
		float pXPosition,
		float pYPosition,
		float pGrayScaleValue0,
		float pGrayScaleValue1,
		float pOutputComparisomValue,
		bool pArePixelIdentical,
		float amountOfIdenticalPixels,
		float amountOfDifferentPixels)

	{
		pixelNumber = pPixelNumber;
		xPosition = pXPosition;
		yPosition = pYPosition;
		grayScaleValue0 = pGrayScaleValue0;
		grayScaleValue1 = pGrayScaleValue1;
		outputComparisonValue = pOutputComparisomValue;
		arePixelsIdenticalVal = pArePixelIdentical;
		pAmountOfIdenticalPixels = amountOfIdenticalPixels;
		pAmountOfDifferentPixels = amountOfDifferentPixels;
	}

};


struct MotionPixelObjectRGB
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float rValue0;
	float gValue0;
	float bValue0;
	float rValue1;
	float gValue1;
	float bValue1;
	float pRedComponentDifference;
	float pGreenComponentDifference;
	float pBlueComponentDifference;
	float pAmountOfIdenticalPixels;
	float pAmountOfDifferentPixels;
	bool pOverallMotionDetected;

	MotionPixelObjectRGB(int pPixelNumberVar,
		float xPositionVar,
		float yPositionVar,
		float pRedComponentVar0,
		float pGreenComponentVar0,
		float pBlueComponentVar0,
		float pRedComponentVar1,
		float pGreenComponentVar1,
		float pBlueComponentVar1,
		float redComponentDifferenceVar,
		float greenComponentDifferenceVar,
		float blueComponentDifferenceVar,
		float amountOfIdenticalPixels,
		float amountOfDifferentPixels,
		bool overallMotionDetectedVar)
	{
		pixelNumber = pPixelNumberVar;
		xPosition = xPositionVar;
		yPosition = yPositionVar;
		rValue0 = pRedComponentVar0;
		gValue0 = pGreenComponentVar0,
			bValue0 = pBlueComponentVar0;
		rValue1 = pRedComponentVar1;
		gValue1 = pGreenComponentVar1,
			bValue1 = pBlueComponentVar1;
		pRedComponentDifference = redComponentDifferenceVar;
		pGreenComponentDifference = greenComponentDifferenceVar;
		pBlueComponentDifference = blueComponentDifferenceVar;
		pAmountOfIdenticalPixels = amountOfIdenticalPixels;
		pAmountOfDifferentPixels = amountOfDifferentPixels;
		pOverallMotionDetected = overallMotionDetectedVar;
	}


};

struct PixelObjectWithALabel
{
	int pixelNumber;
	float pXPosition;
	float pYPosition;
	float rValue;
	float gValue;
	float bValue;
	int pClusterLabel;


};


struct OutputPixelObjectGrayScaleWithState
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float pGrayScaleValue;
	int pLevelOfDarkness;
	int pCurrentState;
};
struct OutputPixelObjectGrayScaleWithStateChange
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float pGrayScaleValueOutput;
	int pLevelOfDarkness;
	int pPreviousLevelOfDarkness;
	int pCurrentState;
	int pPreviousState;
	bool pDidStateChange;
};


struct OutputPixelBufferStateMachineData
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float pGrayScaleValue0;
	float pGrayScaleValue1;
	float pOutputComparisonValue;
	bool arePixelsIdenticalVal;
	int pImageLabel;
	int pCurrentState;
};

struct OutputPixelBufferStateChangeMachineData
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float pGrayScaleValue0;
	float pGrayScaleValue1;
	float pOutputComparisonValue;
	bool arePixelsIdenticalVal;
	int pImageLabel;
	int pCurrentState;
	int pPreviousState;
	bool pDidStateChange;
};



struct OutputPixelObjectGrayScaleStateChange
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float pOutputComparisonValueOne;
	float pOutputComparisonValueTwo;
	float pOutputComparisonValueFinal;
	int pCurrentStateOne;
	int pCurrentStateTwo;
	bool pDidStateChange;
};


struct cbImageProcessingIntensityConstantBuffer
{
	int pIdenticalImageLabel;
	int pDifferentImageLabel;
};



struct cbImageLabelConstants
{
	int pRedCluster;
	int pGreenCluster;
	int pBlueCluster;
	int pBlackCluster;
	int pWhiteCluster;
};


struct CBImageIntensityConstants
{
	float pWhiteVar;
	float pExtraLit;
	float pLitVar;
	float pSemyLit;
	float pBarelyLit;
	float pDarkVar;
	float pExtremlyDark;
	float pCloseToBlack;
	float pBlackColor;
	float pUndefinedColoer;
	int pWhiteState;
	int pExtraListState;
	int pLitVarState;
	int pSemyLitState;
	int pBarelyLitState;
	int pDarkVarState;
	int pExtremlyDarkState;
	int pCloseToBlackState;
	int pBlackColorState;
	int pUndefinedState;
};

struct cbImageProcessingFSMStageTwo
{
	int pIdenticalImageLabel;
	int pDifferentImageLabel;
	int pUndefinedImageLabel;
	int pUndefinedStateLabel;
	int pIdenticalStateLabel;
	int pDifferentStateLabel;
};


struct InputStructureForGraph //Input Structure
{
	int pItemIndex;
	float pInputItemValue;
};
struct OutputStructureForGraph //Output Structure
{
	int pItemIndex;
	float pInputStructureForGraph;
	float pOutputStructureForGraph;
};

struct ConstantBufferCosine
{
	unsigned int pCoefficientParam;
	unsigned int pFunctionFreeParameter;
	unsigned int pi_var;
};


struct cbFinancialSpaceUnit
{
	float pAverageActivityVal;
	float pXValue;
};


struct ResourceObject
{
	float pTimeVal;
	float pResourceId;
	float pMassOfKindValue;
	float pCommonPotential;
	float pGrowthCoefficient;
	float pMassIntroductoryValue;
	float pMassActivityValue;
	float pActivityCoefficient;
	float pDisposalCoefficient;
	float pMediumGrowthLevel;
	float pPreviousPotentialValue;

};


struct OutputResourceObject
{

	float pResourceId;
	float pMassOfKindValue;
	float pCommonPotential;
	float pGrowthCoefficient;
	float pMassIntroductoryValue;
	float pMassActivityValue;
	float pActivityCoefficient;
	float pDisposalCoefficient;
	float pMediumGrowthLevel;
	float pOutputValue;
	float pRegulationParameter;
	float pResourceValueParameter;
	float pQualityCoefficient;


};


struct FinancialSpaceCB
{
	UINT pAverageValue;
	UINT pXValue;
};




struct DateDataType
{
	float pSecondValue;
	float pMinuteValue;
	float pHourValue;
	float pDayValue;
	float pWeekValue;
	float pMonthValue;
	float pYearValue;
};



struct cbCurrencyMetalBuffer
{
	DateDataType pSelectedDateValue;
	float pSelectedMetalId;
	float pSelectedCurrencyId;
};

struct cbCurrencyCurrencyBuffer
{
	DateDataType pSelectedDateValue;
	float pCurrencyId1;
	float pCurrencyId2;
	float pAmountOfInvestment;
};





struct MetalObject
{

	float pMetalIdValue;
	float pYear;
	float pMonth;
	float pDay;
	float pHour;
	float pMinute;
	float pSecond;
	float pMetalPriceOpenValue;
	float pMetalPriceHighValue;
	float pMetalPriceLowValue;
	float pMetalPriceCloseValue;

};

struct CurrencyObject
{
	float pCurrencyIdValue;
	float pYear;
	float pMonth;
	float pDay;
	float pHour;
	float pMinute;
	float pSecond;
	float pCurrencyPriceOpenValue;
	float pCurrencyPriceHighValue;
	float pCurrencyPriceLowValue;
	float pCurrencyPriceCloseValue;
	float pAmountOfInvestment;

public:
	CurrencyObject(float currId,
		float yearVal,
		float pMonVal,
		float pDayVal,
		float pHourVal,
		float pMinuteVal,
		float pSecondVal,
		float priceOpenVal,
		float priceHighVal,
		float priceLowVal,
		float priceCloseVal,
		float amountOfInvestment)
	{
		pCurrencyIdValue = currId;
		pYear = yearVal;
		pMonth = pMonVal;
		pDay = pDayVal;
		pHour = pHourVal;
		pMinute = pMinuteVal;
		pSecond = pSecondVal;
		pCurrencyPriceOpenValue = priceOpenVal;
		pCurrencyPriceHighValue = priceHighVal;
		pCurrencyPriceLowValue = priceLowVal;
		pCurrencyPriceCloseValue = priceCloseVal;
		pAmountOfInvestment = amountOfInvestment;
	}
};

struct CurrencyMetalPairObject
{
	float pMetalId;
	float pCurrencyId;
	float pYear;
	float pMonth;
	float pDay;
	float pHour;
	float pMinute;
	float pSecond;
	float pAmountOfInvestmentVal;
	float pCurrencyPriceOpenValue;
	float pCurrencyPriceHighValue;
	float pCurrencyPriceLowValue;
	float pCurrencyPriceCloseValue;
	float pMetalPriceOpenValue;
	float pMetalPriceHighValue;
	float pMetalPriceLowValue;
	float pMetalPriceCloseValue;
	float pRationInPercentageOpen;
	float pRationInPercentageHigh;
	float pRationInPercentageLow;
	float pRationInPercentageClose;

};




struct CurrencyCurrencyPairObject
{
	float pCurrencyId0;
	float pCurrencyId1;
	float pYear;
	float pMonth;
	float pDay;
	float pHour;
	float pMinute;
	float pSecond;
	float pAmountOfInvestmentVal;
	float pCurrencyPriceOpenValue0;
	float pCurrencyPriceHighValue0;
	float pCurrencyPriceLowValue0;
	float pCurrencyPriceCloseValue0;
	float pCurrencyPriceOpenValue1;
	float pCurrencyPriceHighValue1;
	float pCurrencyPriceLowValue1;
	float pCurrencyPriceCloseValue1;
	float pRationInPercentageOpen;
	float pRationInPercentageHigh;
	float pRationInPercentageLow;
	float pRationInPercentageClose;
	float pPotentialProfitOpen;
	float pPotentialProfitHigh;
	float pPotentialProfitLow;
	float pPotentialProfitClose;

};




struct cbConstantBufferAxctivManagement
{
	float fVar;
	float mVar;
	bool doNeedConstantControl;
	float pMaximumDifference;
	float pLimit;
	bool pDoNeedStability;
};

struct cbSubtractionBuffer
{
	float pItemToSubtract;
};



struct ObjectClassInActiv
{
	float pObjectKindId;
	float pIndesValue;
	float pTimeValue;
	float pCommonPotentialValAgainstTime;
	float pInputKMathGrowth;
	float pGrowthCoefficient;
	float pPotentialMassGrowthIn;
	float pMassInflowIn;
	float pPotentialMassGrowth;
	float pActivityCoefficient;
	float pMassOutflowOut;
	float pKValue;
	float pPreviousK;
	float pLTimeValue;
	float pAverageDynamicGrowthOut;
	bool pContainableGrowthCondition;
	bool pDataVaryStrongly;
};


struct ObjectClassOutActiv
{
	int pObjectKindId;
	float pTimeValue;
	float pMassOutflowOut;
	float pMainEquationResult;
	float pRegulatoryFunctionValue;
	float pAOne;
	float pATwo;
	float pXOne;
	float pXTwo;
	float pMaximizationResult;
	float pMinimizationResult;

};


struct OutputPixelObjectFiltered
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float rValue;
	float gValue;
	float bValue;
	bool pUsedInComputations;
};



struct OutputPixelObjectFilteredGrayScale
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float pGrayScaleValue;
	bool pIsPixelValid;
};


struct OutputPixelObjectFilteredGrayScaleKMeans
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float pGrayScaleValue;
	bool pIsPixelValid;
};

struct OutputPixelObjectFilteredRGBKMeans
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float rValue;
	float gValue;
	float bValue;
	bool pIsPixelValid;
};



struct csSubtractionBuffer
{
	float pColorToSubtractRed;
	float pColorToSubtractGreen;
	float pColorToSubtractBlue;
};



struct KMeansConstantBuffer
{
	float pCenteObjectX;
	float pCentreObjectY;
	float pRadiusValue;
};


struct OutputPixelObjectCorrespondence
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float rValue;
	float gValue;
	float bValue;
	bool pDoesPixelCorrespondWithPoint;


};

struct CBufferPointCorrespondense
{
	float xPositionVar0;
	float yPositionVar0;
	float xPositionVar1;
	float yPositionVar1;
	float xPositionVar2;
	float yPositionVar2;
	float xPositionVar3;
	float yPositionVar3;
};

float pDifferentArraySum;
float pDifferentArrayAverage;

struct YCBRPixelDataBuffer
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float pYValue;
	float pCbValue;
	float pCrValue;


public:
	YCBRPixelDataBuffer(int pPixelNumberVar,
		float pXPositionVar,
		float pYPositionVar,
		float pYValueVar,
		float pCBValueVar,
		float pCRValueVar)
	{
		pixelNumber = pPixelNumberVar;
		xPosition = pXPositionVar;
		yPosition = pYPositionVar;
		pYValue = pYValueVar;
		pCbValue = pCBValueVar;
		pCrValue = pCRValueVar;
	}

};


struct PixelObjectLinkedListStruct
{
	int pLinkedListStructureId;
	int pPixelNumberOne;
	int pPixelNumberTwo;
	float xPositionVar0;
	float yPositionVar0;
	float xPositionVar1;
	float yPositionVar1;
	float pRedComponentVar0;
	float pGreenComponentVar0;
	float pBlueComponentVar0;
	float pRedComponentVar1;
	float pGreenComponentVar1;
	float pBlueComponentVar1;
	float pRedComponentDifferenceVar;
	float pGreenComponentDifferenceVar;
	float pBlueComponentDifferenceVar;
	bool pIsObjectFlat;
public:
	PixelObjectLinkedListStruct(int linkedListItemId,
		int pixelNumberOne,
		int pPileNumberTwo,
		float xPosition0,
		float yPosition0,
		float xPosition1,
		float yPosition1,
		float pRValue0,
		float pGValue0,
		float pBValue0,
		float pRValue1,
		float pGValue1,
		float pBValue1,
		float pRValueDifference,
		float pGValueDifference,
		float pBValueDifference,
		bool pIsFlat)
	{
		pLinkedListStructureId = linkedListItemId;
		pPixelNumberOne = pixelNumberOne;
		pPixelNumberTwo = pPixelNumberTwo;
		xPositionVar0 = xPosition0;
		yPositionVar0 = yPosition0;
		xPositionVar1 = xPosition1;
		yPositionVar1 = yPosition1;
		pRedComponentVar0 = pRValue0;
		pGreenComponentVar0 = pGValue0;
		pBlueComponentVar0 = pBValue0;
		pRedComponentVar1 = pRValue1;
		pGreenComponentVar1 = pGValue1;
		pBlueComponentVar1 = pBValue1;
		pRedComponentDifferenceVar = pRValueDifference;
		pGreenComponentDifferenceVar = pGValueDifference;
		pBlueComponentDifferenceVar = pBValueDifference;
		pIsObjectFlat = pIsFlat;


	}
};



struct PixelObjectLinkedListElement
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float rValue;
	float gValue;
	float bValue;
	bool isFirst;
	bool isLast;
	int pNextPixelNumber;
};


struct cbConstantBufferYCbr
{
	float pEqOneConstantOne;
	float pEqOneConstantTwo;
	float pEqOneConstantThree;
	float pEqTwoConstantOne;
	float pEqTwoConstantTwo;
	float pEqTwoConstantThree;
	float pEqThreeConstantOne;
	float pEqThreeConstantTwo;
	float pEqThreeConstantThree;
};

struct cbConstantBufferYCbrToRGB
{
	float pEqOneConstantOne;
	float pEqOneConstantTwo;
	float pEqTwoConstantOne;
	float pEqTwoConstantTwo;
	float pEqTwoConstantThree;
	float pEqThreeConstantOne;
	float pEqThreeConstantTwo;
};

struct QuantizationItem
{
	int pPixelNumber;
	float pXPosition;
	float pYPosition;
	float pQuontizationValue;

};

struct OutputPixelObjectFilteredYCBCR
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float pYValue;
	float pCbValue;
	float pCrValue;
	bool pIsPixelValid;
};



struct cbCostantBufferCosine
{
	float pi_var;
	float pImageWidthVar;
	float pImageHeightVar;
};

struct CSConstantBufferPixelNumberStruct
{
	float pPixelNumberFactor;
};

struct OutputPixelObjectInterestPoint
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float rValue;
	float gValue;
	float bValue;
	float pRemainderVar;
	bool pIsPixelValid;
};

struct YCBRPixelDataBufferOutputCosine
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float pYValue;
	float pCbValue;
	float pCrValue;
	float pYCosineValue;
	float pCBCosineValue;
	float pCRCosineValue;
};


using namespace std;


struct cbPixelValidationBuffer
{
	float pRedComponentConst;
	float pGreenComponentConst;
	float pBlueComponentConst;

};

struct CBThreshHoldFontKillerVarStruct
{

		float pStandardRed0;
		float pStandardGreen0;
		float pStandardBlue0;
		float pStandardRed1;
		float pStandardGreen1;
		float pStandardBlue1;
	

};


struct CBThreshHoldFontSelectoVarStruct
{
	float pRedValueToSelect;
	float pGreenValueToSelect;
	float pBlueValueToSelect;
};

struct cbCurrencyStateMachineBuffer
{
	int StableStateVar;
	int GrowthStateVar;
	int DeclineStateVar;
	int UndefinedStateVar;
	int VolatileState;
};

struct CurrencyObjectRate
{
	int pCurrencyId;
	float pOpeningPrice;
	float pMorningPrice;
	float pAfternoonPrice;
	float pEveningPrice;

public:
	CurrencyObjectRate(int pIdVar,
		float pOpeningPriceVar,
		float pMorningPriceVar,
		float pAfternoonPriceVar,
		float pEveningPriceVar)
	{
		pCurrencyId = pIdVar;
		pOpeningPrice = pOpeningPriceVar;
		pMorningPrice = pMorningPriceVar;
		pAfternoonPrice = pAfternoonPriceVar;
		pEveningPrice = pEveningPriceVar;
	}


};


struct CBBinaryClassifierConstantBuffer
{
	float pThresholdIntensityValueR;
	float pThresholdIntensityValueG;
	float pThresholdIntensityValueB;
	int pValidPixelConstant;
	int pInvalidPixelConstant;
};


struct CBBinaryClassifierConstantBufferWithCoordinates
{
	float pXLimit0;
	float pYLimit0;
	float pThresholdIntensityR;
	float pThresoldIntensityG;
	float pThresoldIntensityB;
	int pValidPixelConstant;
	int pInvalidPixelConstant;
};



struct OutputPixelObjectClassified
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float rValue;
	float gValue;
	float bValue;
	int pClassOfPixelRed;
	int pClassOfPixelGreen;
	int pClassOfPixelBlue;
	int pClassOfPixelOutput;
};


struct CurrencyRateStateMachineObject
{
	int pCurrencyId;
	float pOpeningPrice;
	float pPriceInRoublesMorning;
	float pPriceInRoublesAfternoon;
	float pPriceInRoublesEvening;
	int pCurrencyStateMorning;
	int pCurrencyStateAfternoon;
	int pCurrencyStateEvening;
	bool pMorningStateChange;
	bool pAfternoonStateChange;
	bool pEveningStateChange;
};


struct OutputPixelBufferRGB
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float pRedValue0;
	float pGreenValue0;
	float pBlueValue0;
	float pRedValue1;
	float pGreenValue1;
	float pBlueValue1;
	float pOutputComparisonValueRed;
	float pOutputComparisonValueGreen;
	float pOutputComparisonValueBlue;
	bool arePixelsIdenticalVal;
	float pAmountOfIdenticalPixels;
	float pAmountOfDifferentPixels;

};

struct PixelBundle
{
	int pBundleId;
	float xPosition;
	float yPosition;
	float pRedValue0;
	float pGreenValue0;
	float pBlueValue0;
	float pRedValue1;
	float pGreenValue1;
	float pBlueValue1;
	float pRedValue2;
	float pGreenValue2;
	float pBlueValue2;
	bool pIsBundleValid;
};


struct PixelBundleInput
{
	int pBundleId;
	float xPosition0;
	float yPosition0;
	float xPosition1;
	float yPosition1;
	float pRedValue0;
	float pGreenValue0;
	float pBlueValue0;
	float pRedValue1;
	float pGreenValue1;
	float pBlueValue1;
};


struct PixelBundleOutput
{
	int pBundleId;
	float xPosition0;
	float yPosition0;
	float xPosition1;
	float yPosition1;
	float pRedValue0;
	float pGreenValue0;
	float pBlueValue0;
	float pRedValue1;
	float pGreenValue1;
	float pBlueValue1;
	float pRedValueOutput;
	float pGreenValueOutput;
	float pBlueValueOutput;
	bool pIsBundleValid;
};



struct OutputPixelBundleWithValidations
{
	int pBundleId;
	float xPosition;
	float yPosition;
	float pRedValue0;
	float pGreenValue0;
	float pBlueValue0;
	float pRedValue1;
	float pGreenValue1;
	float pBlueValue1;
	float pRedValueOutput;
	float pGreenValueOutput;
	float pBlueValueOutput;
	bool pPixelOutputRedMarker;
	bool pPixelOutputGreenMarker;
	bool pPixelOutputBlueMarker;
	bool pOutputBundleLogic;


};


struct cbColorThreshold
{
	float pRedPixelValue;
	float pGreenPixelValue;
	float pBluePixelValue;
};


struct WindowFunctionOutputBuffer
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float rValue;
	float gValue;
	float bValue;
	float pWindowFunctionValue;
};

typedef struct _THREADARG
{
	SRWLOCK pLockObject;
	int pThreadNumber;
	unsigned int tasksToComplete;
	unsigned int tasksCompleted;
} THREADARG;




struct CBThreshHoldVarCoordinatesVar
{
	float pXPositionDivisor;
	float pYPositionDivisor;

};

struct OutputPixelObjectInterestPointCoordinates
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float rValue;
	float gValue;
	float bValue;
	bool pIsPixelValid;
};

struct CBCentrePosiion
{
	float pXValue;
	float pYValue;
	float pi_var;

};







struct CBThreshHoldVarCoordinates
{
	float pXPosition0;
	float pYPosition0;
	float pXPosition1;
	float pYPosition1;
};




struct OutputPixelObjectTemperature
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float rValue;
	float gValue;
	float bValue;
	float pTemperatureRed;
	float pTemperatureGreen;
	float pTemperatureBlue;
	float pOverallTemperature;
	float pRegionMarkerValue;
};

struct CBTemperatureModelBuffer
{
	float pColorMaximumVar;
	float pTemperatureMaximumVar;
	float pStillRegionMarker;
	float pVolatileRegionMarker;
};



struct cbCircleEquationCompute 
{
	float pCentreX;
	float pCentreY;
	float pCircleRadius;
	float pi_var;
	int pPointLiesOnTheCircle;
	int pPointLiesInsideTheCircle;
	int pPointLiesOutsideOfTheCircle;
};


struct CBLinePointDetector
{
	float pACoefficientVar;
	float pBCoefficientVar;
	int pOnTheLineLabel;
	int pBelowTheLineLabel;
	int pAboveTheLineLabel;
	float pRedComponentVar;
	float pGreenComponentVar;
	float pBlueComponentVar;

};


struct CBPointDetectorForXAndYPosition 
{
	float pSelectedX;
	float pSelectedY;
	float pi_var;
	int pOnTheLineLabelX;
	int pLeftOfTheLineLabel;
	int pRightOfTheLineLabel;
	int pOnTheLineLabelY;
	int pBelowTheLineLabel;
	int pAboveTheLineLabel;
	int pQuarterOneLabel;
	int pQuarterTwoLabel;
	int pQuarterThreeLabel;
	int pQuarterFourLabel;
	int pAxisLabel;

};


struct OutputPixelObjectCorrespondenceLinearCluster
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float rValue;
	float gValue;
	float bValue;
	float xCentrePosition;
	float yCentrePosition;
	float pDistanceToPoint;
	float pAngleTangent;
	float pAngleSine;
	float pAngleCosine;
	float pAngleToPixelData;
	int pClassOfPixelValueX;
	int pClassOfPixelValueY;
	int pQuarterValue;
};





struct CBPointDetectorLineFunction
{
	float pACoefficientVar;
	float pBCoefficientVar;
	int pOnTheLineLabel;
	int pBelowTheLineLabel;
	int pAboveTheLineLabel;

};

struct CBLinePointDetectorYPosition
{
	float pYPositionValue;
	int pOnTheLineLabel;
	int pAboveTheLine;
	int pBelowTheLine;
	float pSelectedRed;
	float pSelectedGreen;
	float pSelectedBlue;

};


struct CBLinePointDetectorXPosition
{
	float xPositionValue;
	int pOnTheLineLabel;
	int pLeftOfTheLineLabel;
	int pRightOfTheLineLabel;
	float pRedComponentVar;
	float pGreenComponentVar;
	float pBlueComponentVar;
};



struct CBHoughTransformLineMagnitude
{
	float pAngleValue0;
	float pLineMagnitude;
	int pPixelMarkerValueOnTheLine;
	int pPixelMarkerValueAboveTheLine;
	int pPixelMarkerValueBelowTheLine;

};





struct OutputPixelObjectHoughTransform
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float rValue;
	float gValue;
	float bValue;
	float pAngleValue;
	float pHoughTransformValue;
	bool pIsPixelValid;
};




struct OutputPixelObjectCorrespondenceLinearFunction
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float rValue;
	float gValue;
	float bValue;
	float pLinearFunctionValue;
	int pClassOfPixelValue;
	bool pIsPixelObjectValid;
};

struct CBPointDetectorX
{
	float pACoefficientVar;
	float pBCoefficientVar;
	int pOnTheLineLabel;
	int pLeftOfTheLineLabel;
	int pRightOfTheLineLabel;

};



struct cbImageLinkedListModel
{
	float pImageWidth;
	float pImageHeight;
	float pIndexDelta;
};


struct OutputPixelObjectPoint2D
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float xPositionPoint;
	float yPositionPoint;
	float rValue;
	float gValue;
	float bValue;
	bool pIsPixelValid;
};


struct OutputPixelObjectFunctionCorrelationValue
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float rValue;
	float gValue;
	float bValue;
	float pOutputFunctionValue;
	bool pIsPixelValid;
};




struct CBLineDetectorTransformCoordinates
{
	float pXPositionLine0;
	float pYPositionLine0;
	float pXPositionLine1;
	float pYPositionLine1;
	float pKCoefficient;
	float pUCoefficient;
};

struct AngleItem
{
	int pAngleId;
	float pAngleValueInRadians;
};


struct OutputPixelObjectCMYK
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float pBlackKey;
	float pCyanValue;
	float pMagentaValue;
	float pYellowColorValue;
};

struct SortedPixelOutput
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float rValue;
	float gValue;
	float bValue;
	float pGroupLabelValue;
};

struct CBPixelSortingForXPosition
{
	float pXThresold;
	float pCentreY;
	int pGroupOneLabelX;
	int pGroupTwoLabelX;
	int pInvalidBlockMarker;
};


struct CBPixelSortingForYPosition
{
	float pYThresold;
	float pCentreX;
	int pGroupOneLabelX;
	int pGroupTwoLabelX;
	int pInvalidBlockMarker;

};




struct CBPixelDetectionForXPositionAndYLimits
{
	float pXConstantVar;
	float pSelectedYMin;
	float pSelectedYMax;
	float pCentreY;
	int pGroupOneLabelX;
	int pGroupTwoLabelX;


};


struct CBPixelSortingForYPositionAndXLimits
{
	float pYConstantVar;
	float pSelectedXMin;
	float pSelectedXMax;
	float pCentreX;
	int pGroupOneLabelX;
	int pGroupTwoLabelX;


};




struct SortedPixelOutputBufer
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float rValue;
	float gValue;
	float bValue;
	float pGroupLabelValue;
};



struct CBPixelSortingRestoration
{
	int pGroupToRestore;
};




struct cbDescriptorRestoration
{
	int pClassToRestore;
};




struct CBQRCodeScanner
{
	float pRedValueForWhiteColor;
	float pGreenValueForWhiteColor;
	float pBlueValueForWhiteColor;
	float pRedValueForBlackColor;
	float pGreenValueForBlackColor;
	float pBlueValueForBlackColor;
	int pBlackBlockMarker;
	int pWhiteBlockMarker;
	int pInvalidBlockMarker;
};

struct OutputPixelQRCode
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	int pQRCodeValue;
};

struct OutputPixelObjectDescriptor
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float pRedValue0;
	float pGreenValue0;
	float pBlueValue0;
	int pClassMarkerValue;
};

struct  CBThreshHoldVarColorDescriptor 
{
	float pRedValueOne;
	float pGreenValueOne;
	float pBlueValueOne;
	float pRedValueTwo;
	float pGreenValueTwo;
	float pBlueValueTwo;
	int pClassOneValue;
	int pClassTwoValue;
	int pBackGroundClassValue;
};

struct  CBThreshHoldVarColorDescriptorWithDelta
{
	float pRedValueOne;
	float pGreenValueOne;
	float pBlueValueOne;
	float pRedValueTwo;
	float pGreenValueTwo;
	float pBlueValueTwo;
	int pClassOneValue;
	int pClassTwoValue;
	int pBackGroundValue;
	float pThresoldValue;
};

struct OutputPixelObjectBlackAndWhite
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float rValue;
	float gValue;
	float bValue;
	int pMarkerCode;
};

struct CBPointDetectorForXAndYPositionCDA
{
	float pSelectedX;
	float pSelectedY;
	float pSelectedRed;
	float pSelectedGreen;
	float pSelectedBlue;
	float pDistanceToCentreThresold;
	float pAngleThresold;
};


struct CBColorSubtraction
{
	float pRedValue;
	float pGreenValue;
	float pBlueValue;
};

struct CBRestorationStructDescriptor
{
	int pClassObjectToRestore;
};


struct ImageColor
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float pColorValue;
};


struct ImagePixelObject
{
	ImageColor pRedColor;
	ImageColor pGreenColor;
	ImageColor pBlueColor;
};


struct CBPixelFilterForYPosition
{
	float pYConstant;
	float pXThresold;
	int pGroupOneLabel;
	int pGroupTwoLabel;
	int pInvalidPixelLabel;
};


struct CBPixelFilterForXPosition
{
	float pXConstant;
	float pYThresold;
	int pGroupOneLabel;
	int pGroupTwoLabel;
	int pInvalidPixelLabel;
};



struct FilterPixelLabel
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float rValue;
	float gValue;
	float bValue;
	int pSectorNumberValue;
	bool pIsPixelValid;
};



struct CBPixelSortingRestorationX 
{
	float pXPositionVar;
	int pSectorValueToRestore;
	int pGroupOneLabel;
	int pGroupTwoLabel;
	int pInvalidPixelLabel;
};


struct OutputPixelObjectCircularData
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float rValue;
	float gValue;
	float bValue;
	float pCentreXValue;
	float pCentreYValue;
	float pDistanceToCentre;
	float pAngleToPixelSine;
	float pAngleToPixelCosine;
	float pAngleToPixelTangent;
	float pAngleValueRadians;
	float pAngleValueDegrees;
	int pQuarterValue;
	int pClassOfPixel;
};


struct CBPixelSortingRestorationY 
{
	float pYPositionVar;
	int pSectorValueToRestore;
	int pGroupOneLabel;
	int pGroupTwoLabel;
	int pInvalidPixelLabel;
};



struct OutputPixelBufferObjectCounting
{
	int pixelNumber;
	float xPosition0;
	float yPosition0;
	float xPosition1;
	float yPosition1;
	float pRedValue0;
	float pGreenValue0;
	float pBlueValue0;
	float pXDifferenceValue;
	float pYDifferenceValue;
	float pDistanceBetweenPixels;
	float pAngleTangentValue;
	float pAngleCosineValue;
	float pAngleSinValue;
	float pAngleBetweenPixelsRadians;
	float pAngleBetweenPixelsDegrees;
	bool pArePixelsDifferent;
	float pAmountOfIdenticalPixels;
	float pAmountOfDifferentPixels;
};


struct CountingCB 
{
	float pi_var;
};


struct cbPixelNumberConstantBuffer
{
	int pMinimumPixelNumber;
	int pMaximumPixelNumber;
};


struct cbCircleEquationRestoration 
{
	int pClassOfPixelToRestore;
};

struct  CBThreshHoldVarColorDescriptorBarriers
{
	float pRedValueOne;
	float pGreenValueOne;
	float pBlueValueOne;
	float pRedValueTwo;
	float pGreenValueTwo;
	float pBlueValueTwo;
	float pDeltaValue;
	int pClassOneValue;
	int pClassTwoValue;
	int pBackGroundValue;
};



struct OutputPixelObjectDescriptorSubtraction
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float pRedValue0;
	float pGreenValue0;
	float pBlueValue0;
	float pFinalSubtractionRed;
	float pFinalSubtractionGreen;
	float pFinalSubtractionBlue;
	float pDifferenceSum;
	int pClassMarkerValue;
};

struct  CBThreshHoldVarColorDescriptorSubtraction
{
	float pRedValueOne;
	float pGreenValueOne;
	float pBlueValueOne;
	float pRedValueTwo;
	float pGreenValueTwo;
	float pBlueValueTwo;
	int pClassOneValue;
	int pClassTwoValue;
	int pClassThreeValue;
};

using namespace std;


bool WayToSortFloatsAscendingFunc(float pFloatA, float pFloatB)
{
	bool pSortVar = false;
	pSortVar = pFloatA < pFloatB;
	return pSortVar;
}





struct SectoringRuleX
{
	float pXConstant;
	float pYBreackPoint;
	int pSectorOne;
	int pSectorTwo;
	int pInvalidPixelLabel;

public:
	SectoringRuleX(int xConstant,
		float pBreakPointVar,
		int pSectorOneVar,
		int pSectorTwoVar,
		int pInvalidPixelLabelVar)
	{
		pXConstant = xConstant;
		pYBreackPoint = pBreakPointVar;
		pSectorOne = pSectorOneVar;
		pSectorTwo = pSectorTwoVar;
		pInvalidPixelLabel = pInvalidPixelLabelVar;
	}

};

struct SectoringRuleY
{
	float pYConstant;
	float pXBreackPoint;
	int pSectorOne;
	int pSectorTwo;
	int pInvalidPixelLabel;

public:
	SectoringRuleY(int yConstant,
		float pBreakPointVar,
		int pSectorOneVar,
		int pSectorTwoVar,
		int pInvalidPixelLabelVar)
	{

		pXBreackPoint = pBreakPointVar;
		pYConstant = yConstant;
		pSectorOne = pSectorOneVar;
		pSectorTwo = pSectorTwoVar;
		pInvalidPixelLabel = pInvalidPixelLabelVar;
	}
};


class SectorObjectX
{


public:
	int pSectorNumber;
	float pXConstant;
	float pYBreackPoint;
	int pSectorOne;
	int pSectorTwo;
	float pSectorColorRed;
	float pSectorColorGreen;
	float pSectorColorBlue;


	vector<PixelObject> pSectoringArrayA;
	vector<PixelObject> pSectoringArrayB;
	float pAverageY0;
	float pAverageY1;


	SectorObjectX(int pSectorNumberVar,
		float pXConstantVar,
		float pBreakPointVar,
		int pSectorOnevVar,
		int pSectorTwoVar,
		float pSectorColorRedVar,
		float pSectorColorGreenVar,
		float pSectorColorBlueVar)
	{

		pSectorNumber = pSectorNumberVar;
		pXConstant = pXConstantVar;
		pYBreackPoint = pBreakPointVar;
		pSectorOne = pSectorOnevVar;
		pSectorTwo = pSectorTwoVar;
		pSectoringArrayA = vector<PixelObject>();
		pSectoringArrayB = vector<PixelObject>();
		pSectorColorRed = pSectorColorRedVar;
		pSectorColorGreen = pSectorColorRedVar;
		pSectorColorBlueVar = pSectorColorRedVar;

	}

	float ComputeAverageYForPixelArray(vector<PixelObject> pPixelObjectVector)
	{
		vector<float> pYValuesArray = vector<float>();
		for (PixelObject pPixelItem : pPixelObjectVector)
		{
			pYValuesArray.push_back(pPixelItem.yPosition);
		}
		std::sort(pYValuesArray.begin(), pYValuesArray.end(), WayToSortFloatsAscendingFunc);
		float pAverageY = 0.0f;
		float pAverageYRounded = 0.0f;
		pAverageY = (pYValuesArray[pYValuesArray.size() - 1] - pYValuesArray[0]) / 2.0f + pYValuesArray[0];
		pAverageYRounded = round(pAverageY);
		return pAverageYRounded;
	}











	float ComputeAverageYForPartA()
	{
		float pAverageX = 0.0F;

		if (pSectoringArrayA.size() > 0.0F)
		{
			pAverageX = ComputeAverageYForPixelArray(pSectoringArrayA);
		}
		return pAverageX;
	}


	float ComputeAverageXForPartB()
	{
		float pAverageY = 0.0F;

		if (pSectoringArrayA.size() > 0.0F)
		{
			pAverageY = ComputeAverageYForPixelArray(pSectoringArrayB);
		}
		return pAverageY;
	}


	PixelObject GeneratePixelA(int pPixelNumber, float pXValue,
		float pRedValueToSelect, float pGreenValueToSelect, float pBlueValueToSelect)
	{
		float pAverageX0 = 0.0f;
		pAverageX0 = ComputeAverageYForPixelArray(pSectoringArrayA);
		PixelObject pPixelObjectVar = PixelObject(pPixelNumber, pXValue, pAverageY0,
			pRedValueToSelect, pGreenValueToSelect, pBlueValueToSelect);
		return pPixelObjectVar;
	}

	PixelObject GeneratePixelB(int pPixelNumber, float pXValue,
		float pRedValueToSelect, float pGreenValueToSelect, float pBlueValueToSelect)
	{
		float pAverageX1 = 0.0f;
		pAverageX1 = ComputeAverageYForPixelArray(pSectoringArrayB);
		PixelObject pPixelObjectVar = PixelObject(pPixelNumber, pXValue, pAverageY1,
			pRedValueToSelect, pGreenValueToSelect, pBlueValueToSelect);
		return pPixelObjectVar;
	}





};




class SectorObjectY
{


public:
	int pSectorNumber;
	float pYConstant;
	float pXBreackPoint;
	int pSectorOne;
	int pSectorTwo;
	vector<PixelObject> pSectoringArrayA;
	vector<PixelObject> pSectoringArrayB;
	float pSectorColorRed;
	float pSectorColorGreen;
	float pSectorColorBlue;
	float pAverageX0;
	float pAverageX1;


	SectorObjectY(int pSectorNumberVar,
		float pYConstantVar,
		float pXBreakPointVar,
		int pSectorOnevVar,
		int pSectorTwoVar,
		float pSectorColorRedVar,
		float pSectorColorGreenVar,
		float pSectorColorBlueVar)
	{
		pSectorNumber = pSectorNumberVar;
		pYConstant = pYConstantVar;
		pXBreackPoint = pXBreakPointVar;
		pSectorColorRed = pSectorColorRedVar;
		pSectorColorGreen = pSectorColorGreenVar;
		pSectorColorBlue = pSectorColorBlueVar;
		pSectoringArrayA = vector<PixelObject>();
		pSectoringArrayB = vector<PixelObject>();
	}

	float ComputeAverageXForPixelArray(vector<PixelObject> pPixelObjectVector)
	{
		vector<float> pXValuesArray = vector<float>();
		for (PixelObject pPixelItem : pPixelObjectVector)
		{
			pXValuesArray.push_back(pPixelItem.xPosition);
		}
		std::sort(pXValuesArray.begin(), pXValuesArray.end(), WayToSortFloatsAscendingFunc);
		float pAverageX = 0.0f;

		pAverageX = round((pXValuesArray[pXValuesArray.size() - 1] - pXValuesArray[0]) / 2.0f + pXValuesArray[0]);
		return pAverageX;
	}

	float ComputeAverageXForPartA()
	{
		float pAverageX = 0.0F;

		if (pSectoringArrayA.size() > 0.0F)
		{
			pAverageX = ComputeAverageXForPixelArray(pSectoringArrayA);
		}
		return pAverageX;
	}


	float ComputeAverageXForPartB()
	{
		float pAverageX = 0.0F;

		if (pSectoringArrayA.size() > 0.0F)
		{
			pAverageX = ComputeAverageXForPixelArray(pSectoringArrayB);
		}
		return pAverageX;
	}

	PixelObject GeneratePixelA(int pPixelNumber, float pYValue,
		float pRedValueToSelect, float pGreenValueToSelect, float pBlueValueToSelect)
	{
		float pAverageX0 = 0.0f;
		pAverageX0 = ComputeAverageXForPixelArray(pSectoringArrayA);
		PixelObject pPixelObjectVar = PixelObject(pPixelNumber, pAverageX0, pYValue,
			pRedValueToSelect, pGreenValueToSelect, pBlueValueToSelect);
		return pPixelObjectVar;
	}

	PixelObject GeneratePixelB(int pPixelNumber, float pYValue,
		float pRedValueToSelect, float pGreenValueToSelect, float pBlueValueToSelect)
	{
		float pAverageX1 = 0.0f;
		pAverageX1 = ComputeAverageXForPixelArray(pSectoringArrayB);
		PixelObject pPixelObjectVar = PixelObject(pPixelNumber, pAverageX1, pYValue,
			pRedValueToSelect, pGreenValueToSelect, pBlueValueToSelect);
		return pPixelObjectVar;
	}





};

struct SectorizationRuleX
{
	int pRuleId;
	float pXConstant;
	float pYThresold;
	float pSelectedColorRed;
	float pSelectedColorGreen;
	float pSelectedColorBlue;

public:
	SectorizationRuleX(int pRuleIdVar,
		float xConstant,
		float yThresold,
		float pSelectedColorRedVar,
		float pSelectedColorGreenVar,
		float pSelectedColorBlueVar)
	{
		pRuleId = pRuleIdVar;
		pXConstant = xConstant;
		pYThresold = yThresold;
		pSelectedColorRed = pSelectedColorRedVar;
		pSelectedColorGreen = pSelectedColorGreenVar;
		pSelectedColorBlue = pSelectedColorBlueVar;
	}
};


struct SectorizationRuleY
{
	int pRuleId;
	float pYConstant;
	float pXThresold;
	float pSelectedColorRed;
	float pSelectedColorGreen;
	float pSelectedColorBlue;

public:
	SectorizationRuleY(int pRuleIdVar,
		int yConstant,
		float xThresold,
		float pSelectedColorRedVar,
		float pSelectedColorGreenVar,
		float pSelectedColorBlueVar)
	{
		pRuleId = pRuleIdVar;
		pYConstant = yConstant;
		pXThresold = xThresold;
		pSelectedColorRed = pSelectedColorRedVar;
		pSelectedColorGreen = pSelectedColorGreenVar;
		pSelectedColorBlue = pSelectedColorBlueVar;
	}
};

struct SectorizationOutputX
{
	int pObjectId;
	float pXConstant;
	float pYThresold;
	vector<PixelObject> pSectorAVector;
	vector<PixelObject> pSectorBVector;
	float pAverageY0;
	float pAverageY1;
public:

	SectorizationOutputX(int pObjectIdVar,
		float pXContantVar,
		float pYThresoldVar,
		vector<PixelObject> pPartAVector,
		vector<PixelObject> pPartBVector,
		float pAverageY0Var,
		float pAverageY1Var)
	{
		pObjectId = pObjectIdVar;
		pXConstant = pXContantVar;
		pYThresold = pYThresoldVar;
		pAverageY0 = pAverageY0Var;
		pAverageY1 = pAverageY1Var;
		pSectorAVector = pPartAVector;
		pSectorBVector = pSectorBVector;
	}


};


struct SectorizationOutputY
{
	int pObjectId;
	float pYConstant;
	float pXThresold;
	vector<PixelObject> pSectorAVector;
	vector<PixelObject> pSectorBVector;
	float pAverageX0;
	float pAverageX1;
public:

	SectorizationOutputY(int pObjectIdVar,
		float pYContantVar,
		float pXThresoldVar,
		vector<PixelObject> pPartAVector,
		vector<PixelObject> pPartBVector,
		float pAverageX0Var,
		float pAverageX1Var)
	{
		pObjectId = pObjectIdVar;
		pYConstant = pYContantVar;
		pXThresold = pXThresoldVar;
		pAverageX0 = pAverageX0Var;
		pAverageX1 = pAverageX1Var;
		pSectorAVector = pPartAVector;
		pSectorBVector = pSectorBVector;
	}


};




BOOL DESCRIPTOR_CANCEL_OUTPUT = TRUE;

class DirectXStandardAlgorithmExecutor :
	public DirectXComputeShaderManagementClass
{

protected:
	

	HRESULT InitializeDirect2DModule(HINSTANCE hInstance, HWND hWnd);
	vector<InputStructureForGraph> pInputStructureForGraphGlobal;
	vector<OutputStructureForGraph> pOutputStructureForGraphGlobal;

	IDXGIFactory* m_pAlgorithmComputationsFactory;
	IDXGIAdapter* m_pAlgorithmComputationAdapter;
	IDXGIOutput* m_pAlgorithmOutputs;
	vector<IDXGIOutput*> pAlgoOutputs;
	ID3D11Device* m_pAlgoComputeDevice;
	ID3D11DeviceContext* m_pAlgoComputeContext;
	static DirectXStandardAlgorithmExecutor* _algorithmExecutorInstance;
	
public:
	HRESULT SectorizePixelArrayAgainstXPositionUsingARule(HINSTANCE hInstance, HWND hWnd,
		vector<PixelObject> pPixelObjectArray,
		float pSelectedColorRed,
		float pSelectedColorGreen,
		float pSelectedColorBlue,
		SectoringRuleX pSectoringRule, SectorObjectX* pOutputSectorObject);

	HRESULT SectorizePixelArrayAgainstYPositionUsingARule(HINSTANCE hInstance, HWND hWnd,
		vector<PixelObject> pPixelObjectArray,
		float pSelectedColorRed,
		float pSelectedColorGreen,
		float pSelectedColorBlue,
		SectoringRuleY pSectoringRule, SectorObjectY* pOutputSectorObject);

	HRESULT ApplyVectorOfSectorizationRulesToPixelVector(
		HINSTANCE hInstance, HWND hWnd,
		vector<PixelObject> pInputObjectVector,
		vector<SectorizationRuleY> pRulesToSectorizeForY,
		vector<SectorizationRuleX> pRulesToSectorizeForX,
		vector<SectorizationOutputX> *pSectorizationOutputXVector,
		vector<SectorizationOutputY> *pSectorizationOutputYVector);

















	HRESULT RestoreRGBPixelFromSubtractionDescriptorForSelectedClass(HINSTANCE hInstance,
		HWND hWnd,
		int pSelectedClass,
		vector<OutputPixelObjectDescriptorSubtraction> pPixelObjectGrayScale,
		vector<PixelObject> *ppOutputPixelObjectGrayScale);
	HRESULT ValidatePixelByColorCorrespondenceUsingColorDescriptorWithDefinedBarriersThreaded(
		HINSTANCE hInstance,
		HWND hWnd,
		UINT pImageWidth,
		UINT pImageHeight,
		float pColorOneRed,
		float pColorOneGreen,
		float pColorOneBlue,
		float pColorTwoRed,
		float pColorTwoGreen,
		float pColorTwoBlue,
		int pClassOneValue,
		int pClassTwoValue,
		int pClassThreeValue,
		float pThresoldValueData,
		vector<PixelObject> pInputPixelObjectVector,
		vector<OutputPixelObjectDescriptor> *pOutputPixelObjectVector,
		vector<PixelObject> *pClassOnePixelObjectVector,
		vector<PixelObject> *pClassTwoPixelObjectVector);






	HRESULT InitializeRGBPixelImageRestorationResourcesFromSubtractionDescriptorForSelectedClass(HINSTANCE hInstance,
		HWND hWnd,
		CBRestorationStructDescriptor pConstantBufferStruct,
		vector<OutputPixelObjectDescriptorSubtraction> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** pConstantBuffer,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObject, ID3D11Buffer** ppReadbackBuffer);




	HRESULT ValidatePixelByColorCorrespondenceUsingColorDescriptorSubtraction(HINSTANCE hInstance,
		HWND hWnd,
		float pSampleRedOne,
		float pSampleGreenOne, float pSampleBlueOne,
		float pSampleRedTwo, float pSampleGreenTwo,
		float pSampleBlueTwo,
		int pClassOneLabel, int pClassTwoLabel,
		int pClassThreeLabel,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectDescriptorSubtraction> *ppPixelObjectGrayScale);

	HRESULT InitializePixelsValidationUsingADescrptorColorsViaSubtraction(HINSTANCE hInstance,
		HWND hWnd,
		CBThreshHoldVarColorDescriptorSubtraction pColorThresoldStructure,
		vector<PixelObject> pPixelBundleArrayStructure,
		ID3D11ComputeShader** ppComputeShaderObjectVector,
		ID3D11Buffer** ppConstantBufferObject,
		ID3D11Buffer** ppPixelBundleInputBuffer,
		ID3D11Buffer** ppPixelBundleOutputBuffer,
		ID3D11ShaderResourceView** ppShaderSRV,
		ID3D11UnorderedAccessView** ppUnorderedUAV,
		ID3D11Buffer** ppReadbackBuffer);


	HRESULT ValidatePixelByColorCorrespondenceUsingColorDescriptorWithDefinedBarriersWithNumberOfThreads(HINSTANCE hInstance,
		HWND hWnd,
		float pSampleRedOne,
		float pSampleGreenOne,
		float pSampleBlueOne,
		float pSampleRedTwo,
		float pSampleGreenTwo,
		float pSampleBlueTwo,
		int pClassOneLabel,
		int pClassTwoLabel,
		int pClassThreeLabel,
		float pThresoldValue,
		UINT pNumberOfThreads,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectDescriptor> *ppPixelObjectGrayScale);

	HRESULT InitializePixelsValidationUsingADescrptorColorsWithBarriersParametrizedThreads(HINSTANCE hInstance,
		HWND hWnd,
		UINT pAmountOfThreads,
		CBThreshHoldVarColorDescriptorBarriers pColorThresoldStructure,
		vector<PixelObject> pPixelBundleArrayStructure,
		ID3D11ComputeShader** ppComputeShaderObjectVector,
		ID3D11Buffer** ppConstantBufferObject,
		ID3D11Buffer** ppPixelBundleInputBuffer,
		ID3D11Buffer** ppPixelBundleOutputBuffer,
		ID3D11ShaderResourceView** ppShaderSRV,
		ID3D11UnorderedAccessView** ppUnorderedUAV,
		ID3D11Buffer** ppReadbackBuffer);


	HRESULT RestoreRGBPixelFromDescriptorSelectedClassWithNumberOfThreads(HINSTANCE hInstance,
		HWND hWnd,
		UINT pAmountOfThreads,
		int pClassToRestore,
		vector<OutputPixelObjectDescriptor> pPixelObjectGrayScale,
		vector<PixelObject> *ppOutputPixelObjectGrayScale);

	HRESULT InitializeRGBPixelImageRestorationResourcesFromDescriptorForSelectedClassWithNumberOfThreads(HINSTANCE hInstance,
		HWND hWnd,
		UINT pAmountOfThreads,
		CBRestorationStructDescriptor pConstantBufferStruct,
		vector<OutputPixelObjectDescriptor> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** pConstantBuffer,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObject, ID3D11Buffer** ppReadbackBuffer);


	
	HRESULT RestoreRGBPixelFromDescriptorForSelectedClassWithSpecifiedThreads(HINSTANCE hInstance,
		HWND hWnd,
		int pSelectedClass,
		UINT pAmountOfThreads,
		vector<OutputPixelObjectDescriptor> pPixelObjectGrayScale,
		vector<PixelObject> *ppOutputPixelObjectGrayScale);
	

	HRESULT ValidatePixelByColorCorrespondenceUsingColorDescriptorWithDefinedBarriersClosedMode(HINSTANCE hInstance,
		HWND hWnd,
		float pSampleRedOne,
		float pSampleGreenOne,
		float pSampleBlueOne,
		float pSampleRedTwo,
		float pSampleGreenTwo,
		float pSampleBlueTwo,
		int pClassOneLabel,
		int pClassTwoLabel,
		int pClassThreeLabel,
		float pThresoldValue,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectDescriptor> *ppPixelObjectGrayScale,
		vector<PixelObject> *pClassOnePixelArray,
		vector<PixelObject> *pClassTwoPixelArray);




	HRESULT	RestoreRGBPixelFromDescriptorForSelectedClass(HINSTANCE hInstance,
		HWND hWnd,
		int pSelectedClass,
		vector<OutputPixelObjectDescriptor> pPixelObjectGrayScale,
		vector<PixelObject> *ppOutputPixelObjectGrayScale);
	HRESULT ValidatePixelByColorCorrespondenceUsingColorDescriptorExact(HINSTANCE hInstance,
		HWND hWnd,
		float pSampleRedOne,
		float pSampleGreenOne, float pSampleBlueOne,
		float pSampleRedTwo, float pSampleGreenTwo,
		float pSampleBlueTwo,
		int pClassOneLabel, int pClassTwoLabel,
		int pClassThreeLabel,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectDescriptor> *ppPixelObjectGrayScale);

	HRESULT InitializePixelsValidationUsingADescrptorColorsExact(HINSTANCE hInstance,
		HWND hWnd,
		CBThreshHoldVarColorDescriptor pColorThresoldStructure,
		vector<PixelObject> pPixelBundleArrayStructure,
		ID3D11ComputeShader** ppComputeShaderObjectVector,
		ID3D11Buffer** ppConstantBufferObject,
		ID3D11Buffer** ppPixelBundleInputBuffer,
		ID3D11Buffer** ppPixelBundleOutputBuffer,
		ID3D11ShaderResourceView** ppShaderSRV,
		ID3D11UnorderedAccessView** ppUnorderedUAV,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RestoreRGBPixelFromDescriptorForClassOne(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelObjectDescriptor> pPixelObjectGrayScale,
		vector<PixelObject> *ppOutputPixelObjectGrayScale);

	HRESULT RestoreRGBPixelFromDescriptorSelectedClass(HINSTANCE hInstance,
		HWND hWnd,
		int pClassToRestore,
		vector<OutputPixelObjectDescriptor> pPixelObjectGrayScale,
		vector<PixelObject> *ppOutputPixelObjectGrayScale);

	HRESULT InitializeRGBPixelImageRestorationResourcesFromDescriptorForSelectedClass(HINSTANCE hInstance,
		HWND hWnd,
		CBRestorationStructDescriptor pConstantBufferStruct,
		vector<OutputPixelObjectDescriptor> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** pConstantBuffer,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObject, ID3D11Buffer** ppReadbackBuffer);

	HRESULT InitializeRGBPixelImageRestorationResourcesFromDescriptorForClassTwo(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelObjectDescriptor> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObject, ID3D11Buffer** ppReadbackBuffer);


	

	HRESULT InitializePixelsValidationUsingADescrptorColorsWithBarriers(HINSTANCE hInstance,
		HWND hWnd,
		CBThreshHoldVarColorDescriptorBarriers pColorThresoldStructure,
		vector<PixelObject> pPixelBundleArrayStructure,
		ID3D11ComputeShader** ppComputeShaderObjectVector,
		ID3D11Buffer** ppConstantBufferObject,
		ID3D11Buffer** ppPixelBundleInputBuffer,
		ID3D11Buffer** ppPixelBundleOutputBuffer,
		ID3D11ShaderResourceView** ppShaderSRV,
		ID3D11UnorderedAccessView** ppUnorderedUAV,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RestoreRGBPixelFromCircularDescriptorForAGivenClass(HINSTANCE hInstance,
		HWND hWnd,
		int pClassToRestore,
		vector<OutputPixelObjectCircularData> pPixelObjectGrayScale,
		vector<PixelObject> *ppOutputPixelObjectGrayScale);

	HRESULT RunRGBImageRestorationFromCirclularData(HINSTANCE hInstance, HWND hWnd,
		ID3D11ComputeShader* pImageRestorationComputeShader,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ShaderResourceView* pImageRestorationSRV,
		ID3D11UnorderedAccessView* pImageRestorationUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV, UINT X, UINT Y, UINT Z);

	HRESULT InitializeRGBPixelImageRestorationResourcesFromCircularEquation(HINSTANCE hInstance,
		HWND hWnd,
		int pSectorNumber,
		vector<OutputPixelObjectCircularData> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObject, ID3D11Buffer** ppReadbackBuffer);


	HRESULT ThresoldPixelValuesByPixelNumberRange(HINSTANCE hInstance, HWND hWnd,
		int pPlixelNumberStart,
		int pPixelNumberEnd,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectFiltered> *ppPixelObjectGrayScale);

	HRESULT InitializeImageThresholdingOperationRGBAgainstPixelNumber(HINSTANCE hInstance,
		HWND hWnd,
		int pPixelNumberStart,
		int pPixelNumberEnd,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<PixelObject> pixelArrayOne,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);




	HRESULT CheckIfImagesAreIdenticalInAThreadWithRestoration(HINSTANCE hInstance,
		HWND hWnd,
		UINT pImageWidth,
		UINT pImageHeight,
		vector<PixelObjectGrayScale> pixelArrayOne,
		vector<PixelObjectGrayScale> pixelArrayTwo,
		int *pAmountOfIdenticalPixels,
		int *pAmountODifferentPixels,
		vector<OutputPixelBuffer> *ppOutputPixelData,
		vector<PixelObjectGrayScale> *ppIdenticalPixelBuffer,
		vector<PixelObjectGrayScale> *ppDifferentPixelBuffer,
		float *ppOutputArrayIntensitySum,
		float *ppOutputArrayAverage,
		float *ppIdenticalPixelsIntensitySum,
		float *ppIdenticalPixelsAverage,
		float *ppDifferentPixelsIntensitySum,
		float *ppDifferentPixelsAverage,
		BOOL *pAreIdenticalOutput);


	HRESULT CheckIfImagesAreIdenticalDecomposed(HINSTANCE hInstance,
		HWND hWnd,
		UINT pImageWidth,
		UINT pImageHeight,
		vector<PixelObjectGrayScale> pixelArrayOne,
		vector<PixelObjectGrayScale> pixelArrayTwo,
		int *pAmountOfidenticalPixels,
		int *pAmountODifferentPixels,
		vector<OutputPixelBuffer> *ppOutputPixelData,
		vector<PixelObjectGrayScale> *ppOutputPixelBuffer,
		vector<PixelObjectGrayScale> *ppIdenticalPixelBuffer,
		vector<PixelObjectGrayScale> *ppDifferentPixelBuffer,
		float *ppImageOneIntensitySum,
		float *ppImageOneAverage,
		float *ppImageTwoIntensitySum,
		float *ppImageTwoAverage,
		float *ppOutputArrayIntensitySum,
		float *ppOutputArrayAverage,
		float *ppIdenticalPixelsIntensitySum,
		float *ppIdenticalPixelsAverage,
		float *ppDifferentPixelsIntensitySum,
		float *ppDifferentPixelsAverage,
		BOOL *pAreIdenticalOutput);

	HRESULT InitializeIdentityVerificationResourcesDecomposed(HINSTANCE hInstance,
		HWND hWnd,
		float pImageWidth,
		float pImageHeight,
		vector<PixelObjectGrayScale> pixelArrayOne,
		vector<PixelObjectGrayScale> pixelArrayTwo,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppInputBufferImageOne,
		ID3D11Buffer** ppInputBufferImageTwo,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11ShaderResourceView** ppSRV2,
		ID3D11UnorderedAccessView** ppUAVObjectOutput,
		ID3D11Buffer** ppReadbackBufferOutput);

	HRESULT RestoreOutputPixelsFromImageComparisonGrayScale(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelBuffer> pPixelObjectGrayScale,
		vector<PixelObjectGrayScale> *ppOutputPixelObjectGrayScale);

	HRESULT RestoreIdenticalPixelsFromImageComparisonGrayScale(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelBuffer> pPixelObjectGrayScale,
		vector<PixelObjectGrayScale> *ppOutputPixelObjectGrayScale);


	HRESULT RestoreDifferentPixelsFromImageComparison(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelBuffer> pPixelObjectGrayScale,
		vector<PixelObjectGrayScale> *ppOutputPixelObjectGrayScale);



	HRESULT InitializeGrayScaleOutputRestorationResources(HINSTANCE hInstance, HWND hWnd,
		vector<OutputPixelBuffer> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObject,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT InitializeGrayScaleIdenticalComparisonResourcesRestoration(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelBuffer> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObject, ID3D11Buffer** ppReadbackBuffer);




	HRESULT InitializeGrayScaleOutputComparisonResources(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelBuffer> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObject, ID3D11Buffer** ppReadbackBuffer);



	

	HRESULT InitializeGrayScaleIdenticalComparisonResources(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelBuffer> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObject, ID3D11Buffer** ppReadbackBuffer);

	HRESULT InitializeGrayScaleDifferentComparisonResources(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelBuffer> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObject, ID3D11Buffer** ppReadbackBuffer);


	HRESULT RunGrayScaleImageRestoration(HINSTANCE hInstance, HWND hWnd,
		ID3D11ComputeShader* pImageRestorationComputeShader,
		ID3D11ShaderResourceView* pImageRestorationSRV,
		ID3D11UnorderedAccessView* pImageRestorationUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV, UINT X, UINT Y, UINT Z);



	HRESULT RestoreNonZeroRGBPixelsFromImageCompationInitialPixels(HINSTANCE hInstance, HWND hWnd,
		vector<OutputPixelBufferRGB> pInputFilterGrayscaleVector,
		vector<PixelObject> *pPixelObjectVectorOutput);

	HRESULT InitializeNonZeroElementsRestorationAfterSubtractionRGBInitialPixels(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelBufferRGB> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObject, ID3D11Buffer** ppReadbackBuffer);

	HRESULT RestoreZeroRGBPixelsFromImageCompationInitialPixels(HINSTANCE hInstance, HWND hWnd,
		vector<OutputPixelBufferRGB> pInputFilterGrayscaleVector, vector<PixelObject> *pPixelObjectVectorOutput);

	HRESULT InitializeZeroElementsRestorationAfterSubtractionRGBInitialPixels(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelBufferRGB> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObject, ID3D11Buffer** ppReadbackBuffer);

	HRESULT	RestoreRGBPixelFromDescriptorForAGivenClass(HINSTANCE hInstance,
		HWND hWnd,
		int pClassToRestore,
		vector<OutputPixelObjectDescriptor> pPixelObjectGrayScale,
		vector<PixelObject> *ppOutputPixelObjectGrayScale);

	HRESULT InitializeRGBPixelImageRestorationResourcesFromDescriptor(HINSTANCE hInstance,
		HWND hWnd,
		int pSectorNumber,
		vector<OutputPixelObjectDescriptor> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObject, ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunRGBImageRestorationFromDescriptor(HINSTANCE hInstance, HWND hWnd,
		ID3D11ComputeShader* pImageRestorationComputeShader,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ShaderResourceView* pImageRestorationSRV,
		ID3D11UnorderedAccessView* pImageRestorationUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV, UINT X, UINT Y, UINT Z);


	HRESULT	RestoreRGBPixelFromDescriptorForAGivenClassSubtraction(HINSTANCE hInstance,
		HWND hWnd,
		int pClassToRestore,
		vector<OutputPixelObjectDescriptorSubtraction> pPixelObjectGrayScale,
		vector<PixelObject> *ppOutputPixelObjectGrayScale);

	HRESULT InitializeRGBPixelImageRestorationResourcesFromDescriptorSubtraction(HINSTANCE hInstance,
		HWND hWnd,
		int pSectorNumber,
		vector<OutputPixelObjectDescriptorSubtraction> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObject, ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunRGBImageRestorationFromDescriptorSubtraction(HINSTANCE hInstance, HWND hWnd,
		ID3D11ComputeShader* pImageRestorationComputeShader,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ShaderResourceView* pImageRestorationSRV,
		ID3D11UnorderedAccessView* pImageRestorationUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV, UINT X, UINT Y, UINT Z);


	HRESULT RestorePolarNotationOfPixels(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelObjectWithAngleValues> pOutputPixelVector,
		vector<PixelObject> *pRestorePixelVector);

	HRESULT InitializePolartNotationRestorationResources(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelObjectWithAngleValues> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBufferClassOne,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObjectOne,
		ID3D11Buffer** ppReadbackBufferOne);


	HRESULT RunPolarNotationRestoration(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pImageRestorationComputeShader,
		ID3D11ShaderResourceView* pImageRestorationSRV,
		ID3D11UnorderedAccessView* pImageRestorationUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV, UINT X, UINT Y, UINT Z);

	HRESULT ThresoldPixelValuesByCriteriaRGBUndiscreteRedColor(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectFiltered> *ppPixelObjectGrayScale);



	HRESULT InitializeImageThresholdingOperationRGBUndiscreteRedColor(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<PixelObject> pixelArrayOne,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);



	HRESULT ThresoldPixelValuesByCriteriaRGBUndiscreteGreenColor(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectFiltered> *ppPixelObjectGrayScale);



	HRESULT InitializeImageThresholdingOperationRGBUndiscreteGreenColor(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<PixelObject> pixelArrayOne,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);


	HRESULT ThresoldPixelValuesByCriteriaRGBUndiscreteBlueColor(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectFiltered> *ppPixelObjectGrayScale);



	HRESULT InitializeImageThresholdingOperationRGBUndiscreteBlueColor(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<PixelObject> pixelArrayOne,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);




	HRESULT RunImageThresholdingOperationUndiscrete(HINSTANCE hInstance, HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);

	HRESULT CompareImagePixelsAgainstCoordianteValues(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObject> pixelArrayOne,
		vector<PixelObject> pixelArrayTwo,
		vector<OutputPixelBufferObjectCounting> *ppOutputPixelData,
		vector<PixelObject> *ppIdenticalPixelBuffer,
		vector<PixelObject> *ppDifferentPixelBuffer);

	HRESULT InitializeObjectCountingOperationResources(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObject> pixelArrayOne,
		vector<PixelObject> pixelArrayTwo,
		CountingCB pCountingCB,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBufferImageOne,
		ID3D11Buffer** ppInputBufferImageTwo,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11ShaderResourceView** ppSRV2,
		ID3D11UnorderedAccessView** ppUAVObjectOutput,
		ID3D11Buffer** ppReadbackBufferOutput);

	HRESULT RunObjectCountingOperation(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11Buffer* pConstantBuffers,
		ID3D11ShaderResourceView* ppSRV1,
		ID3D11ShaderResourceView* ppSRV2,
		ID3D11UnorderedAccessView* pUAVObjectOutput,
		UINT pNumberOfSRV,
		UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);




	HRESULT LocalizePixelsUsingCircleEquatuion(HINSTANCE hInstance,
		HWND hWnd, float pCentreXValue,
		float pCentreYValue, float piVar,
		float pRadiusValue,
		int pPointLiesOnTheCircle,
		int pPointLiesInsideTheCircle,
		int pPointLiesOutsideOfTheCircle,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectCircularData> *ppPixelObjectGrayScale);
	

	HRESULT InitializePixelLocalizationUsingCircularEquationResources(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		float pCentreXValue,
		float pCentreYValue,
		float piVar,
		float pCircleRadius,
		int pPointLiesOnTheCircleVar,
		int pPointLiesInsideTheCircleVar,
		int pPointLiesOutsideOfTheCircleVar,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<PixelObject> pixelArrayOne,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT DetectMultiColorPixelsInImage(HINSTANCE hInstance,
		HWND hWnd,
		float pMinimumX,
		float pMinimumY,
		float pMaximumX,
		float pMaximumY,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectFiltered> *ppPixelObjectGrayScale);

	HRESULT InitializeMultiColorImageDetectionResources(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		float pMinimumX,
		float pMinimumY,
		float pMaximumX,
		float pMaximumY,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<PixelObject> pixelArrayOne,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT ThresoldPixelValuesAgainstDistanceAndAngleValues(HINSTANCE hInstance, HWND hWnd,
		float pImageWidth,
		float pImageHeight,
		float pCentreX,
		float pCentreY,
		float pRedValue,
		float pGreenValue,
		float pBlueValue,
		float pDistanceValue,
		float pAngleValue,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectColorDistanceAndAngleFilter> *ppPixelObjectGrayScale);
	

	HRESULT  InitializeResourcesForColorDistanceAngleFilter(HINSTANCE hInstance, HWND hWnd,
		float pImageWidth, float pImageHeight,
		float pCentreX,
		float pCentreY,
		float pSampleRed,
		float pSampleGreen,
		float pSampleBlue,
		float pDistanceValue,
		float pAngleValue,
		vector<PixelObject> pixelArrayOne,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBufferOne,
		ID3D11Buffer** ppOutputBuffer, 
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObjectOutput, 
		ID3D11Buffer** ppReadbackBufferOutput);



	HRESULT RestoreZeroRGBPixelsFromImageCompation(HINSTANCE hInstance, HWND hWnd,
		vector<OutputPixelBufferRGB> pInputFilterGrayscaleVector, vector<PixelObject> *pPixelObjectVectorOutput);


	HRESULT InitializeZeroElementsRestorationAfterSubtractionRGB(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelBufferRGB> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObject, ID3D11Buffer** ppReadbackBuffer);



	HRESULT RestoreNonZeroRGBPixelsFromImageCompation(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelBufferRGB> pInputFilterGrayscaleVector,
		vector<PixelObject> *pPixelObjectVectorOutput);

	HRESULT InitializeNonZeroElementsRestorationAfterSubtractionRGB(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelBufferRGB> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObject, ID3D11Buffer** ppReadbackBuffer);


	HRESULT ConvertImageDataFromQRCodeModelToQRObject(HINSTANCE hInstance,
		HWND hWnd,
		float pRedValueForBlack,
		float pGreenValueForBlack,
		float pBlueValueForBlack, float pRedValueForWhite, float pGreenValueForWhite, float pBlueValueForWhite, int pMarkerForBlackColor, int pMarkerForWhiteColor,
		vector<OutputPixelObjectBlackAndWhite> ppPixelArrayInput, vector<OutputPixelQRCode> *ppPixelObjectGrayScale);

	HRESULT InitializeImageConversionFromQRCodeModelToQRObject(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelObjectBlackAndWhite> pPixelArrayValue,
		float pRedValueForBlack,
		float pGreenValueForBlack,
		float pBlueValueForBlack,
		float pRedValueForWhite,
		float pGreenValueForWhite,
		float pBlueValueForWhite,
		int pMarkerForBlackColor,
		int pMarkerForWhiteColor,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);


	HRESULT RunQRCodeGenerationProcedure(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);



	HRESULT SubtractColorFromRGBImage(HINSTANCE hInstance,
		HWND hWnd,
		UINT pImageWidth,
		UINT pImageHeight,
		vector<PixelObject> pixelArrayOne,
		float pSampleRed,
		float pSampleGreen,
		float pSampleBlue,
		int *pAmountOfidenticalPixels,
		int *pAmountODifferentPixels,
		vector<OutputPixelBufferRGB> *ppOutputPixelData,
		vector<PixelObject> *ppIdenticalPixelBuffer,
		vector<PixelObject> *ppDifferentPixelBuffer,
		float *ppImageOneIntensitySumRed,
		float *ppImageOneIntensitySumGreen,
		float *ppImageOneIntensitySumBlue,
		float *ppImageOutputIntensitySumRed,
		float *ppImageOutputIntensitySumGreen,
		float *ppImageOutputputIntensitySumBlue,
		float *ppImageIdenticalIntensitySumRed,
		float *ppImageIdenticalIntensitySumGreen,
		float *ppImageIdenticaltIntensitySumBlue,
		float *ppImageDifferentIntensitySumRed,
		float *ppImageDifferentIntensitySumGreen,
		float *ppImageDifferenttIntensitySumBlue,
		float *ppImageOneIntensityAverageRed,
		float *ppImageOneIntensityAverageGreen,
		float *ppImageOneIntensityAverageBlue,
		float *ppImageOutputIntensityAverageRed,
		float *ppImageOutputIntensityAverageGreen,
		float *ppImageOutputputIntensityAverageBlue,
		float *ppImageIdenticalIntensityAverageRed,
		float *ppImageIdenticalIntensityAverageGreen,
		float *ppImageIdenticaltIntensityAverageBlue,
		float *ppImageDifferentIntensityAverageRed,
		float *ppImageDifferentIntensityAverageGreen,
		float *ppImageDifferenttIntensityAverageBlue,
		BOOL *pAreIdenticalOutput);

	HRESULT InitializeIdentityVerificationResourcesRGBAgainstAColor(HINSTANCE hInstance, HWND hWnd,
		float pImageWidth, float pImageHeight,
		float pSampleRed,
		float pSampleGreen,
		float pSampleBlue,
		vector<PixelObject> pixelArrayOne, ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBufferOne,
		ID3D11Buffer** ppOutputBuffer, ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObjectOutput, ID3D11Buffer** ppReadbackBufferOutput);
	



	HRESULT AddColorToImagePixelsFromRGBImage(HINSTANCE hInstance,
		HWND hWnd,
		UINT pImageWidth,
		UINT pImageHeight,
		vector<PixelObject> pixelArrayOne,
		float pSampleRed,
		float pSampleGreen,
		float pSampleBlue,
		vector<OutputPixelBufferRGB> *ppOutputPixelData);


	HRESULT	InitializeIdentityVerificationResourcesRGBAgainstAColorAddition(HINSTANCE hInstance, HWND hWnd,
		float pImageWidth, float pImageHeight,
		float pSampleRed, float pSampleGreen, float pSampleBlue,
		vector<PixelObject> pixelArrayOne, ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBufferOne,
		ID3D11Buffer** ppOutputBuffer, ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObjectOutput, ID3D11Buffer** ppReadbackBufferOutput);

	HRESULT SubtractColorFromRGBImageIdentical(HINSTANCE hInstance,
		HWND hWnd,
		UINT pImageWidth,
		UINT pImageHeight,
		vector<PixelObject> pixelArrayOne,
		float pSampleRed,
		float pSampleGreen,
		float pSampleBlue,
		int *pAmountOfidenticalPixels,
		int *pAmountODifferentPixels,
		vector<OutputPixelBufferRGB> *ppOutputPixelData,
		vector<PixelObject> *ppIdenticalPixelBuffer,
		vector<PixelObject> *ppDifferentPixelBuffer,
		float *ppImageOneIntensitySumRed,
		float *ppImageOneIntensitySumGreen,
		float *ppImageOneIntensitySumBlue,
		float *ppImageOutputIntensitySumRed,
		float *ppImageOutputIntensitySumGreen,
		float *ppImageOutputputIntensitySumBlue,
		float *ppImageIdenticalIntensitySumRed,
		float *ppImageIdenticalIntensitySumGreen,
		float *ppImageIdenticaltIntensitySumBlue,
		float *ppImageDifferentIntensitySumRed,
		float *ppImageDifferentIntensitySumGreen,
		float *ppImageDifferenttIntensitySumBlue,
		float *ppImageOneIntensityAverageRed,
		float *ppImageOneIntensityAverageGreen,
		float *ppImageOneIntensityAverageBlue,
		float *ppImageOutputIntensityAverageRed,
		float *ppImageOutputIntensityAverageGreen,
		float *ppImageOutputputIntensityAverageBlue,
		float *ppImageIdenticalIntensityAverageRed,
		float *ppImageIdenticalIntensityAverageGreen,
		float *ppImageIdenticaltIntensityAverageBlue,
		float *ppImageDifferentIntensityAverageRed,
		float *ppImageDifferentIntensityAverageGreen,
		float *ppImageDifferenttIntensityAverageBlue,
		BOOL *pAreIdenticalOutput);


	HRESULT MultiplyImagePixelValuesByAColor(HINSTANCE hInstance,
		HWND hWnd,
		UINT pImageWidth,
		UINT pImageHeight,
		vector<PixelObject> pixelArrayOne,
		float pSampleRed,
		float pSampleGreen,
		float pSampleBlue,
		int *pAmountOfidenticalPixels,
		int *pAmountODifferentPixels,
		vector<OutputPixelBufferRGB> *ppOutputPixelData,
		vector<PixelObject> *ppIdenticalPixelBuffer,
		vector<PixelObject> *ppDifferentPixelBuffer,
		float *ppImageOneIntensitySumRed,
		float *ppImageOneIntensitySumGreen,
		float *ppImageOneIntensitySumBlue,
		float *ppImageOutputIntensitySumRed,
		float *ppImageOutputIntensitySumGreen,
		float *ppImageOutputputIntensitySumBlue,
		float *ppImageIdenticalIntensitySumRed,
		float *ppImageIdenticalIntensitySumGreen,
		float *ppImageIdenticaltIntensitySumBlue,
		float *ppImageDifferentIntensitySumRed,
		float *ppImageDifferentIntensitySumGreen,
		float *ppImageDifferenttIntensitySumBlue,
		float *ppImageOneIntensityAverageRed,
		float *ppImageOneIntensityAverageGreen,
		float *ppImageOneIntensityAverageBlue,
		float *ppImageOutputIntensityAverageRed,
		float *ppImageOutputIntensityAverageGreen,
		float *ppImageOutputputIntensityAverageBlue,
		float *ppImageIdenticalIntensityAverageRed,
		float *ppImageIdenticalIntensityAverageGreen,
		float *ppImageIdenticaltIntensityAverageBlue,
		float *ppImageDifferentIntensityAverageRed,
		float *ppImageDifferentIntensityAverageGreen,
		float *ppImageDifferenttIntensityAverageBlue,
		BOOL *pAreIdenticalOutput);

	HRESULT InitializeIdentityVerificationResourcesRGBAgainstAColorMultiplication(HINSTANCE hInstance, HWND hWnd,
		float pImageWidth, float pImageHeight,
		float pSampleRed,
		float pSampleGreen,
		float pSampleBlue,
		vector<PixelObject> pixelArrayOne, ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBufferOne,
		ID3D11Buffer** ppOutputBuffer, ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObjectOutput, ID3D11Buffer** ppReadbackBufferOutput);




	HRESULT DivideImagePixelValuesByAColor(HINSTANCE hInstance,
		HWND hWnd, UINT pImageWidth, UINT pImageHeight, vector<PixelObject> pixelArrayOne, float pSampleRed, float pSampleGreen, float pSampleBlue,
		int *pAmountOfidenticalPixels, int *pAmountODifferentPixels, vector<OutputPixelBufferRGB> *ppOutputPixelData,
		vector<PixelObject> *ppIdenticalPixelBuffer, vector<PixelObject> *ppDifferentPixelBuffer, float *ppImageOneIntensitySumRed, float *ppImageOneIntensitySumGreen, float *ppImageOneIntensitySumBlue, float *ppImageOutputIntensitySumRed, float *ppImageOutputIntensitySumGreen, float *ppImageOutputputIntensitySumBlue, float *ppImageIdenticalIntensitySumRed, float *ppImageIdenticalIntensitySumGreen, float *ppImageIdenticaltIntensitySumBlue, float *ppImageDifferentIntensitySumRed, float *ppImageDifferentIntensitySumGreen, float *ppImageDifferenttIntensitySumBlue, float *ppImageOneIntensityAverageRed, float *ppImageOneIntensityAverageGreen, float *ppImageOneIntensityAverageBlue, float *ppImageOutputIntensityAverageRed, float *ppImageOutputIntensityAverageGreen, float *ppImageOutputputIntensityAverageBlue, float *ppImageIdenticalIntensityAverageRed, float *ppImageIdenticalIntensityAverageGreen, float *ppImageIdenticaltIntensityAverageBlue, float *ppImageDifferentIntensityAverageRed, float *ppImageDifferentIntensityAverageGreen, float *ppImageDifferenttIntensityAverageBlue, BOOL *pAreIdenticalOutput);


	HRESULT	InitializeIdentityVerificationResourcesRGBAgainstAColorDivision(HINSTANCE hInstance, HWND hWnd,
		float pImageWidth, float pImageHeight,
		float pSampleRed, float pSampleGreen, float pSampleBlue,
		vector<PixelObject> pixelArrayOne, ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBufferOne,
		ID3D11Buffer** ppOutputBuffer, ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObjectOutput, ID3D11Buffer** ppReadbackBufferOutput);
	

	HRESULT RunImageIdentityVerificationOperationRGBAgainstAColor(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ShaderResourceView* ppSRV1,
		ID3D11UnorderedAccessView* ppUAVObjectOutput,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);

	HRESULT ConvertImageDataFromRGBModelToQRCodeModel(HINSTANCE hInstance, HWND hWnd, float pRedValueForBlack, float pGreenValueForBlack, float pBlueValueForBlack, float pRedValueForWhite,
		float pGreenValueForWhite, float pBlueValueForWhite, int pMarkerForBlackColor, int pMarkerForWhiteColor, vector<PixelObject> ppPixelArrayInput, vector<OutputPixelObjectBlackAndWhite> *ppPixelObjectBlackAndWhiteVector);

	HRESULT InitializeImageConversionFromRGBModeloQRCodeModel(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObject> pPixelArrayValue,
		float pRedValueForBlack,
		float pGreenValueForBlack,
		float pBlueValueForBlack,
		float pRedValueForWhite,
		float pGreenValueForWhite,
		float pBlueValueForWhite,
		int pMarkerForBlackColor,
		int pMarkerForWhiteColor,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);


	HRESULT RunPixelToQRCodeModelConversion(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);





	HRESULT ConvertImageDataFromQRCodeModelToRGBModel(HINSTANCE hInstance,
		HWND hWnd,
		float pRedValueForBlack,
		float pGreenValueForBlack,
		float pBlueValueForBlack,
		float pRedValueForWhite,
		float pGreenValueForWhite,
		float pBlueValueForWhite,
		int pMarkerForBlackColor,
		int pMarkerForWhiteColor,
		vector<OutputPixelObjectBlackAndWhite> ppPixelArrayInput,
		vector<PixelObject> *ppPixelObjectGrayScale);

	HRESULT InitializeImageConversionFromQRCodeModelToRGBModel(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelObjectBlackAndWhite> pPixelArrayValue,
		float pRedValueForBlack,
		float pGreenValueForBlack,
		float pBlueValueForBlack,
		float pRedValueForWhite,
		float pGreenValueForWhite,
		float pBlueValueForWhite,
		int pMarkerForBlackColor,
		int pMarkerForWhiteColor,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunQRCodeToPixelConvertion(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);


	HRESULT InitializePixelsValidationUsingADescrptorColorsWithDelta(HINSTANCE hInstance,
		HWND hWnd,
		CBThreshHoldVarColorDescriptorWithDelta pColorThresoldStructure,
		vector<PixelObject> pPixelBundleArrayStructure,
		ID3D11ComputeShader** ppComputeShaderObjectVector,
		ID3D11Buffer** ppConstantBufferObject,
		ID3D11Buffer** ppPixelBundleInputBuffer,
		ID3D11Buffer** ppPixelBundleOutputBuffer,
		ID3D11ShaderResourceView** ppShaderSRV,
		ID3D11UnorderedAccessView** ppUnorderedUAV,
		ID3D11Buffer** ppReadbackBuffer);


	HRESULT ValidatePixelByColorCorrespondenceUsingColorDescriptorWithDefinedBarriers(HINSTANCE hInstance,
		HWND hWnd,
		float pSampleRedOne,
		float pSampleGreenOne,
		float pSampleBlueOne,
		float pSampleRedTwo,
		float pSampleGreenTwo,
		float pSampleBlueTwo,
		int pClassOneLabel,
		int pClassTwoLabel,
		int pClassThreeLabel,
		float pThresoldValue,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectDescriptor> *ppPixelObjectGrayScale);



	HRESULT ValidatePixelByColorCorrespondenceUsingColorDescriptorWithThresoldConstant(HINSTANCE hInstance,
		HWND hWnd,
		float pSampleRedOne,
		float pSampleGreenOne, float pSampleBlueOne,
		float pSampleRedTwo, float pSampleGreenTwo,
		float pSampleBlueTwo,
		float pThresold,
		int pClassOneLabel, int pClassTwoLabel,
		int pClassThreeLabel,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectDescriptor> *ppPixelObjectGrayScale);

	HRESULT InitializePixelsValidationUsingADescrptorColorsWithThresold(HINSTANCE hInstance,
		HWND hWnd,
		CBThreshHoldVarColorDescriptorWithDelta pColorThresoldStructure,
		vector<PixelObject> pPixelBundleArrayStructure,
		ID3D11ComputeShader** ppComputeShaderObjectVector,
		ID3D11Buffer** ppConstantBufferObject,
		ID3D11Buffer** ppPixelBundleInputBuffer,
		ID3D11Buffer** ppPixelBundleOutputBuffer,
		ID3D11ShaderResourceView** ppShaderSRV,
		ID3D11UnorderedAccessView** ppUnorderedUAV,
		ID3D11Buffer** ppReadbackBuffer);



	HRESULT ValidatePixelByColorCorrespondenceUsingColorDescriptor(HINSTANCE hInstance,
		HWND hWnd,
		float pSampleRedOne,
		float pSampleGreenOne, float pSampleBlueOne,
		float pSampleRedTwo, float pSampleGreenTwo,
		float pSampleBlueTwo,
		int pClassOneLabel, int pClassTwoLabel,
		int pClassThreeLabel,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectDescriptor> *ppPixelObjectGrayScale);

	HRESULT InitializePixelsValidationUsingADescrptorColors(HINSTANCE hInstance,
		HWND hWnd,
		CBThreshHoldVarColorDescriptor pColorThresoldStructure,
		vector<PixelObject> pPixelBundleArrayStructure,
		ID3D11ComputeShader** ppComputeShaderObjectVector,
		ID3D11Buffer** ppConstantBufferObject,
		ID3D11Buffer** ppPixelBundleInputBuffer,
		ID3D11Buffer** ppPixelBundleOutputBuffer,
		ID3D11ShaderResourceView** ppShaderSRV,
		ID3D11UnorderedAccessView** ppUnorderedUAV,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT ThresoldPixelValuesByCriteriaRGBWithColors(HINSTANCE hInstance, HWND hWnd,
		float pMinimumX, float pMinimumY, float pMaximumX, float pMaximumY,
		float pThresholdIntensityValueBottomR,
		float pThresholdIntensityValueTopR,
		float pThresholdIntensityValueBottomG,
		float pThresholdIntensityValueTopG,
		float pThresholdIntensityValueBottomB,
		float pThresholdIntensityValueTopB,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectFiltered> *ppPixelObjectGrayScale);


	HRESULT InitializeRGBPixelImageRestorationResourcesFromSortingGroupTwo(HINSTANCE hInstance,
		HWND hWnd,
		vector<SortedPixelOutput> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObject, ID3D11Buffer** ppReadbackBuffer);


	HRESULT InitializeRGBPixelImageRestorationResourcesFromSortingGroupOne(HINSTANCE hInstance,
		HWND hWnd,
		vector<SortedPixelOutput> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObject, ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunRGBImageRestorationFromSorting(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pImageRestorationComputeShader,
		ID3D11ShaderResourceView* pImageRestorationSRV,
		ID3D11UnorderedAccessView* pImageRestorationUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV, UINT X, UINT Y, UINT Z);

	HRESULT SeparatePixelArrayAgainstYPosition(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObject> pInputVectorArray,
		float pCentreY,
		vector<PixelObject> *pFirstClassArray,
		vector<PixelObject> *pSecondClassArray);

	HRESULT SeparatePixelArrayAgainstXPosition(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObject> pInputVectorArray,
		float pCentreX,
		vector<PixelObject> *pFirstClassArray,
		vector<PixelObject> *pSecondClassArray);

	HRESULT RestoreRGBPixelFromsFromSortingGroupOne(HINSTANCE hInstance,
		HWND hWnd,
		vector<SortedPixelOutput> pPixelObjectGrayScale,
		vector<PixelObject> *ppOutputPixelObjectGrayScale);

	HRESULT FilterPixelsForXPositionConstantAndYBreakpoint(HINSTANCE hInstance, HWND hWnd,
		float pXConstant, float pCentreY,
		int pClassOneValue, int pClassTwoValue,
		int pInvalidPixelValue,
		vector<PixelObject> ppPixelArrayInput,
		vector<FilterPixelLabel> *ppPixelObjectGrayScale);
	

	HRESULT InitializeDataAquisitionAlgorithmsFordXConstantAndYBreakPoint(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		float pCentreY,
		float pXConstant,
		int pClassOneLabel,
		int pClassTwoLabel,
		int pInvalidLabel,
		vector<PixelObject> pixelArrayOne,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);



	
	

	HRESULT FilterPixelsForYPositionConstantAndXBreakpoint(HINSTANCE hInstance,
		HWND hWnd,
		float pYConstant,
		float pCentreX,
		int pClassOneValue,
		int pClassTwoValue,
		int pInvalidLabel,
		vector<PixelObject> ppPixelArrayInput,
		vector<FilterPixelLabel> *ppPixelObjectGrayScale);

	HRESULT InitializeDataAquisitionAlgorithmsForXBreakpointAndYConstant(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		float pCentreX,
		float pYConstant,
		int pClassOneLabel,
		int pClassTwoLabel,
		int pInvalidLabel,
		vector<PixelObject> pixelArrayOne,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunPixelAquisitionAlgorithmRGB(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);

	HRESULT RestoreRGBPixelFromSectoringOperationForXPosition(HINSTANCE hInstance,
		HWND hWnd,
		int pSelectedSector,
		vector<FilterPixelLabel> pPixelObjectGrayScale,
		vector<PixelObject> *ppOutputPixelObjectGrayScale);

	HRESULT InitializeRGBPixelImageRestorationResourcesFromSectoringForXPosition(HINSTANCE hInstance,
		HWND hWnd,
		int pSectorNumber,
		vector<FilterPixelLabel> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObject, ID3D11Buffer** ppReadbackBuffer);

	HRESULT InitializeArraySortingAlgorithmsForYParameter(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		float pXThresold,
		int pClassOneValue,
		int pClassTwoValue,
		int pInvalidPixelLabel,
		vector<PixelObject> pixelArrayOne,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);


	HRESULT RestoreRGBPixelFromSectoringOperationForYPosition(HINSTANCE hInstance,
		HWND hWnd,
		int pSectorNumber,
		vector<FilterPixelLabel> pPixelObjectGrayScale,
		vector<PixelObject> *ppOutputPixelObjectGrayScale);

	HRESULT InitializeRGBPixelImageRestorationResourcesFromSectoringForYPosition(HINSTANCE hInstance,
		HWND hWnd,
		int pSectorNumber,
		vector<FilterPixelLabel> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObject, ID3D11Buffer** ppReadbackBuffer);




	HRESULT RunRGBImageRestorationFromSectoringFilter(HINSTANCE hInstance, HWND hWnd,
		ID3D11ComputeShader* pImageRestorationComputeShader,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ShaderResourceView* pImageRestorationSRV,
		ID3D11UnorderedAccessView* pImageRestorationUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV, UINT X, UINT Y, UINT Z);

	HRESULT SortPixelsForXPosition(HINSTANCE hInstance,
		HWND hWnd,
		float pXThresold,
		int pClassLessThanLeast,
		int pClassInBetween,
		vector<PixelObject> ppPixelArrayInput,
		vector<SortedPixelOutput> *ppPixelObjectGrayScale);

	HRESULT InitializeArraySortingAlgorithmsForXParameter(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		float pXThresold,
		int pClassLessThanLeast,
		int pClassInBetween,
		vector<PixelObject> pixelArrayOne,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);


	HRESULT SortPixelsForYPosition(HINSTANCE hInstance,
		HWND hWnd,
		float pYThresold,
		int pClassOneValue,
		int pClassTwoValue,
		vector<PixelObject> ppPixelArrayInput,
		vector<SortedPixelOutput> *ppPixelObjectGrayScale);

	HRESULT InitializeArraySortingAlgorithmsForYParameter(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		float pXThresold,
		int pClassOneValue,
		int pClassTwoValue,
		vector<PixelObject> pixelArrayOne,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);


	HRESULT RunPixelSortingAlgorithmRGB(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);


	HRESULT RunRGBImageRestorationFromFilter(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pImageRestorationComputeShader,
		ID3D11ShaderResourceView* pImageRestorationSRV,
		ID3D11UnorderedAccessView* pImageRestorationUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV, UINT X, UINT Y, UINT Z);




	HRESULT RestoreRGBPixelFromsFromSortingGroupTwo(HINSTANCE hInstance,
		HWND hWnd,
		vector<SortedPixelOutput> pPixelObjectGrayScale,
		vector<PixelObject> *ppOutputPixelObjectGrayScale);



	
	HRESULT RunRGBImageRestorationFromSorting(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pImageRestorationComputeShader,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ShaderResourceView* pImageRestorationSRV,
		ID3D11UnorderedAccessView* pImageRestorationUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV, UINT X, UINT Y, UINT Z);


	HRESULT DetectPixelOrientationAgainstAPoint(HINSTANCE hInstance,
		HWND hWnd,
		float pXValue,
		float pYValue,
		int pOnTheLineLabelValueX,
		int pLeftOfTheLineLabelValueX,
		int pRightOfTheLineLabelValueX,
		int pOnTheLineLabelValueY,
		int pBottomOfTheLineLabelValueY,
		int pTopOfTheLineLabelValueY,
		int pQuarterOneLabel,
		int pQuarterTwoLabel,
		int pQuarterThreeLabel,
		int pQuarterFourLabel,
		int pAxisLabel,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectCorrespondenceLinearCluster> *ppPixelObjectGrayScale);


	HRESULT InitializeStraigntLineDetectionFunctionForXAndYPosition(HINSTANCE hInstance,
		HWND hWnd,
		UINT pAmountOfSamples,
		float pXValue, float pYValue,
		int pOnTheLineLabelValueX,
		int pLeftOfTheLineLabelValueX,
		int pRightOfTheLineLabelValueX,
		int pOnTheLineLabelValueY,
		int pLeftOfTheLineLabelValueY,
		int pRightOfTheLineLabelValueY,
		int pQuarterOneLabel,
		int pQuarterTwoLabel, 
		int pQuarterThreeLabel,
		int pQuarterFourLabel, 
		int pAxisLabelValue,
		vector<PixelObject> pixelArrayOne,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT ComputePixelDeltaFunctionFromCentreDirect(HINSTANCE hInstance,
		HWND hWnd,
		float pCentreX,
		float pCentreY,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectWithAngleValues> *ppPixelObjectGrayScale);

	HRESULT InitializePixelDeltaComputationResourcesDirect(HINSTANCE hInstance,
		HWND hWnd,
		float pCentreX,
		float pCentreY,
		vector<PixelObject> pixelArrayOne,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBufferObject,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);




	HRESULT DetectLineBetweenPoints(HINSTANCE hInstance,
		HWND hWnd,
		float pMinimumX,
		float pMinimumY,
		float pMaximumX,
		float pMaximumY,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectCorrespondenceLinearFunction> *ppPixelObjectGrayScale);

	HRESULT InitializeLineDetectionBetweenPoints(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		float pMinimumX,
		float pMinimumY,
		float pMaximumX,
		float pMaximumY,
		vector<PixelObject> pixelArrayOne,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunImageLineDetectionOperationRGB(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);




	HRESULT DetectAHoughTransformAgainstADefinedLine(HINSTANCE hInstance,
		HWND hWnd,
		int pLineLengthValue,
		int pAngleInRadians,
		int pOnTheLineLabel, int pAboveTheLine, int pBelowTheLine,
		float pSelectedRed, float pSelectedGreen,
		float pSelectedBlue,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectFiltered> *ppPixelObjectGrayScale);



	HRESULT InitializeHoughTransformADefinedLineParameters(HINSTANCE hInstance,
		HWND hWnd,
		UINT pAmountOfSamples,
		int pLineLengthValue,
		int pAngleInRadians,
		int pOnTheLineLabel,
		int pAboveTheLineLabel,
		int pBelowTheLineLabel,
		vector<PixelObject> pixelArrayOne,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);


	HRESULT RunHoughTransformFunction(HINSTANCE hInstance,
		HWND hWnd, ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);



	HRESULT DetectAHoughTransformAgainstADefinedLineCoordinates(HINSTANCE hInstance,
		HWND hWnd,
		float pXPosition0,
		float pYPosition0,
		float pXPosition1,
		float pYPosition1,
		int pOnTheLineLabel, int pAboveTheLine, int pBelowTheLine, float pSelectedRed,
		float pSelectedGreen, float pSelectedBlue, vector<PixelObject> ppPixelArrayInput,
		vector<float> pAngleValueInDegrees,
		vector<OutputPixelObjectFiltered> *ppPixelObjectGrayScale);



	HRESULT InitializeHoughTransformADefinedLineParametersCoordinates(HINSTANCE hInstance,
		HWND hWnd,
		UINT pAmountOfSamples,
		float pXPosition0,
		float pYPosition0,
		float pXPosition1,
		float pYPosition1,
		int pOnTheLineLabel,
		int pAboveTheLineLabel,
		int pBelowTheLineLabel,
		vector<PixelObject> pixelArrayOne,
		vector<AngleItem> pAngelItemVector,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBufferPixels,
		ID3D11Buffer** ppInputBufferAngles,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11ShaderResourceView** ppSRV2,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);


	HRESULT RunHoughTransformFunctionCoordinates(HINSTANCE hInstance,
		HWND hWnd, ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11ShaderResourceView* pShaderResourceView1,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);


	HRESULT ConvertImageDataFromSinglePixelToLinkedListModel(HINSTANCE hInstance,
		HWND hWnd,
		float pImageWidth,
		float pImageHeight,
		float pIndexDelta,
		vector<PixelObject> pixelArrayOne, vector<PixelObjectLinkedListElement> *pOuputResultArray);

	HRESULT RestoreRGBPixelFromLineDetectorOnTheLineYPosition(HINSTANCE hInstance,
		HWND hWnd,
		float pXValue,
		float pRedComponentValue,
		float pGreenComponentValue,
		float pBlueComponentValue,
		int pOnTheLineLabel,
		int pLeftOfTheLineLabel,
		int pRightOfTheLineLabel,
		vector<OutputPixelObjectCorrespondenceLinearFunction> pPixelObjectGrayScale,
		vector<PixelObject> *ppOutputPixelObjectGrayScaleOnTheLine);

	HRESULT InitializeStraigntLineDetectionFunctionForYValues(HINSTANCE hInstance,
		HWND hWnd,
	UINT pAmountOfSamples,
	float pSelectedRed,
	float pSelectedGreen,
	float pSelectedBlue, 
	float pYValues,
	int pOnTheLineLabel,
	int pAboveTheLineLabel,
	int pBelowTheLineLabel,
	vector<PixelObject> pixelArrayOne,
	ID3D11ComputeShader** ppComputeShaderObject,
	ID3D11Buffer** ppConstantBuffer,
	ID3D11Buffer** ppInputBuffer1,
	ID3D11Buffer** ppOutputBuffer,
	ID3D11ShaderResourceView** ppSRV1,
	ID3D11UnorderedAccessView** ppUAVObject1,
	ID3D11Buffer** ppReadbackBuffer);

	HRESULT InitializeRGBPixelImageRestorationFromLineDetectorOnTheLineYPosition(HINSTANCE hInstance,
		HWND hWnd,
		UINT pAmountOfSamples,
		float pXPositionValue,
		int pOnTheLineLabel,
		int pLeftOfTheLineLabel,
		int pRightTheLineLabel,
		float pRedComponentVar,
		float pGreenComponentVar,
		float pBlueComponentVar,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<OutputPixelObjectCorrespondenceLinearFunction> pixelArrayOne,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBufferBelowTheLine,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT InitializeImageConversionFromRGBModelToLinkedListModel(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		vector<PixelObject> pixelArrayOne,
		float pImageWidth,
		float pImageHeight,
		float pIndexDelta,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunPixelToLinkedListModel(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);


	HRESULT RestoreRGBPixelFromLineDetectorOnTheLine(HINSTANCE hInstance,
		HWND hWnd,
		float pACoefficient,
		float pBCoefficient,
		int pOnTheLineLabel,
		int pAboveTheLineLabel,
		int pBelowTheLineLabel,
		vector<OutputPixelObjectCorrespondenceLinearFunction> pPixelObjectGrayScale,
		vector<PixelObject> *ppOutputPixelObjectGrayScaleOnTheLine);

	HRESULT RestoreRGBPixelFromLineDetectorBelowTheLine(HINSTANCE hInstance, HWND hWnd,
		float pACoefficient,
		float pBCoefficient,
		int pOnTheLineLabel,
		int pAboveTheLineLabel,
		int pBelowTheLineLabel,
		vector<OutputPixelObjectCorrespondenceLinearFunction> pPixelObjectGrayScale,
		vector<PixelObject> *ppOutputPixelObjectGrayScaleBelowTheLine
		);

	HRESULT RestoreRGBPixelFromLineDetectorOnTheLineVertical(HINSTANCE hInstance, HWND hWnd,
		float pACoefficient,
		float pBCoefficient,
		int pOnTheLineLabel,
		int pAboveTheLineLabel,
		int pBelowTheLineLabel,
		vector<OutputPixelObjectCorrespondenceLinearFunction> pPixelObjectGrayScale,
		vector<PixelObject> *ppOutputPixelObjectGrayScaleBelowTheLine
		);


	HRESULT RestoreRGBPixelFromLineDetectorOnTheLineHorizontal(HINSTANCE hInstance, HWND hWnd,
		float pACoefficient,
		float pBCoefficient,
		int pOnTheLineLabel,
		int pAboveTheLineLabel,
		int pBelowTheLineLabel,
		vector<OutputPixelObjectCorrespondenceLinearFunction> pPixelObjectGrayScale,
		vector<PixelObject> *ppOutputPixelObjectGrayScaleBelowTheLine
		);

	HRESULT RestoreRGBPixelFromLineDetectorAboveTheLine(HINSTANCE hInstance, HWND hWnd,
		float pACoefficient,
		float pBCoefficient,
		int pOnTheLineLabel,
		int pAboveTheLineLabel,
		int pBelowTheLineLabel,
		float pRedComponentVar,
		float pGreenComponentVar,
		float pBlueComponentVar,
		vector<OutputPixelObjectCorrespondenceLinearFunction> pPixelObjectGrayScale,
		vector<PixelObject> *ppOutputPixelObjectGrayScaleBelowTheLine
		);

	HRESULT InitializeRGBPixelImageRestorationFromLineDetectorOnTheLineXPosition(HINSTANCE hInstance,
		HWND hWnd,
		UINT pAmountOfSamples,
		float pXValue,
		int pOnTheLineLabel,
		int pAboveTheLineLabel,
		int pBelowTheLineLabel,
		float pRedComponentVar,
		float pGreenComponentVar,
		float pBlueComponentVar,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<OutputPixelObjectCorrespondenceLinearFunction> pixelArrayOne,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RestoreRGBPixelFromLineDetectorAboveTheLine(HINSTANCE hInstance,
		HWND hWnd,
		float pACoefficient,
		float pBCoefficient,
		int pOnTheLineLabel,
		int pAboveTheLineLabel,
		int pBelowTheLineLabel,
		vector<OutputPixelObjectCorrespondenceLinearFunction> pPixelObjectGrayScale,
		vector<PixelObject> *ppOutputPixelObjectGrayScaleBelowTheLine);

	HRESULT DetectAStraigntLineAgainstYPosition(HINSTANCE hInstance,
		HWND hWnd,
		float pYValue,
		float pSelectedRed,
		float pSelectedGreen,
		float pSelectedBlue,
		int pOnTheLineLabel,
		int pAboveTheLineLabel,
		int pBelowTheLineLabel,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectCorrespondenceLinearFunction> *ppPixelObjectGrayScale);



	HRESULT InitializeRGBPixelImageRestorationFromLineDetectorBelowTheLine(HINSTANCE hInstance,
		HWND hWnd,
		UINT pAmountOfSamples,
		float pACoefficient,
		float pBCoefficient,
		int pOnTheLineLabel,
		int pAboveTheLineLabel,
		int pBelowTheLineLabel,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<OutputPixelObjectCorrespondenceLinearFunction> pixelArrayOne,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBufferBelowTheLine,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT InitializeRGBPixelImageRestorationFromLineDetectorOnTheLine(HINSTANCE hInstance,
		HWND hWnd,
		UINT pAmountOfSamples,
		float pACoefficient,
		float pBCoefficient,
		int pOnTheLineLabel,
		int pAboveTheLineLabel,
		int pBelowTheLineLabel,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<OutputPixelObjectCorrespondenceLinearFunction> pixelArrayOne,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBufferBelowTheLine,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT InitializeRGBPixelImageRestorationFromLineDetectorAboveTheLine(HINSTANCE hInstance,
		HWND hWnd,
		UINT pAmountOfSamples,
		float pACoefficient,
		float pBCoefficient,
		int pOnTheLineLabel,
		int pAboveTheLineLabel,
		int pBelowTheLineLabel,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<OutputPixelObjectCorrespondenceLinearFunction> pixelArrayOne,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBufferBelowTheLine,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);

	
	HRESULT RunStraigntLineDetectionObjectAgainstX(HINSTANCE hInstance, 
		HWND hWnd, ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject, 
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV, 
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);

	HRESULT RunStraigntLineRestorationObject(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);




	HRESULT RestoreRGBPixelFromLineDetectorOnTheLineXPosition(HINSTANCE hInstance,
		HWND hWnd,
		float pXValue,
		float pRedComponentValue,
		float pGreenComponentValue,
		float pBlueComponentValue,
		int pOnTheLineLabel,
		int pLeftOfTheLineLabel,
		int pRightOfTheLineLabel,
		vector<OutputPixelObjectCorrespondenceLinearFunction> pPixelObjectGrayScale,
		vector<PixelObject> *ppOutputPixelObjectGrayScaleOnTheLine);

	HRESULT DetectAStraigntWithinAnImage(HINSTANCE hInstance,
		HWND hWnd,
		float pACoefficient,
		float pBCoefficient,
		int pOnTheLineLabel,
		int pAboveTheLine,
		int pBelowTheLine,
		float pSelectedRed,
		float pSelectedGreen,
		float pSelectedBlue,
		vector<PixelObject> ppPixelArrayInput, vector<OutputPixelObjectCorrespondenceLinearFunction> *ppPixelObjectGrayScale);


	HRESULT InitializeStraigntLineDetectionFunction(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		float pSelectedRed,
		float pSelectedGreen,
		float pSelectedBlue,
		float pACoefficientVar,
		float pBCoefficientVar,
		int pOnTheLineLabel,
		int pBelowTheLineLabel,
		int pAboveTheLineLabel,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<PixelObject> pixelArrayOne,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);
	


	HRESULT RunStraigntLineDetectionObject(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);


	HRESULT DetectACircleWithinAnImage(HINSTANCE hInstance,
		HWND hWnd,
		float pCentreX,
		float pCentreY,
		float pRadiusVar,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectFiltered> *ppPixelObjectGrayScale);

	HRESULT InitializeCircleDetectionFunction(HINSTANCE hInstance,
		HWND hWnd,
		float pCentreX,
		float pCentreY,
		float pRadiusVar,
		vector<PixelObject> pixelArrayOne,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunCircleDetectionObject(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);



	HRESULT SelectPixelsOfColorFromImage(HINSTANCE hInstance,
		HWND hWnd,
		float pRedValueToSelect,
		float pGreenValueToSelect,
		float pBlueValueToSelect,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectFiltered> *ppPixelObjectGrayScale);

	HRESULT InitializeColorSelectorOperationRGB(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		float pRedColorToSelect,
		float pGreenColorToSelect,
		float pBlueColorToSelect,
		vector<PixelObject> pixelArrayOne,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);



	HRESULT DetectAStraigntLineAgainstXPosition(HINSTANCE hInstance,
		HWND hWnd,
		float pXValue,
		float pSelectedRed, 
		float pSelectedGreen,
		float pSelecteBlue,
		float pOnTHeLineLabel,
		int pLeftOfTheLineLabel, int pRightOfTheLineLabel,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectCorrespondenceLinearFunction> *ppPixelObjectGrayScale);


	HRESULT InitializePixelDetectionAgainstXFunction(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		float pXValue,
		float pSelectedRed,
		float pSelectedGreen,
		float pSelecteBlue,
		int pOnTheLineLabel,
		int pBelowTheLineLabel,
		int pAboveTheLineLabel,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<PixelObject> pixelArrayOne,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);


	HRESULT RunPixelDetectionAgainstX(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);


	HRESULT ConvertImageDataFromRGBModelToTemperatureModel(HINSTANCE hInstance,
		HWND hWnd,
		float pHighestTemperatureValue,
		float pHighestColorValue,
		float pVolatileRegionMark,
		float pStillRegionMark,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectTemperature> *ppPixelObjectGrayScale);

	HRESULT InitializeImageConversionFromRGBModelToTemperatureModel(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		vector<PixelObject> pixelArrayOne,
		float pColorMaximumVar,
		float pTemperatureMaximumVar,
		float pStillRegionMarker,
		float pVolatileRegionMarker,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);


	HRESULT RunPixelToTemperatureModelConversion(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);


	HRESULT InitializeRGBPixelImageRestorationFromLineDetector(HINSTANCE hInstance,
		HWND hWnd,
		UINT pAmountOfSamples,
		float pACoefficient,
		float pBCoefficient,
		int pOnTheLineLabel,
		int pAboveTheLineLabel,
		int pBelowTheLineLabel,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<OutputPixelObjectCorrespondenceLinearFunction> pixelArrayOne,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);





	HRESULT DetectPixelsOnAHorizontalLine(HINSTANCE hInstance,
		HWND hWnd,
		float pMinimumX,
		float pMaximumX,
		float pYConstant,
		float pRedComponentToSelect,
		float pGreenComponentToSelect,
		float pBlueComponentToSelect,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectFiltered> *ppPixelObjectGrayScale);

	HRESULT InitializePixelHorizontalLineDetection(HINSTANCE hInstance,
		HWND hWnd,
		float pMinimumX,
		float pMaximumX,
		float pYConstant,
		float pRedComponentToSelect,
		float pGreenComponentToSelect,
		float pBlueComponentToSelect,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<PixelObject> pixelArrayOne,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);


	HRESULT DetectPixelsOnAVerticalLine(HINSTANCE hInstance,
		HWND hWnd,
		float pMinimumY,
		float pMaximumY,
		float pXConstant,
		float pRedComponentToSelect,
		float pGreenComponentToSelect,
		float pBlueComponentToSelect,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectFiltered> *ppPixelObjectGrayScale);

	HRESULT InitializePixelVerticalLineDetection(HINSTANCE hInstance,
		HWND hWnd,
		float pMinimumY,
		float pMaximumY,
		float pXConstant,
		float pRedComponentToSelect,
		float pGreenComponentToSelect,
		float pBlueComponentToSelect,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<PixelObject> pixelArrayOne,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);


	HRESULT ComputePixelDeltaFunctionFromCentre(HINSTANCE hInstance,
		HWND hWnd,
		float pCentreX,
		float pCentreY,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectWithAngleValues> *ppPixelObjectGrayScale);


	HRESULT InitializePixelDeltaComputationResources(HINSTANCE hInstance,
		HWND hWnd,
		float pCentreX,
		float pCentreY,
		vector<PixelObject> pixelArrayOne,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBufferObject,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunPixelDeltaFunction(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader*pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);


	HRESULT CheckIfImagesAreIdenticalRGBThreadedUsingSemaphores(
		HINSTANCE hInstance,
		HWND hWnd,
		UINT pImageWidth,
		UINT pImageHeight,
		vector<PixelObject> pixelArrayOne,
		vector<PixelObject> pixelArrayTwo,
		int *pAmountOfidenticalPixels,
		int *pAmountODifferentPixels,
		vector<OutputPixelBufferRGB> *ppOutputPixelDataRGB,
		vector<PixelObject> *ppIdenticalPixelBufferRGB,
		vector<PixelObject> *ppDifferentPixelBufferRGB,
		float *ppImageOneIntensitySumRed,
		float *ppImageOneIntensitySumGreen,
		float *ppImageOneIntensitySumBlue,
		float *ppImageTwoIntensitySumRed,
		float *ppImageTwoIntensitySumGreen,
		float *ppImageTwoIntensitySumBlue,
		float *ppImageOutputIntensitySumRed,
		float *ppImageOutputIntensitySumGreen,
		float *ppImageOutputIntensitySumBlue,
		float *ppImageIdenticalIntensitySumRed,
		float *ppImageIdenticalIntensitySumGreen,
		float *ppImageIdenticaltIntensitySumBlue,
		float *ppImageDifferentIntensitySumRed,
		float *ppImageDifferentIntensitySumGreen,
		float *ppImageDifferenttIntensitySumBlue,
		float *ppImageOneIntensityAverageRed,
		float *ppImageOneIntensityAverageGreen,
		float *ppImageOneIntensityAverageBlue,
		float *ppImageTwoIntensityAverageRed,
		float *ppImageTwoIntensityAverageGreen,
		float *ppImageTwoIntensityAverageBlue,
		float *ppImageOutputIntensityAverageRed,
		float *ppImageOutputIntensityAverageGreen,
		float *ppImageOutputIntensityAverageBlue,
		float *ppImageIdenticalIntensityAverageRed,
		float *ppImageIdenticalIntensityAverageGreen,
		float *ppImageIdenticalIntensityAverageBlue,
		float *ppImageDifferentIntensityAverageRed,
		float *ppImageDifferentIntensityAverageGreen,
		float *ppImageDifferenttIntensityAverageBlue,
		BOOL *pAreIdenticalOutput,
		SYSTEMTIME* pCreationTimeVar,
		SYSTEMTIME* pExitTimeVar,
		SYSTEMTIME* pKernelTimeVar,
		SYSTEMTIME* pUserTimeVar);

	HRESULT CheckIfImagesAreIdenticalRGBThreaded(
		HINSTANCE hInstance,
		HWND hWnd,
		UINT pImageWidth,
		UINT pImageHeight,
		vector<PixelObject> pixelArrayOne,
		vector<PixelObject> pixelArrayTwo,
		int *pAmountOfidenticalPixels,
		int *pAmountODifferentPixels,
		vector<OutputPixelBufferRGB> *ppOutputPixelData,
		vector<PixelObject> *ppIdenticalPixelBuffer,
		vector<PixelObject> *ppDifferentPixelBuffer,
		float *ppImageOneIntensitySumRed,
		float *ppImageOneIntensitySumGreen,
		float *ppImageOneIntensitySumBlue,
		float *ppImageTwoIntensitySumRed,
		float *ppImageTwoIntensitySumGreen,
		float *ppImageTwoIntensitySumBlue,
		float *ppImageOutputIntensitySumRed,
		float *ppImageOutputIntensitySumGreen,
		float *ppImageOutputIntensitySumBlue,
		float *ppImageIdenticalIntensitySumRed,
		float *ppImageIdenticalIntensitySumGreen,
		float *ppImageIdenticaltIntensitySumBlue,
		float *ppImageDifferentIntensitySumRed,
		float *ppImageDifferentIntensitySumGreen,
		float *ppImageDifferenttIntensitySumBlue,
		float *ppImageOneIntensityAverageRed,
		float *ppImageOneIntensityAverageGreen,
		float *ppImageOneIntensityAverageBlue,
		float *ppImageTwoIntensityAverageRed,
		float *ppImageTwoIntensityAverageGreen,
		float *ppImageTwoIntensityAverageBlue,
		float *ppImageOutputIntensityAverageRed,
		float *ppImageOutputIntensityAverageGreen,
		float *ppImageOutputIntensityAverageBlue,
		float *ppImageIdenticalIntensityAverageRed,
		float *ppImageIdenticalIntensityAverageGreen,
		float *ppImageIdenticalIntensityAverageBlue,
		float *ppImageDifferentIntensityAverageRed,
		float *ppImageDifferentIntensityAverageGreen,
		float *ppImageDifferenttIntensityAverageBlue,
		BOOL *pAreIdenticalOutput);

	HRESULT CheckIfImagesAreIdenticalInAThreadPool(HINSTANCE hInstance,
		HWND hWnd,
		UINT pImageWidth,
		UINT pImageHeight,
		vector<PixelObjectGrayScale> pixelArrayOne,
		vector<PixelObjectGrayScale> pixelArrayTwo,
		int *pAmountOfIdenticalPixels,
		int *pAmountODifferentPixels,
		vector<OutputPixelBuffer> *ppOutputPixelData,
		vector<PixelObjectGrayScale> *ppIdenticalPixelBuffer,
		vector<PixelObjectGrayScale> *ppDifferentPixelBuffer,
		float *ppOutputArrayIntensitySum,
		float *ppOutputArrayAverage,
		float *ppIdenticalPixelsIntensitySum,
		float *ppIdenticalPixelsAverage,
		float *ppDifferentPixelsIntensitySum,
		float *ppDifferentPixelsAverage,
		BOOL *pAreIdenticalOutput);



	HRESULT SimulateRobotCleanerSystemForVideoFromFTPAutomaticAPIUsingSemaphores(HINSTANCE hInstance, HWND hWnd,
		LPWSTR pFileNameVideoOne, LPWSTR pFileNameVideoTwo,
		int pXPosition, int pYPosition, int pImageWidth, int pImageHeight, int pWaitingInterval,
		vector<PixelObject> *pImageOneRGBPixels,
		vector<PixelObject> *pImageTwoRGBPixels,
		vector<PixelObjectGrayScale> *pImageOneGrayScale,
		vector<PixelObjectGrayScale> *pImageTwoGrayScale,
		vector<OutputPixelBuffer> *ppOutputPixelBufferArray,
		vector<PixelObjectGrayScale> *pppIdenticalPixelOutputArray,
		vector<PixelObjectGrayScale> *pppDifferentPixelOutputArray);

	HRESULT ComputeWindowFunctionForRGBImage(HINSTANCE hInstance,
		HWND hWnd,
		float pMinimumX,
		float pMinimumY,
		float pMaximumX,
		float pMaximumY,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectFiltered> *ppPixelObjectGrayScale);


	HRESULT RunPixelValidationProcedure(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader*pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);

	HRESULT InitializePixelValidationProcedureColors(HINSTANCE hInstance,
		HWND hWnd,
		float pSamplRed,
		float pSampleGreen,
		float pSampleBlue,
		vector<PixelObject> pixelArrayOne,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);




	HRESULT ValidatePixelBundleUsingADescriptor(HINSTANCE hInstance,
		HWND hWnd,
		UINT pImageWidth,
		UINT pImageHeight,
		float pRedValueLimit,
		float pGreenValueLimit,
        float pBlueValueLimit,
		vector<PixelBundle> pPixelBundleVectp,
		vector<OutputPixelBundleWithValidations> *ppOutputPixelBundleWithValidationss);





	HRESULT InitializePixelBundleValidationProcedure(HINSTANCE hInstance,
		HWND hWnd,
		cbColorThreshold pColorThresoldStructure,
		vector<PixelBundle> pPixelBundleArrayStructure,
		ID3D11ComputeShader** pComputeShaderObjectVector,
		ID3D11Buffer** pConstantBufferObject,
		ID3D11Buffer** pPixelBundleInputBuffer,
		ID3D11Buffer** pPixelBundleOutputBuffer,
		ID3D11ShaderResourceView** ppShaderSRV,
		ID3D11UnorderedAccessView** ppUnorderedUAV,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunPixelBundleValidations(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11Buffer* pConstantBufferObject,
		ID3D11ShaderResourceView* pShaderSRV,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV,
		UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);



	HRESULT SplitResultsOfImageComparisonIntoBundles(HINSTANCE hInstance,
		HWND hWnd,
		UINT pImageWidth,
		UINT pImageHeight,
		vector<OutputPixelBufferRGB> pixelArrayOne,
		vector<PixelBundle> *ppOutputBundleVector);


	HRESULT RunBundleSplittingOperationRGB(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* ppSRV1,
		ID3D11UnorderedAccessView* pUAVObjectOutput,
		UINT pNumberOfSRV,
		UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);

	HRESULT SimulateCurrencyStateMachineObject(HINSTANCE hInstance,
		HWND hWnd,
		int StableStateVar,
		int GrowthStateVar,
		int DeclineStateVar,
		int UndefinedStateVar,
		int VolatileState,
		vector<CurrencyObjectRate> ppCurrencyObjectCollection0,
		vector<CurrencyRateStateMachineObject> *pOutputResourceObject);

	HRESULT InitializeCurrencyStateMachine(HINSTANCE hInstance,
		HWND hWnd,
		cbCurrencyStateMachineBuffer pFinancialSpaceBufferConst,
		vector<CurrencyObjectRate> ppCurrencyObjectCollection0,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppFinancialSpaceConstantBuffer,
		ID3D11Buffer** ppCurrencyBuffer0,
		ID3D11Buffer** ppFinancialSpaceOutputBuffer,
		ID3D11ShaderResourceView** ppCurrencyShaderResource0,
		ID3D11UnorderedAccessView** ppUnorderedAccessViewObject,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunCurrencyStateMachine(HINSTANCE hInstance,
		HWND hWnd, ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV, UINT X, UINT Y, UINT Z);


	
	HRESULT ValidatePixelByColorCorrespondence(HINSTANCE hInstance,
		HWND hWnd,
		float pSampleRed,
		float pSampleGreen,
		float pSampleBlue,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectFiltered> *ppPixelObjectGrayScale);

	HRESULT InitializePixelValidationProcedure(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		float pSampleRed,
		float pSampleGreen,
		float pSampleBlue,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<PixelObjectGrayScale> pixelArrayOne,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);


	

	HRESULT InitializePixelValidationProcedureUsingColorDescriptor(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		float pSampleRedOne,
		float pSampleGreenOne,
		float pSampleBlueOne,
		float pSampleRedTwo,
		float pSampleGreenTwo,
		float pSampleBlueTwo,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<PixelObjectGrayScale> pixelArrayOne,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);



	HRESULT InitializeFinacialSpaceSystem(HINSTANCE hInstance,
		HWND hWnd,
		vector<InputSalesUnit> ppResourceObjectCollection,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppFinancialSpaceInputBuffer,
		ID3D11Buffer** ppFinancialSpaceOutputBuffer,
		ID3D11ShaderResourceView** ppShaderResourceViewObject,
		ID3D11UnorderedAccessView** ppUnorderedAccessViewObject,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunFinancialSpaceSystem(HINSTANCE hInstance, HWND hWnd,
		ID3D11ComputeShader* ppComputeShaderFinance,
		ID3D11ShaderResourceView* ppShaderResourceView,
		ID3D11UnorderedAccessView* ppUnorderedAcessView,
		UINT pNumberOfSRV, UINT pNumberOfUAV, UINT X, UINT Y, UINT Z);

	HRESULT QuantizeYCBCRImageMultiplicationCPU(HINSTANCE hInstance, HWND hWnd,
		vector<YCBRPixelDataBuffer> pYCBRArrayToRgb,
		vector<QuantizationItem> pQuatizedDataVector,
		vector<YCBRPixelDataBuffer> *pPixelObject);

	HRESULT RunDiscreteCosineTransformInverse(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV,
		UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);

	HRESULT QuantizeYCBCRImageAdditionCPU(
		HINSTANCE hInstance,
		HWND hWnd,
		vector<YCBRPixelDataBufferOutputCosine> pPixelObjectYCBCR,
		cbSubtractionBuffer pSubtractionBuffer,
		vector<YCBRPixelDataBufferOutputCosine> *ppOutputPixelObjectGrayScale);


	HRESULT QuantizeYCBCRImageAdditionCPU(
		HINSTANCE hInstance,
		HWND hWnd,
		vector<YCBRPixelDataBuffer> pPixelObjectYCBCR,
		cbSubtractionBuffer pSubtractionBuffer,
		vector<YCBRPixelDataBuffer> *ppOutputPixelObjectGrayScale);

	HRESULT QuantizeYCBCRImageAddition(
		HINSTANCE hInstance,
		HWND hWnd,
		vector<YCBRPixelDataBuffer> pPixelObjectYCBCR,
		cbSubtractionBuffer pSubtractionBuffer,
		vector<YCBRPixelDataBuffer> *ppOutputPixelObjectGrayScale);

	HRESULT InitializeYCBCRQuantizedAddition(HINSTANCE hInstance,
		HWND hWnd,
		vector<YCBRPixelDataBuffer> pInputFilterGrayscaleVector,
		cbSubtractionBuffer pSubtractionBufferConst,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppYCBRInputDataBuffer,
		ID3D11Buffer** ppYCBROutputDataBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView0,
		ID3D11UnorderedAccessView** ppOutputUAVObject,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RestoreRGBPixelFromFilterWithoutExclusion(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelObjectFiltered> pPixelObjectGrayScale,
		vector<PixelObject> *ppOutputPixelObjectGrayScale);

	HRESULT InitializeRGBPixelImageRestorationResourcesWithoutExclusion(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelObjectFiltered> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObject, ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunYCBCRImageAddition(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV,
		UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);

	HRESULT InitializeDiscreteCosineTransformInverse(HINSTANCE hInstance,
		HWND hWnd,
		cbCostantBufferCosine pCosineConstantBuffer,
		vector<YCBRPixelDataBuffer> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppcConstantBufferPtr,
		ID3D11Buffer** ppYCBRInputDataBuffer,
		ID3D11Buffer** ppYCBROutputDataBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView0,
		ID3D11UnorderedAccessView** ppOutputUAVObject,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT QuantizeYCBCRImageMultiplicationCPU(HINSTANCE hInstance, HWND hWnd,
		vector<YCBRPixelDataBufferOutputCosine> pYCBRArrayToRgb,
		vector<QuantizationItem> pQuatizedDataVector,
		vector<YCBRPixelDataBufferOutputCosine> *pPixelObject);


	HRESULT ApplyDiscreteCosineTransformAlgorithmInverse(HINSTANCE hInstance, HWND hWnd,
		cbCostantBufferCosine pCosineBuffer,
		vector<YCBRPixelDataBuffer> pYCBRArrayToRgb,
		vector<YCBRPixelDataBufferOutputCosine> *pPixelObject);

	HRESULT RunYCBCRImageMultiplication(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11ShaderResourceView* pShaderResourceView1,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV,
		UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);

	HRESULT InitializeYCBCRMultiplication(HINSTANCE hInstance,
		HWND hWnd,
		vector<YCBRPixelDataBufferOutputCosine> pInputFilterGrayscaleVector,
		vector<QuantizationItem> pQuantizationItem,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppYCBRInputDataBuffer,
		ID3D11Buffer** ppQuantizationDataBuffer,
		ID3D11Buffer** ppYCBROutputDataBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView0,
		ID3D11ShaderResourceView** ppInputShaderResourceView1,
		ID3D11UnorderedAccessView** ppOutputUAVObject,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT ExperimentWithComplexNumbers(HINSTANCE hInstance, HWND hWnd,
		std::complex<float> pNumberOne, std::complex<float> pNumberTwo,
		std::complex<float> *pSummationValue, std::complex<float> *pSubtractionValue,
		std::complex<float> *pMultiplicationValue, std::complex<float> *pDivisionValue);

	HRESULT ConvertImageDataToGrayScaleInAMutex(HINSTANCE hInstance, HWND hWnd, vector<PixelObject> pPixelObjectInput, vector<PixelObjectGrayScale> *ppOutputPixelObjectGrayScaleArray);

	HRESULT ConvertRGBImageToYCbCrColorSpace(
		HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObject> pPixelObjectGrayScale,
		vector<YCBRPixelDataBuffer> *ppOutputPixelObjectGrayScale);


	
	HRESULT InitializeRGBToYCBRPixelDataConversion(HINSTANCE hInstance,
		HWND hWnd,
		cbConstantBufferYCbr pConstantDataBuffer,
		vector<PixelObject> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
        ID3D11Buffer** ppYCBRConstantBuffer,
		ID3D11Buffer** ppYCBRInputDataBuffer,
		ID3D11Buffer** ppYCBROutputDataBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObject, 
        ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunImageToYCBRConversion(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pImageRestorationComputeShader,
        ID3D11Buffer* pConstantDataBuffer,
		ID3D11ShaderResourceView* pImageRestorationSRV,
		ID3D11UnorderedAccessView* pImageRestorationUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV, UINT X, UINT Y, UINT Z);

	HRESULT ApplyDiscreteCosineTransformAlgorithm(
		HINSTANCE hInstance,
		HWND hWnd,
        cbCostantBufferCosine pCosineBuffer,
		vector<YCBRPixelDataBuffer> pPixelObjectGrayScale,
		vector<YCBRPixelDataBufferOutputCosine> *ppOutputPixelObjectGrayScale);
	HRESULT SimulateQuadraticFunctionInAMutex(HINSTANCE hInstance, HWND hWnd, int pACoefficient, int pBCoefficient, int pCoefficient, vector<InputStructureForGraph> pInputArrayStructure, vector<OutputStructureForGraph> *pOutputStructureForGraph,
		SYSTEMTIME *ppStartTime,
		SYSTEMTIME *ppEndTime,
		SYSTEMTIME *ppKernelTime,
		SYSTEMTIME *ppUserTime);

	HRESULT ConvertNumberToBinary(HINSTANCE hInstance, HWND hWnd, UINT pNumberToConvert, vector<float> *ppDivisionSteps, vector<float> *pBinaryNumber);
	HRESULT InitializeDiscreteCosineTransform(HINSTANCE hInstance,
		HWND hWnd,
		cbCostantBufferCosine pCosineConstantBuffer,
		vector<YCBRPixelDataBuffer> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppcConstantBufferPtr,
		ID3D11Buffer** ppYCBRInputDataBuffer,
		ID3D11Buffer** ppYCBROutputDataBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView0,
		ID3D11UnorderedAccessView** ppOutputUAVObject,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunDiscreteCosineTransform(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pImageRestorationComputeShader,
		ID3D11Buffer* pConstantDataBuffer,
		ID3D11ShaderResourceView* pImageRestorationSRV,
		ID3D11UnorderedAccessView* pImageRestorationUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV, UINT X, UINT Y, UINT Z);

	HRESULT QuantizeYCBCRImageDivisionCPU(HINSTANCE hInstance, HWND hWnd,
		vector<YCBRPixelDataBufferOutputCosine> pYCBRArrayToRgb,
		vector<QuantizationItem> pQuatizedDataVector,
		vector<YCBRPixelDataBufferOutputCosine> *pPixelObject);

	HRESULT QuantizeYCBCRImageDivision(
		HINSTANCE hInstance,
		HWND hWnd,
		vector<YCBRPixelDataBufferOutputCosine> pPixelObjectYCBCR,
        vector<QuantizationItem> pQuantizationItem,
		vector<YCBRPixelDataBufferOutputCosine> *ppOutputPixelObjectGrayScale);
	HRESULT InitializeYCBCRDivision(HINSTANCE hInstance,
		HWND hWnd,
		vector<YCBRPixelDataBufferOutputCosine> pInputFilterGrayscaleVector,
		vector<QuantizationItem> pQuantizationItem,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppYCBRInputDataBuffer,
		ID3D11Buffer** ppQuantizationDaBuffer,
		ID3D11Buffer** ppYCBROutputDataBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView0,
		ID3D11ShaderResourceView** ppInputShaderResourceView1,
		ID3D11UnorderedAccessView** ppOutputUAVObject,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunYCBCRImageMDivision(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pImageRestorationComputeShader,
		ID3D11ShaderResourceView* pImageRestorationSRV0,
		ID3D11ShaderResourceView* pImageRestorationSRV1,
		ID3D11UnorderedAccessView* pImageRestorationUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV, UINT X, UINT Y, UINT Z);

	HRESULT QuantizeYCBCRImageSubtraction(
		HINSTANCE hInstance,
		HWND hWnd,
		vector<YCBRPixelDataBuffer> pPixelObjectYCBCR,
		cbSubtractionBuffer pSubtractionBuffer,
		vector<YCBRPixelDataBuffer> *ppOutputPixelObjectGrayScale);

	HRESULT InitializeYCBCRQuantizedSubtraction(HINSTANCE hInstance,
		HWND hWnd,
		vector<YCBRPixelDataBuffer> pInputFilterGrayscaleVector,
		cbSubtractionBuffer pSubtractionBufferConst,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppYCBRInputDataBuffer,
		ID3D11Buffer** ppYCBROutputDataBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView0,
		ID3D11UnorderedAccessView** ppOutputUAVObject,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunYCBCRImageSubtraction(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV,
		UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);

	YCBRPixelDataBuffer GetYCBCRPixelObjectByXAndYPosition(vector<YCBRPixelDataBuffer> pYCBCRPixelCollectionVar, float pXPosition, float pYPosition);
	HRESULT ConvertYCBRImageToRGBColorSpace(HINSTANCE hInstance, HWND hWnd,
		vector<YCBRPixelDataBuffer> pYCBRArrayToRgb,
		vector<PixelObject> *pPixelObjectVector);

	HRESULT InitializeYCBRToRGBPixelDataConversion(HINSTANCE hInstance,
		HWND hWnd,
		cbConstantBufferYCbrToRGB pConstantDataBuffer,
		vector<YCBRPixelDataBuffer> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppYCBRConstantBuffer,
		ID3D11Buffer** ppYCBRInputDataBuffer,
		ID3D11Buffer** ppYCBROutputDataBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObject,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunYCBRToImageConversion(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pImageRestorationComputeShader,
		ID3D11Buffer* pConstantDataBuffer,
		ID3D11ShaderResourceView* pImageRestorationSRV,
		ID3D11UnorderedAccessView* pImageRestorationUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV, UINT X, UINT Y, UINT Z);

	DirectXStandardAlgorithmExecutor(HINSTANCE hInstance, HWND hWnd);
	~DirectXStandardAlgorithmExecutor(void);
	HRESULT CheckIfImagesAreIdenticalInAThread(HINSTANCE hInstance,
		HWND hWnd,
		UINT pImageWidth,
		UINT pImageHeight,
		vector<PixelObjectGrayScale> pixelArrayOne,
		vector<PixelObjectGrayScale> pixelArrayTwo,
		int *pAmountOfIdenticalPixels,
		int *pAmountODifferentPixels,
		vector<OutputPixelBuffer> *ppOutputPixelData,
		vector<PixelObjectGrayScale> *ppIdenticalPixelBuffer,
		vector<PixelObjectGrayScale> *ppDifferentPixelBuffer,
		float *ppOutputArrayIntensitySum,
		float *ppOutputArrayAverage,
		float *ppIdenticalPixelsIntensitySum,
		float *ppIdenticalPixelsAverage,
		float *ppDifferentPixelsIntensitySum,
		float *ppDifferentPixelsAverage,
		BOOL *pAreIdenticalOutput);

	HRESULT ApplyKMeansFilterToRGBImage(HINSTANCE hInstance, HWND hWnd,
		vector<PixelObject> pPixelObjectToCompute,
		float pCentreX, float pCentreY, float pRadiusValue,
		vector<OutputPixelObjectFilteredRGBKMeans> *pFilteredInputObjectArray);
	HRESULT InitializeKMeansFilterRGBResources(HINSTANCE hInstance,
		HWND hWnd,
		KMeansConstantBuffer pKMeansCB,
		vector<PixelObject> pInputPixelObjectVector,
		ID3D11ComputeShader** pKMeansComputeShader,
		ID3D11Buffer** pKMeansConstantBuffer,
		ID3D11Buffer** pKMeansInputDataBuffer,
		ID3D11Buffer** pKMeansOutputDataBuffer,
		ID3D11ShaderResourceView** pKMeansShaderResource,
		ID3D11UnorderedAccessView** pKMeansUnorderedAccess,
		ID3D11Buffer** pKMeansReadbackBuffer);
	HRESULT RunRGBKMeansFilter(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11Buffer* pConstantBufferObject,
		ID3D11ShaderResourceView* pShaderResourceViewObject,
		ID3D11UnorderedAccessView* pUnorderedAcessViewObject,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);

	HRESULT RunGrayScaleKMeansFilter(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11Buffer* pConstantBufferObject,
		ID3D11ShaderResourceView* pShaderResourceViewObject,
		ID3D11UnorderedAccessView* pUnorderedAcessViewObject,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);
	HRESULT InitializeKMeansFilterGrayScaleResources(HINSTANCE hInstance,
		HWND hWnd,
		KMeansConstantBuffer pKMeansBuffer,
		vector<PixelObjectGrayScale> ppResourceObjectCollection,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppShaderResourceViewObject,
		ID3D11UnorderedAccessView** ppUnorderedAccessViewObject,
		ID3D11Buffer** ppReadbackBuffer);
	HRESULT RunImagePointDetector(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ShaderResourceView* pInputSRVFSM1,
		ID3D11UnorderedAccessView* pOutputUAVFSM, UINT X, UINT Y, UINT Z);
	

	HRESULT InitializeWhiteFontOperationRGB(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		float pRedColorToIgnore,
		float pGreenColorToIgnore,
		float pBlueColorToIgnore,
		vector<PixelObject> pixelArrayOne,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);




	HRESULT InitializeGrayScaleFilterImageRestorationResources(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelObjectFilteredGrayScale> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObject, ID3D11Buffer** ppReadbackBuffer);

	HRESULT DestroyWhiteFontFromGraph(HINSTANCE hInstance,
		HWND hWnd,
		float pRedValueToIgnore,
		float pGreenValueToIgnore,
		float pBlueValueToIgnore,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectFiltered> *ppPixelObjectGrayScale);
	HRESULT DetectPointsInRGBImage(HINSTANCE hInstance, HWND hWnd, vector<PixelObject> ppPixelArrayInput, vector<Point2D> pPointArray2D, vector<OutputPixelObjectPoint2D> *ppPixelObjectGrayScale);
	HRESULT RunImagePointDetector(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pInputSRVFSM1,
		ID3D11ShaderResourceView* pInputSRVFSM2,
		ID3D11UnorderedAccessView* pOutputUAVFSM, UINT X, UINT Y, UINT Z);

	HRESULT InitializeImagePointDetector(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObject> pixelArrayOne,
		vector<Point2D> pPointArray,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppInputBuffer2,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11ShaderResourceView** ppSRV2,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT ApplyKMeansFilterToGrayScaleImage(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObjectGrayScale> pPixelObjectToCompute,
		float pCentreX, float pCentreY,
		float pRadiusValue,
		vector<OutputPixelObjectFilteredGrayScaleKMeans> *pFilteredInputObjectArray);



	HRESULT RestoreInterestPointsFromFilter(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelObjectInterestPoint> pPixelObjectGrayScale,
		vector<PixelObject> *ppOutputPixelObjectGrayScale);

	HRESULT InitializeInterestPointRestorationResources(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelObjectInterestPoint> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObject, ID3D11Buffer** ppReadbackBuffer);
	
	HRESULT RunRGBImageInterestPointRestoration(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pImageRestorationComputeShader,
		ID3D11ShaderResourceView* pImageRestorationSRV,
		ID3D11UnorderedAccessView* pImageRestorationUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV, UINT X, UINT Y, UINT Z);



	HRESULT RestoreRGBPixelFromFilter(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelObjectFiltered> pPixelObjectGrayScale,
		vector<PixelObject> *ppOutputPixelObjectGrayScale);
	HRESULT RestoreGrayScalePixelFromFilter(
		HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelObjectFilteredGrayScale> pPixelObjectGrayScale,
		vector<PixelObjectGrayScale> *ppOutputPixelObjectGrayScale);

	HRESULT InitializeRGBPixelImageRestorationResources(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelObjectFiltered> pInputFilterGrayscaleVector,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBuffer,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObject, ID3D11Buffer** ppReadbackBuffer);

	vector<PixelObjectGrayScale> GetPixelObjectsGrayScaleByXPosition(vector<PixelObjectGrayScale> *pPixelGrayScaleList, float pXPosition);
	vector<PixelObjectGrayScale> GetPixelObjectsGrayScaleByYPosition(vector<PixelObjectGrayScale> *pPixelGrayScaleList, float pYPosition);
	PixelObjectGrayScale GetPixelObjectGrayScaleByXAndYPosition(vector<PixelObjectGrayScale> pPixelGrayScaleList, float pXPosition, float pYPosition);
	PixelObjectGrayScale GetNextGreaterPixelByXPosition(vector<PixelObjectGrayScale> pPixelGrayScaleList, PixelObjectGrayScale pixelToCompare);
	GaussianOutputVar GetNextGaussianValueByXPosition(vector<GaussianOutputVar> ppGaussianImageValues, GaussianOutputVar pGaussianValueToCompare, float pDelta);
	GaussianOutputVar GetNextGaussianValueByYPosition(vector<GaussianOutputVar> ppGaussianImageValues, GaussianOutputVar pGaussianValueToCompare, float pDelta);
	HRESULT ComputePixelImageDerivativeFunctionForYValues(HINSTANCE hInsance,
		HWND hWnd,
		vector<PixelObjectGrayScale> pGrayScalePixelList,
		float pImageWidth,
		float pImageHeight,
		vector<CImageDerivativeClass> *ppImageDerivativeObjectListForYValues);

	float ComputeMax(float pVal0, float pVar1, float pVar2);
	float ComputeMin(float pVal0, float pVar1, float pVar2);

	
	
	HRESULT SimulateFinancialSpaceModule(HINSTANCE hInstance,
		HWND hWnd,
		vector<InputSalesUnit> ppResourceObjectCollection,
		vector<OutputSalesUnit> *pOutputResourceObject);

	HRESULT InitializeFinancialSpaceModule(HINSTANCE hInstance,
		HWND hWnd,
		vector<InputSalesUnit> ppResourceObjectCollection,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppFinancialSpaceInputBuffer,
		ID3D11Buffer** ppFinancialSpaceOutputBuffer,
		ID3D11ShaderResourceView** ppShaderResourceViewObject,
		ID3D11UnorderedAccessView** ppUnorderedAccessViewObject,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunFinancialSpaceModule(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* ppComputeShaderFinance,
		ID3D11ShaderResourceView* ppShaderResourceView,
		ID3D11UnorderedAccessView* ppUnorderedAcessView,
		UINT pNumberOfSRV,
		UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);

	
	
	
	HRESULT SimulateActivControlModule(HINSTANCE hInstance,
		HWND hWnd,
		float fVar,
		float mVar,
		bool doNeedConstantControl,
		float pMaximumDifference,
		float pLimit,
		bool pDoNeedStability,
		vector<ObjectClassInActiv> ppResourceObjectCollection,
		vector<ObjectClassOutActiv> *pOutputResourceObject);

	HRESULT InitializeActivControlModule(HINSTANCE hInstance,
		HWND hWnd, cbConstantBufferAxctivManagement pFinancialSpaceCB,
		vector<ObjectClassInActiv> ppResourceObjectCollection,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppFinancialSpaceConstantBuffer,
		ID3D11Buffer** ppFinancialSpaceInputBuffer,
		ID3D11Buffer** ppFinancialSpaceOutputBuffer,
		ID3D11ShaderResourceView** ppShaderResourceViewObject,
		ID3D11UnorderedAccessView** ppUnorderedAccessViewObject,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunActiVControlModule(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* ppComputeShaderFinance,
		ID3D11Buffer* ppConstantBufferFinance,
		ID3D11ShaderResourceView* ppShaderResourceView,
		ID3D11UnorderedAccessView* ppUnorderedAcessView,
		UINT pNumberOfSRV,
		UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);

	HRESULT SimulateUPWardFinancialTrendValue(HINSTANCE hInstance,
		HWND hWnd,
		UINT pAverageValue,
		UINT pXValue,
		vector<ResourceObject> ppResourceObjectCollection,
		vector<OutputResourceObject> *pOutputResourceObject);

	HRESULT InitializeUpwardFinancialTrensResources(HINSTANCE hInstance,
		HWND hWnd, FinancialSpaceCB pFinancialSpaceCB,
		vector<ResourceObject> ppResourceObjectCollection,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppFinancialSpaceConstantBuffer,
		ID3D11Buffer** ppFinancialSpaceInputBuffer,
		ID3D11Buffer** ppFinancialSpaceOutputBuffer,
		ID3D11ShaderResourceView** ppShaderResourceViewObject,
		ID3D11UnorderedAccessView** ppUnorderedAccessViewObject,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunFinancialSpaceUnitSimulation(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* ppComputeShaderFinance,
		ID3D11Buffer* ppConstantBufferFinance,
		ID3D11ShaderResourceView* ppShaderResourceView,
		ID3D11UnorderedAccessView* ppUnorderedAcessView,
		UINT pNumberOfSRV,
		UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);


	HRESULT SimulateMetalCurrencyRelationships(HINSTANCE hInstance, HWND hWnd,
		UINT pCurrencyId,
		UINT pMetalId,
		DateDataType pDateDataType,
		UINT pAverageValue, UINT pXValue, vector<MetalObject> pMetalCollectionObject, vector<CurrencyObject> pCurrencyCollection, vector<CurrencyMetalPairObject> *pOutputResourceObject);

	HRESULT InitializeMetalCurrencyRelationshipResources(HINSTANCE hInstance,
		HWND hWnd,
		cbCurrencyMetalBuffer pFinancialSpaceBufferConst,
		vector<CurrencyObject> ppCurrencyObjectCollection0,
		vector<MetalObject> ppCurrencyObjectCollection1,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppFinancialSpaceConstantBuffer,
		ID3D11Buffer** ppCurrencyBuffer0,
		ID3D11Buffer** ppCurrencyBuffer1,
		ID3D11Buffer** ppFinancialSpaceOutputBuffer,
		ID3D11ShaderResourceView** ppCurrencyShaderResource0,
		ID3D11ShaderResourceView** ppCurrencyShaderResource1,
		ID3D11UnorderedAccessView** ppUnorderedAccessViewObject,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunMetalCurrencyRelationship(HINSTANCE hInstance,
		HWND hWnd, ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11ShaderResourceView* pShaderResourceView1,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV, UINT X, UINT Y, UINT Z);

	HRESULT SimulateCurrencyCurrencyRelationships(HINSTANCE hInstance,
		HWND hWnd,
		UINT pCurrencyId,
		UINT pMetalId,
		float pAmountOfInvestment,
		DateDataType pCurrentDate,
		vector<CurrencyObject> ppCurrencyObjectCollection0,
		vector<CurrencyObject> ppCurrencyObjectCollection1,
		vector<CurrencyCurrencyPairObject> *pOutputResourceObject);

	HRESULT InitializeCurrencyCurrencyRelationshipResources(HINSTANCE hInstance,
		HWND hWnd, cbCurrencyCurrencyBuffer pFinancialSpaceBufferConst,
		vector<CurrencyObject> ppCurrencyObjectCollection0,
		vector<CurrencyObject> ppCurrencyObjectCollection1,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppFinancialSpaceConstantBuffer,
		ID3D11Buffer** ppCurrencyBuffer0,
		ID3D11Buffer** ppCurrencyBuffer1,
		ID3D11Buffer** ppFinancialSpaceOutputBuffer,
		ID3D11ShaderResourceView** ppCurrencyShaderResource0,
		ID3D11ShaderResourceView** ppCurrencyShaderResource1,
		ID3D11UnorderedAccessView** ppUnorderedAccessViewObject,
		ID3D11Buffer** ppReadbackBuffer);



	HRESULT InitializeMetalCurrencyRelashipResources(HINSTANCE hInstance,
		HWND hWnd,
		cbCurrencyMetalBuffer pFinancialSpaceBufferConst,
		vector<CurrencyObject> ppCurrencyObjectCollection0,
		vector<MetalObject> ppCurrencyObjectCollection1,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppFinancialSpaceConstantBuffer,
		ID3D11Buffer** ppCurrencyBuffer0,
		ID3D11Buffer** ppCurrencyBuffer1,
		ID3D11Buffer** ppFinancialSpaceOutputBuffer,
		ID3D11ShaderResourceView** ppCurrencyShaderResource0,
		ID3D11ShaderResourceView** ppCurrencyShaderResource1,
		ID3D11UnorderedAccessView** ppUnorderedAccessViewObject,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT SimulateDownwardFinancialTrendValues(HINSTANCE
		hInstance,
		HWND hWnd,
		UINT pAverageValue,
		UINT pXValue,
		vector<ResourceObject>
		ppResourceObjectCollection,
		vector<OutputResourceObject>
		*pOutputResourceObject);

	HRESULT InitializeDownwardFinancialTrensResources(HINSTANCE hInstance,
		HWND hWnd,
		FinancialSpaceCB pFinancialSpaceCB,
		vector<ResourceObject>
		ppResourceObjectCollection,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppFinancialSpaceConstantBuffer,
		ID3D11Buffer** ppFinancialSpaceInputBuffer,
		ID3D11Buffer** ppFinancialSpaceOutputBuffer,
		ID3D11ShaderResourceView** ppShaderResourceViewObject,
		ID3D11UnorderedAccessView** ppUnorderedAccessViewObject,
		ID3D11Buffer** ppReadbackBuffer);






	HRESULT RunCurrencyCurrencyRelationship(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11ShaderResourceView* pShaderResourceView1,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV,
		UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);


	HRESULT SimulateImageProcessingFiniteStateMachine(HINSTANCE hInstance, HWND hWnd,
		CBImageIntensityConstants pImageIntensityConstants,
		vector<PixelObject> ppPixelObjectInputArray,
		vector<OutputPixelObjectGrayScaleWithState> *ppPixelObjectOutputArray);

	HRESULT InitializeImageProcessingFiniteStateMachineResources(HINSTANCE hInstance,
		HWND hWnd,
		CBImageIntensityConstants pImageIntensityConstants,
		vector<PixelObject> pImageProcessingInputArray,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBufferObject,
		ID3D11Buffer** ppInputBufferObject,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppInputSRV,
		ID3D11UnorderedAccessView** ppOutputUAV,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunImageProcessingFSM(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer* pConstanBufferFSM,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pInputSRVFSM,
		ID3D11UnorderedAccessView* pOutputUAVFSM,
		UINT X, UINT Y, UINT Z);






	HRESULT SimulateImageProcessingFiniteStateMachineStageTwo(HINSTANCE hInstance, HWND hWnd,
		cbImageProcessingFSMStageTwo pConstantBufferCB,
		vector<OutputPixelObjectGrayScaleWithState> ppPixelObjectInputArrayOne,
		vector<OutputPixelObjectGrayScaleWithState> ppPixelObjectInputArrayTwo,
		vector<OutputPixelObjectGrayScaleWithStateChange> *ppPixelObjectOutputArray);

	HRESULT InitializeImageProcessingFiniteStateMachineResourcesStageTwo(HINSTANCE hInstance,
		HWND hWnd,
		cbImageProcessingFSMStageTwo pConstantBufferCB,
		vector<OutputPixelObjectGrayScaleWithState> pInputArrayForStageOne,
		vector<OutputPixelObjectGrayScaleWithState> pInputArrayForStageTwo,	
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** pConstantBufferObject,
		ID3D11Buffer** ppInputBuffer0,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppInputSRV0,
		ID3D11ShaderResourceView** ppInputSRV1,
		ID3D11UnorderedAccessView** ppOutputUAV,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunImageProcessingFSMStageTwo(HINSTANCE hInstance, HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ShaderResourceView* pInputSRVFSM0,
		ID3D11ShaderResourceView* pInputSRVFSM1,
		ID3D11UnorderedAccessView* pOutputUAVFSM,
		UINT X, UINT Y, UINT Z);




	HRESULT SimulateImageProcessingFiniteStateMachineIntensityPreparatorFunction(HINSTANCE hInstance, HWND hWnd,
		int pIdenticalPixelLabelValue,
        int pDifferentPixelLabelVaule,
		vector<PixelObjectGrayScale> ppPixelObjectInputArrayOne,
		vector<PixelObjectGrayScale> ppPixelObjectInputArrayTwo,
		vector<OutputPixelBufferStateMachineData> *ppPixelObjectOutputArray);

	HRESULT InitializeImageIntensityFiniteStateMachinePreparatorFunctionResources(HINSTANCE hInstance,
		HWND hWnd,
		cbImageProcessingIntensityConstantBuffer pImageIntensityConstants,
		vector<PixelObjectGrayScale> pImageProcessingInputArrayOne,
		vector<PixelObjectGrayScale> pImageProcessingInputArrayTwo,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBufferObject,
		ID3D11Buffer** ppInputBufferObjectOne,
		ID3D11Buffer** ppInputBufferObjectTwo,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppInputSRVOne,
		ID3D11ShaderResourceView** ppInputSRVTwo,
		ID3D11UnorderedAccessView** ppOutputUAV,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunImageProcessingFSMPreparator(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer* pConstanBufferFSM,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pInputSRVFSM0,
		ID3D11ShaderResourceView* pInputSRVFSM1,
		ID3D11UnorderedAccessView* pOutputUAVFSM,
		UINT X, UINT Y, UINT Z);


	HRESULT SimulateImageProcessingFiniteStateMachineIntensitySimulator(HINSTANCE hInstance, HWND hWnd,
		cbImageProcessingFSMStageTwo pConstantBufferForSimulation,
		vector<OutputPixelBufferStateMachineData> ppPixelObjectInputArrayOne,
		vector<OutputPixelBufferStateMachineData> ppPixelObjectInputArrayTwo,
		vector<OutputPixelObjectGrayScaleStateChange> *ppPixelObjectOutputArray);

	HRESULT InitializeImageProcessingFiniteStateMachineIntensityFunctionSimulationResources(HINSTANCE hInstance,
		HWND hWnd,
		cbImageProcessingFSMStageTwo pConstantBufferData,
		vector<OutputPixelBufferStateMachineData> pInputArrayForStageOne,
		vector<OutputPixelBufferStateMachineData> pInputArrayForStageTwo,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBufferObject,
		ID3D11Buffer** ppInputBuffer0,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppInputSRV0,
		ID3D11ShaderResourceView** ppInputSRV1,
		ID3D11UnorderedAccessView** ppOutputUAV,
		ID3D11Buffer** ppReadbackBuffer);
	HRESULT RunImageProcessingFSMSimulator(HINSTANCE hInstance, HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11Buffer* pConstantBufferObject,
		ID3D11ShaderResourceView* pInputSRVFSM0,
		ID3D11ShaderResourceView* pInputSRVFSM1,
		ID3D11UnorderedAccessView* pOutputUAVFSM,
		UINT X, UINT Y, UINT Z);




	HRESULT ClassifyImagePixelsByCriteriaSet(HINSTANCE hInstance,
		HWND hWnd,
		float pImageWidth,
		float pImageHeight,
		vector<PixelObject> ppPixelObjectInputArray,
		vector<PixelObjectWithALabel>
		*ppPixelArrayWithCriteria);

	HRESULT InitializeImagaPixelClassificationResources(HINSTANCE hInstance,
		HWND hWnd,
		float pImageWidth,
		float pImageHeight,
		vector<PixelObject> ppPixelObjectInputArray,
		cbImageLabelConstants pImageIntensityConstants,
        ID3D11ComputeShader** ppComputeShaderImageClassification,
		ID3D11Buffer** ppImageProcessingConstantBuffer,
		ID3D11Buffer** ppImageProcessingInputBuffer,
        ID3D11Buffer** ppImageProcessingOutputBuffer,
		ID3D11ShaderResourceView** ppInputImageProcessingSRV,
		ID3D11UnorderedAccessView** ppOutputUnorderedUAV,
		ID3D11Buffer** ppReadbackBufferImageClassifier);
	HRESULT RunImagePixelClassifierCS(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
        ID3D11Buffer* ppConstantBuffer,
		ID3D11ShaderResourceView* ppSRV1,
		ID3D11UnorderedAccessView* ppUAVObjectOutput,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);



	HRESULT SimulateOddEvenNumberFSM(HINSTANCE hInstance, HWND hWnd,
		OddNumberCB pOddNumberCB,
		vector<float> ppEventNumberArray,	
		vector<OutputArrayOddEventStruct> *ppOutputNumberArrayVarEven,
		vector<OutputArrayOddEventStruct> *ppOutputNumberArrayVarOdd,
		vector<OutputArrayOddEvenFinalStruct> *ppOutputOddEvenFinalArray);

	HRESULT InitializeOddEventNumberFSM(HINSTANCE hInstance,
		HWND hWnd,
		vector<InputArrayOddEvenStruct> ppEventNumberArray,
		OddNumberCB pOddEvenNumberCB,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBufferObject,
		ID3D11Buffer** ppInputNumberBufferObject,
		ID3D11Buffer** ppOutputNumberObjectEven,
		ID3D11Buffer** ppOutputNumberObjectOdd,
		ID3D11Buffer** ppOutputNumberObjectFinal,
		ID3D11ShaderResourceView** ppInputNumberSRV,
		ID3D11UnorderedAccessView** ppOutputEvenNumberUAV,
		ID3D11UnorderedAccessView** ppOutputOddNumberUAV,
		ID3D11UnorderedAccessView** ppFinalOutputUAV,
		ID3D11Buffer** ppReadbackEvenNumberBufferObj,
		ID3D11Buffer** ppReadbackOddNumberBufferObj,
		ID3D11Buffer** ppReadbackBufferFinal);

	HRESULT RunOddEvenNumberFSM(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer* pConstantBufferCB,
		ID3D11ShaderResourceView* ppShaderResourceViewOne,
		ID3D11ComputeShader* ppComputeShaderObject,
		ID3D11UnorderedAccessView* ppUnordererdAccessViewEven,
		ID3D11UnorderedAccessView* ppUnordererdAccessViewOdd,
		ID3D11UnorderedAccessView* ppUnorderedAccessViewFinal,
		UINT X, UINT Y, UINT Z);

	HRESULT ComputePixelImageDerivativeFunctionForXValues(HINSTANCE hInsance,
		HWND hWnd,
		vector<PixelObjectGrayScale> pGrayScalePixelList,
		float pImageWidth,
		float pImageHeight,
		vector<CImageDerivativeClass> *ppImageDerivativeObjectListForX);
	HRESULT ComputeSecondOrderDerivativeFoxXValues(HINSTANCE hInsance,
		HWND hWnd,
		vector<CImageDerivativeClass> pGrayScalePixelList,
		float pImageWidth,
		float pImageHeight,
		vector<CImageDerivativeClass> *ppImageDerivativeObjectListForYValues);
	HRESULT FilterImagePixelsByCondition(HINSTANCE hInstance, HWND hWnd, float pThresholdIntensityValueLower,  float pThresholdIntensityValueUpper, vector<PixelObject> ppPixelArrayInput, vector<PixelObjectGrayScale> *ppPixelObjectGrayScale);
	PixelObjectGrayScale GetImagePixelObjectByXAndYPosition(vector
		<PixelObjectGrayScale> ppImageDerivativeList, float pXPosition, float pYPosition);

	HRESULT ComputeSecondOrderDerivativeFoxYValues(HINSTANCE hInsance,
		HWND hWnd,
		vector<CImageDerivativeClass> pGrayScalePixelList,
		float pImageWidth,
		float pImageHeight,
		vector<CImageDerivativeClass> *ppImageDerivativeObjectListForYValues);
	HRESULT ComputeFullImageFunctionDerivativesForDerivativeVector(HINSTANCE hInstance,
		HWND hWnd, vector<CImageDerivativeClass> ppImageDerivativeObjectListForX,
		vector<CImageDerivativeClass> ppImageDerivativeObjectListForY,
		vector<CImageFullDerivativesClass> *pFullDerivativesClass);
	CImageDerivativeClass GetNextImageDerivativeByYPosition(vector<CImageDerivativeClass> pImageDerivativeList, CImageDerivativeClass pImageDerivative);
	CImageDerivativeClass GetNextImageDerivativeByXPosition(vector
		<CImageDerivativeClass> ppImageDerivativeList, CImageDerivativeClass pImageDerivative);
	CImageDerivativeClass GetImageDerivativeObjectByXAndYPosition(vector<CImageDerivativeClass> ppImageDerivativeList, float pXPosition, float pYPosition);
	PixelObjectGrayScale GetNextGreaterPixelByYPosition(vector<PixelObjectGrayScale> pPixelGrayScaleList, PixelObjectGrayScale pixelToCompare);
	PixelObjectGrayScale GetNextGreaterPixelByYPosition(vector<PixelObjectGrayScale> pPixelGrayScaleList, float pXPosition, float pYPosition);
	PixelObjectGrayScale GetNextGreaterPixelByXPosition(vector<PixelObjectGrayScale> pPixelGrayScaleList, float pXPosition, float pYPosition);


	HRESULT ComputeImageFunctionDerivativesFinalGPU(HINSTANCE hInstance,
		HWND hWnd,
		vector<CImageDerivativeClass> ppImageDerivativeObjectListForX,
		vector<CImageDerivativeClass> ppImageDerivativeObjectListForY,
		vector<CImageFullDerivativeClassStruct> *pFullDerivativesClass);

	HRESULT ComputeMatrixDeterminant(HINSTANCE hInstance,
		HWND hWnd, vector<float> pImageArrayOne,
		UINT pMatrixAWidth,
		UINT pMatrixAHeight,
		UINT pMatrixOutWidth,
		UINT pMatrixOutHeight,
		vector<BufType> *ppMatrixVariableOutputType);

	HRESULT InitializeMatrixDeterminantResources(HINSTANCE hInstance,
		HWND hWnd,
		vector<BufType> pMatrixOne,
		UINT pMatrixAHeight,
		UINT pMatrixAWidth,
		UINT pMatrixOutHeight,
		UINT pMatrixOutWidth,
		ID3D11ComputeShader** ppMatrixComputeShader,
		ID3D11Buffer** ppMatrixMultiplyConstantBuffer,
		ID3D11Buffer** ppMatrixMultiplyInputBufferB,
		ID3D11Buffer** ppMatrixMultiplyOutputBuffer,
		ID3D11ShaderResourceView** pOutputSRV0,
		ID3D11UnorderedAccessView** pOutputUAV);

	
	HRESULT CreateSRVForBuffer(HINSTANCE hInstance, HWND hWnd, ID3D11Buffer* pInputBuffer, ID3D11ShaderResourceView** ppShaderResourceSRV);
	HRESULT CreateByteBufferUAV(HINSTANCE hInstance, HWND hWnd, ID3D11Device* pDevice, ID3D11Buffer* ppDataBufferUAV, ID3D11UnorderedAccessView** ppUnorderedAccessView);
	HRESULT ExecuteFFTSimulationData(HINSTANCE hInstance, HWND hWnd,
		vector<BufType> pInitialDataSet0,
		vector<BufType> pInverseDataFFT,
		ID3D11Buffer* pInputBuffer0,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11ShaderResourceView* pHarmonicsSRV,
		ID3D11UnorderedAccessView* pExponentialBufferOutUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);
	HRESULT	SimulateSimpleGPUMultiplication(HINSTANCE hInstance, HWND hWnd,
		vector<UINT> pArrayOneData, vector<UINT> pArrayTwoData, vector<UINT> *pOutputArrayData);
	HRESULT SimulateArrayAdditionAlgorithmUsingAppendConsume(HINSTANCE hInstance, HWND hWnd,
		vector<UINT> pArrayOneData, vector<UINT> pArrayTwoData, vector<UINT> *pOutputArrayData);
	
	HRESULT InitializeArrrayAdditionUsingAppendConsumeResources(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppInputBuffer2,
		ID3D11Buffer** ppOutputBuffer,
		vector<UINT> pArrayOneVals,
		vector<UINT> pArrayTwoVals,
		ID3D11ShaderResourceView** ppInputSRV1,
		ID3D11ShaderResourceView** ppInputSRV2,
		ID3D11UnorderedAccessView** ppInputUAV1,
		ID3D11UnorderedAccessView** ppInputUAV2,
		ID3D11UnorderedAccessView** ppOutputUAV,
		ID3D11Buffer** ppReadbackBuffer,
		vector<BufType> *ppOutputArr0,
		vector<BufType> *ppOutputArr1);

	HRESULT	SimulateParrallelReduction(HINSTANCE hInstance, HWND hWnd,
		vector<UINT> pArrayOneData, vector<BufType> *pOutputArrayData);

	HRESULT SimulateMatrixMultiplicationOptionTwo(HINSTANCE hInstance,
		HWND hWnd,
		vector<UINT> pImageArrayOne,
		vector<UINT> pImageArrayTwo,
		UINT pMatrixSize,
		vector<MatrixVariableType> *ppMatrixVariableOutputType);
	HRESULT ComputeImageFunctionDerivatives(HINSTANCE hInstance,
		HWND hWnd, vector<CImageDerivativeClass> ppImageDerivativeObjectListForX,
		vector<CImageDerivativeClass> ppImageDerivativeObjectListForY,
		vector<CImageFullDerivativesClass> *pFullDerivativesClass);

HRESULT InitalizeSortingAlgorithmResources(HINSTANCE hInstance, HWND hWnd, ID3D11ComputeShader** pComputeShaderSort, ID3D11ComputeShader** pMatrixTranspose, ID3D11Buffer** pBitonicConstantBuffer,
		ID3D11Buffer** pFirstBuffer, ID3D11Buffer** pSecondBuffer, ID3D11ShaderResourceView** pShaderSRV0, ID3D11ShaderResourceView** pShaderSRV1,
		ID3D11UnorderedAccessView** ppUAV0, ID3D11UnorderedAccessView** ppUAV1,
		ID3D11Buffer** pReadBackBuffer);
	HRESULT CreateByteOrderBufferOnGPU(ID3D11Device* pDevice, UINT uElementSize, UINT uCount, VOID* pInitData, ID3D11Buffer** ppBufOut);
	HRESULT CreateBufferSRV(ID3D11Device* pDevice, ID3D11Buffer* pBuffer, ID3D11ShaderResourceView** ppSRVOut);
	HRESULT CreateBufferUAV(ID3D11Device* pDevice, ID3D11Buffer* pBuffer, ID3D11UnorderedAccessView** ppUAVOut);
	ID3D11Buffer* CreateAndCopyToDebugBufferW(ID3D11Buffer* pBuffer, ID3D11Device *pDevice, ID3D11DeviceContext *pContextOut);
	HRESULT PerformFFTSimulation(HINSTANCE hInstance, HWND hWnd, int pJValue, int pKValue, vector<float> pInputData, vector<double> pTimeArray,
		vector<float> *pOutputData);
	HRESULT RunArrayAdditionShaderAlgorithm(
		HINSTANCE hInstance, HWND hWnd,
		ID3D11ShaderResourceView* ppShaderResourceViewOne,
		ID3D11ShaderResourceView* ppShaderResourceView,
		ID3D11ComputeShader* ppComputeShaderObject,
		ID3D11UnorderedAccessView* ppUnordererdAccessViews,
		UINT X, UINT Y, UINT Z);

	HRESULT RunParallelReductionAlgorithm(
		HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer* pConstantBufferCB,
		ID3D11ShaderResourceView* ppShaderResourceViewOne,
		ID3D11ComputeShader* ppComputeShaderObject,
		ID3D11UnorderedAccessView* ppUnordererdAccessViews,
		UINT X, UINT Y, UINT Z);

	HRESULT RunArrayAdditionShaderAlgorithmAppendConsume(HINSTANCE hInstance,
		HWND hWnd,
		vector<BufType> pInputArrayA,
		vector<BufType> pInputArrayB,
		ID3D11Buffer* ppInputBufferA,
		ID3D11Buffer* ppInputBufferB,
		ID3D11ShaderResourceView* pSRV1,
		ID3D11ShaderResourceView* pSRV2,
		ID3D11UnorderedAccessView* pUAV0,
		ID3D11UnorderedAccessView* pUAV1,
		ID3D11ComputeShader* ppComputeShaderObject,
		ID3D11UnorderedAccessView* pUAV2,
		UINT X, UINT Y, UINT Z);

	HRESULT InitializeFFTOneResources(HINSTANCE hInstance,
		HWND hWnd,
		UINT pAmountOfSamples,
		vector<BufType> pInputDataArray,
		vector<BufType> pHarmonicsArray,
		ID3D11ComputeShader** ppComputeShaderFFT,
		ID3D11Buffer** ppBufferOneFFT,
		ID3D11Buffer** ppBitReverseBufferFFT,
		ID3D11ShaderResourceView** ppSRVOneFFT,
		ID3D11ShaderResourceView** pBitReverseFFTSRV,
		ID3D11Buffer** ppExponentOutputBuffer,
		ID3D11UnorderedAccessView** ppUnorderededAccessViewExponents,
		ID3D11Buffer** ppReadbackBufferExonents);

	HRESULT ConvertAnArrayOfNumbersToButterFlyConvert(HINSTANCE hInstance,
		HWND hWnd,
		vector<BufType> pNumberToConvert,
		vector<DivisionRemainder> *ppDivisionRemainders);
	

	HRESULT SimulateMatrixMultiplicatonOnCPU(HINSTANCE hInstance,
		HWND hWnd,
		vector<float> pMatrixArrayOne,
		vector<float> pMatrixArrayTwo,
		vector<float> *pOutputVector);

	HRESULT InitializeButterFlyConvertionResources(HINSTANCE hInstance,
		HWND hWnd,
		vector<BufType> pInputDataBuffer,
		ID3D11ComputeShader** ppConvertToButterFlyComputeShader,
		ID3D11Buffer** ppInputBufferButterfly,
		ID3D11ShaderResourceView** ppInputButterflySRV,
		ID3D11Buffer** ppOutputBufferButterflyForDivisors,
		ID3D11Buffer** ppOutputBufferForRemainders,
		ID3D11UnorderedAccessView** ppOutputBufferForOutcome,
		ID3D11UnorderedAccessView** ppUnorderedAccessViewButterFly,
		ID3D11Buffer** ppReadbackDataBufferOutcome,
		ID3D11Buffer** pRemainderOutcome);

	HRESULT RunButterflyConvertionComputeShader(HINSTANCE hInstance, HWND hWnd,
		vector<BufType> pInitialDataSet,
		ID3D11Buffer* pInputBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAVOutput,
		ID3D11UnorderedAccessView* ppRemainderOutputView,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);

	HRESULT SimulateStandardOnSelectedImage(HINSTANCE hInstance,
		HWND hWnd, 
		vector<PixelObjectGrayScale> pInputPixelDataArray,
		vector<CStandardDeviationImageOutputType> *ppImageDeviationOutputVar);
	HRESULT SimulateStandardDeviationOnSelectedImageOnCPU(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObjectGrayScale> pInputPixelDataArray,
		float *ppStandardDeviationVar);
	HRESULT InitializeStandardDeviationForImageResources(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObjectGrayScale> pInputDataBuffer,
		CBStandardDeviation ppConstantBufferCB,
		ID3D11ComputeShader** ppGaussianBlurComputeShader,
		ID3D11Buffer** ppGaussianBlurConstantBuffer,
		ID3D11Buffer** ppInputBufferGaussianBlur,
		ID3D11ShaderResourceView** ppGaussianBlurSRV,
		ID3D11Buffer** ppOutputBufferGaussianBlur,
		ID3D11UnorderedAccessView** ppOutputBufferForOutcome,
		ID3D11Buffer** ppReadbackDataBufferOutcome);

	HRESULT RunStandardDeviationForImagesCS(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObjectGrayScale> pImageArrayOne,
		CBStandardDeviation pConstantBufferCB,
		ID3D11Buffer* pConstantBuffer,
		ID3D11Buffer* pInputBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV, UINT pNumberOfSRV, UINT pNumberOfUAV, UINT X, UINT Y, UINT Z);

	HRESULT SimulateInterestPointDetectorCoordinates(HINSTANCE hInstance,
		HWND hWnd,
		float pXPositionDivisor,
		float pYPositionDivisor,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectInterestPointCoordinates> *ppPixelObjectGrayScale);

	HRESULT SimulateGaussianFilteringOnSelectedImage2D(HINSTANCE hInstance,
		HWND hWnd, vector<PixelObjectGrayScale> pInputImage, 
		float pi_var, float pStandardDeviationVar,
		vector<GaussianOutputVar> *ppGaussianBlurOutputVar);
	HRESULT InitializeGaussianFilteringResources(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObjectGrayScale> pInputDataBuffer,
		CBGaussianBlurConstantBuffer pConstantBufferCB,
		ID3D11ComputeShader** ppGaussianBlurComputeShader,
		ID3D11Buffer** pGaussianBlurConstantBuffer,
		ID3D11Buffer** ppInputBufferGaussianBlur,
		ID3D11ShaderResourceView** ppGaussianBlurSRV,
		ID3D11Buffer** ppOutputBufferGaussianBlur,
		ID3D11UnorderedAccessView** ppOutputBufferForOutcome,
		ID3D11Buffer** ppReadbackDataBufferOutcome);
	HRESULT RunGaussianFilteringComputeShaderCS(HINSTANCE hInstance, HWND hWnd,
		vector<PixelObjectGrayScale> pImageArrayOne,
		CBGaussianBlurConstantBuffer pConstantBufferCB,
		ID3D11Buffer* pConstantBuffer,
		ID3D11Buffer* pInputBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);


	HRESULT SimulateCannyEdgeDetectionFilteringOnSelectedImage2D(HINSTANCE hInstance,
		HWND hWnd, vector<PixelObjectGrayScale> pInputImage,
		float pi_var, float pStandardDeviationVar,
		vector<CannyOutputVar> *ppGaussianBlurOutputVar);
	HRESULT InitializeCannyEdgeDetectionFilteringResources(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObjectGrayScale> pInputDataBuffer,
		CBGaussianBlurConstantBuffer pConstantBufferCB,
		ID3D11ComputeShader** ppGaussianBlurComputeShader,
		ID3D11Buffer** pGaussianBlurConstantBuffer,
		ID3D11Buffer** ppInputBufferGaussianBlur,
		ID3D11ShaderResourceView** ppGaussianBlurSRV,
		ID3D11Buffer** ppOutputBufferGaussianBlur,
		ID3D11UnorderedAccessView** ppOutputBufferForOutcome,
		ID3D11Buffer** ppReadbackDataBufferOutcome);

	HRESULT RunCannyFilteringComputeShaderCS(HINSTANCE hInstance, HWND hWnd,
		vector<PixelObjectGrayScale> pImageArrayOne,
		CBGaussianBlurConstantBuffer pConstantBufferCB,
		ID3D11Buffer* pConstantBuffer,
		ID3D11Buffer* pInputBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);

	HRESULT ApplyGaussianFilterToImage(HINSTANCE hInstance,
		HWND hWnd,
		float pStandardDeviationVar,
		vector<PixelObjectGrayScale> pInputImage,
		vector<GaussianOutputVar> *ppGaussianBlurInputVar);

	HRESULT InitializeApplyGaussianFilteringResources(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObjectGrayScale> pInputDataBuffer,
		vector<GaussianOutputVar> pFilteredArrayVar,
		ID3D11ComputeShader** ppGaussianBlurComputeShader,
		ID3D11Buffer** ppInputBufferGaussianBlur,
		ID3D11Buffer** pGaussianBlurInputBuffer,
		ID3D11ShaderResourceView** ppInputImageSRV,
		ID3D11ShaderResourceView** ppGaussianBlurSRV,
		ID3D11Buffer** ppOutputBufferGaussianBlur,
		ID3D11UnorderedAccessView** ppOutputBufferForOutcome,
		ID3D11Buffer** ppReadbackDataBufferOutcome);
	HRESULT RunApplyGaussianFilteringComputeShaderCS(HINSTANCE hInstance, HWND hWnd,
		vector<PixelObjectGrayScale> pImageArrayOne,
		vector<GaussianOutputVar> ppGaussianOutputArray,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11ShaderResourceView* pShaderResourceView1,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);


	HRESULT SimulateGaussianFilteringOnSelectedImageOneDimension(HINSTANCE hInstance,
		HWND hWnd, vector<PixelObjectGrayScale> ppPixeInputArray,
		float pi_var, float pStandardDeviationVar,
		vector<GaussianOutputVar> *ppGaussianOutputVarXValues,
		vector<GaussianOutputVar> *ppGaussianBlurOutputVar);



	HRESULT SimulateGaussianFilteringOnSelectedImage1D(HINSTANCE hInstance,
		HWND hWnd, vector<PixelObjectGrayScale> ppPixeInputArray,
		float pi_var, float pStandardDeviationVar,
		vector<GaussianOutputVar> *ppGaussianBlurOutputVar);
	HRESULT InitializeGaussianFilteringResources1D(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObjectGrayScale> pInputDataBuffer,
		CBGaussianBlurConstantBuffer pConstantBufferCB,
		ID3D11ComputeShader** ppGaussianBlurComputeShader,
		ID3D11Buffer** pGaussianBlurConstantBuffer,
		ID3D11Buffer** ppInputBufferGaussianBlur,
		ID3D11ShaderResourceView** ppGaussianBlurSRV,
		ID3D11Buffer** ppOutputBufferGaussianBlur,
		ID3D11UnorderedAccessView** ppOutputBufferForOutcome,
		ID3D11Buffer** ppReadbackDataBufferOutcome);
	HRESULT RunGaussianFilteringComputeShaderCSX(HINSTANCE hInstance, HWND hWnd,
		vector<PixelObjectGrayScale> pImageArrayOne,
		CBGaussianBlurConstantBuffer pConstantBufferCB,
		ID3D11Buffer* pConstantBuffer,
		ID3D11Buffer* pInputBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);

	HRESULT SimulateGaussianFilteringOnSelectedImage1DY(HINSTANCE hInstance,
		HWND hWnd, vector<GaussianOutputVar> pGaussianBlurInputY,
		float pi_var, float pStandardDeviationVar,
		vector<GaussianOutputVar> *ppGaussianBlurOutputVar);
	HRESULT InitializeGaussianFilteringResources1DY(HINSTANCE hInstance,
		HWND hWnd,
		vector<GaussianOutputVar> pInputDataBuffer,
		CBGaussianBlurConstantBuffer pConstantBufferCB,
		ID3D11ComputeShader** ppGaussianBlurComputeShader,
		ID3D11Buffer** pGaussianBlurConstantBuffer,
		ID3D11Buffer** ppInputBufferGaussianBlur,
		ID3D11ShaderResourceView** ppGaussianBlurSRV,
		ID3D11Buffer** ppOutputBufferGaussianBlur,
		ID3D11UnorderedAccessView** ppOutputBufferForOutcome,
		ID3D11Buffer** ppReadbackDataBufferOutcome);
	HRESULT RunGaussianFilteringComputeShaderCSY(HINSTANCE hInstance, HWND hWnd,
		vector<GaussianOutputVar> pImageArrayOne,
		CBGaussianBlurConstantBuffer pConstantBufferCB,
		ID3D11Buffer* pConstantBuffer,
		ID3D11Buffer* pInputBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);







	HRESULT ConvertNumberToBinaryValueAndComputeButterFly(HINSTANCE hInstance, HWND hWnd, UINT pNumberToConvert, vector<UINT> *ppDivisionSteps, vector<UINT> *pRemaindersBeforeReversal, vector<UINT> *pRemaindersAfterReversal, UINT *pReversedNumber);

	HRESULT ConvertAnArrayOfNumbersToButterFlyOnCPU(HINSTANCE hInstance, HWND hWnd, vector<UINT> pInputDataArray, vector<UINT> *pReverseOrderValues);
	HRESULT MixArrayIntoOddAndEvenElements(HINSTANCE hInstance, HWND hWnd, vector<UINT> pInputDataArray, vector<UINT> *pReverseOrderValues);
	HRESULT SegmentImageIntoRectangularSectors(HINSTANCE hInstance, HWND hWnd, LPCWSTR pFileName, float pSampleWidth, float pSampleHeight, vector<ImageSectorObject> *ppImageSectorObjectObjectCollection);
	HRESULT SimulateFastFourierTransformAlgorithmForward(HINSTANCE hInstance,
		HWND hWnd,
		UINT pAmountOfSamples,
		vector<BufType> pInputDataArray0,
		vector<BufType> *pOutputDataArray);

	HRESULT RunComputeShaderFFTOne(vector<UINT> pConstantDataArr,
		ID3D11ComputeShader* ppComputeShaderFFT,
		ID3D11Buffer* pConstantBuffer,
		ID3D11Buffer* ppBufferOneFFT,
		ID3D11ShaderResourceView* ppSRVOneFFT,
		ID3D11Buffer* ppOutputBuffer,
		ID3D11UnorderedAccessView* ppOutputView,
		ID3D11Buffer* ppReadbackBuffer);
	HRESULT PerformFFTSimulation(HINSTANCE hInstance, HWND hWnd, vector<double> pInputData0,
    vector<float> pTimeArray, vector<FLOAT> *pOutputData);
	vector<UINT> pArrayOneValues;
	vector<UINT> pArrayTwoValues;
	BOOL LocateImageFile(HWND hWnd, LPWSTR pszFileName, DWORD cbFileName);
	BufType floatVarArrayA[NUM_ELEMENTS];
	BufType floatVarArrayB[NUM_ELEMENTS];
	BufType g_vBuf0[NUM_ELEMENTS];
	BufType g_vBuf1[NUM_ELEMENTS];
	HRESULT InitializeComputationsDevice(HWND hWnd, int adapterIndex);
	void CreateNewComputationTask(LPCSTR fName, LPCSTR dataSet, LPCSTR output); /*******Создай Новую Задачу**************/
	ComputationState GetTaskState(LPCSTR taskName); /*******Проверь Состояние Задачи**************/
	HRESULT SimulateSortingAlgorithm(HINSTANCE hInstance, HWND hWnd, vector<UINT> pInputVector, vector<UINT> *pOutputVector);
	HRESULT SimulateLinearRegressionAlgorithm(HWND hWnd);
	const std::string OutputCurrentTime();
	void ExecuteComputeShader(ID3D11DeviceContext* pd3dImmediateContext,
		ID3D11ComputeShader* pComputeShader,
		UINT nNumViews, ID3D11ShaderResourceView** pShaderResourceViews,
		ID3D11Buffer* pCBCS, void* pCSData, DWORD dwNumDataBytes,
		ID3D11UnorderedAccessView* pUnorderedAccessView,
		UINT X, UINT Y, UINT Z);
	HRESULT SimulateMatrixMultiplicationA(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateMatrixMultiplicationW(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateRandomAlgorithmA(HINSTANCE hInstance, HWND hWND);
	ID3D11ComputeShader* m_pArrayAddititionAlgorithm;
	ID3D11ComputeShader* m_pComputeShaderBitonic;
	ID3D11ComputeShader* m_pComputeShaderTranspose;
	ID3D11ComputeShader* pGasDynamicsComputeShader;




	ID3D11Buffer* m_pBitonicConstantBuffer;
	ID3D11Buffer* m_pTemporaryBuffer;
	ID3D11Buffer* m_pFirstBuffer;
	ID3D11ShaderResourceView* m_pFirstBufferSRV;
	ID3D11UnorderedAccessView* m_pFirstBufferUAV;
	ID3D11Buffer* pAtomSpeedBufferW;
	ID3D11Buffer* pAtomTempDataBuff;
	ID3D11Buffer* pGasDynamicsConstantBuffer;
	ID3D11ShaderResourceView* pAtomTempBuffSRV;
	ID3D11UnorderedAccessView* pGasSpeedOutputUAV;

	vector<ID3D11Buffer*> 	ppPrecomputedBuffers;
	ID3D11Buffer* pMatrixObjectBuffer0;
	ID3D11Buffer* pMatrixObjectBuffer1;
	ID3D11Buffer* pMatrixComputeOuptutDataBuffer;
	ID3D11ShaderResourceView* pMatrixBufferZeroSRV;
	ID3D11ShaderResourceView* pMatrixBufferOneSRV;
	ID3D11UnorderedAccessView* pMatrixOutputBufferUAV;
	ID3D11Buffer* pMatrixComputeReadbackBuffer;
	ID3D11ComputeShader* pMatixMultiplicationComputeShader;

	ID3D11Buffer* pBufferOneFFT;
	ID3D11Buffer* pTemporaryOutputBufferFFT;
	ID3D11UnorderedAccessView* pUnorderedAccessViewFFT;
	ID3D11UnorderedAccessView* pOutputUnorderededAccessViewFFT;

	ID3DBlob* pMemoryDataBlob;
	float* pConstantData[3];
	ID3D11Buffer* m_pSecondBuffer;
	ID3D11Buffer* m_pResultBuffer;
	ID3D11ShaderResourceView* m_pResultBufferSRV;
	ID3D11ShaderResourceView* m_pSecondBufferSRV;
	ID3D11UnorderedAccessView* m_pSecondBufferUAV;
	ID3D11Buffer* m_pReadBackBuffer;
	ID3D11UnorderedAccessView* m_pResultBufferUAV;
	ID3D11ShaderResourceView* pAdditionAlgorithmsSRV[2];
	ID3D11UnorderedAccessView* pUnorderedAccessView;
	ID3D11ComputeShader* ppGasDynamicsComputeShaderW;
	ID3D11UnorderedAccessView* ppGasSpeedDynamicsUAV;
	HRESULT SimulateAddition(HINSTANCE hInstance, HWND hWnd);
	ID3D11Buffer* pChannelHeightConstantBuffer;
	static DirectXStandardAlgorithmExecutor* Instance(HINSTANCE hInstance, HWND hWnd);
	HRESULT ExecuteGasDynamicsComputeShaderAlgorithm(HINSTANCE hInstance, HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11Buffer* pConstantBuffer, void* pCSDData,
		ID3D11ShaderResourceView* pAtomTempBuffSRV1,  ID3D11UnorderedAccessView* pSpeedUAV1,
		int xVal, int yVal, int zVal);
	HRESULT ComputeGasSpeed(HINSTANCE hInstance, HWND hWnd);
	HRESULT CreateGasDynamicsConstantBuffer(HINSTANCE hInstance, HWND hWnd, GasDynamicsConstantBuffer pBuffer, ID3D11Buffer* pGasDynamicsConstantBuffer);
	HRESULT CreateStructuredBufferForTemperatureValues(HINSTANCE hInstance, HWND hWnd, vector<TemperatureStructure> pTempRanageBuffer, ID3D11Buffer* ppGasTempBuffer,
		ID3D11ShaderResourceView* ppGasDynamicsTempBufferSRV);
	HRESULT CreateStructuredBufferForSpeedValues(HINSTANCE hInstance, HWND hWnd, ID3D11Buffer* ppSpeedValuesBuffer, ID3D11UnorderedAccessView* ppSpeedResultsBufferUAV);
	HRESULT PerformFFTSimulation(HINSTANCE hInstance, HWND hWnd, vector<BDataComplexNumber> ppDataObject, vector<BDataComplexNumber> *ppOutput);
	HRESULT SimulateFastFourierTransformBackward(HINSTANCE hInstance, HWND hWnd, vector<BYTE*> pDataObjectToCompute, vector<BYTE*> *ppDataObjectOutput,
		ID3D11Buffer* ppBufferOneFFT, ID3D11UnorderedAccessView* ppUAVOneFFT, ID3D11Buffer* ppOutputBuffer, ID3D11UnorderedAccessView* ppOutputView);
	BDataComplexNumber *fourrier(BDataComplexNumber *a, int n);
	BDataComplexNumber *invfourrier(BDataComplexNumber *a, int n);

	HRESULT SimulateSimpleGPUAddition(HINSTANCE hInstance, HWND hWnd,
		vector<UINT> pArrayOneData, vector<UINT> pArrayTwoData, vector<UINT> *pOutputArrayData);
	HRESULT SimulateSimpleGPUSubtraction(HINSTANCE hInstance, HWND hWnd,
		vector<UINT> pArrayOneData, vector<UINT> pArrayTwoData, vector<BufType> *pOutputArrayData);
	HRESULT InitializeArraySubtractionResources(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppInputBuffer2,
		ID3D11Buffer** ppOutputBuffer,
		vector<UINT> pArrayOneVals,
		vector<UINT> pArrayTwoVals,
		ID3D11ShaderResourceView** ppInputSRV1,
		ID3D11ShaderResourceView** ppInputSRV2,
		ID3D11UnorderedAccessView** ppOutputUAV,
		ID3D11Buffer** ppReadbackBuffer,
		vector<BufType> *ppOutputArr0,
		vector<BufType> *ppOutputArr1);
    HRESULT RunComputeShaderAlgorithmW(HINSTANCE hInstance, HWND hWnd, vector<BData> pValuesData, ID3D11ComputeShader* pComputeShader, ID3D11Buffer* pConstBufferData, ID3D11ShaderResourceView* pSRV, ID3D11UnorderedAccessView* pConnectableUAV);

	std::vector<UINT> dataArray;
	std::vector<UINT> resultsArray;


	ID3D11ComputeShader* pExponentialComputeShader;
	ID3D11Buffer* pExponentialBufferInput;
	ID3D11Buffer* pExponentialBufferOutput;
	ID3D11ShaderResourceView* pExponentialBufferInputSRV;
	ID3D11UnorderedAccessView* pExponentialBufferOutUAV;
	ID3D11Buffer* pExpoentialReadbackBuffer;

	//exponential Function Vars
	ID3D11ComputeShader* pExponentialComputeShaderCS;
	ID3D11Buffer* pInputBufferForExpFunction;
	ID3D11Buffer* pOutputBufferForExpFunction;
	ID3D11ShaderResourceView* pExponentinalFunctionSRV0;
	ID3D11ShaderResourceView* pExponentinalFunctionSRV1;

	ID3D11UnorderedAccessView* pExponentialFunctionUAV0;
	ID3D11UnorderedAccessView* pExponentialFuntionUAV1 = nullptr;
	ID3D11Buffer* pReadBackBuffer;
	ID3D11UnorderedAccessView* ppUnorderedAccessViews;
	ID3D11ShaderResourceView* ppShaderResourceViews;


	HRESULT InitializeParallelReductionResources(HINSTANCE hInstance,
		HWND hWnd,
		vector<BufType> ppArrayToCompute,
		UINT pAmountOfThreads,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppParallelReductionCB,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppInputSRV1,
		ID3D11UnorderedAccessView** ppOutputUAV,
		ID3D11Buffer** ppReadbackBuffer);



	HRESULT InitializeArrayAdditionResources(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppInputBuffer2,
		ID3D11Buffer** ppOutputBuffer,
		vector<UINT> pArrayOneVals,
		vector<UINT> pArrayTwoVals,
		ID3D11ShaderResourceView** ppInputSRV1,
		ID3D11ShaderResourceView** ppInputSRV2,
		ID3D11UnorderedAccessView** ppOutputUAV,
		ID3D11Buffer** ppReadbackBuffer,
		vector<BufType> *ppOutputArr0,
		vector<BufType> *ppOutputArr1);

	HRESULT InitalizeSortingAlgorithmResources(HINSTANCE hInstance, HWND hWnd, vector<UINT> pInputDataVector, ID3D11ComputeShader** pComputeShaderSort, ID3D11ComputeShader** pMatrixTranspose, ID3D11Buffer** pBitonicConstantBuffer,
		ID3D11Buffer** pFirstBuffer, ID3D11Buffer** pSecondBuffer, ID3D11ShaderResourceView** pShaderSRV0, ID3D11ShaderResourceView** pShaderSRV1,
		ID3D11UnorderedAccessView** ppUAV0, ID3D11UnorderedAccessView** ppUAV1,
		ID3D11Buffer** ppReadBackBuffer,
		vector<BufType> *ppBufferDataArraySet);
	void GPUSort(HINSTANCE hInstance, HWND hWnd, ID3D11ComputeShader* pComputeShaderSort, ID3D11ComputeShader* pMatrixTranspose, ID3D11Buffer* pBitonicConstantBuffer,
		ID3D11Buffer* pFirstBuffer, ID3D11Buffer* pSecondBuffer, ID3D11ShaderResourceView* pShaderSRV0, ID3D11ShaderResourceView* pShaderSRV1,
		ID3D11UnorderedAccessView* ppUAV0, ID3D11UnorderedAccessView* ppUAV1,
		ID3D11Buffer* pReadBackBuffer,
		vector<UINT> dataArray, vector<UINT> *pSortedData);



	HRESULT SimulateDiscreteFastFourierTransform(HINSTANCE hInstance, HWND hWnd, vector<BYTE*> pDataObjectToCompute, vector<BYTE**> pDataObjectOutput);
	HRESULT SimulateReverseFastFourierTransform(HINSTANCE hInstance, HWND hWnd, vector<BYTE*> pDataObjectToCompute, vector<BYTE**> pDataObjectOutput);





	HRESULT PerformExponentialFunctionSimulation(HINSTANCE hInstance, HWND hWnd, float pCoefficientParam, float pFreeCoefficient, vector<BufType> pInputData, vector<BufType> *pOutputData);
	HRESULT SimulateExponentionaFunction(HINSTANCE hInstance, HWND hWnd, float pInputCoefficient, float pFreeCoefficient, vector<BufType> pInputData, vector<BufType> *pOutputBuffer);
	HRESULT InitializeExponentialFunctionResources(HINSTANCE hInstance, HWND hWnd,
		UINT pAmountOfSamples, 
		ID3D11ComputeShader** pComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** pInputBuffer,
		ID3D11Buffer** pOutputBuffer, 
		ID3D11ShaderResourceView** ppSRV1, 
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);
	HRESULT RunExponentialFunction(HINSTANCE hInstance, HWND hWnd,
		vector<BufType> pInitialDataSet,
		ConstantBufferExponential pConstantBufferData,
		ID3D11Buffer* pInputBuffer,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV0,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);


	HRESULT PerformQuadraticFunctionSimulation(HINSTANCE hInstance, HWND hWnd, float pCoeffiicentA, float pCoefficientB, float pCoefficientC, vector<float> pInputData, vector<BufType> *pOutputData);
	HRESULT SimulateQuadraticFunction(HINSTANCE hInstance, 
            HWND hWnd,
            float pCoeffcientA,
            float pCoefficientB,
            float pCoefficentC,
            vector<InputStructureForGraph> pInputData,
            vector<OutputStructureForGraph> *pOutputBuffer);
	HRESULT InitializeQuadraticFunctionResources(HINSTANCE hInstance,
		HWND hWnd,
		ConstantBufferQuadratic pConstantBufferQuadratic,
		vector<InputStructureForGraph> pInitialDataSet,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);
	HRESULT RunQuadraticFunction(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer* pConstantBuffer,
		ID3D11Buffer* pInputBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);


	HRESULT InitializeQuadraticFunctionResources(HINSTANCE hInstance, HWND hWnd,
		vector<BufType> pInitialDataSet,
		ID3D11ComputeShader** pComputeShaderObject,
		ID3D11Buffer** pConstantBuffer,
		ID3D11Buffer** pInputBuffer,
		ID3D11Buffer** pOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);
	HRESULT RunQuadraticFunction(HINSTANCE hInstance, HWND hWnd,
		vector<BufType> pInitialDataSet,
		ConstantBufferQuadratic pConstantData,
		ID3D11Buffer* pConstantBuffer,
		ID3D11Buffer* pInputBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);
	
	HRESULT PerformFFTSimulationData(HINSTANCE hInstance, HWND hWnd, vector
		<float> ppDataObject, vector <float> *ppOutput);

	void SetConstants(ID3D11Buffer* pConstantBufer, UINT iLevel, UINT iLevelMask, UINT iWidth, UINT iHeight);
	void GPUSort(vector<UINT> pDataArray, vector<UINT> *pSortedData);
	void CPUSort(vector<UINT> pDataArray, vector<UINT> *pSortedData);

	HRESULT SimulateCosineFunctionBehaviour(HINSTANCE hInstance,
		HWND hWnd,
		float pCosineFunctiooCoefficient,
		float pEmptyCoeffient,
		vector<InputStructureForGraph> pInputData,
		vector<OutputStructureForGraph> *pOutputBuffer);
	HRESULT InitializeCosineFunctionResources(HINSTANCE hInstance,
		HWND hWnd,
		float pCosineFunctiooCoefficient,
		float pEmptyCoeffient,
		vector<InputStructureForGraph> pInputData,
		ID3D11ComputeShader** pComputeShaderObject,
		ID3D11Buffer** pConstantBuffer,
		ID3D11Buffer** pInputBuffer,
		ID3D11Buffer** pOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunCosineFunctionShaderAlgorithm(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);

	HRESULT MultiplyImageByImageInGrayScaleMode(HINSTANCE hInstance, vector<PixelObjectGrayScale> pImageOne, vector<PixelObjectGrayScale> pImageTwo,
		vector<PixelObjectGrayScale> *pOutputImage,
	 UINT *pIntensityMode);
	HRESULT InitializeImageMultiplicationFunctionResources(HINSTANCE hInstance,
		HWND hWnd,
		UINT pAmountOfSamples,
		vector<PixelObjectGrayScale> pImageArrayOne,
		vector<PixelObjectGrayScale> pImageArrayTwo,
		ID3D11ComputeShader** pComputeShaderObject,
		ID3D11Buffer** pConstantBuffer,
		ID3D11Buffer** pInputBuffer,
		ID3D11Buffer** pOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);
	HRESULT RunImageMultiplicationFunctionShaderAlgorithm(HINSTANCE hInstance, HWND hWnd,
		vector<PixelObjectGrayScale> pImageArrayOne,
		vector<PixelObjectGrayScale> pImageArrayTwo,
		CBCosineFunctionBuffer pConstantData,
		ID3D11Buffer* pConstantBuffer,
		ID3D11Buffer* pInputBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);

	HRESULT SimulateSineFunctionBehaviour(HINSTANCE hInstance,
		HWND hWnd,
		float pCosineFunctionCoefficient,
		float pEmptyCoeffient,
		vector<InputStructureForGraph> pInputData,
		vector<OutputStructureForGraph> *pOutputBuffer);
	HRESULT InitializeSineFunctionResources(HINSTANCE hInstance,
		HWND hWnd,
		float pCosineFunctiooCoefficient,
		float pEmptyCoeffient,
		vector<InputStructureForGraph> pInputData,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);
	HRESULT RunSineFunctionShaderAlgorithm(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);



	HRESULT SortImageSectorsByBrigtness(HINSTANCE hInstance, HWND hWnd,
		LPCWSTR pFileName,
		int pSectorWidth,
		int pSectorHeight,
		vector<vector<PixelObject>> *pImageSectorList);

	HRESULT CompareTwoImageRegionsWithinOneFile(HINSTANCE hInstance, HWND hWnd,
		LPCWSTR pFileName, int pImageXPosition0, int pImageYPosition0, int pWidth0, int pHeight0,
		int pImageXPosition1, int pImageYPosition1, int pWidth1, int pHeight1,
		vector<PixelObject> *pDifferencePixels);

	HRESULT CompareImageRegionInCoupleOFiles(HINSTANCE hInstance, HWND hWnd,
		LPCWSTR pFileName1, LPCWSTR pFileName2, int pImageXPosition0, int pImageYPosition0, int pWidth0, int pHeight0,
		vector<PixelObject> *pDifferencePixels);

	HRESULT ComputeImageDerivativeFunctionForwardForXValuesGaussian(HINSTANCE hInsance, HWND hWnd,
		vector<GaussianOutputVar> pGrayScalePixelList,
		float pImageWidth,
		float pImageHeight,
		float pDeltaVar,
		vector<CImageDerivativeClass> *ppImageDerivativeObjectListForX);

	// Sort Container by name function
	// Sort Container by name function





	HRESULT ComputeImageDerivativeFunctionForwardForYValuesGaussian(HINSTANCE hInsance, HWND hWnd,
		vector<GaussianOutputVar> pGrayScalePixelList,
		float pImageWidth,
		float pImageHeight,
		float pDeltaVar,
		vector<CImageDerivativeClass> *ppImageDerivativeObjectListForYValues);


	HRESULT ComputeSecondOrderDerivativeFoxXValues(HINSTANCE hInstance,
		HWND hWnd, vector<CImageDerivativeClass> ppImageDerivativeObjectListForX,
		vector<CImageDerivativeClass> *ppSecondOrderDerivative);

	HRESULT ComputeSecondOrderDerivativeFoxYValues(HINSTANCE hInstance,
		HWND hWnd, vector<CImageDerivativeClass> ppImageDerivativeObjectListForY,
		vector<CImageDerivativeClass> *ppSecondOrderDerivative);

	HRESULT RunApplyGaussianFilteringComputeShaderCS(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11ShaderResourceView* pShaderResourceView1,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV, UINT X, UINT Y, UINT Z);

	HRESULT ComputeImageFunctionDerivativesFinalGPU(HINSTANCE hInstance,
		HWND hWnd, vector<CImageDerivativeClassStruct> ppImageDerivativeObjectListForX,
		vector<CImageDerivativeClass> ppImageDerivativeObjectListForY,
		vector<CImageFullDerivativeClassStruct> *pFullDerivativesClass);

	HRESULT InitializeComputeFinalDerivativesResources(HINSTANCE hInstance,
		HWND hWnd, vector<CImageDerivativeClassStruct> ppImageDerivativeObjectListForX,
		vector<CImageDerivativeClassStruct> ppImageDerivativeObjectListForY,
		ID3D11ComputeShader** ppComputeDerivaticesCSShader,
        ID3D11Buffer** ppBufferForArrayOne,
        ID3D11Buffer** ppBufferForArrayTwo,
        ID3D11Buffer** ppComputeImageDerivativeOutput,
        ID3D11ShaderResourceView** ppShaderResourceView0,
		ID3D11ShaderResourceView** ppShaderResourceView1,
		ID3D11UnorderedAccessView** ppOutputDerivativesUAV,
        ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunComputeOfDerivativeValues(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11ShaderResourceView* pShaderResourceView1,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV, UINT X, UINT Y, UINT Z);






	HRESULT ComputeImageFunctionDerivativesFinal(HINSTANCE hInstance,
		HWND hWnd, vector<CImageDerivativeClass> ppImageDerivativeObjectListForX,
		vector<CImageDerivativeClass> ppImageDerivativeObjectListForY,
		vector<CImageFullDerivativesClass> *pFullDerivativesClass);


	HRESULT SubtractImageData(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObject> pPixelArrayOne,
		vector<PixelObject> pPixelArrayTwo,
		UINT pXPosition, UINT pYPosition,
		int pImageWidth, int pImageHeight, vector<PixelObject> *ppPixelDataOutput);




	HRESULT SimulaeImageSubtractionGrayScaleOnGPU(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObjectGrayScale> pixelArrayOne,
		vector<PixelObjectGrayScale> pixelArrayTwo,
		vector<PixelObjectGrayScale> *pOuputResultArray);
	HRESULT InitializeImageGrayScaleSubtractionResources(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<PixelObjectGrayScale> pixelArrayOne,
		vector<PixelObjectGrayScale> pixelArrayTwo,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppInputBuffer2,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11ShaderResourceView** ppSRV2,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);
	HRESULT RunGrayScaleImageSubtractionFunctionShaderAlgorithm(HINSTANCE hInstance, HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11ShaderResourceView* pShaderResourceView1,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);





	HRESULT SimulaeImageSubtractionOnGPU(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObject> pixelArrayOne,
		vector<PixelObject> pixelArrayTwo,
		vector<PixelObject> *pOuputResultArray);
	HRESULT InitializeImageSubtractionFunctionResources(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<PixelObject> pixelArrayOne,
		vector<PixelObject> pixelArrayTwo,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppInputBuffer2,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11ShaderResourceView** ppSRV2,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);
	HRESULT RunImageSubtractionFunctionShaderAlgorithm(HINSTANCE hInstance, HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11ShaderResourceView* pShaderResourceView1,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);


	HRESULT ConvertImageDataToHSI(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObject> pixelArrayOne,
		vector<PixelObjectHSI> *pOuputResultArray);
	HRESULT InitializeImageHSIConversionResources(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<PixelObject> pixelArrayOne,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);
	HRESULT RunImageToHSIConversion(HINSTANCE hInstance, HWND hWnd,
		vector<PixelObject> pPixelObjectArrayImageOne,
		ID3D11Buffer* pInputBufferOne,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);



	HRESULT SearchForBordersWithinImages(HINSTANCE hInstance, HWND hWnd,
		vector<PixelObjectGrayScale> pImageOneToTest,
		vector<PixelObjectGrayScale> pImageTwoToTest,
		vector<BorderOutputStructure> *ppBorderObjectCollections);
	HRESULT InitializeBorderSearchFunctionResources(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<PixelObjectGrayScale> pixelArrayOne,
        vector<PixelObjectGrayScale> pixelArrayTwo,
		ID3D11Buffer** ppInputBuffer1,
        ID3D11Buffer** ppInputBuffer2,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11ShaderResourceView** ppSRV2,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);
	HRESULT RunBorderSearchFunction(HINSTANCE hInstance, HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
        ID3D11ShaderResourceView* pShaderResourceView1,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);


    

	HRESULT CheckIfImagesAreIdentical(HINSTANCE hInstance,
		HWND hWnd,
		UINT pImageWidth,
		UINT pImageHeight,
		vector<PixelObjectGrayScale> pixelArrayOne,
		vector<PixelObjectGrayScale> pixelArrayTwo,
		int *pAmountOfidenticalPixels,
		int *pAmountODifferentPixels,
		vector<OutputPixelBuffer> *ppOutputPixelData,
		vector<PixelObjectGrayScale> *ppIdenticalPixelBuffer,
		vector<PixelObjectGrayScale> *ppDifferentPixelBuffer,
		float *ppImageOneIntensitySum,
		float *ppImageOneAverage,
		float *ppImageTwoIntensitySum,
		float *ppImageTwoAverage,
		float *ppOutputArrayIntensitySum,
		float *ppOutputArrayAverage,
		float *ppIdenticalPixelsIntensitySum,
		float *ppIdenticalPixelsAverage,
		float *ppDifferentPixelsIntensitySum,
		float *ppDifferentPixelsAverage,
		BOOL *pAreIdenticalOutput);

	HRESULT InitializeIdentityVerificationResources(HINSTANCE hInstance,
		HWND hWnd,
		float pImageWidth,
		float pImageHeight,
		vector<PixelObjectGrayScale> pixelArrayOne,
		vector<PixelObjectGrayScale> pixelArrayTwo,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppInputBufferImageOne,
		ID3D11Buffer** ppInputBufferImageTwo,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11Buffer** ppOutputBufferIdenticalPixels,
		ID3D11Buffer** ppOutputBufferDifferentPixels,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11ShaderResourceView** ppSRV2,
		ID3D11UnorderedAccessView** ppUAVObjectOutput,
		ID3D11UnorderedAccessView** ppUAVObjectIdentical,
		ID3D11UnorderedAccessView** ppUAVObjectDifferent,
		ID3D11Buffer** ppReadbackBufferOutput,
		ID3D11Buffer** ppReadbackBufferIdentical,
		ID3D11Buffer** ppReadBackBufferDifferent);
	

	HRESULT RunImageIdentityVerificationOperation(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* ppSRV1,
		ID3D11ShaderResourceView* ppSRV2,
		ID3D11UnorderedAccessView* ppUAVObjectOutput,
		ID3D11UnorderedAccessView* ppUAVObjectIdentical,
		ID3D11UnorderedAccessView* ppUAVObjectDifferent, 
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);



	HRESULT CheckIfImagesAreIdenticalRGB(HINSTANCE hInstance,
		HWND hWnd,
		UINT pImageWidth,
		UINT pImageHeight,
		vector<PixelObject> pixelArrayOne,
		vector<PixelObject> pixelArrayTwo,
		int *pAmountOfidenticalPixels,
		int *pAmountODifferentPixels,
		vector<OutputPixelBufferRGB> *ppOutputPixelData,
		vector<PixelObject> *ppIdenticalPixelBuffer,
		vector<PixelObject> *ppDifferentPixelBuffer,
		float *ppImageOneIntensitySumRed,
		float *ppImageOneIntensitySumGreen,
		float *ppImageOneIntensitySumBlue,
		float *ppImageTwoIntensitySumRed,
		float *ppImageTwoIntensitySumGreen,
		float *ppImageTwoIntensitySumBlue,
		float *ppImageOutputIntensitySumRed,
		float *ppImageOutputIntensitySumGreen,
		float *ppImageOutputputIntensitySumBlue,
		float *ppImageIdenticalIntensitySumRed,
		float *ppImageIdenticalIntensitySumGreen,
		float *ppImageIdenticaltIntensitySumBlue,
		float *ppImageDifferentIntensitySumRed,
		float *ppImageDifferentIntensitySumGreen,
		float *ppImageDifferenttIntensitySumBlue,
		float *ppImageOneIntensityAverageRed,
		float *ppImageOneIntensityAverageGreen,
		float *ppImageOneIntensityAverageBlue,
		float *ppImageTwoIntensityAverageRed,
		float *ppImageTwoIntensityAverageGreen,
		float *ppImageTwoIntensityAverageBlue,
		float *ppImageOutputIntensityAverageRed,
		float *ppImageOutputIntensityAverageGreen,
		float *ppImageOutputputIntensityAverageBlue,
		float *ppImageIdenticalIntensityAverageRed,
		float *ppImageIdenticalIntensityAverageGreen,
		float *ppImageIdenticaltIntensityAverageBlue,
		float *ppImageDifferentIntensityAverageRed,
		float *ppImageDifferentIntensityAverageGreen,
		float *ppImageDifferenttIntensityAverageBlue,	
	BOOL *pAreIdenticalOutput);

	HRESULT InitializeIdentityVerificationResourcesRGB(HINSTANCE hInstance,
		HWND hWnd,
		float pImageWidth,
		float pImageHeight,
		vector<PixelObject> pixelArrayOne,
		vector<PixelObject> pixelArrayTwo,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppInputBufferImageOne,
		ID3D11Buffer** ppInputBufferImageTwo,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11ShaderResourceView** ppSRV2,
		ID3D11UnorderedAccessView** ppUAVObjectOutput,
		ID3D11Buffer** ppReadbackBufferOutput
	);


	HRESULT RunImageIdentityVerificationOperationRGB(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* ppSRV1,
		ID3D11ShaderResourceView* ppSRV2,
		ID3D11UnorderedAccessView* ppUAVObjectOutput,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);






	HRESULT InitializeBundleSplittingOperationRGB(HINSTANCE hInstance,
		HWND hWnd,
		float pImageWidth,
		float pImageHeight,
		vector<OutputPixelBufferRGB> pixelArrayOne,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppInputBufferImageOne,
		ID3D11Buffer** ppOutputBufferIdentical,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObjectOutputIdentical,
		ID3D11Buffer** ppReadbackBufferIdentical);

	HRESULT DetectMotionInRGBImage(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObject> pixelArrayOne,
		vector<PixelObject> pixelArrayTwo,
		vector<MotionPixelObjectRGB> *ppOutputPixelData,
		int *pAmountOfidenticalPixels,
		int *pAmountODifferentPixels,
		float *ppImageOneIntensitySumRed,
		float *ppImageOneIntensitySumGreen,
		float *ppImageOneIntensitySumBlue,
		float *ppImageTwoIntensitySumRed,
		float *ppImageTwoIntensitySumGreen,
		float *ppImageTwoIntensitySumBlue,
		float *ppImageOneAverageRed,
		float *ppImageOneAverageGreen,
		float *ppImageOneAverageBlue,
		float *ppImageTwoAverageRed,
		float *ppImageTwoAverageGreen,
		float *ppImageTwoAverageBlue,
		float *ppOutputArrayIntensitySumRed,
		float *ppOutputArrayIntensitySumGreen,
		float *ppOutputArrayIntensitySumBlue,
		float *ppOutputArrayIntensityAverageRed,
		float *ppOutputArrayIntensityAverageGreen,
		float *ppOutputArrayIntensityAverageBlue,

		BOOL *pAreImageIdenticalVar);

	HRESULT InitializeMotionDetectionResourcesRGB(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObject> pixelArrayOne,
		vector<PixelObject> pixelArrayTwo,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppInputBufferImageOne,
		ID3D11Buffer** ppInputBufferImageTwo,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11ShaderResourceView** ppSRV2,
		ID3D11UnorderedAccessView** ppUAVObjectOutput,
		ID3D11Buffer** ppReadbackBufferOutput);


	HRESULT RunMotionDetectiondOperationRGB(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* ppSRV1,
		ID3D11ShaderResourceView* ppSRV2,
		ID3D11UnorderedAccessView* ppUAVObjectOutput,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);






	HRESULT ConvertImageDataToGrayScale(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObject> ppPixelArrayInput,
		vector<PixelObjectGrayScale> *ppPixelObjectGrayScale);

	HRESULT InitializeImageGrayScaleConversionResources(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		vector<PixelObject> pixelArrayOne,
        ID3D11ComputeShader** ppComputeShaderObject,
    	ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);



	HRESULT ConvertImageDataToCMYK(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectCMYK> *ppPixelObjectGrayScale);

	HRESULT InitializeImageCMYKConversionResources(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		vector<PixelObject> pixelArrayOne,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunImageToCMYKConversion(HINSTANCE hInstance, HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);



	HRESULT ConvertImageDataToGrayScaleUsingInts(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObjectInt> ppPixelArrayInput,
		vector<PixelObjectGrayScaleInt> *ppPixelObjectGrayScale);

	HRESULT InitializeImageGrayScaleConversionResourcesUsingInts(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		vector<PixelObjectInt> pixelArrayOne,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunImageToGrayScaleConvertionInts(HINSTANCE hInstance, HWND hWnd,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);




	HRESULT ConvertImageDataToGrayScaleLuminosity(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObject> ppPixelArrayInput,
		vector<PixelObjectGrayScale> *ppPixelObjectGrayScale);

	HRESULT InitializeImageGrayScaleConversionResourcesLuminosity(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<PixelObject> pixelArrayOne,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);


	HRESULT RunImageToGrayScaleConvertion(HINSTANCE hInstance, HWND hWnd,
		vector<PixelObject> pPixelObjectArrayImageOne,
		ID3D11Buffer* pInputBufferOne,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);


	HRESULT ApplyBinaryClassifierForImagePixels(HINSTANCE hInstance,
		HWND hWnd,
		float pThresholdIntensityValueR,
		float pThresholdIntensityValueG,
		float pThresholdIntentityValueB,
		int pValidPixelClassifier,
		int pInvalidPixelClassifier,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectClassified> *ppPixelObjectGrayScale);
	

	HRESULT InitializeImageBinaryClassifierResources(HINSTANCE hInstance,
		HWND hWnd,
		CBBinaryClassifierConstantBuffer pConstantBufferStr,
		vector<PixelObject> pixelArrayOne,
		ID3D11ComputeShader** ppComputeShaderObject,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);
	HRESULT RunResultsRestorationFromBinaryClassifier(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader* pImageRestorationComputeShader,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ShaderResourceView* pImageRestorationSRV,
		ID3D11UnorderedAccessView* pImageRestorationUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV, UINT X, UINT Y, UINT Z);


	HRESULT RestoreValidRGBPixelFromClassifier(HINSTANCE hInstance,
		HWND hWnd,
		CBBinaryClassifierConstantBuffer pBinaryClassifierConstantBuffer,
		vector<OutputPixelObjectClassified> pPixelObjectGrayScale,
		vector<PixelObject> *pClassOnePixelArray);
	

	HRESULT InitializeRGBValidPixelImageRestorationResourcesFromClassifier(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelObjectClassified> pInputFilterGrayscaleVector,
		CBBinaryClassifierConstantBuffer pBinaryClassifierConstantBuffer,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBufferClassOne,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObjectOne,
		ID3D11Buffer** ppReadbackBufferOne);

	HRESULT InitializeInterestPointDetectorCoordinates(HINSTANCE hInstance,
		HWND hWnd,
		float pXPositionFactor,
		float pYPositionFactor,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<PixelObject> pixelArrayOne,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RestoreInvalidRGBPixelFromClassifier(HINSTANCE hInstance,
		HWND hWnd,
		CBBinaryClassifierConstantBuffer pConstantData,
		vector<OutputPixelObjectClassified> pPixelObjectGrayScale,
		vector<PixelObject> *pClassOnePixelArray);

	HRESULT	InitializeRGBInvalidPixelImageRestorationResourcesFromClassifier(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputPixelObjectClassified> pInputFilterGrayscaleVector,
		CBBinaryClassifierConstantBuffer pConstantData,
		ID3D11ComputeShader** ppImageRestorationComputeShader,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppImageRestorationInputBuffer,
		ID3D11Buffer** ppImageRestorationOutputBufferClassOne,
		ID3D11ShaderResourceView** ppInputShaderResourceView,
		ID3D11UnorderedAccessView** ppOutputUAVObjectOne,
		ID3D11Buffer** ppReadbackBufferOne);

	HRESULT RunImageBinaryClassifier(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);


	HRESULT ThresoldPixelValuesByCoordinatesCriteriaYCBCR(HINSTANCE hInstance,
		HWND hWnd,
		float pMinimumX,
		float pMinimumY,
		float pMaximumX,
		float pMaximumY,
		vector<YCBRPixelDataBuffer> ppPixelArrayInput,
		vector<OutputPixelObjectFilteredYCBCR> *ppPixelObjectGrayScale);

	HRESULT InitializeImageThresholdingCoordinatesOperationYCBCR(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		float pMinimumX,
		float pMinimumY,
		float pMaximumX,
		float pMaximumY,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<YCBRPixelDataBuffer> pixelArrayOne,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);


	HRESULT RunImageThresholdingOperationYCBCR(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);

	HRESULT RestoreYCBCRImageFromFilter(HINSTANCE hInstance, HWND hWnd,
		vector<OutputPixelObjectFilteredYCBCR> pBufferToRestorObject,
		vector<YCBRPixelDataBuffer> *pPixelObjectRestored);


	HRESULT InitializeYCBRImageRestorationResources(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<OutputPixelObjectFilteredYCBCR> pixelArrayOne,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);


	HRESULT RunYCBrImageRestorationFunction(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);




	HRESULT ThresoldPixelValuesByCriteriaGrayScale(HINSTANCE hInstance,
		HWND hWnd,
		float pMinimumX,
		float pMinimumY,
		float pMaximumX,
		float pMaximumY,
		float pThresholdIntensityValueBottom,
		float pThresholdIntensityValueTop,
		vector<PixelObjectGrayScale> ppPixelArrayInput,
		vector<OutputPixelObjectFilteredGrayScale> *ppPixelObjectGrayScale);

	HRESULT InitializeImageThresholdingOperation(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		float pMinimumX,
		float pMinimumY,
		float pMaximumX,
		float pMaximumY,
		float pThresholdIntensityValueBottom,
		float pThresholdIntensityValueTop,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<PixelObjectGrayScale> pixelArrayOne,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT ThresoldPixelValuesByCoordinatesCriteria(HINSTANCE hInstance,
		HWND hWnd,
		float pMinimumX,
		float pMinimumY,
		float pMaximumX,
		float pMaximumY,
		vector<PixelObjectGrayScale> ppPixelArrayInput,
		vector<OutputPixelObjectFilteredGrayScale> *ppPixelObjectGrayScale);

	HRESULT InitializeImageThresholdingCoordinatesOperation(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		float pMinimumX,
		float pMinimumY,
		float pMaximumX,
		float pMaximumY,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<PixelObjectGrayScale> pixelArrayOne,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);


	HRESULT RunImageThresholdingOperation(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);


	HRESULT SimulateInterestPointDetector(HINSTANCE hInstance,
		HWND hWnd,
		int pPixelNumberFactor,
		vector<PixelObject> pPixelObjectArrayToProcess,
		vector<OutputPixelObjectInterestPoint> *ppFilterOutputRGB);

	HRESULT InitializeInterestPointDetectorRGB(HINSTANCE hInstance,
		HWND hWnd,
		int pPixelNumberFactor,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<PixelObject> pixelArrayOne,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunInterestingPointDetector(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);


	HRESULT SimulateInterestPointDetectorCoordinates(HINSTANCE hInstance,
		HWND hWnd,
		float pXPositionFactor,
        float pYPositionFactor,
		vector<PixelObject> pPixelObjectArrayToProcess,
		vector<OutputPixelObjectInterestPoint> *ppFilterOutputRGB);

	HRESULT InitializeInterestPointDetectorCoordiantesRGB(HINSTANCE hInstance,
		HWND hWnd,
		float pXPositionFactor,
        float pYPositionFactor,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<PixelObject> pixelArrayOne,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);

	HRESULT RunInterestingPointCoordinatesDetector(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);





	HRESULT ThresoldPixelValuesByCriteriaRGB(HINSTANCE hInstance,
		HWND hWnd,
		float pMinimumX,
		float pMinimumY,
		float pMaximumX,
		float pMaximumY,
		float pThresholdIntensityValueBottomR,
		float pThresholdIntensityValueTopR,
		float pThresholdIntensityValueBottomG,
		float pThresholdIntensityValueTopG,
		float pThresholdIntensityValueBottomB,
		float pThresholdIntensityValueTopB,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectFiltered> *ppPixelObjectGrayScale);

	HRESULT ThresoldPixelValuesByCoordinatesCriteriaRGB(HINSTANCE hInstance,
		HWND hWnd,
		float pMinimumX,
		float pMinimumY,
		float pMaximumX,
		float pMaximumY,
		vector<PixelObject> ppPixelArrayInput,
		vector<OutputPixelObjectFiltered> *ppPixelObjectGrayScale);

	HRESULT InitializeImageThresholdingOperationRGB(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		float pMinimumX,
		float pMinimumY,
		float pMaximumX,
		float pMaximumY,
		float pThresholdIntensityValueBottomR,
		float pThresholdIntensityValueTopR,
		float pThresholdIntensityValueBottomG,
		float pThresholdIntensityValueTopG,
		float pThresholdIntensityValueBottomB,
		float pThresholdIntensityValueTopB,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<PixelObject> pixelArrayOne,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);





	

	HRESULT InitializeImageThresholdingCoordinatesOperationRGB(HINSTANCE hInstance,
		HWND hWnd,
		UINT ppAmountOfSamples,
		float pMinimumX,
		float pMinimumY,
		float pMaximumX,
		float pMaximumY,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<PixelObject> pixelArrayOne,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputBuffer1,
		ID3D11Buffer** ppOutputBuffer,
		ID3D11ShaderResourceView** ppSRV1,
		ID3D11UnorderedAccessView** ppUAVObject1,
		ID3D11Buffer** ppReadbackBuffer);


	HRESULT RunImageThresholdingOperationRGB(HINSTANCE hInstance, HWND hWnd,
		vector<PixelObject> pPixelObjectArrayImageOne,
		ID3D11Buffer* pInputBufferOne,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);





	HRESULT LoadImageDataAsPixelSet(HINSTANCE hInstane, HWND hWnd, LPCWSTR pFileName, vector<PixelObject>* pPixelObject);

	HRESULT SimulateStandardDeviationComputationP(HINSTANCE hInstance, HWND hWnd, vector<InputItemType> pInputDataArray,
		vector<CStandardDeviationOutputType> *pFunctionOutput, int* pOutputValue);


	HRESULT SimulateStandardDeviationComputationStepOne(HINSTANCE hInstance, HWND hWnd,
		vector<float> pInitialData, vector<float> *pFunctionOutput);
	
	HRESULT InitializeStandardDeviationStepOneFunctionResources(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<InputItemType> pInitialDataSet,
		CBStandardDeviation pStandardDeviationCB,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputDataBuffer,
		ID3D11Buffer** ppOutputDataBuffer,
		ID3D11ShaderResourceView** ppInputDataSRV,
		ID3D11UnorderedAccessView** ppOutputDataUAV,
		ID3D11Buffer** ppReadbackDataBuffer);
	
	HRESULT InitializeImageConvolutionResources(
		HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader** ppComputeShaderObject,
		CbConvolultionBuffer pConvolutionCB,
		vector<ImageKernelItem> pImageKernelDataArray,
		vector<PixelObjectGrayScale> pInputDataArray,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputDataBufferImage,
		ID3D11Buffer** ppKernelInputBuffer,
		ID3D11Buffer** ppOutputDataBuffer,
		ID3D11ShaderResourceView** ppInputDataSRV,
		ID3D11ShaderResourceView** ppKernelInputDataSRV,
		ID3D11UnorderedAccessView** ppOutputDataUAV,
		ID3D11Buffer** ppReadbackDataBuffer);

	void RunStandardDeviationAlgorithmStepOne(HINSTANCE hInstance,
		HWND hWnd,
		CBStandardDeviation pStandardDeviationBuffer,
		ID3D11Buffer* ppConstantBufferObject, ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);
	HRESULT SimulateImageConvolutionOperation(HINSTANCE hInstance,
		HWND hWnd,
		UINT pImageWidth,
		UINT pImageHeight,
		vector<PixelObjectGrayScale> pCoreImageObject,
		vector<ImageKernelItem> pImageKernel,
		vector<PixelObjectGrayScale> *ppPixelOutputValues);
	HRESULT SimulateImageConvolutionOperation(HINSTANCE hInstance,
		HWND hWnd,
		UINT pImageWidth,
		UINT pImageHeight,
		vector<PixelObjectGrayScale> pCoreImageObject,
		float pFilterArray[9],
		vector<PixelObjectGrayScale> *ppPixelOutputValues);

	HRESULT SimulateLaplacianOfGaussianFilteringOnSelectedImage(HINSTANCE hInstance,
		HWND hWnd, vector<PixelObjectGrayScale> pInputImageData,
		float pi_var,
		float pStandardDeviationVar,
		vector<GaussianOutputVar> *ppGaussianBlurOutputVar);

	HRESULT InitializeLaplacianofGaussianFilteringResources(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObjectGrayScale> pInputDataBuffer,
		CBGaussianBlurConstantBuffer pConstantBufferCB,
		ID3D11ComputeShader** ppGaussianBlurComputeShader,
		ID3D11Buffer** pGaussianBlurConstantBuffer,
		ID3D11Buffer** ppInputBufferGaussianBlur,
		ID3D11ShaderResourceView** ppGaussianBlurSRV,
		ID3D11Buffer** ppOutputBufferGaussianBlur,
		ID3D11UnorderedAccessView** ppOutputBufferForOutcome,
		ID3D11Buffer** ppReadbackDataBufferOutcome);


	HRESULT InitializeImageConvolutionResources(
		HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader** ppComputeShaderObject,
		CbKernelConvolution ppConvolutionBuff,
		vector<PixelObjectGrayScale> pInputDataArray,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputDataBufferImage,
		ID3D11Buffer** ppOutputDataBuffer,
		ID3D11ShaderResourceView** ppInputDataSRV,
		ID3D11UnorderedAccessView** ppOutputDataUAV,
		ID3D11Buffer** ppReadbackDataBuffer);

	HRESULT SimulateImageConvolutionOperationStructuredX(
		HINSTANCE hInstance,
		HWND hWnd,
		UINT pImageWidth,
		UINT pImageHeight,
		UINT pKernelWidth,
		UINT pKernelHeight,
		UINT pOutputImageWidth,
		UINT pOutputImageHeight,
		vector<PixelObjectGrayScale> pCoreImageObject,
		float pFilterArray[3],
		vector<PixelObjectGrayScale> *ppPixelOutputValues);

	HRESULT InitializeImageConvolutionResourcesStructuredX(HINSTANCE hInstance,
		HWND hWnd,
		CbConvolultionBuffer pConvolutionBufferObject,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<PixelObjectGrayScale> pInputDataArray,
		ID3D11Buffer** ppInputDataBuffer,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppOutputDataBuffer,
		ID3D11ShaderResourceView** ppInputDataSRV,
		ID3D11UnorderedAccessView** ppOutputDataUAV,
		ID3D11Buffer** ppReadbackDataBuffer);

	HRESULT RunConvolutionOperationsStructured(HINSTANCE hInstance, HWND hWnd,
		ID3D11Buffer* ppContstantBufffers,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceViewImage0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);

	HRESULT InitializeImageConvolutionResourcesStructuredY(HINSTANCE hInstance,
		HWND hWnd,
		CbConvolultionBuffer pConvolutionBufferObject,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<PixelObjectGrayScale> pInputDataArray,
		ID3D11Buffer** ppInputDataBuffer,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppOutputDataBuffer,
		ID3D11ShaderResourceView** ppInputDataSRV,
		ID3D11UnorderedAccessView** ppOutputDataUAV,
		ID3D11Buffer** ppReadbackDataBuffer);

	HRESULT SimulateImageConvolutionOperationStructuredY(
		HINSTANCE hInstance,
		HWND hWnd,
		UINT pImageWidth,
		UINT pImageHeight,
		UINT pKernelWidth,
		UINT pKernelHeight,
		UINT pOutputImageWidth,
		UINT pOutputImageHeight,
		vector<PixelObjectGrayScale> pCoreImageObject,
		float pFilterArray[3],
		vector<PixelObjectGrayScale> *ppPixelOutputValues);

	HRESULT  SimulateStandardDeviationComputationStepTwo(HINSTANCE hInstance,
		HWND hWnd,
		vector<float> pInitialData,
		vector<float> *pFunctionOutput);
	HRESULT InitializeStandardDeviationStepTwoFunctionResources(
		HINSTANCE hInstance,
		HWND hWnd,
		ID3D11ComputeShader** ppComputeShaderObject,
		vector<float> pInputDataArray,
		ID3D11Buffer** ppConstantBuffer,
		ID3D11Buffer** ppInputDataBuffer,
		ID3D11Buffer** ppOutputDataBuffer,
		ID3D11ShaderResourceView** ppInputDataSRV,
		ID3D11UnorderedAccessView** ppOutputDataUAV,
		ID3D11Buffer** ppReadbackDataBuffer);
	
	HRESULT RunStandardDeviationAlgorithmStepTwo(HINSTANCE hInstance, HWND hWnd,
		CBStandardDeviation pStandardDeviationBuffer,
		ID3D11Buffer* pInputBufferOne,
		ID3D11Buffer* pConstantBuffer,
		ID3D11ComputeShader* pComputeShaderObject,
		ID3D11ShaderResourceView* pShaderResourceView0,
		ID3D11UnorderedAccessView* pUnorderedUAV,
		UINT pNumberOfSRV, UINT pNumberOfUAV,
		UINT X, UINT Y, UINT Z);





	void PerformArrayAdditionSimulation();
	HRESULT ComputeGasDynamicsFormulaOneOnCPU(HINSTANCE hInstance, HWND hWnd);
	HRESULT ComputeGasDynamicsFormulaTwoOnCPU(HINSTANCE hInstance, HWND hWnd);
	void DestroyTask(LPCSTR fName); /*******Заверши Задачу**************/
	void CreateGroupTask(LPCSTR fName,  LPCSTR dataSet, LPCSTR output); /*******Создай групповую задачу**************/
	void CreateNewAlgorithm(LPCSTR fName,  LPCSTR dataSet, LPCSTR output); /*******Создай Новый Алгоритм**************/
	void StartTask(UINT taskId); /*******Запусти задачу**************/
	void StartGroupTask(UINT taskId); /*******Запусти групповую задачу**************/
	HRESULT CreateNewAlgorithmObject(LPCSTR strFname); /*******Создай Новый Алгоритм**************/
	HRESULT ExecuteAlgorithmObject(LPCSTR strFname); /*******Запусти Новый Алгоритм**************/
	HRESULT ExecuteGroupAlgorithmObject(vector<LPCSTR> strfName); /*******Запусти групповой Алгоритм**************/
	ComputationState GetTaskState(UINT taskId);  /*******Получи состояние задачи**************/
	ComputationState GetGroupTaskState(UINT groupTaskState);  /*******Получи групповой задачи**************/
	ComputationState GetAlgorithmStat(UINT algorithmState); /*******Получи состояние алгоритма**************/
	HRESULT TerminateTask(UINT taskId); /**********Заверши Залачу***********/
	HRESULT TerminateGroupTask(UINT groupTaskId);  /*******Заверши Групповую Залачу**************/
	HRESULT StartDataProcessingThread(HINSTANCE hInstance, HWND hWnd); /*******Начни основной цикл работы**************/
	HRESULT TerminateDataProcessingThread(HINSTANCE hInstance, HWND hWnd);  /*******Заверши основной цикл работы**************/
	void DestroyExecutorData();         
	HRESULT SimulateMatrixMultiplicationForImages(HINSTANCE hInstance, HWND hWnd, 
		vector<float> pImageArrayOne,
		vector<float> pImageArrayTwo,
		UINT pMatrixAWidth,
		UINT pMatrixAHeight,
		UINT pMatrixBWidth,
		UINT pMatrixBHeight,
		UINT pMatrixOutWidth,
		UINT pMatrixOutHeight,
		vector<BufType> *ppMatrixVariableOutputType);

	HRESULT InitializeMatrixMultiplicationResources(HINSTANCE hInstance,
		HWND hWnd,
		vector<BufType> pMatrixOne,
		vector<BufType> pMatrixTwo,
		UINT pMatrixAHeight,
		UINT pMatrixAWidth,
		UINT pMatrixBHeight,
		UINT pMatrixBWidth,
		UINT pMatrixOutHeight,
		UINT pMatrixOutWidth,
		ID3D11ComputeShader** ppMatrixComputeShader,
		ID3D11Buffer** ppMatrixMultiplyConstantBuffer,
		ID3D11Buffer** ppMatrixMultiplyInputBufferA,
		ID3D11Buffer** ppMatrixMultiplyInputBufferB,
		ID3D11Buffer** ppMatrixMultiplyOutputBuffer,
		ID3D11ShaderResourceView** pOutputSRV0,
		ID3D11ShaderResourceView** pOutputSRV1,
		ID3D11UnorderedAccessView** pOutputUAV);
	
	HRESULT RunMatrixMultpilicationAlgorithm(
		HINSTANCE hInstance,
		HWND hWnd,
		vector<BufType> pMatrixOne,
		vector<BufType> pMatrixTwo,
		ID3D11Buffer* ppInputBufferOne,
		ID3D11Buffer* pInputBufferTwo,
		MatrixMultiplyCB pConstantBufferCB,
		ID3D11Buffer* pMatrixMultiplyConstantBuffer,
		ID3D11ComputeShader* pComputeShader,
		ID3D11ShaderResourceView* pSRV0,
		ID3D11ShaderResourceView* pSRV1,
		ID3D11UnorderedAccessView* pUAV0,
		int xThread, int pYThread, int pZThread);

	HRESULT SimulateRandomAlgorithm(HINSTANCE hInstance, HWND hWND);
	HRESULT RunComputeShaderAlgorithm(HINSTANCE hInstance, HWND hWnd, vector<BData> pValuesData, ID3D11ComputeShader* pComputeShader, ID3D11Buffer* pConstBufferData, ID3D11ShaderResourceView* pSRV[2], ID3D11UnorderedAccessView* pConnectableUAV[2]);
	Direct2DManagementClass* pD2DManagementClass;
	HRESULT SimulateMatrixMultiplicationUsingAMP(HINSTANCE hInstance, HWND hWnd, vector<float> pArrayOne,
		vector<float> pArrayTwo,
		UINT amountOfCollMatOne, UINT amountOfRowsMatOne,
		UINT pAmountOfColsMatTwo, UINT pAmountOfRowsMatTwo,
		vector<float> *pOutputArrayVar);

	bool LoadContent() {return true; }
	void UnloadContent() {} 
	void UpdateScene() {}
	void UpdateScene(float dt) {}
	void DrawScene() {}
	void OnResize() {}
	void Update() {}
	void Render();
};

