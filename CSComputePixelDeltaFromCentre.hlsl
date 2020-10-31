

static const float PI = 3.14159265f;


cbuffer pCentreCB : register(b0)
{
	float pXPositionCentre;
	float pYPositionCentre;
	float pYPositionStart;
	float pi_var;
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


struct OutputPixelObjectWithAngleValues
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float rValue;
	float gValue;
	float bValue;
	float pCentreXDifference;
	float pCentreYDifference;
	float pTangentValue;
	float pRadiusValue;
	float pAngleSinValue;
	float pAngleCosValue;
	float pAngleValueRadians;
	float pAngleValueDegrees;
	float pQuarterNumberVar;
};




StructuredBuffer<PixelObject> pPixelArrayInputOne : register(t0);
RWStructuredBuffer<OutputPixelObjectWithAngleValues> pPixelArrayOutputOne : register(u0);

float ComputePixelCategory(float pXPositionCentreValue,
	float pYPositionCentreValue,
	float pSinValueVar)
{
	float pQuarterValue = 0.0f;





	[branch]
	if ((pXPositionCentreValue > 0.0f) && (pYPositionCentreValue > 0.0f) && (pSinValueVar > 0.0f))
	{
			pQuarterValue = 1.0f;
	}
	else if ((pXPositionCentreValue < 0.0f) && (pYPositionCentreValue > 0.0f) && (pSinValueVar > 0.0f))
	{
		pQuarterValue = 2.0f;
	}

	else if ((pXPositionCentreValue < 0.0f) && (pYPositionCentreValue < 0.0f) && (pSinValueVar < 0.0f))
	{
		pQuarterValue = 3.0f;
	}
	else if ((pXPositionCentreValue > 0.0f) && (pYPositionCentreValue < 0.0f) && (pSinValueVar < 0.0f))
	{
		pQuarterValue = 4.0f;
	}
	return pQuarterValue;
}


