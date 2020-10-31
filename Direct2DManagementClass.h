
#pragma once

#ifndef WINVER
#define WINVER 0x0700
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0700
#endif

#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN
#include <iomanip>
#include <windowsx.h>
#include <ShellScalingApi.h>
#include "BasicPlayback.h"
#include <stdlib.h>
#include <wchar.h>
#include <math.h>
#include <fstream>

#include <commdlg.h>


#include "PixelObject.h"
#include "Dx11DemoBase.h"
#include <dwrite.h>
#include <wincodec.h>
#include <wincodecsdk.h>
#include "ShCore.h"
#include <dxgi1_2.h>
#include <d3d11.h>
#include "D3D11.h"
#include "d3dcompiler.h"
#include <Wincodecsdk.h>
#include "D3D11Shader.h"
#include <list>
#include <d2d1_1.h>
#include <d2d1_2.h>
#include <d2d1_1helper.h>
#include  <dwrite_2.h>
#include <d2dbasetypes.h>
#include "BasicPlayback.h"
#include <vector>
#include "ImageFileData.h"
#include "BasicDataTypes.h"
#include "ImageSectorObject.h"
#include "PixelObject.h"
#include "PixelObjectInt.h"
#include "PixelObjectHSI.h"
#include "gdiplus.h"
#include "shellapi.h"


#pragma comment(lib, "Gdiplus.lib")
#pragma comment (lib, "windowscodecs.lib")

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

const UINT MAX_D2D_RENDER_TARGETS = 14;

using namespace std;
using namespace Gdiplus;


struct FormatInfo
{
	UINT32 imageWidthPils;
	UINT32 imageHeightPels;
	BOOL bTopDown;
	RECT rcPicture;

	FormatInfo() : imageWidthPils(0), imageHeightPels(0), bTopDown(0)
	{
		SetRectEmpty(&rcPicture);
	}
};


struct PixelData
{
	int xPosition;
	int yPosition;
	int color;
};



struct OutputStructureForGraph2D //Output Structure
{
	int pItemIndex;
	float pInputStructureForGraph;
	float pOutputStructureForGraph;
};


struct RectangleStructure
{
	int pRectangleNumber;
	D2D1_RECT_F pRectangleObject;
};


struct OutputPixelObjectWithAngleValuesD2D
{
	int pixelNumber;
	float xPosition;
	float yPosition;
	float rValue;
	float gValue;
	float bValue;
	float pXCentreDifference;
	float pYCentreDifference;
	float pCentreDifferenceRatio;
	float pAngleOfPixel;
	float pAngleOfPixelDegrees;
};

struct Point2DVar
{
	int pPointNumber;
	float xPosition;
	float yPosition;
};


Point2D GetPointByXAndYPosition(vector<Point2D> pPoint2DArray, float pXPosition, float pYPosition)
{
	Point2D pPoint2DVar;
	pPoint2DVar.pPointNumber = 0;
	pPoint2DVar.xPosition = 0.0f;
	pPoint2DVar.yPosition = 0.0f;

	for (Point2D var : pPoint2DArray)
	{
		if ((var.xPosition = pXPosition) && (var.yPosition == pYPosition))
		{
			pPoint2DVar = var;
		}
	}

	return pPoint2DVar;

}


Point2D GetPointBySector(vector<Point2D> pPoint2DArray, int pSectorNumber)
{
	Point2D pPoint2DVar;
	pPoint2DVar.pPointNumber = 0;
	pPoint2DVar.xPosition = 0.0f;
	pPoint2DVar.yPosition = 0.0f;

	for (Point2D var : pPoint2DArray)
	{
		if (var.pSectorNumber = pSectorNumber)
		{
			pPoint2DVar = var;
		}
	}

	return pPoint2DVar;

}


using namespace Gdiplus;


float ComputeLineLengthBetweenPointsCoordinated(float pXPosition0, float pYPosition0,
	float pXPosition1, float pYPosition1)
{
	float pOutputVar = 0.0f;
	pOutputVar = sqrtf(pow((pXPosition1 - pXPosition0), 2.0f) + pow((pYPosition1 - pYPosition0), 2.0F));
	return pOutputVar;
}




float ComputeLineLengthBetweenPoint(Point2D pPointOne, Point2D pPointTwo)
{
	float pVectorLength = 0.0f;
	pVectorLength = ComputeLineLengthBetweenPointsCoordinated(pPointOne.xPosition, pPointOne.yPosition, pPointTwo.xPosition, pPointTwo.yPosition);
	return pVectorLength;
}

float ComputeTriangleAreaUsingHeronsFormula(Point2D pPointA, Point2D pPointB, Point2D pPointC)
{
	float pABVector = 0.0f;
	float pBCVector = 0.0f;
	float pACVector = 0.0f;
	float pSemiPerimeterValue = 0.0f;
	float pProdunctValue = 0.0f;
	float pAreaValue = 0.0f;

	pABVector = ComputeLineLengthBetweenPointsCoordinated(pPointA.xPosition, pPointA.yPosition, pPointB.xPosition, pPointB.yPosition);
	pBCVector = ComputeLineLengthBetweenPointsCoordinated(pPointB.xPosition, pPointB.yPosition, pPointC.xPosition, pPointC.yPosition);
	pACVector = ComputeLineLengthBetweenPointsCoordinated(pPointA.xPosition, pPointA.yPosition, pPointC.xPosition, pPointC.yPosition);

	pSemiPerimeterValue = (pABVector + pBCVector + pACVector) / 2.0f;
	pProdunctValue = pSemiPerimeterValue * (pSemiPerimeterValue - pABVector) * (pSemiPerimeterValue - pACVector) * (pSemiPerimeterValue - pBCVector);
	pAreaValue = sqrtf(pProdunctValue);

	return pAreaValue;
}



