


cbuffer CBThreshHoldVar : register(b0)
{
	float pMinimumX;
	float pMinimumY;
	float pMaximumX;
	float pMaximumY;
	float pThresholdIntensityLowerValueR;
	float pThresholdIntensityUpperValueR;
	float pThresholdIntensityLowerValueG;
	float pThresholdIntensityUpperValueG;
	float pThresholdIntensityLowerValueB;
	float pThresholdIntensityUpperValueB;
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


struct OutputPixelObjectFiltered
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float rValue;
	float gValue;
	float bValue;
	bool pIsPixelValid;
};




StructuredBuffer<PixelObject> pPixelArrayInputOne : register(t0);
RWStructuredBuffer<OutputPixelObjectFiltered> pPixelArrayOutputOne : register(u0);





	[numthreads(256, 1, 1)]
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




		pInitialPixelObject = pPixelArrayInputOne[DTid.x + DTid.y * 128];

		pixelNumber0 = pInitialPixelObject.pixelNumber;
		pXPosition0 = pInitialPixelObject.xPosition;
		pYPosition0 = pInitialPixelObject.yPosition;
		pRedValue0 = pInitialPixelObject.rValue;
		pGreenValue0 = pInitialPixelObject.gValue;
		pBlueValue0 = pInitialPixelObject.bValue;

		bool pUsedPixelVal = false;

		OutputPixelObjectFiltered pPixelObjectFiltered;
		pPixelObjectFiltered.pixelNumber = 0;
		pPixelObjectFiltered.xPosition = 0.0f;
		pPixelObjectFiltered.yPosition = 0.0f;
		pPixelObjectFiltered.rValue = 0.0f;
		pPixelObjectFiltered.gValue = 0.0f;
		pPixelObjectFiltered.bValue = 0.0f;
		pPixelObjectFiltered.pIsPixelValid = false;




		[branch]
		if ((pXPosition0 >= pMinimumX) && (pYPosition0 >= pMinimumY) && (pXPosition0 <= pMaximumX) && (pYPosition0 <= pMaximumY) &&
			(pRedValue0 >= pThresholdIntensityLowerValueR) && (pRedValue0 <= pThresholdIntensityUpperValueR) &&
			(pGreenValue0 >= pThresholdIntensityLowerValueG) && (pGreenValue0 <= pThresholdIntensityUpperValueG) &&
			(pBlueValue0 >= pThresholdIntensityLowerValueB) && (pBlueValue0 <= pThresholdIntensityUpperValueB))
		{
			pUsedPixelVal = true;
		}
		else
		{
			pUsedPixelVal = false;
		}

		pPixelObjectFiltered.pixelNumber = pixelNumber0;
		pPixelObjectFiltered.xPosition = pXPosition0;
		pPixelObjectFiltered.yPosition = pYPosition0;
		pPixelObjectFiltered.rValue = pRedValue0;
		pPixelObjectFiltered.gValue = pGreenValue0;
		pPixelObjectFiltered.bValue = pBlueValue0;
		pPixelObjectFiltered.pIsPixelValid = pUsedPixelVal;

		pPixelArrayOutputOne[DTid.x + DTid.y * 128] = pPixelObjectFiltered;

		
	}
