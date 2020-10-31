
cbuffer CBRestorationStructDescriptor : register(b0)
{
	int pClassObjectToRestore;
};

struct OutputPixelObjectDescriptor
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float rValue;
	float gValue;
	float bValue;
	int pClassValue;

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




StructuredBuffer<OutputPixelObjectDescriptor> pPixelArrayInputOne : register(t0);
RWStructuredBuffer<PixelObject> pPixelArrayForCategoryOne : register(u0);


bool CompareClassValues(int pPixelClassValue, int pParameterClassValue)
{
	bool pResult = false;
	[branch]
	if (pPixelClassValue == pParameterClassValue)
	{
		pResult = true;
	}
	else if (pPixelClassValue == pParameterClassValue)
	{
		pResult = false;
	}
	return pResult;
}


[numthreads(512, 1, 1)]
void CS_Main(uint3 Gid : SV_GroupId,
	uint3 DTid : SV_DispatchThreadId,
	uint3 GTid : SV_GroupThreadId,
	uint GI : SV_GroupIndex)
{

	int pFilterPixelNumber = 0;
	float pFilterXValue = 0.0f;
	float pFilterYValue = 0.0f;
	float pFilterRedValue = 0.0f;
	float pFilterGreenValue = 0.0f;
	float pFilterBlueValue = 0.0f;
	int pPixelClassValue = 0;
	bool pIsPixelValid = true;


	OutputPixelObjectDescriptor pInputPixelObjectFiltered;
	pInputPixelObjectFiltered.pixelNumber = 0;
	pInputPixelObjectFiltered.xPosition = 0.0f;
	pInputPixelObjectFiltered.yPosition = 0.0f;
	pInputPixelObjectFiltered.rValue = 0.0f;
	pInputPixelObjectFiltered.gValue = 0.0f;
	pInputPixelObjectFiltered.bValue = 0.0f;
	pInputPixelObjectFiltered.pClassValue = 0;



	PixelObject pFirstClassPixelObject;
	pFirstClassPixelObject.pixelNumber = 0;
	pFirstClassPixelObject.xPosition = 0.0;
	pFirstClassPixelObject.yPosition = 0.0f;
	pFirstClassPixelObject.rValue = 0.0f;
	pFirstClassPixelObject.gValue = 0.0f;
	pFirstClassPixelObject.bValue = 0.0f;





	pInputPixelObjectFiltered = pPixelArrayInputOne[DTid.x + DTid.y * 128];



	pFilterPixelNumber = pInputPixelObjectFiltered.pixelNumber;
	pFilterXValue = pInputPixelObjectFiltered.xPosition;
	pFilterYValue = pInputPixelObjectFiltered.yPosition;
	pFilterRedValue = pInputPixelObjectFiltered.rValue;
	pFilterGreenValue = pInputPixelObjectFiltered.gValue;
	pFilterBlueValue = pInputPixelObjectFiltered.bValue;
	pPixelClassValue = pInputPixelObjectFiltered.pClassValue;


	bool pCompareClassValue = false;
	pCompareClassValue = CompareClassValues(pPixelClassValue, pClassObjectToRestore);


	[branch]
	if (pCompareClassValue == true)
	{

		pFirstClassPixelObject.pixelNumber = pFilterPixelNumber;
		pFirstClassPixelObject.xPosition = pFilterXValue;
		pFirstClassPixelObject.yPosition = pFilterYValue;
		pFirstClassPixelObject.rValue = pFilterRedValue;
		pFirstClassPixelObject.gValue = pFilterGreenValue;
		pFirstClassPixelObject.bValue = pFilterBlueValue;

	}
	else if (pCompareClassValue == false)
	{

		pFirstClassPixelObject.pixelNumber = 777;
		pFirstClassPixelObject.xPosition = 777;
		pFirstClassPixelObject.yPosition = 777;
		pFirstClassPixelObject.rValue = 777;
		pFirstClassPixelObject.gValue = 777;
		pFirstClassPixelObject.bValue = 777;

	}

	pPixelArrayForCategoryOne[DTid.x + DTid.y * 128] = pFirstClassPixelObject;


}