float ComputeTriangleAreaUsingHeronsFormulaLines(Line2D pPointACentre, Line2D pPointBCentre, Line2D pPointAB)
{
	float pABVector = 0.0f;
	float pBCVector = 0.0f;
	float pACVector = 0.0f;
	float pSemiPerimeterValue = 0.0f;
	float pProdunctValue = 0.0f;
	float pAreaValue = 0.0f;


	pSemiPerimeterValue = (pPointACentre.pLineWidth + pPointBCentre.pLineWidth + pPointAB.pLineWidth) / 2.0f;
	pProdunctValue = pSemiPerimeterValue * (pSemiPerimeterValue - pPointACentre.pLineWidth) * (pSemiPerimeterValue - pPointBCentre.pLineWidth) * (pSemiPerimeterValue - pPointAB.pLineWidth);
	pAreaValue = sqrtf(pProdunctValue);



	return pAreaValue;
}


struct TriangleObject
{
	int pTriangleId;
	Point2D pPointA;
	Point2D pPointB;
	Point2D pPointC;
	Line2D pABVector;
	Line2D pBCVector;
	Line2D pACVector;
	float pTriangleArea;
	float pPerimeterValue;

public:
	TriangleObject(int triangleId,
		Point2D aPoint,
		Point2D bPoint,
		Point2D cPoint,
		Line2D abVector,
		Line2D bcVector,
		Line2D acVector,
		float pAreaValue)
	{
		pTriangleId = triangleId;
		pPointA = aPoint;
		pPointB = bPoint;
		pPointC = cPoint;
		pABVector = abVector;
		pBCVector = bcVector;
		pACVector = acVector;
		pTriangleArea = pAreaValue;
		pPerimeterValue = pABVector.pLineWidth + pACVector.pLineWidth + pBCVector.pLineWidth;
	}
};


struct ClippingStage
{
	int pStageNumber;
	string pStageName;
	tm* pStartOfOperation;
	tm* pEndOfOperation;

public:
	ClippingStage(int pStageIdVar,
		string pStageNameVar,
		tm* pStartTime,
		tm* pEndTime)
	{
		pStageNumber = pStageIdVar;
		pStageName = pStageNameVar;
		pStartOfOperation = pStartTime;
		pEndOfOperation = pEndTime;
	}
};

BOOL LOOPS_ARE_TURNED_OFF = TRUE;

class PixelObjectStack
{
	int pStackNumber;
	int pMaxStackSize;
	float pMinXValue0;
	float pMinYValue0;

	vector<PixelObject> pPixelObjectStackArray;

public:
	PixelObjectStack(int pStackNumberVar, int pMaxStackSizeVar,
		float pMinXVar, float pMinYVar)
	{
		pStackNumber = pStackNumberVar;
		pMinXValue0 = pMinXVar;
		pMinYValue0 = pMinYVar;
		pMaxStackSize = pMaxStackSize;
		pPixelObjectStackArray = vector<PixelObject>();
	}

	PixelObjectStack(int pStackNumberVar, int pMaxStackSizeVar)
	{
		pStackNumber = pStackNumberVar;
		pMaxStackSize = pMaxStackSize;
		pPixelObjectStackArray = vector<PixelObject>();
	}

	void AddPixelObjectToStack(PixelObject pNewPixelObject)
	{
		if (pPixelObjectStackArray.size() <= pMaxStackSize)
		{
			pPixelObjectStackArray.push_back(pNewPixelObject);
		}
	}

	vector<PixelObject> GetPixelObjectByXPosition(float pXPosition)
	{
		vector<PixelObject> pSelectionArray = vector<PixelObject>();
		for (PixelObject pVar : pPixelObjectStackArray)
		{
			if (pVar.xPosition == pXPosition)
			{
				pSelectionArray.push_back(pVar);
			}
		}
		return pSelectionArray;
	}

	vector<PixelObject> GetPixelObjectByYPosition(float pXPosition)
	{
		vector<PixelObject> pSelectionArray = vector<PixelObject>();
		for (PixelObject pVar : pPixelObjectStackArray)
		{
			if (pVar.xPosition == pXPosition)
			{
				pSelectionArray.push_back(pVar);
			}
		}
		return pSelectionArray;
	}

	vector<PixelObject> GetNItemsFromPixelVectorToProcess(int pAmountOfItems)
	{
		vector<PixelObject> pSelectionArray = vector<PixelObject>();
		for (int pIndex = 0; pIndex < pAmountOfItems; ++pIndex)
		{
			PixelObject pSelectedObject = pPixelObjectStackArray[pIndex];
			pSelectionArray.push_back(pSelectedObject);
		}

		return pSelectionArray;
	}

};



class Direct2DManagementClass : public Dx11DemoBase
{


public:
	HRESULT DrawSampleTextOnTheScreenDirect2D(HINSTANCE hInstance, HWND hWnd, WCHAR* pMessage,
		UINT pFontSize);
	HRESULT DrawSomeThingUsingGDIInterop(HINSTANCE hInstance, HWND hWnd);
	HRESULT DrawRoadSignMotionDetectedVertical(HINSTANCE hInstance, HWND hWnd,
		float pAmountOfDifferentPixels,
		float pLowerGateValueBottom,
		float pMiddleGateValueBottom,
		float pTopGateValueBottom);
	HRESULT SavePixelArrayChunckToStack(vector<PixelObject> pPixelObjectToPorcess, int pStackSize, int pFirstPixelNumber,
		PixelObjectStack* pStackVar);
	LPWSTR LocateImageFile(HWND hWnd, LPWSTR pszFileName, DWORD cbFileName);
	HRESULT SavePixelArrayChunckToStack(vector<PixelObject> pPixelObjectToPorcess, int pStackSize, int pFirstPixelNumber);
	HRESULT CreateBitmapDrawingClipObjectDIB(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pWidth, float pHeight,
		LPCWSTR szFileName,
		vector<PixelObject> *ppPixelObject);
	HRESULT CreateBitmapDrawingClipObjectDIB(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pWidth, float pHeight,
		vector<PixelObject> *ppPixelObject);
	HRESULT CreateBitmapDrawingClipObjectWithStageRecording(HINSTANCE hInstance, HWND hWnd, LPCSTR pFileName,
		float pXPosition, float pYPosition, float pWidth, float pHeight,
		vector<PixelObject> *ppPixelObject,
		vector<ClippingStage> *pClippingStageArray);

