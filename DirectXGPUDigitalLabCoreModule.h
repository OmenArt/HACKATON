#pragma once
#include <Sddl.h> // for SID management
#include <tchar.h>
#include <strsafe.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <stack>
#include <algorithm>

#ifndef BOOST_NO_EXCEPTIONS
#define BOOST_NO_EXCEPTIONS
#endif



template<class S>
CLSID CreateGUID(const S& hexString)
{
	CLSID clsid;
	CLSIDFromString(CComBSTR(hexString), &clsid);

	return clsid;
}
#include "DirectXComputationalAgentManager.h"
#include "Direct2DManagementClass.h"
#include "Direct2DImageProcessingManager.h"
#include "DirectXGPUDigitalLabCoreModuleKernel_h.h"
#include <atlbase.h>
#include <atlcom.h>
#include "DataLoggerClass.h"
#include <cstdlib>
#include <shellapi.h>
#include <Shlwapi.h>
#include "BasicPlayback.h"
#include "DataLoggerClass.h"
#include "DirectXComputeShaderManagementClass.h"
#include "DirectXGPUDigitalLabSoftwareInterface.h"
#include "DirectXTexturingManager.h"
#include "DirectXConnectedApplicationLauncher.h"
#include "DirectXJobManagmentClass.h"
#include "DirectXKnowledgeDatabaseManagementClass.h"
#include "DirectXStandardAlgorithmExecutor.h"
#include "DirectXGPUDigitalLabClassFactory.h"
#include "comdef.h"
#include "PixelObjectClass.h"
#include <objbase.h>
#include "DirectXVideoAnalyticsManagementEngine.h"
#include "RobotCleanerReportObject.h"
#include <MsXml6.h>
#include <stdexcept>
#include <map>
#include <chrono>
#include <bitset>
#include "PixelObject.h"
#include "BinaryTreeNode.h"
#include <chrono>
#include <sys/timeb.h>

#define _WIN32_DCOM
#define BUFSIZE 512
#define MAX_CONSOLE_LINES 500
#define BUF_SIZE 256


using namespace std;
using namespace std::chrono;


namespace boost
{
	void throw_exception(std::exception const& ex)
	{
		std::cerr << ex.what() << endl;
	}
}

#define LCID_ENGLISH MAKELCID(MAKELANGID(0x09, 0x01), SORT_DEFAULT)
#define LCID_GERMAN MAKELCID(MAKELANGID(0x07, 0x01), SORT_DEFAULT)
#include "ole2.h"

HRESULT UnregisterServer(const CLSID& clsid, const char* szVerIndProgID, const char* szProgID);
DWORD WINAPI ProcessGPUDigitalLabDataLoop(LPVOID lpParam);
DWORD WINAPI InstanceThreadA(LPVOID lpvParam);
DWORD WINAPI ControlWindowsService(LPVOID pServiceName);
BOOL SetKey(const char* szKey, const char* szNamedValue, const char* szValue);
BOOL AddKey(const char* szKey, const char* szSubkey, const char* szValue);
void GuidToStr(REFCLSID clsid, char* szCLSID, int length);
HRESULT WINAPI SimulateArraySortingAlgorirthm(HINSTANCE hInstance, HWND hWnd);
HRESULT WINAPI SimulateGasDynamicsModelsFunction(HINSTANCE hInstance, HWND hWnd);



class DirectXCloudServerConnectionManager;

__declspec(dllexport) HRESULT WINAPI SimulateArraySortingAlgorirthm();
__declspec(dllexport) HRESULT WINAPI SimulateArrayAdditionAlgorithm();
__declspec(dllexport) HRESULT WINAPI InitializeInternalTCPServer();
HRESULT WINAPI ComputeGasDynamicsFunction();
__declspec(dllexport) HRESULT WINAPI SimulateRobotCleanerObject();









namespace constants
{
	LPWSTR pVideoFileNameOne = L"C:\\RobotData\\Videos\\Video1.mp4";
	LPWSTR pVideoFileNameTwo = L"C:\\RobotData\\Videos\\Video2.mp4";
	LPWSTR pSpatialShapeVideoOne= L"C:\\RobotData\\Videos\\SpatialShapeVideo1.mp4";
	LPWSTR pSpatialShapeVideoTwo = L"C:\\RobotData\\Videos\\SpatialShapeVideo2.mp4";

	LPWSTR pImageOneForProcessing = L"C:\\RobotData\\Images\\Image11.jpg";
	LPWSTR pImageTwoForProcessing = L"C:\\RobotData\\Images\\Image12.jpg";
	LPWSTR pImageThreeForProcessing = L"C:\\RobotData\\Images\\Image21.jpg";
	LPWSTR pImageFourForProcessing = L"C:\\RobotData\\Images\\Image22.jpg";
	LPWSTR pImageOneForProcessingAutonomous = L"C:\\RobotData\\Images\\ImageAutonomous11.jpg";
	LPWSTR pImageTwoForProcessingAutonomous = L"C:\\RobotData\\Images\\ImageAutonomous12.jpg";
	LPWSTR pImageThreeForProcessingAutonomous = L"C:\\RobotData\\Images\\ImageAutonomous21.jpg";
	LPWSTR pImageFourForProcessingAutonomous = L"C:\\RobotData\\Images\\ImageAutonomous22.jpg";
	LPWSTR pSpatialShapeImageOne = L"C:\\RobotData\\Images\\SpatialShapeImageOne.jpg";
	LPWSTR pSpatialShapeImageTwo = L"C:\\RobotData\\Images\\SpatialShapeImageTwo.jpg";
	LPWSTR pSpatialShapeImageThree = L"C:\\RobotData\\Images\\SpatialShapeImageThree.jpg";
	LPWSTR pSpatialShapeImageFour = L"C:\\RobotData\\Images\\SpatialShapeImageFour.jpg";

	LPWSTR pDescriptorImage = L"C:\\Descriptor Images\\Image1.png";

}


struct HuffmanCodeItem
{
	float pItemValue;
	float pFrequency;
public: 
	HuffmanCodeItem(float pItemValueVar,
					float pFrequencyVar)
	{
		pItemValueVar = pItemValue;
		pFrequency = pFrequencyVar;
	}
	
};


enum ModelType
{
	SpatialObject,
	Square,
	Circle
};



class ModelObject
{
	
	ModelType pTypeOfModel;
	vector<PixelObject> pPixelObjectXScan;
	vector<PixelObject> pPixelObjectYScan;
	vector<OutputPixelObjectFiltered> pFilteredArray;
	vector<PixelObject> pixelObjectSectorArrayRestored;
	vector<PixelObject> pSectorAverages;
	vector<OutputPixelObjectWithAngleValues> pPixelObjectAfterLocalization;
	vector<OutputPixelObjectWithAngleValues> pSectorOne;
	vector<OutputPixelObjectWithAngleValues> pSectorTwo;
	vector<OutputPixelObjectWithAngleValues> pSectorThree;
	vector<OutputPixelObjectWithAngleValues> pSectorFour;
	vector<Point2D> pGeneratedPointVector;
	vector<Line2D> pGeneratedLineVector;

public:

	float pTopLeftX;
	float pTopLeftY;
	float pImageWidth;
	float pImageHeight;
	float pStepValueX;
	float pStepValueY;
	float pRedValue;
	float pGreenValue;
	float pBlueValue;
	int pModelId;
	int pAmountOfFilteredPixels;
	float pCentreX;
	float pCentreY;
	float pPerimeterValue;
	float pAreaValue;

	ModelObject(int pModelIdVar,
		float pTopLeftXVar,
		float pTopLeftYVar,
		float pImageWidthVar,
		float pImageHeightVar,
		float pStepValueVarX,
		float pStepValueVarY,
		float pRedValueVar,
		float pGreenValueVar,
		float pBlueValueVar)
	{
		pModelId = pModelIdVar;
		pTopLeftX = pTopLeftYVar;
		pTopLeftY = pTopLeftYVar;
		pImageWidth = pImageWidthVar;
		pImageHeight = pImageHeightVar;
		pStepValueX = pStepValueVarX;
		pStepValueY = pStepValueVarY,
		pRedValue = pRedValueVar;
		pGreenValue = pGreenValueVar;
		pBlueValue = pBlueValueVar;
		pAmountOfFilteredPixels = 0.0f;
		pPerimeterValue = 0.0f;
		pAreaValue = 0.0;
		pPixelObjectXScan = vector<PixelObject>();
		pPixelObjectYScan = vector<PixelObject>();
		pFilteredArray = vector<OutputPixelObjectFiltered>();
		pixelObjectSectorArrayRestored = vector<PixelObject>();
		pSectorAverages = vector<PixelObject>();
		pPixelObjectAfterLocalization = vector<OutputPixelObjectWithAngleValues>();
		pSectorOne = vector<OutputPixelObjectWithAngleValues>();
		pSectorTwo = vector<OutputPixelObjectWithAngleValues>();
		pSectorThree = vector<OutputPixelObjectWithAngleValues>();
		pSectorFour = vector<OutputPixelObjectWithAngleValues>();
		pGeneratedPointVector = vector<Point2D>();
		pGeneratedLineVector = vector <Line2D>();
	}

	~ModelObject()
	{
		pModelId = 0;
		pTopLeftX = 0.0F;
		pTopLeftY = 0.0F;
		pImageWidth = 0.0f;
		pImageHeight = 0.0f;
		pStepValueX = 0.0f;
		pStepValueY = 0.0f;
		pRedValue = 0.0f;
		pGreenValue = 0.0f;
		pBlueValue = 0.0f;
		pAmountOfFilteredPixels = 0.0f;
		pPerimeterValue = 0.0f;
		pAreaValue = 0.0f;
		pPixelObjectXScan.clear();
		pPixelObjectYScan.clear();
		pFilteredArray.clear();
		pixelObjectSectorArrayRestored.clear();
		pSectorAverages.clear();
		pPixelObjectAfterLocalization.clear();
		pSectorOne.clear();
		pSectorTwo.clear();
		pSectorThree.clear();
		pSectorFour.clear();
		pGeneratedPointVector.clear();
		pGeneratedLineVector.clear();
		pPerimeterValue = 0.0f;
		pAreaValue = 0.0f;

	}

	void SetPixelArrayXScan(vector<PixelObject> pPixelArrayToProcess)
	{
		pPixelObjectXScan = pPixelArrayToProcess;
	}

	void SetPixelArrayYScan(vector<PixelObject> pPixelArrayToProcess)
	{
		pPixelObjectYScan = pPixelArrayToProcess;
	}

	void SetFilteredArrayObject(vector<OutputPixelObjectFiltered> pFilterArrayToAttach)
	{
		pFilteredArray = pFilterArrayToAttach;
	}

	void SetRestoredPixelArray(vector<PixelObject> pPixelArrayRestoredVar)
	{
		pixelObjectSectorArrayRestored = pPixelArrayRestoredVar;
	}

	void SetSectorAverageArrays(vector<PixelObject> pSectorAverageVector)
	{
		pSectorAverages = pSectorAverageVector;
	}

