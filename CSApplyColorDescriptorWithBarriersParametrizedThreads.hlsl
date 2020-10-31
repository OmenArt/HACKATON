cbuffer  CBThreshHoldVarColorDescriptorBarriers : register(b0)
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

struct PixelObject
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float rValue;
	float gValue;
	float bValue;
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



StructuredBuffer<PixelObject> pPixelArrayInputOne : register(t0);
RWStructuredBuffer<OutputPixelObjectDescriptor> pPixelArrayOutputOne : register(u0);



int ValidateColorAgainstClass(
	float pRedValueImage,
	float pGreenValueImage,
	float pBlueValueImage,
	float pRedValueClassOne,
	float pGreenValueClassOne,
	float pBlueValueClassOne,
	float pRedValueClassTwo,
	float pGreenValueClassTwo,
	float pBlueValueClassTwo,
	int pClassMarkerOne,
	int pClassMarkerTwo,
	int pClassMarkerThree,
	float pDeltaValue)
{
	int pClassMarkerToCompute = 0;
	//Computing Barriers;
	float pLowerBarrierRed = 0.0f;
	float pLowerBarrierGreen = 0.0f;
	float pLowerBarrierBlue = 0.0f;

	//Computing Barriers;
	float pUpperBarrierRed = 0.0f;
	float pUpperBarrierGreen = 0.0f;
	float pUpperBarrierBlue = 0.0f;

	pLowerBarrierRed = pRedValueClassOne + pDeltaValue;
	pLowerBarrierGreen = pGreenValueClassOne + pDeltaValue;
	pLowerBarrierBlue = pBlueValueClassOne + pDeltaValue;

	pUpperBarrierRed = pRedValueClassTwo - pDeltaValue;
	pUpperBarrierGreen = pGreenValueClassTwo - pDeltaValue;
	pUpperBarrierBlue = pBlueValueClassTwo - pDeltaValue;

	[branch]
	if (((pRedValueImage >= pRedValueClassOne) && (pRedValueImage <= pLowerBarrierRed)) &&
		((pGreenValueImage >= pGreenValueClassOne) && (pGreenValueImage <= pLowerBarrierGreen)) &&
		((pBlueValueImage >= pBlueValueClassOne) && (pBlueValueImage <= pLowerBarrierBlue)))
	{
		pClassMarkerToCompute = pClassMarkerOne;
	}
	else if (((pRedValueImage >= pUpperBarrierRed) && (pRedValueImage <= pRedValueClassTwo)) &&
		((pGreenValueImage >= pUpperBarrierGreen) && (pGreenValueImage <= pGreenValueClassTwo)) &&
		((pBlueValueImage >= pUpperBarrierBlue) && (pBlueValueImage <= pBlueValueClassTwo)))
	{
		pClassMarkerToCompute = pClassMarkerTwo;
	}
	else
	{
		pClassMarkerToCompute = pClassMarkerThree;
	}

	return pClassMarkerToCompute;
}




[numthreads(GROUP_SIZE_X, 1, 1)]
void CS_Main(uint3 Gid : SV_GroupId,
	uint3 DTid : SV_DispatchThreadId,
	uint3 GTid : SV_GroupThreadId,
	uint GI : SV_GroupIndex)
{
	PixelObject pInitialPixelObject;
	pInitialPixelObject.pixelNumber = 0;
	pInitialPixelObject.xPosition = 0.0f;
	pInitialPixelObject.yPosition = 0.0f;
	pInitialPixelObject.rValue = 0.0f;
	pInitialPixelObject.gValue = 0.0f;
	pInitialPixelObject.bValue = 0.0;

	int pixelNumber0 = 0;
	float pXPosition0 = 0.0f;
	float pYPosition0 = 0.0f;
	float pRedValue0 = 0.0f;
	float pGreenValue0 = 0.0f;
	float pBlueValue0 = 0.0f;
	int pPixelMarkerValue;

	int pThreadedIndex = 0;
	pThreadedIndex = GROUP_SIZE_X / 2;


	pInitialPixelObject = pPixelArrayInputOne[DTid.x + DTid.y * pThreadedIndex];

	pixelNumber0 = pInitialPixelObject.pixelNumber;
	pXPosition0 = pInitialPixelObject.xPosition;
	pYPosition0 = pInitialPixelObject.yPosition;
	pRedValue0 = pInitialPixelObject.rValue;
	pGreenValue0 = pInitialPixelObject.gValue;
	pBlueValue0 = pInitialPixelObject.bValue;

	OutputPixelObjectDescriptor pPixelObjectFilteredOne;
	pPixelObjectFilteredOne.pixelNumber = 0;
	pPixelObjectFilteredOne.xPosition = 0.0f;
	pPixelObjectFilteredOne.yPosition = 0.0f;
	pPixelObjectFilteredOne.pRedValue0 = 0.0f;
	pPixelObjectFilteredOne.pGreenValue0 = 0.0f;
	pPixelObjectFilteredOne.pBlueValue0 = 0.0f;
	pPixelObjectFilteredOne.pClassMarkerValue = 0;


	pPixelMarkerValue = ValidateColorAgainstClass(pRedValue0, pGreenValue0,
		pBlueValue0, pRedValueOne, pGreenValueOne, pBlueValueOne, pRedValueTwo, pGreenValueTwo,
		pBlueValueTwo,
		pClassOneValue, pClassTwoValue,
		pBackGroundValue,
		pDeltaValue);




	pPixelObjectFilteredOne.pixelNumber = pixelNumber0;
	pPixelObjectFilteredOne.xPosition = pXPosition0;
	pPixelObjectFilteredOne.yPosition = pYPosition0;
	pPixelObjectFilteredOne.pRedValue0 = pRedValue0;
	pPixelObjectFilteredOne.pGreenValue0 = pGreenValue0;
	pPixelObjectFilteredOne.pBlueValue0 = pBlueValue0;
	pPixelObjectFilteredOne.pClassMarkerValue = pPixelMarkerValue;



	pPixelArrayOutputOne[DTid.x + DTid.y * pThreadedIndex] = pPixelObjectFilteredOne;


}