	HRESULT CreateBitmapDrawingClipObjectWithStageRecording(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pWidth, float pHeight,
		vector<PixelObject> *ppPixelObject,
		vector<ClippingStage> *pClippingStageArray);

	HRESULT DrawSampleTextOnTheScreenMultiple(HINSTANCE hInstance, HWND hWnd, WCHAR* pMessage0,
		WCHAR* pMessage1,
		UINT pFontSize);
	HRESULT DrawMessageArray(HINSTANCE hInstance, HWND hWnd, vector<WCHAR*> pMessageArray,
		UINT pFontSize);
	HRESULT GetDesktopDPI(HINSTANCE hInstance, HWND hWnd, float* pDPIX, float* pDPIY);

	HRESULT DrawSampleTextOnTheScreenMultiple(HINSTANCE hInstance, HWND hWnd, 
		vector<WCHAR*> pMessages,
		UINT pFontSize);
	HRESULT CreateBitmapDrawingClipObjectWitSpecifiedStepForASpecifiedFile(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pWidth, float pHeight,
		float pStepValueX,
		float pStepValueY,
		LPWSTR pFileNameToProcess,
		vector<PixelObject> *ppPixelObject);
	HRESULT CreateBitmapDrawingClipObjectWithoutD2DForFileName(HINSTANCE hInstance, HWND hWnd, LPCWSTR pFileName, float pXPosition, float pYPosition, float pWidth, float pHeight,
		vector<PixelObject> *ppPixelObject);
	HRESULT CreateBitmapDrawingClipObjectWithLimitOfPixels(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pWidth, float pHeight,
		float pPixelLimitValue,
		vector<PixelObject> *ppPixelObject);

	
	HRESULT DrawRoadSignImageComparisonPercents(HINSTANCE hInstance, HWND hWnd, UINT pAmountOfIdenticalPixel, UINT pAmountOfDifferentPixels);
	HRESULT ClearRoadSignImageComparisonPercents(HINSTANCE hInstance, HWND hWnd);
	HRESULT DrawRoadSignIntensityDifferenceValue(HINSTANCE hInstance, HWND hWnd, float pDifferenceValue);
	HRESULT ClearRoadSignIntensityDifference(HINSTANCE hInstance, HWND hWnd);
	HRESULT DrawRoadSignIntensityDifference(HINSTANCE hInstance, HWND hWnd, float pIntensityDifference);
	HRESULT ConnectPointsUsingLinesSectorsComplex(HINSTANCE hInstance, HWND hWnd,
		float pCentreX,
		float pCentreY,
		vector<Point2D> pPointArrayVector,
		vector<Line2D> *ppOutputLineArray,
		float* ppTotalArea);
	HRESULT DrawSampleTextOnTheScreen(HINSTANCE hInstance, HWND hWnd, WCHAR* pMessage,
		UINT pFontSize);
	HRESULT ConnectPointsUsingLinesSectors(HINSTANCE hInstance, HWND hWnd,
		float pCentreX,
		float pCentreY,
		vector<Point2D> pPointArrayVector,
		vector<Line2D> *ppOutputLineArray,
		float* ppTotalArea);
	HRESULT DrawSampleTextOnTheScreenGDI(HINSTANCE hInstance, HWND hWnd, LPCWSTR pMessage);
	HRESULT GenerateSyntheticImageOfDefinedColor(HINSTANCE hInstance,
		HWND hWnd,
		UINT pTopLeftX,
		UINT pTopLeftY,
		UINT pImageWidth,
		UINT pImageHeight,
		float pRedValue,
		float pGreenValue,
		float pBlueValue,
		float pStepX,
		float pSteoY,
		vector<PixelObject> *ppPixelObject);
	HRESULT FillScreenWithColor(HINSTANCE hInstancr, HWND hWnd);
	HRESULT ConnectPointsUsingLinesDirectForSquare(HINSTANCE hInstance, HWND hWnd,
		vector<Point2D> pPointArrayVector);
	HRESULT ClearRoadSignMotionDetectedVerticalState(HINSTANCE hInstance, HWND hWnd);
	IWICImagingFactory *m_pWICFactory;
	HRESULT CreateD2DBitmapFromFileSpecified(HINSTANCE hInstance, HWND hWnd, LPCWSTR pImageFileName,
		UINT pImageWidth, UINT pImageHeight,
		ID2D1Bitmap** ppBitmapToDrawD2D);
	HRESULT SimulateBehaviourOfPathGeometryTools(HINSTANCE hInstance, HWND hWnd);
	HRESULT InitializePathGeometryResources(HINSTANCE hInstance,
		HWND hWnd,
		ID2D1SolidColorBrush** ppBackGroundBrush,
		ID2D1SolidColorBrush** ppSolidColorBrush,
		ID2D1PathGeometry** ppLeftMountainGeometryObject,
		ID2D1PathGeometry** ppRightMountainGeometryObject,
		ID2D1PathGeometry** ppSunGeometryObject,
		ID2D1PathGeometry** ppRiverGeometryObject);
	HRESULT Direct2DManagementClass::RenderPathGeometryOnThePlane(HINSTANCE hInstance,
		HWND hWnd,
		ID2D1SolidColorBrush* pSolidColorBrushBackGround,
		ID2D1SolidColorBrush* pSolidColorBrush,
		ID2D1PathGeometry* pLeftMountainGeometryObject,
		ID2D1PathGeometry* pRightMountainGeometryObject,
		ID2D1PathGeometry* pSunGeometryObject,
		ID2D1PathGeometry* pRiverGeometryObject);
	HRESULT DrawPathGeometryForGivenRGBPixels(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObject> ppixelDataArray);

	HRESULT CreateBitmapDrawingClipObjectWitSpecifiedStep(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pWidth, float pHeight,
		float pStepValueX,
		float pStepValueY,
		vector<PixelObject> *ppPixelObject);