	void SetLocalizedPixelArray(vector<OutputPixelObjectWithAngleValues> pLocalizedPixelArray)
	{
		pPixelObjectAfterLocalization = pLocalizedPixelArray;
	}

	void SetSectorOneVector(vector<OutputPixelObjectWithAngleValues> pSectorOneVector)
	{
		pSectorOne = pSectorOneVector;
	}

	void SetSectorTwoVector(vector<OutputPixelObjectWithAngleValues> pSectorTwoVector)
	{
		pSectorTwo = pSectorTwoVector;
	}


	void SetSectorThreeVector(vector<OutputPixelObjectWithAngleValues> pSectorThreeVector)
	{
		pSectorThree = pSectorThreeVector;
	}

	void SetSectorFourVector(vector<OutputPixelObjectWithAngleValues> pSectorFourVector)
	{
		pSectorFour = pSectorFourVector;
	}

	void SetGeneratedPointVector(vector<Point2D> pPointArrayVector)
	{
		pGeneratedPointVector = pPointArrayVector;
	}

	void SetGeneratedLineVector(vector<Line2D> pLineArrayVector)
	{
		pGeneratedLineVector = pLineArrayVector;
	}

	void SetPerimeterValue(float pPerimeterValueVar)
	{
		pPerimeterValue = pPerimeterValueVar;
	}
	
	void SetAreaValue(float pAreaValueVar)
	{
		pAreaValue = pAreaValue;
	}

	vector<PixelObject> GetPixelArrayXScal()
	{
		return pPixelObjectXScan;
	}

	vector<PixelObject>  GetPixelArrayYScan()
	{
		return pPixelObjectYScan;
	}

	vector<PixelObject> GetRestoredPixelArray()
	{
		return pixelObjectSectorArrayRestored;
	}

	vector<PixelObject>  GetSectorAverageArrays()
	{
		return pSectorAverages;
	}

	vector<OutputPixelObjectWithAngleValues> GetLocalizedPixelArray()
	{
		return  pPixelObjectAfterLocalization;
	}

	vector<OutputPixelObjectWithAngleValues> GetSectorOneVector()
	{
		return pSectorOne;
	}

	vector<OutputPixelObjectWithAngleValues> GetSectorTwoVector()
	{
		return pSectorTwo;
	}


	vector<OutputPixelObjectWithAngleValues> GetSectorThreeVector()
	{
		return pSectorThree;
	}

	vector<OutputPixelObjectWithAngleValues> GetSectorFourVector()
	{
		return pSectorFour;
	}


	vector<Point2D> GetPointArrayData()
	{
		return pGeneratedPointVector;
	}

	vector<Line2D> GetLineArrayData()
	{
		return pGeneratedLineVector;
	}

	float GetPerimeterValue()
	{
		return pPerimeterValue;
	}

	float GetAreaValue()
	{
		return pAreaValue;
	}

	int GetPixelArrayXScalArraySize()
	{
		return pPixelObjectXScan.size();
	}

	int GetPixelArrayYScalArraySize()
	{
		return pPixelObjectXScan.size();
	}

	int GetRestoredPixelArraySize()
	{
		return pixelObjectSectorArrayRestored.size();
	}

	int GetSectorAverageArraySize()
	{
		return pSectorAverages.size();
	}

	int GetSectorOneArraySize()
	{
		return pSectorOne.size();
	}


	int GetSectorTwoArraySize()
	{
		return pSectorTwo.size();
	}

	int GetSectorThreeArraySize()
	{
		return pSectorThree.size();
	}

	int GetSectorFourArraySize()
	{
		return pSectorFour.size();
	}

	vector<PixelObject> GetCommonArrayForProcessing()
	{
		pPixelObjectXScan.insert(pPixelObjectXScan.end(), pPixelObjectYScan.begin(), pPixelObjectYScan.end());
		return pPixelObjectXScan;
	}

	

};




class ColorObject
{
public:

	float pRedValue;
	float pGreenValue;
	float pBlueValue;

	ColorObject(float pColorRedVar,
		float pColorGreenVar,
		float pColorBlueVar)
	{
		pRedValue = pColorRedVar;
		pGreenValue = pColorGreenVar;
		pBlueValue = pColorBlueVar;
	}
	~ColorObject()
	{
		pRedValue = 0.0f;
		pGreenValue = 0.0f;
		pBlueValue = 0.0f;
	}
};

class EthalonObjectPixels
{
	

public:
	int pEthalonNumber;
	float pAmountOfPixelValues;
	int pAmountOfItems;
	float pRedColorValue;
	float pGreenColorValue;
	float pBlueColorValue;
	ColorObject pEthalonColor;

	EthalonObjectPixels(int pEthalonId,
		float pAmountOfPixels,
		int pAmountOfItemsInVector,
		float pRedValue,
		float pGreenValue,
		float pBlueValue) : pEthalonColor(pRedValue,
		pGreenValue, pBlueValue)
	{
		pEthalonNumber = pEthalonId;
		pAmountOfPixelValues = pAmountOfPixels;
		pAmountOfItems = pAmountOfItemsInVector;
		pRedColorValue = pRedValue;
		pGreenColorValue = pGreenValue;
		pBlueColorValue = pBlueValue;
		pEthalonColor = ColorObject(pRedValue,
			pGreenValue,
			pBlueValue);
	}
	~EthalonObjectPixels()
	{
		pEthalonNumber = 0;
		pAmountOfPixelValues = 0;
		pAmountOfItems = 0;
		pRedColorValue = 0.0f;
		pGreenColorValue = 0.0f;
		pBlueColorValue = 0.0f;
	}
	int GetAmountOfPixels()
	{
		return pAmountOfPixelValues;
	}
	int GetEthalonId()
	{
		return pEthalonNumber;
	}
	void SetEthalonColor(float pRedVar,
		float pGreenVar,
		float pBlueVar)
	{
		pRedColorValue = pRedVar;
		pGreenColorValue = pGreenVar;
		pBlueColorValue = pBlueVar;
		pEthalonColor.pRedValue = pRedVar;
		pEthalonColor.pGreenValue = pGreenVar;
		pEthalonColor.pBlueValue = pBlueVar;
	}
	ColorObject GetEthalonColor()
	{
		return pEthalonColor;
	}
};


using namespace std;

class DECLSPEC_UUID("7f601e3d-5638-4f3c-903c-e396ab5e8a21") DirectXGPUDigitalLabCoreModule : public DirectXComputeShaderManagementClass
{
	
public:

	HRESULT ValidatePairOfImagesUsingDescriptorAndCompareTheirResults(HINSTANCE hInstance, HWND hWnd);
	HRESULT ValidatePixelObjectUsingABoundaryDescriptorWithBoundariesSeparateRecoveriesThreaded(HINSTANCE hInstance, HWND hWnd);
	HRESULT DrawSomethingUsingGDIInterop(HINSTANCE hInstance, HWND hWnd);
	HRESULT CheckIfTwoImagesAreIdenticalInAThreadPoolViaClassOfMathtmaticalModels(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateObjectCountingOperationOnRGBImagesForRedPixelsAgainstEthalon(HINSTANCE hInstance, HWND hWnd);

	HRESULT ValidatePixelObjectUsingABoundaryDescriptorWithBoundariesSeparateRecoveriesSubtraction(HINSTANCE hInstance, HWND hWnd);

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

	HRESULT ValidatePixelObjectUsingABoundaryDescriptorWithBoundariesSeparateRecoveriesWithSpecifiedThreadSizeMultipleDimensions(HINSTANCE hInstance, HWND hWnd);
	HRESULT ValidatePixelObjectUsingABoundaryDescriptorWithBoundariesSeparateRecoveriesWithSpecifiedThreadSize(HINSTANCE hInstance, HWND hWnd);
	HRESULT ValidatePixelObjectUsingABoundaryDescriptorClosedMode(HINSTANCE hInstance, HWND hWnd);
	HRESULT ValidatePixelObjectUsingABoundaryDescriptorWithBoundariesSeparateRecoveries(HINSTANCE hInstance, HWND hWnd);
	HRESULT ValidatePixelObjectUsingABoundaryDescriptorWithBoundariesForADefinedFile(HINSTANCE hInstance, HWND hWnd);
	HRESULT ValidatePixelObjectUsingABoundaryDescriptorWithBoundariesWithVideoCapture(HINSTANCE hInstance, HWND hWnd);
	BOOL LocateImageFile(HWND hWnd, LPWSTR pszFileName, DWORD cbFileName);
	HRESULT DetectObjectOfSpatialShapeOnWhiteFontFourthExperimentRGBFilterWithSectorsAllocationOnGPUDynamicUsingDescriptors(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateRobotCleanerSystemForVideoFromFTPAutomaticAPISptialShapeAnalysisDemo(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateRobotCleanerSystemForVideoFromFTPAutomaticAPISptialShapeAnalysis(HINSTANCE hInstance, HWND hWnd,
		LPWSTR pFileNameVideoOne,
		LPWSTR pFileNameVideoTwo,
		int pXPosition, int pYPosition, int pImageWidth, int pImageHeight,
		float pRedValueToSelect,
		float pGreenValueTosSelect,
		float pBlueValueToSelect,
		UINT pStepY,
		UINT pStepX,
		int pWaitingInterval,
		vector<PixelObject> pPixelArrayToProcessOneYAxis,
		vector<PixelObject> pPixelArrayToProcessOneXAxis,
		vector<OutputPixelObjectFiltered> *ppOutputFilterOperationDataOne,
		vector<PixelObject> *ppOutputFilterOperationDataRestoredOne,
		vector<PixelObject> *ppOutputPixelArraySectoredOne,
		vector<OutputPixelObjectWithAngleValues> *ppCommonPixelArrayObjecOne,
		vector<OutputPixelObjectWithAngleValues> *ppSectorOneExperimentOne,
		vector<OutputPixelObjectWithAngleValues> *ppSectorTwoExperimentOne,
		vector<OutputPixelObjectWithAngleValues> *ppSectorThreeExperimentOne,
		vector<OutputPixelObjectWithAngleValues> *ppSectorFourExperimentOne,
		vector<Point2D> *ppPoinstArrayOne,
		vector<Line2D> *ppLineArrayOne,
		vector<PixelObject> pPixelArrayToProcessTwoYAxis,
		vector<PixelObject> pPixelArrayToProcessTwoXAxis,
		vector<OutputPixelObjectFiltered> *ppOutputFilterOperationDataTwo,
		vector<PixelObject> *ppOutputFilterOperationDataRestoredTwo,
		vector<PixelObject> *ppOutputPixelArraySectoredTwo,
		vector<OutputPixelObjectWithAngleValues> *ppCommonPixelArrayObjecTwo,
		vector<OutputPixelObjectWithAngleValues> *ppSectorOneExperimentTwo,
		vector<OutputPixelObjectWithAngleValues> *ppSectorTwoExperimentTwo,
		vector<OutputPixelObjectWithAngleValues> *ppSectorThreeExperimentTwo,
		vector<OutputPixelObjectWithAngleValues> *ppSectorFourExperimentTwo,
		vector<Point2D> *ppPoinstArrayTwo,
		vector<Line2D> *ppLineArrayTwo,
		float* pPeriemerValueOne, float* pAreaValueOne,
		float* pPeriemerValueTwo, float* pAreaValueTwo,
		float* pPerimeterDifference, float* pAreaDifference);
	HRESULT DetectStraightLineWithinImagesForYPosition(HINSTANCE hInstance, HWND hWnd);

	HRESULT CheckIfTwoImagesAreIdenticalThreadedOnGPU(HINSTANCE hInstance, HWND hWnd);
	HRESULT CheckIfTwoImagesAreIdenticalDecomposedVersion(HINSTANCE hInstance, HWND hWnd);
	HRESULT ThreshHoldGrayScaleImagePixelByCriteriaWithRecoveryWithStepsDefined(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateCarNumberDetectionOperationFromImageUsingBoundaryDescriptorWithDefinedDelta(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateCarNumberDetectionOperationFromImageUsingBoundaryDescriptor(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateCarNumberDetectionOperationFromImage(HINSTANCE hInstance, HWND hWnd);
	vector<EthalonObjectPixels> pEthalonVectorObject;
	ColorObject ComputeColorDifference(ColorObject pColorOne, ColorObject pColotTwo);
	EthalonObjectPixels GenerateNewEthalonForAPixelVector(int pEthalonId, float pAmountOfPixels, int pAmountOfItemsInVector,
		float pRedValueToSelect,
		float pGreenValueToSelect,
		float pBlueValueToSelect);
	EthalonObjectPixels GetEthalonById(int pEthalonId);

	int ComputeTheAmountOfObjectsAgainstEthalon(EthalonObjectPixels pEthatonObject, vector<PixelObject> pPixelArrayToValidate);
	HRESULT SimulateObjectCountingOperationOnRGBImagesForGreenPixels(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateObjectCountingOperationOnRGBImagesForBluePixels(HINSTANCE hInstance, HWND hWnd);

	HRESULT SelectPixelsFromImageWithPredominantBlueColor(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateObjectCountingOperationOnRGBImagesForRedPixels(HINSTANCE hInstance, HWND hWnd);
	HRESULT SelectPixelsFromImageWithPredominantGreenColor(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateObjectCountingOperationOnRGBImages(HINSTANCE hInstance, HWND hWnd);
	HRESULT DetectObjectOfSpatialShapeOnWhiteFontFourthExperimentRGBFilterWithSectorsAllocationOnGPUDynamicUsingAPI(HINSTANCE hInstance, HWND hWnd);
	HRESULT SelectPixelsFromImageWithPredominantRedColor(HINSTANCE hInstance, HWND hWnd);

	HRESULT PerformSpatialShapeAnalysisForMultipleStepsOption(HINSTANCE hInstance,
		HWND hWnd);



	HRESULT ExecuteSpatialShapeObjectOnSelectedImageForAModelObject(HINSTANCE hInstance, HWND hWnd,
		ModelObject pModelObject, ModelObject* pModelOutput);

	HRESULT DetectObjectOfSpatialShapeOnWhiteFontFourthExperimentRGBFilterWithSectorsAllocationOnGPUDynamicWithPreprocessedPixels(HINSTANCE hInstance, HWND hWnd,
		float pTopLeftX, float pTopLeftY,
		float pImageWidth, float pImageHeight,
		float pStepXValue, float pStepYValue,
		float pRedValueToSelect, float pGreenValueToSelect,
		float pBlueValueToSelect,
		vector<PixelObject> pPixelArrayToProcess,
		vector<OutputPixelObjectFiltered> *ppOutputFilterOperationData,
		vector<PixelObject> *ppOutputFilterOperationDataRestored,
		vector<PixelObject> *ppOutputPixelArraySectored,
		vector<OutputPixelObjectWithAngleValues> *ppCommonPixelArrayObject,
		vector<OutputPixelObjectWithAngleValues> *ppSectorOneVector,
		vector<OutputPixelObjectWithAngleValues> *ppSectorTwoVector,
		vector<OutputPixelObjectWithAngleValues> *ppSectorThreeVector,
		vector<OutputPixelObjectWithAngleValues> *ppSectorFourVector,
		vector<Point2D> *ppPoinstArray,
		vector<Line2D> *ppLineArray,
		float* pPeriemerValue, float* pAreaValue);




	float ComputePolygonAreaUsingShoelaceFormula(vector<Point2D> pPointArray, int pArraySize);
	HRESULT  DetectObjectOfSpatialShapeOnWhiteFontFourthExperimentRGBFilterWithSectorsAllocationOnGPUDynamic(HINSTANCE hInstance, HWND hWnd);
	HRESULT DetectObjectOfSpatialShapeOnWhiteFontFourthExperimentRGBFilterWithSectorsAllocationOnGPU(HINSTANCE hInstance, HWND hWnd);
	HRESULT DetectObjectOfSpatialShapeOnWhiteFontFourthExperimentRGBFilterWithYConstantAndXBreakpoint(HINSTANCE hInstance, HWND hWnd);
	HRESULT DetectObjectOfSpatialShapeOnWhiteFontFourthExperimentRGBFilterWithXConstantAndYBreakpoint(HINSTANCE hInstance, HWND hWnd);
	HRESULT FilterSelectedImageForXPositionValue(HINSTANCE hInstance, HWND hWnd);
	HRESULT FilterSelectedImageForYPositionValue(HINSTANCE hInstance, HWND hWnd);
	HRESULT ComputeAverageXForPixelArrayOnAGPU(HINSTANCE hInstance, HWND hWnd,
		float pYValue,
		float pXThresold,
		int pSectorOne,
		int pSectorTwo,
		int pInvalidSector,
		vector<PixelObject> pInputArrayObject,
		vector<PixelObject> *ppPartAVector,
		vector<PixelObject> *ppPartBVector,
		float* ppAverageX0, float *ppAverageX1);

	HRESULT ComputeAverageYForPixelArrayOnAGPU(HINSTANCE hInstance, HWND hWnd,
		float pXValue,
		float pYThresold,
		int pSectorOne,
		int pSectorTwo,
		int pInvalidSector,
		vector<PixelObject> pInputArrayObject,
		vector<PixelObject> *ppPartAVector,
		vector<PixelObject> *ppPartBVector,
		float* pAverageY0, float *pAverageY1);



	HRESULT DetectMultiColorPixelsInsideImagesWithRecovery(HINSTANCE hInstance, HWND hWnd);
	HRESULT ComputePixelDistanceFromCentreWithSpecifiedColor(HINSTANCE hInstance, HWND hWnd);
	HRESULT LocalizePixelsOfAnImageAgainstSelectedCentre(HINSTANCE hInstance, HWND hWnd);
	HRESULT DrawSampleTextOnTheScreen(HINSTANCE hInstance, HWND hWnd);
	HRESULT MultiplyImageIntensityByANumber(HINSTANCE hInstance, HWND hWnd);
	HRESULT DivideImageIntensityByANumber(HINSTANCE hInstance, HWND hWnd);
	HRESULT SubtractValuesFromColorsOfRGBImageWithResultsRestorationForIdenticalPixels(HINSTANCE hInstance, HWND hWnd);
	HRESULT SubtractValuesFromColorsOfRGBImageWithResultsRestorationForNonIdenticalPixels(HINSTANCE hInstance, HWND hWnd);
	HRESULT DetectObjectOfSpatialShapeOnWhiteFontFourthExperimentRGBFilterViaPixelSubrtractionIdenticalPixels(HINSTANCE hInstance, HWND hWnd);
	HRESULT DetectObjectOfSpatialShapeOnWhiteFontFourthExperimentRGBFilterViaPixelSubrtraction(HINSTANCE hInstance, HWND hWnd);
	HRESULT DetectObjectOfSpatialShapeOnWhiteFontFourthExperimentRGBFilterViaPixelAddition(HINSTANCE hInstance, HWND hWnd);
	HRESULT SubtractValuesFromColorsOfRGBImageWithResultsRestoration(HINSTANCE hInstance, HWND hWnd);
	HRESULT ThreshHoldRGBImagePixelByCriteriaWithRecoveryWithStepsDefined(HINSTANCE hInstance, HWND hWnd);
	HRESULT ConvertRGBImageToQRCodeModelWithQRGeneration(HINSTANCE hInstance, HWND hWnd);
	HRESULT AddInternsityValuesToColorsOfRGBImage(HINSTANCE hInstance, HWND hWnd);
	HRESULT SubtractValuesFromColorsOfRGBImage(HINSTANCE hInstance, HWND hWnd);
	HRESULT ConvertRGBImageToQRCodeModel(HINSTANCE hInstance, HWND hWnd);
	HRESULT ValidatePixelObjectUsingABoundaryDescriptorWithBoundaries(HINSTANCE hInstance, HWND hWnd);
	HRESULT ValidatePixelObjectUsingABoundaryDescriptorWithThresold(HINSTANCE hInstance, HWND hWnd);

	HRESULT CheckIfTwoImagesAreIdenticalRGBAgainstSyntheticImages(HINSTANCE hInstance, HWND hWnd);
	HRESULT DetectObjectOfSpatialShapeOnWhiteFontSeventhExperiment(HINSTANCE hInstance, HWND hWnd);
	HRESULT DetectObjectOfSpatialShapeOnWhiteFontFourthExperimentRGBFilterWithSorting(HINSTANCE hInstance, HWND hWnd);
	HRESULT SortPixelsInTheRightOrderForYPosition(HINSTANCE hInstance, HWND hWnd);
	HRESULT DetectPixelOrientationAgainstAPoint(HINSTANCE hInstance, HWND hWnd);
	HRESULT ConvertRGBImageToCMYK(HINSTANCE hInstance, HWND hWnd);
	HRESULT SortPixelsInTheRightOrderForXPosition(HINSTANCE hInstance, HWND hWnd);
	HRESULT SortPixelsInTheRightOrder(HINSTANCE hInstance, HWND hWnd);
	HRESULT CheckIfTwoImagesAreIdenticalAgainstLargeImageParts(HINSTANCE hInstance, HWND hWnd);
	HRESULT ComputePixelDeltaForImageDirect(HINSTANCE hInstance, HWND hWnd);
	HRESULT DectectLinesOnWhiteFontExperimentViaThreePoints(HINSTANCE hInstance, HWND hWnd);
	HRESULT ConnectPointsUsingLines(HINSTANCE hInstance, HWND hWnd);
	HRESULT DetectStraightLineUsingHoughTransform(HINSTANCE hInstance, HWND hWnd);
	HRESULT ComputeHypotenuseValues(HINSTANCE hInstance, HWND hWnd);
	float ComputeLineSlope(float pXPositionOne, float pYPositionOne, float pXPositionTwo, float pYPositionTwo);
	float ComputeLineLength(float pXPositionOne, float pYPositionOne, float pXPositionTwo, float pYPositionTwo);
	HRESULT DetectSquareOnWhiteFont(HINSTANCE hInstance, HWND hWnd);
	HRESULT DetectObjectOfSpatialShapeOnWhiteFontSixthExperimentRGBFilter(HINSTANCE hInstance, HWND hWnd);
	HRESULT DetectStraightLineWithinImagesUsingPoints(HINSTANCE hInstance, HWND hWnd);
	HRESULT DetectStraightLineWithinImagesForXPosition(HINSTANCE hInstance, HWND hWnd);
	HRESULT DetectStraightLineWithinImagesUsingALinearFunction(HINSTANCE hInstance, HWND hWnd);
	HRESULT ComputeDeltaFunctionForImages(HINSTANCE hInstance, HWND hWnd);
	HRESULT DetectCircularClipWithinImages(HINSTANCE hInstance, HWND hWnd);
	HRESULT DetectPositionAgainstXValues(HINSTANCE hInstance, HWND hWnd);
	HRESULT ComputePixelDeltaForImage(HINSTANCE hInstance, HWND hWnd);
	HRESULT SelectPixelsOfColorFromImage(HINSTANCE hInstance, HWND hWnd);
	HRESULT DetectObjectOfSpatialShapeOnWhiteFontFifthExperimentRGBFilter(HINSTANCE hInstance, HWND hWnd);
	HRESULT ConvertImageFromRGBModelLinkedListModel(HINSTANCE hInstance, HWND hWnd);
	HRESULT DetectObjectOfSpatialShapeOnWhiteFontFourthExperimentRGBFilter(HINSTANCE hInstance, HWND hWnd);
	HRESULT ConvertImageFromRGBModelToTemperatureModel(HINSTANCE hInstance, HWND hWnd);
	HRESULT DetectPixelsOnAVerticalLine(HINSTANCE hInstance, HWND hWnd);
	HRESULT DetectPixelSOnAHorizontalLine(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateInterestingPointCoordinatesDetector(HINSTANCE hInstance, HWND hWnd);
	HRESULT DetectObjectOfSpatialShapeOnWhiteFont(HINSTANCE hInstance, HWND hWnd);
	HRESULT DetectObjectOfSpatialShapeOnWhiteFontSecondExperiment(HINSTANCE hInstance, HWND hWnd);
	HRESULT DetectObjectOfSpatialShapeOnWhiteFontThirddExperiment(HINSTANCE hInstance, HWND hWnd);
	HRESULT ClipSelectedImageWithAStep(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateInterestingPointDetector(HINSTANCE hInstance, HWND hWnd);
	HRESULT CheckIfTwoImagesAreIdenticalInAThreadPool(HINSTANCE hInstance, HWND hWnd);
	HRESULT CheckIfTwoImagesAreIdenticalRGBThreadedWithPixelMarking(HINSTANCE hInstance, HWND hWnd);
	HRESULT CheckIfTwoImagesAreIdenticalRGBThreadedUsingSemaphores(HINSTANCE hInstance, HWND hWnd);
	HRESULT CheckIfTwoImagesAreIdenticalRGBThreaded(HINSTANCE hInstance, HWND hWnd);
	HRESULT CheckIfTwoImagesAreIdenticalRGBWithBundleCreation(HINSTANCE hInstance, HWND hWnd);
	HRESULT CheckIfTwoImagesAreIdenticalRGB(HINSTANCE hInstance, HWND hWnd);
	HRESULT ApplyBinaryFilterForSelectedImageFile(HINSTANCE hInstance, HWND hWnd);
	HRESULT LookForColoredObjectInImage(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateCurrencyStateMachineObject(HINSTANCE hInstance, HWND hWnd);
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

	HRESULT ThreshHoldImagePixelByCriteriaRGBWithoutExclusion(HINSTANCE hInstance, HWND hWnd);

	HRESULT SimulateRobotCleanerSystemObjectViaImageComparisonForRGBImagesSelectedFilesThreaded(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateRobotCleanerSystemObjectViaImageComparisonForRGBImagesSelectedFiles(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateRobotCleanerSystemObjectViaImageComparisonForRGBImagesAutomatic(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateRobotCleanerSystemObjectViaImageComparisonForRGBImagesAutomaticThreaded(HINSTANCE hInstance, HWND hWnd);

	HRESULT SimulateRobotCleanerSystemObjectViaImageComparisonForRGBImagesAutomatic(HINSTANCE hInstance, HWND hWnd,
		LPWSTR pFileNameVideoOne, LPWSTR pFileNameVideoTwo, int pXPosition, int pYPosition, int pImageWidth, int pImageHeight, int pWaitingInterval,
		vector<PixelObject> *pImageOneRGBPixels, vector<PixelObject> *pImageTwoRGBPixels, vector<OutputPixelBufferRGB> *ppOutputPixelBufferArray,
		vector<OutputPixelBufferRGB> *ppIdenticalPixelBufferArray, vector<OutputPixelBufferRGB> *ppDifferentPixelBufferArray);

	HRESULT SimulateRobotCleanerSystemObjectViaImageComparisonForRGBImagesThreaded(HINSTANCE hInstance, HWND hWnd,
		LPWSTR pFileNameVideoOne, LPWSTR pFileNameVideoTwo, int pXPosition, int pYPosition, int pImageWidth, int pImageHeight, int pWaitingInterval,
		vector<PixelObject> *pImageOneRGBPixels, vector<PixelObject> *pImageTwoRGBPixels, vector<OutputPixelBufferRGB> *ppOutputPixelBufferArray,
		vector<OutputPixelBufferRGB> *ppIdenticalPixelBufferArray, vector<OutputPixelBufferRGB> *ppDifferentPixelBufferArray);


	HRESULT SimulatePathGeometryExperiment(HINSTANCE hInstance, HWND hWnd);

	HRESULT FilterOutRGBImageByCriteria(HINSTANCE hInstance,
		HWND hWnd, vector<PixelObject> pInputPixelObjectArray,
		float pMinimumX,
		float pMinimumY,
		float pMaximumX,
		float pMaximumY,
		float pMinimumRed,
		float pMaximumRed,
		float pMinimumGreen, float pMaximumGreen, float pMinimumBlue, float pMaximumBlue, vector<OutputPixelObjectFiltered> *pFilterResults);

	HRESULT PerformTheComplexAnalysisOfImage(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateFincancialSpaceModule(HINSTANCE hInstance, HWND hWnd);
	HRESULT ClipImageAgainstABitmapMask(HINSTANCE hInstance, HWND hWnd);
	HRESULT CompressComplexImage(HINSTANCE hInstance, HWND hWnd);
	HRESULT TestHuffmanTableBehaviour(HINSTANCE hInstance, HWND hWnd);
	HRESULT ApplyCosineTransformToBlock(HINSTANCE hInstance,
		HWND hWnd, vector<YCBRPixelDataBuffer> pOutputArrayCosine,
		UINT pTopLeftX,
		UINT pTopLeftY,
		UINT pImageWidth,
		UINT pImageHeight,
		vector<YCBRPixelDataBufferOutputCosine> *pCosineBlock,
		vector<float> *ppYCosineArray,
		vector<float> *ppCBCosineArray,
		vector<float> *ppbCRCosineArray);

	HRESULT ExperimentWithComplexNumbers(HINSTANCE hInstance, HWND hWnd);


	HRESULT SimulateRobotCleanerSystemForVideoFromFTPAutomaticAPICriticalSections(HINSTANCE hInstance, HWND hWnd,
		LPWSTR pFileNameVideoOne, LPWSTR pFileNameVideoTwo,
		int pXPosition, int pYPosition, int pImageWidth, int pImageHeight, int pWaitingInterval,
		vector<PixelObject> *pImageOneRGBPixels,
		vector<PixelObject> *pImageTwoRGBPixels,
		vector<PixelObjectGrayScale> *pImageOneGrayScale,
		vector<PixelObjectGrayScale> *pImageTwoGrayScale,
		vector<OutputPixelBuffer> *ppOutputPixelBufferArray,
		vector<PixelObjectGrayScale> *pppIdenticalPixelOutputArray,
		vector<PixelObjectGrayScale> *pppDifferentPixelOutputArray);

	
	HRESULT CheckIfTwoImagesAreIdenticalApi(HINSTANCE hInstance, HWND hWnd,
		vector<PixelObjectGrayScale> ppArrayOne,
		vector<PixelObjectGrayScale> ppArrayTwo,
		vector<OutputPixelBuffer> *ppOutputPixelGrayScale,
		vector<PixelObjectGrayScale> *ppIdenticalPixelVarables,
		vector<PixelObjectGrayScale> *ppDifferentPixelVariables);
	HRESULT ConvertRGBImageToGrayScaleInAMutex(HINSTANCE hInstance, HWND hWnd);

	HRESULT SimulateImageClippingExperimentBinaryTree(HINSTANCE hInstance, HWND hWnd);
	PixelObject GetPixelObjectByXAndYPositiion(vector<PixelObject> pPixelObjectVector, float pXPosition, float pYPosition);

	HRESULT SimulateWaitableTimerOperationForRobotCleanerSkype(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateRobotCleanerAsyncronousModeSkypeDemoWithCriticalSections(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateBehaviourOfRobotCleanerUsingSemaphores(HINSTANCE hInstance,
		HWND hWnd);
	HRESULT SimulateProducerConsumerSystem(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateImageClippingExperimentWithSegmentation(HINSTANCE hInstance, HWND hWnd, int xPosition, int yPosition, int pImageWidth, int pImageHeight,
		vector<PixelObject> *ppPixelObjectArray);
	HRESULT SimutateQuadraticFunctionAlgorithmUsingMutexObject(HINSTANCE hInstance, HWND hWnd);
	HRESULT CheckIfTwoImagesAreIdenticalThreaded(HINSTANCE hInstance, HWND hWnd);
	HRESULT DestroyFilePermanently(HINSTANCE hInstance, HWND hWnd, LPWSTR pFileName);
	HRESULT ConvertRGBImageToYCBCRColorSpace(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateJPEGRestorationAlgorithmUsingCosineTransform(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateJPEGDecompressionAlgorithmUsingCosineTransform(HINSTANCE hInstance, HWND hWnd);

	HRESULT ApplyKMeansToRGBImage(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateRobotCleanerSystemForVideoFromFTPAutomaticAPI(HINSTANCE hInstance, HWND hWnd,
		LPWSTR pFileNameVideoOne, LPWSTR pFileNameVideoTwo,
		int pXPosition, int pYPosition, int pImageWidth, int pImageHeight, int pWaitingInterval,
		vector<PixelObject> *pImageOneRGBPixels,
		vector<PixelObject> *pImageTwoRGBPixels,
		vector<PixelObjectGrayScale> *pImageOneGrayScale,
		vector<PixelObjectGrayScale> *pImageTwoGrayScale,
		vector<OutputPixelBuffer> *ppOutputPixelBufferArray,
		vector<PixelObjectGrayScale> *pppIdenticalPixelOutputArray,
		vector<PixelObjectGrayScale> *pppDifferentPixelOutputArray);
	HRESULT SimulateRobotCleanerSystemForVideoFromFTPAutomaticAPIMotionDetected(HINSTANCE hInstance, HWND hWnd,
		LPWSTR pFileNameVideoOne, LPWSTR pFileNameVideoTwo,
		int pXPosition, int pYPosition, int pImageWidth, int pImageHeight, int pWaitingInterval,
		vector<PixelObject> *pImageOneRGBPixels,
		vector<PixelObject> *pImageTwoRGBPixels,
		vector<MotionPixelObjectRGB> *ppOutputPixelBufferArray);
	HRESULT DetectStraightLineWithinImagesForXPositionConstant(HINSTANCE hInstance, HWND hWnd);
	HRESULT CompressRGBPixelVectorToImageFile(HINSTANCE hInstance, HWND hWnd, UINT pImageWidth, UINT pImageHeight, vector<PixelObject> pPixelObjectArray, LPCWSTR pImageFile);
	HRESULT DrawPixelArrayOnTheScreen(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateImageClippingExperimentWithoutDirect2D(HINSTANCE hInstance, HWND hWnd);
	HRESULT DrawRenctangleSet(HINSTANCE hInstance, HWND hWnd);
	HRESULT ClearRenctangleSet(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateRobotCleanerAsyncronousModeSkypeMotionDetection(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateRobotCleanerSystemObjectViaMotionDetectionSystemWithCapture(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateRobotCleanerSystemObjectViaMotionDetectionSystemFromSelectedVideos(HINSTANCE hInstance, HWND hWnd);
	HRESULT ApplyKMeansToGrayScaleImage(HINSTANCE hInstance, HWND hWnd);
	HRESULT DestroyWhiteFontFromGraph(HINSTANCE hInstance, HWND hWnd);
	HRESULT ExtractPointsFromGraph(HINSTANCE hInstance, HWND hWnd);
	HRESULT ConvertRGBImageToGrayScaleUsingInts(HINSTANCE hInstance, HWND hWnd);
	HRESULT DetectMotionInRGBImages(HINSTANCE hInstance, HWND hWnd);
	HRESULT ThreshHoldImagePixelByCriteriaWithRecovery(HINSTANCE hInstance, HWND hWnd);
	HRESULT ThreshHoldRGBImagePixelByCriteriaWithRecovery(HINSTANCE hInstance, HWND hWnd);
	HRESULT ClearTheScreenBuffer(HINSTANCE hInstance, HWND hWnd);
	vector<RobotCleanerReportObject> pRobotCleanerObjectReportArray;
	Direct2DImageProcessingManager* pImageProcessingManager;
	void ElevateProcessStatus(HINSTANCE hInstance, HWND hWnd);
	void ReportError(LPCWSTR pszFunction);
	bool IsRunAsAdministrator();
	DirectXStandardAlgorithmExecutor* m_pStandardAlgorithmExecutor;
	DirectXGPUDigitalLabClassFactory* md3dgpudigitallabclassFactory;
	ITypeInfo* m_pTypeInfo;
	HRESULT SimulateRobotCleanerAsyncronousModeDemo(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateActivControlModule(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateRobotCleanerSystemForVideoFromFTP(HINSTANCE hInstance, HWND hWnd);
	HRESULT DownloadSampleFileFromWeb(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateCurrencyMetalRelationshipModels(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateCurrencyCurrencyRelationshipModels(HINSTANCE hInstance,
		HWND hWnd,
		UINT pCurrencyId,
		UINT pMetalId,
		DateDataType pCurrentDate,
		vector<CurrencyObject> ppCurrencyObjectCollection0,
		vector<CurrencyObject> ppCurrencyObjectCollection1,
		vector<CurrencyCurrencyPairObject> *pOutputResourceObject);
	HRESULT InitializeBluetoothServerSocket(HINSTANCE hInstance, HWND hWND);
	HRESULT SimulateCurrencyCurrencyRelationshipModels(HINSTANCE hInstance, HWND hWnd);
	HRESULT RecoverRGBImageFromTextFile(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateRobotCleanerSystemForVideoWindowsMedia(HINSTANCE hInstance, HWND hWnd);
	HRESULT SaveScreenDataToImage(HINSTANCE hInstance, HWND hWnd);
	HRESULT TryBinaryTreeExperiment(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateRobotCleanerSystemObjectViaImageComparisonFromVideoArchiveExistingFiles(HINSTANCE hInstance, HWND hWnd);
	HRESULT LaunchImageComparisonOperationInAThread(HINSTANCE hInstance, HWND hWnd);
	HRESULT CheckIfTwoImagesAreIdenticalParametrized(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateUpwardFinancialTrend(HINSTANCE hInstance, HWND hWnd);
	HRESULT __stdcall GetTypeInfoCount(UINT* pCountTypeInfo) { *pCountTypeInfo = 1;  return S_OK; };
	HRESULT __stdcall GetTypeInfo(UINT iTypeInfo, LCID lcid, ITypeInfo** ppITypeInfo);
	HRESULT __stdcall GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId)
	{
		return DispGetIDsOfNames(m_pTypeInfo, rgszNames, cNames, rgDispId);
	};
	HRESULT __stdcall Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr)
	{
		return DispInvoke(this, m_pTypeInfo, dispIdMember, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
	}
	HRESULT __stdcall Double(int x, int *retval);
	long m_cRef;
	Direct2DManagementClass* m_pD2DManagementClass;
	DirectXCloudServerConnectionManager* m_pCloudServerConnectionManager;
	DirectXComputeShaderManagementClass* m_pGPUComputeShaderManagerClass;
	Direct2DManagementClass* md2dmanager;
	Direct2DImageProcessingManager* pD2DManagementClass;
	DirectXJobManagmentClass* m_pGPUJobManagementClass;
	DirectXKnowledgeDatabaseManagementClass* m_pKnowledgeDatabaseManager;
	DirectXConnectedApplicationLauncher* m_pConnectedApplicationLauncher;
	DirectXVideoAnalyticsManagementEngineClass* pVideoAnalyticsManagementClass;
	DirectXCloudServerConnectionManager* m_pCloudServerConnectionManagerClass;
	~DirectXGPUDigitalLabCoreModule(void);
	ULONG m_nRefCount;
	HRESULT StartMultiagentSimulation(HINSTANCE hInstance, HWND hWnd);
	HRESULT OpenImageFileForProcessing(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateImageSubtractionAlgorithm(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateImageClippingExperiment(HINSTANCE hInstance, HWND hWnd);
	HRESULT GenerateIndexedFileNamesForVideos(HINSTANCE hInstance, HWND hWnd);
	HRESULT GenerateIndexedFileNamesForImages(HINSTANCE hInstance, HWND hWnd);
	HRESULT GenerateIndexedFileNamesForImages(HINSTANCE hInstance, HWND hWnd, vector<std::string> *pOutputVectorStreamForImages);
	HRESULT GenerateIndexedFileNamesForVideos(HINSTANCE hInstance, HWND hWnd, vector<std::string> *pOutputVectorStreamForVideos);
	HRESULT SimulateImageClippingExperimentWithCoordinates(HINSTANCE hInstance,
		HWND hWnd,
		UINT pXPosition,
		UINT pYPosition,
		UINT pClipWidth,
		UINT pClipHeight,
		vector<PixelObject> *ppPixelObjectArray);
	std::stack<int> ConvertIntToStack(int pNumber);
	HRESULT SimulateImagePixelClassificationExperiment(HINSTANCE hInstance, HWND hWnd);
	HRESULT OpenVideoFileForProcessing(HINSTANCE hInstance, HWND hWnd);
	HRESULT PlayVideo(HINSTANCE hInstance, HWND hWnd);
	HRESULT PauseVideo(HINSTANCE hInstance, HWND hWnd);
	HRESULT StopVideo(HINSTANCE hInstance, HWND hWnd);
	HRESULT InitializeRPCClient(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateRobotCleanerAsyncronousMode(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateRobotCleanerSystemForVideoFromFTPAutomatic(HINSTANCE hInstance, HWND hWnd,
	LPWSTR pFileNameVideoOne, LPWSTR pFileNameVidewTwo,
	int pXPosition, int pYPosition, int pImageWidth, int pImageHeight, int pWaitingInterval);
	HRESULT SimulateRobotCleanerAsyncronousModeSkypeDemo(HINSTANCE hInstance, HWND hWnd);

	HRESULT SimulateParallelReductionAlgorithm(HINSTANCE hInstance, HWND hWnd);
	BOOL LocateCSVFile(HWND hWnd, LPWSTR pszFileName, DWORD cbFileName);
	HRESULT SimulateStandardDeviationExperiment(HINSTANCE hInstance, HWND hWnd);
	HRESULT ConvertAnArrayOfNumbersToButterflyForm(HINSTANCE hInstance, HWND hWnd);
	DirectXGPUDigitalLabCoreModule(HINSTANCE hInstance, HWND hWnd);
	DirectXGPUDigitalLabCoreModule();
 	HRESULT InitializeClientSocket(HINSTANCE hInstance, HWND hWnd);
	HRESULT LoadSkypeApplication(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulatePipeServerCollaborationForRobotCleaner(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateRobotCleanerSystemObjectViaImageComparisonFromSelectedFilesStateMachines(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateRobotCleanerSystemObjectViaImageComparisonFromSelectedFilesStateMachinesIntensity(HINSTANCE hInstance, HWND hWnd);
	HRESULT InitializeServerSocketRobotCleaner(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateRobotCleanerSystemObjectViaImageSubtraction(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateRobotCleanerSystemObjectViaImageComparison(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateRobotCleanerSystemObjectViaImageComparisonFromExistingFiles(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateRobotCleanerSystemObjectViaImageComparisonFromSelectedFiles(HINSTANCE hInstance, HWND hWnd);
	HRESULT DrawRoadSign(HINSTANCE hInstance, HWND hWnd, UINT pLevelOfIdentity);
	HRESULT SimulateImageProcessingFiniteStateMachineIntensityFunction(HINSTANCE hInstance, HWND hWnd);
	HRESULT GenerateVideoArchiveObject(HINSTANCE hInstance, HWND hWnd);
	HRESULT GenerateIndexedFileNamesForVideosParametrized(HINSTANCE hInstance, HWND hWnd, vector<std::string> *ppVideoFileArray);
	HRESULT SimulateRobotCleanerSystemObjectViaImageComparisonFromVideoArchive(HINSTANCE hInstance, HWND hWnd);
	HRESULT STDMETHODCALLTYPE ShutdownSimulationSystem(HINSTANCE hInstance, HWND hWnd);
	HRESULT STDMETHODCALLTYPE ShutdownSimulationSystem();
	HRESULT ReadDataFromPipeMethod(HINSTANCE hInstance, HWND hWnd);
	HRESULT WriteDataToPipeMethod(HINSTANCE hInstance, HWND hWnd);
	HRESULT TerminateNamedPipe(HINSTANCE hInstance, HWND hWnd);
	HRESULT OpenWebBrowserApplication(HINSTANCE hInstance, HWND hWnd);
	HRESULT OpenImageFileForProcessingWithDimensions(HINSTANCE hInstance, HWND hWnd, vector<PixelObject> *ppPixelObjectArray);
	HRESULT SegmentImageIntoRectangularSectors(HINSTANCE hInstance, HWND hWnd);
	PixelObjectGrayScale GetImagePixelObjectByXAndYPosition(vector
		<PixelObjectGrayScale> ppImageDerivativeList, float pXPosition, float pYPosition);
	HRESULT ConvertRGBImageToGrayScaleViaLuminosityParameters(HINSTANCE hInstance, HWND hWnd);
	HRESULT STDMETHODCALLTYPE StartDataGetRequest();
	HRESULT STDMETHODCALLTYPE InitializeGPUDigitalLabKernel();
	HRESULT STDMETHODCALLTYPE InitializeConnectionToDataEngine();
	HRESULT STDMETHODCALLTYPE InitializeMultiAgentSystem();
	HRESULT STDMETHODCALLTYPE InitializeGraphicsEngineModule();
	HRESULT STDMETHODCALLTYPE CreateMathematicalModelObject();
	HRESULT STDMETHODCALLTYPE CreateScientificExperimentObject();
	HRESULT STDMETHODCALLTYPE SimulateArrayAdditionAlgorithm();
	HRESULT STDMETHODCALLTYPE SimulateSortingAlgorithm();
	HRESULT STDMETHODCALLTYPE InitializeSimulationSystem(); // Инициализация Системы GPUDigitalLab (ссылка на основной модуль windiws, дескриптор окна)
	HRESULT STDMETHODCALLTYPE InitializeGPUDigitalLabCore(); //Инициализация ядра Системы (ссылка на основной модуль windiws, дескриптор окна)
	HRESULT STDMETHODCALLTYPE InitializeUserAccountController(); // Инициализация контроллера пользовательских сеансов
	HRESULT STDMETHODCALLTYPE LogonGPUDigitalLabUser(LPCSTR fUserName, LPCSTR fPassword); //Вход пользователя в систему
	HRESULT STDMETHODCALLTYPE CreateUserProcessingSession(); //Запуск обработчика действий пользователя
	HRESULT STDMETHODCALLTYPE InitializeEventProcessingModule(); //Запуск обработчика событий системы
	HRESULT STDMETHODCALLTYPE CreateAppicationDomainManagerClass(); //Запуск менеджера доменов приложений
	HRESULT STDMETHODCALLTYPE CreateApplicationDomainInstanceClass(); //Запуск обработчика сеансов пользователя
	HRESULT STDMETHODCALLTYPE InitializeModuleProcessingWidgets(); //Запуск контроллера программных модулей
	HRESULT STDMETHODCALLTYPE CreateFluidDynamicsAnimation(LPCSTR fName); //Запуск модуля механики жидкостей
	HRESULT STDMETHODCALLTYPE CreateIndustrialSimulationAnimation(LPCSTR fProgrammingScript, LPCSTR fDataArchive); //Запуск Симулятора производственных процессов
	HRESULT STDMETHODCALLTYPE CreateDMOLSimulationAnimation(LPCSTR fProgrammingScript, LPCSTR fDataArchive); //Запуск Симулятора Метода DMOL
	HRESULT STDMETHODCALLTYPE  InitializeRaytracerRenderingEngine(); // Включение модуля трассировки лучей
	HRESULT STDMETHODCALLTYPE InitializeRenderToTextureRenderingEngine(); // Включение модуля рендеринга на текстуру
	HRESULT STDMETHODCALLTYPE InitializeVideoProcessingEngine(); //Включение модуля видео обработки
	HRESULT STDMETHODCALLTYPE InitializeRenderFarmSModule(); //Включение модуля Рендер-Ферма
	HRESULT STDMETHODCALLTYPE InitializeMolecularSimulationRenderingModule(); //Включение модуля Молекулярной динамики
	//******************************************End Reegio*********************************************************************************//
	HRESULT STDMETHODCALLTYPE InitializeConectionToDataEngine();
	HRESULT STDMETHODCALLTYPE InitializeConnectionTo1C(HINSTANCE hInstance, HWND hWnd);

	//**********************Облачные Вычисления***********************************//
	HRESULT STDMETHODCALLTYPE SimulateComputationalTask(LPCSTR address, LPCSTR usernme, LPCSTR password, LPCSTR cloudServeerName);
	HRESULT STDMETHODCALLTYPE EstblishAConnectionWithACloudServer(LPCSTR address, LPCSTR usernme, LPCSTR password);
	HRESULT STDMETHODCALLTYPE EstablishAConnectionToHTTPServer(LPCSTR address, LPCSTR usernme, LPCSTR password);
	HRESULT STDMETHODCALLTYPE DownloadDataPacketFromWeb(LPCSTR webAddress);
	HRESULT STDMETHODCALLTYPE ExtractDataPacketFromWeb(LPCSTR dataAddress);
	HRESULT STDMETHODCALLTYPE CreateCloudServerConnection(LPCWSTR webConnectionName, LPCWSTR webAddress, LPCWSTR portNumber);
	//*************************************End Region**********************************************************************//
	HRESULT STDMETHODCALLTYPE InitializeInternalTCPServer();
	HRESULT STDMETHODCALLTYPE CreateNewComputationalAgent(LPCSTR agentReader, LPCSTR agentWriter);
	HRESULT STDMETHODCALLTYPE OutputLocalIpAddress();
	HRESULT STDMETHODCALLTYPE ConnectToAgentManagementServer();
	HRESULT STDMETHODCALLTYPE InitializeInternelAccess();
	HRESULT STDMETHODCALLTYPE InitializeHTTPAccess(LPCSTR webAddress);
	HRESULT STDMETHODCALLTYPE InitializeHTTPAccessToKernel();
	HRESULT STDMETHODCALLTYPE StartProcessingPostRequests();
	HRESULT STDMETHODCALLTYPE InitializeComputationalTask(UINT taskId);
	HRESULT STDMETHODCALLTYPE InitializeLocalTransactionServer();
	HRESULT STDMETHODCALLTYPE ShutdownGPUDigitalLabCore();
	HRESULT STDMETHODCALLTYPE LogoffGPUDigitalLabUser(HWND hWnd);
	//***********************************Выгрузка Ресурсов Системы*********************************************************************************************//

	HRESULT ConnectToAgentNamedPipeServer(HINSTANCE hInstance, HWND hWnd);
	HRESULT ReverseBitsOfANumber(HINSTANCE hInstance, HWND hWnd);
	HRESULT ShutdownGPUDigitalLabCore(HINSTANCE hInstance, HWND hWnd); //Отключение ядра Системы (ссылка на основной модуль windiws, дескриптор окна)
	HRESULT ShutdownUserAccountController(HINSTANCE hInstance, HWND hWND);
	HRESULT STDMETHODCALLTYPE  ShutdownUserAccountController(); //Отключение контроллера сеансов пользователя (ссылка на основной модуль windiws, дескриптор окна)
	HRESULT STDMETHODCALLTYPE ShutdownUserProcessingSession(); //Отключение пользовательского сеанса
	HRESULT STDMETHODCALLTYPE ShutdownEventProcessingModule(); //Отключение обработчика событий
	HRESULT STDMETHODCALLTYPE ShutdownAppicationDomainManagerClass(); //Отключение менеджера доменов приложений
	HRESULT STDMETHODCALLTYPE ShutdownApplicationDomainInstanceClass(); //Отключение домена приложений
	HRESULT STDMETHODCALLTYPE ShutdownModuleProcessingWidgets(); //Отключение обработчика модулен
	HRESULT STDMETHODCALLTYPE ShutdownFluidDynamicsAnimation(); //Отключение модуля механики жидкости
	HRESULT STDMETHODCALLTYPE ShutdownIndustrialSimulationAnimation(); //Отключение симулятора производственных процессов
	HRESULT STDMETHODCALLTYPE ShutdownDMOLSimulationAnimation(); //Отключение модуля  метода DMOL
	HRESULT STDMETHODCALLTYPE ShutdownRaytracerRenderingEngine(); //Отключение модуля трассировки лучкй 
	HRESULT STDMETHODCALLTYPE ShutdownVideoProcessingEngine(); //Отключение модуля видео обработки
	HRESULT STDMETHODCALLTYPE ShutdownRenderFarmSModule(); //Отключение модуля Рендер Ферма
	HRESULT STDMETHODCALLTYPE ShutdownMolecularSimulationRenderingModule(); //Отключение модуля молекулярной динамики
	//*************************************End Region**********************************************************************//
	void ControlAgentServer(HINSTANCE hInstance, HWND hWnd);
	HRESULT SearchForBorderControlWithinImages(HINSTANCE hInstance, HWND hWnd);
	HRESULT ConvertAnArrayOfNumbersToButterflyFormOnCPU(HINSTANCE hInstance, HWND hWnd);
	HRESULT STDMETHODCALLTYPE ControlAgentServer();
	TCHAR szDefaultName;
	TCHAR szDefaultMsg;
	void RedirectIOToConsole();
	HRESULT OpenTheContentOfTheRosFileImage(HINSTANCE hInstance, HWND hWnd);
	HRESULT ComputeImageGradientFunctionForGrayscaleImageOnGPU(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateImageSubtractionGrayScaleOnGPU(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateOddEvenFSMWithManualInput(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateOddEvenFSM(HINSTANCE hInstance, HWND hWnd);
	HRESULT ConnectToLocalModuleService(HINSTANCE hInstance, HWND hWnd, LPCSTR pServiceName);
	HRESULT InitializeLocalTransactionServer(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateScientificAlgortihm(HINSTANCE hInstance, HWND hWnd, LPCSTR webAddress);
	HRESULT InitializePipeConnection(HINSTANCE hInstance, HWND hWnd);
	HRESULT InitializeConnectionApplicationServerProcessor(HINSTANCE hInstance, HWND hWnd);
	HRESULT InititializeTCPNetworkServer(HINSTANCE hInstance, HWND hWnd);
	HRESULT InitializeDeamonInstance(HINSTANCE hInstance, HWND hWnd, LPCSTR filename, LPCSTR errorFile);
	HRESULT CreateNewComputationalUnit(HINSTANCE hInstance, HWND hWnd);
	HRESULT InitializeHTTPAccessToKernel(HINSTANCE hInstance, HWND hWnd);
	HRESULT InitializeHTTPAccess(HINSTANCE hInstance, HWND hWnd, LPCSTR webAddress);
	HRESULT InitializeInternalTCPServer(HINSTANCE hInstance, HWND hWnd);
	HRESULT InitializeInternelAccess(HINSTANCE hInstance, HWND hWnd);
	HRESULT StartProcessingPostRequests(HINSTANCE hInstance, HWND hWnd);
	HRESULT CreateChildProcess(char* filename);
	HRESULT RegisterComServerObject(HINSTANCE hInstance, HWND hWnd);
	HRESULT CreateClassInstance(HINSTANCE hiNSTANCE, HWND hwND);
	HRESULT CreateCloudServerConnection(HINSTANCE hInstance, HWND hWnd, LPCWSTR webConnectionName, LPCWSTR webAddress, LPCWSTR portNumber);
	HRESULT SimulateAddition(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateComputationalTask(HINSTANCE hInstance, HWND hWnd, LPCSTR address, LPCSTR usernme, LPCSTR password, LPCSTR cloudServeerName);
	HRESULT InitializeComputationalTask(HINSTANCE hInstance, HWND hWnd, UINT taskId);
	HRESULT EstblishAConnectionWithACloudServer(HINSTANCE hInstance, HWND hWnd, LPCSTR address, LPCSTR usernme, LPCSTR password);
	HRESULT ExtractDataPacketFromWeb(HINSTANCE hInstance, HWND hWnd, LPCSTR dataAddress);
	HRESULT InitializeDataProcessingStream(HINSTANCE hInstance, HWND hWnd, LPCSTR address, LPCSTR usernme, LPCSTR password);
	HRESULT DownloadDataPacketFromWeb(HINSTANCE hInstance, HWND hWnd, LPCSTR webAddress);
	HRESULT StartDataProcessingThreads(HINSTANCE hInstance, HWND hWnd);
	HRESULT PerformLocalComRegistration(HINSTANCE hInstance, HWND hWnd, IUnknown* pUnkPtr, char* pMonikerName);
	HRESULT StartDataPostRequestLoop(HINSTANCE hInstance, HWND hWnd);
	HRESULT OutputLocalIpAddress(HINSTANCE hInstance, HWND hWnd);
	HRESULT StartDataGetRequest(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateGasDynamicsLittleAlgoritmOnCPU(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateGasDynamicsMajorAlgoritmOnCPU(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateMatrixMultiplicationAlgorithm(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateMatrixMultiplicationAlgorithmWithManualInput(HINSTANCE hInstance, HWND hWnd);
	HRESULT ApplyLaplacianOfGaussianFilterToSelectedImageFile(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateMatrixMultiplicationAlgorithmUsingAMP(HINSTANCE hInstance, HWND hWnd);
	HRESULT ComputeStandardDeviationForSelectedImageOnCPU(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateImageProcessingFiniteStateMachine(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateImageProcessingFiniteStateMachineStageTwo(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateImageProcessingFiniteStateMachineIntensityFunctionCrossTab(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateArrayMultiplicationAlgorithm(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateRandomizingAlgorithm(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateFastFourierTransformAlgorithm(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateFFTAlgorithmObject(HINSTANCE hInstance, HWND hWnd);
	HRESULT ConvertIntegerValueToBinary(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateSineFunctionCS(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateCosineFunctionCS(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateExponentialFunctionAlgorithm(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateQuadraticFunctionAlgorithm(HINSTANCE hInstance, HWND hWnd);
	void InstallProcessingService();
	void StartProcessingService();
	void StopProcessingService();
	void CLSIDToChar(const CLSID& clsid,
		WCHAR* szCLSID,
		int length);
	HRESULT LaunchInternalServer();
	HANDLE g_hChildStd_IN_Rd = NULL;
	HANDLE g_hChildStd_IN_Wr = NULL;
	HANDLE g_hChildStd_OUT_Rd = NULL;
	HANDLE g_hChildStd_OUT_Wr = NULL;
	void NotifyError(HWND hwnd, PCWSTR sErrorMessage, HRESULT hrErr);
	void OnPlayerEvent(HWND hwnd, WPARAM pUnkPtr);
	void UpdateUI(HWND hwnd, PlayerState state);
	DirectXVideoPlayer* md3dVideoManagementClass;

	DirectXStandardAlgorithmExecutor* GetAlgorithmExecutor() { return m_pStandardAlgorithmExecutor; }
	void InitializeGPUDigitalLabServiceObject(HINSTANCE hInstance, HWND hWnd);
	HRESULT PrintRobotCleanerReportObject(HINSTANCE hInstance, HWND hWnd, RobotCleanerReportObject pReportObject);
	ULONG g_ServerLocks;
	HRESULT OpenDataProcessingLoop(HINSTANCE hInstance, HWND hWnd);
	HRESULT ConnectToAgentManagementServer(HINSTANCE hInstance, HWND hWnd);
	HRESULT StartLocalServiceControlNetwork(HINSTANCE hInstance, HWND hWnd);
	HRESULT ComputeImageGradientFunctionForGrayscaleImageWithGaussianSmoothing(HINSTANCE hInstance, HWND hWnd);
	HRESULT ComputeImageGradientFunctionForGrayscaleImage(HINSTANCE hInstance, HWND hWnd);
	HRESULT InitializeConectionToDataEngine(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateGasDynamicsModelsFunction(HINSTANCE hInstance, HWND hWnd);
	HRESULT ComputeGasSpeedData(HINSTANCE hInstance, HWND hWnd);
	HRESULT CreateDirect3D11TextureFromAnImageFileW(
		HINSTANCE hInstance, HWND hWnd);
	HRESULT CreateDirect3D11TextureForFurtherProcessing(HINSTANCE hInstance,
		HWND hWnd,
		ID3D11Texture2D** ppTexture2DObject,
		ID3D11ShaderResourceView** ppShaderResourceObject);
	HRESULT SimulateImageClippingExperimentUsingClipper(HINSTANCE hInstance, HWND hWnd);
	HRESULT __stdcall CreateInstance(
		IUnknown *pUnkOuter,
		REFIID riid,
		void **ppvObject,
		HINSTANCE hInstance, HWND hWnd);
	HRESULT CheckIfTwoImagesAreIdentical(HINSTANCE hInstance, HWND hWnd);
	static STDMETHODIMP CreateInstance(HINSTANCE hInstance,
		HWND hVideo,
		HWND hEvent,
		DirectXGPUDigitalLabCoreModule **m_pD3DGPUDigitalLabCoreClass);
	STDMETHODIMP put_Visible(BOOL bVisible);
	STDMETHODIMP LockServer(BOOL fLock);
	
	STDMETHOD(MyComMessage) ();

	HRESULT InitializeGPUDigitalLabCore(HINSTANCE hInstance, HWND hWnd);
	HRESULT ExecuteComputeShader(HINSTANCE hInstance, HWND hWnd, LPCSTR filename);
	HRESULT OpenTheContentOfTheRosFileImageRGB(HINSTANCE hInstance, HWND hWnd);

	HRESULT SimulateMatrixMultiplicationOnCPU(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateStandardDeviationExperimentFromTextFile(HINSTANCE hInstance, HWND hWnd);
	
	HRESULT SimulateMatrixMultiplicationSecondOption(HINSTANCE hInstance, HWND hWnd);
	HRESULT ApplyMeanFilterForSelectedImageFile(HINSTANCE hInstance, HWND hWnd);

	HRESULT SimulateSortingAlgorithm(HINSTANCE hInstance, HWND hWnd);

	vector<HANDLE> pMailslotDataCollection;
	HRESULT InitializePipeServer(HINSTANCE hInstance, HWND hWnd);
	void InitializeSharedMemory(HINSTANCE hInstance, HWND hWnd, UINT amoountOfBytes);
	HRESULT InitializeMailSlot(HINSTANCE hInstance, HWND hWnd, HANDLE hSlot, LPTSTR lpszSlotName);
	BOOL IsUserInAdminGroup();
	BOOL IsRunAsAdmin();
	DWORD GetProcessIntegrityLevel();
	HRESULT StartChildProcess(HINSTANCE hInstance, HWND hWnd, LPCSTR jobName);
	HRESULT SimulateBitonicSortImplementation(HINSTANCE hInstance, HWND hWnd);
	HRESULT OpenNetworkAccessToKernel(HINSTANCE hInstance, HWND hWnd);
	HRESULT OpenIntertetDataStream(HINSTANCE hInstance, HWND hWnd, LPCWSTR fName);
	HRESULT CreateComputationalTaskObject(HINSTANCE hInstnce, HWND hWnd, UINT pTaskObject);
	HRESULT StartDataProcessingTimerThread(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateArrayAdditionAlgorithmUsingAppendConsumeTechnique(HINSTANCE hInstance, HWND hWnd);
	IClassFactory* pKernelObjectFactory;
	IDirectXDigitalLabSoftwareKernelInterface *pMainKernelInterface;
	HRESULT ConnectToPipeServer(HINSTANCE hInstancem, HWND hWnd, LPCWSTR fName);
	HANDLE   g_hSingleton;
	HANDLE   g_hBoundary;
	HANDLE   g_hNamespace;
	HANDLE pConnectionPipe;
	LPTYPEINFO m_ptinfoEnglish;
	LPTYPEINFO m_ptinfoGerman;
	HANDLE hInputFile;
	// Keep track whether or not the namespace was created or open for clean-up
	BOOL     g_bNamespaceOpened;
	HANDLE pDataVisualizationHandle;
	HANDLE pDataVisualizationThread;
	HRESULT SimulateArraySubtractionAlgorithm(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateArrayAdditionOfNRandomNumbers(HINSTANCE hInstance, HWND hWnd);
	HRESULT SimulateArrauSubtractionOfNRandomNumbers(HINSTANCE hInstance, HWND hWnd);

	HRESULT RegisterGPUDigitalLabServer(
		const CLSID& clsid,
		const WCHAR* szFriendlyName,
		const WCHAR* szSerIDProgID,
		const WCHAR* szProgramId);

	HRESULT UnRegisterGPUDigitalLabServer(const CLSID& clsid,
		const WCHAR* szVerIdProgId,
		const WCHAR* szProgId);

	GUID GPUDigitalLabAgentObj;

	void SetGUID(GUID OBJGuid)
	{
		GPUDigitalLabAgentObj = OBJGuid;
	}

	GUID GetGUID()
	{
		return GPUDigitalLabAgentObj;
	}

	HRESULT DrawSampleTextOnTheScreenD2D(HINSTANCE hInstance, HWND hWnd);
	HRESULT ComputeMatrixDeterminantSingleNumber(HINSTANCE hInstance, HWND hWnd);

	BOOL SetKeyAndValue(const WCHAR* szKey,
		const WCHAR* szSubKey,
		const WCHAR* szValue);
	LONG RecursivelyDeleteKey(HKEY hKeyParent, const WCHAR* szKeyChild);
	vector<LPTSTR> pipeList;
	vector<LPTSTR> pPipeInstanceObjects;

	BOOL IsProcessElevated();
	void MainDataLoop();

	RPC_IF_HANDLE hello_ServerIfHandle;
	// Names of boundary and private namespace
	int CreateModuleConnectionPipe(HWND hWnd, LPCWSTR fname);
	HRESULT InitalizeExternalProgram(HINSTANCE hInstance, HWND hWnd, LPCSTR fName);
	void WriteToPipe(void);
	void ReadFromPipe(void);
	void ErrorExit(PTSTR lpszFunction);

	HRESULT InitializeComputeShaderInstanceFromFile(HINSTANCE hInstance, HWND hWnd, LPCSTR filename);

	void DumpModule();
	void EleveteProcessStatus(HWND hWnd);
	HRESULT LogoffGPUDigitalLabUser(HINSTANCE hInstance, HWND hWnd);
	HRESULT InitializeNewJobObject(HINSTANCE hInstance, HWND hWnd, LPCSTR fileName);
	void RedirectIOToFile();
	HRESULT SimulateArrayAdditionAlgorithm(HINSTANCE hInstance, HWND hWnd);
	HRESULT InitializeSimulationSystem(HINSTANCE hInstance, HWND hWnd); // Инициализация Системы GPUDigitalLab (ссылка на основной модуль windiws, дескриптор окна)
	HRESULT InitializeApplicationDomainManager(HINSTANCE hInstance, HWND hWnd);
	void SetupSimulationSystemNetwork(HINSTANCE hInstance, HWND hWnd);
	HRESULT CreateMultiagentSimulation(HINSTANCE hInstance, HWND hWnd); //Симулятор мультиагентной операции
	HRESULT InitializeUserAccountController(HINSTANCE hInstance, HWND hWnd); // Инициализация контроллера пользовательских сеансов
	HRESULT LogonGPUDigitalLabUser(HWND hWnd, LPCSTR fUserName, LPCSTR fPassword); //Вход пользователя в систему
	HRESULT CreateUserProcessingSession(HINSTANCE hInstance, HWND hWnd); //Запуск обработчика действий пользователя
	HRESULT InitializeEventProcessingModule(HINSTANCE hInstance, HWND hWnd); //Запуск обработчика событий системы
	HRESULT	CreateAppicationDomainManagerClass(HINSTANCE hInstance, HWND hWnd); //Запуск менеджера доменов приложений
	HRESULT CreateApplicationDomainInstanceClass(HINSTANCE hInstance, HWND hWnd); //Запуск обработчика сеансов пользователя
	HRESULT InitalizeModuleProcessingWidgets(HINSTANCE hInstance, HWND hWnd); //Запуск контроллера программных модулей
	HRESULT DownloadDataFromInternetServiceObject(HINSTANCE hInstance, HWND hWnd, LPCSTR webAddress);
	HRESULT CreateFluidDynamicsAnimation(HINSTANCE hInstance, HWND hWnd, LPCSTR fName); //Запуск модуля механики жидкостей
	HRESULT CreateIndustrialSimulationAnimation(HINSTANCE hInstance, HWND hWnd, LPCSTR fProgrammingScript, LPCSTR fDataArchive); //Запуск Симулятора производственных процессов
	HRESULT CreateDMOLSimulationAnimation(HINSTANCE hInstance, HWND hWnd, LPCSTR fProgrammingScript, LPCSTR fDataArchive); //Запуск Симулятора Метода DMOL
	HRESULT InitializeRaytracerRenderingEngine(HINSTANCE hInstance, HWND hWnd); // Включение модуля трассировки лучей
	HRESULT InitializeRenderToTextureRenderingEngine(HINSTANCE hInstance, HWND hWnd); // Включение модуля рендеринга на текстуру
	HRESULT InitializeVideoProcessingEngine(HINSTANCE hInstance, HWND hWnd); //Включение модуля видео обработки
	HRESULT InitializeRenderFarmSModule(HINSTANCE hInstance, HWND hWnd); //Включение модуля Рендер-Ферма
	HRESULT InitializeMolecularSimulationRenderingModule(HINSTANCE hInstance, HWND hWmd); //Включение модуля Молекулярной динамики
	HRESULT InitializeRenderingPipeLine(HINSTANCE hInstance, HWND hWnd);
	HRESULT InitializeModuleProcessingWidgets(HINSTANCE hInstance, HWND hWnd);
	HRESULT InitializeServerSocket(HINSTANCE hInstance, HWND hWND);
	HRESULT InitializeOverlappedNamedPipeServer(HINSTANCE hInstance, HWND hWnd);
	void OpenNewLogFile(LPCSTR fName);
	HRESULT ConvertRGBImageToBinary(HINSTANCE hInstance, HWND hWnd);
	HRESULT ThreshHoldImagePixelByCriteria(HINSTANCE hInstance, HWND hWnd);
	HRESULT ThreshHoldImagePixelByCriteriaRGB(HINSTANCE hInstance, HWND hWnd);
	HRESULT ThreshHoldImagePixelByCoordinatesCriteria(HINSTANCE hInstance, HWND hWnd);
	HRESULT ThreshHoldImagePixelByCoordinatesCriteriaRGB(HINSTANCE hInstance, HWND hWnd);
	HRESULT ConvertRGBImageToGrayScale(HINSTANCE hInstance, HWND hWnd);
	HRESULT ConvertRGBImageToGrayScale(HINSTANCE hInstance, HWND hWnd, vector<PixelObjectGrayScale> *ppGrayScale);
	HRESULT ConvertRGBImageToLinkedList(HINSTANCE hInstance, HWND hWnd);

	void Garbage(HINSTANCE hInstance, HWND hWnd);
	HRESULT ShutdownUserProcessingSession(HINSTANCE hInstance, HWND hWnd); //Отключение пользовательского сеанса
	HRESULT ShutdownEventProcessingModule(HINSTANCE hInstance, HWND hWnd); //Отключение обработчика событий
	HRESULT	ShutdownAppicationDomainManagerClass(HINSTANCE hInstance, HWND hWnd); //Отключение менеджера доменов приложений
	HRESULT ShutdownApplicationDomainInstanceClass(HINSTANCE hInstance, HWND hWnd); //Отключение домена приложений
	HRESULT ShutdownModuleProcessingWidgets(HINSTANCE hInstance, HWND hWnd); //Отключение обработчика модулен
	HRESULT ShutdownFluidDynamicsAnimation(HINSTANCE hInstance, HWND hWnd); //Отключение модуля механики жидкости
	HRESULT ShutdownIndustrialSimulationAnimation(HINSTANCE hInstance, HWND hWnd); //Отключение симулятора производственных процессов
	HRESULT ShutdownDMOLSimulationAnimation(HINSTANCE hInstance, HWND hWnd); //Отключение модуля  метода DMOL
	HRESULT ShutdownRaytracerRenderingEngine(HINSTANCE hInstance, HWND hWnd); //Отключение модуля трассировки лучкй 
	HRESULT ShutdownVideoProcessingEngine(HINSTANCE hInstance, HWND hWnd); //Отключение модуля видео обработки
	HRESULT ShutdownRenderFarmSModule(HINSTANCE hInstance, HWND hWnd); //Отключение модуля Рендер Ферма
	HRESULT ShutdownMolecularSimulationRenderingModule(HINSTANCE hInstance, HWND hWmd); //Отключение модуля молекулярной динамики

	HRESULT InitializeComputationalAgent(HINSTANCE hInstance, HWND hWnd, LPCSTR fName);
	HRESULT InitializeComputationalController(HINSTANCE hInstance, HWND hWnd, LPCSTR fName);
	HRESULT ResolveComputationalTask(HINSTANCE hInstance, HWND hWnd, LPCSTR fName);
	HRESULT CreateCoreSimulationModude(HINSTANCE hInstance, HWND hWnd, LPCSTR fName);

	void WriteDataToPipe();
	bool CreateChildProcess(LPCSTR fName, HANDLE pInputHandle, HANDLE pOutputHandle);
	void ProcessWindowResize(HWND hWnd);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	STDMETHODIMP QueryInterface(REFIID riid, void** ppv);
	bool LoadContent() { return true; }
	void UnloadContent() {}
	void UpdateScene() {}
	void UpdateScene(float dt) {}
	void DrawScene() {}
	void OnResize() {}
	void Update() {}
	void Render() {}
	HRESULT InitializeRPCServer(HINSTANCE hInstance,
		HWND hWnd);

	HRESULT CreateNewSystemInstance(HINSTANCE hInstance, HWND hWnd, LPCWSTR g_szBoundary, LPCWSTR szNamespace);
	HRESULT ExecuteExternalProgramWidget(HINSTANCE hInstance, HWND hWnd, LPCSTR fName);
	BOOL GetProcessElevation(HINSTANCE hInstance, HWND hWnd, HANDLE pProcessHandle, TOKEN_ELEVATION_TYPE* pElevationType, BOOL* pIsAdmin);


	IDXGIFactory* pStandardAlgorithmFactory;
	HANDLE hNamespace;
	HANDLE pFluidDynamicsHandle;
	HANDLE pAdaptiveTesselationClass;
	HANDLE pNumericalProcessorHandle;
	HANDLE pRenderFarmHandle;
	HANDLE pDMOLSimulatorClass;
	HANDLE pImagine3DRenderingSoftware;
	HANDLE pIndustrialSimulationEngineSoftware;
	HANDLE pMedicisineSimulationSoftware;
	HANDLE pVideoEditiingSoftware;
	vector<HANDLE> pProgramDataModulesCollection;
	vector<HANDLE> m_pConnectedPipeCollection;
	IDirectXDigitalLabSoftwareKernelInterface* pKernelInterface;
	HANDLE hMainServerPipe;
	HANDLE hMainServerThread;
	HWND mainWindow;
	HANDLE hSingleton;
	HANDLE hBoundary;

	DWORD dwLastError;
	LPCWSTR szNamespace;
	LPCWSTR szBoundary;
	//Utility Functions;
	HRESULT CheckAvailableInstance();
	void MapFiles();
	BOOL IsUserAnAdmin();
	void AddText(PCTSTR pszFormat, DWORD lastError);
	HRESULT StartRestricedProcess(HINSTANCE hInstance, HWND hWnd, LPCSTR fName);
	HRESULT LoadApplicationDomainByName(HINSTANCE hInstance, HWND hWnd, LPCSTR appDomain);
	HRESULT LoadD3DGraphicsRenderingProject(HINSTANCE hInstance, HWND hWnd, LPCSTR graphicsProject);
	HRESULT LoadMathematicalModelFromFile(HINSTANCE hInstance, HWND hWnd, LPCSTR mathematicalModel);
	HRESULT ConnectToDataProcessingEngine(HINSTANCE hInstance, HWND hWnd);
	HRESULT CreateEmptyRenderingExperiment(HINSTANCE hInstance, HWND hWnd);
	HRESULT ConvertRGBImageToHSI(HINSTANCE hInstance, HWND hWnd);
	HRESULT ConvertAnArrayOfNRandomNumbersToBitReverse(HINSTANCE hInstance, HWND hWnd);
	HRESULT ApplyStandardDeviationFunctionToGrayscaleImage(HINSTANCE hInstance, HWND hWnd);
	HRESULT ApplyGaussianFilterToSelectedImageFile(HINSTANCE hInstance, HWND hWnd);
	HRESULT ApplyGaussianFilterToSelectedImageFileOneDimension(HINSTANCE hInstance, HWND hWnd);
	HRESULT CreateAndRednerHistogramForAnImage(HINSTANCE hInstance, HWND hWnd);
	HRESULT ApplyCannyEdgeDetectionFilterToSelectedImageFile(HINSTANCE hInstance, HWND hWnd);
	HRESULT AutomateOutlookUsingCOMAPI(HINSTANCE hInstance, HWND hWnd);
	HRESULT SendTestEmailObject(HINSTANCE hInstance, HWND hWnd);
	void RunMessageLoop();
    


};

static DirectXGPUDigitalLabCoreModule*  pd3ddmoldimulator = nullptr;