[numthreads(256, 1, 1)]
void CS_Main(uint3 Gid : SV_GroupId,
	uint3 DTid : SV_DispatchThreadId,
	uint3 GTid : SV_GroupThreadId,
	uint GI : SV_GroupIndex)
{

	PixelObject pPixelObjectItemOne;
	pPixelObjectItemOne.pixelNumber = 0;
	pPixelObjectItemOne.xPosition = 0.0f;
	pPixelObjectItemOne.yPosition = 0.0f;
	pPixelObjectItemOne.rValue = 0.0f;
	pPixelObjectItemOne.gValue = 0.0f;
	pPixelObjectItemOne.bValue = 0.0f;

	OutputPixelObjectWithAngleValues pOutputPixelValueObject;
	pOutputPixelValueObject.pixelNumber = 0;
	pOutputPixelValueObject.xPosition = 0.0f;
	pOutputPixelValueObject.yPosition = 0.0f;
	pOutputPixelValueObject.rValue = 0.0f;
	pOutputPixelValueObject.gValue = 0.0f;
	pOutputPixelValueObject.bValue = 0.0f;
	pOutputPixelValueObject.pCentreXDifference = 0.0f;
	pOutputPixelValueObject.pCentreYDifference = 0.0f;
	pOutputPixelValueObject.pTangentValue = 0.0f;
	pOutputPixelValueObject.pRadiusValue = 0.0f;
	pOutputPixelValueObject.pAngleSinValue = 0.0f;
	pOutputPixelValueObject.pAngleCosValue = 0.0f;
	pOutputPixelValueObject.pAngleValueRadians = 0.0f;
	pOutputPixelValueObject.pAngleValueDegrees = 0.0f;
	pOutputPixelValueObject.pQuarterNumberVar = 0.0f;


	int pPixelNumberVar = 0;
	float pPixelXPosition = 0.0f;
	float pPixelYPosition = 0.0f;
	float pRedValue = 0.0f;
	float pGreenValue = 0.0f;
	float pBlueValue = 0.0f;
	float pCentreDifferenceXPosition = 0.0f;
	float pCentreDifferenceYPosition = 0.0f;
	float pTangentValue = 0.0f;
	float pDifferenceRatioVarMode = 0.0f;
	float pPixelSectorNumber = 0.0f;
	float pRadiusValue = 0.0f;
	float pAngleValueRadians = 0.0f;
	float pAngleValueDegrees = 0.0f;
	float pAngleSinValueVar = 0.0f;
	float pAngleCosValueVar = 0.0f;
	float pAngleValueRadiansVar = 0.0f;
	float pAngleValueDegreesVar = 0.0f;


	float pQuarterValue = 0.0f;


	pPixelObjectItemOne = pPixelArrayInputOne[DTid.x + DTid.y * 128];
	pPixelNumberVar = pPixelObjectItemOne.pixelNumber;
	pPixelXPosition = pPixelObjectItemOne.xPosition;
	pPixelYPosition = pPixelObjectItemOne.yPosition;
	pRedValue = pPixelObjectItemOne.rValue;
	pGreenValue = pPixelObjectItemOne.gValue;
	pBlueValue = pPixelObjectItemOne.bValue;

	[branch]
	if ((pPixelXPosition < pXPositionCentre) && (pPixelYPosition < pYPositionCentre))
	{
		pCentreDifferenceXPosition = pPixelXPosition - pXPositionCentre;
		pCentreDifferenceYPosition = pYPositionCentre - pPixelYPosition;
	}
	else if ((pPixelXPosition > pXPositionCentre) && (pPixelYPosition < pYPositionCentre))
	{
		pCentreDifferenceXPosition = pPixelXPosition - pXPositionCentre;
		pCentreDifferenceYPosition = pYPositionCentre - pPixelYPosition;
		
	}
	else if ((pPixelXPosition < pXPositionCentre) && (pPixelYPosition > pYPositionCentre))
	{
		pCentreDifferenceXPosition = pPixelXPosition - pXPositionCentre;
		pCentreDifferenceYPosition = pYPositionCentre - pPixelYPosition;
	}
	else if ((pPixelXPosition > pXPositionCentre) && (pPixelYPosition > pYPositionCentre))
	{
		pCentreDifferenceXPosition = pPixelXPosition - pXPositionCentre;
		pCentreDifferenceYPosition = pYPositionCentre - pPixelYPosition;

	}


	pRadiusValue = sqrt(pow((pPixelXPosition - pXPositionCentre), 2.0f) + pow((pPixelYPosition - pYPositionCentre), 2.0f));
	pTangentValue = pCentreDifferenceYPosition / pCentreDifferenceXPosition;
	pAngleSinValueVar = pCentreDifferenceYPosition / pRadiusValue;
	pAngleCosValueVar = pCentreDifferenceXPosition / pRadiusValue;
	pAngleValueRadiansVar = asin(pAngleSinValueVar);
	pAngleValueDegrees = (pAngleValueRadiansVar / 22 * 7) * 180;
	pAngleValueDegreesVar = degrees(pAngleValueRadiansVar);

	pQuarterValue = ComputePixelCategory(pCentreDifferenceXPosition, pCentreDifferenceYPosition,
		pAngleSinValueVar);


	pOutputPixelValueObject.pixelNumber = pPixelNumberVar;
	pOutputPixelValueObject.xPosition = pPixelXPosition;
	pOutputPixelValueObject.yPosition = pPixelYPosition;
	pOutputPixelValueObject.rValue = pRedValue;
	pOutputPixelValueObject.gValue = pGreenValue;
	pOutputPixelValueObject.bValue = pBlueValue;
	pOutputPixelValueObject.pCentreXDifference = pCentreDifferenceXPosition;
	pOutputPixelValueObject.pCentreYDifference = pCentreDifferenceYPosition;
	pOutputPixelValueObject.pTangentValue = pTangentValue;
	pOutputPixelValueObject.pRadiusValue = pRadiusValue;
	pOutputPixelValueObject.pAngleSinValue = pAngleSinValueVar;
	pOutputPixelValueObject.pAngleCosValue = pAngleCosValueVar;
	pOutputPixelValueObject.pAngleValueRadians = pAngleValueRadiansVar;
	pOutputPixelValueObject.pAngleValueDegrees = pAngleValueDegreesVar;
	pOutputPixelValueObject.pQuarterNumberVar = pQuarterValue;


	pPixelArrayOutputOne[DTid.x + DTid.y * 128] = pOutputPixelValueObject;



}