	HRESULT CreateBitmapDrawingClipObjectWitSpecifiedStepForXAxis(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pWidth, float pHeight,
		float pStepValueX,
		vector<PixelObject> *ppPixelObject);

	HRESULT CreateBitmapDrawingClipObjectWitSpecifiedStepForYAxis(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pWidth, float pHeight,
		float pStepValueX,
		vector<PixelObject> *ppPixelObject);




	HRESULT DrawPixelArray(HINSTANCE hInstance, HWND hWnd, vector<PixelObject> pPixelObjectArray);
	HRESULT CaptureImageToFile(HINSTANCE hInstance, HWND hWnd, HBITMAP pBitmapObjectGDI, LPCWSTR filename);
	D2D1_POINT_2F ConvertPointToD2DPoint(Point2D pPoint2DObject);
	HRESULT ConnectPointsUsingLines(HINSTANCE hInstance, HWND hWnd,
		float pCentreX,
		float pCentreY,
		vector<Point2D> pPointArrayVector);

	HRESULT ConnectPointsUsingLinesD2D(HINSTANCE hInstance, HWND hWnd,
		vector<Point2D> pPointArrayVectorOne,
		vector<Line2D> *pLineSegmentsVector);
	HRESULT ConnectPointsUsingLines(HINSTANCE hInstance, HWND hWnd,
		vector<Point2D> pPointArrayVectorOne,
		vector<Point2D> pPointArrayVectorTwo,
		vector<Point2D> pPointArrayVectorThree,
		vector<Point2D> pPointArrayVectorFour);
	
