

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


struct PixelObject
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float rValue;
	float gValue;
	float bValue;
};




StructuredBuffer<OutputPixelObjectFiltered> pPixelArrayInputOne : register(t0);
RWStructuredBuffer<PixelObject> pPixelArrayForValidPixels : register(u0);








[numthreads(256, 1, 1)]
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
	bool pIsPixelUsedParam = false;


	OutputPixelObjectFiltered pInputPixelObjectFiltered;
	pInputPixelObjectFiltered.pixelNumber = 0;
	pInputPixelObjectFiltered.xPosition = 0.0f;
	pInputPixelObjectFiltered.yPosition = 0.0;
	pInputPixelObjectFiltered.rValue = 0.0f;
	pInputPixelObjectFiltered.gValue = 0.0f;
	pInputPixelObjectFiltered.bValue = 0.0f;
	pInputPixelObjectFiltered.pIsPixelValid = false;

	PixelObject pPixelObjectToRestore;
	pPixelObjectToRestore.pixelNumber = 0;
	pPixelObjectToRestore.xPosition = 0.0f;
	pPixelObjectToRestore.yPosition = 0.0f;
	pPixelObjectToRestore.rValue = 0.0f;
	pPixelObjectToRestore.gValue = 0.0f;
	pPixelObjectToRestore.bValue = 0.0f;

	pInputPixelObjectFiltered = pPixelArrayInputOne[DTid.x + DTid.y * 128];

	

	pFilterPixelNumber = pInputPixelObjectFiltered.pixelNumber;
	pFilterXValue = pInputPixelObjectFiltered.xPosition;
	pFilterYValue = pInputPixelObjectFiltered.yPosition;
	pFilterRedValue = pInputPixelObjectFiltered.rValue;
	pFilterGreenValue = pInputPixelObjectFiltered.gValue;
	pFilterBlueValue = pInputPixelObjectFiltered.gValue;
	pIsPixelUsedParam = pInputPixelObjectFiltered.pIsPixelValid;

	
	[branch]
	if (pIsPixelUsedParam == true || pIsPixelUsedParam == 255)
	{
		pPixelObjectToRestore.pixelNumber = pFilterPixelNumber;
		pPixelObjectToRestore.xPosition = pFilterXValue;
		pPixelObjectToRestore.yPosition = pFilterYValue;
		pPixelObjectToRestore.rValue = pFilterRedValue;
		pPixelObjectToRestore.gValue = pFilterGreenValue;
		pPixelObjectToRestore.bValue = pFilterBlueValue;
	}
	else
	{
		pPixelObjectToRestore.pixelNumber = 777;
		pPixelObjectToRestore.xPosition = 777;
		pPixelObjectToRestore.yPosition = 777;
		pPixelObjectToRestore.rValue = 777;
		pPixelObjectToRestore.gValue = 777;
		pPixelObjectToRestore.bValue = 777;
	}


	pPixelArrayForValidPixels[DTid.x + DTid.y * 128] = pPixelObjectToRestore;


	
}