	HRESULT DrawPixelBuffer(HINSTANCE hInstance, HWND hWnd, vector<PixelObject> pPixelObjectArray,
		UINT pXPosition, UINT pYPosition, UINT pImageWidth, UINT pImageHeight);
	HRESULT SaveHBITMAPToFile(HBITMAP hBitmap, LPCTSTR lpszFileName);
	HRESULT SaveBitmapToStream(
		HINSTANCE hInstance,
		HWND hWnd,
		LPCWSTR pFileName,
		UINT pImageWidth,
		UINT pImageHeight,
		IWICBitmapSource* pImageToSave);
	HRESULT CreateImageEncoder(HINSTANCE hInstance, HWND hWnd, LPCWSTR outputFilePath, IWICBitmapEncoder** ppOutputEncoder);
	bool HDCToFile(const char* FilePath, HDC Context, RECT Area, uint16_t BitsPerPixel = 24);
	PBITMAPINFO CreateBitmapInfoStructure(HINSTANCE hInstance, HWND hWnd, HBITMAP hBmp);
	void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi,
		HBITMAP hBMP, HDC hDC);
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	HRESULT CaptureScreenRectToFile(HINSTANCE hInstance, HWND hWnd, int pXPosition, int yPosition, int pWidth, int pHeight, LPCWSTR filename);
	ID3D11Device* pD3DGraphicsDevice;
	ID3D11DeviceContext* pD3DGraphicsContext;
	HRESULT ClearRoadOddEvenFSM(HINSTANCE hInstance, HWND hWnd);
	HRESULT ClearRoadSignImageComparison(HINSTANCE hInstance, HWND hWnd);
	HRESULT CreateAndDrawBitmapFromPixelVector(HINSTANCE hInstance, HWND hWnd,
    int pImageWidth, int pImageHeight, vector<PixelObject> pCorePixelArray);
	HRESULT DrawD2DContentWithPixels(HINSTANCE hInstance, HWND hWnd, vector<PixelObject> pPixelObjectArray, ID2D1Bitmap* pBrushToFill);
	HRESULT CreateD2DBitmapFromFileSpecified(HINSTANCE hInstance, HWND hWnd, LPCWSTR pImageFileName,
		UINT pImageWidth, UINT pImageHeight,
		vector<PixelObject> *ppPixelObjectArray);
	HRESULT CaptureScreenShotToFile(HINSTANCE hInstance, HWND hWnd, LPCWSTR filename, char** pScreenShotCode);
	HRESULT ClearRoadSignFSM(HINSTANCE hInstance, HWND hWnd);
	HRESULT DrawRoadSignImageProcessingFSM(HINSTANCE hInstance, HWND hWnd, UINT pLevelOfIntensity);
	HRESULT DrawRoadSignOddEven(HINSTANCE hInstance, HWND hWnd, UINT pInputNumber, UINT pLevelOfIntensity);
	ID2D1Factory1 *m_pD2DFactory;
	ID2D1HwndRenderTarget *pRenderTarget;
	ID2D1Bitmap *m_pD2DBitmap;
	ID2D1Bitmap1* pBitmapRenderTarget;
	IWICFormatConverter *m_pConvertedSourceBitmap;
	IWICBitmapSource *m_pWICBitmap;
	ID2D1PathGeometry *m_pPathGeometry;
	ID2D1PathGeometry *m_pObjectGeometry;
	ID2D1DeviceContext* m_pD2DDeviceContext;
	ID2D1DeviceContext1* m_pD2DDeviceContext1;
	ID3D11RenderTargetView* backBufferTargetObject;
	IDXGISwapChain1* pSwapChainObjectD2D;
	IDXGIFactory2* m_pDXGIFactory2;
	ID2D1SolidColorBrush* pBlackColorBrush;
	IDXGISurface* pDXGISurfaceObject;
	ID2D1Device* m_pD2DDevice;
	ID2D1DCRenderTarget* pD2DDXGITarget;
	ID3D11Texture2D* m_pSurfaceNV12;
	ID3D11Texture2D* m_pSurfaceNV12_cpu_copy;
	IWICBitmap* pBitmapObject;
	IWICBitmapLock* pBitmapLock;
	IWICBitmapDecoder *pImageDecoder;
	IDWriteFactory2* pFactoryObjectDWrite;
	IDWriteFontCollectionLoader* pFontCollectionLoader;
	HRESULT DrawRenctangleSet(HINSTANCE hInstance, HWND hWnd, vector<D2D1_RECT_F> *ppHouseArrayOne, vector<D2D1_RECT_F> *ppHouseArrayTwo);
	HRESULT ClearRenctangleSet(HINSTANCE hInstance, HWND hWnd,
		vector<D2D1_RECT_F> *ppHouseArrayOne, vector<D2D1_RECT_F> *ppHouseArrayTwo);
	HRESULT CreateBitmapFromMemory(HINSTANCE hInstance, HWND hWnd,
		UINT pImageWidth,
		UINT pImageHeight,
		BYTE* pImageDataFile,
		LPCWSTR pImageToWriteFile,
		vector<PixelObject> *ppPixelObjectOutput);
	HRESULT ClearRoadSignVertical(HINSTANCE hInstance, HWND hWnd);
	HRESULT DrawRoadSignImageComparison(HINSTANCE hInstance, HWND hWnd, UINT pAmountOfIdenticalPixel, UINT pAmountOfDifferentPixels);
	HRESULT ClearRoadSignMotionDetectedVertical(HINSTANCE hInstance, HWND hWnd);
	HRESULT ClearRoadSignMotionDetected(HINSTANCE hInstance, HWND hWnd);
	HRESULT CreateD2DBitmapFromFile(HINSTANCE hInstance, HWND hWnd, LPCWSTR pImageFileName,
		UINT pImageWidth, UINT pImageHeight,
		vector<PixelObject> *ppPixelObjectArray);
	HRESULT DrawLinesBetweenPoints(HINSTANCE hInstance, HWND hWnd, Point2D pPointArray[3]);
	HRESULT DrawLinesBetweenPoints(HINSTANCE hInstance, HWND hWnd, vector<Point2D> pPointArray);

	HRESULT DrawRoadSignStateMachineOddEven(HINSTANCE hInstance, HWND hWnd, UINT pOutputIntensity);
	HRESULT DrawRoadSign(HINSTANCE hInstance, HWND hWnd, UINT pLevelOfIdentity);
	HRESULT DrawRoadSign(HINSTANCE hInstance, HWND hWnd, UINT pLevelOfIdentity,
		LPWSTR pTexToWrite);
	HRESULT DrawRoadSignVertical(HINSTANCE hInstance, HWND hWnd, UINT pPixelNumber, UINT pLevelOfIntensity);
	HRESULT DrawRoadSignMotionDetectedVertical(HINSTANCE hInstance, HWND hWnd, BOOL pIsMotionUndefined);
	HRESULT DrawRoadSign(HINSTANCE hInstance, HWND hWnd, UINT pAmountOfIdenticalPixels, UINT pAmountOfDifferentPixels);
	HRESULT DrawRoadSignStateMachine(HINSTANCE hInstance, HWND hWnd, UINT pOutputIntensity);
	HRESULT CreateHistogramEffect(HINSTANCE hInstance, HWND hWnd, ID2D1Bitmap* pInputBitmap, ID2D1Effect** ppHistogramEffect);
	HRESULT RenderHistogramEffect(HINSTANCE hInstance, HWND hWnd, ID2D1Effect* pHistogramEffect, vector<float> *pDataArray);
	HRESULT CreateBitmapDrawingClipObjectUsingClipper(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pWidth, float pHeight,
		vector<PixelObject> *ppPixelObject);
	HRESULT CreateBitmapDrawingClipObject(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pWidth, float pHeight,
		vector<PixelObject> *ppPixelObject);
	HRESULT CreateBitmapDrawingClipObjectWithInts(HINSTANCE hInstance,
		HWND hWnd,
		int pXPosition,
		int pYPosition,
		int pWidth,
		int pHeight,
		vector<PixelObjectInt> *ppPixelObject);
	HRESULT SaveImageFromPixelObjectArray(HINSTANCE hInstance,
		HWND hWnd, UINT pImageWidth, UINT pImageHeight, 
		vector<PixelObject> pInputPixelDataVector,
		LPTSTR pFileName);
	HRESULT SaveBitmapToStream(
		HINSTANCE hInstance,
		HWND hWnd,
		LPCSTR pFileName,
		ID2D1Bitmap1* pBItmapObject);
	HRESULT CreateBitmapInfoStruct(HINSTANCE hInstance,
		HWND hWnd, HBITMAP hBmp, PBITMAPINFO* pBitmapInfoObject);
	HRESULT CreateBMPFile(HINSTANCE hInstance,
		HWND hWnd,
		LPTSTR pFileName,
		PBITMAPINFO pBitmapInfoObject,
		HBITMAP pBitmapObject,
		HDC hDC);
	HRESULT GeneratePathGeometryForGivenRGBPixels(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObject> ppixelDataArray,
		ID2D1Factory* pFactoryObject,
		ID2D1GeometrySink** pGeometrySink,
		ID2D1PathGeometry** pGraphicsPathGeometry,
		ID2D1SolidColorBrush** pSolidColorBrushBlack,
		ID2D1SolidColorBrush** pSolidColorBrushWhite);
	HRESULT DrawPathGeometryForGivenDataSetOfPixels(HINSTANCE hInstance, HWND hWnd, vector<PixelObject> pPixelObjectArray);

	HRESULT CreateD2DBitmapClipObject(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pWidth, float pHeight, vector<PixelObject> *pixelObjectOutput);
	HRESULT CreateD2DBitmapClipObjectWithPixels(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pWidth, float pHeight, vector<PixelObject> *pixelObjectOutput);
	HRESULT SegmentImageIntoRectangularSectors(HINSTANCE hInstance, HWND hWnd, LPCWSTR
		pFileName, float pImageWidth, float pImageHeight, float pSampleWidth, float pSampleHeight, vector<ImageSectorObject> *ppImageSectorObjectObjectCollection);
	HRESULT CreateBitmapDrawingClipObject(HINSTANCE hInstance, HWND hWnd, LPCWSTR
		pFileName, float pXPosition, float pYPosition, float pWidth, float pHeight,
		vector<PixelObject> *ppPixelObject);

	HRESULT CreateBitmapDrawingClipObject(HINSTANCE hInstance, HWND hWnd, LPCWSTR
		pFileName, float pXPosition, float pYPosition, float pWidth, float pHeight,
		float pRectXPosition, float pRectangleYPosition, float pRectangleWidth, float pRectangleHeight,
		vector<PixelObject> *ppPixelObject);


	HRESULT PlotAGraphGeometryForGivenQuadraticData(HINSTANCE hInstance, HWND hWnd, vector<OutputStructureForGraph2D> pPixelDataArray);
	HRESULT GeneratePathGeometryForQuadraticFunctuinData(HINSTANCE hInstance,
		HWND hWnd,
		vector<OutputStructureForGraph2D> ppixelDataArray,
		ID2D1Factory* pFactoryObject,
		ID2D1GeometrySink** pGeometrySink,
		ID2D1PathGeometry** pGraphicsPathGeometry,
		ID2D1SolidColorBrush** pSolidColorBrushBlack,
		ID2D1SolidColorBrush** pSolidColorBrushWhite);

	HRESULT GeneratePathGeometryForGivenIntensityDataAlongX(HINSTANCE hInstance,
		HWND hWnd,
		vector<PixelObjectHSI> ppixelDataArray,
		ID2D1Factory* pFactoryObject,
		ID2D1GeometrySink** pGeometrySink,
		ID2D1PathGeometry** pGraphicsPathGeometry,
		ID2D1SolidColorBrush** pSolidColorBrushBlack,
		ID2D1SolidColorBrush** pSolidColorBrushWhite);
	HRESULT ApplyHistogramEffect(HINSTANCE hInstance, HWND hWnd, vector<float> *pImageDataCollection);
	HRESULT DrawCoordinateAxesObject(HINSTANCE hInstance,
		HWND hWnd,
		ID2D1SolidColorBrush* pSolidColorBrush);
	HRESULT DrawCoordinateAxesObject(HINSTANCE hInstance, HWND hWnd, ID2D1SolidColorBrush* pSolidColorBrush, UINT pLineThickness);
	HRESULT CreateBitmapDrawingClipObjectUsingClipperWithFileNamesForMasking(HINSTANCE hInstance, HWND hWnd, LPCWSTR pImageFileName, float pXPosition, float pYPosition, float pWidth, float pHeight,
		vector<PixelObject> *ppPixelObject,
		ID2D1Bitmap **ppOutputBitmap);
	HRESULT CreateBitmapDrawingClipObjectUsingClipperWithFileNames(HINSTANCE hInstance, HWND hWnd, LPCWSTR pImageFileName,
		float pXPosition, float pYPosition, float pImageWidth, float pImageHeight,
		vector<PixelObject> *ppPixelObject);

	HRESULT RenderGraphGeometryAlongThePlane(HINSTANCE hInstance, HWND hWnd,
		float pLineThicknessValue,
		ID2D1SolidColorBrush* pSolidColorBrushObjectBackground,
		ID2D1SolidColorBrush* pSolidColorBrushObjectGraphDrawing,
		ID2D1PathGeometry* pPathGeometryObject);
	void Update(float timeTotal, float timeDelta);

	HRESULT CreateBitmapFromArrayOfPixels(HINSTANCE hInstance, HWND hWnd, LPVOID& pixelArray, HBITMAP pImageBitmap, ID2D1Bitmap** ppOuputBitmap);
	HRESULT ConvertD2DBitmapToWICBitmap(HINSTANCE hInstance, HWND hWnd, ID2D1Bitmap* pBitmap, IWICBitmap* pBitmapObvect);
	HRESULT CompareIndependentBitmaps(HINSTANCE hInstance, HWND hWnd, IWICBitmap* pBitmap1, IWICBitmap* pBitmap2, vector<PixelObject**> pOutputData);
	HRESULT LoadAndClipBitmapImage(HINSTANCE hInstance, HWND hWnd);
	ID3D11Texture2D* m_pSurfaceRGBA;
	WICPixelFormatGUID* pImageContentGuid;
	ID2D1GradientStopCollection *pGradientStops;
	ID2D1RadialGradientBrush* m_pRadialGradientBrush;
	
	IDWriteTextFormat *m_pTextFormat;
	HWND childWindow;
	ID2D1SolidColorBrush *m_pBlueBrush;
	ID2D1SolidColorBrush *m_pRedBrush;
	ID2D1SolidColorBrush *m_pWhiteBrush;
	ID2D1SolidColorBrush *m_pBlackBrush;
	Direct2DManagementClass(HINSTANCE hInstance, HWND hWnd);
	~Direct2DManagementClass();
	PAINTSTRUCT ps;
	IWICBitmapEncoder* m_pWICEncoder;
	IWICStream* pBitmapStream;
	IWICBitmapSource* updatedBitmap;
	IWICColorContext* pImageColorContext;
	IWICMetadataQueryReader* pImageMetadataReader;
	IWICStream* pImageReadingStream;
	ID2D1Device* md2ddevice;
	HRESULT CreateChildWindow(HINSTANCE hInstance, HWND hWnd);
	void HandleDeviceLost();
	void UpdateForWindowSizeChange();
	void Present();
	void SetDpi(float dpi);
	void ClearRenderTargetContent(HINSTANCE hInstance, HWND hWnd);
	HRESULT CreateBitmapWithASetOfDimensions(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pImageWidth, float pImageHeight);
	HRESULT CreateD2DBitmapFromFile(HINSTANCE hInstance, HWND hWnd, LPCSTR pCSVFileName,
		UINT pImageWidth, UINT pImageHeight,
		vector<PixelObject> *ppPixelObjectArray);
	HRESULT OpenAndDrawImageFile(HINSTANCE hInstance, HWND hWnd);
	D2D1_RECT_F pRect;
	HRESULT SelectImageRectangleAndOutputPixels(HINSTANCE hInstance, HWND hWnd);
	HRESULT DrawD2DContentAxisAlignedClip(HINSTANCE hInstance, HWND hWnd, ID2D1Bitmap* pBitmapToDraw, D2D1_RECT_F outputRect);
	HRESULT DrawD2DContentAxisAlignedClipWithRectangleGeometry(HINSTANCE hInstance, HWND hWnd,
		ID2D1Bitmap* pBitmapToDraw,
		ID2D1PathGeometry* pPathGeometryObject,
		D2D1_RECT_F outputRect);
	HRESULT DrawPixelArrayOutput(HINSTANCE hInstance, HWND hWnd, vector<OutputPixelObjectWithAngleValuesD2D> pPixelObjectArray);
	HRESULT DrawD2DContentClip(HINSTANCE hInstance, HWND hWnd, ID2D1Bitmap* pBitmapToDraw,  D2D1_RECT_F outputRect);
	HRESULT CreateBitmapWithASetOfLayers(HINSTANCE hInstance, HWND hWnd);
	HRESULT RenderImageRegionObject(HINSTANCE hInstance, HWND hWnd, ID2D1Bitmap* pBitmapToDraw);
	HRESULT RenderBitmapWithLayers(HINSTANCE hInstance, HWND hWnd, ID2D1Bitmap* pBitmapToDraw);
	HRESULT CreateWicBitmapFromMemory(HINSTANCE hInstance, HWND hWnd, BYTE* pDataToLoad, IWICFormatConverter** pConvertedBitmap, ID2D1Bitmap** pD2D1Bitmap);
	HRESULT CreateWicBitmapFromSelectedFile(HINSTANCE hInstance, HWND hWnd, IWICFormatConverter** pConvertedBitmap, ID2D1Bitmap** pD2D1Bitmap);
	HRESULT CreateAndDrawAGeometryForAGivenData(HINSTANCE hInstance, HWND hWnd, vector<float> pXValueInputData, vector<float> pYValues);
	HRESULT InitializeGeometryResources(HINSTANCE hInstance, HWND hWnd, ID2D1GeometrySink** pGeometrySinkObject,
		ID2D1Brush** pBrushToDrawGeometry);
	HRESULT DrawPathGeometryForGivenDataSet(HINSTANCE hInstance, HWND hWnd,
		float pCentreX, float pCentreY,
		 vector<PixelObject> pPixelObjectArray);
	HRESULT DestroyFilePermanently(HINSTANCE hInstance, HWND hWnd, LPWSTR pFileName);
	HRESULT SaveScreenDataToImage(HINSTANCE hInstance, HWND hWnd,
		UINT pImageWidth,
		UINT pImageHeight,
		IWICBitmapSource* pSourceObject,
		LPCTSTR lpszFileName);
	HRESULT RenderGraphGeometryQuadAlongThePlane(HINSTANCE hInstance, HWND hWnd,
		float pLineThicknessValue,
		ID2D1SolidColorBrush* pSolidColorBrushObjectBackground,
		ID2D1SolidColorBrush* pSolidColorBrushObjectGraphDrawing,
		ID2D1PathGeometry* pPathGeometryObjectOne,
		ID2D1PathGeometry* pPathGeometryObjectTwo,
		ID2D1PathGeometry* pPathGeometryObjectThree,
		ID2D1PathGeometry* pPathGeometryObjectFour);
	BOOL SaveHBITMAPToFileObject(HBITMAP hBitmap, LPCTSTR lpszFileName);
	HRESULT CreateBitmapDrawingClipObjectWithoutD2D(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pWidth, float pHeight,
		vector<PixelObject> *ppPixelObject);
	HRESULT ExportDataFromTextFileAndDrawGeometry(HINSTANCE hInstance, HWND hWnd, LPCSTR pFileName, vector<float> *pXValues, vector<float> *pYValues);
	HRESULT PrepareDataAndDrawGeometry(HINSTANCE hInstance, HWND hWnd, vector<float> *pXValues, vector<float> *pYValues);
	HRESULT ConnectToProgramViaPipeAndLoadData(HINSTANCE hInstance, HWND hWnd, HANDLE pHandle, vector<float> pXValues, vector<float> pYValues);	
	HRESULT SaveRenderTargetToStream(HINSTANCE hInstance, HWND hWnd);
	HRESULT DrawPixelBuffer(HINSTANCE hIntance, HWND hWnd, vector<PixelObject> pPixelObjectArray);
	HRESULT PlotGraphForGivenDataSet(HINSTANCE hInstance, HWND hWnd, vector<float> pXValuesForGiverData, vector<float> pYValuesForGivenData);
	HRESULT CreatePathGeometryForASetOfData(HINSTANCE hInstance, HWND hWnd, vector<float> pXValues, vector<float> pYValues, vector<D2D1_RECT_F> *ppRectangleSet);
	HRESULT RenderCoordinatePlane(HINSTANCE hInstance, HWND hWnd, ID2D1SolidColorBrush* pSolidColorBrush);
	HRESULT RenderPathGeometryObject(HINSTANCE hInstance, HWND hWnd, vector<D2D1_RECT_F> pPathGeometry,
		ID2D1SolidColorBrush* pSolidColorBrush);
	HRESULT CreateD2DBitmapWithStoredPixels(HINSTANCE hInstance, HWND hWnd, ID2D1Bitmap** pBitmapObject, vector<PixelObject> *pOutputVector);
	HRESULT WriteMediaDataToFile(HANDLE hFile, void* pData, DWORD cb);
	
	HRESULT InitializeDirect2DEngine(HINSTANCE hInstance, HWND hWnd);
	HRESULT CreateDeviceIndependentResources(HINSTANCE hInstance, HWND hWnd);
	HRESULT CreateDeviceResources(HINSTANCE hInstance, HWND hWnd);
	HRESULT DrawD2DContent(HINSTANCE hInstance, HWND hWnd, ID2D1Bitmap* pBitmapToDraw);
	HRESULT DrawDefaultText(HWND hWnd);
	void DiscardDeviceResources();
	void OnResize(D2D1_SIZE_U size);
	void CreateBitmapGrid(HWND hWnd);
	void CreateScaleEffect();
	void DestroyBitmapGrid(HWND hWnd);
	HRESULT CreateD2DBitmapAndRenderToArea(HWND hWnd, int renderTargetIndex);
	void RenderImageToRenderTarget(HWND hWnd, ID2D1Bitmap* currentBitmap, int renderTargetIndex);
	HRESULT CreateDesktopIcon(HWND hWnd, LPCSTR fname);
	HRESULT CreateD2DBitmapFromFileW(HWND hWnd, WCHAR* szFileName);
	HRESULT DrawImageWithoutScaling(HWND hWnd);
	void SetFullscreenMode(HWND hWnd);
	HRESULT CreatePieChartDiagram(HWND hWnd);
	HRESULT CreateSecondaryDeviceIndependentResources();
	HRESULT CreateSecondaryDeviceResources(HWND hWnd);
	HRESULT CreateWindowSizeDependentResources(HINSTANCE hInstance, HWND hWnd);
	IWICImagingFactory2* pImagingFactoryUpdate;
	IWICBitmapEncoder* pImageEncoder;
	HRESULT SaveBitmapToStream(
		HINSTANCE hInstance,
		HWND hWnd,
		LPCWSTR pFileName,
		UINT pImageWidth,
		UINT pImageHeight,
		BYTE* pBItmapObject);

	IDWriteFactory *m_pDWriteFactory;
	HRESULT CreateBitmapDrawingResources(HWND hWnd);
	HRESULT CreateDeviceResourcesW(HWND hWnd);
	HRESULT SaveImageToFile(HINSTANCE hInstance, HWND hWnd);
	UINT frameCount;
	HRESULT CreateD2DBitmapFromFile(HINSTANCE hInstance, HWND hWnd,
	UINT pImageWidth, UINT pImageHeight,
	vector<PixelObject> *ppPixelObjectArray);
	HRESULT CreateD2DBitmapFromFileD2D(HINSTANCE hInstance, HWND hWnd, ID2D1Bitmap** ppBitmapObject);
	HRESULT DrawRoadSignMotionDetected(HINSTANCE hInstance, HWND hWnd,  BOOL pIsMotionUndefined);
    
    HRESULT CreateD2DBitmapWithStoredPixels(HINSTANCE hInstance, HWND hWnd, vector<PixelObject> *pOutputVector);
	HRESULT CaptureScreenShotToFile(HINSTANCE hInstance, HWND hWnd, LPCWSTR filename);
	HRESULT SaveBitmapObject(LPCSTR filename, HBITMAP bmp, HPALETTE pal);
	IDWriteFactory2* pDwriteFactoryObject = nullptr;


	ID3D11Device1* pGraphicsDrawingDevice;
	ID3D11DeviceContext1* pGraphicsDrawingContext;

	void ProcessWindowResize(HWND hWnd);

	bool LoadContent() {return true; }
	void UnloadContent() {} 
	void UpdateScene() {}
	void UpdateScene(float dt) {}
	void DrawScene() {}
	void OnResize() {}
	void Update() {}
	void Render() {}
	void ValidateD3D11Device(HINSTANCE hInstance, HWND hWnd);

	HRESULT CreateImageEncoder(HINSTANCE hInstance, HWND hWnd, LPCWSTR outputFilePath);
	HRESULT SaveScreenDataToImage(HINSTANCE hInstance, HWND hWnd);
	vector<RectangleStructure> pRectangleStructureArrayOne;
	vector<RectangleStructure> pRectangleStructureArrayTwo;
	vector<D2D1_RECT_F> pHouseArrayOne;
	vector<D2D1_RECT_F> pHouseArrayTwo;
	HRESULT CreateBitmapDrawingClipObject(HINSTANCE hInstance, HWND hWnd, LPWSTR szFileName, float pXPosition, float pYPosition, float pWidth, float pHeight,
		vector<PixelObject> *ppPixelObject,
		ID2D1Bitmap** pBitmapObject);
	HRESULT ClipImageAgainstABitmapMask(HINSTANCE hInstance,
		HWND hWnd, LPWSTR szFileNameOne,
		LPWSTR szFileNameTwo,
		float pXPosition,
		float pYPosition,
		float pWidth,
		float pHeight,
		vector<PixelObject> *pPixelObjectVector);
	



private:
	HINSTANCE m_hInst;
	ID2D1Factory* pSecondaryD2DFactory;
	ID2D1HwndRenderTarget* pSecondaryRenderTarget;
	HRESULT SaveBitmapToStream(
            HINSTANCE hInstance,
		HWND hWnd,
        LPCSTR pFileName,
		ID2D1Bitmap* pBItmapObject);

	int m_dpiX;
	int m_dpiY;
	int m_childOffsetX;
	int m_childOffsetY;
	IDXGIDevice* pDXGIDevice;
	D3D_FEATURE_LEVEL featureLevel;
	ID2D1SolidColorBrush *m_pSolidColorBrush;
	ID2D1SimplifiedGeometrySink *pGeometrySink;
	ID2D1RenderTarget* backBufferTarger2D;
	IWICBitmapDecoder *pDecoder;
	IWICImagingFactory* pImagingFactory;
	std::vector<ID2D1DCRenderTarget*>m_pD2DDCRenderTargetCollection;
	std::vector<ID2D1RenderTarget*> m_pD2DBitmapRenderTargerCollection;
	std::vector<IWICBitmap*> m_pD2DBitmapCollection;
	D3D11_MAPPED_SUBRESOURCE mappedTex;
	ID2D1SolidColorBrush* pRedSolidColorBrush;
	ID2D1RadialGradientBrush* pRadialGradientBrush;
	ID3D11Device *md3ddevice;
	ID3D11DeviceContext *md3dcontext;
	IWICBitmapDecoder* pDecoderObject;
	list<LPCSTR> ImagesList;
	
	IDXGIDevice* mdxgiDevice;
	IDXGIAdapter* pdxgiAdapter;
	IWICBitmapScaler *m_pBitmapScaler;
	IDXGISurface* pGraphicSourceBuffer;
	IWICDevelopRaw *myImageFixingPipe;
	IWICComponentFactory* m_pComponentFactory;
	
	
	
	
};