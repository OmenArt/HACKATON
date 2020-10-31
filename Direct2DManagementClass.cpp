#include "stdafx.h"
#include "Direct2DManagementClass.h"
#include "d2d1helper.h"
#include <CommDlg.h>
#include "BasicPlayback.h"
#include <ole2.h>
#include <olectl.h>
#include <time.h>



bool WayToSortPixelssAscending(PixelObject pItemOne, PixelObject pItemTwo)
{
	bool pResult = false;
	pResult = pItemOne.xPosition < pItemTwo.xPosition;
	return pResult;
}

bool WayToSortPixelsDescending(PixelObject pItemOne, PixelObject pItemTwo)
{
	bool pResult = false;
	pResult = pItemOne.xPosition > pItemTwo.xPosition;
	return pResult;
}



POINT ConvertPoint2DToPoint(Point2D pPoint2D)
{
	POINT pPointVar = POINT();
	pPointVar.x = pPoint2D.xPosition;
	pPointVar.y = pPoint2D.yPosition;
	return pPointVar;
}


Point2D GetPointByNumber(vector<Point2D> pPoint2DArray, int pPixelNumber)
{
	Point2D pPointToGet;
	pPointToGet.pPointNumber = 0.0f;
	pPointToGet.xPosition = 0.0f;
	pPointToGet.yPosition = 0.0f;

	for each (Point2D var in pPoint2DArray)
	{
		if (var.pPointNumber == pPixelNumber)
		{
			pPointToGet.pPointNumber = var.pPointNumber;
			pPointToGet.xPosition = var.xPosition;
			pPointToGet.yPosition = var.yPosition;
		}
	}

	return pPointToGet;

}





const FLOAT DEFAULT_DPI = 96.0f;
const D2D1_PIXEL_FORMAT format = D2D1::PixelFormat();
const UINT DO_YOU_WANT_MESSAGES = 0;

using namespace Gdiplus;

struct WICConvert
{
	GUID        source;
	GUID        target;
};

struct WICTranslate
{
	GUID                wic;
	DXGI_FORMAT         format;
};

static WICTranslate g_WICFormats[] =
{
	{ GUID_WICPixelFormat128bppRGBAFloat, DXGI_FORMAT_R32G32B32A32_FLOAT },

	{ GUID_WICPixelFormat64bppRGBAHalf, DXGI_FORMAT_R16G16B16A16_FLOAT },
	{ GUID_WICPixelFormat64bppRGBA, DXGI_FORMAT_R16G16B16A16_UNORM },

	{ GUID_WICPixelFormat32bppRGBA, DXGI_FORMAT_R8G8B8A8_UNORM },
	{ GUID_WICPixelFormat32bppBGRA, DXGI_FORMAT_B8G8R8A8_UNORM }, // DXGI 1.1
	{ GUID_WICPixelFormat32bppBGR, DXGI_FORMAT_B8G8R8X8_UNORM }, // DXGI 1.1

	{ GUID_WICPixelFormat32bppRGBA1010102XR, DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM }, // DXGI 1.1
	{ GUID_WICPixelFormat32bppRGBA1010102, DXGI_FORMAT_R10G10B10A2_UNORM },
	{ GUID_WICPixelFormat32bppRGBE, DXGI_FORMAT_R9G9B9E5_SHAREDEXP },

#ifdef DXGI_1_2_FORMATS

	{ GUID_WICPixelFormat16bppBGRA5551, DXGI_FORMAT_B5G5R5A1_UNORM },
	{ GUID_WICPixelFormat16bppBGR565, DXGI_FORMAT_B5G6R5_UNORM },

#endif // DXGI_1_2_FORMATS

	{ GUID_WICPixelFormat32bppGrayFloat, DXGI_FORMAT_R32_FLOAT },
	{ GUID_WICPixelFormat16bppGrayHalf, DXGI_FORMAT_R16_FLOAT },
	{ GUID_WICPixelFormat16bppGray, DXGI_FORMAT_R16_UNORM },
	{ GUID_WICPixelFormat8bppGray, DXGI_FORMAT_R8_UNORM },

	{ GUID_WICPixelFormat8bppAlpha, DXGI_FORMAT_A8_UNORM },

#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/)
	{ GUID_WICPixelFormat96bppRGBFloat, DXGI_FORMAT_R32G32B32_FLOAT },
#endif
};

//-------------------------------------------------------------------------------------
// WIC Pixel Format nearest conversion table
//-------------------------------------------------------------------------------------



// WIC Pixel Format nearest conversion table
//-------------------------------------------------------------------------------------



const D2D1_RENDER_TARGET_PROPERTIES properties = 
	D2D1::RenderTargetProperties(
	D2D1_RENDER_TARGET_TYPE_DEFAULT,
	 D2D1::PixelFormat(),
	DEFAULT_DPI, // default dpi
	DEFAULT_DPI, // default dpi
	D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE);


LRESULT CALLBACK ChildWindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


LRESULT CALLBACK ChildWindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if (msg == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		Direct2DManagementClass *pDemoApp = (Direct2DManagementClass*)pcs->lpCreateParams;

		::SetWindowLongPtrW(
			hwnd,
			GWLP_USERDATA,
			PtrToUlong(pDemoApp)
			);

		result = 1;
	}
	else
	{
		Direct2DManagementClass *pDemoApp = reinterpret_cast<Direct2DManagementClass *>(static_cast<LONG_PTR>(
			::GetWindowLongPtrW(
			hwnd,
			GWLP_USERDATA
			)));

		bool wasHandled = false;

		if (pDemoApp)
		{
			switch (msg)
			{
			case WM_PAINT:
				{
					break;
				}
				wasHandled = true;
				result = 0;
				break;

			case WM_LBUTTONDOWN:
				{
					break;
					
				}   
					
				wasHandled = true;
				result = 0;
				break;

			case WM_TIMER:
				{
					break;
				}
				wasHandled = true;
				result = 0;
				break;
			}
		}

		if (!wasHandled)
		{
			result = DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}

	return result;
}


Direct2DManagementClass::Direct2DManagementClass(HINSTANCE hInstance, HWND hWnd) :
			m_pD2DFactory(NULL),
			m_pConvertedSourceBitmap(NULL),
			m_pWICFactory(NULL),
			m_pD2DBitmap(NULL),
			m_pD2DDeviceContext1(NULL),
			m_pPathGeometry(NULL),
			m_pObjectGeometry(NULL),
			m_pBlueBrush(NULL),
			m_pRedBrush(NULL)
{
		
	pDXGIFactory = nullptr;
	pGraphicsDrawingDevice = nullptr;
	pGraphicsDrawingContext = nullptr;
	mdxgidevice = nullptr;
	m_pDXGIFactory2 = nullptr;
	m_pD2DDevice = nullptr;
	m_pD2DDeviceContext = nullptr;
	m_pD2DDeviceContext1 = nullptr;

}



Direct2DManagementClass::~Direct2DManagementClass()
{
	
		DiscardDeviceResources();
		
		
		

}

HRESULT Direct2DManagementClass::SimulateBehaviourOfPathGeometryTools(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr = S_OK;
	cout << "This Function will simulate the Behaviour of  Path Geometry system" << endl;
	cout << "For this Experiment we will Require the following tools" << endl;

	cout << "1 Direct2D Device Context" << endl;
	cout << "1 Solid Color brush" << endl;
	cout << "4 Path Geometry Items" << endl;

	ID2D1SolidColorBrush* pBacKGroundBrush = nullptr;
	ID2D1SolidColorBrush* pBrushToDraw = nullptr;
	ID2D1PathGeometry* pLeftMountaintGeometry = nullptr;
	ID2D1PathGeometry* pRightMountaintGeometry = nullptr;
	ID2D1PathGeometry* pSunGeometryObject = nullptr;
	ID2D1PathGeometry* pRiverGeometryObject = nullptr;

	cout << "We will start by populating the necessary resources for the simulation" << endl;
	hr = InitializePathGeometryResources(hInstance,
		hWnd,
		&pBacKGroundBrush,
		&pBrushToDraw,
		&pLeftMountaintGeometry,
		&pRightMountaintGeometry,
		&pSunGeometryObject,
		&pRiverGeometryObject);

	if (FAILED(hr))
	{
		int pLastError = GetLastError();
		cout << "Unable to initialize the resources for the simulation Error: " << pLastError << endl;
		return hr;
 	}

	cout << "Rendering The Geometry Along the plane" << endl;

	hr = RenderPathGeometryOnThePlane(hInstance,
		hWnd,
		pBacKGroundBrush,
		pBrushToDraw,
		pLeftMountaintGeometry,
		pRightMountaintGeometry,
		pSunGeometryObject,
		pRiverGeometryObject);

	cout << "Cleaning the simulation resources" << endl;

	SafeRelease(&pBrushToDraw);
	SafeRelease(&pLeftMountaintGeometry);
	SafeRelease(&pRightMountaintGeometry);
	SafeRelease(&pSunGeometryObject);
	SafeRelease(&pRiverGeometryObject);

	return hr;
}



HRESULT Direct2DManagementClass::InitializePathGeometryResources(HINSTANCE hInstance,
	HWND hWnd,
	ID2D1SolidColorBrush** ppBackGroundBrush,
	ID2D1SolidColorBrush** ppSolidColorBrush,
	ID2D1PathGeometry** ppLeftMountainGeometryObject,
	ID2D1PathGeometry** ppRightMountainGeometryObject,
	ID2D1PathGeometry** ppSunGeometryObject,
	ID2D1PathGeometry** ppRiverGeometryObject)
{
	HRESULT hr = S_OK;

	ID2D1PathGeometry* pLeftMountainGeometry = nullptr;
	ID2D1PathGeometry* pRightMountainGeometry = nullptr;
	ID2D1PathGeometry* pSunGeometry = nullptr;
	ID2D1PathGeometry* pRiverGeometry = nullptr;
	ID2D1GeometrySink* pGeometrySinkObject = nullptr;
	ID2D1SolidColorBrush* pSolidColorBrush = nullptr;
	ID2D1SolidColorBrush* pSolidColorBrushBackground = nullptr;

	cout << "This Method will be used To Simulate the behaviour of path geometry" << endl;
	cout << "First We need to Check that Our Devices are in order" << endl;
	cout << "First we will check our factories" << endl;
	

	cout << "Our Next Step Will be creation of device resources" << endl;
	m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::WhiteSmoke), &pSolidColorBrushBackground);
	m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pSolidColorBrush);

	ID2D1Factory* pFactoryObject = nullptr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &pFactoryObject);
	if (SUCCEEDED(hr))
	{
		if (DISABLE_ALL_MESSAGE_BOXES == FALSE)
		{
			MessageBoxA(NULL, (LPCSTR)"Cоздан объект Direct2D Factory", (LPCSTR)"Message", 0);
		}
	}
	
	cout << "Now We will create the instruments for path geometry" << endl;
	hr = pFactoryObject->CreatePathGeometry(&pLeftMountainGeometry);

	if (FAILED(hr))
	{
		int pLastError = GetLastError();
		cout << "Unable to Create the Path Geometry Object Error: " << pLastError << endl;
		return hr;
	}

	cout << "Our Next Step would be creation of a geometry sink object for left mountain" << endl;
	hr = pLeftMountainGeometry->Open(&pGeometrySinkObject);

	
	if (FAILED(hr))
	{
		int pLastError = GetLastError();
		cout << "Unable to Open the Geometry Sink Object: " << pLastError << endl;
		return hr;
	}

	cout << "Setting Up the Fill Mode" << endl;
	pGeometrySinkObject->SetFillMode(D2D1_FILL_MODE_WINDING);

	pGeometrySinkObject->BeginFigure(
		D2D1::Point2F(346, 255),
		D2D1_FIGURE_BEGIN_FILLED
		);
	D2D1_POINT_2F points[5] = {
		D2D1::Point2F(267, 177),
		D2D1::Point2F(236, 192),
		D2D1::Point2F(212, 160),
		D2D1::Point2F(156, 255),
		D2D1::Point2F(346, 255),
	};
	pGeometrySinkObject->AddLines(points, ARRAYSIZE(points));
	pGeometrySinkObject->EndFigure(D2D1_FIGURE_END_CLOSED);

	SafeRelease(&pGeometrySinkObject);

	cout << "Now We will create the right mountain geometry" << endl;
	hr = pFactoryObject->CreatePathGeometry(&pRightMountainGeometry);

	if (FAILED(hr))
	{
		int pLastError = GetLastError();
		cout << "Unable to create the right mountain geometry" << endl;
		return hr;
	}

	hr = pRightMountainGeometry->Open(&pGeometrySinkObject);
	
	if (FAILED(hr))
	{
		int pLastError = GetLastError();
		cout << "Unable to create the right mountain geometry" << endl;
		return hr;
	}

	pGeometrySinkObject->SetFillMode(D2D1_FILL_MODE_WINDING);

	pGeometrySinkObject->BeginFigure(
		D2D1::Point2F(575, 263),
		D2D1_FIGURE_BEGIN_FILLED
		);
	D2D1_POINT_2F points1[] = {
		D2D1::Point2F(481, 146),
		D2D1::Point2F(449, 181),
		D2D1::Point2F(433, 159),
		D2D1::Point2F(401, 214),
		D2D1::Point2F(381, 199),
		D2D1::Point2F(323, 263),
		D2D1::Point2F(575, 263)
	};
	pGeometrySinkObject->AddLines(points1, ARRAYSIZE(points1));
	pGeometrySinkObject->EndFigure(D2D1_FIGURE_END_CLOSED);
	hr = pGeometrySinkObject->Close();

	SafeRelease(&pGeometrySinkObject);

	cout << "Creating the Sun Geometry Object" << endl;

	hr = pFactoryObject->CreatePathGeometry(&pSunGeometry);
	if (SUCCEEDED(hr))
	{
		pGeometrySinkObject = NULL;

		hr = pSunGeometry->Open(&pGeometrySinkObject);
		if (SUCCEEDED(hr))
		{
			pGeometrySinkObject->SetFillMode(D2D1_FILL_MODE_WINDING);

			pGeometrySinkObject->BeginFigure(
				D2D1::Point2F(270, 255),
				D2D1_FIGURE_BEGIN_FILLED
				);
			pGeometrySinkObject->AddArc(
				D2D1::ArcSegment(
				D2D1::Point2F(440, 255), // end point
				D2D1::SizeF(85, 85),
				0.0f, // rotation angle
				D2D1_SWEEP_DIRECTION_CLOCKWISE,
				D2D1_ARC_SIZE_SMALL
				));
			pGeometrySinkObject->EndFigure(D2D1_FIGURE_END_CLOSED);

			pGeometrySinkObject->BeginFigure(
				D2D1::Point2F(299, 182),
				D2D1_FIGURE_BEGIN_HOLLOW
				);
			pGeometrySinkObject->AddBezier(
				D2D1::BezierSegment(
				D2D1::Point2F(299, 182),
				D2D1::Point2F(294, 176),
				D2D1::Point2F(285, 178)
				));
			pGeometrySinkObject->AddBezier(
				D2D1::BezierSegment(
				D2D1::Point2F(276, 179),
				D2D1::Point2F(272, 173),
				D2D1::Point2F(272, 173)
				));
			pGeometrySinkObject->EndFigure(D2D1_FIGURE_END_OPEN);

			pGeometrySinkObject->BeginFigure(
				D2D1::Point2F(354, 156),
				D2D1_FIGURE_BEGIN_HOLLOW
				);
			pGeometrySinkObject->AddBezier(
				D2D1::BezierSegment(
				D2D1::Point2F(354, 156),
				D2D1::Point2F(358, 149),
				D2D1::Point2F(354, 142)
				));
			pGeometrySinkObject->AddBezier(
				D2D1::BezierSegment(
				D2D1::Point2F(349, 134),
				D2D1::Point2F(354, 127),
				D2D1::Point2F(354, 127)
				));
			pGeometrySinkObject->EndFigure(D2D1_FIGURE_END_OPEN);

			pGeometrySinkObject->BeginFigure(
				D2D1::Point2F(322, 164),
				D2D1_FIGURE_BEGIN_HOLLOW
				);
			pGeometrySinkObject->AddBezier(
				D2D1::BezierSegment(
				D2D1::Point2F(322, 164),
				D2D1::Point2F(322, 156),
				D2D1::Point2F(314, 152)
				));
			pGeometrySinkObject->AddBezier(
				D2D1::BezierSegment(
				D2D1::Point2F(306, 149),
				D2D1::Point2F(305, 141),
				D2D1::Point2F(305, 141)
				));
			pGeometrySinkObject->EndFigure(D2D1_FIGURE_END_OPEN);

			pGeometrySinkObject->BeginFigure(
				D2D1::Point2F(385, 164),
				D2D1_FIGURE_BEGIN_HOLLOW
				);
			pGeometrySinkObject->AddBezier(
				D2D1::BezierSegment(
				D2D1::Point2F(385, 164),
				D2D1::Point2F(392, 161),
				D2D1::Point2F(394, 152)
				));
			pGeometrySinkObject->AddBezier(
				D2D1::BezierSegment(
				D2D1::Point2F(395, 144),
				D2D1::Point2F(402, 141),
				D2D1::Point2F(402, 142)
				));
			pGeometrySinkObject->EndFigure(D2D1_FIGURE_END_OPEN);

			pGeometrySinkObject->BeginFigure(
				D2D1::Point2F(408, 182),
				D2D1_FIGURE_BEGIN_HOLLOW
				);
			pGeometrySinkObject->AddBezier(
				D2D1::BezierSegment(
				D2D1::Point2F(408, 182),
				D2D1::Point2F(416, 184),
				D2D1::Point2F(422, 178)
				));
			pGeometrySinkObject->AddBezier(
				D2D1::BezierSegment(
				D2D1::Point2F(428, 171),
				D2D1::Point2F(435, 173),
				D2D1::Point2F(435, 173)
				));
			pGeometrySinkObject->EndFigure(D2D1_FIGURE_END_OPEN);
		}
		hr = pGeometrySinkObject->Close();

		SafeRelease(&pGeometrySinkObject);
	}

	hr = m_pD2DFactory->CreatePathGeometry(&pRiverGeometry);

	if (SUCCEEDED(hr))
	{
		ID2D1GeometrySink *pGeometrySinkObject = NULL;

		hr = pRiverGeometry->Open(&pGeometrySinkObject);
		if (SUCCEEDED(hr))
		{
			pGeometrySinkObject->SetFillMode(D2D1_FILL_MODE_WINDING);
			pGeometrySinkObject->BeginFigure(
				D2D1::Point2F(183, 392),
				D2D1_FIGURE_BEGIN_FILLED
				);
			pGeometrySinkObject->AddBezier(
				D2D1::BezierSegment(
				D2D1::Point2F(238, 284),
				D2D1::Point2F(472, 345),
				D2D1::Point2F(356, 303)
				));
			pGeometrySinkObject->AddBezier(
				D2D1::BezierSegment(
				D2D1::Point2F(237, 261),
				D2D1::Point2F(333, 256),
				D2D1::Point2F(333, 256)
				));
			pGeometrySinkObject->AddBezier(
				D2D1::BezierSegment(
				D2D1::Point2F(335, 257),
				D2D1::Point2F(241, 261),
				D2D1::Point2F(411, 306)
				));
			pGeometrySinkObject->AddBezier(
				D2D1::BezierSegment(
				D2D1::Point2F(574, 350),
				D2D1::Point2F(288, 324),
				D2D1::Point2F(296, 392)
				));
			pGeometrySinkObject->EndFigure(D2D1_FIGURE_END_OPEN);
		}
	}
	*ppBackGroundBrush = pSolidColorBrushBackground;
	*ppSolidColorBrush = pSolidColorBrush;
	*ppLeftMountainGeometryObject = pLeftMountainGeometry;
	*ppRightMountainGeometryObject = pRightMountainGeometry;
	*ppRiverGeometryObject = pRiverGeometry;
	*ppSunGeometryObject = pSunGeometry;
	return hr;
}

HRESULT Direct2DManagementClass::RenderPathGeometryOnThePlane(HINSTANCE hInstance,
	HWND hWnd,
	ID2D1SolidColorBrush* pSolidColorBrushBackGround,
	ID2D1SolidColorBrush* pSolidColorBrush,
	ID2D1PathGeometry* pLeftMountainGeometryObject,
	ID2D1PathGeometry* pRightMountainGeometryObject,
	ID2D1PathGeometry* pSunGeometryObject,
	ID2D1PathGeometry* pRiverGeometryObject)
{

	HRESULT hr = S_OK;


	ID2D1SolidColorBrush* pBlackColorBrush = nullptr;
	ID2D1SolidColorBrush* pRedColorBrush = nullptr;
	ID2D1SolidColorBrush* pGreenColorBrush = nullptr;

	std::cout << "Creating Black Brush" << endl;
	m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackColorBrush);

	std::cout << "Creating Green Brush" << endl;
	m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenColorBrush);

	std::cout << "Creating Red Brush Brush" << endl;
	m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedColorBrush);



	if (BeginPaint(hWnd, &ps))
	{
		std::cout << "First We Will Have to Generate our coordinate plane" << endl;

		D2D1_SIZE_F pSizeValue = m_pD2DDeviceContext1->GetSize();

		m_pD2DDeviceContext1->BeginDraw();
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));

	

		HRESULT hr = S_OK;
		BYTE* selectedAreaData = NULL;
		PAINTSTRUCT ps;

		IWICBitmapLock* pLockObject = nullptr;
		RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
		GetClientRect(hWnd, &rc);
		UINT pWidth = rc.right - rc.left;
		UINT pHeight = rc.bottom - rc.top;
		ID2D1BitmapBrush* pBitmapBrush = nullptr;
		D2D1_RECT_F clippedArea = D2D1::RectF(rc.left, rc.top, rc.right, rc.bottom);
		int clippedAreaWidth = pWidth;
		int clippedAreaHeight = pHeight;


		std::cout << "First We Will Have to Generate our coordinate plane" << endl;

		pSizeValue = m_pD2DDeviceContext1->GetSize();
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));


		m_pD2DDeviceContext1->SetTransform(D2D1::Matrix3x2F::Identity());

		D2D1_SIZE_F pRenderTargetSize = m_pD2DDeviceContext1->GetSize();
		m_pD2DDeviceContext1->FillRectangle(D2D1::RectF(0.0f, 0.0f, pRenderTargetSize.width, pRenderTargetSize.height),
			pSolidColorBrushBackGround);
		pSolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Orange));
		m_pD2DDeviceContext1->FillGeometry(pSunGeometryObject, pRedColorBrush);


		pSolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black, 1.f));
		m_pD2DDeviceContext1->DrawGeometry(pSunGeometryObject, pBlackColorBrush, 1.f);

		pSolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::OliveDrab, 1.f));
		m_pD2DDeviceContext1->FillGeometry(pLeftMountainGeometryObject, pRedColorBrush);


		pSolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black, 1.f));
		m_pD2DDeviceContext1->DrawGeometry(pLeftMountainGeometryObject, pBlackColorBrush, 1.f);

		pSolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::LightSkyBlue, 1.f));
		m_pD2DDeviceContext1->FillGeometry(pRiverGeometryObject, pRedColorBrush);

		pSolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black, 1.f));
		m_pD2DDeviceContext1->DrawGeometry(pRiverGeometryObject, pBlackColorBrush, 1.f);

		pSolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::YellowGreen, 1.f));
		m_pD2DDeviceContext1->FillGeometry(pRightMountainGeometryObject, pRedColorBrush);

		pSolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black, 1.f));
		m_pD2DDeviceContext1->DrawGeometry(pRightMountainGeometryObject, pBlackColorBrush, 1.0f);


	

		hr = m_pD2DDeviceContext1->EndDraw();


		if (hr == D2DERR_RECREATE_TARGET)
		{
			SafeReleaseA(&m_pD2DBitmap);
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}
		pSwapChainObjectD2D->Present(0, 0);




	}
	EndPaint(hWnd, &ps);
	return hr;
}



HRESULT Direct2DManagementClass::CreateBitmapFromMemory(HINSTANCE hInstance, HWND hWnd,
	UINT pImageWidth,
	UINT pImageHeight,
	BYTE* pImageDataFile,
	LPCWSTR pImageToWriteFile,
	vector<PixelObject> *ppPixelObjectOutput)
{
	ofstream myfile("imagebuffer.txt");
	myfile << "Please write this text to a file.\n this text is written using C++\n";
	HRESULT hr = S_OK;
	hr = CreateDeviceResources(hInstance, hWnd);
	hr = CreateWindowSizeDependentResources(hInstance, hWnd);
	UINT cbBufferSize = 0;

	WCHAR szFileName[MAX_PATH];
	BYTE* pixelArrayPointer;
	vector <PixelObject> pixelDataVector = vector <PixelObject>();

	vector<PixelObject> pixelDataArray = vector<PixelObject>();
	IWICBitmapDecoder* pBitmapDecoder = nullptr;
	IWICBitmapFrameDecode* pBitmapDecodeFrame = nullptr;
	IWICStream* pImageReadingStream = nullptr;
	IWICBitmapSource* pSource = nullptr;
	IWICBitmapSource* pSecondarySource = nullptr;
	IWICImagingFactory* pImagingFactory = nullptr;
	IWICBitmapFrameDecode *pFrame = nullptr;
	IWICFormatConverter* pConvertedSourceBitmap = nullptr;
	IWICStream* pByteStream = nullptr;

	IWICBitmap* pSecondaryBMP = nullptr;
	ID2D1Bitmap* pBitmapObject = nullptr;
	IWICBitmap* pBitmapObj = nullptr;
	double dpiX, dpiY;

	

		UINT cbStride = pImageWidth * sizeof(float) * 4;
		 cbBufferSize = cbStride;

			float *pixels = new float[cbBufferSize / sizeof(float)];


		double pWidth = pImageWidth;
			double pHeight = pImageHeight;
			UINT bSize = 0;

			UINT rowPitch1 = 4 * pWidth;
			UINT stride = 0; // stride  = bytes per row					 // (width * (bpp/8)) + padding and round to the nearest DWORD
			stride = (((((UINT)pWidth * 32) + 31) & ~31) >> 3);
			bSize = pHeight * pWidth;
			HDC hDC;

			hDC = GetDC(hWnd);

			HBITMAP map = CreateBitmap((UINT)pWidth,// width. 512 in my case
				(UINT)pHeight, // height
				1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
				8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
				(void*)pImageDataFile); // pointer to array
			// Temp HDC to copy picture
			HDC src = CreateCompatibleDC(hDC); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
			SelectObject(src, map); // Inserting picture into our temp HDC
			// Copy image from temp HDC to window




			std::cout << "Pixels Begin" << endl;

			int pixelCounter = 0;


			//	pBitmapObj->GetResolution(&pWidth, &pHeight);

			std::cout << "We will select a rect area with size 20x20 " << endl;
			for (UINT pXVal = 0; pXVal < pWidth; ++pXVal)
			{
				for (UINT pYVal = 0; pYVal < pHeight; ++pYVal)
				{
					COLORREF pCurrentPixel = GetPixel(src, pXVal, pYVal);
					int pRedValue = GetRValue(pCurrentPixel);
					int pGreenValue = GetGValue(pCurrentPixel);
					int pBlueValue = GetBValue(pCurrentPixel);
					PixelObject pixelObj = PixelObject(pixelCounter, pXVal, pYVal, pRedValue, pGreenValue, pBlueValue);
					pixelDataVector.push_back(pixelObj);
					std::cout << "Amount of total pixels processed  = " << pixelCounter << endl;
					pixelCounter++;
				}

			}

			std::cout << "The size of pixel buffer = " << pixelDataVector.size() << endl;
			int pAmountOfPixelsLeft = 0;
			pAmountOfPixelsLeft = pixelDataVector.size();


			fstream pixelDataFile;
			pixelDataFile.open("C:\\pixels\\pixels5.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);

			pixelDataFile.clear();
			pixelDataFile << " " << endl;
			pixelDataFile << " " << endl;
			pixelDataFile << "New Operation Begin" << endl;
			time_t currentTime = time(0);
			pixelDataFile << "current time" << currentTime << endl;
			for (PixelObject currentPixel : pixelDataVector)
			{



				std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
					"X Position = " << currentPixel.xPosition << " " <<
					"Y Position = " << currentPixel.yPosition << " " <<
					"The Value Of Red = " << currentPixel.RMask << " "
					<< "The Value Of Green = " << currentPixel.GMask << " " <<
					"The Value Of Blue = " << currentPixel.BMask << endl;

				pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
				pixelDataFile << "X Position = " << currentPixel.xPosition << endl;
				pixelDataFile << "Y Position = " << currentPixel.yPosition << endl;
				pixelDataFile << "The Value Of Red = " << currentPixel.RMask << endl;
				pixelDataFile << "The Value Of Green = " << currentPixel.GMask << endl;
				pixelDataFile << "The Value Of Blue = " << currentPixel.BMask << endl;

				pAmountOfPixelsLeft--;

			}
			pixelDataFile << "New Operation End" << endl;
			pixelDataFile << " " << endl;
			pixelDataFile << " " << endl;
			pixelDataFile.close();
			std::cout << "Pixels END" << endl;


			IWICBitmapEncoder* pBitmapEncoder = nullptr;
			IWICStream* pFileStream = nullptr;
			IWICBitmapFrameEncode* pBitmapFrameEncode = nullptr;
			IPropertyBag2* pPropertyBag = nullptr;
		
			std::cout << "Creating A WIC Bitmap Source from HBitmap" << endl;
			 pBitmapObj = nullptr;

			hr = m_pWICFactory->CreateBitmapFromHBITMAP(map, 
				NULL, WICBitmapIgnoreAlpha,
				&pBitmapObj);

			IWICBitmapSource* pSourceObject = nullptr;
			pBitmapObj->QueryInterface(&pSourceObject);


			if (SUCCEEDED(hr))
			{
				SafeReleaseA(&m_pConvertedSourceBitmap);
				hr = m_pWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
			}

			if (SUCCEEDED(hr))
			{
				hr = m_pConvertedSourceBitmap->Initialize(
					pBitmapObj,
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone,
					NULL,
					0.f,
					WICBitmapPaletteTypeCustom);

			}
			if (SUCCEEDED(hr))
			{
				hr = CreateDeviceResources(hInstance, hWnd);
			}
			
			D2D1_RECT_F rectangle = D2D1::RectF(0, 0, pImageWidth, pImageHeight);


			ID2D1Bitmap1* pBitmapObjectVar = nullptr;
			if (SUCCEEDED(hr))
			{
				m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(pBitmapObj, &pBitmapObjectVar);
			}

			if (SUCCEEDED(hr))
			{
				hr = DrawD2DContentAxisAlignedClip(hInstance, hWnd, pBitmapObjectVar, rectangle);
			}


			hr = SaveScreenDataToImage(hInstance,
				hWnd,
				pImageWidth,
				pImageHeight,
				pSourceObject,
				pImageToWriteFile);
		
			*ppPixelObjectOutput = pixelDataVector;

			return hr;
}



HRESULT Direct2DManagementClass::ClearRoadSignImageComparison(HINSTANCE hInstance, HWND hWnd)
{

	HRESULT hr = S_OK;
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;

	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
	D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
	int clippedAreaWidth = clippedArea.right - clippedArea.left;
	int clippedAreaHeight = clippedArea.bottom - clippedArea.top;

	ID2D1SolidColorBrush* pGreenBrush = nullptr;
	ID2D1SolidColorBrush* pRedBrush = nullptr;
	ID2D1SolidColorBrush* pYellowBrush = nullptr;
	ID2D1SolidColorBrush* pBlackBrush = nullptr;
	ID2D1SolidColorBrush* pWhiteBrush = nullptr;


	vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();
	static const WCHAR pText0[] = L"Image State No Change";
	static const WCHAR pText1[] = L"Unknown State ";
	static const WCHAR pText2[] = L"Image State Changed ";
	static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
	static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
	static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;


	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);


	HDC hDC = BeginPaint(hWnd, &ps);
	if (hDC)
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}



		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &pYellowBrush);



		D2D1_POINT_2F centre = { pWidth / 2.0f, pHeight / 2.0f };
		D2D1_POINT_2F centreLeft = { pWidth / 2.0f - 320, pHeight / 2.0f };
		D2D1_POINT_2F centreRight = { pWidth / 2.0f + 320, pHeight / 2.0f };

		D2D1_POINT_2F centre0 = { pWidth / 2.0f, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreLeft0 = { pWidth / 2.0f - 320, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreRight0 = { pWidth / 2.0f + 320, pHeight / 2.0f - 20 };

		D2D1_ELLIPSE ellipse = { centre, 150, 150 };
		D2D1_ELLIPSE ellipse0 = { centreLeft, 150, 150 };
		D2D1_ELLIPSE ellipse1 = { centreRight, 150, 150 };
		pEllipseArray.push_back(ellipse);
		pEllipseArray.push_back(ellipse0);
		pEllipseArray.push_back(ellipse1);
		m_pD2DDeviceContext1->BeginDraw();
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));


		// draw the ellipses
		m_pD2DDeviceContext1->DrawEllipse(ellipse, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse0, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse1, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);

		IDWriteTextFormat* pTextFormat = nullptr;
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			DWRITE_FONT_STYLE_ITALIC,
			DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
			24,
			L"",
			&pTextFormat);

		std::cout << "Setting Text Alignment Object" << endl;
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		std::cout << "Setting Text Layout Information 0" << endl;
		IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText0,
			stringLength0,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation0
			);

		std::cout << "Setting Text Layout Information 0" << endl;
		pTextLayoutInformation0->SetMaxHeight(40);
		pTextLayoutInformation0->SetMaxWidth(40);
		pTextLayoutInformation0->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation0->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		IDWriteTextLayout* pTextLayoutInformation1 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText1,
			stringLength1,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation1
			);
		pTextLayoutInformation1->SetMaxHeight(40);
		pTextLayoutInformation1->SetMaxWidth(40);
		pTextLayoutInformation1->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation1->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		std::cout << "Setting Text Layout Information 1" << endl;
		IDWriteTextLayout* pTextLayoutInformation2 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText2,
			stringLength2,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation2
			);
		pTextLayoutInformation2->SetMaxHeight(40);
		pTextLayoutInformation2->SetMaxWidth(40);
		pTextLayoutInformation2->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation2->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);



		std::cout << "Filling The Relevant Circle With Color" << endl;


		D2D1_RECT_F rectangle0 = D2D1::RectF(centreLeft.x, centreLeft.y, 10, 10);
		D2D1_RECT_F rectangle = D2D1::RectF(centre.x, centre.y, 10, 10);
		D2D1_RECT_F rectangle1 = D2D1::RectF(centreRight.x, centreRight.y, 10, 10);

		m_pD2DDeviceContext1->DrawTextLayout(
			centreLeft,
			pTextLayoutInformation0,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);


		m_pD2DDeviceContext1->DrawTextLayout(
			centre,
			pTextLayoutInformation1,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);


		m_pD2DDeviceContext1->DrawTextLayout(
			centreRight,
			pTextLayoutInformation2,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);




		D2D1_SIZE_F renderTargetSize = m_pD2DDeviceContext1->GetSize();

		m_pD2DDeviceContext1->EndDraw();


		// Retrieve D2D Device.



		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}






		pSwapChainObjectD2D->Present(0, 0);
		EndPaint(hWnd, &ps);


	}
	return SUCCEEDED(hr) ? 0 : 1;
}




HRESULT Direct2DManagementClass::ClearRoadSignImageComparisonPercents(HINSTANCE hInstance, HWND hWnd)
{

	HRESULT hr = S_OK;
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;

	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
	D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
	int clippedAreaWidth = clippedArea.right - clippedArea.left;
	int clippedAreaHeight = clippedArea.bottom - clippedArea.top;

	ID2D1SolidColorBrush* pGreenBrush = nullptr;
	ID2D1SolidColorBrush* pRedBrush = nullptr;
	ID2D1SolidColorBrush* pYellowBrush = nullptr;
	ID2D1SolidColorBrush* pBlackBrush = nullptr;
	ID2D1SolidColorBrush* pWhiteBrush = nullptr;


	vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();
	static const WCHAR pText0[] = L"33 Percents";
	static const WCHAR pText1[] = L"66 Percents ";
	static const WCHAR pText2[] = L"100 Percents ";
	static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
	static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
	static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;


	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);


	HDC hDC = BeginPaint(hWnd, &ps);
	if (hDC)
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}



		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &pYellowBrush);



		D2D1_POINT_2F centre = { pWidth / 2.0f, pHeight / 2.0f };
		D2D1_POINT_2F centreLeft = { pWidth / 2.0f - 320, pHeight / 2.0f };
		D2D1_POINT_2F centreRight = { pWidth / 2.0f + 320, pHeight / 2.0f };

		D2D1_POINT_2F centre0 = { pWidth / 2.0f, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreLeft0 = { pWidth / 2.0f - 320, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreRight0 = { pWidth / 2.0f + 320, pHeight / 2.0f - 20 };

		D2D1_ELLIPSE ellipse = { centre, 150, 150 };
		D2D1_ELLIPSE ellipse0 = { centreLeft, 150, 150 };
		D2D1_ELLIPSE ellipse1 = { centreRight, 150, 150 };
		pEllipseArray.push_back(ellipse);
		pEllipseArray.push_back(ellipse0);
		pEllipseArray.push_back(ellipse1);
		m_pD2DDeviceContext1->BeginDraw();
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));


		// draw the ellipses
		m_pD2DDeviceContext1->DrawEllipse(ellipse, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse0, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse1, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);

		IDWriteTextFormat* pTextFormat = nullptr;
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			DWRITE_FONT_STYLE_ITALIC,
			DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
			24,
			L"",
			&pTextFormat);

		std::cout << "Setting Text Alignment Object" << endl;
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		std::cout << "Setting Text Layout Information 0" << endl;
		IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText0,
			stringLength0,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation0
			);

		std::cout << "Setting Text Layout Information 0" << endl;
		pTextLayoutInformation0->SetMaxHeight(40);
		pTextLayoutInformation0->SetMaxWidth(40);
		pTextLayoutInformation0->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation0->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		IDWriteTextLayout* pTextLayoutInformation1 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText1,
			stringLength1,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation1
			);
		pTextLayoutInformation1->SetMaxHeight(40);
		pTextLayoutInformation1->SetMaxWidth(40);
		pTextLayoutInformation1->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation1->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		std::cout << "Setting Text Layout Information 1" << endl;
		IDWriteTextLayout* pTextLayoutInformation2 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText2,
			stringLength2,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation2
			);
		pTextLayoutInformation2->SetMaxHeight(40);
		pTextLayoutInformation2->SetMaxWidth(40);
		pTextLayoutInformation2->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation2->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);



		std::cout << "Filling The Relevant Circle With Color" << endl;


		D2D1_RECT_F rectangle0 = D2D1::RectF(centreLeft.x, centreLeft.y, 10, 10);
		D2D1_RECT_F rectangle = D2D1::RectF(centre.x, centre.y, 10, 10);
		D2D1_RECT_F rectangle1 = D2D1::RectF(centreRight.x, centreRight.y, 10, 10);

		m_pD2DDeviceContext1->DrawTextLayout(
			centreLeft,
			pTextLayoutInformation0,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);


		m_pD2DDeviceContext1->DrawTextLayout(
			centre,
			pTextLayoutInformation1,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);


		m_pD2DDeviceContext1->DrawTextLayout(
			centreRight,
			pTextLayoutInformation2,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);




		D2D1_SIZE_F renderTargetSize = m_pD2DDeviceContext1->GetSize();

		m_pD2DDeviceContext1->EndDraw();


		// Retrieve D2D Device.



		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}






		pSwapChainObjectD2D->Present(0, 0);
		EndPaint(hWnd, &ps);


	}
	return SUCCEEDED(hr) ? 0 : 1;
}

HRESULT Direct2DManagementClass::ClearRoadSignFSM(HINSTANCE hInstance, HWND hWnd)
{

	HRESULT hr = S_OK;
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;

	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
	D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
	int clippedAreaWidth = clippedArea.right - clippedArea.left;
	int clippedAreaHeight = clippedArea.bottom - clippedArea.top;

	ID2D1SolidColorBrush* pGreenBrush = nullptr;
	ID2D1SolidColorBrush* pRedBrush = nullptr;
	ID2D1SolidColorBrush* pYellowBrush = nullptr;
	ID2D1SolidColorBrush* pBlackBrush = nullptr;
	ID2D1SolidColorBrush* pWhiteBrush = nullptr;


	vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();
	static const WCHAR pText0[] = L"Image State No Change";
	static const WCHAR pText1[] = L"Unknown State ";
	static const WCHAR pText2[] = L"Image State Changed ";
	static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
	static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
	static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;


	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);


	HDC hDC = BeginPaint(hWnd, &ps);
	if (hDC)
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}



		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &pYellowBrush);



		D2D1_POINT_2F centre = { pWidth / 2.0f, pHeight / 2.0f };
		D2D1_POINT_2F centreLeft = { pWidth / 2.0f - 320, pHeight / 2.0f };
		D2D1_POINT_2F centreRight = { pWidth / 2.0f + 320, pHeight / 2.0f };

		D2D1_POINT_2F centre0 = { pWidth / 2.0f, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreLeft0 = { pWidth / 2.0f - 320, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreRight0 = { pWidth / 2.0f + 320, pHeight / 2.0f - 20 };

		D2D1_ELLIPSE ellipse = { centre, 150, 150 };
		D2D1_ELLIPSE ellipse0 = { centreLeft, 150, 150 };
		D2D1_ELLIPSE ellipse1 = { centreRight, 150, 150 };
		pEllipseArray.push_back(ellipse);
		pEllipseArray.push_back(ellipse0);
		pEllipseArray.push_back(ellipse1);
		m_pD2DDeviceContext1->BeginDraw();
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));


		// draw the ellipses
		m_pD2DDeviceContext1->DrawEllipse(ellipse, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse0, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse1, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);

		IDWriteTextFormat* pTextFormat = nullptr;
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			DWRITE_FONT_STYLE_ITALIC,
			DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
			24,
			L"",
			&pTextFormat);

		std::cout << "Setting Text Alignment Object" << endl;
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		std::cout << "Setting Text Layout Information 0" << endl;
		IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText0,
			stringLength0,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation0
			);

		std::cout << "Setting Text Layout Information 0" << endl;
		pTextLayoutInformation0->SetMaxHeight(40);
		pTextLayoutInformation0->SetMaxWidth(40);
		pTextLayoutInformation0->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation0->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		IDWriteTextLayout* pTextLayoutInformation1 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText1,
			stringLength1,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation1
			);
		pTextLayoutInformation1->SetMaxHeight(40);
		pTextLayoutInformation1->SetMaxWidth(40);
		pTextLayoutInformation1->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation1->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		std::cout << "Setting Text Layout Information 1" << endl;
		IDWriteTextLayout* pTextLayoutInformation2 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText2,
			stringLength2,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation2
			);
		pTextLayoutInformation2->SetMaxHeight(40);
		pTextLayoutInformation2->SetMaxWidth(40);
		pTextLayoutInformation2->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation2->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);



		std::cout << "Filling The Relevant Circle With Color" << endl;


		D2D1_RECT_F rectangle0 = D2D1::RectF(centreLeft.x, centreLeft.y, 10, 10);
		D2D1_RECT_F rectangle = D2D1::RectF(centre.x, centre.y, 10, 10);
		D2D1_RECT_F rectangle1 = D2D1::RectF(centreRight.x, centreRight.y, 10, 10);

		m_pD2DDeviceContext1->DrawTextLayout(
			centreLeft,
			pTextLayoutInformation0,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);


		m_pD2DDeviceContext1->DrawTextLayout(
			centre,
			pTextLayoutInformation1,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);


		m_pD2DDeviceContext1->DrawTextLayout(
			centreRight,
			pTextLayoutInformation2,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);




		D2D1_SIZE_F renderTargetSize = m_pD2DDeviceContext1->GetSize();

		m_pD2DDeviceContext1->EndDraw();


		// Retrieve D2D Device.



		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}






		pSwapChainObjectD2D->Present(0, 0);
		EndPaint(hWnd, &ps);


	}
	return SUCCEEDED(hr) ? 0 : 1;
}



HRESULT Direct2DManagementClass::ClearRoadSignIntensityDifference(HINSTANCE hInstance, HWND hWnd)
{

	HRESULT hr = S_OK;
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;

	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
	D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
	int clippedAreaWidth = clippedArea.right - clippedArea.left;
	int clippedAreaHeight = clippedArea.bottom - clippedArea.top;

	ID2D1SolidColorBrush* pGreenBrush = nullptr;
	ID2D1SolidColorBrush* pRedBrush = nullptr;
	ID2D1SolidColorBrush* pYellowBrush = nullptr;
	ID2D1SolidColorBrush* pBlackBrush = nullptr;
	ID2D1SolidColorBrush* pWhiteBrush = nullptr;


	vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();
	static const WCHAR pText0[] = L"<= 85";
	static const WCHAR pText1[] = L" > 85 && <= 170 ";
	static const WCHAR pText2[] = L"> 170 && <= 255  ";
	static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
	static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
	static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;


	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);


	HDC hDC = BeginPaint(hWnd, &ps);
	if (hDC)
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}



		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &pYellowBrush);



		D2D1_POINT_2F centre = { pWidth / 2.0f, pHeight / 2.0f };
		D2D1_POINT_2F centreLeft = { pWidth / 2.0f - 320, pHeight / 2.0f };
		D2D1_POINT_2F centreRight = { pWidth / 2.0f + 320, pHeight / 2.0f };

		D2D1_POINT_2F centre0 = { pWidth / 2.0f, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreLeft0 = { pWidth / 2.0f - 320, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreRight0 = { pWidth / 2.0f + 320, pHeight / 2.0f - 20 };

		D2D1_ELLIPSE ellipse = { centre, 150, 150 };
		D2D1_ELLIPSE ellipse0 = { centreLeft, 150, 150 };
		D2D1_ELLIPSE ellipse1 = { centreRight, 150, 150 };
		pEllipseArray.push_back(ellipse);
		pEllipseArray.push_back(ellipse0);
		pEllipseArray.push_back(ellipse1);
		m_pD2DDeviceContext1->BeginDraw();
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));


		// draw the ellipses
		m_pD2DDeviceContext1->DrawEllipse(ellipse, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse0, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse1, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);

		IDWriteTextFormat* pTextFormat = nullptr;
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			DWRITE_FONT_STYLE_ITALIC,
			DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
			24,
			L"",
			&pTextFormat);

		std::cout << "Setting Text Alignment Object" << endl;
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		std::cout << "Setting Text Layout Information 0" << endl;
		IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText0,
			stringLength0,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation0
			);

		std::cout << "Setting Text Layout Information 0" << endl;
		pTextLayoutInformation0->SetMaxHeight(40);
		pTextLayoutInformation0->SetMaxWidth(40);
		pTextLayoutInformation0->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation0->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		IDWriteTextLayout* pTextLayoutInformation1 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText1,
			stringLength1,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation1
			);
		pTextLayoutInformation1->SetMaxHeight(40);
		pTextLayoutInformation1->SetMaxWidth(40);
		pTextLayoutInformation1->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation1->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		std::cout << "Setting Text Layout Information 1" << endl;
		IDWriteTextLayout* pTextLayoutInformation2 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText2,
			stringLength2,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation2
			);
		pTextLayoutInformation2->SetMaxHeight(40);
		pTextLayoutInformation2->SetMaxWidth(40);
		pTextLayoutInformation2->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation2->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);



		std::cout << "Filling The Relevant Circle With Color" << endl;


		D2D1_RECT_F rectangle0 = D2D1::RectF(centreLeft.x, centreLeft.y, 10, 10);
		D2D1_RECT_F rectangle = D2D1::RectF(centre.x, centre.y, 10, 10);
		D2D1_RECT_F rectangle1 = D2D1::RectF(centreRight.x, centreRight.y, 10, 10);

		m_pD2DDeviceContext1->DrawTextLayout(
			centreLeft,
			pTextLayoutInformation0,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);


		m_pD2DDeviceContext1->DrawTextLayout(
			centre,
			pTextLayoutInformation1,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);


		m_pD2DDeviceContext1->DrawTextLayout(
			centreRight,
			pTextLayoutInformation2,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);




		D2D1_SIZE_F renderTargetSize = m_pD2DDeviceContext1->GetSize();

		m_pD2DDeviceContext1->EndDraw();


		// Retrieve D2D Device.



		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}






		pSwapChainObjectD2D->Present(0, 0);
		EndPaint(hWnd, &ps);


	}
	return SUCCEEDED(hr) ? 0 : 1;
}

HRESULT Direct2DManagementClass::DrawRoadSignStateMachine(HINSTANCE hInstance, HWND hWnd, UINT pOutputIntensity)
{

	HRESULT hr = S_OK;
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;

	
	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
	D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
	int clippedAreaWidth = clippedArea.right - clippedArea.left;
	int clippedAreaHeight = clippedArea.bottom - clippedArea.top;

	ID2D1SolidColorBrush* pGreenBrush = nullptr;
	ID2D1SolidColorBrush* pRedBrush = nullptr;
	ID2D1SolidColorBrush* pYellowBrush = nullptr;
	ID2D1SolidColorBrush* pBlackBrush = nullptr;
	ID2D1SolidColorBrush* pWhiteBrush = nullptr;

	vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();
	static const WCHAR pText0[] = L"Identical Pixel";
	static const WCHAR pText1[] = L"Undefined Pixel";
	static const WCHAR pText2[] = L"Different Pixel";
	static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
	static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
	static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;


	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);
	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenBrush);
	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedBrush);
	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteBrush);
	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &pYellowBrush);



	D2D1_POINT_2F centre = { pWidth / 2.0f, pHeight / 2.0f };
	D2D1_POINT_2F centreLeft = { pWidth / 2.0f - 320, pHeight / 2.0f };
	D2D1_POINT_2F centreRight = { pWidth / 2.0f + 320, pHeight / 2.0f };

	D2D1_POINT_2F centre0 = { pWidth / 2.0f, pHeight / 2.0f - 20 };
	D2D1_POINT_2F centreLeft0 = { pWidth / 2.0f - 320, pHeight / 2.0f - 20 };
	D2D1_POINT_2F centreRight0 = { pWidth / 2.0f + 320, pHeight / 2.0f - 20 };

	D2D1_ELLIPSE ellipse = D2D1::Ellipse(centre, 130, 130);
	D2D1_ELLIPSE ellipse0 = D2D1::Ellipse(centreLeft, 130, 130);
	D2D1_ELLIPSE ellipse1 = D2D1::Ellipse(centreRight, 130, 130);
	pEllipseArray.push_back(ellipse);
	pEllipseArray.push_back(ellipse0);
	pEllipseArray.push_back(ellipse1);
	m_pD2DDeviceContext1->BeginDraw();

	// draw the ellipses
	m_pD2DDeviceContext1->DrawEllipse(ellipse, pBlackBrush, 25);
	m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
	m_pD2DDeviceContext1->DrawEllipse(ellipse0, pBlackBrush, 25);
	m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);
	m_pD2DDeviceContext1->DrawEllipse(ellipse1, pBlackBrush, 25);
	m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);

	IDWriteTextFormat* pTextFormat = nullptr;
	hr = m_pDWriteFactory->CreateTextFormat(
		L"Times New Roman",
		NULL,
		DWRITE_FONT_WEIGHT_EXTRA_BOLD,
		DWRITE_FONT_STYLE_ITALIC,
		DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
		24,
		L"",
		&pTextFormat);

	std::cout << "Setting Text Alignment Object" << endl;
	pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);


	std::cout << "Setting Text Layout Information 0" << endl;
	IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
	IDWriteTextLayout* pTextLayoutInformation1 = nullptr;

	hr = m_pDWriteFactory->CreateTextLayout(
		pText0,
		stringLength0,
		pTextFormat,
		20, // maxWidth
		20, // maxHeight
		&pTextLayoutInformation0
		);


	hr = m_pDWriteFactory->CreateTextLayout(
		pText0,
		stringLength0,
		pTextFormat,
		20, // maxWidth
		20, // maxHeight
		&pTextLayoutInformation1
		);


	std::cout << "Setting Text Layout Information 1" << endl;
	IDWriteTextLayout* pTextLayoutInformation2 = nullptr;
	hr = m_pDWriteFactory->CreateTextLayout(
		pText1,
		stringLength1,
		pTextFormat,
		20, // maxWidth
		20, // maxHeight
		&pTextLayoutInformation2
		);


	std::cout << "Filling The Relevant Circle With Color" << endl;

	if (pOutputIntensity <= 33)
	{
		m_pD2DDeviceContext1->FillEllipse(ellipse0, pGreenBrush);
		m_pD2DDeviceContext1->FillEllipse(ellipse, pBlackBrush);
		m_pD2DDeviceContext1->FillEllipse(ellipse1, pBlackBrush);
	}

	if (pOutputIntensity > 33 && pOutputIntensity <= 66)
	{
		m_pD2DDeviceContext1->FillEllipse(ellipse, pYellowBrush);
		m_pD2DDeviceContext1->FillEllipse(ellipse, pBlackBrush);
		m_pD2DDeviceContext1->FillEllipse(ellipse1, pBlackBrush);
	}

	else if (pOutputIntensity > 66 && pOutputIntensity <= 255)
	{
		m_pD2DDeviceContext1->FillEllipse(ellipse1, pRedBrush);
		m_pD2DDeviceContext1->FillEllipse(ellipse0, pBlackBrush);
		m_pD2DDeviceContext1->FillEllipse(ellipse, pBlackBrush);
	}



	D2D1_RECT_F rectangle0 = D2D1::RectF(centreLeft.x, centreLeft.y, 10, 10);
	D2D1_RECT_F rectangle = D2D1::RectF(centre.x, centre.y, 10, 10);
	D2D1_RECT_F rectangle1 = D2D1::RectF(centreRight.x, centreRight.y, 10, 10);

	m_pD2DDeviceContext1->DrawTextLayout(
		centreLeft,
		pTextLayoutInformation0,
		pBlackBrush,
		D2D1_DRAW_TEXT_OPTIONS_NONE);


	m_pD2DDeviceContext1->DrawTextLayout(
		centre,
		pTextLayoutInformation1,
		pBlackBrush,
		D2D1_DRAW_TEXT_OPTIONS_NONE);


	m_pD2DDeviceContext1->DrawTextLayout(
		centreRight,
		pTextLayoutInformation2,
		pBlackBrush,
		D2D1_DRAW_TEXT_OPTIONS_NONE);




	D2D1_SIZE_F renderTargetSize = m_pD2DDeviceContext1->GetSize();

	m_pD2DDeviceContext1->EndDraw();


	// Retrieve D2D Device.



	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
	}






	pSwapChainObjectD2D->Present(0, 0);

	EndPaint(hWnd, &ps);
	return hr;
}



void Direct2DManagementClass::ClearRenderTargetContent(HINSTANCE hInstance, HWND hWnd)
{

	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);
	try
	{

	
			m_pD2DDeviceContext1->BeginDraw();
			m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));
			m_pD2DDeviceContext1->EndDraw();
			pSwapChainObjectD2D->Present(0, 0);

	}
	catch (std::exception ex)
	{
		throw ex;
	}
	EndPaint(hWnd, &ps);
}


	

void Direct2DManagementClass::DiscardDeviceResources()
{
	if (m_pD2DBitmap)
	{
		SafeReleaseA(&m_pD2DBitmap);
	}

	if (m_pPathGeometry)
	{
		SafeReleaseA(&m_pPathGeometry);
	}

	if (m_pD2DDeviceContext1)
	{
		SafeReleaseA(&m_pD2DDeviceContext1);
	}
		
	if (m_pDWriteFactory)
	{
		SafeReleaseA(&m_pDWriteFactory);
	}
	
	if (m_pWICFactory)
	{
		SafeReleaseA(&m_pWICFactory);
	}
		
	if (m_pD2DFactory)
	{
		SafeReleaseA(&m_pD2DFactory);
	}
	
}



HRESULT Direct2DManagementClass::CreateChildWindow(HINSTANCE hInstance, HWND hWnd)
{
	int thumbWidth = 48;
	int thumbHeight = 32;

	HRESULT hr = S_OK;

	WNDCLASSEXW wcex;
	wcex.cbSize         = sizeof(wcex);
	wcex.style          = 0;
	wcex.lpfnWndProc    = ChildWindowProcedure;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = HINST_THISCOMPONENT;
	wcex.hIcon          = NULL;
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH) GetStockObject (GRAY_BRUSH);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = L"DCompLayeredChildWindow";
	wcex.hIconSm        = NULL;

	if (!RegisterClassExW(&wcex))
	{
		return FALSE;
	}

	childWindow = CreateWindowEx(WS_EX_LAYERED,
		L"DCompLayeredChildWindow",                
		NULL,                                    
		WS_CHILD | WS_CLIPSIBLINGS,             
		0, 
		0, 
		static_cast<UINT>(ceil(640.0f * m_dpiX / 96.0f)),
		static_cast<UINT>(ceil(480.0f * m_dpiY / 96.0f)),                                
		0,                               
		NULL,                                     
		HINST_THISCOMPONENT,                    
		this);                                   

	hr = childWindow ? S_OK : E_FAIL;
	
	if (SUCCEEDED(hr))
	{
		// Reduce the window to thumbsize.
		MoveWindow(childWindow , m_childOffsetX, m_childOffsetY, 
			thumbWidth, thumbHeight, TRUE);
		ShowWindow(childWindow, SW_SHOWNORMAL);
		UpdateWindow(childWindow);
	}

	return hr;


}

HRESULT Direct2DManagementClass::InitializeDirect2DEngine(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr = S_OK;
	hr = CreateDeviceIndependentResources(hInstance, hWnd);
	if (SUCCEEDED(hr))
	{
		hr = CreateDeviceResources(hInstance, hWnd);
	}
    
	return hr;
}

HRESULT Direct2DManagementClass::CreateDeviceIndependentResources(HINSTANCE hInstance, HWND hWnd)
{
	
	static const WCHAR msc_fontname[] = L"Times New Roman";
	static const FLOAT msc_fontsize = 120;
	
	HRESULT hr = S_OK;
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	m_pWICFactory = nullptr;


	ID2D1GeometrySink *pSink = NULL;
	D2D1_FACTORY_OPTIONS options;
	ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));
	IWICImagingFactory* pFactoryObject = nullptr;
#if defined(_DEBUG)
	// If the project is in a debug build, enable Direct2D debugging via SDK Layers.
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

	

	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pFactoryObject));

	m_pWICFactory = pFactoryObject;
	
	
	
	if (SUCCEEDED(hr))
	{
		if (DISABLE_ALL_MESSAGE_BOXES == FALSE)
		{
			MessageBoxA(NULL, (LPCSTR)"Cоздан объект WIC Factory", (LPCSTR)"Message", 0);

		}
	
	
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_pD2DFactory);
		if (SUCCEEDED(hr))
		{
			if (DISABLE_ALL_MESSAGE_BOXES == FALSE)
			{
				MessageBoxA(NULL, (LPCSTR)"Cоздан объект Direct2D Factory", (LPCSTR)"Message", 0);
			}
		}
		if (FAILED(hr))
		{
			if (DISABLE_ALL_MESSAGE_BOXES == FALSE)
			{
				MessageBoxA(NULL, (LPCSTR)"Не удалось создать объект Direct2D Factory", (LPCSTR)"Message", 0);
			}
		}


		cout << "Launching DirectWrite Library" << endl;

		IDWriteFontCollectionLoader* pFontCollectionLoader = nullptr;

		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
		    __uuidof(IDWriteFactory2),
			reinterpret_cast<IUnknown **>(&pFactoryObjectDWrite));

		cout << "Creating Font Collection Loader" << endl;

	

		pFactoryObjectDWrite->RegisterFontCollectionLoader(pFontCollectionLoader);
		
		if (SUCCEEDED(hr))
		{
			if (DISABLE_ALL_MESSAGE_BOXES == FALSE)
			{
				MessageBoxA(NULL, (LPCSTR)"Cоздан объект DirectWrite Factory", (LPCSTR)"Message", 0);
			}
			hr = pFactoryObjectDWrite->CreateTextFormat(
						msc_fontname,
						NULL,
						DWRITE_FONT_WEIGHT_EXTRA_BOLD,
						DWRITE_FONT_STYLE_ITALIC,
						DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
						msc_fontsize,
						L"",
						&m_pTextFormat);
			if (FAILED(hr))
			{
				if (DISABLE_ALL_MESSAGE_BOXES == FALSE)
				{
					MessageBoxA(NULL, (LPCSTR)"Не удалось создать объект Текстовый Формат", (LPCSTR)"Message", 0);
				}
			}
			if (SUCCEEDED(hr))
			{
				if (DISABLE_ALL_MESSAGE_BOXES == FALSE)
				{
					MessageBoxA(NULL, (LPCSTR)"Создан Текстовый Формат", (LPCSTR)"Message", 0);
				}
				m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
				m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
			}
		}
		if (FAILED(hr))
		{
		//	MessageBoxA(NULL, (LPCSTR)"Не удалось создать объект DirectWrite Factory", (LPCSTR)"Message", 0);
		}

		
		cout << "Creating A Font Collector Loader" << endl;
		pFontCollectionLoader = nullptr;

		hr = pFactoryObjectDWrite->RegisterFontCollectionLoader(pFontCollectionLoader);



		D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(
			DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_IGNORE),
			0,
			0,
			D2D1_RENDER_TARGET_USAGE_NONE,
			D2D1_FEATURE_LEVEL_DEFAULT
			);


	}
	


	m_pDWriteFactory = pFactoryObjectDWrite;



	return hr;
}

void Direct2DManagementClass::DestroyBitmapGrid(HWND hWnd)
{
	m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::Azure));
}

void Direct2DManagementClass::CreateBitmapGrid(HWND hWnd)
{
	m_pD2DDeviceContext->BeginDraw();
	m_pD2DDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::White));
	D2D1_SIZE_F rtSize = m_pD2DDeviceContext->GetSize();
	
	int width = static_cast<int>(rtSize.width);
	int height = static_cast<int>(rtSize.height);

	m_pD2DDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pBlueBrush);

	for (int x = 0; x < width; x += 10)
	{
		m_pD2DDeviceContext->DrawLine(
					D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
					D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
					m_pBlueBrush,
					0.5f
					);
	}

	for (int y = 0; y < height; y += 10)
	{
		m_pD2DDeviceContext->DrawLine(
			D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
			D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
			m_pBlueBrush,
			0.5f);
	}
	m_pD2DDeviceContext->EndDraw();
}


HRESULT Direct2DManagementClass::DrawDefaultText(HWND hWnd)
{
	HRESULT hr;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	hr = CreateDeviceResources(hInstance, hWnd);

	if (SUCCEEDED(hr))
	{
		
		static const WCHAR sc_WelcomeToRenderer[] = L"Multimedia System Digital Universe";
		D2D1_SIZE_F renderTargetSize = m_pD2DDeviceContext1->GetSize();

		m_pD2DDeviceContext1->BeginDraw();
		m_pD2DDeviceContext1->SetTransform(D2D1::Matrix3x2F::Identity());
	
		m_pD2DDeviceContext1->DrawText(
			sc_WelcomeToRenderer,
			ARRAYSIZE(sc_WelcomeToRenderer) - 1,
			m_pTextFormat,
			D2D1::RectF(0, 700, renderTargetSize.width, renderTargetSize.height),
			m_pRedBrush);


		m_pD2DDeviceContext1->DrawText(
			sc_WelcomeToRenderer,
			ARRAYSIZE(sc_WelcomeToRenderer) - 1,
			m_pTextFormat,
			D2D1::RectF(0, 350, renderTargetSize.width, renderTargetSize.height),
			m_pBlueBrush);


		m_pD2DDeviceContext1->DrawText(
			sc_WelcomeToRenderer,
			ARRAYSIZE(sc_WelcomeToRenderer) - 1,
			m_pTextFormat,
			D2D1::RectF(0, 0, renderTargetSize.width, renderTargetSize.height),
			m_pWhiteBrush);



	
		hr = m_pD2DDeviceContext1->EndDraw();

		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = S_OK;
			DiscardDeviceResources();
		}
	}
	return hr;
}

void Direct2DManagementClass::RenderImageToRenderTarget(HWND hWnd, ID2D1Bitmap* currentBitmap, int renderTargetIndex)
{
	HRESULT hr;
	PAINTSTRUCT ps;
	HINSTANCE hInstance = GetModuleHandle(NULL);

	if (BeginPaint(hWnd, &ps))
	{
		hr = CreateDeviceResources(hInstance, hWnd);

		if (SUCCEEDED(hr))
		{

			m_pD2DBitmapRenderTargerCollection[renderTargetIndex]->BeginDraw();

			m_pD2DBitmapRenderTargerCollection[renderTargetIndex]->SetTransform(D2D1::Matrix3x2F::Identity());
			m_pD2DBitmapRenderTargerCollection[renderTargetIndex]->Clear(D2D1::ColorF(D2D1::ColorF::Black));

			D2D1_SIZE_F rtSize = m_pD2DDeviceContext1->GetSize();
			D2D1_RECT_F rectangle = D2D1::RectF(0, 0, rtSize.width / 12, rtSize.height/ 2);

			

			if (m_pD2DBitmap)
			{
				m_pD2DBitmapRenderTargerCollection[renderTargetIndex]->DrawBitmap(m_pD2DBitmap, rectangle);
			}
			hr = m_pD2DBitmapRenderTargerCollection[renderTargetIndex]->EndDraw();

			if (hr == D2DERR_RECREATE_TARGET)
			{
				SafeReleaseA(&currentBitmap);
				SafeReleaseA(&m_pD2DBitmapRenderTargerCollection[renderTargetIndex]);
				hr = InvalidateRect(hWnd, NULL, TRUE)? S_OK : E_FAIL;
			}
		}
		EndPaint(hWnd, &ps);
	}
	
}


HRESULT Direct2DManagementClass::CreateImageEncoder(HINSTANCE hInstance, HWND hWnd, LPCWSTR outputFilePath)
{
	HRESULT hr = S_OK;
	IWICBitmapEncoder* pEncoderVar = nullptr;
	GUID containerFormat = GUID_ContainerFormatJpeg;
	std::wstring pFileName = std::wstring(outputFilePath);
	std::wstring fileExtension;
	// Convert all characters to lower case
	std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), tolower);

	// Default value is bitmap encoding
	if (fileExtension.compare(L".jpg") == 0 ||
		fileExtension.compare(L".jpeg") == 0 ||
		fileExtension.compare(L".jpe") == 0 ||
		fileExtension.compare(L".jfif") == 0)
	{
		containerFormat = GUID_ContainerFormatJpeg;
	}
	else if (fileExtension.compare(L".tif") == 0 ||
		fileExtension.compare(L".tiff") == 0)
	{
		containerFormat = GUID_ContainerFormatTiff;
	}
	else
	{
		containerFormat = GUID_ContainerFormatPng;
	}


	// Other code omitted

	m_pWICFactory->CreateEncoder(containerFormat, nullptr, &pEncoderVar);
	return hr;
}


HRESULT Direct2DManagementClass::CreateImageEncoder(HINSTANCE hInstance, HWND hWnd, LPCWSTR outputFilePath, IWICBitmapEncoder** ppOutputEncoder)
{
	HRESULT hr = S_OK;
	IWICBitmapEncoder* pEncoderVar = nullptr;
	GUID containerFormat = GUID_ContainerFormatJpeg;
	std::wstring pFileName = std::wstring(outputFilePath);
	std::wstring fileExtension;
	// Convert all characters to lower case
	std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), tolower);

	// Default value is bitmap encoding
	if (fileExtension.compare(L".jpg") == 0 ||
		fileExtension.compare(L".jpeg") == 0 ||
		fileExtension.compare(L".jpe") == 0 ||
		fileExtension.compare(L".jfif") == 0)
	{
		containerFormat = GUID_ContainerFormatJpeg;
	}
	else if (fileExtension.compare(L".tif") == 0 ||
		fileExtension.compare(L".tiff") == 0)
	{
		containerFormat = GUID_ContainerFormatTiff;
	}
	else
	{
		containerFormat = GUID_ContainerFormatPng;
	}


	// Other code omitted

	m_pWICFactory->CreateEncoder(containerFormat, nullptr, &pEncoderVar);
	*ppOutputEncoder = pEncoderVar;
	return hr;
}


HRESULT Direct2DManagementClass::SaveScreenDataToImage(HINSTANCE hInstance, HWND hWnd,
	UINT pImageWidth,
	UINT pImageHeight,
	IWICBitmapSource* pSourceObject,
	LPCTSTR lpszFileName)
{
	HRESULT hr = S_OK;

	std::cout << "Creating a Stream ToSave Image File" << endl;
	WICRect pWicRect = { 0.0f, 0.0f, pImageWidth, pImageHeight };


	IWICStream*  pStreamToWriteFile = nullptr;
	IWICBitmapEncoder* pEncoderObbject = nullptr;
	IWICBitmapFrameEncode* pEncoderObbjectFrame = nullptr;


	if (m_pWICFactory != nullptr)
	{
		hr = m_pWICFactory->CreateStream(&pStreamToWriteFile);
	}

	std::cout << "Selecting Image File Formato To Save Data" << endl;
	WICPixelFormatGUID format = GUID_WICPixelFormat32bppPBGRA;

	if (SUCCEEDED(hr))
	{
		hr = pStreamToWriteFile->InitializeFromFilename(lpszFileName, GENERIC_WRITE);
	}
	else
	{
		return hr;
	}

	std::cout << "Creating A Png Encoder object" << endl;
	
	CreateImageEncoder(hInstance, hWnd, lpszFileName, &pEncoderObbject);

	if (SUCCEEDED(hr))
	{
		hr = pEncoderObbject->Initialize(pStreamToWriteFile, WICBitmapEncoderNoCache);
	}

	IPropertyBag2* pPropertyBag = nullptr;


	if (SUCCEEDED(hr))
	{
		hr = pEncoderObbject->CreateNewFrame(&pEncoderObbjectFrame, &pPropertyBag);
	}


	if (SUCCEEDED(hr))
	{
		
		hr = pEncoderObbjectFrame->Initialize(pPropertyBag);
	}



	if (SUCCEEDED(hr))
	{
		pEncoderObbjectFrame->SetSize(pImageWidth, pImageHeight);
	}


	format = GUID_WICPixelFormatDontCare; 
	PROPBAG2 option0 = { 0 };
    option0.pstrName = L"ImageQuality";
    VARIANT varValue;    
    VariantInit(&varValue);
	varValue.vt = VT_R4;
	varValue.bVal = 1;
	
	hr = pPropertyBag->Write(1, &option0, &varValue);

	PROPBAG2 option1 = { 0 };
	option1.pstrName = L"BitmapTransform";
	VARIANT varValue1;
	VariantInit(&varValue1);
	varValue1.vt = VT_UI1;
	varValue1.bVal = WICBitmapTransformFlipHorizontal;


	hr = pPropertyBag->Write(2, &option1, &varValue1);

	PROPBAG2 option2 = { 0 };
	option2.pstrName = L"Luminance";
	VARIANT varValue2;
	VariantInit(&varValue2);
	varValue2.vt = VT_UI4;
	varValue2.bVal = 64;

	hr = pPropertyBag->Write(3, &option2, &varValue2);

	PROPBAG2 option3 = { 0 };
	option3.pstrName = L"Chrominance";
	VARIANT varValue3;
	VariantInit(&varValue3);
	varValue3.vt = VT_R4;
	varValue3.bVal = 1;

	hr = pPropertyBag->Write(4, &option3, &varValue3);

	PROPBAG2 option4 = { 0 };
	option4.pstrName = L"JpegYCBCRSubsampling";
	VARIANT varValue4;
	VariantInit(&varValue4);
	varValue4.vt = VT_UI4;
	varValue4.bVal = 1;

	hr = pPropertyBag->Write(5, &option4, &varValue4);

	PROPBAG2 option5 = { 0 };
	option5.pstrName = L"SuppressApp0";
	VARIANT varValue5;
	VariantInit(&varValue5);
	varValue5.vt = VT_R4;
	varValue5.bVal = 1;

	hr = pPropertyBag->Write(6, &option5, &varValue5);
	hr = pEncoderObbjectFrame->Initialize(pPropertyBag);	
	pEncoderObbjectFrame->SetPixelFormat(&format);
	

	
	hr = pEncoderObbjectFrame->WriteSource(pSourceObject, &pWicRect);
	

	if (SUCCEEDED(hr))
	{
		pEncoderObbjectFrame->Commit();
	}
	if (SUCCEEDED(hr))
	{
		pEncoderObbject->Commit();
	}



	SafeRelease(&pEncoderObbject);
	SafeRelease(&pEncoderObbjectFrame);
	SafeRelease(&pStreamToWriteFile);

	return hr;


}





HRESULT Direct2DManagementClass::CreateDeviceResourcesW(HWND hWnd)
{
	HRESULT hr = S_OK;


	return hr;

}




HRESULT Direct2DManagementClass::CreateD2DBitmapAndRenderToArea(HWND hWnd, int renderTargetIndex)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	HRESULT hr = S_OK;
	WCHAR szFileName[MAX_PATH];

	if (LocateImageFile(hWnd, szFileName, ARRAYSIZE(szFileName)))
	{
		IWICBitmapDecoder *pDecoder = NULL;
		hr = m_pWICFactory->CreateDecoderFromFilename(
			szFileName,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&pImageDecoder);

		IWICBitmapFrameDecode *pFrame = NULL;

		if (SUCCEEDED(hr))
		{
			hr = pImageDecoder->GetFrame(0, &pFrame);
		}

		if (SUCCEEDED(hr))
		{
			SafeReleaseA(&m_pConvertedSourceBitmap);
			hr = m_pWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pConvertedSourceBitmap->Initialize(
				pFrame,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeCustom);
		}


	
		if (SUCCEEDED(hr))
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}
		if (SUCCEEDED(hr))
		{
		
			SafeReleaseA(&m_pD2DBitmap);
			hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);
		}
		if (SUCCEEDED(hr))
		{
			try
			{
				RenderImageToRenderTarget(hWnd, m_pD2DBitmap, renderTargetIndex);
			}
			catch (std::exception ex)
			{
				throw ex;
			}

		
	}
	
}
	
	return hr;
}

HRESULT Direct2DManagementClass::CreateBitmapDrawingResources(HWND hWnd)
{
	HRESULT hr = S_OK;

	if (!m_pD2DDeviceContext1)
	{
		RECT rc;
		hr = GetClientRect(hWnd, &rc) ? S_OK : E_FAIL;

		if (SUCCEEDED(hr))
		{
			D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties = D2D1::RenderTargetProperties();

			renderTargetProperties.dpiX = DEFAULT_DPI;
			renderTargetProperties.dpiY = DEFAULT_DPI;

			D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);


		//	hr = m_pD2DFactory->CreateDxgiSurfaceRenderTarget(ppBackBufferSurface,
			//	&renderTargetProperties, &m_pD2DDeviceContext1);
	
			
			

			if (SUCCEEDED(hr))
			{
				hr = m_pD2DDeviceContext1->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF::Blue, 1.0f),
					&m_pBlueBrush
					);

				hr = m_pD2DDeviceContext1->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF::Red, 1.0f),
					&m_pRedBrush);

				hr = m_pD2DDeviceContext1->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF::Gray, 1.0f),
					&m_pWhiteBrush);
			}




			m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));

			for (UINT iIndex = 0; iIndex < m_pD2DDCRenderTargetCollection.size(); ++iIndex)
			{
				hr = m_pD2DFactory->CreateDCRenderTarget(&renderTargetProperties,
					&m_pD2DDCRenderTargetCollection[iIndex]);
			}


			for (UINT iIndex = 0; iIndex < m_pD2DBitmapCollection.size(); ++iIndex)
			{
				IWICBitmapSource* pCurrentSouce;
				m_pWICFactory->CreateBitmap(DEFAULT_DPI / 12,
					DEFAULT_DPI / 2,
					GUID_WICPixelFormat4bppIndexed,
					WICBitmapCacheOnDemand,
					&m_pD2DBitmapCollection[iIndex]);

				D2D1_PIXEL_FORMAT format =
					D2D1::PixelFormat(
					DXGI_FORMAT_B8G8R8A8_UNORM,
					D2D1_ALPHA_MODE_PREMULTIPLIED);



			


			}

		}
	}

	return hr;

}


HRESULT Direct2DManagementClass::CreateWicBitmapFromSelectedFile(HINSTANCE hInstance, HWND hWnd, IWICFormatConverter** pConvertedBitmap, ID2D1Bitmap** pD2D1Bitmap)
{
	HRESULT hr = S_OK;
	WCHAR szFileName[MAX_PATH];

	if (LocateImageFile(hWnd, szFileName, ARRAYSIZE(szFileName)))
	{
		IWICBitmapDecoder *pDecoder = NULL;
		hr = m_pWICFactory->CreateDecoderFromFilename(
			szFileName,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&pImageDecoder);

		IWICBitmapFrameDecode *pFrame = NULL;

		if (SUCCEEDED(hr))
		{
			hr = pImageDecoder->GetFrame(0, &pFrame);
		}

		if (SUCCEEDED(hr))
		{
			SafeReleaseA(&m_pConvertedSourceBitmap);
			hr = m_pWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pConvertedSourceBitmap->Initialize(
				pFrame,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeCustom);
		}
		if (SUCCEEDED(hr))
		{
			if (m_pD2DDeviceContext1 == nullptr)
			{
				hr = CreateDeviceResources(hInstance, hWnd);
			}
		}
		if (SUCCEEDED(hr))
		{
	
			SafeReleaseA(&m_pD2DBitmap);
			hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);
		}

		if (SUCCEEDED(hr))
		{
			*pConvertedBitmap = m_pConvertedSourceBitmap;
			*pD2D1Bitmap = m_pD2DBitmap;
		}
		
	}
	return hr;
}



HRESULT Direct2DManagementClass::CreateD2DBitmapFromFileW(HWND hWnd, WCHAR* szFileName)
{

	ofstream myfile("imagebuffer.txt");
	myfile << "Please write this text to a file.\n this text is written using C++\n";
	vector<PixelObject> pixelObjectCollection = vector<PixelObject>();
	HRESULT hr = S_OK;
	UINT cbBufferSize = 0;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	IWICImagingFactory* pImagingFactory = nullptr;
	BYTE* pixelDataArray = nullptr;
	WICRect rcLock = { 0.0f, 0.0f, 40.0f, 40.0f };



	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pImagingFactory));

		

		
		hr = pImagingFactory->CreateDecoderFromFilename(
			szFileName,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&pImageDecoder);

		IWICBitmapFrameDecode *pFrame = NULL;

		if (SUCCEEDED(hr))
		{
			hr = pImageDecoder->GetFrame(0, &pFrame);
		}

		if (SUCCEEDED(hr))
		{


			hr = m_pWICFactory->CreateBitmapFromSource(pFrame,
				WICBitmapCacheOnDemand,
				&pBitmapObject);
			IWICPalette* pImagePalette = nullptr;
			pBitmapObject->CopyPalette(pImagePalette);
		}
		if (SUCCEEDED(hr))
		{
			hr = pBitmapObject->Lock(&rcLock, WICBitmapLockWrite, &pBitmapLock);
		}



		if (SUCCEEDED(hr))
		{
			hr = pBitmapLock->GetDataPointer(&cbBufferSize, &pixelDataArray);
		}

		UINT pixelIndex = 0;
		for (UINT pIndex = 0; pIndex <= sizeof(pixelDataArray); ++pIndex)
		{
			float pixelX = (float)pixelDataArray[pIndex];
			float pixelY = (float)pixelDataArray[pIndex + 1];
			float pixelR = (float)pixelDataArray[pIndex + 2];
			float pixelG = (float)pixelDataArray[pIndex + 2];
			float pixelB = (float)pixelDataArray[pIndex + 2];

			PixelObject pNewPixelObject = PixelObject(pIndex, pixelX,
				pixelY,
				pixelR,
				pixelG,
				pixelB);

			std::cout << "Pixel Id = " << pNewPixelObject.pixelNumber << endl;
			std::cout << "Pixel X = " << pNewPixelObject.xPosition << endl;
			std::cout << "Pixel Y = " << pNewPixelObject.yPosition << endl;
			std::cout << "Pixel R = " << pNewPixelObject.RMask << endl;
			std::cout << "Pixel G = " << pNewPixelObject.GMask << endl;
			std::cout << "Pixel B = " << pNewPixelObject.BMask << endl;
			pixelObjectCollection.push_back(pNewPixelObject);

		}




		if (SUCCEEDED(hr))
		{
			IWICMetadataQueryReader* pQueryReader = nullptr;
			IWICColorContext* pColorContext = nullptr;
			WICColorContextType pType;
			UINT actualColorContexts = 0;

			hr = pFrame->GetMetadataQueryReader(&pQueryReader);
			if (SUCCEEDED(hr))
			{
				std::cout << "We have created a metadata Reader for image" << endl;
				MessageBoxA(hWnd, (LPCSTR)"Cоздан Объект чтения метаданных для изображения", (LPCSTR)"Message", 0);

			}
			if (SUCCEEDED(hr))
			{
				hr = m_pWICFactory->CreateColorContext(&pColorContext);
				std::cout << "We have created a color context object" << endl;
				MessageBoxA(hWnd, (LPCSTR)"Создан контекст цветов изображения", (LPCSTR)"Message", 0);
			}
			if (SUCCEEDED(hr))
			{
				hr = pFrame->GetColorContexts(1, &pColorContext, &actualColorContexts);
				pColorContext->GetType(&pType);
			}
			switch (pType)
			{
			case WICColorContextUninitialized:
			{
				MessageBoxA(NULL, (LPCSTR)"Контекст цветов для вашего изображения не инициализирован", (LPCSTR)"Message", 0);
				std::cout << "The color context for selected image cannot be initialized" << endl;
				std::ifstream file(szFileName, ios::binary);
				// Prepare iterator pairs to iterate the file content!
			

		
				

				// Outputs to example.txt through a_fil
				// Close the file stream explicitly

				break;
			}
			default:
				break;
			}



			SafeReleaseA(&m_pConvertedSourceBitmap);
			hr = m_pWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pConvertedSourceBitmap->Initialize(
				pFrame,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeCustom);

		}
		if (SUCCEEDED(hr))
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}
		if (SUCCEEDED(hr))
		{
			SafeReleaseA(&m_pD2DBitmap);
			hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);
		}
		if (SUCCEEDED(hr))
		{
			hr = DrawD2DContent(hInstance, hWnd, m_pD2DBitmap);
		}

		SafeReleaseA(&pDecoder);
		SafeReleaseA(&pFrame);
		myfile.close();
		return hr;
	}
	
	HRESULT Direct2DManagementClass::OpenAndDrawImageFile(HINSTANCE hInstance, HWND hWnd)
	{
		HRESULT hr = S_OK;
		ID2D1Bitmap* pD2D1BitmapObject = nullptr;
		hr = CreateD2DBitmapFromFileD2D(hInstance, hWnd, &pD2D1BitmapObject);
		return hr;
	}

	HRESULT Direct2DManagementClass::CreateD2DBitmapFromFileD2D(HINSTANCE hInstance, HWND hWnd, ID2D1Bitmap** ppBitmapObject)
	{
		ofstream myfile("imagebuffer.txt");
		myfile << "Please write this text to a file.\n this text is written using C++\n";
		HRESULT hr = S_OK;
		CreateWindowSizeDependentResources(hInstance, hWnd);
		UINT cbBufferSize = 0;

		WCHAR szFileName[MAX_PATH];
		BYTE* pixelArrayPointer;

		vector<PixelObject> pixelDataArray = vector<PixelObject>();
		IWICStream* pImageReadingStream = nullptr;
		IWICBitmapSource* pSource = nullptr;
		IWICBitmapSource* pSecondarySource = nullptr;
		IWICImagingFactory* pImagingFactory = nullptr;
		IWICBitmapFrameDecode *pFrame = nullptr;
		IWICFormatConverter* pConvertedSourceBitmap = nullptr;

		ID2D1Bitmap* pBitmapObject = nullptr;
		IWICBitmap* pBitmapObj = nullptr;
		float pTargetHeight = 20.0f;
		float pTargetWidth = 20.0f;
		float xPositionStart = 0.0f;
		float yPositionStart = 0.0f;
		double dpiX, dpiY;
		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&pImagingFactory));


		if (LocateImageFile(hWnd, szFileName, ARRAYSIZE(szFileName)))
		{


			IWICBitmapDecoder *pDecoder = NULL;
			hr = pImagingFactory->CreateDecoderFromFilename(
				szFileName,
				NULL,
				GENERIC_READ,
				WICDecodeMetadataCacheOnDemand,
				&pDecoder);




			hr = pImagingFactory->CreateStream(&pImageReadingStream);

			if (SUCCEEDED(hr))
			{
				hr = pImageReadingStream->InitializeFromFilename(szFileName, GENERIC_READ);
			}

			UINT pFrameCount = 0;

			if (SUCCEEDED(hr))
			{
				pDecoder->GetFrameCount(&pFrameCount);
			}


			if (SUCCEEDED(hr) && pFrameCount > 0)
			{
				hr = pDecoder->GetFrame(0, &pFrame);

			}
			
			pSource = pFrame;
			pSecondarySource = pFrame;

			if (SUCCEEDED(hr))
			{


				hr = m_pWICFactory->CreateBitmapFromSource(pFrame,
					WICBitmapCacheOnDemand,
					&pBitmapObj);
				IWICPalette* pImagePalette = nullptr;
				pBitmapObj->CopyPalette(pImagePalette);
			}


			pBitmapObj->GetResolution(&dpiX, &dpiY);

			D2D1_SIZE_F pSize = m_pD2DDeviceContext1->GetSize();
			WICRect rc = { 0.0f, 0.0f, dpiX, dpiX };


			if (SUCCEEDED(hr))
			{
				hr = pBitmapObj->Lock(&rc, WICBitmapLockRead, &pBitmapLock);
			}



			UINT cbBufferSize = 0;
			UINT cbStride = 0;
			BYTE  *pv;

			hr = pBitmapLock->GetStride(&cbStride);

			if (SUCCEEDED(hr))
			{
				hr = pBitmapLock->GetDataPointer(&cbBufferSize, &pv);
			}

			pixelArrayPointer = &pv[0];


			vector<BYTE*> pixelVector = vector<BYTE*>();
			pixelVector.insert(pixelVector.end(), pixelArrayPointer);

			for (int pIndex = 0; pIndex < sizeof(pv); ++pIndex)

			{

				float pixelX = (float)pv[pIndex];
				float pixelY = (float)pv[pIndex + 1];
				float pixelR = (float)pv[pIndex + 2];
				float pixelG = (float)pv[pIndex + 3];
				float pixelB = (float)pv[pIndex + 4];

				PixelObject pNewPixelObject = PixelObject(pIndex, pixelX,
					pixelY,
					pixelR,
					pixelG,
					pixelB);

				std::cout << "Pixel Id = " << pNewPixelObject.pixelNumber <<
					"Pixel X = " << pNewPixelObject.xPosition <<
					"Pixel Y = " << pNewPixelObject.yPosition <<
					"Pixel R = " << pNewPixelObject.RMask <<
					"Pixel G = " << pNewPixelObject.GMask <<
					"Pixel B = " << pNewPixelObject.BMask << endl;
				pixelDataArray.push_back(pNewPixelObject);

			}


			if (SUCCEEDED(hr))
			{
				hr = pImagingFactory->CreateFormatConverter(&pConvertedSourceBitmap);
			}


			hr = pConvertedSourceBitmap->Initialize(
				pFrame,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeCustom);

		}

		if (SUCCEEDED(hr))
		{

			hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(pConvertedSourceBitmap, NULL, &pBitmapObject);
		}
		*ppBitmapObject = pBitmapObject;
		if (SUCCEEDED(hr))
		{
			hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidColorBrush);
		}

		if (SUCCEEDED(hr))
		{
			hr = DrawD2DContent(hInstance, hWnd, pBitmapObject);
		}

		UINT pImageWidth = 0;
		UINT pImageHeight = 0;
		IWICBitmapSource* pConverter = nullptr;
		WICPixelFormatGUID pixelFormat;

		if (SUCCEEDED(hr))
		{
			pSecondarySource = pFrame;
			pSecondarySource->AddRef();
			pSecondarySource->GetSize(&pImageWidth, &pImageHeight);
		}
		IWICPixelFormatInfo* pixelFormatInfo = nullptr;

		if (SUCCEEDED(hr))
		{
			pSecondarySource->GetPixelFormat(&pixelFormat);
		}



		if (!IsEqualGUID(pixelFormat, GUID_WICPixelFormat128bppPRGBAFloat))
		{

			hr = WICConvertBitmapSource(GUID_WICPixelFormat128bppPRGBAFloat, pSecondarySource, &pConverter);
			IWICBitmapLock* pBitmapLock = nullptr;


			if (SUCCEEDED(hr))
			{
				pSecondarySource->Release();     // the converter has a reference to the source
				pSecondarySource = NULL;         // so we don't need it anymore.
				pSecondarySource = pConverter;   // let's treat the 128bppPABGR converter as the source
			}


			IWICPalette* pImagePalette = nullptr;
			pSecondarySource->CopyPalette(pImagePalette);
		}
	



		CImageFileData* pImageClass = new CImageFileData((char*)pixelArrayPointer);


		if (pConverter)
		{
			UINT cbStride = pImageWidth * sizeof(float) * 4;
			UINT cbBufferSize = cbStride;

			float *pixels = new float[cbBufferSize / sizeof(float)];


			hr = pSource->GetSize(&pImageWidth, &pImageHeight);
			double pWidth = pImageWidth;
			double pHeight = pImageHeight;
			UINT bSize = 0;

			UINT rowPitch1 = 4 * pWidth;
			UINT stride = 0; // stride  = bytes per row					 // (width * (bpp/8)) + padding and round to the nearest DWORD
			stride = (((((UINT)pWidth * 32) + 31) & ~31) >> 3);
			bSize = pHeight * pWidth;
			BYTE *pixelArray = new BYTE[(UINT)pTargetHeight * stride];
			pSecondarySource->CopyPixels(NULL, stride, bSize, pixelArray);
			HDC hDC;

			hDC = GetDC(hWnd);

			HBITMAP map = CreateBitmap((UINT)pWidth,// width. 512 in my case
				(UINT)pHeight, // height
				1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
				8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
				(void*)pixelArray); // pointer to array
			// Temp HDC to copy picture
			HDC src = CreateCompatibleDC(hDC); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
			SelectObject(src, map); // Inserting picture into our temp HDC
			// Copy image from temp HDC to window




			std::cout << "Pixels Begin" << endl;
			vector <PixelObject> pixelDataVector = vector <PixelObject>();

			int pixelCounter = 0;


			//	pBitmapObj->GetResolution(&pWidth, &pHeight);

			std::cout << "We will select a rect area with size 20x20 " << endl;
			for (UINT pXVal = 0; pXVal <= pWidth - 1; ++pXVal)
			{
				for (UINT pYVal = 0; pYVal <= pHeight - 1; ++pYVal)
				{
					COLORREF pCurrentPixel = GetPixel(src, pXVal, pYVal);
					int pRedValue = GetRValue(pCurrentPixel);
					int pGreenValue = GetGValue(pCurrentPixel);
					int pBlueValue = GetBValue(pCurrentPixel);
					PixelObject pixelObj = PixelObject(pixelCounter, pXVal, pYVal, pRedValue, pGreenValue, pBlueValue);
					pixelDataVector.push_back(pixelObj);
					std::cout << "Amount of total pixels processed  = " << pixelCounter << endl;
					pixelCounter++;
				}

			}

			std::cout << "The size of pixel buffer = " << pixelDataVector.size() << endl;
			int pAmountOfPixelsLeft = 0;
			pAmountOfPixelsLeft = pixelDataVector.size();


			fstream pixelDataFile;
			pixelDataFile.open("C:\\pixels\\pixels5.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);

			pixelDataFile.clear();
			pixelDataFile << " " << endl;
			pixelDataFile << " " << endl;
			pixelDataFile << "New Operation Begin" << endl;
			time_t currentTime = time(0);
			pixelDataFile << "current time" << currentTime << endl;
			for (PixelObject currentPixel : pixelDataVector)
			{



				std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
					"X Position = " << currentPixel.xPosition << " " <<
					"Y Position = " << currentPixel.yPosition << " " <<
					"The Value Of Red = " << currentPixel.RMask << " "
					<< "The Value Of Green = " << currentPixel.GMask << " " <<
					"The Value Of Blue = " << currentPixel.BMask << endl;

				pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
				pixelDataFile << "X Position = " << currentPixel.xPosition  << endl;
				pixelDataFile << "Y Position = " << currentPixel.yPosition << endl;
				pixelDataFile << "The Value Of Red = " << currentPixel.RMask << endl;
				pixelDataFile << "The Value Of Green = " << currentPixel.GMask << endl;
				pixelDataFile << "The Value Of Blue = " << currentPixel.BMask << endl;

				pAmountOfPixelsLeft--;

			}
			pixelDataFile << "New Operation End" << endl;
			pixelDataFile << " " << endl;
			pixelDataFile << " " << endl;
			pixelDataFile.close();
			std::cout << "Pixels END" << endl;






			SafeReleaseA(&pDecoder);
			myfile.close();


		}
		return hr;
	}
















HRESULT Direct2DManagementClass::CreateBitmapFromArrayOfPixels(HINSTANCE hInstance, HWND hWnd, LPVOID& pixelArray, HBITMAP pImageBitmap, ID2D1Bitmap** ppOuputBitmap)
{
	HRESULT hr = S_OK;
	BITMAPINFO *pBitmapInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256);
	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	HBITMAP hBitmap;
	HDC hDC = GetDC(hWnd);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;
	ID2D1Bitmap* pProducedBitmap = nullptr;


	BITMAPINFOHEADER &biHeader(pBitmapInfo->bmiHeader);
	biHeader.biSize = sizeof(BITMAPINFOHEADER);
	biHeader.biWidth = width;
	biHeader.biHeight = height;

	biHeader.biPlanes = 1;
	biHeader.biBitCount = 8;
	biHeader.biCompression = BI_RGB;
	biHeader.biSizeImage = 0;
	biHeader.biXPelsPerMeter = 14173;
	biHeader.biYPelsPerMeter = 14173;
	biHeader.biClrUsed = 0;
	biHeader.biClrImportant = 0;

	for (int iIndex = 0; iIndex <= 255; iIndex++)
	{
		pBitmapInfo->bmiColors[iIndex].rgbBlue = pBitmapInfo->bmiColors[iIndex].rgbGreen = pBitmapInfo->bmiColors[iIndex].rgbRed = (BYTE)iIndex;
		pBitmapInfo->bmiColors[iIndex].rgbReserved = 0;
	}
	void* pixelData = NULL;
	hBitmap = CreateDIBSection(hDC, pBitmapInfo, DIB_RGB_COLORS, &pixelData, NULL, 0);

	if (pixelArray != NULL)
	{
		BYTE* pbBits = (BYTE*)pixelArray;
		BYTE* pixelArray = (BYTE*)pixelData;
		memcpy(pixelArray, pbBits, width + height);
	}
	D2D1_SIZE_U imageSize = D2D1::SizeU(width, height);
	D2D1_BITMAP_PROPERTIES props = D2D1::BitmapProperties();

	hr = m_pD2DDeviceContext->CreateBitmap(imageSize, props,
		&pProducedBitmap);
	if (SUCCEEDED(hr))
	{
		hr = DrawD2DContent(hInstance, hWnd, pProducedBitmap);
	}

	return hr;
}

HRESULT Direct2DManagementClass::CreateD2DBitmapClipObjectWithPixels(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pWidth, float pHeight, vector<PixelObject> *pixelObjectOutput)
{

	vector <PixelObject> pixelDataVector = vector <PixelObject>();
	ofstream myfile("imagebuffer.txt");
	myfile << "Please write this text to a file.\n this text is written using C++\n";
	BYTE* pixelArrayPointer;
	vector<PixelObject> pixelDataArray = vector<PixelObject>();
	IWICBitmapFrameDecode *pFrame = NULL;
	IWICFormatConverter* pConvertedSourceBitmap = nullptr;
	HRESULT hr = S_OK;
	WCHAR szFileName[MAX_PATH];
	UINT cbBufferSize = 0;
	WICRect pWicRect;
	double pDpiX = 0.0f;
	double pDpiY = 0.0;
	IWICBitmap* pBitmapObj = nullptr;



	UINT pImageWidth = pWidth;
	UINT pImageHeight = pHeight;

	pWicRect = { pXPosition, pYPosition, pImageWidth, pImageHeight };

	std::cout << "Locating the Initial Image" << endl;

	if (LocateImageFile(hWnd, szFileName, ARRAYSIZE(szFileName)))
	{


		IWICBitmapDecoder *pDecoder = NULL;
		hr = m_pWICFactory->CreateDecoderFromFilename(
			szFileName,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&pDecoder);




		hr = m_pWICFactory->CreateStream(&pImageReadingStream);

		if (SUCCEEDED(hr))
		{
			hr = pImageReadingStream->InitializeFromFilename(szFileName, GENERIC_READ);
		}

		UINT pFrameCount = 0;

		if (SUCCEEDED(hr))
		{
			pDecoder->GetFrameCount(&pFrameCount);
		}


		if (SUCCEEDED(hr) && pFrameCount > 0)
		{
			hr = pDecoder->GetFrame(0, &pFrame);

		}



		if (SUCCEEDED(hr))
		{


			IWICBitmap* pBitmapObject = nullptr;

			hr = m_pWICFactory->CreateBitmapFromSourceRect(
				pFrame,
				pXPosition,
				pYPosition,
				pWidth,
				pHeight,
				&pBitmapObject);



			if (SUCCEEDED(hr))
			{






				UINT cbStride = pImageWidth * sizeof(float) * 4;
				UINT cbBufferSize = cbStride;

				float *pixels = new float[cbBufferSize / sizeof(float)];

				UINT pRectWidth = 0;
				UINT pRectHeight = 0;
				pBitmapObject->GetSize(&pRectWidth, &pRectHeight);


				UINT bSize = 0;

				UINT rowPitch1 = 4 * pRectWidth;
				UINT stride = 0; // stride  = bytes per row					 // (width * (bpp/8)) + padding and round to the nearest DWORD
				stride = (((((UINT)pRectWidth * 32) + 31) & ~31) >> 3);
				bSize = pRectHeight * pRectWidth;
				BYTE *pixelArray = new BYTE[(UINT)pRectHeight * stride];
				
				pBitmapObject->CopyPixels(nullptr, stride, bSize, pixelArray);
				HDC hDC;

				hDC = GetDC(hWnd);

				HBITMAP map = CreateBitmap((UINT)pRectWidth,// width. 512 in my case
					(UINT)pRectHeight, // height
					1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
					8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
					(void*)pixelArray); // pointer to array
				// Temp HDC to copy picture
				HDC src = CreateCompatibleDC(hDC); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
				SelectObject(src, map); // Inserting picture into our temp HDC
				// Copy image from temp HDC to window




				std::cout << "Pixels Begin" << endl;

				int pixelCounter = 0;


				//	pBitmapObj->GetResolution(&pWidth, &pHeight);

				std::cout << "We will select a rect area with size 20x20 " << endl;
				for (UINT pXVal = pWicRect.X; pXVal <= pWicRect.X + pWicRect.Width; ++pXVal)
				{
					for (UINT pYVal = pWicRect.Y; pYVal <= pWicRect.Y + pWicRect.Height; ++pYVal)
					{
						COLORREF pCurrentPixel = GetPixel(hDC, pXVal, pYVal);
						int pRedValue = GetRValue(pCurrentPixel);
						int pGreenValue = GetGValue(pCurrentPixel);
						int pBlueValue = GetBValue(pCurrentPixel);
						PixelObject pixelObj = PixelObject(pixelCounter, pXVal, pYVal, pRedValue, pGreenValue, pBlueValue);
						pixelDataVector.push_back(pixelObj);

						std::cout << "Amount of total pixels processed  = " << pixelCounter << endl;
						pixelCounter++;
					}

				}

				std::cout << "The size of pixel buffer = " << pixelDataVector.size() << endl;
				int pAmountOfPixelsLeft = 0;
				pAmountOfPixelsLeft = pixelDataVector.size();


				fstream pixelDataFile;
				pixelDataFile.open("C:\\pixels\\pixels5.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);

				pixelDataFile.clear();
				pixelDataFile << " " << endl;
				pixelDataFile << " " << endl;
				pixelDataFile << "New Operation Begin" << endl;

				for (PixelObject currentPixel : pixelDataVector)
				{



					std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
						"X Position = " << currentPixel.xPosition << " " <<
						"Y Position = " << currentPixel.yPosition << " " <<
						"The Value Of Red = " << currentPixel.RMask << " "
						<< "The Value Of Green = " << currentPixel.GMask << " " <<
						"The Value Of Blue = " << currentPixel.BMask << endl;

					pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
					pixelDataFile << "X Position = " << currentPixel.xPosition << endl;
					pixelDataFile << "Y Position = " << currentPixel.yPosition << endl;
					pixelDataFile << "The Value Of Red = " << currentPixel.RMask << endl;
					pixelDataFile << "The Value Of Green = " << currentPixel.GMask << endl;
					pixelDataFile << "The Value Of Blue = " << currentPixel.BMask << endl;

					pAmountOfPixelsLeft--;

				}

		}










		if (SUCCEEDED(hr))
		{
			hr = m_pWICFactory->CreateFormatConverter(&pConvertedSourceBitmap);
		}


		hr = pConvertedSourceBitmap->Initialize(
			pBitmapObject,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeCustom);

	}

	if (SUCCEEDED(hr))
	{

		hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(pConvertedSourceBitmap, NULL, &m_pD2DBitmap);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidColorBrush);
	}

	D2D1_RECT_F pOutputRect = D2D1::RectF(0.0f, 0.0f, pImageWidth, pImageHeight);
	if (SUCCEEDED(hr))
	{
		hr = DrawD2DContent(hInstance, hWnd, m_pD2DBitmap); 
	}


	std::cout << "Now we will receive clipped pixel values" << endl;
	std::ifstream file(szFileName, ios::binary);
	// Prepare iterator pairs to iterate the file content!
	std::istream_iterator<unsigned char> begin(file), end;
	// Reading the file content using the iterator!
	std::vector<unsigned char> buffer(begin, end);

	myfile << buffer.data();

	SafeReleaseA(&pDecoder);




	*pixelObjectOutput = pixelDataVector;


	SafeReleaseA(&pDecoder);

	myfile.close();
	return hr;

		
	}

	
}

HRESULT Direct2DManagementClass::CreateD2DBitmapClipObject(HINSTANCE hInstance, HWND hWnd, float pXPosition0, float pYPosition0, float pWidth, float pHeight,
	vector<PixelObject> *pixelObjectOutput)
{

	
		ofstream myfile("imagebuffer.txt");
		myfile << "Please write this text to a file.\n this text is written using C++\n";
		BYTE* pixelArrayPointer;
		vector <PixelObject> pixelDataVector = vector <PixelObject>();

		IWICBitmapFrameDecode *pFrame = NULL;
		IWICFormatConverter* pConvertedSourceBitmap = nullptr;
		HRESULT hr = S_OK;
		WCHAR szFileName[MAX_PATH];
		UINT cbBufferSize = 0;
		WICRect pWicRect;
		double pDpiX = 0.0f;
		double pDpiY = 0.0;
		IWICBitmap* pBitmapObj = nullptr;

		

		UINT pImageWidth = pWidth;
		UINT pImageHeight = pHeight;

		pWicRect = { pXPosition0, pYPosition0, pImageWidth, pImageHeight };
		
		if (LocateImageFile(hWnd, szFileName, ARRAYSIZE(szFileName)))
		{


			IWICBitmapDecoder *pDecoder = NULL;
			hr = m_pWICFactory->CreateDecoderFromFilename(
				szFileName,
				NULL,
				GENERIC_READ,
				WICDecodeMetadataCacheOnDemand,
				&pDecoder);




			hr = m_pWICFactory->CreateStream(&pImageReadingStream);

			if (SUCCEEDED(hr))
			{
				hr = pImageReadingStream->InitializeFromFilename(szFileName, GENERIC_READ);
			}

			UINT pFrameCount = 0;

			if (SUCCEEDED(hr))
			{
				pDecoder->GetFrameCount(&pFrameCount);
			}


			if (SUCCEEDED(hr) && pFrameCount > 0)
			{
				hr = pDecoder->GetFrame(0, &pFrame);

			}



			if (SUCCEEDED(hr))
			{


				hr = m_pWICFactory->CreateBitmapFromSource(pFrame,
					WICBitmapCacheOnDemand,
					&pBitmapObj);
				IWICPalette* pImagePalette = nullptr;
				pBitmapObj->CopyPalette(pImagePalette);
			}


			pBitmapObj->GetResolution(&pDpiX, &pDpiY);

			pBitmapObj->GetSize(&pImageWidth, &pImageHeight);

		
			


			if (SUCCEEDED(hr))
			{
				hr = m_pWICFactory->CreateFormatConverter(&pConvertedSourceBitmap);
			}


				hr = pConvertedSourceBitmap->Initialize(
					pFrame,
							GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeCustom);

		}

		if (SUCCEEDED(hr))
		{

			hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(pConvertedSourceBitmap, NULL, &m_pD2DBitmap);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidColorBrush);
		}

		D2D1_RECT_F pOutputRect = D2D1::RectF(0.0f, 0.0f, pImageWidth, pImageHeight);
		if (SUCCEEDED(hr))
		{
			hr = DrawD2DContentAxisAlignedClip(hInstance, hWnd, m_pD2DBitmap, pOutputRect);
		}


			std::cout << "Now we will receive clipped pixel values" << endl;
			std::ifstream file(szFileName, ios::binary);
			// Prepare iterator pairs to iterate the file content!
			// Reading the file content using the iterator!
			SafeReleaseA(&pDecoder);
			hr = m_pWICFactory->CreateDecoderFromFilename(
				szFileName,
				NULL,
				GENERIC_READ,
				WICDecodeMetadataCacheOnDemand,
				&pDecoder);
	


			if (SUCCEEDED(hr))
			{
				hr = pDecoder->GetFrame(0, &pFrame);
			}

			IWICBitmapClipper* pBitmapClipper = nullptr;

			if (SUCCEEDED(hr))
			{
				hr = m_pWICFactory->CreateBitmapClipper(&pBitmapClipper);
			}

			

			if (SUCCEEDED(hr))
			{

				hr = pBitmapClipper->Initialize(pFrame, &pWicRect);

				

		



			}
			if (SUCCEEDED(hr))
			{

			




				UINT cbStride = pImageWidth * sizeof(float) * 4;
				UINT cbBufferSize = cbStride;

				float *pixels = new float[cbBufferSize / sizeof(float)];



				UINT bSize = 0;

				UINT rowPitch1 = 4 * pImageWidth;
				UINT stride = 0; // stride  = bytes per row					 // (width * (bpp/8)) + padding and round to the nearest DWORD
				stride = (((((UINT)pImageWidth * 32) + 31) & ~31) >> 3);
				bSize = pImageHeight * pImageWidth;
				BYTE *pixelArray = new BYTE[(UINT)pImageHeight * stride];
				pBitmapClipper->CopyPixels(&pWicRect, stride, bSize, pixelArray);
				HDC hDC;

				hDC = GetDC(hWnd);

				HBITMAP map = CreateBitmap((UINT)pImageWidth,// width. 512 in my case
					(UINT)pImageHeight, // height
					1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
					8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
					(void*)pixelArray); // pointer to array
				// Temp HDC to copy picture
				HDC src = CreateCompatibleDC(hDC); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
				SelectObject(src, map); // Inserting picture into our temp HDC
				// Copy image from temp HDC to window




				std::cout << "Pixels Begin" << endl;
				int pixelCounter = 0;


				//	pBitmapObj->GetResolution(&pWidth, &pHeight);

				std::cout << "We will select a rect area with size 20x20 " << endl;
				for (UINT pXVal = pWicRect.X; pXVal <= pWicRect.X + pWicRect.Width; ++pXVal)
				{
					for (UINT pYVal = pWicRect.Y; pYVal <= pWicRect.Y + pWicRect.Height; ++pYVal)
					{
						COLORREF pCurrentPixel = GetPixel(hDC, pXVal, pYVal);
						int pRedValue = GetRValue(pCurrentPixel);
						int pGreenValue = GetGValue(pCurrentPixel);
						int pBlueValue = GetBValue(pCurrentPixel);
						PixelObject pixelObj = PixelObject(pixelCounter, pXVal, pYVal, pRedValue, pGreenValue, pBlueValue);
						pixelDataVector.push_back(pixelObj);
						
						std::cout << "Amount of total pixels processed  = " << pixelCounter << endl;
						pixelCounter++;
					}

				}

				std::cout << "The size of pixel buffer = " << pixelDataVector.size() << endl;
				int pAmountOfPixelsLeft = 0;
				pAmountOfPixelsLeft = pixelDataVector.size();


				fstream pixelDataFile;
				pixelDataFile.open("C:\\pixels\\pixels5.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);

				pixelDataFile.clear();
				pixelDataFile << " " << endl;
				pixelDataFile << " " << endl;
				pixelDataFile << "New Operation Begin" << endl;

				for (PixelObject currentPixel : pixelDataVector)
				{



					std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
						"X Position = " << currentPixel.xPosition << " " <<
						"Y Position = " << currentPixel.yPosition << " " <<
						"The Value Of Red = " << currentPixel.RMask << " "
						<< "The Value Of Green = " << currentPixel.GMask << " " <<
						"The Value Of Blue = " << currentPixel.BMask << endl;

					pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
					pixelDataFile << "X Position = " << currentPixel.xPosition << endl;
					pixelDataFile << "Y Position = " << currentPixel.yPosition << endl;
					pixelDataFile << "The Value Of Red = " << currentPixel.RMask << endl;
					pixelDataFile << "The Value Of Green = " << currentPixel.GMask << endl;
					pixelDataFile << "The Value Of Blue = " << currentPixel.BMask << endl;

					pAmountOfPixelsLeft--;

				}



			}



			SafeReleaseA(&pDecoder);
		
	myfile.close();
	*pixelObjectOutput = pixelDataVector;

	return hr;
}

			



			


			


HRESULT Direct2DManagementClass::CreateD2DBitmapWithStoredPixels(HINSTANCE hInstance, HWND hWnd, ID2D1Bitmap** pBitmapObject, vector<PixelObject> *pOutputVector)
{
	HRESULT hr = S_OK;
	vector<PixelData*> outputVector = vector<PixelData*>();
		ofstream myfile("imagebuffer.txt");
	myfile << "Please write this text to a file.\n this text is written using C++\n";
	
	CreateWindowSizeDependentResources(hInstance, hWnd);
	UINT cbBufferSize = 0;

	WCHAR szFileName[MAX_PATH];
	BYTE* pixelArrayPointer;

	vector<PixelObject> pixelDataArray = vector<PixelObject>();
	IWICStream* pImageReadingStream = nullptr;
	IWICBitmapSource* pSource = nullptr;
	IWICImagingFactory* pImagingFactory = nullptr;
	IWICBitmapFrameDecode *pFrame = nullptr;
	IWICFormatConverter* pConvertedSourceBitmap = nullptr;
	IWICBitmap* pBitmapObj = nullptr;
	ID2D1Bitmap* pOutputBitmap;
	float pTargetHeight = 20.0f;
	float pTargetWidth = 20.0f;
	float xPositionStart = 0.0f;
	float yPositionStart = 0.0f;
	WICRect rc = { 0.0f, 0.0f, 40.0f, 40.0f };
	double dpiX, dpiY;
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pImagingFactory));


	if (LocateImageFile(hWnd, szFileName, ARRAYSIZE(szFileName)))
	{


		IWICBitmapDecoder *pDecoder = NULL;
		hr = pImagingFactory->CreateDecoderFromFilename(
			szFileName,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&pDecoder);




		hr = pImagingFactory->CreateStream(&pImageReadingStream);

		if (SUCCEEDED(hr))
		{
			hr = pImageReadingStream->InitializeFromFilename(szFileName, GENERIC_READ);
		}

		UINT pFrameCount = 0;

		if (SUCCEEDED(hr))
		{
			pDecoder->GetFrameCount(&pFrameCount);
		}


		if (SUCCEEDED(hr) && pFrameCount > 0)
		{
			hr = pDecoder->GetFrame(0, &pFrame);

		}



		if (SUCCEEDED(hr))
		{


			hr = m_pWICFactory->CreateBitmapFromSource(pFrame,
				WICBitmapCacheOnDemand,
				&pBitmapObj);
			IWICPalette* pImagePalette = nullptr;
			pBitmapObj->CopyPalette(pImagePalette);
		}


		pBitmapObj->GetResolution(&dpiX, &dpiY);

		D2D1_SIZE_F pSize = m_pD2DDeviceContext1->GetSize();



		if (SUCCEEDED(hr))
		{
			hr = pBitmapObj->Lock(&rc, WICBitmapLockRead, &pBitmapLock);
		}



		UINT cbBufferSize = 0;
		UINT cbStride = 0;
		BYTE  *pv;

		hr = pBitmapLock->GetStride(&cbStride);

		if (SUCCEEDED(hr))
		{
			hr = pBitmapLock->GetDataPointer(&cbBufferSize, &pv);
		}

		pixelArrayPointer = &pv[0];


		vector<BYTE*> pixelVector = vector<BYTE*>();
		pixelVector.insert(pixelVector.end(), pixelArrayPointer);

		for (int pIndex = 0; pIndex < sizeof(pv); ++pIndex)

		{

			float pixelX = (float)pv[pIndex];
			float pixelY = (float)pv[pIndex + 1];
			float pixelR = (float)pv[pIndex + 2];
			float pixelG = (float)pv[pIndex + 3];
			float pixelB = (float)pv[pIndex + 4];

			PixelObject pNewPixelObject = PixelObject(pIndex, pixelX,
				pixelY,
				pixelR,
				pixelG,
				pixelB);

			std::cout << "Pixel Id = " << pNewPixelObject.pixelNumber <<
				"Pixel X = " << pNewPixelObject.xPosition <<
				"Pixel Y = " << pNewPixelObject.yPosition <<
				"Pixel R = " << pNewPixelObject.RMask <<
				"Pixel G = " << pNewPixelObject.GMask <<
				"Pixel B = " << pNewPixelObject.BMask << endl;
			pixelDataArray.push_back(pNewPixelObject);
			
		}


		if (SUCCEEDED(hr))
		{
			hr = pImagingFactory->CreateFormatConverter(&pConvertedSourceBitmap);
		}


		hr = pConvertedSourceBitmap->Initialize(
			pFrame,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeCustom);

	}

	if (SUCCEEDED(hr))
	{

		hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(pConvertedSourceBitmap, NULL, &pOutputBitmap);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidColorBrush);
	}

	if (SUCCEEDED(hr))
	{
		hr = DrawD2DContent(hInstance, hWnd, pOutputBitmap);
	}

	UINT pImageWidth = 0;
	UINT pImageHeight = 0;
	IWICBitmapSource* pConverter = nullptr;
	WICPixelFormatGUID pixelFormat;

	if (SUCCEEDED(hr))
	{
		pSource = pFrame;
		pSource->AddRef();
		pSource->GetSize(&pImageWidth, &pImageHeight);
	}
	IWICPixelFormatInfo* pixelFormatInfo = nullptr;

	if (SUCCEEDED(hr))
	{
		pSource->GetPixelFormat(&pixelFormat);
	}



	if (!IsEqualGUID(pixelFormat, GUID_WICPixelFormat128bppPRGBAFloat))
	{

		hr = WICConvertBitmapSource(GUID_WICPixelFormat128bppPRGBAFloat, pSource, &pConverter);
		IWICBitmapLock* pBitmapLock = nullptr;


		if (SUCCEEDED(hr))
		{
			pSource->Release();     // the converter has a reference to the source
			pSource = NULL;         // so we don't need it anymore.
			pSource = pConverter;   // let's treat the 128bppPABGR converter as the source
		}


		IWICPalette* pImagePalette = nullptr;
		pSource->CopyPalette(pImagePalette);
	}
	IWICBitmap* pIBitmap = nullptr;
	hr = m_pWICFactory->CreateBitmapFromSource(
		pFrame,          // Create a bitmap from the image frame
		WICBitmapCacheOnDemand,  // Cache metadata when needed
		&pIBitmap);              // Pointer to the bitmap








	if (SUCCEEDED(hr))
	{
		hr = pIBitmap->Lock(&rc, WICBitmapLockRead, &pBitmapLock);
	}
	if (SUCCEEDED(hr))
	{
		hr = pBitmapLock->GetDataPointer(&cbBufferSize, &pixelArrayPointer);
	}

	CImageFileData* pImageClass = new CImageFileData((char*)pixelArrayPointer);


	if (pConverter)
	{
		UINT cbStride = pImageWidth * sizeof(float) * 4;
		UINT cbBufferSize = cbStride;

		float *pixels = new float[cbBufferSize / sizeof(float)];

		double pWidth = 40.0f;
		double pHeight = 40.0f;
		
	
		WICRect pWicRect = { 0.0f, 0.0f, pWidth, pHeight };
		UINT bSize = 0;

		UINT rowPitch1 = 4 * pWidth;
		UINT stride = 0; // stride  = bytes per row					 // (width * (bpp/8)) + padding and round to the nearest DWORD
		stride = (((((UINT)pWidth * 32) + 31) & ~31) >> 3);
		bSize = pTargetHeight * pWidth;
		BYTE *pixelArray = new BYTE[(UINT)pTargetHeight * stride];
		pSource->CopyPixels(&pWicRect, stride, bSize, pixelArray);
		HDC hDC;

		hDC = GetDC(hWnd);

		HBITMAP map = CreateBitmap((UINT)pWidth,// width. 512 in my case
			(UINT)pHeight, // height
			1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
			8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
			(void*)pixelArray); // pointer to array
		// Temp HDC to copy picture
		HDC src = CreateCompatibleDC(hDC); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
		SelectObject(src, map); // Inserting picture into our temp HDC
		// Copy image from temp HDC to window




		std::cout << "Pixels Begin" << endl;
		vector <PixelObject> pixelDataVector = vector <PixelObject>();

		int pixelCounter = 0;


		//	pBitmapObj->GetResolution(&pWidth, &pHeight);

		std::cout << "We will select a rect area with size 20x20 " << endl;
		for (UINT pXVal = 0; pXVal <= pWidth - 1; ++pXVal)
		{
			for (UINT pYVal = 0; pYVal <= pHeight - 1; ++pYVal)
			{
				COLORREF pCurrentPixel = GetPixel(hDC, pXVal, pYVal);
				int pRedValue = GetRValue(pCurrentPixel);
				int pGreenValue = GetGValue(pCurrentPixel);
				int pBlueValue = GetBValue(pCurrentPixel);
				PixelObject pixelObj = PixelObject(pixelCounter, pXVal, pYVal, pRedValue, pGreenValue, pBlueValue);
				pixelDataVector.push_back(pixelObj);
				
				std::cout << "Amount of total pixels processed  = " << pixelCounter << endl;
				pixelCounter++;
			}

		}

		std::cout << "The size of pixel buffer = " << pixelDataVector.size() << endl;
		int pAmountOfPixelsLeft = 0;
		pAmountOfPixelsLeft = pixelDataVector.size();


		fstream pixelDataFile;
		pixelDataFile.open("C:\\pixels\\pixels5.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);

		pixelDataFile.clear();
		pixelDataFile << " " << endl;
		pixelDataFile << " " << endl;
		pixelDataFile << "New Operation Begin" << endl;
		time_t currentTime = time(0);
		pixelDataFile << "current time" << currentTime << endl;
		for (PixelObject currentPixel : pixelDataVector)
		{

			

			std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
				"X Position = " << currentPixel.xPosition << " " <<
				"Y Position = " << currentPixel.yPosition << " " <<
				"The Value Of Red = " << currentPixel.RMask << " "
				<< "The Value Of Green = " << currentPixel.GMask << " " <<
				"The Value Of Blue = " << currentPixel.BMask << endl;

			pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
			pixelDataFile << "X Position = " << currentPixel.xPosition << endl;
			pixelDataFile << "Y Position = " << currentPixel.yPosition << endl;
			pixelDataFile << "The Value Of Red = " << currentPixel.RMask << endl;
			pixelDataFile << "The Value Of Green = " << currentPixel.GMask << endl;
			pixelDataFile << "The Value Of Blue = " << currentPixel.BMask << endl;

			pAmountOfPixelsLeft--;

		}
		pixelDataFile << "New Operation End" << endl;
		pixelDataFile << " " << endl;
		pixelDataFile << " " << endl;
		pixelDataFile.close();
		std::cout << "Pixels END" << endl;


 		*pOutputVector = pixelDataVector;



	
		myfile.close();


	}
	return hr;
}


HRESULT Direct2DManagementClass::CreateBitmapWithASetOfDimensions(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pImageWidth, float pImageHeight)
{
	ofstream myfile("imagebuffer.txt");
	myfile << "Please write this text to a file.\n this text is written using C++\n";
	HRESULT hr = S_OK;
	CreateWindowSizeDependentResources(hInstance, hWnd);
	UINT cbBufferSize = 0;
	WICRect pWicRect;
	WCHAR szFileName[MAX_PATH];
	BYTE* pixelArrayPointer;
	UINT pWidth = 0.0f;
	UINT pHeight = 0.0f;
	vector<PixelObject> pixelDataArray = vector<PixelObject>();
	IWICStream* pImageReadingStream = nullptr;
	IWICBitmapSource* pSource = nullptr;
	IWICImagingFactory* pImagingFactory = nullptr;
	IWICBitmapFrameDecode *pFrame = nullptr;
	IWICFormatConverter* pConvertedSourceBitmap = nullptr;
	ID2D1Bitmap* pBitmapObject = nullptr;
	IWICBitmap* pBitmapObj = nullptr;

	WICRect rc = {0.0f, 0.0f, 0.0f, 0.0f };
	double dpiX, dpiY;
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pImagingFactory));


	if (LocateImageFile(hWnd, szFileName, ARRAYSIZE(szFileName)))
	{


		IWICBitmapDecoder *pDecoder = NULL;
		hr = pImagingFactory->CreateDecoderFromFilename(
			szFileName,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&pDecoder);




		hr = pImagingFactory->CreateStream(&pImageReadingStream);

		if (SUCCEEDED(hr))
		{
			hr = pImageReadingStream->InitializeFromFilename(szFileName, GENERIC_READ);
		}

		UINT pFrameCount = 0;

		if (SUCCEEDED(hr))
		{
			pDecoder->GetFrameCount(&pFrameCount);
		}


		if (SUCCEEDED(hr) && pFrameCount > 0)
		{
			hr = pDecoder->GetFrame(0, &pFrame);

		}



		if (SUCCEEDED(hr))
		{


			hr = m_pWICFactory->CreateBitmapFromSource(pFrame,
				WICBitmapCacheOnDemand,
				&pBitmapObj);
			IWICPalette* pImagePalette = nullptr;
			pBitmapObj->CopyPalette(pImagePalette);
		}


		pBitmapObj->GetResolution(&dpiX, &dpiY);

		pBitmapObj->GetSize(&pWidth, &pHeight);

		rc = { pXPosition, pYPosition, pImageWidth, pImageHeight };
		pWicRect = { pXPosition, pYPosition, pImageWidth, pImageHeight };
		if (SUCCEEDED(hr))
		{
			hr = pBitmapObj->Lock(&rc, WICBitmapLockRead, &pBitmapLock);
		}



		UINT cbBufferSize = 0;
		UINT cbStride = 0;
		BYTE  *pv;

		hr = pBitmapLock->GetStride(&cbStride);

		if (SUCCEEDED(hr))
		{
			hr = pBitmapLock->GetDataPointer(&cbBufferSize, &pv);
		}

		pixelArrayPointer = &pv[0];


		vector<BYTE*> pixelVector = vector<BYTE*>();
		pixelVector.insert(pixelVector.end(), pixelArrayPointer);

		for (int pIndex = 0; pIndex < sizeof(pv); ++pIndex)

		{

			float pixelX = (float)pv[pIndex];
			float pixelY = (float)pv[pIndex + 1];
			float pixelR = (float)pv[pIndex + 2];
			float pixelG = (float)pv[pIndex + 3];
			float pixelB = (float)pv[pIndex + 4];

			PixelObject pNewPixelObject = PixelObject(pIndex, pixelX,
				pixelY,
				pixelR,
				pixelG,
				pixelB);

			std::cout << "Pixel Id = " << pNewPixelObject.pixelNumber <<
				"Pixel X = " << pNewPixelObject.xPosition <<
				"Pixel Y = " << pNewPixelObject.yPosition <<
				"Pixel R = " << pNewPixelObject.RMask <<
				"Pixel G = " << pNewPixelObject.GMask <<
				"Pixel B = " << pNewPixelObject.BMask << endl;
			pixelDataArray.push_back(pNewPixelObject);

		}


		if (SUCCEEDED(hr))
		{
			hr = pImagingFactory->CreateFormatConverter(&pConvertedSourceBitmap);
		}


		hr = pConvertedSourceBitmap->Initialize(
			pFrame,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeCustom);

	}

	if (SUCCEEDED(hr))
	{

		hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(pConvertedSourceBitmap, NULL, &pBitmapObject);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidColorBrush);
	}

	if (SUCCEEDED(hr))
	{
		hr = DrawD2DContent(hInstance, hWnd, pBitmapObject);
	}


	IWICBitmapSource* pConverter = nullptr;
	WICPixelFormatGUID pixelFormat;

	if (SUCCEEDED(hr))
	{

		pSource = pFrame;
		pSource->AddRef();
		pSource->GetSize(&pWidth, &pHeight);
	}
	IWICPixelFormatInfo* pixelFormatInfo = nullptr;

	if (SUCCEEDED(hr))
	{
		pSource->GetPixelFormat(&pixelFormat);
	}



	if (!IsEqualGUID(pixelFormat, GUID_WICPixelFormat128bppPRGBAFloat))
	{

		hr = WICConvertBitmapSource(GUID_WICPixelFormat128bppPRGBAFloat, pSource, &pConverter);
		IWICBitmapLock* pBitmapLock = nullptr;


		if (SUCCEEDED(hr))
		{
			pSource->Release();     // the converter has a reference to the source
			pSource = NULL;         // so we don't need it anymore.
			pSource = pConverter;   // let's treat the 128bppPABGR converter as the source
		}


		IWICPalette* pImagePalette = nullptr;
		pSource->CopyPalette(pImagePalette);
	}
	IWICBitmap* pIBitmap = nullptr;
	hr = m_pWICFactory->CreateBitmapFromSource(
		pFrame,          // Create a bitmap from the image frame
		WICBitmapCacheOnDemand,  // Cache metadata when needed
		&pIBitmap);              // Pointer to the bitmap








	if (SUCCEEDED(hr))
	{
		hr = pIBitmap->Lock(&rc, WICBitmapLockRead, &pBitmapLock);
	}
	if (SUCCEEDED(hr))
	{
		hr = pBitmapLock->GetDataPointer(&cbBufferSize, &pixelArrayPointer);
	}

	CImageFileData* pImageClass = new CImageFileData((char*)pixelArrayPointer);



	if (pConverter)
	{
		UINT cbStride = pWidth * sizeof(float) * 4;
		UINT cbBufferSize = cbStride;

		float *pixels = new float[cbBufferSize / sizeof(float)];





	
	}


	pWicRect = { pXPosition, pYPosition, pImageWidth, pImageHeight };
	UINT bSize = 0;




	UINT rowPitch1 = 4 * pImageWidth;
	UINT stride = 0; // stride  = bytes per row					 // (width * (bpp/8)) + padding and round to the nearest DWORD
	stride = (((((UINT)pWidth * 32) + 31) & ~31) >> 3);
	bSize = pImageHeight * pImageWidth;
	BYTE *pixelArray = new BYTE[(UINT)pImageHeight * stride];

	pSource->CopyPixels(&pWicRect, stride, bSize, pixelArray);
	HDC hDC;

	hDC = GetDC(hWnd);

	HBITMAP map = CreateBitmap((UINT)pWidth,// width. 512 in my case
		(UINT)pImageHeight, // height
		1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
		8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
		(void*)pixelArray); // pointer to array
	// Temp HDC to copy picture
	HDC src = CreateCompatibleDC(hDC); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
	SelectObject(src, map); // Inserting picture into our temp HDC
	// Copy image from temp HDC to window




	std::cout << "Pixels Begin" << endl;
	vector <PixelObject> pixelDataVector = vector <PixelObject>();

	int pixelCounter = 0;


	//	pBitmapObj->GetResolution(&pWidth, &pHeight);

	std::cout << "We will select a rect area with size 20x20 " << endl;
	for (UINT pXVal = 0; pXVal <= pImageWidth - 1; ++pXVal)
	{
		for (UINT pYVal = 0; pYVal <= pImageHeight - 1; ++pYVal)
		{
			COLORREF pCurrentPixel = GetPixel(hDC, pXVal, pYVal);
			int pRedValue = GetRValue(pCurrentPixel);
			int pGreenValue = GetGValue(pCurrentPixel);
			int pBlueValue = GetBValue(pCurrentPixel);
			PixelObject pixelObj = PixelObject(pixelCounter, pXVal, pYVal, pRedValue, pGreenValue, pBlueValue);
			pixelDataVector.push_back(pixelObj);
			std::cout << "Amount of total pixels processed  = " << pixelCounter << endl;
			pixelCounter++;
		}

	}

	std::cout << "The size of pixel buffer = " << pixelDataVector.size() << endl;
	int pAmountOfPixelsLeft = 0;
	pAmountOfPixelsLeft = pixelDataVector.size();


	fstream pixelDataFile;
	pixelDataFile.open("C:\\pixels\\pixels5.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);

	pixelDataFile.clear();
	pixelDataFile << " " << endl;
	pixelDataFile << " " << endl;
	pixelDataFile << "New Operation Begin" << endl;
	time_t currentTime = time(0);
	pixelDataFile << "current time" << currentTime << endl;
	for (PixelObject currentPixel : pixelDataVector)
	{



		std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
			"X Position = " << currentPixel.xPosition << " " <<
			"Y Position = " << currentPixel.yPosition << " " <<
			"The Value Of Red = " << currentPixel.RMask << " "
			<< "The Value Of Green = " << currentPixel.GMask << " " <<
			"The Value Of Blue = " << currentPixel.BMask << endl;

		pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
		pixelDataFile << "X Position = " << currentPixel.xPosition << endl;
		pixelDataFile << "Y Position = " << currentPixel.yPosition << endl;
		pixelDataFile << "The Value Of Red = " << currentPixel.RMask << endl;
		pixelDataFile << "The Value Of Green = " << currentPixel.GMask << endl;
		pixelDataFile << "The Value Of Blue = " << currentPixel.BMask << endl;

		pAmountOfPixelsLeft--;

	}
	pixelDataFile << "New Operation End" << endl;
	pixelDataFile << " " << endl;
	pixelDataFile << " " << endl;
	pixelDataFile.close();
	std::cout << "Pixels END" << endl;






	SafeReleaseA(&pDecoder);
	myfile.close();

	return hr;
}

HRESULT Direct2DManagementClass::PlotGraphForGivenDataSet(HINSTANCE hInstance, HWND hWnd, vector<float> pXValuesForGiverData, vector<float> pYValuesForGivenData)
{
	HRESULT hr = S_OK;
	std::cout << "Now We Will Plot the Functions Graph For Selected Sets of Data" << endl;

	int pIndex = 0;


	vector<float> pMaximizedXValues = vector<float>();
	vector<float> pMaximizedYValues = vector<float>();


	for (float pXPosition : pXValuesForGiverData)
	{
		float pMaximimizedX = pXPosition * 20;
		pMaximizedXValues.push_back(pMaximimizedX);
	}

	for (float pYPosition : pYValuesForGivenData)
	{
		float pMaximimizedY = pYPosition * 20;
		pMaximizedYValues.push_back(pMaximimizedY);
	}

	ID2D1PathGeometry* pPathGeometryObject = nullptr;

	std::cout << "Creating new Path Geometry Object" << endl;


	std::cout << "Creating the Black Color Brush" << endl;
	ID2D1SolidColorBrush* pBlackColorBrush = nullptr;
	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackColorBrush);
	ID2D1SolidColorBrush* pWhiteColorBrush = nullptr;
	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pWhiteColorBrush);

	std::cout << "Constructing Path Geometry Data" << endl;

	vector<D2D1_RECT_F> pRectangleSet = vector<D2D1_RECT_F>();
	hr = CreatePathGeometryForASetOfData(hInstance, hWnd, pMaximizedXValues, pMaximizedYValues, &pRectangleSet);

	if (SUCCEEDED(hr))
	{
		hr = RenderPathGeometryObject(hInstance, hWnd, pRectangleSet, pBlackColorBrush);
	}

	if (SUCCEEDED(hr))
	{
		pSwapChainObjectD2D->Present(0, 0);
	}

	
	return hr;
}



HRESULT Direct2DManagementClass::CreatePathGeometryForASetOfData(HINSTANCE hInstance, HWND hWnd, vector<float> pXValues, vector<float> pYValues, vector<D2D1_RECT_F> *ppRectangleSet)
{
	HRESULT hr = S_OK;
	std::cout << "Initializing Path Geometry Object" << endl;
	ID2D1PathGeometry* pPathGeometryObject = nullptr;
	std::cout << "Getting the Factory Object" << endl;
	ID2D1Factory* m_pD2D1FactoryObject = nullptr;
	m_pD2DDevice->GetFactory(&m_pD2D1FactoryObject);
	vector<D2D1_POINT_2F> pPointsVectorData = vector<D2D1_POINT_2F>();
	vector<D2D1_RECT_F> pRectangleSet = vector<D2D1_RECT_F>();
	std::cout << "Creating the Path Geometry Object" << endl;
	MessageBoxA(hWnd, (LPCSTR)"Создаем Объект PathGeometry", (LPCSTR)"Message", 0);

	


	std::cout << "Filling the Values For the Function Graph" << endl;
	int pIndex = 0;
	for (float pXValue : pXValues)
	{
		for (float pYValue : pYValues)
		{
			std::cout << "Processing Point At Index: " << pIndex << "X =" << pXValue << "Y = " << pYValue << endl;
			D2D1_POINT_2F pPointObject = D2D1::Point2F(pXValue, pYValue);

			D2D1_RECT_F pRectangle = D2D1::RectF(pXValue, pYValue, pXValue + 30, pYValue + 30);

			pRectangleSet.push_back(pRectangle);
	
		}
	}
	
	std::cout << "Adding Points And Lines" << endl;

	
	
	*ppRectangleSet = pRectangleSet;

	return hr;
}


HRESULT Direct2DManagementClass::RenderCoordinatePlane(HINSTANCE hInstance, HWND hWnd, ID2D1SolidColorBrush* pSolidColorBrush)
{
	HRESULT hr = S_OK;
	std::cout << "Rendering the coordinate Plane Object" << endl;
	std::cout << "First We Mus Establish the Size of the Plane" << endl;
	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);


	float pWidth = rc.right - rc.left;
	float pHeight = rc.bottom - rc.top;
	
	D2D1_POINT_2F pLeftTop = D2D1::Point2F(0.0f, 0.0f);
	D2D1_POINT_2F pLeftBottom = D2D1::Point2F(0.0f, pHeight);
	D2D1_POINT_2F pRightTop = D2D1::Point2F(pWidth, 0.0f);
	D2D1_POINT_2F pRightBottom = D2D1::Point2F(pWidth, pHeight);
	D2D1_POINT_2F pCentreTop = D2D1::Point2F(pWidth / 2, 0.0f);
	D2D1_POINT_2F pCentreBottom = D2D1::Point2F(pWidth / 2, pHeight);


	D2D1_RECT_F pRectObject = D2D1::RectF(0.0f, 0.0, pWidth, pHeight);
	m_pD2DDeviceContext1->DrawLine(pLeftTop, pLeftBottom, pSolidColorBrush);
	m_pD2DDeviceContext1->DrawLine(pLeftBottom, pRightBottom, pSolidColorBrush);

	return hr;

}

HRESULT Direct2DManagementClass::RenderPathGeometryObject(HINSTANCE hInstance, HWND hWnd, vector<D2D1_RECT_F> pRectangleSet,
	ID2D1SolidColorBrush* pSolidColorBrush)
{
	HRESULT hr = S_OK;
	std::cout << "Staring to render the path geometry Object" << endl;
	PAINTSTRUCT ps;


	if (BeginPaint(hWnd, &ps))
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}

		m_pD2DDeviceContext1->BeginDraw();
		
		for (D2D1_RECT_F pRectangle : pRectangleSet)
		{
			m_pD2DDeviceContext1->DrawRectangle(pRectangle, pSolidColorBrush, 10.0f);
		}

		m_pD2DDeviceContext1->EndDraw();

		if (hr == D2DERR_RECREATE_TARGET)
		{
			SafeReleaseA(&m_pD2DBitmap);
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}






		pSwapChainObjectD2D->Present(0, 0);

		EndPaint(hWnd, &ps);

		return hr;
	}
}

HRESULT Direct2DManagementClass::CreateBitmapDrawingClipObject(HINSTANCE hInstance, HWND hWnd, LPCWSTR
	pFileName, float pXPosition, float pYPosition, float pWidth, float pHeight,
	vector<PixelObject> *ppPixelObject)
{
	ofstream myfile("imagebuffer.txt");
	myfile << "Please write this text to a file.\n this text is written using C++\n";
	IWICBitmapSource* pSource = nullptr;
	IWICBitmapSource* pSecondarySource = nullptr;

	IWICBitmap* pBitmapObject = nullptr;
	IWICBitmapLock* pBitmapLock = nullptr;
	IWICBitmapLock* pBitmapLock1 = nullptr;
	IWICBitmapClipper *pIClipper = nullptr;
	BYTE* pixels = nullptr;
	HRESULT hr = S_OK;
	WCHAR szFileName[MAX_PATH];
	WICRect rcLock = { pXPosition, pYPosition, pWidth, pHeight };
	ID2D1Image* outputImage = nullptr;
	vector <PixelObject> pixelDataVector = vector <PixelObject>();
	HDC src = NULL;
	HDC hDC = NULL;
	WICPixelFormatGUID pixelFormat;
	IWICBitmapSource* piConverter = nullptr;
	UINT pImageHeight = NULL;
	UINT pImageWidth = NULL;
	BYTE* pixelArrayPointer = nullptr;
	D2D1_RECT_F rectangle = D2D1::RectF(0, 0, pXPosition + pWidth, pYPosition + pHeight);
	UINT cbBufferSize = 0;
	BYTE* pixelDataArray = NULL;
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapDecoder* pImageDecoder = nullptr;
	IWICBitmapFrameDecode *pFrame = NULL;
	UINT uiWidth = 0;
	UINT uiHeight = 0;
	WICRect rcClip = { pXPosition, pYPosition, pWidth, pHeight };


	fstream pixelDataFile;
	pixelDataFile.open("C:\\pixels\\ImageClippingExperimentsTests.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);

	pixelDataFile.clear();
	pixelDataFile << " " << endl;
	pixelDataFile << " " << endl;
	pixelDataFile << "New Operation Begin" << endl;
	time_t currentTimeVar = time(0);
	tm* currentTime = localtime(&currentTimeVar);
	cout << "current time" << currentTimeVar << endl;
	pixelDataFile << "current time" << currentTimeVar << endl;

	
		// Prepare iterator pairs to iterate the file content!

		cout << "Stage 1: Decoder" << endl;
		pixelDataFile << "Stage 1: Decoder" << endl;
		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);

		cout << "Decoder Creation Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Decoder Creation Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;

		hr = m_pWICFactory->CreateDecoderFromFilename(
			pFileName,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&pImageDecoder);

		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);

		cout << "Decoder Creation Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Decoder Creation Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;


		cout << "Stage 2: Frame" << endl;
		pixelDataFile << "Stage 2: Frame" << endl;
		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Frame Creation Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Frame Creation Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;



		if (SUCCEEDED(hr))
		{
			hr = pImageDecoder->GetFrame(0, &pFrame);
		}


		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Frame Creation Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Frame Creation Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;

		cout << "Stage 3: Bitmap Source" << endl;
		pixelDataFile << "Stage 3: Bitmap Source" << endl;
		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);

		cout << "Bitmap Source Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Frame Creation Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;

		if (SUCCEEDED(hr))
		{
			pSource = pFrame;
			pSource->AddRef();
			pSource->GetSize(&uiWidth, &uiHeight);

			pSecondarySource = pFrame;
			pSecondarySource->AddRef();
			pSecondarySource->GetSize(&uiWidth, &uiHeight);

			IWICPalette* pImagePalette = nullptr;
			pSecondarySource->CopyPalette(pImagePalette);
		}

		WICRect rc = { pXPosition, pYPosition, pWidth, pHeight };


		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Bitmap Source Creation Terminated At At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Bitmap Source Creation Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;

		cout << "Stage 4: Converting Wic Formats to Direct2D Formats" << endl;
		pixelDataFile << "Stage 4: Converting Wic Formats to Direct2D Formats" << endl;
		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Format Conversion  Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Format Conversion Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;



		// Initialize the clipper with the given rectangle of the frame's image data.

		if (SUCCEEDED(hr))
		{
			SafeReleaseA(&m_pConvertedSourceBitmap);
			hr = m_pWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pConvertedSourceBitmap->Initialize(
				pSecondarySource,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeCustom);

		}
		if (SUCCEEDED(hr))
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}
		if (SUCCEEDED(hr))
		{

			SafeReleaseA(&m_pD2DBitmap);
			hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);

		}

		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Format Conversion terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Format Conversion terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;


		cout << "Stage 5: Drawing Pixels with Direct2D" << endl;
		pixelDataFile << "Stage 5: Drawing Pixels with Direct2D" << endl;
		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Drawing  Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Drawing Conversion Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;



		if (SUCCEEDED(hr))
		{
			D2D1_SIZE_F pSize = m_pD2DBitmap->GetSize();
			pImageWidth = pSize.width;
			pImageHeight = pSize.height;

			hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidColorBrush);
		}

		if (SUCCEEDED(hr))
		{

			hr = DrawD2DContentAxisAlignedClip(hInstance, hWnd, m_pD2DBitmap, rectangle);

		}

		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Drawing Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Drawing terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;


		cout << "Stage 6: Convertion of image data to GDI HBITMAP For Pixel Extraction" << endl;
		pixelDataFile << "Stage 6: Convertion of image data to GDI HBITMAP For Pixel Extraction" << endl;
		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Convertion Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Convertion Started: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;



		IWICBitmapSource* pConverter = nullptr;
		
		if (SUCCEEDED(hr))
		{


			pSecondarySource->AddRef();
			pSecondarySource->GetSize(&uiWidth, &uiHeight);
		}

		IWICPixelFormatInfo* pixelFormatInfo = nullptr;
		if (SUCCEEDED(hr))
		{
			pSecondarySource->GetPixelFormat(&pixelFormat);
		}

		if (!IsEqualGUID(pixelFormat, GUID_WICPixelFormat128bppPRGBAFloat))
		{

			hr = WICConvertBitmapSource(GUID_WICPixelFormat128bppPRGBAFloat, pSecondarySource, &pConverter);
			IWICBitmapLock* pBitmapLock = nullptr;


			if (SUCCEEDED(hr))
			{
				pSecondarySource->Release();     // the converter has a reference to the source
				pSecondarySource = NULL;         // so we don't need it anymore.
				pSecondarySource = pConverter;   // let's treat the 128bppPABGR converter as the source
			}


			IWICPalette* pImagePalette = nullptr;
			pSecondarySource->CopyPalette(pImagePalette);
		}




		CImageFileData* pImageClass = new CImageFileData((char*)pixelArrayPointer);

		if (pConverter)
		{
			UINT cbStride = pWidth * sizeof(float) * 4;
			UINT cbBufferSize = cbStride;
			float *pixels = new float[cbBufferSize / sizeof(float)];
			if (pHeight < rc.Height && pWidth < rc.Width)
			{
				rc.Height = pHeight;
				rc.Width = pWidth;
			}
			else
			{
				pWidth = rc.Width;
				pHeight = rc.Height;
			}
		}


		UINT bSize = 0;




		UINT rowPitch1 = 4 * pWidth;
		UINT stride = 0; // stride  = bytes per row					 // (width * (bpp/8)) + padding and round to the nearest DWORD
		stride = (((((UINT)pWidth * 32) + 31) & ~31) >> 3);
		bSize = pHeight * pWidth;
		BYTE *pixelArray = new BYTE[(UINT)pHeight * stride];


		pSecondarySource->CopyPixels(NULL, stride, bSize, pixelArray);
		hDC = NULL;
		hDC = GetDC(hWnd);

		COLORREF bkColor = RGB(255, 255, 255);

		SetBkColor(hDC, bkColor);

		HBITMAP map = CreateBitmap((UINT)pWidth,// width. 512 in my case
			(UINT)pHeight, // height
			1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
			8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
			(void*)pixelArray); // pointer to array
		// Temp HDC to copy picture
		src = CreateCompatibleDC(hDC); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
		SelectObject(src, map); // Inserting picture into our temp HDC
		// Copy image from temp HDC to window

		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Convertion Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Drawing terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;


		cout << "Stage 7: Reading Pixels and Preparing Pixel Vector" << endl;
		pixelDataFile << "Stage 7: Reading Pixels and Preparing Pixel Vector" << endl;
		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Pixel Extraction Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Pixel Extraction Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;





		std::cout << "Pixels Begin" << endl;

		int pixelCounter = 0;


		//	pBitmapObj->GetResolution(&pWidth, &pHeight);

		UINT pYVal = 0;
		UINT pXVal = 0;


		for (pXVal = pXPosition; pXVal < pXPosition + pWidth; ++pXVal)
		{
			for (pYVal = pYPosition; pYVal < pYPosition + pHeight; ++pYVal)
				// Your code here
			{

				std::cout << "Current X Value: " << pXVal << " " << "Current Y Value: " << pYVal << endl;
				COLORREF pCurrentPixel = GetPixel(hDC, pXVal, pYVal);
				int pRedValue = GetRValue(pCurrentPixel);
				int pGreenValue = GetGValue(pCurrentPixel);
				int pBlueValue = GetBValue(pCurrentPixel);
				PixelObject pixelObj = PixelObject(pixelCounter, pXVal, pYVal, pRedValue, pGreenValue, pBlueValue);
				pixelDataVector.push_back(pixelObj);
				std::cout << "Amount of total pixels processed  = " << pixelCounter << endl;
				pixelCounter = pixelCounter + 1;


			}
		}

		std::cout << "The size of pixel buffer = " << pixelDataVector.size() << endl;
		int pAmountOfPixelsLeft = 0;
		pAmountOfPixelsLeft = pixelDataVector.size();

		if (LOOPS_ARE_TURNED_OFF == FALSE)
		{
			for (PixelObject currentPixel : pixelDataVector)
			{


				if ((currentPixel.RMask != 255.0f) && (currentPixel.GMask != 255.0f) && (currentPixel.BMask != 255.0f))

				{
					std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
						"X Position = " << currentPixel.xPosition << " " <<
						"Y (Position = " << currentPixel.yPosition << " " <<
						"The Value Of Red = " << currentPixel.RMask << " "
						<< "The Value Of Green = " << currentPixel.GMask << " " <<
						"The Value Of Blue = " << currentPixel.BMask << endl;
				}


				pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
				pixelDataFile << "X Position = " << currentPixel.xPosition << endl;
				pixelDataFile << "Y Position = " << currentPixel.yPosition << endl;
				pixelDataFile << "The Value Of Red = " << currentPixel.RMask << endl;
				pixelDataFile << "The Value Of Green = " << currentPixel.GMask << endl;
				pixelDataFile << "The Value Of Blue = " << currentPixel.BMask << endl;

			}
		}




		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Clipping Terminated at: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Clipping Terminated at: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;



		pixelDataFile << "New Operation End" << endl;
		pixelDataFile << " " << endl;
		pixelDataFile << " " << endl;
		pixelDataFile.close();
		std::cout << "Pixels END" << endl;




		ReleaseDC(hWnd, src);
		ReleaseDC(hWnd, hDC);
		DeleteDC(src);
		DeleteDC(hDC);
		SafeReleaseA(&pDecoder);
		SafeReleaseA(&pFrame);
		myfile.close();
		*ppPixelObject = pixelDataVector;

		return hr;





	}





HRESULT Direct2DManagementClass::CreateBitmapDrawingClipObjectUsingClipperWithFileNames(HINSTANCE hInstance, HWND hWnd, LPCWSTR pImageFileName,
	float pXPosition, float pYPosition, float pImageWidth, float pImageHeight,
	vector<PixelObject> *ppPixelObject)
{


	HRESULT hr = S_OK;


	ofstream myfile("imagebuffer.txt");

	myfile << "Please write this text to a file.\n this text is written using C++\n";
	IWICBitmapSource* pSource = nullptr;
	IWICBitmapSource* pSecondarySource = nullptr;

	IWICBitmap* pBitmapObject = nullptr;
	IWICBitmapLock* pBitmapLock = nullptr;
	IWICBitmapLock* pBitmapLock1 = nullptr;
	IWICBitmapClipper *pIClipper = nullptr;
	BYTE* pixels = nullptr;
	WCHAR szFileName[MAX_PATH];
	WICRect rcLock = { pXPosition, pYPosition, pImageWidth, pImageHeight };
	ID2D1Image* outputImage = nullptr;
	vector <PixelObject> pixelDataVector = vector <PixelObject>();
	HDC src;
	HDC hDC;
	WICPixelFormatGUID pixelFormat;
	IWICBitmapSource* piConverter = nullptr;
	BYTE* pixelArrayPointer = nullptr;
	D2D1_RECT_F rectangle = D2D1::RectF(0, 0, pXPosition + pImageWidth, pYPosition + pImageHeight);
	UINT cbBufferSize = 0;
	BYTE* pixelDataArray = NULL;
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapDecoder* pImageDecoder = nullptr;
	IWICBitmapFrameDecode *pFrame = NULL;
	UINT uiWidth = 0;
	UINT uiHeight = 0;
	WICRect rcClip = { pXPosition, pYPosition, pImageWidth, pImageHeight };

	// Prepare iterator pairs to iterate the file content!



	hr = m_pWICFactory->CreateDecoderFromFilename(
		pImageFileName,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&pImageDecoder);




	if (SUCCEEDED(hr))
	{
		hr = pImageDecoder->GetFrame(0, &pFrame);
	}



	if (SUCCEEDED(hr))
	{
		pSource = pFrame;
		pSource->AddRef();
		pSource->GetSize(&uiWidth, &uiHeight);

		pSecondarySource = pFrame;
		pSecondarySource->AddRef();
		pSecondarySource->GetSize(&uiWidth, &uiHeight);

		IWICPalette* pImagePalette = nullptr;
		pSecondarySource->CopyPalette(pImagePalette);
	}

	WICRect rc = { pXPosition, pYPosition, pImageWidth, pImageHeight };


	if (SUCCEEDED(hr))
	{
		hr = m_pWICFactory->CreateBitmapClipper(&pIClipper);
	}


	// Initialize the clipper with the given rectangle of the frame's image data.
	if (SUCCEEDED(hr))
	{
		hr = pIClipper->Initialize(pSource, &rcClip);
	}


	if (SUCCEEDED(hr))
	{
		SafeReleaseA(&m_pConvertedSourceBitmap);
		hr = m_pWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pConvertedSourceBitmap->Initialize(
			pIClipper,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeCustom);

	}
	if (SUCCEEDED(hr))
	{
		hr = CreateDeviceResources(hInstance, hWnd);
	}
	if (SUCCEEDED(hr))
	{

		SafeReleaseA(&m_pD2DBitmap);
		hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);

	}



	if (SUCCEEDED(hr))
	{
		D2D1_SIZE_F pSize = m_pD2DBitmap->GetSize();
		pImageWidth = pSize.width;
		pImageHeight = pSize.height;

		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidColorBrush);
	}

	if (SUCCEEDED(hr))
	{


		if (DO_YOU_WANT_MESSAGES == 1)
		{
			if (DISABLE_ALL_MESSAGE_BOXES == FALSE)
			{
			}
		}
		hr = DrawD2DContentAxisAlignedClip(hInstance, hWnd, m_pD2DBitmap, rectangle);

	}


	IWICBitmapSource* pConverter = nullptr;


	if (SUCCEEDED(hr))
	{


		pSecondarySource->AddRef();
		pSecondarySource->GetSize(&uiWidth, &uiHeight);
	}

	IWICPixelFormatInfo* pixelFormatInfo = nullptr;
	if (SUCCEEDED(hr))
	{
		pSecondarySource->GetPixelFormat(&pixelFormat);
	}

	if (!IsEqualGUID(pixelFormat, GUID_WICPixelFormat128bppPRGBAFloat))
	{

		hr = WICConvertBitmapSource(GUID_WICPixelFormat128bppPRGBAFloat, pSecondarySource, &pConverter);
		IWICBitmapLock* pBitmapLock = nullptr;


		if (SUCCEEDED(hr))
		{
			pSecondarySource->Release();     // the converter has a reference to the source
			pSecondarySource = NULL;         // so we don't need it anymore.
			pSecondarySource = pConverter;   // let's treat the 128bppPABGR converter as the source
		}


		IWICPalette* pImagePalette = nullptr;
		pSecondarySource->CopyPalette(pImagePalette);
	}




	CImageFileData* pImageClass = new CImageFileData((char*)pixelArrayPointer);

	if (pConverter)
	{
		UINT cbStride = pImageWidth * sizeof(float) * 4;
		UINT cbBufferSize = cbStride;
		float *pixels = new float[cbBufferSize / sizeof(float)];
		if (pImageHeight < rc.Height && pImageWidth < rc.Width)
		{
			rc.Height = pImageHeight;
			rc.Width = pImageWidth;
		}
		else
		{
			pImageWidth = rc.Width;
			pImageHeight = rc.Height;
		}
	}


	UINT bSize = 0;




	UINT rowPitch1 = 4 * pImageWidth;
	UINT stride = 0; // stride  = bytes per row					 // (width * (bpp/8)) + padding and round to the nearest DWORD
	stride = (((((UINT)pImageWidth * 32) + 31) & ~31) >> 3);
	bSize = pImageHeight * pImageWidth;
	BYTE *pixelArray = new BYTE[(UINT)pImageHeight * stride];


	pSecondarySource->CopyPixels(NULL, stride, bSize, pixelArray);

	hDC = NULL;
	ReleaseDC(hWnd, hDC);
	hDC = GetDC(hWnd);

	COLORREF bkColor = RGB(0, 0, 0);

	SetBkColor(hDC, bkColor);

	HBITMAP map = CreateBitmap((UINT)pImageWidth,// width. 512 in my case
		(UINT)pImageHeight, // height
		1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
		8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
		(void*)pixelArray); // pointer to array
	// Temp HDC to copy picture
	src = CreateCompatibleDC(hDC); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
	SelectObject(src, map); // Inserting picture into our temp HDC
	// Copy image from temp HDC to window




	std::cout << "Pixels Begin" << endl;

	int pixelCounter = 0;


	//	pBitmapObj->GetResolution(&pImageWidth, &pImageHeight);

	UINT pYVal = 0;
	UINT pXVal = 0;
	for (pXVal = pXPosition; pXVal < pXPosition + pImageWidth; ++pXVal)
	{
		for (pYVal = pYPosition; pYVal < pYPosition + pImageHeight; ++pYVal)
			// Your code here
		{
			std::cout << "Current X Value: " << pXVal << " " << "Current Y Value: " << pYVal << endl;
			COLORREF pCurrentPixel = GetPixel(hDC, pXVal, pYVal);
			int pRedValue = GetRValue(pCurrentPixel);
			int pGreenValue = GetGValue(pCurrentPixel);
			int pBlueValue = GetBValue(pCurrentPixel);
			PixelObject pixelObj = PixelObject(pixelCounter, pXVal, pYVal, pRedValue, pGreenValue, pBlueValue);
			pixelDataVector.push_back(pixelObj);
			std::cout << "Amount of total pixels processed  = " << pixelCounter << endl;
			pixelCounter = pixelCounter + 1;

		}
	}

	std::cout << "The size of pixel buffer = " << pixelDataVector.size() << endl;
	int pAmountOfPixelsLeft = 0;
	pAmountOfPixelsLeft = pixelDataVector.size();


	fstream pixelDataFile;
	pixelDataFile.open("C:\\pixels\\ImageClipperTests.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);

	pixelDataFile.clear();
	pixelDataFile << " " << endl;
	pixelDataFile << " " << endl;
	pixelDataFile << "New Operation Begin" << endl;
	time_t currentTime = time(0);
	pixelDataFile << "current time" << currentTime << endl;
	for (PixelObject currentPixel : pixelDataVector)
	{



	//	std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
	//		"X Position = " << currentPixel.xPosition << " " <<
	//		"Y Position = " << currentPixel.yPosition << " " <<
	//		"The Value Of Red = " << currentPixel.RMask << " "
	//		<< "The Value Of Green = " << currentPixel.GMask << " " <<
	//		"The Value Of Blue = " << currentPixel.BMask << endl;

	//	pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
	//	pixelDataFile << "X Position = " << pXPosition << endl;
	//	pixelDataFile << "Y Position = " << pYPosition << endl;
	//	pixelDataFile << "The Value Of Red = " << currentPixel.RMask << endl;
	//	pixelDataFile << "The Value Of Green = " << currentPixel.GMask << endl;
	//	pixelDataFile << "The Value Of Blue = " << currentPixel.BMask << endl;

		pAmountOfPixelsLeft--;

	}










	pixelDataFile << "New Operation End" << endl;
	pixelDataFile << " " << endl;
	pixelDataFile << " " << endl;
	pixelDataFile.close();
	std::cout << "Pixels END" << endl;

	ReleaseDC(hWnd, src);
	ReleaseDC(hWnd, hDC);
	DeleteDC(src);
	DeleteDC(hDC);
	SafeReleaseA(&pDecoder);
	SafeReleaseA(&pFrame);
	SafeReleaseA(&pImageDecoder);
	myfile.close();
	*ppPixelObject = pixelDataVector;

	return hr;
}






HRESULT Direct2DManagementClass::CreateBitmapDrawingClipObjectUsingClipperWithFileNamesForMasking(HINSTANCE hInstance, HWND hWnd, LPCWSTR pImageFileName,
	float pXPosition, float pYPosition, float pImageWidth, float pImageHeight,
	vector<PixelObject> *ppPixelObject,
	ID2D1Bitmap** ppD2DBitmapObject)
{


	HRESULT hr = S_OK;


	ofstream myfile("imagebuffer.txt");

	myfile << "Please write this text to a file.\n this text is written using C++\n";
	IWICBitmapSource* pSource = nullptr;
	IWICBitmapSource* pSecondarySource = nullptr;

	IWICBitmap* pBitmapObject = nullptr;
	IWICBitmapLock* pBitmapLock = nullptr;
	IWICBitmapLock* pBitmapLock1 = nullptr;
	IWICBitmapClipper *pIClipper = nullptr;
	BYTE* pixels = nullptr;
	WCHAR szFileName[MAX_PATH];
	WICRect rcLock = { pXPosition, pYPosition, pImageWidth, pImageHeight };
	ID2D1Image* outputImage = nullptr;
	vector <PixelObject> pixelDataVector = vector <PixelObject>();
	HDC src;
	HDC hDC;
	WICPixelFormatGUID pixelFormat;
	IWICBitmapSource* piConverter = nullptr;
	BYTE* pixelArrayPointer = nullptr;
	D2D1_RECT_F rectangle = D2D1::RectF(0, 0, pXPosition + pImageWidth, pYPosition + pImageHeight);
	UINT cbBufferSize = 0;
	BYTE* pixelDataArray = NULL;
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapDecoder* pImageDecoder = nullptr;
	IWICBitmapFrameDecode *pFrame = NULL;
	UINT uiWidth = 0;
	UINT uiHeight = 0;
	WICRect rcClip = { pXPosition, pYPosition, pImageWidth, pImageHeight };

	// Prepare iterator pairs to iterate the file content!



	hr = m_pWICFactory->CreateDecoderFromFilename(
		pImageFileName,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&pImageDecoder);




	if (SUCCEEDED(hr))
	{
		hr = pImageDecoder->GetFrame(0, &pFrame);
	}



	if (SUCCEEDED(hr))
	{
		pSource = pFrame;
		pSource->AddRef();
		pSource->GetSize(&uiWidth, &uiHeight);

		pSecondarySource = pFrame;
		pSecondarySource->AddRef();
		pSecondarySource->GetSize(&uiWidth, &uiHeight);

		IWICPalette* pImagePalette = nullptr;
		pSecondarySource->CopyPalette(pImagePalette);
	}

	WICRect rc = { pXPosition, pYPosition, pImageWidth, pImageHeight };


	if (SUCCEEDED(hr))
	{
		hr = m_pWICFactory->CreateBitmapClipper(&pIClipper);
	}


	// Initialize the clipper with the given rectangle of the frame's image data.
	if (SUCCEEDED(hr))
	{
		hr = pIClipper->Initialize(pSource, &rcClip);
	}


	if (SUCCEEDED(hr))
	{
		SafeReleaseA(&m_pConvertedSourceBitmap);
		hr = m_pWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pConvertedSourceBitmap->Initialize(
			pIClipper,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeCustom);

	}
	if (SUCCEEDED(hr))
	{
		hr = CreateDeviceResources(hInstance, hWnd);
	}
	if (SUCCEEDED(hr))
	{

		SafeReleaseA(&m_pD2DBitmap);
		hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);

	}



	if (SUCCEEDED(hr))
	{
		D2D1_SIZE_F pSize = m_pD2DBitmap->GetSize();
		pImageWidth = pSize.width;
		pImageHeight = pSize.height;

		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidColorBrush);
	}

	if (SUCCEEDED(hr))
	{


		if (DO_YOU_WANT_MESSAGES == 1)
		{
			if (DISABLE_ALL_MESSAGE_BOXES == FALSE)
			{
			}
		}
		hr = DrawD2DContentAxisAlignedClip(hInstance, hWnd, m_pD2DBitmap, rectangle);

	}


	IWICBitmapSource* pConverter = nullptr;


	if (SUCCEEDED(hr))
	{


		pSecondarySource->AddRef();
		pSecondarySource->GetSize(&uiWidth, &uiHeight);
	}

	IWICPixelFormatInfo* pixelFormatInfo = nullptr;
	if (SUCCEEDED(hr))
	{
		pSecondarySource->GetPixelFormat(&pixelFormat);
	}

	if (!IsEqualGUID(pixelFormat, GUID_WICPixelFormat128bppPRGBAFloat))
	{

		hr = WICConvertBitmapSource(GUID_WICPixelFormat128bppPRGBAFloat, pSecondarySource, &pConverter);
		IWICBitmapLock* pBitmapLock = nullptr;


		if (SUCCEEDED(hr))
		{
			pSecondarySource->Release();     // the converter has a reference to the source
			pSecondarySource = NULL;         // so we don't need it anymore.
			pSecondarySource = pConverter;   // let's treat the 128bppPABGR converter as the source
		}


		IWICPalette* pImagePalette = nullptr;
		pSecondarySource->CopyPalette(pImagePalette);
	}




	CImageFileData* pImageClass = new CImageFileData((char*)pixelArrayPointer);

	if (pConverter)
	{
		UINT cbStride = pImageWidth * sizeof(float) * 4;
		UINT cbBufferSize = cbStride;
		float *pixels = new float[cbBufferSize / sizeof(float)];
		if (pImageHeight < rc.Height && pImageWidth < rc.Width)
		{
			rc.Height = pImageHeight;
			rc.Width = pImageWidth;
		}
		else
		{
			pImageWidth = rc.Width;
			pImageHeight = rc.Height;
		}
	}


	UINT bSize = 0;




	UINT rowPitch1 = 4 * pImageWidth;
	UINT stride = 0; // stride  = bytes per row					 // (width * (bpp/8)) + padding and round to the nearest DWORD
	stride = (((((UINT)pImageWidth * 32) + 31) & ~31) >> 3);
	bSize = pImageHeight * pImageWidth;
	BYTE *pixelArray = new BYTE[(UINT)pImageHeight * stride];


	pSecondarySource->CopyPixels(NULL, stride, bSize, pixelArray);

	hDC = NULL;
	ReleaseDC(hWnd, hDC);
	hDC = GetDC(hWnd);

	COLORREF bkColor = RGB(0, 0, 0);

	SetBkColor(hDC, bkColor);

	HBITMAP map = CreateBitmap((UINT)pImageWidth,// width. 512 in my case
		(UINT)pImageHeight, // height
		1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
		8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
		(void*)pixelArray); // pointer to array
	// Temp HDC to copy picture
	src = CreateCompatibleDC(hDC); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
	SelectObject(src, map); // Inserting picture into our temp HDC
	// Copy image from temp HDC to window




	std::cout << "Pixels Begin" << endl;

	int pixelCounter = 0;


	//	pBitmapObj->GetResolution(&pImageWidth, &pImageHeight);

	UINT pYVal = 0;
	UINT pXVal = 0;
	for (pXVal = pXPosition; pXVal < pXPosition + pImageWidth; ++pXVal)
	{
		for (pYVal = pYPosition; pYVal < pYPosition + pImageHeight; ++pYVal)
			// Your code here
		{
			std::cout << "Current X Value: " << pXVal << " " << "Current Y Value: " << pYVal << endl;
			COLORREF pCurrentPixel = GetPixel(hDC, pXVal, pYVal);
			int pRedValue = GetRValue(pCurrentPixel);
			int pGreenValue = GetGValue(pCurrentPixel);
			int pBlueValue = GetBValue(pCurrentPixel);
			PixelObject pixelObj = PixelObject(pixelCounter, pXVal, pYVal, pRedValue, pGreenValue, pBlueValue);
			pixelDataVector.push_back(pixelObj);
			std::cout << "Amount of total pixels processed  = " << pixelCounter << endl;
			pixelCounter = pixelCounter + 1;

		}
	}

	std::cout << "The size of pixel buffer = " << pixelDataVector.size() << endl;
	int pAmountOfPixelsLeft = 0;
	pAmountOfPixelsLeft = pixelDataVector.size();


	fstream pixelDataFile;
	pixelDataFile.open("C:\\pixels\\ImageClipperTests.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);

	pixelDataFile.clear();
	pixelDataFile << " " << endl;
	pixelDataFile << " " << endl;
	pixelDataFile << "New Operation Begin" << endl;
	time_t currentTime = time(0);
	pixelDataFile << "current time" << currentTime << endl;











	pixelDataFile << "New Operation End" << endl;
	pixelDataFile << " " << endl;
	pixelDataFile << " " << endl;
	pixelDataFile.close();
	std::cout << "Pixels END" << endl;

	ReleaseDC(hWnd, src);
	ReleaseDC(hWnd, hDC);
	DeleteDC(src);
	DeleteDC(hDC);
	SafeReleaseA(&pDecoder);
	SafeReleaseA(&pFrame);
	SafeReleaseA(&pImageDecoder);
	myfile.close();
	*ppPixelObject = pixelDataVector;
	*ppD2DBitmapObject = m_pD2DBitmap;
	return hr;
}






HRESULT Direct2DManagementClass::CreateD2DBitmapFromFile(HINSTANCE hInstance, HWND hWnd,
	UINT pImageWidth, UINT pImageHeight,
	vector<PixelObject> *ppPixelObjectArray)
{

	ofstream myfile("imagebuffer.txt");
	myfile << "Please write this text to a file.\n this text is written using C++\n";
	HRESULT hr = S_OK;
	UINT cbBufferSize = 0;
	WICRect pWicRect;
	WCHAR szFileName[MAX_PATH];
	BYTE* pixelArrayPointer;
	
	vector<PixelObject> pixelDataArray = vector<PixelObject>();
	IWICStream* pImageReadingStream = nullptr;
	IWICBitmapSource* pSource = nullptr;
	IWICImagingFactory* pImagingFactory = nullptr;
	IWICBitmapDecoder *pDecoder = nullptr;
	IWICBitmapFrameDecode *pFrame = nullptr;
	IWICFormatConverter* pConvertedSourceBitmap = nullptr;
	ID2D1Bitmap* pBitmapObject = nullptr;
	IWICBitmap* pBitmapObj = nullptr;
	vector <PixelObject> pixelDataVector = vector <PixelObject>();

	std::cout << "Creating The Imaging Factory" << endl;


	WICRect rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	double dpiX, dpiY;
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pImagingFactory));

	std::cout << "Loading AN IMAGE FILE" << endl;

	if (LocateImageFile(hWnd, szFileName, ARRAYSIZE(szFileName)))
	{

		std::cout << "Creating An Image Decoder" << endl;


				hr = pImagingFactory->CreateDecoderFromFilename(
			szFileName,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&pDecoder);


		std::cout << "Creating An Image Stream" << endl;


		hr = pImagingFactory->CreateStream(&pImageReadingStream);

		if (SUCCEEDED(hr))
		{
			hr = pImageReadingStream->InitializeFromFilename(szFileName, GENERIC_READ);
		}

		UINT pFrameCount = 0;

		std::cout << "Getting Data About Frames" << endl;

		if (SUCCEEDED(hr))
		{
			pDecoder->GetFrameCount(&pFrameCount);
		}


		std::cout << "Locating the Image Frame" << endl;

		if (SUCCEEDED(hr) && pFrameCount > 0)
		{
			hr = pDecoder->GetFrame(0, &pFrame);

		}



		if (SUCCEEDED(hr))
		{

			std::cout << "Creating the Wic Bitmap From Image FRAME" << endl;

			hr = pImagingFactory->CreateBitmapFromSource(pFrame,
				WICBitmapCacheOnDemand,
				&pBitmapObj);
			IWICPalette* pImagePalette = nullptr;
			pBitmapObj->CopyPalette(pImagePalette);
		}


		pBitmapObj->GetResolution(&dpiX, &dpiY);

		UINT pWidth = NULL;
		UINT pHeight = NULL;
		pBitmapObj->GetSize(&pWidth, &pHeight);
		rc = { 0.0f, 0.0f, pImageWidth, pImageHeight };


		std::cout << "Creating the Format Converter" << endl;
		if (SUCCEEDED(hr))
		{
			hr = pImagingFactory->CreateFormatConverter(&pConvertedSourceBitmap);
		}

		std::cout << "Configuring the Format Converter" << endl;
		hr = pConvertedSourceBitmap->Initialize(
			pFrame,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeCustom);

	}


	if (SUCCEEDED(hr))
	{

		hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(pConvertedSourceBitmap, NULL, &pBitmapObject);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidColorBrush);
	}

	if (SUCCEEDED(hr))
	{
		hr = DrawD2DContent(hInstance, hWnd, pBitmapObject);
	}



	IWICBitmapSource* pConverter = nullptr;
	WICPixelFormatGUID pixelFormat;

	if (SUCCEEDED(hr))
	{
		UINT pWidth = 0;
		UINT pHeight = 0;
		pSource = pFrame;
		pSource->AddRef();
		pSource->GetSize(&pWidth, &pHeight);
	}
	IWICPixelFormatInfo* pixelFormatInfo = nullptr;
	std::cout << "Getting Image Pixel Format" << endl;

	if (SUCCEEDED(hr))
	{
		pSource->GetPixelFormat(&pixelFormat);
	}



	if (!IsEqualGUID(pixelFormat, GUID_WICPixelFormat128bppPRGBAFloat))
	{
		std::cout << "Converting Pixel Format" << endl;


		hr = WICConvertBitmapSource(GUID_WICPixelFormat128bppPRGBAFloat, pSource, &pConverter);
		IWICBitmapLock* pBitmapLock = nullptr;


		if (SUCCEEDED(hr))
		{
			pSource->Release();     // the converter has a reference to the source
			pSource = NULL;         // so we don't need it anymore.
			pSource = pConverter;   // let's treat the 128bppPABGR converter as the source
		}
	

		IWICPalette* pImagePalette = nullptr;
		pSource->CopyPalette(pImagePalette);
	}
		
	UINT iWidth = 0;
	UINT iHeight = 0;
	std::cout << "Getting Image Size" << endl;
	pBitmapObj->GetSize(&iWidth, &iHeight);



	if (pConverter)
	{
		UINT cbStride = pImageWidth * sizeof(float) * 4;
		UINT cbBufferSize = cbStride;

		float *pixels = new float[cbBufferSize / sizeof(float)];







		UINT rowPitch1 = 4 * pImageWidth;
		UINT stride = 0; // stride  = bytes per row					 // (width * (bpp/8)) + padding and round to the nearest DWORD
		stride = (((((UINT)pImageWidth * 32) + 31) & ~31) >> 3);
		UINT bSize = pImageHeight * pImageWidth;
		BYTE *pixelArray = new BYTE[bSize];
		std::cout << "Copying Image Pixels" << endl;
		pSource->CopyPixels(NULL, stride, bSize, pixelArray);
		HDC hDC = GetDC(hWnd);


		std::cout << "Creating the HBITMAP Object" << endl;

		HBITMAP map = CreateBitmap((UINT)pImageWidth,// width. 512 in my case
			(UINT)pImageHeight, // height
			1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
			8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
			(void*)pixelArray); // pointer to array
		// Temp HDC to copy picture
		std::cout << "Creating the Device Context for Drawing" << endl;
		HDC src = CreateCompatibleDC(hDC); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
		SelectObject(src, map); // Inserting picture into our temp HDC
		// Copy image from temp HDC to window

		std::cout << "Copying Image Pixel Data" << endl;


		std::cout << "Pixels Begin" << endl;

		int pixelCounter = 0;


		//	pBitmapObj->GetResolution(&pWidth, &pHeight);

		std::cout << "We will select a rect area with size 20x20 " << endl;
		for (UINT pXVal = 0; pXVal <= pImageWidth - 1; ++pXVal)
		{
			for (UINT pYVal = 0; pYVal <= pImageHeight - 1; ++pYVal)
			{
				COLORREF pCurrentPixel = GetPixel(hDC, pXVal, pYVal);
				int pRedValue = GetRValue(pCurrentPixel);
				int pGreenValue = GetGValue(pCurrentPixel);
				int pBlueValue = GetBValue(pCurrentPixel);
				PixelObject pixelObj = PixelObject(pixelCounter, pXVal, pYVal, pRedValue, pGreenValue, pBlueValue);
				pixelDataVector.push_back(pixelObj);
				std::cout << "Amount of total pixels processed  = " << pixelCounter << endl;
				pixelCounter++;
			}

		}

		std::cout << "The size of pixel buffer = " << pixelDataVector.size() << endl;
		int pAmountOfPixelsLeft = 0;
		pAmountOfPixelsLeft = pixelDataVector.size();


		ofstream pixelDataFile;
		fstream secondStream;
		pixelDataFile.open("C:\\pixels\\output_pixels.csv");
		secondStream.open("C:\\pixels\\pixels5.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
		pixelDataFile.clear();
		pixelDataFile << "Pixel Number" << "X" << "Y" << "RED" << "GREEN" << "BLUE" << "\n";
		secondStream << "Image Start:" << endl;


		for (PixelObject currentPixel : pixelDataVector)
		{


			std::cout << "PixelNumber  = " << currentPixel.pixelNumber <<
				"X Position = " << currentPixel.xPosition <<
				"Y Position = " << currentPixel.yPosition <<
				"The Value Of Red = " << currentPixel.RMask <<
				"The Value Of Green = " << currentPixel.GMask << endl;


			pixelDataFile << currentPixel.pixelNumber << "," << " " <<
				currentPixel.xPosition << "," << " " <<
				currentPixel.yPosition << "," << " " <<
				currentPixel.RMask << "," << " " <<
				currentPixel.GMask << "," << " " <<
				currentPixel.BMask << '\n';
			pAmountOfPixelsLeft--;



			secondStream << "PixelNumber  = " << currentPixel.pixelNumber << endl;
			secondStream << "X Position = " << currentPixel.xPosition << endl;
			secondStream << "Y Position = " << currentPixel.yPosition << endl;
			secondStream << "The Value Of Red = " << currentPixel.RMask << endl;
			secondStream << "The Value Of Green = " << currentPixel.GMask << endl;
			secondStream << "The Value Of Blue = " << currentPixel.BMask << endl;


		}







		pixelDataFile << "New Operation End" << endl;
		pixelDataFile << " " << endl;
		pixelDataFile << " " << endl;
		pixelDataFile.close();
		std::cout << "Pixels END" << endl;
		secondStream << "Image End:" << endl;
		secondStream.close();



	}

	SafeReleaseA(&pDecoder);
	myfile.close();
	*ppPixelObjectArray = pixelDataVector;

	return hr;
}



HRESULT Direct2DManagementClass::CreateD2DBitmapFromFileSpecified(HINSTANCE hInstance, HWND hWnd, LPCWSTR pImageFileName,
	UINT pImageWidth, UINT pImageHeight,
	vector<PixelObject> *ppPixelObjectArray)
{

	ofstream myfile("imagebuffer.txt");
	myfile << "Please write this text to a file.\n this text is written using C++\n";
	HRESULT hr = S_OK;
	UINT cbBufferSize = 0;
	WICRect pWicRect;
	WCHAR szFileName[MAX_PATH];
	BYTE* pixelArrayPointer;

	vector<PixelObject> pixelDataArray = vector<PixelObject>();
	IWICStream* pImageReadingStream = nullptr;
	IWICBitmapSource* pSource = nullptr;
	IWICImagingFactory* pImagingFactory = nullptr;
	IWICBitmapDecoder *pDecoder = nullptr;
	IWICBitmapFrameDecode *pFrame = nullptr;
	IWICFormatConverter* pConvertedSourceBitmap = nullptr;
	ID2D1Bitmap* pBitmapObject = nullptr;
	IWICBitmap* pBitmapObj = nullptr;
	vector <PixelObject> pixelDataVector = vector <PixelObject>();

	std::cout << "Creating The Imaging Factory" << endl;


	WICRect rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	double dpiX, dpiY;
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pImagingFactory));

	std::cout << "Loading AN IMAGE FILE" << endl;

	

		std::cout << "Creating An Image Decoder" << endl;


hr = pImagingFactory->CreateDecoderFromFilename(
			pImageFileName,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&pDecoder);


		std::cout << "Creating An Image Stream" << endl;


		hr = pImagingFactory->CreateStream(&pImageReadingStream);

		if (SUCCEEDED(hr))
		{
			hr = pImageReadingStream->InitializeFromFilename(szFileName, GENERIC_READ);
		}

		UINT pFrameCount = 0;

		std::cout << "Getting Data About Frames" << endl;

		if (SUCCEEDED(hr))
		{
			pDecoder->GetFrameCount(&pFrameCount);
		}


		std::cout << "Locating the Image Frame" << endl;

		if (SUCCEEDED(hr) && pFrameCount > 0)
		{
			hr = pDecoder->GetFrame(0, &pFrame);

		}



		if (SUCCEEDED(hr))
		{

			std::cout << "Creating the Wic Bitmap From Image FRAME" << endl;

			hr = pImagingFactory->CreateBitmapFromSource(pFrame,
				WICBitmapCacheOnDemand,
				&pBitmapObj);
			IWICPalette* pImagePalette = nullptr;
			pBitmapObj->CopyPalette(pImagePalette);
		}


		pBitmapObj->GetResolution(&dpiX, &dpiY);

		UINT pWidth = NULL;
		UINT pHeight = NULL;
		pBitmapObj->GetSize(&pWidth, &pHeight);
		rc = { 0.0f, 0.0f, pImageWidth, pImageHeight };


		std::cout << "Creating the Format Converter" << endl;
		if (SUCCEEDED(hr))
		{
			hr = pImagingFactory->CreateFormatConverter(&pConvertedSourceBitmap);
		}

		std::cout << "Configuring the Format Converter" << endl;
		hr = pConvertedSourceBitmap->Initialize(
			pFrame,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeCustom);

	


	if (SUCCEEDED(hr))
	{

		hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(pConvertedSourceBitmap, NULL, &pBitmapObject);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidColorBrush);
	}

	if (SUCCEEDED(hr))
	{
		hr = DrawD2DContent(hInstance, hWnd, pBitmapObject);
	}



	IWICBitmapSource* pConverter = nullptr;
	WICPixelFormatGUID pixelFormat;

	if (SUCCEEDED(hr))
	{
		UINT pWidth = 0;
		UINT pHeight = 0;
		pSource = pFrame;
		pSource->AddRef();
		pSource->GetSize(&pWidth, &pHeight);
	}
	IWICPixelFormatInfo* pixelFormatInfo = nullptr;
	std::cout << "Getting Image Pixel Format" << endl;

	if (SUCCEEDED(hr))
	{
		pSource->GetPixelFormat(&pixelFormat);
	}



	if (!IsEqualGUID(pixelFormat, GUID_WICPixelFormat128bppPRGBAFloat))
	{
		std::cout << "Converting Pixel Format" << endl;


		hr = WICConvertBitmapSource(GUID_WICPixelFormat128bppPRGBAFloat, pSource, &pConverter);
		IWICBitmapLock* pBitmapLock = nullptr;


		if (SUCCEEDED(hr))
		{
			pSource->Release();     // the converter has a reference to the source
			pSource = NULL;         // so we don't need it anymore.
			pSource = pConverter;   // let's treat the 128bppPABGR converter as the source
		}


		IWICPalette* pImagePalette = nullptr;
		pSource->CopyPalette(pImagePalette);
	}

	UINT iWidth = 0;
	UINT iHeight = 0;
	std::cout << "Getting Image Size" << endl;
	pBitmapObj->GetSize(&iWidth, &iHeight);



	if (pConverter)
	{
		UINT cbStride = pImageWidth * sizeof(float) * 4;
		UINT cbBufferSize = cbStride;

		float *pixels = new float[cbBufferSize / sizeof(float)];







		UINT rowPitch1 = 4 * pImageWidth;
		UINT stride = 0; // stride  = bytes per row					 // (width * (bpp/8)) + padding and round to the nearest DWORD
		stride = (((((UINT)pImageWidth * 32) + 31) & ~31) >> 3);
		UINT bSize = pImageHeight * pImageWidth;
		BYTE *pixelArray = new BYTE[bSize];
		std::cout << "Copying Image Pixels" << endl;
		pSource->CopyPixels(NULL, stride, bSize, pixelArray);
		HDC hDC = GetDC(hWnd);


		std::cout << "Creating the HBITMAP Object" << endl;

		HBITMAP map = CreateBitmap((UINT)pImageWidth,// width. 512 in my case
			(UINT)pImageHeight, // height
			1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
			8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
			(void*)pixelArray); // pointer to array
		// Temp HDC to copy picture
		std::cout << "Creating the Device Context for Drawing" << endl;
		HDC src = CreateCompatibleDC(hDC); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
		SelectObject(src, map); // Inserting picture into our temp HDC
		// Copy image from temp HDC to window

		std::cout << "Copying Image Pixel Data" << endl;


		std::cout << "Pixels Begin" << endl;

		int pixelCounter = 0;


		//	pBitmapObj->GetResolution(&pWidth, &pHeight);

		std::cout << "We will select a rect area with size 20x20 " << endl;
		for (UINT pXVal = 0; pXVal <= pImageWidth - 1; ++pXVal)
		{
			for (UINT pYVal = 0; pYVal <= pImageHeight - 1; ++pYVal)
			{
				COLORREF pCurrentPixel = GetPixel(hDC, pXVal, pYVal);
				int pRedValue = GetRValue(pCurrentPixel);
				int pGreenValue = GetGValue(pCurrentPixel);
				int pBlueValue = GetBValue(pCurrentPixel);
				PixelObject pixelObj = PixelObject(pixelCounter, pXVal, pYVal, pRedValue, pGreenValue, pBlueValue);
				pixelDataVector.push_back(pixelObj);
				std::cout << "Amount of total pixels processed  = " << pixelCounter << endl;
				pixelCounter++;
			}

		}

		std::cout << "The size of pixel buffer = " << pixelDataVector.size() << endl;
		int pAmountOfPixelsLeft = 0;
		pAmountOfPixelsLeft = pixelDataVector.size();


		ofstream pixelDataFile;
		fstream secondStream;
		pixelDataFile.open("C:\\pixels\\output_pixels.csv");
		secondStream.open("C:\\pixels\\pixels5.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
		pixelDataFile.clear();
		pixelDataFile << "Pixel Number" << "X" << "Y" << "RED" << "GREEN" << "BLUE" << "\n";
		secondStream << "Image Start:" << endl;


		for (PixelObject currentPixel : pixelDataVector)
		{


			std::cout << "PixelNumber  = " << currentPixel.pixelNumber <<
				"X Position = " << currentPixel.xPosition <<
				"Y Position = " << currentPixel.yPosition <<
				"The Value Of Red = " << currentPixel.RMask <<
				"The Value Of Green = " << currentPixel.GMask << endl;


			pixelDataFile << currentPixel.pixelNumber << "," << " " <<
				currentPixel.xPosition << "," << " " <<
				currentPixel.yPosition << "," << " " <<
				currentPixel.RMask << "," << " " <<
				currentPixel.GMask << "," << " " <<
				currentPixel.BMask << '\n';
			pAmountOfPixelsLeft--;



			secondStream << "PixelNumber  = " << currentPixel.pixelNumber << endl;
			secondStream << "X Position = " << currentPixel.xPosition << endl;
			secondStream << "Y Position = " << currentPixel.yPosition << endl;
			secondStream << "The Value Of Red = " << currentPixel.RMask << endl;
			secondStream << "The Value Of Green = " << currentPixel.GMask << endl;
			secondStream << "The Value Of Blue = " << currentPixel.BMask << endl;


		}







		pixelDataFile << "New Operation End" << endl;
		pixelDataFile << " " << endl;
		pixelDataFile << " " << endl;
		pixelDataFile.close();
		std::cout << "Pixels END" << endl;
		secondStream << "Image End:" << endl;
		secondStream.close();



	}

	SafeReleaseA(&pDecoder);
	myfile.close();
	*ppPixelObjectArray = pixelDataVector;

	return hr;
}

HRESULT Direct2DManagementClass::CreateD2DBitmapFromFileSpecified(HINSTANCE hInstance, HWND hWnd, LPCWSTR pImageFileName,
	UINT pImageWidth, UINT pImageHeight,
	ID2D1Bitmap** ppBitmapToDrawD2D)
{

	ofstream myfile("imagebuffer.txt");
	myfile << "Please write this text to a file.\n this text is written using C++\n";
	HRESULT hr = S_OK;
	UINT cbBufferSize = 0;
	WICRect pWicRect;
	WCHAR szFileName[MAX_PATH];
	BYTE* pixelArrayPointer;

	vector<PixelObject> pixelDataArray = vector<PixelObject>();
	IWICStream* pImageReadingStream = nullptr;
	IWICBitmapSource* pSource = nullptr;
	IWICImagingFactory* pImagingFactory = nullptr;
	IWICBitmapDecoder *pDecoder = nullptr;
	IWICBitmapFrameDecode *pFrame = nullptr;
	IWICFormatConverter* pConvertedSourceBitmap = nullptr;
	ID2D1Bitmap* pBitmapObject = nullptr;
	IWICBitmap* pBitmapObj = nullptr;
	vector <PixelObject> pixelDataVector = vector <PixelObject>();

	std::cout << "Creating The Imaging Factory" << endl;


	WICRect rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	double dpiX, dpiY;
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pImagingFactory));

	std::cout << "Loading AN IMAGE FILE" << endl;



	std::cout << "Creating An Image Decoder" << endl;


	hr = pImagingFactory->CreateDecoderFromFilename(
		pImageFileName,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&pDecoder);


	std::cout << "Creating An Image Stream" << endl;


	hr = pImagingFactory->CreateStream(&pImageReadingStream);

	if (SUCCEEDED(hr))
	{
		hr = pImageReadingStream->InitializeFromFilename(szFileName, GENERIC_READ);
	}

	UINT pFrameCount = 0;

	std::cout << "Getting Data About Frames" << endl;

	if (SUCCEEDED(hr))
	{
		pDecoder->GetFrameCount(&pFrameCount);
	}


	std::cout << "Locating the Image Frame" << endl;

	if (SUCCEEDED(hr) && pFrameCount > 0)
	{
		hr = pDecoder->GetFrame(0, &pFrame);

	}



	if (SUCCEEDED(hr))
	{

		std::cout << "Creating the Wic Bitmap From Image FRAME" << endl;

		hr = pImagingFactory->CreateBitmapFromSource(pFrame,
			WICBitmapCacheOnDemand,
			&pBitmapObj);
		IWICPalette* pImagePalette = nullptr;
		pBitmapObj->CopyPalette(pImagePalette);
	}


	pBitmapObj->GetResolution(&dpiX, &dpiY);

	UINT pWidth = NULL;
	UINT pHeight = NULL;
	pBitmapObj->GetSize(&pWidth, &pHeight);
	rc = { 0.0f, 0.0f, pImageWidth, pImageHeight };


	std::cout << "Creating the Format Converter" << endl;
	if (SUCCEEDED(hr))
	{
		hr = pImagingFactory->CreateFormatConverter(&pConvertedSourceBitmap);
	}

	std::cout << "Configuring the Format Converter" << endl;
	hr = pConvertedSourceBitmap->Initialize(
		pFrame,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.f,
		WICBitmapPaletteTypeCustom);




	if (SUCCEEDED(hr))
	{

		hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(pConvertedSourceBitmap, NULL, &pBitmapObject);
	}

	*ppBitmapToDrawD2D = pBitmapObject;
	return hr;
}

	


HRESULT Direct2DManagementClass::RenderBitmapWithLayers(HINSTANCE hInstance, HWND hWnd, ID2D1Bitmap* pBitmapToDraw)
{
	HRESULT hr;
	PAINTSTRUCT ps;

	ID2D1SolidColorBrush* pRedSolidColorBrush = nullptr;
	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidColorBrush);

	if (BeginPaint(hWnd, &ps))
	{
		if (m_pD2DDeviceContext == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}
		m_pD2DDeviceContext1->BeginDraw();
	
		ID2D1Layer* pLayerObject = nullptr;
		hr = m_pD2DDeviceContext1->CreateLayer(NULL, &pLayerObject);
		m_pD2DDeviceContext1->SetTransform(D2D1::Matrix3x2F::Translation(300, 250));

		// Push the layer with the content bounds.
		m_pD2DDeviceContext1->PushLayer(
			D2D1::LayerParameters(
			D2D1::InfiniteRect(),
			NULL,
			D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,
			D2D1::IdentityMatrix(),
			1.0,
			m_pRadialGradientBrush,
			D2D1_LAYER_OPTIONS_NONE),
			pLayerObject
			);

		

		m_pD2DDeviceContext1->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::Black));

		D2D1_SIZE_F rtSize = m_pD2DDeviceContext1->GetSize();
		D2D1_RECT_F rectangle = D2D1::RectF(0, 0, rtSize.width, rtSize.height);


		if (pBitmapToDraw)
		{
			m_pD2DDeviceContext1->DrawBitmap(pBitmapToDraw, rectangle);
			m_pD2DDeviceContext1->FillRectangle(
				D2D1::RectF(25.f, 25.f, 50.f, 50.f),
				pRedSolidColorBrush
				);
			m_pD2DDeviceContext1->FillRectangle(
				D2D1::RectF(50.f, 50.f, 75.f, 75.f),
				pRedSolidColorBrush
				);
			m_pD2DDeviceContext1->FillRectangle(
				D2D1::RectF(75.f, 75.f, 100.f, 100.f),
				pRedSolidColorBrush
				);
		}
		m_pD2DDeviceContext1->PopLayer();

		



		hr = m_pD2DDeviceContext1->EndDraw();
		

		if (hr == D2DERR_RECREATE_TARGET)
		{
			SafeReleaseA(&m_pD2DBitmap);
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}
		pSwapChainObjectD2D->Present(0, 0);
		EndPaint(hWnd, &ps);
	}
	return SUCCEEDED(hr) ? 0 : 1;
}


HRESULT Direct2DManagementClass::CreateBitmapWithASetOfLayers(HINSTANCE hInstance, HWND hWnd)
{

	vector<PixelObject> pixelDataArray = vector<PixelObject>();
	ofstream myfile("imagebuffer.txt");
	myfile << "Please write this text to a file.\n this text is written using C++\n";
	IWICBitmap* pBitmapObject = nullptr;
	IWICBitmapLock* pBitmapLock = nullptr;
	HRESULT hr = S_OK;
	WCHAR szFileName[MAX_PATH];
	UINT cbBufferSize = 0;
	UINT cbStride = 0;
	BYTE  *pv;

	
	BYTE* pixelArrayPointer;

	hInstance = GetModuleHandle(NULL);
	if (LocateImageFile(hWnd, szFileName, ARRAYSIZE(szFileName)))
	{

		std::ifstream file(szFileName, ios::binary);
		// Prepare iterator pairs to iterate the file content!
		std::istream_iterator<unsigned char> begin(file), end;
		// Reading the file content using the iterator!
		std::vector<unsigned char> buffer(begin, end);

		myfile << buffer.data();

		IWICBitmapDecoder *pDecoder = NULL;
		hr = m_pWICFactory->CreateDecoderFromFilename(
			szFileName,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&pDecoder);

		hr = m_pWICFactory->CreateStream(&pImageReadingStream);

		if (SUCCEEDED(hr))
		{
			hr = pImageReadingStream->InitializeFromFilename(szFileName, GENERIC_READ);
		}



		IWICBitmapFrameDecode *pFrame = NULL;

		if (SUCCEEDED(hr))
		{
			hr = pDecoder->GetFrame(0, &pFrame);
		}

		UINT uiWidth = 0;
		UINT uiHeight = 0;
		pFrame->GetSize(&uiWidth, &uiHeight);
		WICRect rcLock = { 0, 0, uiWidth, uiHeight };


		if (SUCCEEDED(hr))
		{



			hr = m_pWICFactory->CreateBitmapFromSource(pFrame,
				WICBitmapCacheOnDemand,
				&pBitmapObject);
			IWICPalette* pImagePalette = nullptr;
			pBitmapObject->CopyPalette(pImagePalette);


		}
		if (SUCCEEDED(hr))
		{
			hr = pBitmapObject->Lock(&rcLock, WICBitmapLockWrite, &pBitmapLock);
		}



		if (SUCCEEDED(hr))
		{

			hr = pBitmapLock->GetDataPointer(&cbBufferSize, &pixelArrayPointer);

		}

		hr = pBitmapLock->GetStride(&cbStride);

		if (SUCCEEDED(hr))
		{
			hr = pBitmapLock->GetDataPointer(&cbBufferSize, &pv);
		}

		pixelArrayPointer = &pv[0];


		vector<BYTE*> pixelVector = vector<BYTE*>();
		pixelVector.insert(pixelVector.end(), pixelArrayPointer);

		for (int pIndex = 0; pIndex < sizeof(pv); ++pIndex)

		{

			float pixelX = (float)pv[pIndex];
			float pixelY = (float)pv[pIndex + 1];
			float pixelR = (float)pv[pIndex + 2];
			float pixelG = (float)pv[pIndex + 3];
			float pixelB = (float)pv[pIndex + 4];

			PixelObject pNewPixelObject = PixelObject(pIndex, pixelX,
				pixelY,
				pixelR,
				pixelG,
				pixelB);

			std::cout << "Pixel Id = " << pNewPixelObject.pixelNumber <<
				"Pixel X = " << pNewPixelObject.xPosition <<
				"Pixel Y = " << pNewPixelObject.yPosition <<
				"Pixel R = " << pNewPixelObject.RMask <<
				"Pixel G = " << pNewPixelObject.GMask <<
				"Pixel B = " << pNewPixelObject.BMask << endl;
			

		}

		char* pOuputFile = "C:\\reports\\reports.txt";
	//	std::fstream fs;
	//	fs.open(pOuputFile, std::fstream::in | std::fstream::out | std::fstream::app);
	//	time_t rawtime;
	//	struct tm * timeinfo;

	//	time(&rawtime);
	//	timeinfo = localtime(&rawtime);
	//	fs << endl;
	//	fs << (char*)timeinfo << endl;
	//	fs << (char*)"Start of new Record\n" << endl;
	//	fs << (char*)pixelDataArray << endl;
	//	time(&rawtime);
	//	timeinfo = localtime(&rawtime);
	//	fs << (char*)"end of record" << (char*)timeinfo << endl;
	//	fs.close();

		ofstream outFile(pOuputFile);
		time_t rawtime;
		struct tm * timeinfo;

			time(&rawtime);
			timeinfo = localtime(&rawtime);
		
			outFile  << (char*)timeinfo << endl;
			outFile << (char*)"Start of new Record\n" << endl;
			
			time(&rawtime);
			timeinfo = localtime(&rawtime);
			outFile << (char*)timeinfo << endl;
			outFile << (char*)"end of record \n" << endl;
		outFile.close();
		if (SUCCEEDED(hr))
		{
			SafeReleaseA(&m_pConvertedSourceBitmap);
			hr = m_pWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pConvertedSourceBitmap->Initialize(
				pFrame,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeCustom);

		}
		if (SUCCEEDED(hr))
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}
		if (SUCCEEDED(hr))
		{
			
			SafeReleaseA(&m_pD2DBitmap);
			hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);
		}
		if (SUCCEEDED(hr))
		{
			D2D1_GRADIENT_STOP gradientStops[2];
			gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::Red, 1);
			gradientStops[0].position = 0.0f;
			gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Red, 1);
			gradientStops[1].position = 1.0f;
			// Create the ID2D1GradientStopCollection from a previously
			// declared array of D2D1_GRADIENT_STOP structs.
			hr = m_pD2DDeviceContext1->CreateGradientStopCollection(
				gradientStops,
				2,
				D2D1_GAMMA_2_2,
				D2D1_EXTEND_MODE_CLAMP,
				&pGradientStops
				);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pD2DDeviceContext1->CreateRadialGradientBrush(
				D2D1::RadialGradientBrushProperties(
				D2D1::Point2F(75, 75),
				D2D1::Point2F(0, 0),
				75,
				75),
				pGradientStops,
				&m_pRadialGradientBrush
				);
		}


		if (SUCCEEDED(hr))
		{
			hr = RenderBitmapWithLayers(hInstance, hWnd, m_pD2DBitmap);
		}

		SafeReleaseA(&pDecoder);
		SafeReleaseA(&pFrame);
		myfile.close();
	}
	return hr;
}




HRESULT Direct2DManagementClass::CreateDesktopIcon(HWND hWnd, LPCSTR fName)
{
	HRESULT hr = S_OK;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WCHAR szFileName[MAX_PATH];

		IWICBitmapDecoder *pDecoder = NULL;
		hr = m_pWICFactory->CreateDecoderFromFilename(
			(LPCWSTR)fName,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&pDecoder);

		IWICBitmapFrameDecode *pFrame = NULL;

		if (SUCCEEDED(hr))
		{
			hr = pDecoder->GetFrame(0, &pFrame);
		}

		if (SUCCEEDED(hr))
		{
			SafeReleaseA(&m_pConvertedSourceBitmap);
			hr = m_pWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pConvertedSourceBitmap->Initialize(
				pFrame,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeCustom);
		}
		if (SUCCEEDED(hr))
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}
		if (SUCCEEDED(hr))
		{
			SafeReleaseA(&m_pD2DBitmap);
			hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);
		}
		if (SUCCEEDED(hr))
		{
			hr = DrawD2DContent(hInstance, hWnd, m_pD2DBitmap);
		}

		SafeReleaseA(&pDecoder);
		SafeReleaseA(&pFrame);
		return hr;
	}
	





LPWSTR Direct2DManagementClass::LocateImageFile(HWND hWnd, LPWSTR pszFileName, DWORD cbFileName)
{
	pszFileName[0] = L'\0';
	OPENFILENAME ofn;	
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = L"All Image Files\0"              L"*.bmp;*.dib;*.wdp;*.mdp;*.hdp;*.gif;*.png;*.jpg;*.jpeg;*.tif;*.ico\0"
					  L"Windows Bitmap\0"               L"*.bmp;*.dib\0"
					  L"High Definition Photo\0"        L"*.wdp;*.mdp;*.hdp\0"
		              L"Graphics Interchange Format\0"  L"*.gif\0"
		              L"Portable Network Graphics\0"    L"*.png\0"
					  L"JPEG File Interchange Format\0" L"*.jpg;*.jpeg\0"
		              L"Tiff File\0"                    L"*.tif\0"
		              L"Icon\0"                         L"*.ico\0"
		              L"All Files\0"                    L"*.*\0"
		              L"\0";
	ofn.lpstrFile = pszFileName;
	ofn.nMaxFile = cbFileName;
	ofn.lpstrTitle = L"Open Image";
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	GetOpenFileName(&ofn);
	pszFileName = ofn.lpstrFile;
	return pszFileName;
}

void Direct2DManagementClass::ProcessWindowResize(HWND hWnd)
{
	if (!m_pD2DDeviceContext1)
	{
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::Aqua));
	}
}




// Save render target bitmap to a stream using WIC.
HRESULT Direct2DManagementClass::SaveBitmapToStream(
	HINSTANCE hInstance,
	HWND hWnd,
	LPCWSTR pFileName,
	UINT pImageWidth,
	UINT pImageHeight,
	IWICBitmapSource* pImageToSave)
{
	HRESULT hr = S_OK;
	// Create and initialize WIC Bitmap Encoder.
	IWICBitmapEncoder* wicBitmapEncoder = nullptr;
	IWICBitmapFrameEncode* pEncodedFrame = nullptr;
	IPropertyBag2* pPropertyBag = nullptr;
	IWICStream* pImageStream = nullptr;
	UINT uiWidth = pImageWidth;
	UINT uiHeight = pImageHeight;
	GUID containerFormat = GUID_ContainerFormatJpeg;


	




	IWICStream* pStream = nullptr;
	m_pWICFactory->CreateStream(&pStream);
	pStream->InitializeFromFilename(pFileName, GENERIC_WRITE);


	WICRect wicRC = { 0.0f, 0.0f, pImageWidth, pImageHeight };
	std::cout << "Creating Image Encoder Object" << endl;

	IWICStream* pStreamObject = nullptr;

	GUID wicFormat = GUID_WICPixelFormat32bppPBGRA;
		pImageToSave->GetPixelFormat(&wicFormat);

	
	std::cout << "Creating Encoder Object" << endl;
	
	hr = m_pWICFactory->CreateEncoder(GUID_ContainerFormatJpeg,
		NULL,
		&wicBitmapEncoder);


	if (FAILED(hr))
	{
		int pLastError = GetLastError();
		MessageBoxA(hWnd, (LPCSTR)"Не удалось создать энкодер изображений WIC", (LPCSTR)"Message", 0);
		std::cout << "Unable To Create Image Encoder Object" << endl;
		return hr;
	}
	
	std::cout << "Creating a stream Object" << endl;
	
	wicBitmapEncoder->Initialize(pStream, WICBitmapEncoderNoCache);

	hr = m_pWICFactory->CreateStream(&pStreamObject);

	if (FAILED(hr))
	{
		int pLastError = GetLastError();
		std::cout << "Unable To Create Image Encoder Stream Object" << endl;
		return hr;
	}
	

	std::cout << "Initializing From File Name" << endl;
	hr = pStreamObject->InitializeFromFilename(pFileName,
		GENERIC_WRITE);


	if (FAILED(hr))
	{
		int pLastError = GetLastError();
		std::cout << "Unable To Initialize Image Encoder Stream Object" << endl;
		return hr;
	}
	
	std::cout << "Creating New Frame Buffer" << endl;

	hr = wicBitmapEncoder->CreateNewFrame(&pEncodedFrame, NULL);

	

	if (SUCCEEDED(hr))
	{
		pEncodedFrame->Initialize(NULL);
	}
	
	if (SUCCEEDED(hr))
	{
		hr = pEncodedFrame->SetSize(pImageWidth, pImageHeight);
	}


	if (SUCCEEDED(hr))
	{
		hr = pEncodedFrame->SetPixelFormat(&wicFormat);
	}

	

	

	if (SUCCEEDED(hr))
	{
		hr = pEncodedFrame->WriteSource(pImageToSave, NULL);

	}
	
	else
	{
		hr = E_OUTOFMEMORY;
	}
	

	if (SUCCEEDED(hr))
	{
		hr = pEncodedFrame->Commit();
	}

	if (SUCCEEDED(hr))
	{
		hr = wicBitmapEncoder->Commit();
	}


	

	return hr;


}




HRESULT Direct2DManagementClass::CaptureImageToFile(HINSTANCE hInstance, HWND hWnd, HBITMAP pBitmapObject, LPCWSTR filename)
{

	HRESULT hr = S_OK;
	BOOL pResult = TRUE;
	HDC hMemDC = NULL;
	HBITMAP hbmScreen = NULL;
	BITMAP bmpScreen;
	HDC hdScreen = GetDC(NULL);
	HDC hdcWindow = GetDC(hWnd);


	hMemDC = CreateCompatibleDC(hdcWindow);
	if (!hMemDC)
	{
		MessageBox(hWnd, L"CreateCompatibleDC has failed", L"Failed", MB_OK);
		std::cout << "Unable to get desktop pointer" << endl;
		DeleteObject(hbmScreen);
		DeleteObject(hMemDC);
		ReleaseDC(NULL, hdScreen);
		ReleaseDC(hWnd, hdcWindow);
		return hr;
	}
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);
	UINT width = rcClient.right - rcClient.left;
	UINT height = rcClient.bottom - rcClient.top;
	SetStretchBltMode(hdcWindow, HALFTONE);


	HBITMAP hOldBitmap = pBitmapObject;

	pResult = BitBlt(hMemDC,
		0, 0,
		rcClient.right - rcClient.left,
		rcClient.bottom - rcClient.top,
		hdcWindow,
		0, 0,
		SRCCOPY
		);

	if (!pResult)
	{
		MessageBox(hWnd, L"BitBlt has failed", L"Failed", MB_OK);
		std::cout << "BitBlt has failed" << endl;
	}


	GetObject(pBitmapObject, sizeof(BITMAP), &bmpScreen);
	BITMAPINFOHEADER bmpHeader;
	BITMAPFILEHEADER bmfHeader;
	bmpHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpHeader.biWidth = bmpScreen.bmWidth;
	bmpHeader.biHeight = bmpScreen.bmHeight;
	bmpHeader.biPlanes = 1;
	bmpHeader.biBitCount = 32;
	bmpHeader.biCompression = BI_RGB;
	bmpHeader.biSizeImage = 0;
	bmpHeader.biXPelsPerMeter = 0;
	bmpHeader.biYPelsPerMeter = 0;
	bmpHeader.biClrUsed = 0;
	bmpHeader.biClrImportant = 0;
	DWORD dwBmpSize = ((bmpScreen.bmWidth * bmpHeader.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;
	HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);

	char *lpbitmap = (char *)GlobalLock(hDIB);
	// Gets the "bits" from the bitmap and copies them into a buffer
	// which is pointed to by lpbitmap.

	GetDIBits(hdcWindow, pBitmapObject, 0,
		(UINT)bmpScreen.bmHeight,
		lpbitmap,
		(BITMAPINFO *)&bmpHeader, DIB_RGB_COLORS);
	// A file is created, this is where we will save the screen capture.

	HANDLE hFile = CreateFile(filename,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);

	// Add the size of the headers to the size of the bitmap to get the total file size
	DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	//Offset to where the actual bitmap bits start.
	bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
	//Size of the file
	bmfHeader.bfSize = dwSizeofDIB;
	//bfType must always be BM for Bitmaps
	bmfHeader.bfType = 0x4D42; //BM
	DWORD dwBytesWritten = 0;
	WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)&bmpHeader, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);
	//Unlock and Free the DIB from the heap
	GlobalUnlock(hDIB);
	GlobalFree(hDIB);
	//Close the handle for the file that was created
	CloseHandle(hFile);

	return hr;
}




HRESULT Direct2DManagementClass::CaptureScreenShotToFile(HINSTANCE hInstance, HWND hWnd, LPCWSTR filename)
{
	HRESULT hr = S_OK;
	bool pResult = true;
	HDC hdScreen;
	HDC hdcWindow;
	HDC hMemDC = NULL;
	HBITMAP hbmScreen = NULL;
	BITMAP bmpScreen;
	hdScreen = GetDC(NULL);
	hdcWindow = GetDC(hWnd);


	hMemDC = CreateCompatibleDC(hdcWindow);
	if (!hMemDC)
	{
		MessageBox(hWnd, L"CreateCompatibleDC has failed", L"Failed", MB_OK);
		std::cout << "Unable to get desktop pointer" << endl;
		DeleteObject(hbmScreen);
		DeleteObject(hMemDC);
		ReleaseDC(NULL, hdScreen);
		ReleaseDC(hWnd, hdcWindow);
		return hr;
	}
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);
	UINT width = rcClient.right - rcClient.left;
	UINT height = rcClient.bottom - rcClient.top;
	SetStretchBltMode(hdcWindow, HALFTONE);

	pResult = StretchBlt(hdcWindow,
		0, 0,
		rcClient.right,
		rcClient.bottom,
		hdScreen,
		0, 0,
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN),
		SRCCOPY);
	if (!pResult)
	{
		MessageBox(hWnd, L"StretchBlt has failed", L"Failed", MB_OK);
		std::cout << "Stretch BLT Has Failed" << endl;
		DeleteObject(hbmScreen);
		DeleteObject(hMemDC);
		ReleaseDC(NULL, hdScreen);
		ReleaseDC(hWnd, hdcWindow);
		hr = E_FAIL;
		return hr;
	}
	hbmScreen = CreateCompatibleBitmap(hdcWindow,
		rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
	if (!hbmScreen)
	{
		MessageBoxA(hWnd, (LPCSTR)"Не удалось записать область экрана", (LPCSTR)"Message", 0);
		std::cout << "Unable to record screen image" << endl;
	}

	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hbmScreen); 

	pResult = BitBlt(hMemDC,
		0, 0,
		rcClient.right - rcClient.left,
		rcClient.bottom - rcClient.top,
		hdcWindow,
		0, 0,
		SRCCOPY
		);

	if (!pResult)
	{
		MessageBox(hWnd, L"BitBlt has failed", L"Failed", MB_OK);
		std::cout << "BitBlt has failed" << endl;
	}
	
	
	GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);
	BITMAPINFOHEADER bmpHeader;
	BITMAPFILEHEADER bmfHeader;
	bmpHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpHeader.biWidth = bmpScreen.bmWidth;
	bmpHeader.biHeight = bmpScreen.bmHeight;
	bmpHeader.biPlanes = 1;
	bmpHeader.biBitCount = 32;
	bmpHeader.biCompression = BI_RGB;
	bmpHeader.biSizeImage = 0;
	bmpHeader.biXPelsPerMeter = 0;
	bmpHeader.biYPelsPerMeter = 0;
	bmpHeader.biClrUsed = 0;
	bmpHeader.biClrImportant = 0;
	DWORD dwBmpSize = ((bmpScreen.bmWidth * bmpHeader.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;
	HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);

	char *lpbitmap = (char *)GlobalLock(hDIB);
	// Gets the "bits" from the bitmap and copies them into a buffer
	// which is pointed to by lpbitmap.

	GetDIBits(hdcWindow, hbmScreen, 0,
		(UINT)bmpScreen.bmHeight,
		lpbitmap,
		(BITMAPINFO *)&bmpHeader, DIB_RGB_COLORS);
	// A file is created, this is where we will save the screen capture.

	HANDLE hFile = CreateFile(filename,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);

	// Add the size of the headers to the size of the bitmap to get the total file size
	DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	//Offset to where the actual bitmap bits start.
	bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
	//Size of the file
	bmfHeader.bfSize = dwSizeofDIB;
	//bfType must always be BM for Bitmaps
	bmfHeader.bfType = 0x4D42; //BM
	DWORD dwBytesWritten = 0;
	WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)&bmpHeader, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);
	//Unlock and Free the DIB from the heap
	GlobalUnlock(hDIB);
	GlobalFree(hDIB);
	//Close the handle for the file that was created
	CloseHandle(hFile);
	
	return hr;
}


HRESULT Direct2DManagementClass::CaptureScreenShotToFile(HINSTANCE hInstance, HWND hWnd, LPCWSTR filename, char** pScreenShotCode)
{
	HRESULT hr = S_OK;
	bool pResult = true;
	HDC hdScreen;
	HDC hdcWindow;
	HDC hMemDC = NULL;
	HBITMAP hbmScreen = NULL;
	BITMAP bmpScreen;
	hdScreen = GetDC(NULL);
	hdcWindow = GetDC(hWnd);

	hMemDC = CreateCompatibleDC(hdcWindow);
	if (!hMemDC)
	{
		MessageBox(hWnd, L"CreateCompatibleDC has failed", L"Failed", MB_OK);
		std::cout << "Unable to get desktop pointer" << endl;
		DeleteObject(hbmScreen);
		DeleteObject(hMemDC);
		ReleaseDC(NULL, hdScreen);
		ReleaseDC(hWnd, hdcWindow);
		return hr;
	}
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);
	UINT width = rcClient.right - rcClient.left;
	UINT height = rcClient.bottom - rcClient.top;
	SetStretchBltMode(hdcWindow, HALFTONE);

	pResult = StretchBlt(hdcWindow,
		0, 0,
		rcClient.right,
		rcClient.bottom,
		hdScreen,
		0, 0,
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN),
		SRCCOPY);
	if (!pResult)
	{
		MessageBox(hWnd, L"StretchBlt has failed", L"Failed", MB_OK);
		std::cout << "Stretch BLT Has Failed" << endl;
		DeleteObject(hbmScreen);
		DeleteObject(hMemDC);
		ReleaseDC(NULL, hdScreen);
		ReleaseDC(hWnd, hdcWindow);
		hr = E_FAIL;
		return hr;
	}
	hbmScreen = CreateCompatibleBitmap(hdcWindow,
		rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
	if (!hbmScreen)
	{
		MessageBoxA(hWnd, (LPCSTR)"Не удалось записать область экрана", (LPCSTR)"Message", 0);
		std::cout << "Unable to record screen image" << endl;
	}

	SelectObject(hMemDC, hbmScreen);
	pResult = BitBlt(hMemDC,
		0, 0,
		rcClient.right - rcClient.left,
		rcClient.bottom - rcClient.top,
		hdcWindow,
		0, 0,
		SRCCOPY
		);
	if (!pResult)
	{
		MessageBox(hWnd, L"BitBlt has failed", L"Failed", MB_OK);
		std::cout << "BitBlt has failed" << endl;
	}

	GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);
	BITMAPINFOHEADER bmpHeader;
	BITMAPFILEHEADER bmfHeader;
	bmpHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpHeader.biWidth = bmpScreen.bmWidth;
	bmpHeader.biHeight = bmpScreen.bmHeight;
	bmpHeader.biPlanes = 1;
	bmpHeader.biBitCount = 32;
	bmpHeader.biCompression = BI_RGB;
	bmpHeader.biSizeImage = 0;
	bmpHeader.biXPelsPerMeter = 0;
	bmpHeader.biYPelsPerMeter = 0;
	bmpHeader.biClrUsed = 0;
	bmpHeader.biClrImportant = 0;
	DWORD dwBmpSize = ((bmpScreen.bmWidth * bmpHeader.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;
	HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);

	char *lpbitmap = (char *)GlobalLock(hDIB);
	// Gets the "bits" from the bitmap and copies them into a buffer
	// which is pointed to by lpbitmap.

	GetDIBits(hdcWindow, hbmScreen, 0,
		(UINT)bmpScreen.bmHeight,
		lpbitmap,
		(BITMAPINFO *)&bmpHeader, DIB_RGB_COLORS);
	// A file is created, this is where we will save the screen capture.

	HANDLE hFile = CreateFile(filename,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	// Add the size of the headers to the size of the bitmap to get the total file size
	DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	//Offset to where the actual bitmap bits start.
	bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
	//Size of the file
	bmfHeader.bfSize = dwSizeofDIB;
	//bfType must always be BM for Bitmaps
	bmfHeader.bfType = 0x4D42; //BM
	DWORD dwBytesWritten = 0;
	WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)&bmpHeader, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);
	//Unlock and Free the DIB from the heap
	GlobalUnlock(hDIB);
	GlobalFree(hDIB);
	//Close the handle for the file that was created
	CloseHandle(hFile);
	*pScreenShotCode = lpbitmap;
	return hr;
}


HRESULT Direct2DManagementClass::CreateDeviceResources(HINSTANCE hInstance, HWND hWnd)
{
	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);

	pD2DDXGITarget = nullptr;

	HRESULT hr = S_OK;

	SafeRelease(&pDXGIFactory);
	SafeRelease(&pGraphicsDrawingDevice);
	SafeRelease(&pGraphicsDrawingContext);
	SafeRelease(&mdxgidevice);
	SafeRelease(&mdxgidevice);
	SafeRelease(&m_pDXGIFactory2);
	SafeRelease(&m_pD2DDevice);
	SafeRelease(&m_pD2DDeviceContext);
	SafeRelease(&m_pD2DDeviceContext1);


	pDXGIFactory = nullptr;
	pGraphicsDrawingDevice = nullptr;
	pGraphicsDrawingContext = nullptr;
	mdxgidevice = nullptr;
	pDXGIAdapter = nullptr;
	m_pDXGIFactory2 = nullptr;
	m_pD2DDevice = nullptr;
	m_pD2DDeviceContext = nullptr;
	m_pD2DDeviceContext1 = nullptr;


	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;
	height = 900;

	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pDXGIFactory));

	pAdapters.resize(4);
	if (SUCCEEDED(hr))
	{
		for (UINT i = 0;
			pDXGIFactory->EnumAdapters(i, &pDXGIAdapter) != DXGI_ERROR_NOT_FOUND;
			++i)
		{
			pAdapters[i] = pDXGIAdapter;
		}
	}

	hr = D3D11CreateDevice(pAdapters[0],
		D3D_DRIVER_TYPE_UNKNOWN,
		0,
		creationFlags,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&pD3DGraphicsDevice,
		&featureLevel,
		&pD3DGraphicsContext);

	if (FAILED(hr))
	{

		hr = D3D11CreateDevice(pAdapters[1],
			D3D_DRIVER_TYPE_UNKNOWN,
			0,
			creationFlags,
			featureLevels,
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION,
			&pD3DGraphicsDevice,
			&featureLevel,
			&pD3DGraphicsContext);
	}

	
	if (FAILED(hr))
	{
		MessageBoxA(hWnd, (LPCSTR)"Возникла Проблема с вашим графическим процессором", (LPCSTR)"Message", 0);
		return hr;
	}

	hr = pD3DGraphicsDevice->QueryInterface(__uuidof(pGraphicsDrawingDevice), reinterpret_cast<void**>(&

		pGraphicsDrawingDevice));
	if (FAILED(hr))
	{
		std::cout << "Unable to Initialize Graphics Device Direct3D 11.1" << endl;
		return hr;
	}
	std::cout << "Graphics Device Direct3D 11.1 is Successfully Created" << endl;

	hr = pD3DGraphicsContext->QueryInterface(__uuidof(pGraphicsDrawingContext), reinterpret_cast<void**>(&
		pGraphicsDrawingContext));
	if (FAILED(hr))
	{
		std::cout << "Unable to Initialize Graphics Device Context Direct3D11.1" << endl;
		return hr;
	}
	std::cout << "Graphics Device Context Direct3D 11.1 is Successfully Created" << endl;

	hr = pD3DGraphicsDevice->QueryInterface(__uuidof(mdxgidevice), reinterpret_cast<void**>(&mdxgidevice));
	if (FAILED(hr))
	{
		return hr;
	}

	hr = mdxgidevice->GetParent(__uuidof(pDXGIAdapter), (void**)&pDXGIAdapter);
	if (SUCCEEDED(hr))
	{
		if (DISABLE_ALL_MESSAGE_BOXES == FALSE)
		{
			MessageBoxA(NULL, (LPCSTR)"Cоздан IDXGIAdapter", (LPCSTR)"Message", 0);
		}
	}

	hr = pDXGIAdapter->GetParent(__uuidof(m_pDXGIFactory2), reinterpret_cast<void**>(&m_pDXGIFactory2));
	if (FAILED(hr))
	{
		if (DISABLE_ALL_MESSAGE_BOXES == FALSE)
		{
			MessageBoxA(NULL, (LPCSTR)"Не удалось создать IDXGIFactory", (LPCSTR)"Message", 0);
		}
	}

	hr = m_pD2DFactory->CreateDevice(mdxgidevice, &m_pD2DDevice);
	if (FAILED(hr))
	{
		std::cout << "Unable to initalize Direct2D 1.1 device" << endl;
		return hr;
	}
	std::cout << "Direct2D Device is Successfully created" << endl;

	hr = m_pD2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_pD2DDeviceContext);
	if (FAILED(hr))
	{
		std::cout << "Unable to initalize Direct2D 1.1 device" << endl;
		return hr;
	}
	std::cout << "Direct2D Device Context is Successfully created" << endl;

	hr = m_pD2DDeviceContext->QueryInterface(__uuidof(m_pD2DDeviceContext1), reinterpret_cast<void**>(&
		m_pD2DDeviceContext1));



	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
	swapChainDesc.Width = 0;
	swapChainDesc.Height = 0;
	swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.Stereo = false;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE;

	hr = m_pDXGIFactory2->CreateSwapChainForHwnd(mdxgidevice, hWnd, &swapChainDesc,
		nullptr, nullptr, &pSwapChainObjectD2D);
	if (FAILED(hr))
	{
		std::cout << "Unable to create the swap chain object" << endl;
		return hr;
	}

	hr = pSwapChainObjectD2D->GetBuffer(0, IID_PPV_ARGS(&pDXGISurfaceObject));
	FLOAT dpiX, dpiY;

	m_pD2DFactory->GetDesktopDpi(&dpiX, &dpiY);

	D2D1_BITMAP_PROPERTIES1 bitmapProperies =
		D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE), dpiX, dpiY);
	hr = m_pD2DDeviceContext1->CreateBitmapFromDxgiSurface(pDXGISurfaceObject,
		&bitmapProperies,
		&pBitmapRenderTarget);

	m_pD2DDeviceContext1->SetTarget(pBitmapRenderTarget);
	ClearRenderTargetContent(hInstance, hWnd);


	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		D2D1_ALPHA_MODE_IGNORE),
		0,
		0,
		D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE,
		D2D1_FEATURE_LEVEL_DEFAULT
		);

        
	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidColorBrush);

	
	return hr;
}




HRESULT Direct2DManagementClass::DrawSampleTextOnTheScreenDirect2D(HINSTANCE hInstance, HWND hWnd, WCHAR* pMessage,
	UINT pFontSize)
{
	HRESULT hr = S_OK;
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;
	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	D2D1_POINT_2F centre = { pWidth / 2.0f, pHeight / 2.0f };
	D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
	D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
	int clippedAreaWidth = clippedArea.right - clippedArea.left;
	int clippedAreaHeight = clippedArea.bottom - clippedArea.top;
	hr = CreateDeviceResources(hInstance, hWnd);
	LPCWSTR pStringVar = (LPCWSTR)pMessage;
	ID2D1SolidColorBrush* pGreenBrush = nullptr;
	ID2D1SolidColorBrush* pRedBrush = nullptr;
	ID2D1SolidColorBrush* pYellowBrush = nullptr;
	ID2D1SolidColorBrush* pBlackBrush = nullptr;
	ID2D1SolidColorBrush* pWhiteBrush = nullptr;
	IDWriteTextFormat* pTextFormat = nullptr;
	IDWriteTextLayout* pTextLayout = nullptr;
	IDWriteBitmapRenderTarget* pDWriteBitmapRenderTarget = nullptr;

	IDWriteFactory* pDwriteFactoryObjectVar = nullptr;


	HDC hDC = BeginPaint(hWnd, &ps);
	if (hDC)
	{

		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}

		m_pD2DDeviceContext1->BeginDraw();
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));
		m_pD2DDeviceContext1->SetTransform(D2D1::IdentityMatrix());
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);


		ID2D1Brush* pBlackBrushInterface = nullptr;
		pBlackBrushInterface = pBlackBrush;



		D2D1_POINT_2F centre = { pWidth / 2.0f, pHeight / 2.0f };
		D2D1_POINT_2F centreLeft = { pWidth / 2.0f - 320, pHeight / 2.0f };
		D2D1_POINT_2F centreRight = { pWidth / 2.0f + 320, pHeight / 2.0f };

		D2D1_POINT_2F centre0 = { pWidth / 2.0f, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreLeft0 = { pWidth / 2.0f - 320, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreRight0 = { pWidth / 2.0f + 320, pHeight / 2.0f - 20 };



		IDWriteGdiInterop* pGDIInterop = nullptr;
		IDWritePixelSnapping* pPixelSnapping = nullptr;


		cout << "Creating A text Format for Gabriola Font" << endl;

		IDWriteFontFace* pFontFaceObject = nullptr;
		IDWriteFont* pFontObject = nullptr;







		std::cout << "Setting Text Layout Information 0" << endl;
		IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
		IDWriteTextLayout* pTextLayoutInformation1 = nullptr;
		IDWriteTextLayout* pTextLayoutInformation2 = nullptr;
		IDWriteBitmapRenderTarget* pTargetDWrite = nullptr;


		IDWriteTextFormat* pTextFormat = nullptr;
		hr = pFactoryObjectDWrite->CreateTextFormat(
			L"Times New Roman",              // Font family name.
			NULL,                       // Font collection (NULL sets it to use the system font collection).
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			pFontSize,
			L"ru",
			&pTextFormat
			);

		static const WCHAR pText0[] = L"Hello World";
		UINT dpiScaleX = DEFAULT_DPI;
		UINT dpiScaleY = DEFAULT_DPI;
		std::cout << "Setting Text Alignment Object" << endl;
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
		DWORD dwFlags = 0;
		HMONITOR pMonitorValue = NULL;
		pMonitorValue = MonitorFromWindow(
			hWnd,
			dwFlags
			);



		static const UINT stringLength0 = wcslen(pMessage) + 1;

		D2D1_RECT_F layoutRect = D2D1::RectF(
			static_cast<FLOAT>(rc.left) / dpiScaleX,
			static_cast<FLOAT>(rc.top) / dpiScaleY,
			static_cast<FLOAT>(rc.right - rc.left) / dpiScaleX,
			static_cast<FLOAT>(rc.bottom - rc.top) / dpiScaleY
			);




		hr = pFactoryObjectDWrite->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			DWRITE_FONT_STYLE_ITALIC,
			DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
			pFontSize,
			L"",
			&pTextFormat);

		std::cout << "Setting Text Alignment Object" << endl;
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);


		std::cout << "Setting Text Layout Information 0" << endl;
		hr = pFactoryObjectDWrite->CreateTextLayout(
			pMessage,
			stringLength0,
			pTextFormat,
			300, // maxWidth
		    300, // maxHeight
			&pTextLayoutInformation0
			);



		pFactoryObjectDWrite->GetGdiInterop(&pGDIInterop);

		IDWriteTextRenderer1* pTextRenderer = nullptr;


		hr = pGDIInterop->CreateBitmapRenderTarget(hDC, pWidth, pHeight,
			&pTargetDWrite);

		std::cout << "Setting Text Layout Information 0" << endl;
		pTextLayoutInformation0->SetMaxHeight(300);
		pTextLayoutInformation0->SetMaxWidth(300);
		pTextLayoutInformation0->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation0->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
		pTextLayoutInformation0->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

		RECT rcRect = { 0.0f, 0.0f, 0.0f, 0.0f };
		GetClientRect(hWnd, &rcRect);


		ID2D1Brush* pBaseBursh = nullptr;
		hr = pBlackBrush->QueryInterface(__uuidof(pBaseBursh), reinterpret_cast<void**>(&pBaseBursh));

   

		D2D1_POINT_2F pPointVar = D2D1::Point2F((rcRect.right - rcRect.left) / 2.0f, (rcRect.bottom - rcRect.top) / 2.0f);

		UINT32 cTextLength = (UINT32)wcslen(pMessage);



		m_pD2DDeviceContext1->DrawTextLayout(pPointVar, pTextLayoutInformation0, pBaseBursh);

		if (hr == D2DERR_RECREATE_TARGET)
		{
			SafeReleaseA(&m_pD2DBitmap);
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}
		m_pD2DDeviceContext1->EndDraw();


		pSwapChainObjectD2D->Present(0, 0);

		EndPaint(hWnd, &ps);
	}
	return hr;
}

HRESULT Direct2DManagementClass::DrawSomeThingUsingGDIInterop(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr = S_OK;

	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };

	ID2D1SolidColorBrush* pBlueBrush = nullptr;


	if (pD2DDXGITarget == nullptr)
	{
		hr = CreateDeviceResources(hInstance, hWnd);
	}

	m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), &pBlueBrush);
	m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackColorBrush);


	PAINTSTRUCT ps = {};
	HDC pHDC = NULL;

	pHDC = BeginPaint(hWnd, &ps);

             
	GetClientRect(hWnd, &rc);


	float pXCentre = 0.0f;
	float pYCentre = 0.0f; 


	pXCentre = (rc.right - rc.left) / 2.0f;
	pYCentre = (rc.bottom - rc.top) / 2.0f;

	float pXCentreLeft = pXCentre - 200.0f;
	float pXCentreRight = pXCentre + 200.0f;


		// Bind the DC to the DC render target.
         
		m_pD2DDeviceContext1->BeginDraw();


		m_pD2DDeviceContext1->SetTransform(D2D1::Matrix3x2F::Identity());

	
		m_pD2DDeviceContext1->FillEllipse(
			D2D1::Ellipse(
			D2D1::Point2F(pXCentre, pYCentre),
			100.0f,
			100.0f),
			pBlueBrush
			);

		m_pD2DDeviceContext1->FillEllipse(
			D2D1::Ellipse(
			D2D1::Point2F(pXCentreLeft, pYCentre),
			100.0f,
			100.0f),
			pBlueBrush
			);


		m_pD2DDeviceContext1->FillEllipse(
			D2D1::Ellipse(
			D2D1::Point2F(pXCentreRight, pYCentre),
			100.0f,
			100.0f),
			pBlueBrush
			);


		m_pD2DDeviceContext1->DrawLine(
			D2D1::Point2F(pXCentre, pYCentre),
			D2D1::Point2F(
			(pXCentre + 100.0f * 0.15425f),
			(pXCentre - 100.0f * 0.988f)),
			pBlackColorBrush,
			3.0
			);

		m_pD2DDeviceContext1->DrawLine(
			D2D1::Point2F(pXCentreLeft, pYCentre),
			D2D1::Point2F(
			(pXCentreLeft + 100.0f * 0.525f),
			(pXCentreLeft + 100.0f * 0.8509f)),
			pBlackColorBrush,
			3.0
			);
		m_pD2DDeviceContext1->DrawLine(
			D2D1::Point2F(pXCentreRight, pYCentre),
			D2D1::Point2F(
			(pXCentreRight - 100.0f * 0.988f),
			(pXCentreRight - 100.0f * 0.15425f)),
			pBlackColorBrush,
			3.0
			);

	
		if (FAILED(hr))
		{
			int pLastError = GetLastError();
			cout << "Output to a GDI RESOURCE FAILED WITH ERROR: " << pLastError << endl;
			return hr;
		}

		if (SUCCEEDED(hr))
		{
			//
			// Draw the pie chart with GDI.
			//

			// Save the original object.
			HGDIOBJ original = NULL;
			original = SelectObject(
				pHDC,
				GetStockObject(DC_BRUSH)
				);

			HBRUSH pBrush = NULL;
			pBrush = CreateSolidBrush(RGB(255.0f, 0.0f, 0.0f));
			SelectObject(pHDC, pBrush);

			Ellipse(pHDC, 300, 50, 500, 250);

			POINT pntArray1[2];
			pntArray1[0].x = 400;
			pntArray1[0].y = 150;
			pntArray1[1].x = static_cast<LONG>(400 + 100 * 0.15425);
			pntArray1[1].y = static_cast<LONG>(150 - 100 * 0.9885);

			POINT pntArray2[2];
			pntArray2[0].x = 400;
			pntArray2[0].y = 150;
			pntArray2[1].x = static_cast<LONG>(400 + 100 * 0.525);
			pntArray2[1].y = static_cast<LONG>(150 + 100 * 0.8509);


			POINT pntArray3[2];
			pntArray3[0].x = 400;
			pntArray3[0].y = 150;
			pntArray3[1].x = static_cast<LONG>(400 - 100 * 0.988);
			pntArray3[1].y = static_cast<LONG>(150 - 100 * 0.15425);

			Polyline(pHDC, pntArray1, 2);
			Polyline(pHDC, pntArray2, 2);
			Polyline(pHDC, pntArray3, 2);

			

			
			// Restore the original object.
		}
		m_pD2DDeviceContext1->EndDraw();

	

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}
	
	EndPaint(hWnd, &ps);
	pSwapChainObjectD2D->Present(0, 0);
	return hr;
}





HRESULT Direct2DManagementClass::ApplyHistogramEffect(HINSTANCE hInstance, HWND hWnd, vector<float> *pImageData)
{
	HRESULT hr = S_OK;
	std::cout << "We WILL START BY SELECTING A NEW IMAGE FILE" << endl;

	UINT pTopLeftX = 0;
	UINT pTopLeftY = 0;
	float pImageWidth = 0.0f;
	float pImageHeight = 0.0f;
	std::cout << "Enter the X Value for Top Left Position of the Image Sector" << endl;
	cin >> pTopLeftX;
	std::cout << "Enter the Y Value for Top Left Position of the Image Sector" << endl;
	cin >> pTopLeftY;


	std::cout << "Enter the Width of Image Sector" << endl;
	cin >> pImageWidth;

	std::cout << "Enter the Width of Image Sector" << endl;
	cin >> pImageHeight;

	ID2D1Bitmap* pBitmapToApply = nullptr;
	ID2D1Effect* pHistogramEffect = nullptr;
	IWICBitmapFrameDecode* pFrame = nullptr;
	WCHAR szFileName[MAX_PATH];
	vector<float> pImageFloatData = vector<float>();
	if (LocateImageFile(hWnd, szFileName, ARRAYSIZE(szFileName)))
	{

		// Prepare iterator pairs to iterate the file content!


		std::cout << "Creating a decoder " << endl;
		hr = m_pWICFactory->CreateDecoderFromFilename(
			szFileName,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&pImageDecoder);



		std::cout << "Gettiing A Frame " << endl;
		if (SUCCEEDED(hr))
		{
			hr = pImageDecoder->GetFrame(0, &pFrame);
		}


		if (SUCCEEDED(hr))
		{

			std::cout << "Loading a Bitmap" << endl;
			hr = m_pWICFactory->CreateBitmapFromSourceRect(pFrame,
				pTopLeftX, pTopLeftY, pImageWidth, pImageHeight,
				&pBitmapObject);
		}


		std::cout << "Creating a Converter Object" << endl;
		if (SUCCEEDED(hr))
		{
			SafeReleaseA(&m_pConvertedSourceBitmap);
			hr = m_pWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pConvertedSourceBitmap->Initialize(
				pBitmapObject,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeCustom);

		}
		if (SUCCEEDED(hr))
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}
		if (SUCCEEDED(hr))
		{

			SafeReleaseA(&pBitmapToApply);
			hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &pBitmapToApply);

		}




		if (SUCCEEDED(hr))
		{
			hr = CreateHistogramEffect(hInstance, hWnd, pBitmapToApply, &pHistogramEffect);
		}
		if (SUCCEEDED(hr))
		{
			hr = RenderHistogramEffect(hInstance, hWnd, pHistogramEffect, &pImageFloatData);
		}

		if (SUCCEEDED(hr))
		{
			pSwapChainObjectD2D->Present(0, 0);
		}
	}
	*pImageData = pImageFloatData;
	return hr;
}

	HRESULT Direct2DManagementClass::CreateHistogramEffect(HINSTANCE hInstance, HWND hWnd, ID2D1Bitmap* pInputBitmap, ID2D1Effect** ppHistogramEffect)
	{
		HRESULT hr = S_OK;
		IWICFormatConverter* pBitmapFormatConverter = nullptr;
		ID2D1Effect* pHistogramEffect = nullptr;

		hr = m_pD2DDeviceContext->CreateEffect(CLSID_D2D1Histogram, &pHistogramEffect);

		if (SUCCEEDED(hr))
		{
		
			pHistogramEffect->SetInput(0, pInputBitmap);
		}


		if (SUCCEEDED(hr))
		{
			*ppHistogramEffect = pHistogramEffect;
		}


		return hr;
	}

	HRESULT Direct2DManagementClass::RenderHistogramEffect(HINSTANCE hInstance, HWND hWnd, ID2D1Effect* pHistogramEffect, vector<float> *pDataArray)
	{
		HRESULT hr = S_OK;
		vector<float> pDataArrayObject = vector<float>();
		m_pD2DDeviceContext->BeginDraw();
		m_pD2DDeviceContext->DrawImage(pHistogramEffect);
		m_pD2DDeviceContext->EndDraw();

		// The histogram data is only available once the effect has been 'drawn'.
		int histogramBinCount;

		hr = pHistogramEffect->GetValue(D2D1_HISTOGRAM_PROP_NUM_BINS, &histogramBinCount);

		float *histogramData = new float[histogramBinCount];
		hr = pHistogramEffect->GetValue(D2D1_HISTOGRAM_PROP_HISTOGRAM_OUTPUT,
			reinterpret_cast<BYTE*>(histogramData),
			histogramBinCount * sizeof(float));

		if (SUCCEEDED(hr))
		{
			hr = pSwapChainObjectD2D->Present(0, 0);
		}
		for (UINT pIndex = 0; pIndex < histogramBinCount; ++pIndex)
		{
			pDataArrayObject.push_back(histogramData[pIndex]);
		}

		*pDataArray = pDataArrayObject;
		return hr;
	}


	HRESULT Direct2DManagementClass::DrawLinesBetweenPoints(HINSTANCE hInstance, HWND hWnd, Point2D pPointArray[3])
	{

		HRESULT hr = S_OK;
		BYTE* selectedAreaData = NULL;
		PAINTSTRUCT ps;

		RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
		GetClientRect(hWnd, &rc);
		UINT pWidth = rc.right - rc.left;
		UINT pHeight = rc.bottom - rc.top;
		D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
		D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
		int clippedAreaWidth = clippedArea.right - clippedArea.left;
		int clippedAreaHeight = clippedArea.bottom - clippedArea.top;

		ID2D1SolidColorBrush* pGreenBrush = nullptr;
		ID2D1SolidColorBrush* pRedBrush = nullptr;
		ID2D1SolidColorBrush* pYellowBrush = nullptr;
		ID2D1SolidColorBrush* pBlackBrush = nullptr;
		ID2D1SolidColorBrush* pWhiteBrush = nullptr;

		vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();



		static const WCHAR pText0[] = L"33";
		static const WCHAR pText1[] = L"66";
		static const WCHAR pText2[] = L"100";



		static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
		static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
		static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;


		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);


		HDC hDC = BeginPaint(hWnd, &ps);
		if (hDC)
		{
			if (m_pD2DDeviceContext1 == nullptr)
			{
				hr = CreateDeviceResources(hInstance, hWnd);
			}



			


			D2D1_POINT_2F pointOne = { pPointArray[0].xPosition, pPointArray[0].yPosition};
			D2D1_POINT_2F pointTwo = { pPointArray[1].xPosition, pPointArray[1].yPosition};
			D2D1_POINT_2F pointThree = { pPointArray[2].xPosition, pPointArray[2].yPosition };

		
			m_pD2DDeviceContext1->BeginDraw();
			m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));


			m_pD2DDeviceContext1->DrawLine(pointOne, pointTwo, pBlackBrush);
			m_pD2DDeviceContext1->DrawLine(pointTwo, pointThree, pBlackBrush);
	


			D2D1_SIZE_F renderTargetSize = m_pD2DDeviceContext1->GetSize();

			m_pD2DDeviceContext1->EndDraw();


			// Retrieve D2D Device.



			if (hr == D2DERR_RECREATE_TARGET)
			{
				hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
			}






			pSwapChainObjectD2D->Present(0, 0);
			EndPaint(hWnd, &ps);


		}
		return SUCCEEDED(hr) ? 0 : 1;
	}



	HRESULT Direct2DManagementClass::ConnectPointsUsingLines(HINSTANCE hInstance, HWND hWnd,
		vector<Point2D> pPointArrayVectorOne,
		vector<Point2D> pPointArrayVectorTwo,
		vector<Point2D> pPointArrayVectorThree,
		vector<Point2D> pPointArrayVectorFour)
	{

		HRESULT hr = S_OK;
		BYTE* selectedAreaData = NULL;
		PAINTSTRUCT ps;

		RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
		GetClientRect(hWnd, &rc);
		UINT pWidth = rc.right - rc.left;
		UINT pHeight = rc.bottom - rc.top;
		D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
		D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
		int clippedAreaWidth = clippedArea.right - clippedArea.left;
		int clippedAreaHeight = clippedArea.bottom - clippedArea.top;

		ID2D1SolidColorBrush* pGreenBrush = nullptr;
		ID2D1SolidColorBrush* pRedBrush = nullptr;
		ID2D1SolidColorBrush* pYellowBrush = nullptr;
		ID2D1SolidColorBrush* pBlackBrush = nullptr;
		ID2D1SolidColorBrush* pWhiteBrush = nullptr;

		vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();


		
		static const WCHAR pText0[] = L"33";
		static const WCHAR pText1[] = L"66";
		static const WCHAR pText2[] = L"100";



		static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
		static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
		static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;


		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);


		BeginPaint(hWnd, &ps);
		HDC hDC = GetDC(hWnd);
		HDC src = CreateCompatibleDC(hDC);
		HGDIOBJ hPen = GetStockObject(BLACK_PEN);
		HGDIOBJ hOldPen = SelectObject(hDC, hPen);

	

		if (hDC)
		{
			POINT pPointOne;
			pPointOne.x = pPointArrayVectorOne[0].xPosition;
			pPointOne.y = pPointArrayVectorOne[0].yPosition;

			POINT pPointTwo;
			pPointTwo.x = pPointArrayVectorOne[1].xPosition;
			pPointTwo.y = pPointArrayVectorOne[1].yPosition;


			POINT pPointThree;
			pPointThree.x = pPointArrayVectorOne[2].xPosition;
			pPointThree.y = pPointArrayVectorOne[2].xPosition;

			POINT pPointFour;
			pPointThree.x = pPointArrayVectorOne[3].xPosition;
			pPointThree.y = pPointArrayVectorOne[3].xPosition;



			POINT pPointFive;
			pPointFive.x = pPointArrayVectorTwo[0].xPosition;
			pPointFive.y = pPointArrayVectorTwo[0].xPosition;

			POINT pPointSix;
			pPointSix.x = pPointArrayVectorTwo[1].xPosition;
			pPointSix.y = pPointArrayVectorTwo[1].xPosition;

			POINT pPointSeven;
			pPointSeven.x = pPointArrayVectorTwo[2].xPosition;
			pPointSeven.y = pPointArrayVectorTwo[2].xPosition;

			POINT pPointEight;
			pPointEight.x = pPointArrayVectorTwo[3].xPosition;
			pPointEight.y = pPointArrayVectorTwo[3].xPosition;


			POINT pPointNine;
			pPointNine.x = pPointArrayVectorThree[0].xPosition;
			pPointNine.y = pPointArrayVectorThree[0].yPosition;

			POINT pPointTen;
			pPointTen.x = pPointArrayVectorFour[0].xPosition;
			pPointTen.y = pPointArrayVectorFour[0].yPosition;

			POINT pPointEleven;
			pPointEleven.x = pPointArrayVectorFour[1].xPosition;
			pPointEleven.y = pPointArrayVectorFour[1].yPosition;


			POINT pPointTwelve;
			pPointTwelve.x = pPointArrayVectorFour[2].xPosition;
			pPointTwelve.y = pPointArrayVectorFour[2].yPosition;

		

			MoveToEx(hDC, pPointOne.x, pPointOne.y, &pPointOne);
			LineTo(hDC, pPointTwo.x, pPointTwo.x);
			MoveToEx(hDC, pPointTwo.x, pPointTwo.y, &pPointTwo);
			LineTo(hDC, pPointThree.x, pPointThree.y);
			MoveToEx(hDC, pPointThree.x, pPointThree.y, &pPointThree);
			LineTo(hDC, pPointFour.x, pPointFour.y);
			MoveToEx(hDC, pPointFour.x, pPointFour.y, &pPointFour);
			LineTo(hDC, pPointFive.x, pPointFive.y);
			MoveToEx(hDC, pPointFive.x, pPointFive.y, &pPointFive);
			LineTo(hDC, pPointSix.x, pPointSix.y);
			MoveToEx(hDC, pPointSix.x, pPointSix.y, &pPointSix);
			LineTo(hDC, pPointSeven.x, pPointSeven.y);
			MoveToEx(hDC, pPointSeven.x, pPointSeven.y, &pPointSeven);
			LineTo(hDC, pPointEight.x, pPointEight.y);
			MoveToEx(hDC, pPointEight.x, pPointEight.y, &pPointEight);
			LineTo(hDC, pPointNine.x, pPointNine.y);
			MoveToEx(hDC, pPointNine.x, pPointNine.y, &pPointNine);
			LineTo(hDC, pPointTen.x, pPointTen.y);
			MoveToEx(hDC, pPointTen.x, pPointTen.y, &pPointTen);
			LineTo(hDC, pPointEleven.x, pPointEleven.y);
			MoveToEx(hDC, pPointEleven.x, pPointEleven.y, &pPointEleven);
			LineTo(hDC, pPointTwelve.x, pPointTwelve.y);
			MoveToEx(hDC, pPointTwelve.x, pPointTwelve.y, &pPointTwelve);
			LineTo(hDC, pPointOne.x, pPointOne.y);










	

		}
		EndPaint(hWnd, &ps);

		return hr;
	}



	HRESULT Direct2DManagementClass::GenerateSyntheticImageOfDefinedColor(HINSTANCE hInstance,
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
		vector<PixelObject> *ppPixelObject)
	{
		HRESULT hr = S_OK;
		vector<PixelObject> ppPixelArrayToGenerateOne = vector<PixelObject>();
		float pXVal = 0.0f;
		float pYVal = 0.0f;
		int pixelCounter = 0;
	

		for (pXVal = pTopLeftX; pXVal < pTopLeftX + pImageWidth; ++pXVal)
		{
			for (pYVal = pTopLeftY; pYVal < pTopLeftY + pImageHeight; ++pYVal)
				// Your code here
			{

				std::cout << "Current X Value: " << pXVal << " " << "Current Y Value: " << pYVal << endl;
				PixelObject pixelObj = PixelObject(pixelCounter, pXVal, pYVal, pRedValue, pGreenValue, pBlueValue);
				ppPixelArrayToGenerateOne.push_back(pixelObj);
			pixelCounter = pixelCounter + 1;

			}
		}

		*ppPixelObject = ppPixelArrayToGenerateOne;
		return hr;
	}




	HRESULT Direct2DManagementClass::ConnectPointsUsingLines(HINSTANCE hInstance, HWND hWnd,
		float pCentreX,
		float pCentreY,
		vector<Point2D> pPointArrayVector)
	{

		Point2D pCentre;
		pCentre.pPointNumber = 0;
		pCentre.xPosition = pCentreX;
		pCentre.yPosition = pCentreY;
		float pTriangleAreaTotal = 0.0f;


		vector<Line2D> pLineArrayValues = vector<Line2D>();
		HRESULT hr = S_OK;
		BYTE* selectedAreaData = NULL;
		PAINTSTRUCT ps;

		RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
		GetClientRect(hWnd, &rc);
		UINT pWidth = rc.right - rc.left;
		UINT pHeight = rc.bottom - rc.top;
		D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
		D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
		int clippedAreaWidth = clippedArea.right - clippedArea.left;
		int clippedAreaHeight = clippedArea.bottom - clippedArea.top;

		ID2D1SolidColorBrush* pGreenBrush = nullptr;
		ID2D1SolidColorBrush* pRedBrush = nullptr;
		ID2D1SolidColorBrush* pYellowBrush = nullptr;
		ID2D1SolidColorBrush* pBlackBrush = nullptr;
		ID2D1SolidColorBrush* pWhiteBrush = nullptr;

		vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();


		static const WCHAR pText0[] = L"33";
		static const WCHAR pText1[] = L"66";
		static const WCHAR pText2[] = L"100";

		const char* pFilePath = "E:\\RobotData\\my_pixel_array_line_connector.bmp";


		static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
		static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
		static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;


		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);


		BeginPaint(hWnd, &ps);
		HDC hDC = GetDC(hWnd);
		HDC MemDC = CreateCompatibleDC(hDC);

		HGDIOBJ hPen = GetStockObject(BLACK_PEN);
		HGDIOBJ hOldPen = SelectObject(hDC, hPen);




		if (hDC)
		{

			vector<POINT> pPointVectorOne = vector<POINT>();
			vector<POINT> pPointVectorTwo = vector<POINT>();
			vector<POINT> pPointVectorThree = vector<POINT>();
			vector<POINT> pPointVectorFour = vector<POINT>();

			for (Point2D pPointObject : pPointArrayVector)
			{

				if (pPointObject.pSectorNumber == 1)
				{
					POINT pPointVar = ConvertPoint2DToPoint(pPointObject);
					pPointVectorOne.push_back(pPointVar);
				}
				else if (pPointObject.pSectorNumber == 2)
				{
					POINT pPointVar = ConvertPoint2DToPoint(pPointObject);
					pPointVectorTwo.push_back(pPointVar);
				}
				else if (pPointObject.pSectorNumber == 3)
				{
					POINT pPointVar = ConvertPoint2DToPoint(pPointObject);
					pPointVectorThree.push_back(pPointVar);
				}
				else if (pPointObject.pSectorNumber == 4)
				{
					POINT pPointVar = ConvertPoint2DToPoint(pPointObject);
					pPointVectorFour.push_back(pPointVar);
				}



			}


			POINT pPointOneVarCopy = pPointVectorOne[0];


			Point2D pCentre;
			pCentre.pPointNumber = 0;
			pCentre.xPosition = 430;
			pCentre.yPosition = 410;

			for (UINT pIndex = 0; pIndex <= pPointVectorOne.size() - 2; pIndex++)
			{
				MoveToEx(hDC, pPointVectorOne[pIndex].x, pPointVectorOne[pIndex].y, &pPointVectorOne[pIndex]);
				LineTo(hDC, pPointVectorOne[pIndex + 1].x, pPointVectorOne[pIndex + 1].y);
				
			}

			MoveToEx(hDC, pPointVectorOne[pPointVectorOne.size() - 1].x, pPointVectorOne[pPointVectorOne.size() - 1].y, &pPointVectorOne[pPointVectorOne.size() - 1]);
			LineTo(hDC, pPointVectorTwo[0].x, pPointVectorTwo[0].y);

			for (UINT pIndex = 0; pIndex <= pPointVectorTwo.size() - 2; pIndex++)
			{
				MoveToEx(hDC, pPointVectorTwo[pIndex].x, pPointVectorTwo[pIndex].y, &pPointVectorTwo[pIndex]);
				LineTo(hDC, pPointVectorTwo[pIndex + 1].x, pPointVectorTwo[pIndex + 1].y);

			}

			MoveToEx(hDC, pPointVectorTwo[pPointVectorTwo.size() - 1].x, pPointVectorTwo[pPointVectorTwo.size() - 1].y, &pPointVectorTwo[pPointVectorTwo.size() - 1]);
			LineTo(hDC, pPointVectorThree[0].x, pPointVectorThree[0].y);

			for (UINT pIndex = 0; pIndex <= pPointVectorThree.size() - 2; pIndex++)
			{
				MoveToEx(hDC, pPointVectorThree[pIndex].x, pPointVectorThree[pIndex].y, &pPointVectorThree[pIndex]);
				LineTo(hDC, pPointVectorThree[pIndex + 1].x, pPointVectorThree[pIndex + 1].y);

			}

			MoveToEx(hDC, pPointVectorThree[pPointVectorThree.size() - 1].x, pPointVectorThree[pPointVectorThree.size() - 1].y, &pPointVectorThree[pPointVectorThree.size() - 1]);
			LineTo(hDC, pPointVectorFour[0].x, pPointVectorFour[0].y);

			if (pPointVectorFour.size() > 1)
			{
				for (UINT pIndex = 0; pIndex <= pPointVectorFour.size() - 2; pIndex++)
				{
					MoveToEx(hDC, pPointVectorFour[pIndex].x, pPointVectorFour[pIndex].y, &pPointVectorFour[pIndex]);
					LineTo(hDC, pPointVectorFour[pIndex + 1].x, pPointVectorFour[pIndex + 1].y);

				}
			}
			

			MoveToEx(hDC, pPointVectorFour[pPointVectorFour.size() - 1].x, pPointVectorFour[pPointVectorFour.size() - 1].y, &pPointVectorFour[pPointVectorFour.size() - 1]);
			LineTo(hDC, pPointOneVarCopy.x, pPointOneVarCopy.y);




			BITMAPINFO Info;
			BITMAPFILEHEADER Header;
			memset(&Info, 0, sizeof(Info));
			memset(&Header, 0, sizeof(Header));
			Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			Info.bmiHeader.biWidth = 800;
			Info.bmiHeader.biHeight = 800;
			Info.bmiHeader.biPlanes = 1;
			Info.bmiHeader.biBitCount = 24;
			Info.bmiHeader.biCompression = BI_RGB;
			Info.bmiHeader.biSizeImage = 800 * 800 * 24;
			Header.bfType = 0x4D42;
			Header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
			char* Pixels = NULL;
			HBITMAP Section = CreateDIBSection(MemDC, &Info, DIB_RGB_COLORS, (void**)&Pixels, 0, 0);
			DeleteObject(SelectObject(MemDC, Section));
			BitBlt(MemDC, 0, 0, 800, 800, hDC, rc.left, rc.top, SRCCOPY);
			DeleteDC(MemDC);
			std::fstream hFile(pFilePath, std::ios::out | std::ios::binary);
			if (hFile.is_open())
			{
				hFile.write((char*)&Header, sizeof(Header));
				hFile.write((char*)&Info.bmiHeader, sizeof(Info.bmiHeader));
				hFile.write(Pixels, (((24 * 800 + 31) & ~31) / 8) * 800);
				hFile.close();
				DeleteObject(Section);
				return true;
			}
			DeleteObject(Section);














		}
		EndPaint(hWnd, &ps);

		return hr;
	}



	HRESULT Direct2DManagementClass::ConnectPointsUsingLinesSectors(HINSTANCE hInstance, HWND hWnd,
		float pCentreX,
		float pCentreY,
		vector<Point2D> pPointArrayVector,
		vector<Line2D> *ppOutputLineArray,
		float* ppTotalArea)
	{

		Point2D pCentre;
		pCentre.pPointNumber = 0;
		pCentre.xPosition = pCentreX;
		pCentre.yPosition = pCentreY;
		float pTriangleAreaTotal = 0.0f;


		vector<Line2D> pLineArrayValues = vector<Line2D>();
		HRESULT hr = S_OK;
		BYTE* selectedAreaData = NULL;
		PAINTSTRUCT ps;

		RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
		GetClientRect(hWnd, &rc);
		UINT pWidth = rc.right - rc.left;
		UINT pHeight = rc.bottom - rc.top;
		D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
		D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
		int clippedAreaWidth = clippedArea.right - clippedArea.left;
		int clippedAreaHeight = clippedArea.bottom - clippedArea.top;

		ID2D1SolidColorBrush* pGreenBrush = nullptr;
		ID2D1SolidColorBrush* pRedBrush = nullptr;
		ID2D1SolidColorBrush* pYellowBrush = nullptr;
		ID2D1SolidColorBrush* pBlackBrush = nullptr;
		ID2D1SolidColorBrush* pWhiteBrush = nullptr;

		vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();
	

		static const WCHAR pText0[] = L"33";
		static const WCHAR pText1[] = L"66";
		static const WCHAR pText2[] = L"100";

		const char* pFilePath = "E:\\RobotData\\my_pixel_array_line_connector.bmp";


		static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
		static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
		static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;


		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);


		BeginPaint(hWnd, &ps);
		HDC hDC = GetDC(hWnd);
		HDC MemDC = CreateCompatibleDC(hDC);

		HGDIOBJ hPen = GetStockObject(BLACK_PEN);
		HGDIOBJ hOldPen = SelectObject(hDC, hPen);




		if (hDC)
		{

						vector<Point2D> pSectorOnePoints = vector<Point2D>();
						vector<Point2D> pSectorTwoPoints = vector<Point2D>();
						vector<Point2D> pSectorThreePoints = vector<Point2D>();
						vector<Point2D> pSectorFourPoints = vector<Point2D>();


						Point2D pPointOne;
						pPointOne.pPointNumber = 0;
						pPointOne.xPosition = 0.0f;
						pPointOne.yPosition = 0.0F;
						pPointOne.pSectorNumber = 0;

						Point2D pPointOneCopy;
						pPointOneCopy.pPointNumber = 0;
						pPointOneCopy.xPosition = 0.0f;
						pPointOneCopy.yPosition = 0.0F;
						pPointOneCopy.pSectorNumber = 0;

						pPointOne = pPointArrayVector[0];
						pPointOneCopy = pPointArrayVector[0];

						POINT pInitialPoint = ConvertPoint2DToPoint(pPointOne);
						POINT pInitialPointCopy = ConvertPoint2DToPoint(pPointOneCopy);

						for (Point2D pPointObject : pPointArrayVector)
						{
							if (pPointObject.pSectorNumber == 1)
							{
								pSectorOnePoints.push_back(pPointObject);
							}
							else if (pPointObject.pSectorNumber == 2)
							{
								pSectorTwoPoints.push_back(pPointObject);
							}
							else if (pPointObject.pSectorNumber == 3)
							{
								pSectorThreePoints.push_back(pPointObject);
							}
							else if (pPointObject.pSectorNumber == 4)
							{
								pSectorFourPoints.push_back(pPointObject);
							}

						}
						

						vector<POINT> pPointVectorOne = vector<POINT>();
						vector<POINT> pPointVectorTwo = vector<POINT>();
						vector<POINT> pPointVectorThree = vector<POINT>();
						vector<POINT> pPointVectorFour = vector<POINT>();



						for (Point2D pPoint2D : pSectorOnePoints)
						{
							POINT pPointOneVar = ConvertPoint2DToPoint(pPoint2D);
							pPointVectorOne.push_back(pPointOneVar);
				
						}

						POINT pPointOneCopyVar = pPointVectorOne[0];

						for (Point2D pPoint2D : pSectorTwoPoints)
						{
							POINT pPointOneVar = ConvertPoint2DToPoint(pPoint2D);
							pPointVectorTwo.push_back(pPointOneVar);

						}


						for (Point2D pPoint2D : pSectorThreePoints)
						{
							POINT pPointOneVar = ConvertPoint2DToPoint(pPoint2D);
							pPointVectorThree.push_back(pPointOneVar);

						}


						for (Point2D pPoint2D : pSectorFourPoints)
						{
							POINT pPointOneVar = ConvertPoint2DToPoint(pPoint2D);
							pPointVectorFour.push_back(pPointOneVar);

						}


			
				for (UINT pIndex = 0; pIndex <= pPointVectorOne.size() - 2; pIndex++)
				{
					MoveToEx(hDC, pPointVectorOne[pIndex].x, pPointVectorOne[pIndex].y, &pPointVectorOne[pIndex]);
					LineTo(hDC, pPointVectorOne[pIndex + 1].x, pSectorOnePoints[pIndex + 1].yPosition);

				}
	
				MoveToEx(hDC, pPointVectorOne[pPointVectorOne.size() - 1].x, pPointVectorOne[pPointVectorOne.size() - 1].y, &pPointVectorOne[pPointVectorOne.size() - 1]);
				LineTo(hDC, pPointVectorTwo[0].x, pPointVectorTwo[0].y);



				for (UINT pIndex = 0; pIndex <= pPointVectorTwo.size() - 2; pIndex++)
				{
					MoveToEx(hDC, pPointVectorTwo[pIndex].x, pPointVectorTwo[pIndex].y, &pPointVectorTwo[pIndex]);
					LineTo(hDC, pPointVectorTwo[pIndex + 1].x, pSectorTwoPoints[pIndex + 1].yPosition);

				}

				MoveToEx(hDC, pPointVectorTwo[pPointVectorTwo.size() - 1].x, pPointVectorTwo[pPointVectorTwo.size() - 1].y, &pPointVectorTwo[pPointVectorTwo.size() - 1]);
				LineTo(hDC, pPointVectorThree[0].x, pPointVectorThree[0].y);




				for (UINT pIndex = 0; pIndex <= pPointVectorThree.size() - 2; pIndex++)
				{
					MoveToEx(hDC, pPointVectorThree[pIndex].x, pPointVectorThree[pIndex].y, &pPointVectorThree[pIndex]);
					LineTo(hDC, pPointVectorThree[pIndex + 1].x, pSectorThreePoints[pIndex + 1].yPosition);

				}

				MoveToEx(hDC, pPointVectorThree[pPointVectorThree.size() - 1].x, pPointVectorThree[pPointVectorThree.size() - 1].y, &pPointVectorThree[pPointVectorThree.size() - 1]);
				LineTo(hDC, pPointVectorFour[0].x, pPointVectorFour[0].y);



				if (pPointVectorFour.size() > 2)
				{
					for (UINT pIndex = 0; pIndex <= pPointVectorFour.size() - 2; pIndex++)
					{
						MoveToEx(hDC, pPointVectorFour[pIndex].x, pPointVectorFour[pIndex].y, &pPointVectorFour[pIndex]);
						LineTo(hDC, pPointVectorFour[pIndex + 1].x, pSectorFourPoints[pIndex + 1].yPosition);

					}
				}

		

				MoveToEx(hDC, pPointVectorFour[pPointVectorFour.size() - 1].x, pPointVectorFour[pPointVectorFour.size() - 1].y, &pPointVectorFour[pPointVectorFour.size() - 1]);
				LineTo(hDC, pPointOneCopyVar.x, pPointOneCopyVar.y);




			BITMAPINFO Info;
			BITMAPFILEHEADER Header;
			memset(&Info, 0, sizeof(Info));
			memset(&Header, 0, sizeof(Header));
			Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			Info.bmiHeader.biWidth = 800;
			Info.bmiHeader.biHeight = 800;
			Info.bmiHeader.biPlanes = 1;
			Info.bmiHeader.biBitCount = 24;
			Info.bmiHeader.biCompression = BI_RGB;
			Info.bmiHeader.biSizeImage = 800 * 800 * 24;
			Header.bfType = 0x4D42;
			Header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
			char* Pixels = NULL;
			HBITMAP Section = CreateDIBSection(MemDC, &Info, DIB_RGB_COLORS, (void**)&Pixels, 0, 0);
			DeleteObject(SelectObject(MemDC, Section));
			BitBlt(MemDC, 0, 0, 800,  800, hDC, rc.left, rc.top, SRCCOPY);
			DeleteDC(MemDC);
			std::fstream hFile(pFilePath, std::ios::out | std::ios::binary);
			if (hFile.is_open())
			{
				hFile.write((char*)&Header, sizeof(Header));
				hFile.write((char*)&Info.bmiHeader, sizeof(Info.bmiHeader));
				hFile.write(Pixels, (((24 * 800 + 31) & ~31) / 8) * 800);
				hFile.close();
				DeleteObject(Section);
				return true;
			}
			DeleteObject(Section);

			












		}
		EndPaint(hWnd, &ps);

		
		*ppOutputLineArray = pLineArrayValues;
		*ppTotalArea = pTriangleAreaTotal;
		return hr;
	}




	HRESULT Direct2DManagementClass::ConnectPointsUsingLinesSectorsComplex(HINSTANCE hInstance, HWND hWnd,
		float pCentreX,
		float pCentreY,
		vector<Point2D> pPointArrayVector,
		vector<Line2D> *ppOutputLineArray,
		float* ppTotalArea)
	{

		Point2D pCentre;
		pCentre.pPointNumber = 0;
		pCentre.xPosition = pCentreX;
		pCentre.yPosition = pCentreY;
		float pTriangleAreaTotal = 0.0f;


		vector<Line2D> pLineArrayValues = vector<Line2D>();
		HRESULT hr = S_OK;
		BYTE* selectedAreaData = NULL;
		PAINTSTRUCT ps;

		RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
		GetClientRect(hWnd, &rc);
		UINT pWidth = rc.right - rc.left;
		UINT pHeight = rc.bottom - rc.top;
		D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
		D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
		int clippedAreaWidth = clippedArea.right - clippedArea.left;
		int clippedAreaHeight = clippedArea.bottom - clippedArea.top;

		ID2D1SolidColorBrush* pGreenBrush = nullptr;
		ID2D1SolidColorBrush* pRedBrush = nullptr;
		ID2D1SolidColorBrush* pYellowBrush = nullptr;
		ID2D1SolidColorBrush* pBlackBrush = nullptr;
		ID2D1SolidColorBrush* pWhiteBrush = nullptr;

		vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();


		static const WCHAR pText0[] = L"33";
		static const WCHAR pText1[] = L"66";
		static const WCHAR pText2[] = L"100";

		const char* pFilePath = "E:\\RobotData\\my_pixel_array_line_connector.bmp";


		static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
		static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
		static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;


		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);


		BeginPaint(hWnd, &ps);
		HDC hDC = GetDC(hWnd);
		HDC MemDC = CreateCompatibleDC(hDC);

		HGDIOBJ hPen = GetStockObject(BLACK_PEN);
		HGDIOBJ hOldPen = SelectObject(hDC, hPen);




		if (hDC)
		{

			vector<Point2D> pSectorOnePoints = vector<Point2D>();
			vector<Point2D> pSectorTwoPoints = vector<Point2D>();
			vector<Point2D> pSectorThreePoints = vector<Point2D>();
			vector<Point2D> pSectorFourPoints = vector<Point2D>();

			for (Point2D pPointObject : pPointArrayVector)
			{
				if (pPointObject.pSectorNumber == 1)
				{
					pSectorOnePoints.push_back(pPointObject);
				}
				else if (pPointObject.pSectorNumber == 2)
				{
					pSectorTwoPoints.push_back(pPointObject);
				}
				else if (pPointObject.pSectorNumber == 3)
				{
					pSectorThreePoints.push_back(pPointObject);
				}
				else if (pPointObject.pSectorNumber == 4)
				{
					pSectorFourPoints.push_back(pPointObject);
				}

			}



			Point2D pPointOne = pSectorOnePoints[0];
			Point2D pPointTwo = pSectorOnePoints[1];
			Point2D pPointThree = pSectorOnePoints[2];
			Point2D pPointFour = pSectorOnePoints[3];
			Point2D pPointFive = pSectorTwoPoints[0];
			Point2D pPointSix = pSectorTwoPoints[1];;
			Point2D pPointSeven = pSectorTwoPoints[2];
			Point2D pPointEight = pSectorTwoPoints[3];
			Point2D pPointNine = pSectorThreePoints[0];
			Point2D pPointTen = pSectorThreePoints[1];
			Point2D pPointEleven = pSectorThreePoints[2];
			Point2D pPointTwelve = pSectorFourPoints[0];
			Point2D pCentre;
			pCentre.pPointNumber = 0;
			pCentre.xPosition = 430;
			pCentre.yPosition = 410;

			POINT pPointOneVar = ConvertPoint2DToPoint(pPointOne);
			POINT pPointOneVarCopy = ConvertPoint2DToPoint(pPointOne);
			POINT pPointTwoVar = ConvertPoint2DToPoint(pPointTwo);
			POINT pPointThreeVar = ConvertPoint2DToPoint(pPointThree);
			POINT pPointFourVar = ConvertPoint2DToPoint(pPointFour);
			POINT pPointFiveVar = ConvertPoint2DToPoint(pPointFive);
			POINT pPointSixVar = ConvertPoint2DToPoint(pPointSix);
			POINT pPointSevenVar = ConvertPoint2DToPoint(pPointSeven);
			POINT pPointEightVar = ConvertPoint2DToPoint(pPointEight);
			POINT pPointNineVar = ConvertPoint2DToPoint(pPointNine);
			POINT pPointTenVar = ConvertPoint2DToPoint(pPointTen);
			POINT pPointElevenVar = ConvertPoint2DToPoint(pPointEleven);
			POINT pPointTwelveVar = ConvertPoint2DToPoint(pPointTwelve);

			vector<POINT> pPointVector = vector<POINT>();
			pPointVector.push_back(pPointOneVar);
			pPointVector.push_back(pPointTwoVar);
			pPointVector.push_back(pPointThreeVar);
			pPointVector.push_back(pPointFourVar);
			pPointVector.push_back(pPointFiveVar);
			pPointVector.push_back(pPointSixVar);
			pPointVector.push_back(pPointSevenVar);
			pPointVector.push_back(pPointEightVar);
			pPointVector.push_back(pPointNineVar);
			pPointVector.push_back(pPointTenVar);
			pPointVector.push_back(pPointElevenVar);
			pPointVector.push_back(pPointTwelveVar);

			for (UINT pIndex = 0; pIndex <= pPointVector.size() - 2; pIndex++)
			{
				MoveToEx(hDC, pPointVector[pIndex].x, pPointVector[pIndex].y, &pPointVector[pIndex]);
				LineTo(hDC, pPointVector[pIndex + 1].x, pPointVector[pIndex + 1].y);

			}

			MoveToEx(hDC, pPointVector[pPointVector.size() - 1].x, pPointVector[pPointVector.size() - 1].y, &pPointVector[pPointVector.size() - 1]);
			LineTo(hDC, pPointOneVarCopy.x, pPointOneVarCopy.y);





			BITMAPINFO Info;
			BITMAPFILEHEADER Header;
			memset(&Info, 0, sizeof(Info));
			memset(&Header, 0, sizeof(Header));
			Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			Info.bmiHeader.biWidth = 800;
			Info.bmiHeader.biHeight = 800;
			Info.bmiHeader.biPlanes = 1;
			Info.bmiHeader.biBitCount = 24;
			Info.bmiHeader.biCompression = BI_RGB;
			Info.bmiHeader.biSizeImage = 800 * 800 * 24;
			Header.bfType = 0x4D42;
			Header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
			char* Pixels = NULL;
			HBITMAP Section = CreateDIBSection(MemDC, &Info, DIB_RGB_COLORS, (void**)&Pixels, 0, 0);
			DeleteObject(SelectObject(MemDC, Section));
			BitBlt(MemDC, 0, 0, 800, 800, hDC, rc.left, rc.top, SRCCOPY);
			DeleteDC(MemDC);
			std::fstream hFile(pFilePath, std::ios::out | std::ios::binary);
			if (hFile.is_open())
			{
				hFile.write((char*)&Header, sizeof(Header));
				hFile.write((char*)&Info.bmiHeader, sizeof(Info.bmiHeader));
				hFile.write(Pixels, (((24 * 800 + 31) & ~31) / 8) * 800);
				hFile.close();
				DeleteObject(Section);
				return true;
			}
			DeleteObject(Section);














		}
		EndPaint(hWnd, &ps);


		*ppOutputLineArray = pLineArrayValues;
		*ppTotalArea = pTriangleAreaTotal;
		return hr;
	}





	HRESULT Direct2DManagementClass::ConnectPointsUsingLinesDirectForSquare(HINSTANCE hInstance, HWND hWnd,
		vector<Point2D> pPointArrayVector)
	{

		HRESULT hr = S_OK;
		BYTE* selectedAreaData = NULL;
		PAINTSTRUCT ps;

		RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
		GetClientRect(hWnd, &rc);
		UINT pWidth = rc.right - rc.left;
		UINT pHeight = rc.bottom - rc.top;
		D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
		D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
		int clippedAreaWidth = clippedArea.right - clippedArea.left;
		int clippedAreaHeight = clippedArea.bottom - clippedArea.top;

		ID2D1SolidColorBrush* pGreenBrush = nullptr;
		ID2D1SolidColorBrush* pRedBrush = nullptr;
		ID2D1SolidColorBrush* pYellowBrush = nullptr;
		ID2D1SolidColorBrush* pBlackBrush = nullptr;
		ID2D1SolidColorBrush* pWhiteBrush = nullptr;

		vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();



		static const WCHAR pText0[] = L"33";
		static const WCHAR pText1[] = L"66";
		static const WCHAR pText2[] = L"100";

		const char* pFilePath = "E:\\RobotData\\my_pixel_array_line_connector_square.bmp";


		static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
		static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
		static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;


		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);


		BeginPaint(hWnd, &ps);
		HDC hDC = GetDC(hWnd);
		HDC MemDC = CreateCompatibleDC(hDC);

		HGDIOBJ hPen = GetStockObject(BLACK_PEN);
		HGDIOBJ hOldPen = SelectObject(hDC, hPen);




		if (hDC)
		{

			Point2D pPointOne = GetPointByNumber(pPointArrayVector, 1);
			Point2D pPointTwo = GetPointByNumber(pPointArrayVector, 2);
			Point2D pPointThree = GetPointByNumber(pPointArrayVector, 3);
			Point2D pPointFour = GetPointByNumber(pPointArrayVector, 4);
			Point2D pPointFive = GetPointByNumber(pPointArrayVector, 5);
			Point2D pPointSix = GetPointByNumber(pPointArrayVector, 6);
			Point2D pPointSeven = GetPointByNumber(pPointArrayVector, 7);
			Point2D pPointEight = GetPointByNumber(pPointArrayVector, 8);
			Point2D pPointNine = GetPointByNumber(pPointArrayVector, 9);
			Point2D pPointTen = GetPointByNumber(pPointArrayVector, 10);
			Point2D pPointEleven = GetPointByNumber(pPointArrayVector, 11);
			Point2D pPointTwelve = GetPointByNumber(pPointArrayVector, 12);


			POINT pPointOneVar;
			pPointOneVar.x = pPointOne.xPosition;
			pPointOneVar.y = pPointOne.yPosition;

			POINT pPointOneVarCopy;
			pPointOneVarCopy.x = pPointOne.xPosition;
			pPointOneVarCopy.y = pPointOne.yPosition;


			POINT pPointTwoVar;
			pPointTwoVar.x = pPointTwo.xPosition;
			pPointTwoVar.y = pPointTwo.yPosition;


			POINT pPointThreeVar;
			pPointThreeVar.x = pPointThree.xPosition;
			pPointThreeVar.y = pPointThree.yPosition;

			POINT pPointFourVar;
			pPointFourVar.x = pPointFour.xPosition;
			pPointFourVar.y = pPointFour.yPosition;


			POINT pPointFiveVar;
			pPointFiveVar.x = pPointFive.xPosition;
			pPointFiveVar.y = pPointFive.yPosition;

			POINT pPointSixVar;
			pPointSixVar.x = pPointSix.xPosition;
			pPointSixVar.y = pPointSix.yPosition;

			POINT pPointSevenVar;
			pPointSevenVar.x = pPointSeven.xPosition;
			pPointSevenVar.y = pPointSeven.yPosition;

			POINT pPointEightVar;
			pPointEightVar.x = pPointEight.xPosition;
			pPointEightVar.y = pPointEight.yPosition;


			POINT pPointNineVar;
			pPointNineVar.x = pPointNine.xPosition;
			pPointNineVar.y = pPointNine.yPosition;


			POINT pPointTenVar;
			pPointTenVar.x = pPointTen.xPosition;
			pPointTenVar.y = pPointTen.yPosition;

			POINT pPointElevenVar;
			pPointElevenVar.x = pPointEleven.xPosition;
			pPointElevenVar.y = pPointEleven.yPosition;


			POINT pPointTwelveVar;
			pPointTwelveVar.x = pPointTwelve.xPosition;
			pPointTwelveVar.y = pPointTwelve.yPosition;




			MoveToEx(hDC, pPointOneVar.x, pPointOneVar.y, &pPointOneVar);
			LineTo(hDC, pPointSevenVar.x, pPointSevenVar.y);
			MoveToEx(hDC, pPointSevenVar.x, pPointSevenVar.y, &pPointSevenVar);
			LineTo(hDC, pPointNineVar.x, pPointNineVar.y);
			MoveToEx(hDC, pPointNineVar.x, pPointNineVar.y, &pPointNineVar);
			LineTo(hDC, pPointElevenVar.x, pPointElevenVar.y);
			MoveToEx(hDC, pPointElevenVar.x, pPointElevenVar.y, &pPointElevenVar);
			LineTo(hDC, pPointFourVar.x, pPointFourVar.y);
			MoveToEx(hDC, pPointFourVar.x, pPointFourVar.y, &pPointFourVar);
			LineTo(hDC, pPointFiveVar.x, pPointFiveVar.y);
			MoveToEx(hDC, pPointFiveVar.x, pPointFiveVar.y, &pPointFiveVar);
			LineTo(hDC, pPointSixVar.x, pPointSixVar.y);
			MoveToEx(hDC, pPointSixVar.x, pPointSixVar.y, &pPointSixVar);
			LineTo(hDC, pPointTwelveVar.x, pPointTwelveVar.y);
			MoveToEx(hDC, pPointTwelveVar.x, pPointEightVar.y, &pPointTwelveVar);
			LineTo(hDC, pPointTenVar.x, pPointTenVar.y);
			MoveToEx(hDC, pPointTenVar.x, pPointTenVar.y, &pPointTenVar);
			LineTo(hDC, pPointEightVar.x, pPointEightVar.y);
			MoveToEx(hDC, pPointEightVar.x, pPointEightVar.y, &pPointEightVar);
			LineTo(hDC, pPointThreeVar.x, pPointThreeVar.y);
			MoveToEx(hDC, pPointThreeVar.x, pPointThreeVar.y, &pPointThreeVar);
			LineTo(hDC, pPointTwoVar.x, pPointTwoVar.y);
			MoveToEx(hDC, pPointTwoVar.x, pPointTwoVar.y, &pPointTwoVar);
			LineTo(hDC, pPointOneVarCopy.x, pPointOneVarCopy.y);


			BITMAPINFO Info;
			BITMAPFILEHEADER Header;
			memset(&Info, 0, sizeof(Info));
			memset(&Header, 0, sizeof(Header));
			Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			Info.bmiHeader.biWidth = 800;
			Info.bmiHeader.biHeight = 800;
			Info.bmiHeader.biPlanes = 1;
			Info.bmiHeader.biBitCount = 24;
			Info.bmiHeader.biCompression = BI_RGB;
			Info.bmiHeader.biSizeImage = 800 * 800 * 24;
			Header.bfType = 0x4D42;
			Header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
			char* Pixels = NULL;
			HBITMAP Section = CreateDIBSection(MemDC, &Info, DIB_RGB_COLORS, (void**)&Pixels, 0, 0);
			DeleteObject(SelectObject(MemDC, Section));
			BitBlt(MemDC, 0, 0, 800, 800, hDC, rc.left, rc.top, SRCCOPY);
			DeleteDC(MemDC);
			std::fstream hFile(pFilePath, std::ios::out | std::ios::binary);
			if (hFile.is_open())
			{
				hFile.write((char*)&Header, sizeof(Header));
				hFile.write((char*)&Info.bmiHeader, sizeof(Info.bmiHeader));
				hFile.write(Pixels, (((24 * 800 + 31) & ~31) / 8) * 800);
				hFile.close();
				DeleteObject(Section);
				return true;
			}
			DeleteObject(Section);














		}
		EndPaint(hWnd, &ps);



		return hr;
	}





	D2D1_POINT_2F Direct2DManagementClass::ConvertPointToD2DPoint(Point2D pPoint2DObject)
	{
		D2D1_POINT_2F pPointObjectVar = D2D1::Point2F(pPoint2DObject.xPosition, pPoint2DObject.yPosition);
		return pPointObjectVar;
	}
























	HRESULT Direct2DManagementClass::CreateBitmapDrawingClipObjectWitSpecifiedStep(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pWidth, float pHeight,
		float pStepValueX,
		float pStepValueY,
		vector<PixelObject> *ppPixelObject)
	{

		IWICBitmapSource* pSource = nullptr;
		IWICBitmapSource* pSecondarySource = nullptr;

		IWICBitmap* pBitmapObject = nullptr;
		IWICBitmapLock* pBitmapLock = nullptr;
		IWICBitmapLock* pBitmapLock1 = nullptr;
		IWICBitmapClipper *pIClipper = nullptr;
		BYTE* pixels = nullptr;
		HRESULT hr = S_OK;
		WCHAR szFileName[MAX_PATH];
		WICRect rcLock = { pXPosition, pYPosition, pWidth, pHeight };
		ID2D1Image* outputImage = nullptr;
		vector <PixelObject> pixelDataVector = vector <PixelObject>();
		HDC src = NULL;
		HDC hDC = NULL;
		WICPixelFormatGUID pixelFormat;
		IWICBitmapSource* piConverter = nullptr;
		UINT pImageHeight = NULL;
		UINT pImageWidth = NULL;
		BYTE* pixelArrayPointer = nullptr;
		D2D1_RECT_F rectangle = D2D1::RectF(0, 0, pXPosition + pWidth, pYPosition + pHeight);
		UINT cbBufferSize = 0;
		BYTE* pixelDataArray = NULL;
		IWICBitmapDecoder *pDecoder = NULL;
		IWICBitmapDecoder* pImageDecoder = nullptr;
		IWICBitmapFrameDecode *pFrame = NULL;
		UINT uiWidth = 0;
		UINT uiHeight = 0;
		WICRect rcClip = { pXPosition, pYPosition, pWidth, pHeight };


		fstream pixelDataFile;
		pixelDataFile.open("C:\\pixels\\ImageClippingExperimentsTestsWithAStep.txt", fstream::in | fstream::out | fstream::trunc);
		pixelDataFile.clear();

		pixelDataFile.clear();
		pixelDataFile << " " << endl;
		pixelDataFile << " " << endl;
		pixelDataFile << "New Operation Begin" << endl;
		time_t currentTimeVar = time(0);
		tm* currentTime = localtime(&currentTimeVar);
		cout << "current time" << currentTimeVar << endl;
		pixelDataFile << "current time" << currentTimeVar << endl;

		if (LocateImageFile(hWnd, szFileName, ARRAYSIZE(szFileName)))
		{

			// Prepare iterator pairs to iterate the file content!

			cout << "Stage 1: Decoder" << endl;
			pixelDataFile << "Stage 1: Decoder" << endl;
			time_t currentTimeVar = time(0);
			tm* currentTime = localtime(&currentTimeVar);

			cout << "Decoder Creation Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
			pixelDataFile << "Decoder Creation Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;

			hr = m_pWICFactory->CreateDecoderFromFilename(
				szFileName,
				NULL,
				GENERIC_READ,
				WICDecodeMetadataCacheOnDemand,
				&pImageDecoder);

			currentTimeVar = time(0);
			currentTime = localtime(&currentTimeVar);

			cout << "Decoder Creation Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
			pixelDataFile << "Decoder Creation Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;


			cout << "Stage 2: Frame" << endl;
			pixelDataFile << "Stage 2: Frame" << endl;
			currentTimeVar = time(0);
			currentTime = localtime(&currentTimeVar);
			cout << "Frame Creation Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
			pixelDataFile << "Frame Creation Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;



			if (SUCCEEDED(hr))
			{
				hr = pImageDecoder->GetFrame(0, &pFrame);
			}


			currentTimeVar = time(0);
			currentTime = localtime(&currentTimeVar);
			cout << "Frame Creation Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
			pixelDataFile << "Frame Creation Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;

			cout << "Stage 3: Bitmap Source" << endl;
			pixelDataFile << "Stage 3: Bitmap Source" << endl;
			currentTimeVar = time(0);
			currentTime = localtime(&currentTimeVar);

			cout << "Bitmap Source Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
			pixelDataFile << "Frame Creation Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;

			if (SUCCEEDED(hr))
			{
				pSource = pFrame;
				pSource->AddRef();
				pSource->GetSize(&uiWidth, &uiHeight);

				pSecondarySource = pFrame;
				pSecondarySource->AddRef();
				pSecondarySource->GetSize(&uiWidth, &uiHeight);

				IWICPalette* pImagePalette = nullptr;
				pSecondarySource->CopyPalette(pImagePalette);
			}

			WICRect rc = { pXPosition, pYPosition, pWidth, pHeight };


			currentTimeVar = time(0);
			currentTime = localtime(&currentTimeVar);
			cout << "Bitmap Source Creation Terminated At At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
			pixelDataFile << "Bitmap Source Creation Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;

			cout << "Stage 4: Converting Wic Formats to Direct2D Formats" << endl;
			pixelDataFile << "Stage 4: Converting Wic Formats to Direct2D Formats" << endl;
			currentTimeVar = time(0);
			currentTime = localtime(&currentTimeVar);
			cout << "Format Conversion  Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
			pixelDataFile << "Format Conversion Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;



			// Initialize the clipper with the given rectangle of the frame's image data.

			if (SUCCEEDED(hr))
			{
				SafeReleaseA(&m_pConvertedSourceBitmap);
				hr = m_pWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
			}

			if (SUCCEEDED(hr))
			{
				hr = m_pConvertedSourceBitmap->Initialize(
					pSecondarySource,
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone,
					NULL,
					0.f,
					WICBitmapPaletteTypeCustom);

			}
			if (SUCCEEDED(hr))
			{
				hr = CreateDeviceResources(hInstance, hWnd);
			}
			if (SUCCEEDED(hr))
			{

				SafeReleaseA(&m_pD2DBitmap);
				hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);

			}

			currentTimeVar = time(0);
			currentTime = localtime(&currentTimeVar);
			cout << "Format Conversion terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
			pixelDataFile << "Format Conversion terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;


			cout << "Stage 5: Drawing Pixels with Direct2D" << endl;
			pixelDataFile << "Stage 5: Drawing Pixels with Direct2D" << endl;
			currentTimeVar = time(0);
			currentTime = localtime(&currentTimeVar);
			cout << "Drawing  Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
			pixelDataFile << "Drawing Conversion Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;



			if (SUCCEEDED(hr))
			{
				D2D1_SIZE_F pSize = m_pD2DBitmap->GetSize();
				pImageWidth = pSize.width;
				pImageHeight = pSize.height;

				hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidColorBrush);
			}

			if (SUCCEEDED(hr))
			{


				hr = DrawD2DContentAxisAlignedClip(hInstance, hWnd, m_pD2DBitmap, rectangle);

			}

			currentTimeVar = time(0);
			currentTime = localtime(&currentTimeVar);
			cout << "Drawing Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
			pixelDataFile << "Drawing terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;


			cout << "Stage 6: Convertion of image data to GDI HBITMAP For Pixel Extraction" << endl;
			pixelDataFile << "Stage 6: Convertion of image data to GDI HBITMAP For Pixel Extraction" << endl;
			currentTimeVar = time(0);
			currentTime = localtime(&currentTimeVar);
			cout << "Convertion Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
			pixelDataFile << "Convertion Started: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;



			IWICBitmapSource* pConverter = nullptr;
			WICPixelFormatGUID pixelFormat;

			if (SUCCEEDED(hr))
			{


				pSecondarySource->AddRef();
				pSecondarySource->GetSize(&uiWidth, &uiHeight);
			}

			IWICPixelFormatInfo* pixelFormatInfo = nullptr;
			if (SUCCEEDED(hr))
			{
				pSecondarySource->GetPixelFormat(&pixelFormat);
			}

			if (!IsEqualGUID(pixelFormat, GUID_WICPixelFormat128bppPRGBAFloat))
			{

				hr = WICConvertBitmapSource(GUID_WICPixelFormat128bppPRGBAFloat, pSecondarySource, &pConverter);
				IWICBitmapLock* pBitmapLock = nullptr;


				if (SUCCEEDED(hr))
				{
					pSecondarySource->Release();     // the converter has a reference to the source
					pSecondarySource = NULL;         // so we don't need it anymore.
					pSecondarySource = pConverter;   // let's treat the 128bppPABGR converter as the source
				}


				IWICPalette* pImagePalette = nullptr;
				pSecondarySource->CopyPalette(pImagePalette);
			}




			CImageFileData* pImageClass = new CImageFileData((char*)pixelArrayPointer);

			if (pConverter)
			{
				UINT cbStride = pWidth * sizeof(float) * 4;
				UINT cbBufferSize = cbStride;
				float *pixels = new float[cbBufferSize / sizeof(float)];
				if (pHeight < rc.Height && pWidth < rc.Width)
				{
					rc.Height = pHeight;
					rc.Width = pWidth;
				}
				else
				{
					pWidth = rc.Width;
					pHeight = rc.Height;
				}
			}


			UINT bSize = 0;




			UINT rowPitch1 = 4 * pWidth;
			UINT stride = 0; // stride  = bytes per row					 // (width * (bpp/8)) + padding and round to the nearest DWORD
			stride = (((((UINT)pWidth * 32) + 31) & ~31) >> 3);
			bSize = pHeight * pWidth;
			BYTE *pixelArray = new BYTE[(UINT)pHeight * stride];


			pSecondarySource->CopyPixels(NULL, stride, bSize, pixelArray);
			HDC hDC = NULL;
			hDC = GetDC(hWnd);

			COLORREF bkColor = RGB(255, 255, 255);

			SetBkColor(hDC, bkColor);

			HBITMAP map = CreateBitmap((UINT)pWidth,// width. 512 in my case
				(UINT)pHeight, // height
				1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
				8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
				(void*)pixelArray); // pointer to array
			// Temp HDC to copy picture
			src = CreateCompatibleDC(hDC); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
			SelectObject(src, map); // Inserting picture into our temp HDC
			// Copy image from temp HDC to window

			currentTimeVar = time(0);
			currentTime = localtime(&currentTimeVar);
			cout << "Convertion Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
			pixelDataFile << "Drawing terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;


			cout << "Stage 7: Reading Pixels and Preparing Pixel Vector" << endl;
			pixelDataFile << "Stage 7: Reading Pixels and Preparing Pixel Vector" << endl;
			currentTimeVar = time(0);
			currentTime = localtime(&currentTimeVar);
			cout << "Pixel Extraction Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
			pixelDataFile << "Pixel Extraction Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;





			std::cout << "Pixels Begin" << endl;

			int pixelCounter = 0;

			//	pBitmapObj->GetResolution(&pWidth, &pHeight);

			UINT pYVal = 0;
			UINT pXVal = 0;


			for (pXVal = pXPosition; pXVal < pXPosition + pWidth; pXVal += pStepValueX)
			{
				for (pYVal = pYPosition; pYVal < pYPosition + pHeight; pYVal += pStepValueY)
					// Your code here
				{

					std::cout << "Current X Value: " << pXVal << " " << "Current Y Value: " << pYVal << endl;
					COLORREF pCurrentPixel = GetPixel(hDC, pXVal, pYVal);
					int pRedValue = GetRValue(pCurrentPixel);
					int pGreenValue = GetGValue(pCurrentPixel);
					int pBlueValue = GetBValue(pCurrentPixel);
					PixelObject pixelObj = PixelObject(pixelCounter, pXVal, pYVal, pRedValue, pGreenValue, pBlueValue);
					pixelDataVector.push_back(pixelObj);
					std::cout << "Amount of total pixels processed  = " << pixelCounter << endl;
					pixelCounter = pixelCounter + 1;

				}
			}




			std::cout << "The size of pixel buffer = " << pixelDataVector.size() << endl;
			int pAmountOfPixelsLeft = 0;
			pAmountOfPixelsLeft = pixelDataVector.size();


			pixelDataFile.clear();
			pixelDataFile << " " << endl;
			pixelDataFile << " " << endl;
			pixelDataFile << "New Operation Begin" << endl;
			currentTimeVar = time(0);
			currentTime = localtime(&currentTimeVar);
			cout << "Pixel Extraction Finished aAt: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
			pixelDataFile << "Pixel Extraction Finished aAt: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;

			for (PixelObject currentPixel : pixelDataVector)
			{


				if ((currentPixel.RMask == 255.0f) && (currentPixel.GMask == 255.0f) && (currentPixel.BMask == 255.0f))

				{
					std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
						"X Position = " << currentPixel.xPosition << " " <<
						"Y (Position = " << currentPixel.yPosition << " " <<
						"The Value Of Red = " << currentPixel.RMask << " "
						<< "The Value Of Green = " << currentPixel.GMask << " " <<
						"The Value Of Blue = " << currentPixel.BMask << endl;
				}


				pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
				pixelDataFile << "X Position = " << currentPixel.xPosition << endl;
				pixelDataFile << "Y Position = " << currentPixel.yPosition << endl;
				pixelDataFile << "The Value Of Red = " << currentPixel.RMask << endl;
				pixelDataFile << "The Value Of Green = " << currentPixel.GMask << endl;
				pixelDataFile << "The Value Of Blue = " << currentPixel.BMask << endl;

			}



			pAmountOfPixelsLeft--;

		}


		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Clipping Terminated at: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Clipping Terminated at: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile.close();











		ReleaseDC(hWnd, src);
		ReleaseDC(hWnd, hDC);
		DeleteDC(src);
		DeleteDC(hDC);
		SafeReleaseA(&pDecoder);
		SafeReleaseA(&pFrame);
		*ppPixelObject = pixelDataVector;

		return hr;


	}





	
	







	HRESULT Direct2DManagementClass::CreateBitmapDrawingClipObjectWitSpecifiedStepForASpecifiedFile(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pWidth, float pHeight,
		float pStepValueX,
		float pStepValueY,
		LPWSTR pFileName,
		vector<PixelObject> *ppPixelObject)
	{
		ofstream myfile("imagebuffer.txt");
		//	MessageBoxA(hWnd, (LPCSTR)"Эта Функция Рисует Клиповую Область Изображения", (LPCSTR)"Message", 0);
		myfile << "Please write this text to a file.\n this text is written using C++\n";
		IWICBitmapSource* pSource = nullptr;
		IWICBitmapSource* pSecondarySource = nullptr;

		IWICBitmap* pBitmapObject = nullptr;
		IWICBitmapLock* pBitmapLock = nullptr;
		IWICBitmapLock* pBitmapLock1 = nullptr;
		IWICBitmapClipper *pIClipper = nullptr;
		BYTE* pixels = nullptr;
		HRESULT hr = S_OK;
		WCHAR szFileName[MAX_PATH];
		WICRect rcLock = { pXPosition, pYPosition, pWidth, pHeight };
		ID2D1Image* outputImage = nullptr;
		vector <PixelObject> pixelDataVector = vector <PixelObject>();
		HDC src = NULL;
		HDC hDC = NULL;
		WICPixelFormatGUID pixelFormat;
		IWICBitmapSource* piConverter = nullptr;
		UINT pImageHeight = NULL;
		UINT pImageWidth = NULL;
		BYTE* pixelArrayPointer = nullptr;
		D2D1_RECT_F rectangle = D2D1::RectF(0, 0, pXPosition + pWidth, pYPosition + pHeight);
		UINT cbBufferSize = 0;
		BYTE* pixelDataArray = NULL;
		IWICBitmapDecoder *pDecoder = NULL;
		IWICBitmapDecoder* pImageDecoder = nullptr;
		IWICBitmapFrameDecode *pFrame = NULL;
		UINT uiWidth = 0;
		UINT uiHeight = 0;
		WICRect rcClip = { pXPosition, pYPosition, pWidth, pHeight };

		fstream pixelDataFile;
		pixelDataFile.open("C:\\pixels\\ImageClipperTests.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
		pixelDataFile.clear();

		

			// Prepare iterator pairs to iterate the file content!



			hr = m_pWICFactory->CreateDecoderFromFilename(
				pFileName,
				NULL,
				GENERIC_READ,
				WICDecodeMetadataCacheOnDemand,
				&pImageDecoder);




			if (SUCCEEDED(hr))
			{
				hr = pImageDecoder->GetFrame(0, &pFrame);
			}



			if (SUCCEEDED(hr))
			{
				pSource = pFrame;
				pSource->AddRef();
				pSource->GetSize(&uiWidth, &uiHeight);

				pSecondarySource = pFrame;
				pSecondarySource->AddRef();
				pSecondarySource->GetSize(&uiWidth, &uiHeight);

				IWICPalette* pImagePalette = nullptr;
				pSecondarySource->CopyPalette(pImagePalette);
			}

			WICRect rc = { pXPosition, pYPosition, pWidth, pHeight };



			if (SUCCEEDED(hr))
			{
				SafeReleaseA(&m_pConvertedSourceBitmap);
				hr = m_pWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
			}

			if (SUCCEEDED(hr))
			{
				hr = m_pConvertedSourceBitmap->Initialize(
					pSecondarySource,
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone,
					NULL,
					0.f,
					WICBitmapPaletteTypeCustom);

			}
			if (SUCCEEDED(hr))
			{
				hr = CreateDeviceResources(hInstance, hWnd);
			}
			if (SUCCEEDED(hr))
			{

				SafeReleaseA(&m_pD2DBitmap);
				hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);

			}



			if (SUCCEEDED(hr))
			{
				D2D1_SIZE_F pSize = m_pD2DBitmap->GetSize();
				pImageWidth = pSize.width;
				pImageHeight = pSize.height;

				hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidColorBrush);
			}

			if (SUCCEEDED(hr))
			{


					hr = DrawD2DContentAxisAlignedClip(hInstance, hWnd, m_pD2DBitmap, rectangle);

			}


			IWICBitmapSource* pConverter = nullptr;
		
			if (SUCCEEDED(hr))
			{


				pSecondarySource->AddRef();
				pSecondarySource->GetSize(&uiWidth, &uiHeight);
			}

			IWICPixelFormatInfo* pixelFormatInfo = nullptr;
			if (SUCCEEDED(hr))
			{
				pSecondarySource->GetPixelFormat(&pixelFormat);
			}

			if (!IsEqualGUID(pixelFormat, GUID_WICPixelFormat128bppPRGBAFloat))
			{

				hr = WICConvertBitmapSource(GUID_WICPixelFormat128bppPRGBAFloat, pSecondarySource, &pConverter);
				IWICBitmapLock* pBitmapLock = nullptr;


				if (SUCCEEDED(hr))
				{
					pSecondarySource->Release();     // the converter has a reference to the source
					pSecondarySource = NULL;         // so we don't need it anymore.
					pSecondarySource = pConverter;   // let's treat the 128bppPABGR converter as the source
				}


				IWICPalette* pImagePalette = nullptr;
				pSecondarySource->CopyPalette(pImagePalette);
			}




			CImageFileData* pImageClass = new CImageFileData((char*)pixelArrayPointer);

			if (pConverter)
			{
				UINT cbStride = pWidth * sizeof(float) * 4;
				UINT cbBufferSize = cbStride;
				float *pixels = new float[cbBufferSize / sizeof(float)];
				if (pHeight < rc.Height && pWidth < rc.Width)
				{
					rc.Height = pHeight;
					rc.Width = pWidth;
				}
				else
				{
					pWidth = rc.Width;
					pHeight = rc.Height;
				}
			}


			UINT bSize = 0;




			UINT rowPitch1 = 4 * pWidth;
			UINT stride = 0; // stride  = bytes per row					 // (width * (bpp/8)) + padding and round to the nearest DWORD
			stride = (((((UINT)pWidth * 32) + 31) & ~31) >> 3);
			bSize = pHeight * pWidth;
			BYTE *pixelArray = new BYTE[(UINT)pHeight * stride];


			pSecondarySource->CopyPixels(NULL, stride, bSize, pixelArray);
			 hDC = NULL;
			hDC = GetDC(hWnd);

			COLORREF bkColor = RGB(255, 255, 255);

			SetBkColor(hDC, bkColor);

			HBITMAP map = CreateBitmap((UINT)pWidth,// width. 512 in my case
				(UINT)pHeight, // height
				1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
				8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
				(void*)pixelArray); // pointer to array
			// Temp HDC to copy picture
			src = CreateCompatibleDC(hDC); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
			SelectObject(src, map); // Inserting picture into our temp HDC
			// Copy image from temp HDC to window


			std::cout << "Pixels Begin" << endl;

			int pixelCounter = 0;




			std::cout << "Pixels Begin" << endl;

			pixelCounter = 0;


			//	pBitmapObj->GetResolution(&pWidth, &pHeight);

			UINT pYVal = 0;
			UINT pXVal = 0;
			for (pXVal = pXPosition; pXVal < pXPosition + pWidth; pXVal += pStepValueX)
			{
				for (pYVal = pYPosition; pYVal < pYPosition + pHeight; pYVal += pStepValueY)
					// Your code here
				{

					std::cout << "Current X Value: " << pXVal << " " << "Current Y Value: " << pYVal << endl;
					COLORREF pCurrentPixel = GetPixel(hDC, pXVal, pYVal);
					int pRedValue = GetRValue(pCurrentPixel);
					int pGreenValue = GetGValue(pCurrentPixel);
					int pBlueValue = GetBValue(pCurrentPixel);
					PixelObject pixelObj = PixelObject(pixelCounter, pXVal, pYVal, pRedValue, pGreenValue, pBlueValue);
					pixelDataVector.push_back(pixelObj);
					std::cout << "Amount of total pixels processed  = " << pixelCounter << endl;
					pixelCounter = pixelCounter + 1;

				}
			}




			std::cout << "The size of pixel buffer = " << pixelDataVector.size() << endl;
			int pAmountOfPixelsLeft = 0;
			pAmountOfPixelsLeft = pixelDataVector.size();


			pixelDataFile.clear();
			pixelDataFile << " " << endl;
			pixelDataFile << " " << endl;
			pixelDataFile << "New Operation Begin" << endl;
			time_t currentTime = time(0);
			pixelDataFile << "current time" << currentTime << endl;
			for (PixelObject currentPixel : pixelDataVector)
			{


				if ((currentPixel.RMask != 255.0f) && (currentPixel.GMask != 255.0f) && (currentPixel.BMask != 255.0f))

				{
					//				std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
					//					"X Position = " << currentPixel.xPosition << " " <<
					//					"Y (Position = " << currentPixel.yPosition << " " <<
					//					"The Value Of Red = " << currentPixel.RMask << " "
					//					<< "The Value Of Green = " << currentPixel.GMask << " " <<
					//					"The Value Of Blue = " << currentPixel.BMask << endl;
					//			}


					//		pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
					//			pixelDataFile << "X Position = " << pXPosition << endl;
					//			pixelDataFile << "Y Position = " << pYPosition << endl;
					//			pixelDataFile << "The Value Of Red = " << currentPixel.RMask << endl;
					//			pixelDataFile << "The Value Of Green = " << currentPixel.GMask << endl;
					//			pixelDataFile << "The Value Of Blue = " << currentPixel.BMask << endl;

				}


				pAmountOfPixelsLeft--;

				pixelDataFile << "New Operation End" << endl;
				pixelDataFile << " " << endl;
				pixelDataFile << " " << endl;
				pixelDataFile.close();
				std::cout << "Pixels END" << endl;



				ReleaseDC(hWnd, src);
				ReleaseDC(hWnd, hDC);
				DeleteDC(src);
				DeleteDC(hDC);
				SafeReleaseA(&pDecoder);
				SafeReleaseA(&pFrame);
				myfile.close();
				*ppPixelObject = pixelDataVector;
			}
			return hr;

		}













	


	HRESULT Direct2DManagementClass::CreateBitmapDrawingClipObjectWitSpecifiedStepForXAxis(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pWidth, float pHeight,
		float pStepValueX,
		vector<PixelObject> *ppPixelObject)
	{
		ofstream myfile("imagebuffer.txt");
		//	MessageBoxA(hWnd, (LPCSTR)"Эта Функция Рисует Клиповую Область Изображения", (LPCSTR)"Message", 0);
		myfile << "Please write this text to a file.\n this text is written using C++\n";
		IWICBitmapSource* pSource = nullptr;
		IWICBitmapSource* pSecondarySource = nullptr;

		IWICBitmap* pBitmapObject = nullptr;
		IWICBitmapLock* pBitmapLock = nullptr;
		IWICBitmapLock* pBitmapLock1 = nullptr;
		IWICBitmapClipper *pIClipper = nullptr;
		BYTE* pixels = nullptr;
		HRESULT hr = S_OK;
		WCHAR szFileName[MAX_PATH];
		WICRect rcLock = { pXPosition, pYPosition, pWidth, pHeight };
		ID2D1Image* outputImage = nullptr;
		vector <PixelObject> pixelDataVector = vector <PixelObject>();
		HDC src = NULL;
		HDC hDC = NULL;
		WICPixelFormatGUID pixelFormat;
		IWICBitmapSource* piConverter = nullptr;
		UINT pImageHeight = NULL;
		UINT pImageWidth = NULL;
		BYTE* pixelArrayPointer = nullptr;
		D2D1_RECT_F rectangle = D2D1::RectF(0, 0, pXPosition + pWidth, pYPosition + pHeight);
		UINT cbBufferSize = 0;
		BYTE* pixelDataArray = NULL;
		IWICBitmapDecoder *pDecoder = NULL;
		IWICBitmapDecoder* pImageDecoder = nullptr;
		IWICBitmapFrameDecode *pFrame = NULL;
		UINT uiWidth = 0;
		UINT uiHeight = 0;
		WICRect rcClip = { pXPosition, pYPosition, pWidth, pHeight };

		fstream pixelDataFile;
		pixelDataFile.open("C:\\pixels\\ImageClipperTests.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
		pixelDataFile.clear();

		if (LocateImageFile(hWnd, szFileName, ARRAYSIZE(szFileName)))
		{

			// Prepare iterator pairs to iterate the file content!



			hr = m_pWICFactory->CreateDecoderFromFilename(
				szFileName,
				NULL,
				GENERIC_READ,
				WICDecodeMetadataCacheOnDemand,
				&pImageDecoder);




			if (SUCCEEDED(hr))
			{
				hr = pImageDecoder->GetFrame(0, &pFrame);
			}



			if (SUCCEEDED(hr))
			{
				pSource = pFrame;
				pSource->AddRef();
				pSource->GetSize(&uiWidth, &uiHeight);

				pSecondarySource = pFrame;
				pSecondarySource->AddRef();
				pSecondarySource->GetSize(&uiWidth, &uiHeight);

				IWICPalette* pImagePalette = nullptr;
				pSecondarySource->CopyPalette(pImagePalette);
			}

			WICRect rc = { pXPosition, pYPosition, pWidth, pHeight };



			if (SUCCEEDED(hr))
			{
				SafeReleaseA(&m_pConvertedSourceBitmap);
				hr = m_pWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
			}

			if (SUCCEEDED(hr))
			{
				hr = m_pConvertedSourceBitmap->Initialize(
					pSecondarySource,
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone,
					NULL,
					0.f,
					WICBitmapPaletteTypeCustom);

			}
			if (SUCCEEDED(hr))
			{
				hr = CreateDeviceResources(hInstance, hWnd);
			}
			if (SUCCEEDED(hr))
			{

				SafeReleaseA(&m_pD2DBitmap);
				hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);

			}



			if (SUCCEEDED(hr))
			{
				D2D1_SIZE_F pSize = m_pD2DBitmap->GetSize();
				pImageWidth = pSize.width;
				pImageHeight = pSize.height;

				hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidColorBrush);
			}

			if (SUCCEEDED(hr))
			{


				hr = DrawD2DContentAxisAlignedClip(hInstance, hWnd, m_pD2DBitmap, rectangle);

			}


			IWICBitmapSource* pConverter = nullptr;
			WICPixelFormatGUID pixelFormat;

			if (SUCCEEDED(hr))
			{


				pSecondarySource->AddRef();
				pSecondarySource->GetSize(&uiWidth, &uiHeight);
			}

			IWICPixelFormatInfo* pixelFormatInfo = nullptr;
			if (SUCCEEDED(hr))
			{
				pSecondarySource->GetPixelFormat(&pixelFormat);
			}

			if (!IsEqualGUID(pixelFormat, GUID_WICPixelFormat128bppPRGBAFloat))
			{

				hr = WICConvertBitmapSource(GUID_WICPixelFormat128bppPRGBAFloat, pSecondarySource, &pConverter);
				IWICBitmapLock* pBitmapLock = nullptr;


				if (SUCCEEDED(hr))
				{
					pSecondarySource->Release();     // the converter has a reference to the source
					pSecondarySource = NULL;         // so we don't need it anymore.
					pSecondarySource = pConverter;   // let's treat the 128bppPABGR converter as the source
				}


				IWICPalette* pImagePalette = nullptr;
				pSecondarySource->CopyPalette(pImagePalette);
			}




			CImageFileData* pImageClass = new CImageFileData((char*)pixelArrayPointer);

			if (pConverter)
			{
				UINT cbStride = pWidth * sizeof(float) * 4;
				UINT cbBufferSize = cbStride;
				float *pixels = new float[cbBufferSize / sizeof(float)];
				if (pHeight < rc.Height && pWidth < rc.Width)
				{
					rc.Height = pHeight;
					rc.Width = pWidth;
				}
				else
				{
					pWidth = rc.Width;
					pHeight = rc.Height;
				}
			}


			UINT bSize = 0;




			UINT rowPitch1 = 4 * pWidth;
			UINT stride = 0; // stride  = bytes per row					 // (width * (bpp/8)) + padding and round to the nearest DWORD
			stride = (((((UINT)pWidth * 32) + 31) & ~31) >> 3);
			bSize = pHeight * pWidth;
			BYTE *pixelArray = new BYTE[(UINT)pHeight * stride];


			pSecondarySource->CopyPixels(NULL, stride, bSize, pixelArray);
			HDC hDC = NULL;
			hDC = GetDC(hWnd);

			COLORREF bkColor = RGB(255, 255, 255);

			SetBkColor(hDC, bkColor);

			HBITMAP map = CreateBitmap((UINT)pWidth,// width. 512 in my case
				(UINT)pHeight, // height
				1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
				8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
				(void*)pixelArray); // pointer to array
			// Temp HDC to copy picture
			src = CreateCompatibleDC(hDC); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
			SelectObject(src, map); // Inserting picture into our temp HDC
			// Copy image from temp HDC to window


			std::cout << "Pixels Begin" << endl;

			int pixelCounter = 0;




			std::cout << "Pixels Begin" << endl;

			pixelCounter = 0;


			//	pBitmapObj->GetResolution(&pWidth, &pHeight);

			UINT pYVal = 0;
			UINT pXVal = 0;
			for (pXVal = pXPosition; pXVal < pXPosition + pWidth + 1; pXVal += pStepValueX)
			{
				for (pYVal = pYPosition; pYVal < pYPosition + pHeight + 1; ++pYVal)
					// Your code here
				{

					std::cout << "Current X Value: " << pXVal << " " << "Current Y Value: " << pYVal << endl;
					COLORREF pCurrentPixel = GetPixel(hDC, pXVal, pYVal);
					int pRedValue = GetRValue(pCurrentPixel);
					int pGreenValue = GetGValue(pCurrentPixel);
					int pBlueValue = GetBValue(pCurrentPixel);
					PixelObject pixelObj = PixelObject(pixelCounter, pXVal, pYVal, pRedValue, pGreenValue, pBlueValue);
					pixelDataVector.push_back(pixelObj);
					std::cout << "Amount of total pixels processed  = " << pixelCounter << endl;
					pixelCounter = pixelCounter + 1;

				}
			}




			std::cout << "The size of pixel buffer = " << pixelDataVector.size() << endl;
			int pAmountOfPixelsLeft = 0;
			pAmountOfPixelsLeft = pixelDataVector.size();


			pixelDataFile.clear();
			pixelDataFile << " " << endl;
			pixelDataFile << " " << endl;
			pixelDataFile << "New Operation Begin" << endl;
			time_t currentTime = time(0);
			pixelDataFile << "current time" << currentTime << endl;
			for (PixelObject currentPixel : pixelDataVector)
			{


				if ((currentPixel.RMask != 255.0f) && (currentPixel.GMask != 255.0f) && (currentPixel.BMask != 255.0f))
				{
					std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
						"X Position = " << currentPixel.xPosition << " " <<
						"Y Position = " << currentPixel.yPosition << " " <<
						"The Value Of Red = " << currentPixel.RMask << " "
						<< "The Value Of Green = " << currentPixel.GMask << " " <<
						"The Value Of Blue = " << currentPixel.BMask << endl;
				}


				pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
				pixelDataFile << "X Position = " << pXPosition << endl;
				pixelDataFile << "Y Position = " << pYPosition << endl;
				pixelDataFile << "The Value Of Red = " << currentPixel.RMask << endl;
				pixelDataFile << "The Value Of Green = " << currentPixel.GMask << endl;
				pixelDataFile << "The Value Of Blue = " << currentPixel.BMask << endl;

			}


			pAmountOfPixelsLeft--;

		}




		SaveScreenDataToImage(hInstance, hWnd, pWidth, pHeight, pSecondarySource, (LPCTSTR)L"E:\\RobotData\\SavedClippedRect.jpg");





		pixelDataFile << "New Operation End" << endl;
		pixelDataFile << " " << endl;
		pixelDataFile << " " << endl;
		pixelDataFile.close();
		std::cout << "Pixels END" << endl;



		ReleaseDC(hWnd, src);
		ReleaseDC(hWnd, hDC);
		DeleteDC(src);
		DeleteDC(hDC);
		SafeReleaseA(&pDecoder);
		SafeReleaseA(&pFrame);
		myfile.close();
		*ppPixelObject = pixelDataVector;

		return hr;






	}





	HRESULT Direct2DManagementClass::CreateBitmapDrawingClipObjectWitSpecifiedStepForYAxis(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pWidth, float pHeight,
		float pStepValueY,
		vector<PixelObject> *ppPixelObject)
	{
		ofstream myfile("imagebuffer.txt");
		//	MessageBoxA(hWnd, (LPCSTR)"Эта Функция Рисует Клиповую Область Изображения", (LPCSTR)"Message", 0);
		myfile << "Please write this text to a file.\n this text is written using C++\n";
		IWICBitmapSource* pSource = nullptr;
		IWICBitmapSource* pSecondarySource = nullptr;

		IWICBitmap* pBitmapObject = nullptr;
		IWICBitmapLock* pBitmapLock = nullptr;
		IWICBitmapLock* pBitmapLock1 = nullptr;
		IWICBitmapClipper *pIClipper = nullptr;
		BYTE* pixels = nullptr;
		HRESULT hr = S_OK;
		WCHAR szFileName[MAX_PATH];
		WICRect rcLock = { pXPosition, pYPosition, pWidth, pHeight };
		ID2D1Image* outputImage = nullptr;
		vector <PixelObject> pixelDataVector = vector <PixelObject>();
		HDC src = NULL;
		HDC hDC = NULL;
		WICPixelFormatGUID pixelFormat;
		IWICBitmapSource* piConverter = nullptr;
		UINT pImageHeight = NULL;
		UINT pImageWidth = NULL;
		BYTE* pixelArrayPointer = nullptr;
		D2D1_RECT_F rectangle = D2D1::RectF(0, 0, pXPosition + pWidth, pYPosition + pHeight);
		UINT cbBufferSize = 0;
		BYTE* pixelDataArray = NULL;
		IWICBitmapDecoder *pDecoder = NULL;
		IWICBitmapDecoder* pImageDecoder = nullptr;
		IWICBitmapFrameDecode *pFrame = NULL;
		UINT uiWidth = 0;
		UINT uiHeight = 0;
		WICRect rcClip = { pXPosition, pYPosition, pWidth, pHeight };

		fstream pixelDataFile;
		pixelDataFile.open("C:\\pixels\\ImageClipperTests.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
		pixelDataFile.clear();

		if (LocateImageFile(hWnd, szFileName, ARRAYSIZE(szFileName)))
		{

			// Prepare iterator pairs to iterate the file content!



			hr = m_pWICFactory->CreateDecoderFromFilename(
				szFileName,
				NULL,
				GENERIC_READ,
				WICDecodeMetadataCacheOnDemand,
				&pImageDecoder);




			if (SUCCEEDED(hr))
			{
				hr = pImageDecoder->GetFrame(0, &pFrame);
			}



			if (SUCCEEDED(hr))
			{
				pSource = pFrame;
				pSource->AddRef();
				pSource->GetSize(&uiWidth, &uiHeight);

				pSecondarySource = pFrame;
				pSecondarySource->AddRef();
				pSecondarySource->GetSize(&uiWidth, &uiHeight);

				IWICPalette* pImagePalette = nullptr;
				pSecondarySource->CopyPalette(pImagePalette);
			}

			WICRect rc = { pXPosition, pYPosition, pWidth, pHeight };



			if (SUCCEEDED(hr))
			{
				SafeReleaseA(&m_pConvertedSourceBitmap);
				hr = m_pWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
			}

			if (SUCCEEDED(hr))
			{
				hr = m_pConvertedSourceBitmap->Initialize(
					pSecondarySource,
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone,
					NULL,
					0.f,
					WICBitmapPaletteTypeCustom);

			}
			if (SUCCEEDED(hr))
			{
				hr = CreateDeviceResources(hInstance, hWnd);
			}
			if (SUCCEEDED(hr))
			{

				SafeReleaseA(&m_pD2DBitmap);
				hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);

			}



			if (SUCCEEDED(hr))
			{
				D2D1_SIZE_F pSize = m_pD2DBitmap->GetSize();
				pImageWidth = pSize.width;
				pImageHeight = pSize.height;

				hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidColorBrush);
			}

			if (SUCCEEDED(hr))
			{


				hr = DrawD2DContentAxisAlignedClip(hInstance, hWnd, m_pD2DBitmap, rectangle);

			}


			IWICBitmapSource* pConverter = nullptr;
			WICPixelFormatGUID pixelFormat;

			if (SUCCEEDED(hr))
			{


				pSecondarySource->AddRef();
				pSecondarySource->GetSize(&uiWidth, &uiHeight);
			}

			IWICPixelFormatInfo* pixelFormatInfo = nullptr;
			if (SUCCEEDED(hr))
			{
				pSecondarySource->GetPixelFormat(&pixelFormat);
			}

			if (!IsEqualGUID(pixelFormat, GUID_WICPixelFormat128bppPRGBAFloat))
			{

				hr = WICConvertBitmapSource(GUID_WICPixelFormat128bppPRGBAFloat, pSecondarySource, &pConverter);
				IWICBitmapLock* pBitmapLock = nullptr;


				if (SUCCEEDED(hr))
				{
					pSecondarySource->Release();     // the converter has a reference to the source
					pSecondarySource = NULL;         // so we don't need it anymore.
					pSecondarySource = pConverter;   // let's treat the 128bppPABGR converter as the source
				}


				IWICPalette* pImagePalette = nullptr;
				pSecondarySource->CopyPalette(pImagePalette);
			}




			CImageFileData* pImageClass = new CImageFileData((char*)pixelArrayPointer);

			if (pConverter)
			{
				UINT cbStride = pWidth * sizeof(float) * 4;
				UINT cbBufferSize = cbStride;
				float *pixels = new float[cbBufferSize / sizeof(float)];
				if (pHeight < rc.Height && pWidth < rc.Width)
				{
					rc.Height = pHeight;
					rc.Width = pWidth;
				}
				else
				{
					pWidth = rc.Width;
					pHeight = rc.Height;
				}
			}


			UINT bSize = 0;




			UINT rowPitch1 = 4 * pWidth;
			UINT stride = 0; // stride  = bytes per row					 // (width * (bpp/8)) + padding and round to the nearest DWORD
			stride = (((((UINT)pWidth * 32) + 31) & ~31) >> 3);
			bSize = pHeight * pWidth;
			BYTE *pixelArray = new BYTE[(UINT)pHeight * stride];


			pSecondarySource->CopyPixels(NULL, stride, bSize, pixelArray);
			HDC hDC = NULL;
			hDC = GetDC(hWnd);

			COLORREF bkColor = RGB(255, 255, 255);

			SetBkColor(hDC, bkColor);

			HBITMAP map = CreateBitmap((UINT)pWidth,// width. 512 in my case
				(UINT)pHeight, // height
				1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
				8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
				(void*)pixelArray); // pointer to array
			// Temp HDC to copy picture
			src = CreateCompatibleDC(hDC); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
			SelectObject(src, map); // Inserting picture into our temp HDC
			// Copy image from temp HDC to window


			std::cout << "Pixels Begin" << endl;

			int pixelCounter = 0;




			std::cout << "Pixels Begin" << endl;

			pixelCounter = 0;


			//	pBitmapObj->GetResolution(&pWidth, &pHeight);

			UINT pYVal = 0;
			UINT pXVal = 0;
			for (pXVal = pXPosition; pXVal < pXPosition + pWidth + 1; ++pXPosition)
			{
				for (pYVal = pYPosition; pYVal < pYPosition + pHeight + 1; pYVal += pStepValueY)
					// Your code here
				{

					std::cout << "Current X Value: " << pXVal << " " << "Current Y Value: " << pYVal << endl;
					COLORREF pCurrentPixel = GetPixel(hDC, pXVal, pYVal);
					int pRedValue = GetRValue(pCurrentPixel);
					int pGreenValue = GetGValue(pCurrentPixel);
					int pBlueValue = GetBValue(pCurrentPixel);
					PixelObject pixelObj = PixelObject(pixelCounter, pXVal, pYVal, pRedValue, pGreenValue, pBlueValue);
					pixelDataVector.push_back(pixelObj);
					std::cout << "Amount of total pixels processed  = " << pixelCounter << endl;
					pixelCounter = pixelCounter + 1;

				}
			}




			std::cout << "The size of pixel buffer = " << pixelDataVector.size() << endl;
			int pAmountOfPixelsLeft = 0;
			pAmountOfPixelsLeft = pixelDataVector.size();


			pixelDataFile.clear();
			pixelDataFile << " " << endl;
			pixelDataFile << " " << endl;
			pixelDataFile << "New Operation Begin" << endl;
			time_t currentTime = time(0);
			pixelDataFile << "current time" << currentTime << endl;
			for (PixelObject currentPixel : pixelDataVector)
			{


				if ((currentPixel.RMask != 255.0f) && (currentPixel.GMask != 255.0f) && (currentPixel.BMask != 255.0f))
				{
					std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
						"X Position = " << currentPixel.xPosition << " " <<
						"Y Position = " << currentPixel.yPosition << " " <<
						"The Value Of Red = " << currentPixel.RMask << " "
						<< "The Value Of Green = " << currentPixel.GMask << " " <<
						"The Value Of Blue = " << currentPixel.BMask << endl;
				}


				pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
				pixelDataFile << "X Position = " << pXPosition << endl;
				pixelDataFile << "Y Position = " << pYPosition << endl;
				pixelDataFile << "The Value Of Red = " << currentPixel.RMask << endl;
				pixelDataFile << "The Value Of Green = " << currentPixel.GMask << endl;
				pixelDataFile << "The Value Of Blue = " << currentPixel.BMask << endl;

			}


			pAmountOfPixelsLeft--;

		}




		SaveScreenDataToImage(hInstance, hWnd, pWidth, pHeight, pSecondarySource, (LPCTSTR)L"E:\\RobotData\\SavedClippedRect.jpg");





		pixelDataFile << "New Operation End" << endl;
		pixelDataFile << " " << endl;
		pixelDataFile << " " << endl;
		pixelDataFile.close();
		std::cout << "Pixels END" << endl;



		ReleaseDC(hWnd, src);
		ReleaseDC(hWnd, hDC);
		DeleteDC(src);
		DeleteDC(hDC);
		SafeReleaseA(&pDecoder);
		SafeReleaseA(&pFrame);
		myfile.close();
		*ppPixelObject = pixelDataVector;

		return hr;






	}

HRESULT Direct2DManagementClass::CreateBitmapDrawingClipObject(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pWidth, float pHeight,
	vector<PixelObject> *ppPixelObject)
{
	ofstream myfile("imagebuffer.txt");
	myfile << "Please write this text to a file.\n this text is written using C++\n";
	IWICBitmapSource* pSource = nullptr;
	IWICBitmapSource* pSecondarySource = nullptr;

	IWICBitmap* pBitmapObject = nullptr;
	IWICBitmapLock* pBitmapLock = nullptr;
	IWICBitmapLock* pBitmapLock1 = nullptr;
	IWICBitmapClipper *pIClipper = nullptr;
	BYTE* pixels = nullptr;
	HRESULT hr = S_OK;
	WCHAR szFileName[MAX_PATH];
	WICRect rcLock = { pXPosition, pYPosition, pWidth, pHeight };
	ID2D1Image* outputImage = nullptr;
	vector <PixelObject> pixelDataVector = vector <PixelObject>();
	HDC src = NULL;
	HDC hDC = NULL;
	WICPixelFormatGUID pixelFormat;
	IWICBitmapSource* piConverter = nullptr;
	UINT pImageHeight = NULL;
	UINT pImageWidth = NULL;
	BYTE* pixelArrayPointer = nullptr;
	D2D1_RECT_F rectangle = D2D1::RectF(0, 0, pXPosition + pWidth, pYPosition + pHeight);
	UINT cbBufferSize = 0;
	BYTE* pixelDataArray = NULL;
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapDecoder* pImageDecoder = nullptr;
	IWICBitmapFrameDecode *pFrame = NULL;
	UINT uiWidth = 0;
	UINT uiHeight = 0;
	WICRect rcClip = { pXPosition, pYPosition, pWidth, pHeight };


	fstream pixelDataFile;
	pixelDataFile.open("C:\\pixels\\ImageClippingExperimentsTests.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);

	pixelDataFile.clear();
	pixelDataFile << " " << endl;
	pixelDataFile << " " << endl;
	pixelDataFile << "New Operation Begin" << endl;
	time_t currentTimeVar = time(0);
	tm* currentTime = localtime(&currentTimeVar);
	cout << "current time" << currentTimeVar << endl;
	pixelDataFile << "current time" << currentTimeVar << endl;

	if (LocateImageFile(hWnd, szFileName, ARRAYSIZE(szFileName)))
	{

		// Prepare iterator pairs to iterate the file content!

		cout << "Stage 1: Decoder" << endl;
		pixelDataFile << "Stage 1: Decoder" << endl;
		time_t currentTimeVar = time(0);
		tm* currentTime = localtime(&currentTimeVar);

		cout << "Decoder Creation Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Decoder Creation Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;

		hr = m_pWICFactory->CreateDecoderFromFilename(
			szFileName,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&pImageDecoder);

		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);

		cout << "Decoder Creation Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Decoder Creation Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;


		cout << "Stage 2: Frame" << endl;
		pixelDataFile << "Stage 2: Frame" << endl;
		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Frame Creation Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Frame Creation Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;



		if (SUCCEEDED(hr))
		{
			hr = pImageDecoder->GetFrame(0, &pFrame);
		}


		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Frame Creation Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Frame Creation Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;

		cout << "Stage 3: Bitmap Source" << endl;
		pixelDataFile << "Stage 3: Bitmap Source" << endl;
		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);

		cout << "Bitmap Source Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Frame Creation Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;

		if (SUCCEEDED(hr))
		{
			pSource = pFrame;
			pSource->AddRef();
			pSource->GetSize(&uiWidth, &uiHeight);

			pSecondarySource = pFrame;
			pSecondarySource->AddRef();
			pSecondarySource->GetSize(&uiWidth, &uiHeight);

			IWICPalette* pImagePalette = nullptr;
			pSecondarySource->CopyPalette(pImagePalette);
		}

		WICRect rc = { pXPosition, pYPosition, pWidth, pHeight };


		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Bitmap Source Creation Terminated At At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Bitmap Source Creation Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;

		cout << "Stage 4: Converting Wic Formats to Direct2D Formats" << endl;
		pixelDataFile << "Stage 4: Converting Wic Formats to Direct2D Formats" << endl;
		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Format Conversion  Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Format Conversion Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;

	

		// Initialize the clipper with the given rectangle of the frame's image data.

		if (SUCCEEDED(hr))
		{
			SafeReleaseA(&m_pConvertedSourceBitmap);
			hr = m_pWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pConvertedSourceBitmap->Initialize(
				pSecondarySource,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeCustom);

		}
		if (SUCCEEDED(hr))
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}
		if (SUCCEEDED(hr))
		{

			SafeReleaseA(&m_pD2DBitmap);
			hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);

		}

		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Format Conversion terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Format Conversion terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;


		cout << "Stage 5: Drawing Pixels with Direct2D" << endl;
		pixelDataFile << "Stage 5: Drawing Pixels with Direct2D" << endl;
		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Drawing  Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Drawing Conversion Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;



		if (SUCCEEDED(hr))
		{
			D2D1_SIZE_F pSize = m_pD2DBitmap->GetSize();
			pImageWidth = pSize.width;
			pImageHeight = pSize.height;

			hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidColorBrush);
		}

		if (SUCCEEDED(hr))
		{


			
			hr = DrawD2DContentAxisAlignedClip(hInstance, hWnd, m_pD2DBitmap, rectangle);

		}

		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Drawing Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Drawing terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;


		cout << "Stage 6: Convertion of image data to GDI HBITMAP For Pixel Extraction" << endl;
		pixelDataFile << "Stage 6: Convertion of image data to GDI HBITMAP For Pixel Extraction" << endl;
		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Convertion Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Convertion Started: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;



		IWICBitmapSource* pConverter = nullptr;
		WICPixelFormatGUID pixelFormat;

		if (SUCCEEDED(hr))
		{


			pSecondarySource->AddRef();
			pSecondarySource->GetSize(&uiWidth, &uiHeight);
		}

		IWICPixelFormatInfo* pixelFormatInfo = nullptr;
		if (SUCCEEDED(hr))
		{
			pSecondarySource->GetPixelFormat(&pixelFormat);
		}

		if (!IsEqualGUID(pixelFormat, GUID_WICPixelFormat128bppPRGBAFloat))
		{

			hr = WICConvertBitmapSource(GUID_WICPixelFormat128bppPRGBAFloat, pSecondarySource, &pConverter);
			IWICBitmapLock* pBitmapLock = nullptr;


			if (SUCCEEDED(hr))
			{
				pSecondarySource->Release();     // the converter has a reference to the source
				pSecondarySource = NULL;         // so we don't need it anymore.
				pSecondarySource = pConverter;   // let's treat the 128bppPABGR converter as the source
			}


			IWICPalette* pImagePalette = nullptr;
			pSecondarySource->CopyPalette(pImagePalette);
		}




		CImageFileData* pImageClass = new CImageFileData((char*)pixelArrayPointer);

		if (pConverter)
		{
			UINT cbStride = pWidth * sizeof(float) * 4;
			UINT cbBufferSize = cbStride;
			float *pixels = new float[cbBufferSize / sizeof(float)];
			if (pHeight < rc.Height && pWidth < rc.Width)
			{
				rc.Height = pHeight;
				rc.Width = pWidth;
			}
			else
			{
				pWidth = rc.Width;
				pHeight = rc.Height;
			}
		}


		UINT bSize = 0;




		UINT rowPitch1 = 4 * pWidth;
		UINT stride = 0; // stride  = bytes per row					 // (width * (bpp/8)) + padding and round to the nearest DWORD
		stride = (((((UINT)pWidth * 32) + 31) & ~31) >> 3);
		bSize = pHeight * pWidth;
		BYTE *pixelArray = new BYTE[(UINT)pHeight * stride];


		pSecondarySource->CopyPixels(NULL, stride, bSize, pixelArray);
		HDC hDC = NULL;
		hDC = GetDC(hWnd);

		COLORREF bkColor = RGB(255, 255, 255);

		SetBkColor(hDC, bkColor);

		HBITMAP map = CreateBitmap((UINT)pWidth,// width. 512 in my case
			(UINT)pHeight, // height
			1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
			8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
			(void*)pixelArray); // pointer to array
		// Temp HDC to copy picture
		src = CreateCompatibleDC(hDC); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
		SelectObject(src, map); // Inserting picture into our temp HDC
		// Copy image from temp HDC to window

		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Convertion Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Drawing terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;


		cout << "Stage 7: Reading Pixels and Preparing Pixel Vector" << endl;
		pixelDataFile << "Stage 7: Reading Pixels and Preparing Pixel Vector" << endl;
		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Pixel Extraction Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Pixel Extraction Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;





		std::cout << "Pixels Begin" << endl;

		int pixelCounter = 0;


		//	pBitmapObj->GetResolution(&pWidth, &pHeight);

		UINT pYVal = 0;
		UINT pXVal = 0;
		
		
		for (pXVal = pXPosition; pXVal < pXPosition + pWidth; ++pXVal)
		{
			for (pYVal = pYPosition; pYVal < pYPosition + pHeight; ++pYVal)
				// Your code here
			{

				std::cout << "Current X Value: " << pXVal << " " << "Current Y Value: " << pYVal << endl;
				COLORREF pCurrentPixel = GetPixel(hDC, pXVal, pYVal);
				int pRedValue = GetRValue(pCurrentPixel);
				int pGreenValue = GetGValue(pCurrentPixel);
				int pBlueValue = GetBValue(pCurrentPixel);
				PixelObject pixelObj = PixelObject(pixelCounter, pXVal, pYVal, pRedValue, pGreenValue, pBlueValue);
				pixelDataVector.push_back(pixelObj);
				std::cout << "Amount of total pixels processed  = " << pixelCounter << endl;
				pixelCounter = pixelCounter + 1;


			}
		}

		std::cout << "The size of pixel buffer = " << pixelDataVector.size() << endl;
		int pAmountOfPixelsLeft = 0;
		pAmountOfPixelsLeft = pixelDataVector.size();

		if (LOOPS_ARE_TURNED_OFF == FALSE)
		{
			for (PixelObject currentPixel : pixelDataVector)
			{


				if ((currentPixel.RMask != 255.0f) && (currentPixel.GMask != 255.0f) && (currentPixel.BMask != 255.0f))

				{
					std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
						"X Position = " << currentPixel.xPosition << " " <<
						"Y (Position = " << currentPixel.yPosition << " " <<
						"The Value Of Red = " << currentPixel.RMask << " "
						<< "The Value Of Green = " << currentPixel.GMask << " " <<
						"The Value Of Blue = " << currentPixel.BMask << endl;
				}


				pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
				pixelDataFile << "X Position = " << currentPixel.xPosition << endl;
				pixelDataFile << "Y Position = " << currentPixel.yPosition << endl;
				pixelDataFile << "The Value Of Red = " << currentPixel.RMask << endl;
				pixelDataFile << "The Value Of Green = " << currentPixel.GMask << endl;
				pixelDataFile << "The Value Of Blue = " << currentPixel.BMask << endl;

			}
		}




		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Clipping Terminated at: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Clipping Terminated at: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;



		pixelDataFile << "New Operation End" << endl;
		pixelDataFile << " " << endl;
		pixelDataFile << " " << endl;
		pixelDataFile.close();
		std::cout << "Pixels END" << endl;










	}
	
	ReleaseDC(hWnd, src);
	ReleaseDC(hWnd, hDC);
	DeleteDC(src);
	DeleteDC(hDC);
	SafeReleaseA(&pDecoder);
	SafeReleaseA(&pFrame);
	myfile.close();
	*ppPixelObject = pixelDataVector;

	return hr;
}



HRESULT Direct2DManagementClass::CreateBitmapDrawingClipObjectDIB(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pWidth, float pHeight,
	vector<PixelObject> *ppPixelObject)
{
	ofstream myfile("imagebuffer.txt");
	myfile << "Please write this text to a file.\n this text is written using C++\n";
	IWICBitmapSource* pSource = nullptr;
	IWICBitmapSource* pSecondarySource = nullptr;

	IWICBitmap* pBitmapObject = nullptr;
	IWICBitmapLock* pBitmapLock = nullptr;
	IWICBitmapLock* pBitmapLock1 = nullptr;
	IWICBitmapClipper *pIClipper = nullptr;
	BYTE* pixels = nullptr;
	HRESULT hr = S_OK;
	WCHAR szFileName[MAX_PATH];
	WICRect rcLock = { pXPosition, pYPosition, pWidth, pHeight };
	ID2D1Image* outputImage = nullptr;
	vector <PixelObject> pixelDataVector = vector <PixelObject>();
	HDC src = NULL;
	HDC hDC = NULL;
	WICPixelFormatGUID pixelFormat;
	IWICBitmapSource* piConverter = nullptr;
	UINT pImageHeight = NULL;
	UINT pImageWidth = NULL;
	BYTE* pixelArrayPointer = nullptr;
	D2D1_RECT_F rectangle = D2D1::RectF(0, 0, pXPosition + pWidth, pYPosition + pHeight);
	UINT cbBufferSize = 0;
	BYTE* pixelDataArray = NULL;
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapDecoder* pImageDecoder = nullptr;
	IWICBitmapFrameDecode *pFrame = NULL;
	UINT uiWidth = 0;
	UINT uiHeight = 0;
	WICRect rcClip = { pXPosition, pYPosition, pWidth, pHeight };


	fstream pixelDataFile;
	pixelDataFile.open("C:\\pixels\\ImageClippingExperimentsTests.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);

	pixelDataFile.clear();
	pixelDataFile << " " << endl;
	pixelDataFile << " " << endl;
	pixelDataFile << "New Operation Begin" << endl;
	time_t currentTimeVar = time(0);
	tm* currentTime = localtime(&currentTimeVar);
	cout << "current time" << currentTimeVar << endl;
	pixelDataFile << "current time" << currentTimeVar << endl;

	if (LocateImageFile(hWnd, szFileName, ARRAYSIZE(szFileName)))
	{

		// Prepare iterator pairs to iterate the file content!

		cout << "Stage 1: Decoder" << endl;
		pixelDataFile << "Stage 1: Decoder" << endl;
		time_t currentTimeVar = time(0);
		tm* currentTime = localtime(&currentTimeVar);

		cout << "Decoder Creation Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Decoder Creation Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;

		hr = m_pWICFactory->CreateDecoderFromFilename(
			szFileName,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&pImageDecoder);

		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);

		cout << "Decoder Creation Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Decoder Creation Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;


		cout << "Stage 2: Frame" << endl;
		pixelDataFile << "Stage 2: Frame" << endl;
		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Frame Creation Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Frame Creation Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;



		if (SUCCEEDED(hr))
		{
			hr = pImageDecoder->GetFrame(0, &pFrame);
		}


		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Frame Creation Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Frame Creation Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;

		cout << "Stage 3: Bitmap Source" << endl;
		pixelDataFile << "Stage 3: Bitmap Source" << endl;
		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);

		cout << "Bitmap Source Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Frame Creation Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;

		if (SUCCEEDED(hr))
		{
			pSource = pFrame;
			pSource->AddRef();
			pSource->GetSize(&uiWidth, &uiHeight);

			pSecondarySource = pFrame;
			pSecondarySource->AddRef();
			pSecondarySource->GetSize(&uiWidth, &uiHeight);

			IWICPalette* pImagePalette = nullptr;
			pSecondarySource->CopyPalette(pImagePalette);
		}

		WICRect rc = { pXPosition, pYPosition, pWidth, pHeight };


		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Bitmap Source Creation Terminated At At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Bitmap Source Creation Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;

		cout << "Stage 4: Converting Wic Formats to Direct2D Formats" << endl;
		pixelDataFile << "Stage 4: Converting Wic Formats to Direct2D Formats" << endl;
		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Format Conversion  Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Format Conversion Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;



		// Initialize the clipper with the given rectangle of the frame's image data.

		if (SUCCEEDED(hr))
		{
			SafeReleaseA(&m_pConvertedSourceBitmap);
			hr = m_pWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pConvertedSourceBitmap->Initialize(
				pSecondarySource,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeCustom);

		}
		if (SUCCEEDED(hr))
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}
		if (SUCCEEDED(hr))
		{

			SafeReleaseA(&m_pD2DBitmap);
			hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);

		}

		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Format Conversion terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Format Conversion terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;


		cout << "Stage 5: Drawing Pixels with Direct2D" << endl;
		pixelDataFile << "Stage 5: Drawing Pixels with Direct2D" << endl;
		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Drawing  Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Drawing Conversion Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;



		if (SUCCEEDED(hr))
		{
			D2D1_SIZE_F pSize = m_pD2DBitmap->GetSize();
			pImageWidth = pSize.width;
			pImageHeight = pSize.height;

			hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidColorBrush);
		}

		if (SUCCEEDED(hr))
		{



			hr = DrawD2DContentAxisAlignedClip(hInstance, hWnd, m_pD2DBitmap, rectangle);

		}

		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Drawing Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Drawing terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;


		cout << "Stage 6: Convertion of image data to GDI HBITMAP For Pixel Extraction" << endl;
		pixelDataFile << "Stage 6: Convertion of image data to GDI HBITMAP For Pixel Extraction" << endl;
		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Convertion Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Convertion Started: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;



		IWICBitmapSource* pConverter = nullptr;
		WICPixelFormatGUID pixelFormat;

		if (SUCCEEDED(hr))
		{


			pSecondarySource->AddRef();
			pSecondarySource->GetSize(&uiWidth, &uiHeight);
		}

		IWICPixelFormatInfo* pixelFormatInfo = nullptr;
		if (SUCCEEDED(hr))
		{
			pSecondarySource->GetPixelFormat(&pixelFormat);
		}

		if (!IsEqualGUID(pixelFormat, GUID_WICPixelFormat128bppPRGBAFloat))
		{

			hr = WICConvertBitmapSource(GUID_WICPixelFormat128bppPRGBAFloat, pSecondarySource, &pConverter);
			IWICBitmapLock* pBitmapLock = nullptr;


			if (SUCCEEDED(hr))
			{
				pSecondarySource->Release();     // the converter has a reference to the source
				pSecondarySource = NULL;         // so we don't need it anymore.
				pSecondarySource = pConverter;   // let's treat the 128bppPABGR converter as the source
			}


			IWICPalette* pImagePalette = nullptr;
			pSecondarySource->CopyPalette(pImagePalette);
		}




		CImageFileData* pImageClass = new CImageFileData((char*)pixelArrayPointer);

		if (pConverter)
		{
			UINT cbStride = pWidth * sizeof(float) * 4;
			UINT cbBufferSize = cbStride;
			float *pixels = new float[cbBufferSize / sizeof(float)];
			if (pHeight < rc.Height && pWidth < rc.Width)
			{
				rc.Height = pHeight;
				rc.Width = pWidth;
			}
			else
			{
				pWidth = rc.Width;
				pHeight = rc.Height;
			}
		}


		UINT bSize = 0;




		UINT rowPitch1 = 4 * pWidth;
		UINT stride = 0; // stride  = bytes per row					 // (width * (bpp/8)) + padding and round to the nearest DWORD
		stride = (((((UINT)pWidth * 32) + 31) & ~31) >> 3);
		bSize = pHeight * pWidth;
		BYTE *pixelArray = new BYTE[(UINT)pHeight * stride];


		pSecondarySource->CopyPixels(NULL, stride, bSize, pixelArray);
		HDC hDC = NULL;
		hDC = GetDC(hWnd);

		COLORREF bkColor = RGB(255, 255, 255);

		SetBkColor(hDC, bkColor);

		BITMAPINFO pBitmap;
		pBitmap.bmiHeader.biSize = sizeof(pBitmap.bmiHeader);
		pBitmap.bmiHeader.biWidth = pWidth;
		pBitmap.bmiHeader.biHeight = pHeight;
		pBitmap.bmiHeader.biPlanes = 1; 
		pBitmap.bmiHeader.biBitCount = 32;
		pBitmap.bmiHeader.biCompression = BI_RGB;
		pBitmap.bmiHeader.biSizeImage = pWidth * pHeight * 4;
		pBitmap.bmiHeader.biClrUsed = 0;
		pBitmap.bmiHeader.biClrImportant = 0;

		HBITMAP map = CreateDIBSection(hDC,
			&pBitmap,
			DIB_RGB_COLORS,
			(void**)(&pixelArray),
			NULL,
			NULL);
		
		// Temp HDC to copy picture
		src = CreateCompatibleDC(hDC); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
		SelectObject(src, map); // Inserting picture into our temp HDC
		// Copy image from temp HDC to window

		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Convertion Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Drawing terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;


		cout << "Stage 7: Reading Pixels and Preparing Pixel Vector" << endl;
		pixelDataFile << "Stage 7: Reading Pixels and Preparing Pixel Vector" << endl;
		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Pixel Extraction Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Pixel Extraction Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;





		std::cout << "Pixels Begin" << endl;

		int pixelCounter = 0;


		//	pBitmapObj->GetResolution(&pWidth, &pHeight);

		UINT pYVal = 0;
		UINT pXVal = 0;


		for (pXVal = pXPosition; pXVal < pXPosition + pWidth - 1; ++pXVal)
		{
			for (pYVal = pYPosition; pYVal < pYPosition + pHeight - 1; ++pYVal)
				// Your code here
			{

				std::cout << "Current X Value: " << pXVal << " " << "Current Y Value: " << pYVal << endl;
				COLORREF pCurrentPixel = GetPixel(hDC, pXVal, pYVal);
				int pRedValue = GetRValue(pCurrentPixel);
				int pGreenValue = GetGValue(pCurrentPixel);
				int pBlueValue = GetBValue(pCurrentPixel);
				PixelObject pixelObj = PixelObject(pixelCounter, pXVal, pYVal, pRedValue, pGreenValue, pBlueValue);
				pixelDataVector.push_back(pixelObj);
				std::cout << "Amount of total pixels processed  = " << pixelCounter << endl;
				pixelCounter = pixelCounter + 1;


			}
		}

		std::cout << "The size of pixel buffer = " << pixelDataVector.size() << endl;
		int pAmountOfPixelsLeft = 0;
		pAmountOfPixelsLeft = pixelDataVector.size();

		if (LOOPS_ARE_TURNED_OFF == FALSE)
		{
			for (PixelObject currentPixel : pixelDataVector)
			{


				if ((currentPixel.RMask != 255.0f) && (currentPixel.GMask != 255.0f) && (currentPixel.BMask != 255.0f))

				{
					std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
						"X Position = " << currentPixel.xPosition << " " <<
						"Y (Position = " << currentPixel.yPosition << " " <<
						"The Value Of Red = " << currentPixel.RMask << " "
						<< "The Value Of Green = " << currentPixel.GMask << " " <<
						"The Value Of Blue = " << currentPixel.BMask << endl;
				}


				pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
				pixelDataFile << "X Position = " << currentPixel.xPosition << endl;
				pixelDataFile << "Y Position = " << currentPixel.yPosition << endl;
				pixelDataFile << "The Value Of Red = " << currentPixel.RMask << endl;
				pixelDataFile << "The Value Of Green = " << currentPixel.GMask << endl;
				pixelDataFile << "The Value Of Blue = " << currentPixel.BMask << endl;

			}
		}




		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Clipping Terminated at: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Clipping Terminated at: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;



		pixelDataFile << "New Operation End" << endl;
		pixelDataFile << " " << endl;
		pixelDataFile << " " << endl;
		pixelDataFile.close();
		std::cout << "Pixels END" << endl;










	}

	ReleaseDC(hWnd, src);
	ReleaseDC(hWnd, hDC);
	DeleteDC(src);
	DeleteDC(hDC);
	SafeReleaseA(&pDecoder);
	SafeReleaseA(&pFrame);
	myfile.close();
	*ppPixelObject = pixelDataVector;

	return hr;
}



HRESULT Direct2DManagementClass::SavePixelArrayChunckToStack(vector<PixelObject> pPixelObjectToPorcess, int pStackSize, int pFirstPixelNumber,
	PixelObjectStack* pStackVar)
{
	HRESULT hr = S_OK;
	PixelObjectStack pStackObject = PixelObjectStack(pFirstPixelNumber, pPixelObjectToPorcess.size());
	for (PixelObject pPixelObject : pPixelObjectToPorcess)
	{
		pStackObject.AddPixelObjectToStack(pPixelObject);
	}
	*pStackVar = pStackObject;
	return hr;
}


HRESULT Direct2DManagementClass::CreateBitmapDrawingClipObjectDIB(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pWidth, float pHeight,
	LPCWSTR szFileName,
	vector<PixelObject> *ppPixelObject)
{
	ofstream myfile("imagebuffer.txt");
	myfile << "Please write this text to a file.\n this text is written using C++\n";
	IWICBitmapSource* pSource = nullptr;
	IWICBitmapSource* pSecondarySource = nullptr;

	IWICBitmap* pBitmapObject = nullptr;
	IWICBitmapLock* pBitmapLock = nullptr;
	IWICBitmapLock* pBitmapLock1 = nullptr;
	IWICBitmapClipper *pIClipper = nullptr;
	BYTE* pixels = nullptr;
	HRESULT hr = S_OK;
	WICRect rcLock = { pXPosition, pYPosition, pWidth, pHeight };
	ID2D1Image* outputImage = nullptr;
	vector <PixelObject> pixelDataVector = vector <PixelObject>();
	HDC src = NULL;
	HDC hDC = NULL;
	WICPixelFormatGUID pixelFormat;
	IWICBitmapSource* piConverter = nullptr;
	UINT pImageHeight = NULL;
	UINT pImageWidth = NULL;
	BYTE* pixelArrayPointer = nullptr;
	D2D1_RECT_F rectangle = D2D1::RectF(0, 0, pXPosition + pWidth, pYPosition + pHeight);
	UINT cbBufferSize = 0;
	BYTE* pixelDataArray = NULL;
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapDecoder* pImageDecoder = nullptr;
	IWICBitmapFrameDecode *pFrame = NULL;
	UINT uiWidth = 0;
	UINT uiHeight = 0;
	WICRect rcClip = { pXPosition, pYPosition, pWidth, pHeight };


	fstream pixelDataFile;
	pixelDataFile.open("C:\\pixels\\ImageClippingExperimentsTests.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);

	pixelDataFile.clear();
	pixelDataFile << " " << endl;
	pixelDataFile << " " << endl;
	pixelDataFile << "New Operation Begin" << endl;
	time_t currentTimeVar = time(0);
	tm* currentTime = localtime(&currentTimeVar);
	cout << "current time" << currentTimeVar << endl;
	pixelDataFile << "current time" << currentTimeVar << endl;

	
		// Prepare iterator pairs to iterate the file content!

		cout << "Stage 1: Decoder" << endl;
		pixelDataFile << "Stage 1: Decoder" << endl;
		currentTimeVar = time(0);
	    currentTime = localtime(&currentTimeVar);

		cout << "Decoder Creation Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Decoder Creation Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;

		hr = m_pWICFactory->CreateDecoderFromFilename(
			szFileName,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&pImageDecoder);

		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);

		cout << "Decoder Creation Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Decoder Creation Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;


		cout << "Stage 2: Frame" << endl;
		pixelDataFile << "Stage 2: Frame" << endl;
		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Frame Creation Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Frame Creation Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;



		if (SUCCEEDED(hr))
		{
			hr = pImageDecoder->GetFrame(0, &pFrame);
		}


		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Frame Creation Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Frame Creation Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;

		cout << "Stage 3: Bitmap Source" << endl;
		pixelDataFile << "Stage 3: Bitmap Source" << endl;
		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);

		cout << "Bitmap Source Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Frame Creation Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;

		if (SUCCEEDED(hr))
		{
			pSource = pFrame;
			pSource->AddRef();
			pSource->GetSize(&uiWidth, &uiHeight);

			pSecondarySource = pFrame;
			pSecondarySource->AddRef();
			pSecondarySource->GetSize(&uiWidth, &uiHeight);

			IWICPalette* pImagePalette = nullptr;
			pSecondarySource->CopyPalette(pImagePalette);
		}

		WICRect rc = { pXPosition, pYPosition, pWidth, pHeight };


		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Bitmap Source Creation Terminated At At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Bitmap Source Creation Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;

		cout << "Stage 4: Converting Wic Formats to Direct2D Formats" << endl;
		pixelDataFile << "Stage 4: Converting Wic Formats to Direct2D Formats" << endl;
		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Format Conversion  Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Format Conversion Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;



		// Initialize the clipper with the given rectangle of the frame's image data.

		if (SUCCEEDED(hr))
		{
			SafeReleaseA(&m_pConvertedSourceBitmap);
			hr = m_pWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pConvertedSourceBitmap->Initialize(
				pSecondarySource,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeCustom);

		}
		if (SUCCEEDED(hr))
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}
		if (SUCCEEDED(hr))
		{

			SafeReleaseA(&m_pD2DBitmap);
			hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);

		}

		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Format Conversion terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Format Conversion terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;


		cout << "Stage 5: Drawing Pixels with Direct2D" << endl;
		pixelDataFile << "Stage 5: Drawing Pixels with Direct2D" << endl;
		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Drawing  Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Drawing Conversion Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;



		if (SUCCEEDED(hr))
		{
			D2D1_SIZE_F pSize = m_pD2DBitmap->GetSize();
			pImageWidth = pSize.width;
			pImageHeight = pSize.height;

			hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidColorBrush);
		}

		if (SUCCEEDED(hr))
		{



			hr = DrawD2DContentAxisAlignedClip(hInstance, hWnd, m_pD2DBitmap, rectangle);

		}

		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Drawing Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Drawing terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;


		cout << "Stage 6: Convertion of image data to GDI HBITMAP For Pixel Extraction" << endl;
		pixelDataFile << "Stage 6: Convertion of image data to GDI HBITMAP For Pixel Extraction" << endl;
		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Convertion Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Convertion Started: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;



		IWICBitmapSource* pConverter = nullptr;
	
		if (SUCCEEDED(hr))
		{


			pSecondarySource->AddRef();
			pSecondarySource->GetSize(&uiWidth, &uiHeight);
		}

		IWICPixelFormatInfo* pixelFormatInfo = nullptr;
		if (SUCCEEDED(hr))
		{
			pSecondarySource->GetPixelFormat(&pixelFormat);
		}

		if (!IsEqualGUID(pixelFormat, GUID_WICPixelFormat128bppPRGBAFloat))
		{

			hr = WICConvertBitmapSource(GUID_WICPixelFormat128bppPRGBAFloat, pSecondarySource, &pConverter);
			IWICBitmapLock* pBitmapLock = nullptr;


			if (SUCCEEDED(hr))
			{
				pSecondarySource->Release();     // the converter has a reference to the source
				pSecondarySource = NULL;         // so we don't need it anymore.
				pSecondarySource = pConverter;   // let's treat the 128bppPABGR converter as the source
			}


			IWICPalette* pImagePalette = nullptr;
			pSecondarySource->CopyPalette(pImagePalette);
		}




		CImageFileData* pImageClass = new CImageFileData((char*)pixelArrayPointer);

		if (pConverter)
		{
			UINT cbStride = pWidth * sizeof(float) * 4;
			UINT cbBufferSize = cbStride;
			float *pixels = new float[cbBufferSize / sizeof(float)];
			if (pHeight < rc.Height && pWidth < rc.Width)
			{
				rc.Height = pHeight;
				rc.Width = pWidth;
			}
			else
			{
				pWidth = rc.Width;
				pHeight = rc.Height;
			}
		}


		UINT bSize = 0;




		UINT rowPitch1 = 4 * pWidth;
		UINT stride = 0; // stride  = bytes per row					 // (width * (bpp/8)) + padding and round to the nearest DWORD
		stride = (((((UINT)pWidth * 32) + 31) & ~31) >> 3);
		bSize = pHeight * pWidth;
		BYTE *pixelArray = new BYTE[(UINT)pHeight * stride];


		pSecondarySource->CopyPixels(NULL, stride, bSize, pixelArray);
		hDC = NULL;
		hDC = GetDC(hWnd);

		COLORREF bkColor = RGB(255, 255, 255);

		SetBkColor(hDC, bkColor);

		BITMAPINFO pBitmap;
		pBitmap.bmiHeader.biSize = sizeof(pBitmap.bmiHeader);
		pBitmap.bmiHeader.biWidth = pWidth;
		pBitmap.bmiHeader.biHeight = pHeight;
		pBitmap.bmiHeader.biPlanes = 1;
		pBitmap.bmiHeader.biBitCount = 32;
		pBitmap.bmiHeader.biCompression = BI_RGB;
		pBitmap.bmiHeader.biSizeImage = pWidth * pHeight * 4;
		pBitmap.bmiHeader.biClrUsed = 0;
		pBitmap.bmiHeader.biClrImportant = 0;

		HBITMAP map = CreateDIBSection(hDC,
			&pBitmap,
			DIB_RGB_COLORS,
			(void**)(&pixelArray),
			NULL,
			NULL);

		// Temp HDC to copy picture
		src = CreateCompatibleDC(hDC); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
		SelectObject(src, map); // Inserting picture into our temp HDC
		// Copy image from temp HDC to window

		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Convertion Terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Drawing terminated At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;


		cout << "Stage 7: Reading Pixels and Preparing Pixel Vector" << endl;
		pixelDataFile << "Stage 7: Reading Pixels and Preparing Pixel Vector" << endl;
		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Pixel Extraction Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Pixel Extraction Started At: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;





		std::cout << "Pixels Begin" << endl;

		int pixelCounter = 0;


		//	pBitmapObj->GetResolution(&pWidth, &pHeight);

		UINT pYVal = 0;
		UINT pXVal = 0;


		for (pXVal = pXPosition; pXVal < pXPosition + pWidth - 1; ++pXVal)
		{
			for (pYVal = pYPosition; pYVal < pYPosition + pHeight - 1; ++pYVal)
				// Your code here
			{

				std::cout << "Current X Value: " << pXVal << " " << "Current Y Value: " << pYVal << endl;
				COLORREF pCurrentPixel = GetPixel(hDC, pXVal, pYVal);
				int pRedValue = GetRValue(pCurrentPixel);
				int pGreenValue = GetGValue(pCurrentPixel);
				int pBlueValue = GetBValue(pCurrentPixel);
				PixelObject pixelObj = PixelObject(pixelCounter, pXVal, pYVal, pRedValue, pGreenValue, pBlueValue);
				pixelDataVector.push_back(pixelObj);
				std::cout << "Amount of total pixels processed  = " << pixelCounter << endl;
				pixelCounter = pixelCounter + 1;


			}
		}

		std::cout << "The size of pixel buffer = " << pixelDataVector.size() << endl;
		int pAmountOfPixelsLeft = 0;
		pAmountOfPixelsLeft = pixelDataVector.size();

		if (LOOPS_ARE_TURNED_OFF == FALSE)
		{
			for (PixelObject currentPixel : pixelDataVector)
			{


				if ((currentPixel.RMask != 255.0f) && (currentPixel.GMask != 255.0f) && (currentPixel.BMask != 255.0f))

				{
					std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
						"X Position = " << currentPixel.xPosition << " " <<
						"Y (Position = " << currentPixel.yPosition << " " <<
						"The Value Of Red = " << currentPixel.RMask << " "
						<< "The Value Of Green = " << currentPixel.GMask << " " <<
						"The Value Of Blue = " << currentPixel.BMask << endl;
				}


				pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
				pixelDataFile << "X Position = " << currentPixel.xPosition << endl;
				pixelDataFile << "Y Position = " << currentPixel.yPosition << endl;
				pixelDataFile << "The Value Of Red = " << currentPixel.RMask << endl;
				pixelDataFile << "The Value Of Green = " << currentPixel.GMask << endl;
				pixelDataFile << "The Value Of Blue = " << currentPixel.BMask << endl;

			}
		}




		currentTimeVar = time(0);
		currentTime = localtime(&currentTimeVar);
		cout << "Clipping Terminated at: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;
		pixelDataFile << "Clipping Terminated at: " << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << endl;



		pixelDataFile << "New Operation End" << endl;
		pixelDataFile << " " << endl;
		pixelDataFile << " " << endl;
		pixelDataFile.close();
		std::cout << "Pixels END" << endl;





		ReleaseDC(hWnd, src);
		ReleaseDC(hWnd, hDC);
		DeleteDC(src);
		DeleteDC(hDC);
		SafeReleaseA(&pDecoder);
		SafeReleaseA(&pFrame);
		myfile.close();
		*ppPixelObject = pixelDataVector;

		return hr;





	}





HRESULT Direct2DManagementClass::CreateBitmapDrawingClipObjectWithStageRecording(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pWidth, float pHeight,
	vector<PixelObject> *ppPixelObject,
	vector<ClippingStage> *pClippingStageArray)
{
	ofstream myfile("imagebuffer.txt");
	myfile << "Please write this text to a file.\n this text is written using C++\n";
	IWICBitmapSource* pSource = nullptr;
	IWICBitmapSource* pSecondarySource = nullptr;

	IWICBitmap* pBitmapObject = nullptr;
	IWICBitmapLock* pBitmapLock = nullptr;
	IWICBitmapLock* pBitmapLock1 = nullptr;
	IWICBitmapClipper *pIClipper = nullptr;
	BYTE* pixels = nullptr;
	HRESULT hr = S_OK;
	WCHAR szFileName[MAX_PATH];
	WICRect rcLock = { pXPosition, pYPosition, pWidth, pHeight };
	ID2D1Image* outputImage = nullptr;
	vector <PixelObject> pixelDataVector = vector <PixelObject>();
	HDC src = NULL;
	HDC hDC = NULL;
	WICPixelFormatGUID pixelFormat;
	IWICBitmapSource* piConverter = nullptr;
	UINT pImageHeight = NULL;
	UINT pImageWidth = NULL;
	BYTE* pixelArrayPointer = nullptr;
	D2D1_RECT_F rectangle = D2D1::RectF(0, 0, pXPosition + pWidth, pYPosition + pHeight);
	UINT cbBufferSize = 0;
	BYTE* pixelDataArray = NULL;
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapDecoder* pImageDecoder = nullptr;
	IWICBitmapFrameDecode *pFrame = NULL;
	UINT uiWidth = 0;
	UINT uiHeight = 0;
	WICRect rcClip = { pXPosition, pYPosition, pWidth, pHeight };


	fstream pixelDataFile;
	pixelDataFile.open("C:\\pixels\\ImageClippingExperimentsTests.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);

	pixelDataFile.clear();
	pixelDataFile << " " << endl;
	pixelDataFile << " " << endl;
	pixelDataFile << "New Operation Begin" << endl;
	time_t currentTimeVarStart = time(0);
	tm* currentTimeStart = localtime(&currentTimeVarStart);
	cout << "current time" << currentTimeStart << endl;
	pixelDataFile << "current time" << currentTimeStart << endl;

	vector<ClippingStage> pStageVector = vector<ClippingStage>();

	if (LocateImageFile(hWnd, szFileName, ARRAYSIZE(szFileName)))
	{

		// Prepare iterator pairs to iterate the file content!

		cout << "Stage 1: Decoder" << endl;
		pixelDataFile << "Stage 1: Decoder" << endl;
		currentTimeVarStart = time(0);
		currentTimeStart = localtime(&currentTimeVarStart);

		cout << "Decoder Creation Started At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;
		pixelDataFile << "Decoder Creation Started At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;

		hr = m_pWICFactory->CreateDecoderFromFilename(
			szFileName,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&pImageDecoder);

		time_t currentTimeVarEnd = time(0);
		tm* currentTimeEnd = localtime(&currentTimeVarEnd);

		cout << "Decoder Creation Terminated At: " << currentTimeEnd->tm_hour << ":" << currentTimeEnd->tm_min << ":" << currentTimeEnd->tm_sec << endl;
		pixelDataFile << "Decoder Creation Terminated At: " << currentTimeEnd->tm_hour << ":" << currentTimeEnd->tm_min << ":" << currentTimeEnd->tm_sec << endl;


		ClippingStage pDecoderStage = ClippingStage(0, "Decoder", currentTimeStart, currentTimeEnd);
		pStageVector.push_back(pDecoderStage);


		cout << "Stage 2: Frame" << endl;
		pixelDataFile << "Stage 2: Frame" << endl;
		currentTimeVarStart = time(0);
		currentTimeStart = localtime(&currentTimeVarStart);
		cout << "Frame Creation Started At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;
		pixelDataFile << "Frame Creation Started At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;



		if (SUCCEEDED(hr))
		{
			hr = pImageDecoder->GetFrame(0, &pFrame);
		}


		currentTimeVarEnd = time(0);
		currentTimeEnd = localtime(&currentTimeVarEnd);
		cout << "Frame Creation Terminated At: " << currentTimeEnd->tm_hour << ":" << currentTimeEnd->tm_min << ":" << currentTimeEnd->tm_sec << endl;
		pixelDataFile << "Frame Creation Terminated At: " << currentTimeEnd->tm_hour << ":" << currentTimeEnd->tm_min << ":" << currentTimeEnd->tm_sec << endl;

		ClippingStage pFrameStage = ClippingStage(1, "Frame", currentTimeStart, currentTimeEnd);
		pStageVector.push_back(pFrameStage);


		cout << "Stage 3: Bitmap Source" << endl;
		pixelDataFile << "Stage 3: Bitmap Source" << endl;
		currentTimeVarStart = time(0);
		currentTimeStart = localtime(&currentTimeVarStart);

		cout << "Bitmap Source Started At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;
		pixelDataFile << "Frame Creation Started At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;

		if (SUCCEEDED(hr))
		{
			pSource = pFrame;
			pSource->AddRef();
			pSource->GetSize(&uiWidth, &uiHeight);

			pSecondarySource = pFrame;
			pSecondarySource->AddRef();
			pSecondarySource->GetSize(&uiWidth, &uiHeight);

			IWICPalette* pImagePalette = nullptr;
			pSecondarySource->CopyPalette(pImagePalette);
		}

		WICRect rc = { pXPosition, pYPosition, pWidth, pHeight };


		currentTimeVarEnd = time(0);
		currentTimeEnd = localtime(&currentTimeVarEnd);
		cout << "Bitmap Source Creation Terminated At At: " << currentTimeEnd->tm_hour << ":" << currentTimeEnd->tm_min << ":" << currentTimeEnd->tm_sec << endl;
		pixelDataFile << "Bitmap Source Creation Terminated At: " << currentTimeEnd->tm_hour << ":" << currentTimeEnd->tm_min << ":" << currentTimeEnd->tm_sec << endl;

		ClippingStage pBitmapSourceStage = ClippingStage(2, "Bitmap Source", currentTimeStart, currentTimeEnd);
		pStageVector.push_back(pBitmapSourceStage);



		cout << "Stage 4: Converting Wic Formats to Direct2D Formats" << endl;
		pixelDataFile << "Stage 4: Converting Wic Formats to Direct2D Formats" << endl;
		currentTimeVarStart = time(0);
		currentTimeStart = localtime(&currentTimeVarStart);
		cout << "Format Conversion  Started At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;
		pixelDataFile << "Format Conversion Started At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;



		// Initialize the clipper with the given rectangle of the frame's image data.

		if (SUCCEEDED(hr))
		{
			SafeReleaseA(&m_pConvertedSourceBitmap);
			hr = m_pWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pConvertedSourceBitmap->Initialize(
				pSecondarySource,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeCustom);

		}
		if (SUCCEEDED(hr))
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}
		if (SUCCEEDED(hr))
		{

			SafeReleaseA(&m_pD2DBitmap);
			hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);

		}

		currentTimeVarEnd = time(0);
		currentTimeEnd = localtime(&currentTimeVarEnd);
		cout << "Format Conversion terminated At: " << currentTimeEnd->tm_hour << ":" << currentTimeEnd->tm_min << ":" << currentTimeEnd->tm_sec << endl;
		pixelDataFile << "Format Conversion terminated At: " << currentTimeEnd->tm_hour << ":" << currentTimeEnd->tm_min << ":" << currentTimeEnd->tm_sec << endl;


		ClippingStage pDirect2DStage = ClippingStage(3, "Direct2D Stage", currentTimeStart, currentTimeEnd);
		pStageVector.push_back(pDirect2DStage);


		cout << "Stage 5: Drawing Pixels with Direct2D" << endl;
		pixelDataFile << "Stage 5: Drawing Pixels with Direct2D" << endl;
		currentTimeVarStart = time(0);
		currentTimeStart = localtime(&currentTimeVarStart);
		cout << "Drawing  Started At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;
		pixelDataFile << "Drawing Conversion Started At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;



		if (SUCCEEDED(hr))
		{
			D2D1_SIZE_F pSize = m_pD2DBitmap->GetSize();
			pImageWidth = pSize.width;
			pImageHeight = pSize.height;

			hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidColorBrush);
		}

		if (SUCCEEDED(hr))
		{


			hr = DrawD2DContentAxisAlignedClip(hInstance, hWnd, m_pD2DBitmap, rectangle);

		}


		currentTimeVarEnd = time(0);
		currentTimeEnd = localtime(&currentTimeVarEnd);
		cout << "Bitmap Source Creation Terminated At At: " << currentTimeEnd->tm_hour << ":" << currentTimeEnd->tm_min << ":" << currentTimeEnd->tm_sec << endl;
		pixelDataFile << "Bitmap Source Creation Terminated At: " << currentTimeEnd->tm_hour << ":" << currentTimeEnd->tm_min << ":" << currentTimeEnd->tm_sec << endl;

		ClippingStage pDrawingStage = ClippingStage(4, "Drawing Stage", currentTimeStart, currentTimeEnd);
		pStageVector.push_back(pDrawingStage);

		cout << "Stage 6: Convertion of image data to GDI HBITMAP For Pixel Extraction" << endl;
		pixelDataFile << "Stage 6: Convertion of image data to GDI HBITMAP For Pixel Extraction" << endl;
		currentTimeVarStart = time(0);
		currentTimeStart = localtime(&currentTimeVarStart);
		cout << "Convertion Started At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;
		pixelDataFile << "Convertion Started: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;



		IWICBitmapSource* pConverter = nullptr;
		WICPixelFormatGUID pixelFormat;

		if (SUCCEEDED(hr))
		{


			pSecondarySource->AddRef();
			pSecondarySource->GetSize(&uiWidth, &uiHeight);
		}

		IWICPixelFormatInfo* pixelFormatInfo = nullptr;
		if (SUCCEEDED(hr))
		{
			pSecondarySource->GetPixelFormat(&pixelFormat);
		}

		if (!IsEqualGUID(pixelFormat, GUID_WICPixelFormat128bppPRGBAFloat))
		{

			hr = WICConvertBitmapSource(GUID_WICPixelFormat128bppPRGBAFloat, pSecondarySource, &pConverter);
			IWICBitmapLock* pBitmapLock = nullptr;


			if (SUCCEEDED(hr))
			{
				pSecondarySource->Release();     // the converter has a reference to the source
				pSecondarySource = NULL;         // so we don't need it anymore.
				pSecondarySource = pConverter;   // let's treat the 128bppPABGR converter as the source
			}


			IWICPalette* pImagePalette = nullptr;
			pSecondarySource->CopyPalette(pImagePalette);
		}




		CImageFileData* pImageClass = new CImageFileData((char*)pixelArrayPointer);

		if (pConverter)
		{
			UINT cbStride = pWidth * sizeof(float) * 4;
			UINT cbBufferSize = cbStride;
			float *pixels = new float[cbBufferSize / sizeof(float)];
			if (pHeight < rc.Height && pWidth < rc.Width)
			{
				rc.Height = pHeight;
				rc.Width = pWidth;
			}
			else
			{
				pWidth = rc.Width;
				pHeight = rc.Height;
			}
		}


		UINT bSize = 0;




		UINT rowPitch1 = 4 * pWidth;
		UINT stride = 0; // stride  = bytes per row					 // (width * (bpp/8)) + padding and round to the nearest DWORD
		stride = (((((UINT)pWidth * 32) + 31) & ~31) >> 3);
		bSize = pHeight * pWidth;
		BYTE *pixelArray = new BYTE[(UINT)pHeight * stride];


		pSecondarySource->CopyPixels(NULL, stride, bSize, pixelArray);
		HDC hDC = NULL;
		hDC = GetDC(hWnd);

		COLORREF bkColor = RGB(255, 255, 255);

		SetBkColor(hDC, bkColor);

		HBITMAP map = CreateBitmap((UINT)pWidth,// width. 512 in my case
			(UINT)pHeight, // height
			1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
			8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
			(void*)pixelArray); // pointer to array
		// Temp HDC to copy picture
		src = CreateCompatibleDC(hDC); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
		SelectObject(src, map); // Inserting picture into our temp HDC
		// Copy image from temp HDC to window

		currentTimeVarStart = time(0);
		currentTimeStart = localtime(&currentTimeVarStart);
		cout << "Convertion Terminated At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;
		pixelDataFile << "Drawing terminated At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;

		ClippingStage pGDIStage = ClippingStage(4, "GDI Load Stage", currentTimeStart, currentTimeEnd);
		pStageVector.push_back(pGDIStage);

		cout << "Stage 7: Reading Pixels and Preparing Pixel Vector" << endl;
		pixelDataFile << "Stage 7: Reading Pixels and Preparing Pixel Vector" << endl;
		currentTimeVarStart = time(0);
		currentTimeStart = localtime(&currentTimeVarStart);
		cout << "Pixel Extraction Started At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;
		pixelDataFile << "Pixel Extraction Started At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;





		std::cout << "Pixels Begin" << endl;

		int pixelCounter = 0;


		//	pBitmapObj->GetResolution(&pWidth, &pHeight);

		UINT pYVal = 0;
		UINT pXVal = 0;


		for (pXVal = pXPosition; pXVal < pXPosition + pWidth; ++pXVal)
		{
			for (pYVal = pYPosition; pYVal < pYPosition + pHeight; ++pYVal)
				// Your code here
			{

				std::cout << "Current X Value: " << pXVal << " " << "Current Y Value: " << pYVal << endl;
				COLORREF pCurrentPixel = GetPixel(hDC, pXVal, pYVal);
				int pRedValue = GetRValue(pCurrentPixel);
				int pGreenValue = GetGValue(pCurrentPixel);
				int pBlueValue = GetBValue(pCurrentPixel);
				PixelObject pixelObj = PixelObject(pixelCounter, pXVal, pYVal, pRedValue, pGreenValue, pBlueValue);
				pixelDataVector.push_back(pixelObj);
				std::cout << "Amount of total pixels processed  = " << pixelCounter << endl;
				pixelCounter = pixelCounter + 1;


			}
		}

		std::cout << "The size of pixel buffer = " << pixelDataVector.size() << endl;
		int pAmountOfPixelsLeft = 0;
		pAmountOfPixelsLeft = pixelDataVector.size();


		for (PixelObject currentPixel : pixelDataVector)
		{



			std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
				"X Position = " << currentPixel.xPosition << " " <<
				"Y Position = " << currentPixel.yPosition << " " <<
				"The Value Of Red = " << currentPixel.RMask << " "
				<< "The Value Of Green = " << currentPixel.GMask << " " <<
				"The Value Of Blue = " << currentPixel.BMask << endl;

			pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
			pixelDataFile << "X Position = " << currentPixel.xPosition << endl;
			pixelDataFile << "Y Position = " << currentPixel.yPosition << endl;
			pixelDataFile << "The Value Of Red = " << currentPixel.RMask << endl;
			pixelDataFile << "The Value Of Green = " << currentPixel.GMask << endl;
			pixelDataFile << "The Value Of Blue = " << currentPixel.BMask << endl;

			pAmountOfPixelsLeft--;

		}




		currentTimeVarEnd = time(0);
		currentTimeEnd = localtime(&currentTimeVarEnd);
		cout << "Clipping Terminated at: " << currentTimeEnd->tm_hour << ":" << currentTimeEnd->tm_min << ":" << currentTimeEnd->tm_sec << endl;
		pixelDataFile << "Clipping Terminated at: " << currentTimeEnd->tm_hour << ":" << currentTimeEnd->tm_min << ":" << currentTimeEnd->tm_sec << endl;

		ClippingStage pPixelsStage = ClippingStage(4, "Vector of Pixels", currentTimeStart, currentTimeEnd);
		pStageVector.push_back(pPixelsStage);



		pixelDataFile << "New Operation End" << endl;
		pixelDataFile << " " << endl;
		pixelDataFile << " " << endl;
		pixelDataFile.close();
		std::cout << "Pixels END" << endl;










	}

	ReleaseDC(hWnd, src);
	ReleaseDC(hWnd, hDC);
	DeleteDC(src);
	DeleteDC(hDC);
	SafeReleaseA(&pDecoder);
	SafeReleaseA(&pFrame);
	myfile.close();
	*ppPixelObject = pixelDataVector;
	*pClippingStageArray = pStageVector;
	return hr;
}

HRESULT Direct2DManagementClass::CreateBitmapDrawingClipObjectWithStageRecording(HINSTANCE hInstance, HWND hWnd, LPCSTR pFileName,
	float pXPosition, float pYPosition, float pWidth, float pHeight,
	vector<PixelObject> *ppPixelObject,
	vector<ClippingStage> *pClippingStageArray)
{
	ofstream myfile("imagebuffer.txt");
	myfile << "Please write this text to a file.\n this text is written using C++\n";
	IWICBitmapSource* pSource = nullptr;
	IWICBitmapSource* pSecondarySource = nullptr;
	vector<ClippingStage> pStageVector = vector<ClippingStage>();

	IWICBitmap* pBitmapObject = nullptr;
	IWICBitmapLock* pBitmapLock = nullptr;
	IWICBitmapLock* pBitmapLock1 = nullptr;
	IWICBitmapClipper *pIClipper = nullptr;
	BYTE* pixels = nullptr;
	HRESULT hr = S_OK;
	WCHAR szFileName[MAX_PATH];
	WICRect rcLock = { pXPosition, pYPosition, pWidth, pHeight };
	ID2D1Image* outputImage = nullptr;
	vector <PixelObject> pixelDataVector = vector <PixelObject>();
	HDC src = NULL;
	HDC hDC = NULL;
	WICPixelFormatGUID pixelFormat;
	IWICBitmapSource* piConverter = nullptr;
	UINT pImageHeight = NULL;
	UINT pImageWidth = NULL;
	BYTE* pixelArrayPointer = nullptr;
	D2D1_RECT_F rectangle = D2D1::RectF(0, 0, pXPosition + pWidth, pYPosition + pHeight);
	UINT cbBufferSize = 0;
	BYTE* pixelDataArray = NULL;
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapDecoder* pImageDecoder = nullptr;
	IWICBitmapFrameDecode *pFrame = NULL;
	UINT uiWidth = 0;
	UINT uiHeight = 0;
	WICRect rcClip = { pXPosition, pYPosition, pWidth, pHeight };


	fstream pixelDataFile;
	pixelDataFile.open("C:\\pixels\\ImageClippingExperimentsTests.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);

	pixelDataFile.clear();
	pixelDataFile << " " << endl;
	pixelDataFile << " " << endl;
	pixelDataFile << "New Operation Begin" << endl;
	time_t currentTimeVarStart = time(0);
	tm* currentTimeStart = localtime(&currentTimeVarStart);
	cout << "current time" << currentTimeStart << endl;
	pixelDataFile << "current time" << currentTimeStart << endl;


	
		// Prepare iterator pairs to iterate the file content!

		cout << "Stage 1: Decoder" << endl;
		pixelDataFile << "Stage 1: Decoder" << endl;
		currentTimeVarStart = time(0);
		currentTimeStart = localtime(&currentTimeVarStart);

		cout << "Decoder Creation Started At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;
		pixelDataFile << "Decoder Creation Started At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;

		hr = m_pWICFactory->CreateDecoderFromFilename(
			(LPCWSTR)pFileName,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&pImageDecoder);

		time_t currentTimeVarEnd = time(0);
		tm* currentTimeEnd = localtime(&currentTimeVarEnd);

		cout << "Decoder Creation Terminated At: " << currentTimeEnd->tm_hour << ":" << currentTimeEnd->tm_min << ":" << currentTimeEnd->tm_sec << endl;
		pixelDataFile << "Decoder Creation Terminated At: " << currentTimeEnd->tm_hour << ":" << currentTimeEnd->tm_min << ":" << currentTimeEnd->tm_sec << endl;


		ClippingStage pDecoderStage = ClippingStage(0, "Decoder", currentTimeStart, currentTimeEnd);
		pStageVector.push_back(pDecoderStage);


		cout << "Stage 2: Frame" << endl;
		pixelDataFile << "Stage 2: Frame" << endl;
		currentTimeVarStart = time(0);
		currentTimeStart = localtime(&currentTimeVarStart);
		cout << "Frame Creation Started At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;
		pixelDataFile << "Frame Creation Started At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;



		if (SUCCEEDED(hr))
		{
			hr = pImageDecoder->GetFrame(0, &pFrame);
		}


		currentTimeVarEnd = time(0);
		currentTimeEnd = localtime(&currentTimeVarEnd);
		cout << "Frame Creation Terminated At: " << currentTimeEnd->tm_hour << ":" << currentTimeEnd->tm_min << ":" << currentTimeEnd->tm_sec << endl;
		pixelDataFile << "Frame Creation Terminated At: " << currentTimeEnd->tm_hour << ":" << currentTimeEnd->tm_min << ":" << currentTimeEnd->tm_sec << endl;

		ClippingStage pFrameStage = ClippingStage(1, "Frame", currentTimeStart, currentTimeEnd);
		pStageVector.push_back(pFrameStage);


		cout << "Stage 3: Bitmap Source" << endl;
		pixelDataFile << "Stage 3: Bitmap Source" << endl;
		currentTimeVarStart = time(0);
		currentTimeStart = localtime(&currentTimeVarStart);

		cout << "Bitmap Source Started At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;
		pixelDataFile << "Frame Creation Started At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;

		if (SUCCEEDED(hr))
		{
			pSource = pFrame;
			pSource->AddRef();
			pSource->GetSize(&uiWidth, &uiHeight);

			pSecondarySource = pFrame;
			pSecondarySource->AddRef();
			pSecondarySource->GetSize(&uiWidth, &uiHeight);

			IWICPalette* pImagePalette = nullptr;
			pSecondarySource->CopyPalette(pImagePalette);
		}

		WICRect rc = { pXPosition, pYPosition, pWidth, pHeight };


		currentTimeVarEnd = time(0);
		currentTimeEnd = localtime(&currentTimeVarEnd);
		cout << "Bitmap Source Creation Terminated At At: " << currentTimeEnd->tm_hour << ":" << currentTimeEnd->tm_min << ":" << currentTimeEnd->tm_sec << endl;
		pixelDataFile << "Bitmap Source Creation Terminated At: " << currentTimeEnd->tm_hour << ":" << currentTimeEnd->tm_min << ":" << currentTimeEnd->tm_sec << endl;

		ClippingStage pBitmapSourceStage = ClippingStage(2, "Bitmap Source", currentTimeStart, currentTimeEnd);
		pStageVector.push_back(pBitmapSourceStage);



		cout << "Stage 4: Converting Wic Formats to Direct2D Formats" << endl;
		pixelDataFile << "Stage 4: Converting Wic Formats to Direct2D Formats" << endl;
		currentTimeVarStart = time(0);
		currentTimeStart = localtime(&currentTimeVarStart);
		cout << "Format Conversion  Started At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;
		pixelDataFile << "Format Conversion Started At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;



		// Initialize the clipper with the given rectangle of the frame's image data.

		if (SUCCEEDED(hr))
		{
			SafeReleaseA(&m_pConvertedSourceBitmap);
			hr = m_pWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pConvertedSourceBitmap->Initialize(
				pSecondarySource,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeCustom);

		}
		if (SUCCEEDED(hr))
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}
		if (SUCCEEDED(hr))
		{

			SafeReleaseA(&m_pD2DBitmap);
			hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);

		}

		currentTimeVarEnd = time(0);
		currentTimeEnd = localtime(&currentTimeVarEnd);
		cout << "Format Conversion terminated At: " << currentTimeEnd->tm_hour << ":" << currentTimeEnd->tm_min << ":" << currentTimeEnd->tm_sec << endl;
		pixelDataFile << "Format Conversion terminated At: " << currentTimeEnd->tm_hour << ":" << currentTimeEnd->tm_min << ":" << currentTimeEnd->tm_sec << endl;


		ClippingStage pDirect2DStage = ClippingStage(3, "Direct2D Stage", currentTimeStart, currentTimeEnd);
		pStageVector.push_back(pDirect2DStage);


		cout << "Stage 5: Drawing Pixels with Direct2D" << endl;
		pixelDataFile << "Stage 5: Drawing Pixels with Direct2D" << endl;
		currentTimeVarStart = time(0);
		currentTimeStart = localtime(&currentTimeVarStart);
		cout << "Drawing  Started At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;
		pixelDataFile << "Drawing Conversion Started At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;



		if (SUCCEEDED(hr))
		{
			D2D1_SIZE_F pSize = m_pD2DBitmap->GetSize();
			pImageWidth = pSize.width;
			pImageHeight = pSize.height;

			hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidColorBrush);
		}

		if (SUCCEEDED(hr))
		{


			hr = DrawD2DContentAxisAlignedClip(hInstance, hWnd, m_pD2DBitmap, rectangle);

		}


		currentTimeVarEnd = time(0);
		currentTimeEnd = localtime(&currentTimeVarEnd);
		cout << "Bitmap Source Creation Terminated At At: " << currentTimeEnd->tm_hour << ":" << currentTimeEnd->tm_min << ":" << currentTimeEnd->tm_sec << endl;
		pixelDataFile << "Bitmap Source Creation Terminated At: " << currentTimeEnd->tm_hour << ":" << currentTimeEnd->tm_min << ":" << currentTimeEnd->tm_sec << endl;

		ClippingStage pDrawingStage = ClippingStage(4, "Drawing Stage", currentTimeStart, currentTimeEnd);
		pStageVector.push_back(pDrawingStage);

		cout << "Stage 6: Convertion of image data to GDI HBITMAP For Pixel Extraction" << endl;
		pixelDataFile << "Stage 6: Convertion of image data to GDI HBITMAP For Pixel Extraction" << endl;
		currentTimeVarStart = time(0);
		currentTimeStart = localtime(&currentTimeVarStart);
		cout << "Convertion Started At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;
		pixelDataFile << "Convertion Started: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;



		IWICBitmapSource* pConverter = nullptr;
		

		if (SUCCEEDED(hr))
		{


			pSecondarySource->AddRef();
			pSecondarySource->GetSize(&uiWidth, &uiHeight);
		}

		IWICPixelFormatInfo* pixelFormatInfo = nullptr;
		if (SUCCEEDED(hr))
		{
			pSecondarySource->GetPixelFormat(&pixelFormat);
		}

		if (!IsEqualGUID(pixelFormat, GUID_WICPixelFormat128bppPRGBAFloat))
		{

			hr = WICConvertBitmapSource(GUID_WICPixelFormat128bppPRGBAFloat, pSecondarySource, &pConverter);
			IWICBitmapLock* pBitmapLock = nullptr;


			if (SUCCEEDED(hr))
			{
				pSecondarySource->Release();     // the converter has a reference to the source
				pSecondarySource = NULL;         // so we don't need it anymore.
				pSecondarySource = pConverter;   // let's treat the 128bppPABGR converter as the source
			}


			IWICPalette* pImagePalette = nullptr;
			pSecondarySource->CopyPalette(pImagePalette);
		}




		CImageFileData* pImageClass = new CImageFileData((char*)pixelArrayPointer);

		if (pConverter)
		{
			UINT cbStride = pWidth * sizeof(float) * 4;
			UINT cbBufferSize = cbStride;
			float *pixels = new float[cbBufferSize / sizeof(float)];
			if (pHeight < rc.Height && pWidth < rc.Width)
			{
				rc.Height = pHeight;
				rc.Width = pWidth;
			}
			else
			{
				pWidth = rc.Width;
				pHeight = rc.Height;
			}
		}


		UINT bSize = 0;




		UINT rowPitch1 = 4 * pWidth;
		UINT stride = 0; // stride  = bytes per row					 // (width * (bpp/8)) + padding and round to the nearest DWORD
		stride = (((((UINT)pWidth * 32) + 31) & ~31) >> 3);
		bSize = pHeight * pWidth;
		BYTE *pixelArray = new BYTE[(UINT)pHeight * stride];


		pSecondarySource->CopyPixels(NULL, stride, bSize, pixelArray);
		hDC = NULL;
		hDC = GetDC(hWnd);

		COLORREF bkColor = RGB(255, 255, 255);

		SetBkColor(hDC, bkColor);

		HBITMAP map = CreateBitmap((UINT)pWidth,// width. 512 in my case
			(UINT)pHeight, // height
			1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
			8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
			(void*)pixelArray); // pointer to array
		// Temp HDC to copy picture
		src = CreateCompatibleDC(hDC); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
		SelectObject(src, map); // Inserting picture into our temp HDC
		// Copy image from temp HDC to window

		currentTimeVarStart = time(0);
		currentTimeStart = localtime(&currentTimeVarStart);
		cout << "Convertion Terminated At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;
		pixelDataFile << "Drawing terminated At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;

		ClippingStage pGDIStage = ClippingStage(4, "GDI Load Stage", currentTimeStart, currentTimeEnd);
		pStageVector.push_back(pGDIStage);

		cout << "Stage 7: Reading Pixels and Preparing Pixel Vector" << endl;
		pixelDataFile << "Stage 7: Reading Pixels and Preparing Pixel Vector" << endl;
		currentTimeVarStart = time(0);
		currentTimeStart = localtime(&currentTimeVarStart);
		cout << "Pixel Extraction Started At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;
		pixelDataFile << "Pixel Extraction Started At: " << currentTimeStart->tm_hour << ":" << currentTimeStart->tm_min << ":" << currentTimeStart->tm_sec << endl;





		std::cout << "Pixels Begin" << endl;

		int pixelCounter = 0;


		//	pBitmapObj->GetResolution(&pWidth, &pHeight);

		UINT pYVal = 0;
		UINT pXVal = 0;


		for (pXVal = pXPosition; pXVal < pXPosition + pWidth; ++pXVal)
		{
			for (pYVal = pYPosition; pYVal < pYPosition + pHeight; ++pYVal)
				// Your code here
			{

				std::cout << "Current X Value: " << pXVal << " " << "Current Y Value: " << pYVal << endl;
				COLORREF pCurrentPixel = GetPixel(hDC, pXVal, pYVal);
				int pRedValue = GetRValue(pCurrentPixel);
				int pGreenValue = GetGValue(pCurrentPixel);
				int pBlueValue = GetBValue(pCurrentPixel);
				PixelObject pixelObj = PixelObject(pixelCounter, pXVal, pYVal, pRedValue, pGreenValue, pBlueValue);
				pixelDataVector.push_back(pixelObj);
				std::cout << "Amount of total pixels processed  = " << pixelCounter << endl;
				pixelCounter = pixelCounter + 1;


			}
		}

		std::cout << "The size of pixel buffer = " << pixelDataVector.size() << endl;
		int pAmountOfPixelsLeft = 0;
		pAmountOfPixelsLeft = pixelDataVector.size();

		if (LOOPS_ARE_TURNED_OFF == FALSE)
		{
			for (PixelObject currentPixel : pixelDataVector)
			{



				std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
					"X Position = " << currentPixel.xPosition << " " <<
					"Y Position = " << currentPixel.yPosition << " " <<
					"The Value Of Red = " << currentPixel.RMask << " "
					<< "The Value Of Green = " << currentPixel.GMask << " " <<
					"The Value Of Blue = " << currentPixel.BMask << endl;

				pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
				pixelDataFile << "X Position = " << currentPixel.xPosition << endl;
				pixelDataFile << "Y Position = " << currentPixel.yPosition << endl;
				pixelDataFile << "The Value Of Red = " << currentPixel.RMask << endl;
				pixelDataFile << "The Value Of Green = " << currentPixel.GMask << endl;
				pixelDataFile << "The Value Of Blue = " << currentPixel.BMask << endl;

				pAmountOfPixelsLeft--;

			}

		}
	



		currentTimeVarEnd = time(0);
		currentTimeEnd = localtime(&currentTimeVarEnd);
		cout << "Clipping Terminated at: " << currentTimeEnd->tm_hour << ":" << currentTimeEnd->tm_min << ":" << currentTimeEnd->tm_sec << endl;
		pixelDataFile << "Clipping Terminated at: " << currentTimeEnd->tm_hour << ":" << currentTimeEnd->tm_min << ":" << currentTimeEnd->tm_sec << endl;

		ClippingStage pPixelsStage = ClippingStage(4, "Vector of Pixels", currentTimeStart, currentTimeEnd);
		pStageVector.push_back(pPixelsStage);



		pixelDataFile << "New Operation End" << endl;
		pixelDataFile << " " << endl;
		pixelDataFile << " " << endl;
		pixelDataFile.close();
		std::cout << "Pixels END" << endl;





		ReleaseDC(hWnd, src);
		ReleaseDC(hWnd, hDC);
		DeleteDC(src);
		DeleteDC(hDC);
		SafeReleaseA(&pDecoder);
		SafeReleaseA(&pFrame);
		myfile.close();
		*ppPixelObject = pixelDataVector;
		*pClippingStageArray = pStageVector;
		return hr;




	}

	



HRESULT Direct2DManagementClass::CreateBitmapDrawingClipObjectWithLimitOfPixels(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pWidth, float pHeight,
	float pPixelLimitValue,
	vector<PixelObject> *ppPixelObject)
{
	ofstream myfile("imagebuffer.txt");
	myfile << "Please write this text to a file.\n this text is written using C++\n";
	IWICBitmapSource* pSource = nullptr;
	IWICBitmapSource* pSecondarySource = nullptr;

	IWICBitmap* pBitmapObject = nullptr;
	IWICBitmapLock* pBitmapLock = nullptr;
	IWICBitmapLock* pBitmapLock1 = nullptr;
	IWICBitmapClipper *pIClipper = nullptr;
	BYTE* pixels = nullptr;
	HRESULT hr = S_OK;
	WCHAR szFileName[MAX_PATH];
	WICRect rcLock = { pXPosition, pYPosition, pWidth, pHeight };
	ID2D1Image* outputImage = nullptr;
	vector <PixelObject> pixelDataVector = vector <PixelObject>();
	HDC src = NULL;
	HDC hDC = NULL;
	WICPixelFormatGUID pixelFormat;
	IWICBitmapSource* piConverter = nullptr;
	UINT pImageHeight = NULL;
	UINT pImageWidth = NULL;
	BYTE* pixelArrayPointer = nullptr;
	D2D1_RECT_F rectangle = D2D1::RectF(0, 0, pXPosition + pWidth, pYPosition + pHeight);
	UINT cbBufferSize = 0;
	BYTE* pixelDataArray = NULL;
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapDecoder* pImageDecoder = nullptr;
	IWICBitmapFrameDecode *pFrame = NULL;
	UINT uiWidth = 0;
	UINT uiHeight = 0;
	WICRect rcClip = { pXPosition, pYPosition, pWidth, pHeight };

	if (LocateImageFile(hWnd, szFileName, ARRAYSIZE(szFileName)))
	{

		// Prepare iterator pairs to iterate the file content!



		hr = m_pWICFactory->CreateDecoderFromFilename(
			szFileName,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&pImageDecoder);




		if (SUCCEEDED(hr))
		{
			hr = pImageDecoder->GetFrame(0, &pFrame);
		}



		if (SUCCEEDED(hr))
		{
			pSource = pFrame;
			pSource->AddRef();
			pSource->GetSize(&uiWidth, &uiHeight);

			pSecondarySource = pFrame;
			pSecondarySource->AddRef();
			pSecondarySource->GetSize(&uiWidth, &uiHeight);

			IWICPalette* pImagePalette = nullptr;
			pSecondarySource->CopyPalette(pImagePalette);
		}

		WICRect rc = { pXPosition, pYPosition, pWidth, pHeight };


		if (SUCCEEDED(hr))
		{
			hr = m_pWICFactory->CreateBitmapClipper(&pIClipper);
		}


		// Initialize the clipper with the given rectangle of the frame's image data.
		if (SUCCEEDED(hr))
		{
			hr = pIClipper->Initialize(pSource, &rcClip);
		}


		if (SUCCEEDED(hr))
		{
			SafeReleaseA(&m_pConvertedSourceBitmap);
			hr = m_pWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pConvertedSourceBitmap->Initialize(
				pSecondarySource,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeCustom);

		}
		if (SUCCEEDED(hr))
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}
		if (SUCCEEDED(hr))
		{

			SafeReleaseA(&m_pD2DBitmap);
			hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);

		}



		if (SUCCEEDED(hr))
		{
			D2D1_SIZE_F pSize = m_pD2DBitmap->GetSize();
			pImageWidth = pSize.width;
			pImageHeight = pSize.height;

			hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidColorBrush);
		}

		if (SUCCEEDED(hr))
		{


			hr = DrawD2DContentAxisAlignedClip(hInstance, hWnd, m_pD2DBitmap, rectangle);

		}


		IWICBitmapSource* pConverter = nullptr;
		WICPixelFormatGUID pixelFormat;

		if (SUCCEEDED(hr))
		{


			pSecondarySource->AddRef();
			pSecondarySource->GetSize(&uiWidth, &uiHeight);
		}

		IWICPixelFormatInfo* pixelFormatInfo = nullptr;
		if (SUCCEEDED(hr))
		{
			pSecondarySource->GetPixelFormat(&pixelFormat);
		}

		if (!IsEqualGUID(pixelFormat, GUID_WICPixelFormat128bppPRGBAFloat))
		{

			hr = WICConvertBitmapSource(GUID_WICPixelFormat128bppPRGBAFloat, pSecondarySource, &pConverter);
			IWICBitmapLock* pBitmapLock = nullptr;


			if (SUCCEEDED(hr))
			{
				pSecondarySource->Release();     // the converter has a reference to the source
				pSecondarySource = NULL;         // so we don't need it anymore.
				pSecondarySource = pConverter;   // let's treat the 128bppPABGR converter as the source
			}


			IWICPalette* pImagePalette = nullptr;
			pSecondarySource->CopyPalette(pImagePalette);
		}




		CImageFileData* pImageClass = new CImageFileData((char*)pixelArrayPointer);

		if (pConverter)
		{
			UINT cbStride = pWidth * sizeof(float) * 4;
			UINT cbBufferSize = cbStride;
			float *pixels = new float[cbBufferSize / sizeof(float)];
			if (pHeight < rc.Height && pWidth < rc.Width)
			{
				rc.Height = pHeight;
				rc.Width = pWidth;
			}
			else
			{
				pWidth = rc.Width;
				pHeight = rc.Height;
			}
		}


		UINT bSize = 0;




		UINT rowPitch1 = 4 * pWidth;
		UINT stride = 0; // stride  = bytes per row					 // (width * (bpp/8)) + padding and round to the nearest DWORD
		stride = (((((UINT)pWidth * 32) + 31) & ~31) >> 3);
		bSize = pHeight * pWidth;
		BYTE *pixelArray = new BYTE[(UINT)pHeight * stride];


		pSecondarySource->CopyPixels(NULL, stride, bSize, pixelArray);
		HDC hDC = NULL;
		hDC = GetDC(hWnd);

		COLORREF bkColor = RGB(255, 255, 255);

		SetBkColor(hDC, bkColor);

		HBITMAP map = CreateBitmap((UINT)pWidth,// width. 512 in my case
			(UINT)pHeight, // height
			1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
			8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
			(void*)pixelArray); // pointer to array
		// Temp HDC to copy picture
		src = CreateCompatibleDC(hDC); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
		SelectObject(src, map); // Inserting picture into our temp HDC
		// Copy image from temp HDC to window


		std::cout << "Pixels Begin" << endl;

		int pixelCounter = 0;


		//	pBitmapObj->GetResolution(&pWidth, &pHeight);

		UINT pYVal = 0;
		UINT pXVal = 0;


		for (pXVal = pXPosition; pXVal < pXPosition + pWidth; ++pXVal)
		{
			for (pYVal = pYPosition; pYVal < pYPosition + pHeight; ++pYVal)
				// Your code here
			{

				std::cout << "Current X Value: " << pXVal << " " << "Current Y Value: " << pYVal << endl;
				COLORREF pCurrentPixel = GetPixel(hDC, pXVal, pYVal);
				int pRedValue = GetRValue(pCurrentPixel);
				int pGreenValue = GetGValue(pCurrentPixel);
				int pBlueValue = GetBValue(pCurrentPixel);
				PixelObject pixelObj = PixelObject(pixelCounter, pXVal, pYVal, pRedValue, pGreenValue, pBlueValue);
				pixelDataVector.push_back(pixelObj);
				std::cout << "Amount of total pixels processed  = " << pixelCounter << endl;
				pixelCounter = pixelCounter + 1;

				if (pixelDataVector.size() >= pPixelLimitValue)
				{
					break;
				}

			}
		}

		std::cout << "The size of pixel buffer = " << pixelDataVector.size() << endl;
		int pAmountOfPixelsLeft = 0;
		pAmountOfPixelsLeft = pixelDataVector.size();


		fstream pixelDataFile;
		pixelDataFile.open("C:\\pixels\\ImageClipperTests.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);

		pixelDataFile.clear();
		pixelDataFile << " " << endl;
		pixelDataFile << " " << endl;
		pixelDataFile << "New Operation Begin" << endl;
		time_t currentTime = time(0);
		pixelDataFile << "current time" << currentTime << endl;
		for (PixelObject currentPixel : pixelDataVector)
		{



			std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
				"X Position = " << currentPixel.xPosition << " " <<
				"Y Position = " << currentPixel.yPosition << " " <<
				"The Value Of Red = " << currentPixel.RMask << " "
				<< "The Value Of Green = " << currentPixel.GMask << " " <<
				"The Value Of Blue = " << currentPixel.BMask << endl;

			pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
			pixelDataFile << "X Position = " << pXPosition << endl;
			pixelDataFile << "Y Position = " << pYPosition << endl;
			pixelDataFile << "The Value Of Red = " << currentPixel.RMask << endl;
			pixelDataFile << "The Value Of Green = " << currentPixel.GMask << endl;
			pixelDataFile << "The Value Of Blue = " << currentPixel.BMask << endl;

			pAmountOfPixelsLeft--;

		}




		SaveScreenDataToImage(hInstance, hWnd, pWidth, pHeight, pSecondarySource, (LPCTSTR)L"E:\\RobotData\\SavedClippedRect.jpg");





		pixelDataFile << "New Operation End" << endl;
		pixelDataFile << " " << endl;
		pixelDataFile << " " << endl;
		pixelDataFile.close();
		std::cout << "Pixels END" << endl;










	}

	ReleaseDC(hWnd, src);
	ReleaseDC(hWnd, hDC);
	DeleteDC(src);
	DeleteDC(hDC);
	SafeReleaseA(&pDecoder);
	SafeReleaseA(&pFrame);
	myfile.close();
	*ppPixelObject = pixelDataVector;

	return hr;
}

int  Direct2DManagementClass::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}


HRESULT Direct2DManagementClass::CreateBitmapDrawingClipObjectWithoutD2DForFileName(HINSTANCE hInstance, HWND hWnd, LPCWSTR pFileName, float pXPosition, float pYPosition, float pWidth, float pHeight,
	vector<PixelObject> *ppPixelObject)
{
	ofstream myfile("imagebuffer.txt");
	myfile << "Please write this text to a file.\n this text is written using C++\n";
	IWICBitmapSource* pSource = nullptr;
	IWICBitmapSource* pSecondarySource = nullptr;

	IWICBitmap* pBitmapObject = nullptr;
	IWICBitmapLock* pBitmapLock = nullptr;
	IWICBitmapLock* pBitmapLock1 = nullptr;
	IWICBitmapClipper *pIClipper = nullptr;
	BYTE* pixels = nullptr;
	HRESULT hr = S_OK;
	WCHAR szFileName[MAX_PATH];
	WICRect rcLock = { pXPosition, pYPosition, pWidth, pHeight };
	ID2D1Image* outputImage = nullptr;
	vector <PixelObject> pixelDataVector = vector <PixelObject>();
	HDC src = NULL;
	HDC hDC = NULL;
	WICPixelFormatGUID pixelFormat;
	IWICBitmapSource* piConverter = nullptr;
	UINT pImageHeight = NULL;
	UINT pImageWidth = NULL;
	BYTE* pixelArrayPointer = nullptr;
	D2D1_RECT_F rectangle = D2D1::RectF(0, 0, pXPosition + pWidth, pYPosition + pHeight);
	UINT cbBufferSize = 0;
	BYTE* pixelDataArray = NULL;
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapDecoder* pImageDecoder = nullptr;
	IWICBitmapFrameDecode *pFrame = NULL;
	UINT uiWidth = 0;
	UINT uiHeight = 0;
	WICRect rcClip = { pXPosition, pYPosition, pWidth, pHeight };

		// Prepare iterator pairs to iterate the file content!



		hr = m_pWICFactory->CreateDecoderFromFilename(
			szFileName,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&pImageDecoder);




		if (SUCCEEDED(hr))
		{
			hr = pImageDecoder->GetFrame(0, &pFrame);
		}



		if (SUCCEEDED(hr))
		{
			pSource = pFrame;
			pSource->AddRef();
			pSource->GetSize(&uiWidth, &uiHeight);

			pSecondarySource = pFrame;
			pSecondarySource->AddRef();
			pSecondarySource->GetSize(&uiWidth, &uiHeight);

			IWICPalette* pImagePalette = nullptr;
			pSecondarySource->CopyPalette(pImagePalette);
		}

		WICRect rc = { pXPosition, pYPosition, pWidth, pHeight };


		if (SUCCEEDED(hr))
		{
			hr = m_pWICFactory->CreateBitmapClipper(&pIClipper);
		}


		// Initialize the clipper with the given rectangle of the frame's image data.
		if (SUCCEEDED(hr))
		{
			hr = pIClipper->Initialize(pSource, &rcClip);
		}


		if (SUCCEEDED(hr))
		{
			SafeReleaseA(&m_pConvertedSourceBitmap);
			hr = m_pWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pConvertedSourceBitmap->Initialize(
				pSecondarySource,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeCustom);

		}
		if (SUCCEEDED(hr))
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}
		if (SUCCEEDED(hr))
		{

			SafeReleaseA(&m_pD2DBitmap);
			hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);

		}



		if (SUCCEEDED(hr))
		{
			D2D1_SIZE_F pSize = m_pD2DBitmap->GetSize();
			pImageWidth = pSize.width;
			pImageHeight = pSize.height;

			hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidColorBrush);
		}

		if (SUCCEEDED(hr))
		{


			hr = DrawD2DContentAxisAlignedClip(hInstance, hWnd, m_pD2DBitmap, rectangle);

		}


		IWICBitmapSource* pConverter = nullptr;
	
		if (SUCCEEDED(hr))
		{


			pSecondarySource->AddRef();
			pSecondarySource->GetSize(&uiWidth, &uiHeight);
		}

		IWICPixelFormatInfo* pixelFormatInfo = nullptr;
		if (SUCCEEDED(hr))
		{
			pSecondarySource->GetPixelFormat(&pixelFormat);
		}

		if (!IsEqualGUID(pixelFormat, GUID_WICPixelFormat128bppPRGBAFloat))
		{

			hr = WICConvertBitmapSource(GUID_WICPixelFormat128bppPRGBAFloat, pSecondarySource, &pConverter);
			IWICBitmapLock* pBitmapLock = nullptr;


			if (SUCCEEDED(hr))
			{
				pSecondarySource->Release();     // the converter has a reference to the source
				pSecondarySource = NULL;         // so we don't need it anymore.
				pSecondarySource = pConverter;   // let's treat the 128bppPABGR converter as the source
			}


			IWICPalette* pImagePalette = nullptr;
			pSecondarySource->CopyPalette(pImagePalette);
		}




		CImageFileData* pImageClass = new CImageFileData((char*)pixelArrayPointer);

		if (pConverter)
		{
			UINT cbStride = pWidth * sizeof(float) * 4;
			UINT cbBufferSize = cbStride;
			float *pixels = new float[cbBufferSize / sizeof(float)];
			if (pHeight < rc.Height && pWidth < rc.Width)
			{
				rc.Height = pHeight;
				rc.Width = pWidth;
			}
			else
			{
				pWidth = rc.Width;
				pHeight = rc.Height;
			}
		}


		UINT bSize = 0;




		UINT rowPitch1 = 4 * pWidth;
		UINT stride = 0; // stride  = bytes per row					 // (width * (bpp/8)) + padding and round to the nearest DWORD
		stride = (((((UINT)pWidth * 32) + 31) & ~31) >> 3);
		bSize = pHeight * pWidth;
		BYTE *pixelArray = new BYTE[(UINT)pHeight * stride];


		pSecondarySource->CopyPixels(NULL, stride, bSize, pixelArray);
		 hDC = NULL;
		hDC = GetDC(hWnd);

		COLORREF bkColor = RGB(255, 255, 255);

		SetBkColor(hDC, bkColor);

		HBITMAP map = CreateBitmap((UINT)pWidth,// width. 512 in my case
			(UINT)pHeight, // height
			1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
			8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
			(void*)pixelArray); // pointer to array
		// Temp HDC to copy picture
		src = CreateCompatibleDC(hDC); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
		SelectObject(src, map); // Inserting picture into our temp HDC
		// Copy image from temp HDC to window



		std::cout << "Pixels Begin" << endl;

		int pixelCounter = 0;


		//	pBitmapObj->GetResolution(&pWidth, &pHeight);

		UINT pYVal = 0;
		UINT pXVal = 0;
		for (pXVal = pXPosition; pXVal < pXPosition + pWidth; ++pXVal)
		{
			for (pYVal = pYPosition; pYVal < pYPosition + pHeight; ++pYVal)
				// Your code here
			{

				std::cout << "Current X Value: " << pXVal << " " << "Current Y Value: " << pYVal << endl;
				COLORREF pCurrentPixel = GetPixel(hDC, pXVal, pYVal);
				int pRedValue = GetRValue(pCurrentPixel);
				int pGreenValue = GetGValue(pCurrentPixel);
				int pBlueValue = GetBValue(pCurrentPixel);
				PixelObject pixelObj = PixelObject(pixelCounter, pXVal, pYVal, pRedValue, pGreenValue, pBlueValue);
				pixelDataVector.push_back(pixelObj);
				std::cout << "Amount of total pixels processed  = " << pixelCounter << endl;
				pixelCounter = pixelCounter + 1;

			}
		}

		std::cout << "The size of pixel buffer = " << pixelDataVector.size() << endl;
		int pAmountOfPixelsLeft = 0;
		pAmountOfPixelsLeft = pixelDataVector.size();


		fstream pixelDataFile;
		pixelDataFile.open("C:\\pixels\\ImageClipperTests.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);

		pixelDataFile.clear();
		pixelDataFile << " " << endl;
		pixelDataFile << " " << endl;
		pixelDataFile << "New Operation Begin" << endl;
		time_t currentTime = time(0);
		pixelDataFile << "current time" << currentTime << endl;
		for (PixelObject currentPixel : pixelDataVector)
		{



			std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
				"X Position = " << currentPixel.xPosition << " " <<
				"Y Position = " << currentPixel.yPosition << " " <<
				"The Value Of Red = " << currentPixel.RMask << " "
				<< "The Value Of Green = " << currentPixel.GMask << " " <<
				"The Value Of Blue = " << currentPixel.BMask << endl;

			pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
			pixelDataFile << "X Position = " << pXPosition << endl;
			pixelDataFile << "Y Position = " << pYPosition << endl;
			pixelDataFile << "The Value Of Red = " << currentPixel.RMask << endl;
			pixelDataFile << "The Value Of Green = " << currentPixel.GMask << endl;
			pixelDataFile << "The Value Of Blue = " << currentPixel.BMask << endl;

			pAmountOfPixelsLeft--;

		}










		pixelDataFile << "New Operation End" << endl;
		pixelDataFile << " " << endl;
		pixelDataFile << " " << endl;
		pixelDataFile.close();
		std::cout << "Pixels END" << endl;





		ReleaseDC(hWnd, src);
		ReleaseDC(hWnd, hDC);
		DeleteDC(src);
		DeleteDC(hDC);
		SafeReleaseA(&pDecoder);
		SafeReleaseA(&pFrame);
		myfile.close();
		*ppPixelObject = pixelDataVector;

		return hr;




	}





HRESULT Direct2DManagementClass::CreateBitmapDrawingClipObjectWithoutD2D(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pWidth, float pHeight,
	vector<PixelObject> *ppPixelObject)
{
	ofstream myfile("imagebuffer.txt");
	myfile << "Please write this text to a file.\n this text is written using C++\n";
	IWICBitmapSource* pSource = nullptr;
	IWICBitmapSource* pSecondarySource = nullptr;

	IWICBitmap* pBitmapObject = nullptr;
	IWICBitmapLock* pBitmapLock = nullptr;
	IWICBitmapLock* pBitmapLock1 = nullptr;
	IWICBitmapClipper *pIClipper = nullptr;
	BYTE* pixels = nullptr;
	HRESULT hr = S_OK;
	WCHAR szFileName[MAX_PATH];
	WICRect rcLock = { pXPosition, pYPosition, pWidth, pHeight };
	ID2D1Image* outputImage = nullptr;
	vector <PixelObject> pixelDataVector = vector <PixelObject>();
	HDC src = NULL;
	HDC hDC = NULL;
	WICPixelFormatGUID pixelFormat;
	IWICBitmapSource* piConverter = nullptr;
	UINT pImageHeight = NULL;
	UINT pImageWidth = NULL;
	BYTE* pixelArrayPointer = nullptr;
	D2D1_RECT_F rectangle = D2D1::RectF(0, 0, pXPosition + pWidth, pYPosition + pHeight);
	UINT cbBufferSize = 0;
	BYTE* pixelDataArray = NULL;
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapDecoder* pImageDecoder = nullptr;
	IWICBitmapFrameDecode *pFrame = NULL;
	UINT uiWidth = 0;
	UINT uiHeight = 0;
	WICRect rcClip = { pXPosition, pYPosition, pWidth, pHeight };

	if (LocateImageFile(hWnd, szFileName, ARRAYSIZE(szFileName)))
	{

		// Prepare iterator pairs to iterate the file content!



		hr = m_pWICFactory->CreateDecoderFromFilename(
			szFileName,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&pImageDecoder);




		if (SUCCEEDED(hr))
		{
			hr = pImageDecoder->GetFrame(0, &pFrame);
		}



		if (SUCCEEDED(hr))
		{
			pSource = pFrame;
			pSource->AddRef();
			pSource->GetSize(&uiWidth, &uiHeight);

			pSecondarySource = pFrame;
			pSecondarySource->AddRef();
			pSecondarySource->GetSize(&uiWidth, &uiHeight);

			IWICPalette* pImagePalette = nullptr;
			pSecondarySource->CopyPalette(pImagePalette);
		}

		WICRect rc = { pXPosition, pYPosition, pWidth, pHeight };


		if (SUCCEEDED(hr))
		{
			hr = m_pWICFactory->CreateBitmapClipper(&pIClipper);
		}


		// Initialize the clipper with the given rectangle of the frame's image data.
		if (SUCCEEDED(hr))
		{
			hr = pIClipper->Initialize(pSource, &rcClip);
		}


		if (SUCCEEDED(hr))
		{
			SafeReleaseA(&m_pConvertedSourceBitmap);
			hr = m_pWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pConvertedSourceBitmap->Initialize(
				pSecondarySource,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeCustom);

		}
		if (SUCCEEDED(hr))
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}
		if (SUCCEEDED(hr))
		{

			SafeReleaseA(&m_pD2DBitmap);
			hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);

		}



		if (SUCCEEDED(hr))
		{
			D2D1_SIZE_F pSize = m_pD2DBitmap->GetSize();
			pImageWidth = pSize.width;
			pImageHeight = pSize.height;

			hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidColorBrush);
		}

		if (SUCCEEDED(hr))
		{


			hr = DrawD2DContentAxisAlignedClip(hInstance, hWnd, m_pD2DBitmap, rectangle);

		}


		IWICBitmapSource* pConverter = nullptr;
		WICPixelFormatGUID pixelFormat;

		if (SUCCEEDED(hr))
		{


			pSecondarySource->AddRef();
			pSecondarySource->GetSize(&uiWidth, &uiHeight);
		}

		IWICPixelFormatInfo* pixelFormatInfo = nullptr;
		if (SUCCEEDED(hr))
		{
			pSecondarySource->GetPixelFormat(&pixelFormat);
		}

		if (!IsEqualGUID(pixelFormat, GUID_WICPixelFormat128bppPRGBAFloat))
		{

			hr = WICConvertBitmapSource(GUID_WICPixelFormat128bppPRGBAFloat, pSecondarySource, &pConverter);
			IWICBitmapLock* pBitmapLock = nullptr;


			if (SUCCEEDED(hr))
			{
				pSecondarySource->Release();     // the converter has a reference to the source
				pSecondarySource = NULL;         // so we don't need it anymore.
				pSecondarySource = pConverter;   // let's treat the 128bppPABGR converter as the source
			}


			IWICPalette* pImagePalette = nullptr;
			pSecondarySource->CopyPalette(pImagePalette);
		}




		CImageFileData* pImageClass = new CImageFileData((char*)pixelArrayPointer);

		if (pConverter)
		{
			UINT cbStride = pWidth * sizeof(float) * 4;
			UINT cbBufferSize = cbStride;
			float *pixels = new float[cbBufferSize / sizeof(float)];
			if (pHeight < rc.Height && pWidth < rc.Width)
			{
				rc.Height = pHeight;
				rc.Width = pWidth;
			}
			else
			{
				pWidth = rc.Width;
				pHeight = rc.Height;
			}
		}


		UINT bSize = 0;




		UINT rowPitch1 = 4 * pWidth;
		UINT stride = 0; // stride  = bytes per row					 // (width * (bpp/8)) + padding and round to the nearest DWORD
		stride = (((((UINT)pWidth * 32) + 31) & ~31) >> 3);
		bSize = pHeight * pWidth;
		BYTE *pixelArray = new BYTE[(UINT)pHeight * stride];


		pSecondarySource->CopyPixels(NULL, stride, bSize, pixelArray);
		HDC hDC = NULL;
		hDC = GetDC(hWnd);

		COLORREF bkColor = RGB(255, 255, 255);

		SetBkColor(hDC, bkColor);

		HBITMAP map = CreateBitmap((UINT)pWidth,// width. 512 in my case
			(UINT)pHeight, // height
			1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
			8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
			(void*)pixelArray); // pointer to array
		// Temp HDC to copy picture
		src = CreateCompatibleDC(hDC); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
		SelectObject(src, map); // Inserting picture into our temp HDC
		// Copy image from temp HDC to window

	

		std::cout << "Pixels Begin" << endl;

		int pixelCounter = 0;


		//	pBitmapObj->GetResolution(&pWidth, &pHeight);

		UINT pYVal = 0;
		UINT pXVal = 0;
		for (pXVal = pXPosition; pXVal < pXPosition + pWidth; ++pXVal)
		{
			for (pYVal = pYPosition; pYVal < pYPosition + pHeight; ++pYVal)
				// Your code here
			{

				std::cout << "Current X Value: " << pXVal << " " << "Current Y Value: " << pYVal << endl;
				COLORREF pCurrentPixel = GetPixel(hDC, pXVal, pYVal);
				int pRedValue = GetRValue(pCurrentPixel);
				int pGreenValue = GetGValue(pCurrentPixel);
				int pBlueValue = GetBValue(pCurrentPixel);
				PixelObject pixelObj = PixelObject(pixelCounter, pXVal, pYVal, pRedValue, pGreenValue, pBlueValue);
				pixelDataVector.push_back(pixelObj);
				std::cout << "Amount of total pixels processed  = " << pixelCounter << endl;
				pixelCounter = pixelCounter + 1;

			}
		}

		std::cout << "The size of pixel buffer = " << pixelDataVector.size() << endl;
		int pAmountOfPixelsLeft = 0;
		pAmountOfPixelsLeft = pixelDataVector.size();


		fstream pixelDataFile;
		pixelDataFile.open("C:\\pixels\\ImageClipperTests.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);

		pixelDataFile.clear();
		pixelDataFile << " " << endl;
		pixelDataFile << " " << endl;
		pixelDataFile << "New Operation Begin" << endl;
		time_t currentTime = time(0);
		pixelDataFile << "current time" << currentTime << endl;
		for (PixelObject currentPixel : pixelDataVector)
		{



			std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
				"X Position = " << currentPixel.xPosition << " " <<
				"Y Position = " << currentPixel.yPosition << " " <<
				"The Value Of Red = " << currentPixel.RMask << " "
				<< "The Value Of Green = " << currentPixel.GMask << " " <<
				"The Value Of Blue = " << currentPixel.BMask << endl;

			pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
			pixelDataFile << "X Position = " << pXPosition << endl;
			pixelDataFile << "Y Position = " << pYPosition << endl;
			pixelDataFile << "The Value Of Red = " << currentPixel.RMask << endl;
			pixelDataFile << "The Value Of Green = " << currentPixel.GMask << endl;
			pixelDataFile << "The Value Of Blue = " << currentPixel.BMask << endl;

			pAmountOfPixelsLeft--;

		}










		pixelDataFile << "New Operation End" << endl;
		pixelDataFile << " " << endl;
		pixelDataFile << " " << endl;
		pixelDataFile.close();
		std::cout << "Pixels END" << endl;










	}

	ReleaseDC(hWnd, src);
	ReleaseDC(hWnd, hDC);
	DeleteDC(src);
	DeleteDC(hDC);
	SafeReleaseA(&pDecoder);
	SafeReleaseA(&pFrame);
	myfile.close();
	*ppPixelObject = pixelDataVector;

	return hr;
}



HRESULT Direct2DManagementClass::DrawSampleTextOnTheScreenGDI(HINSTANCE hInstance, HWND hWnd, LPCWSTR pMessage)
{
	HRESULT hr = S_OK;
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;
	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	D2D1_POINT_2F centre = { pWidth / 2.0f, pHeight / 2.0f };
	D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
	D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
	int clippedAreaWidth = clippedArea.right - clippedArea.left;
	int clippedAreaHeight = clippedArea.bottom - clippedArea.top;

	BeginPaint(hWnd, &ps);
	HDC hDC = NULL;
	HDC src = NULL;	fstream pixelDataFile;
	pixelDataFile.open("C:\\pixels\\SetPixelTextFile.txt", fstream::in | fstream::out | fstream::trunc);
	pixelDataFile.clear();

	hDC = NULL;
	ReleaseDC(hWnd, hDC);
	hDC = GetDC(hWnd);
	src = CreateCompatibleDC(hDC);

	HBITMAP pBitmapObj;

	HGDIOBJ hPen = GetStockObject(BLACK_PEN);
	HGDIOBJ hOldPen = SelectObject(hDC, hPen);


	if (hDC)
	{
		TextOut(hDC,
			pWidth / 2.0f,
			pHeight / 2.0f,
			pMessage,
			wcslen(pMessage));

	}
	EndPaint(hWnd, &ps);
	return SUCCEEDED(hr) ? 0 : 1;
}



HRESULT Direct2DManagementClass::DrawMessageArray(HINSTANCE hInstance, HWND hWnd, vector<WCHAR*> pMessageArray,
	UINT pFontSize)
{
	HRESULT hr = S_OK;
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;
	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	D2D1_POINT_2F centre = { pWidth / 2.0f, pHeight / 2.0f };
	D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
	D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
	int clippedAreaWidth = clippedArea.right - clippedArea.left;
	int clippedAreaHeight = clippedArea.bottom - clippedArea.top;
	hr = CreateDeviceResources(hInstance, hWnd);
	ID2D1SolidColorBrush* pGreenBrush = nullptr;
	ID2D1SolidColorBrush* pRedBrush = nullptr;
	ID2D1SolidColorBrush* pYellowBrush = nullptr;
	ID2D1SolidColorBrush* pBlackBrush = nullptr;
	ID2D1SolidColorBrush* pWhiteBrush = nullptr;
	IDWriteTextFormat* pTextFormat = nullptr;
	IDWriteTextLayout* pTextLayout = nullptr;
	IDWriteBitmapRenderTarget* pDWriteBitmapRenderTarget = nullptr;

	IDWriteFactory* pDwriteFactoryObjectVar = nullptr;


	HDC hDC = BeginPaint(hWnd, &ps);
	if (hDC)
	{

		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}

		m_pD2DDeviceContext1->BeginDraw();
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));
		m_pD2DDeviceContext1->SetTransform(D2D1::IdentityMatrix());
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);


		ID2D1Brush* pBlackBrushInterface = nullptr;
		pBlackBrushInterface = pBlackBrush;


		D2D1_POINT_2F centreTop = { pWidth / 2.0f, pHeight };
		D2D1_POINT_2F centreLeftTop = { pWidth / 2.0f, pHeight };

		D2D1_POINT_2F centre = { pWidth / 2.0f, pHeight / 2.0f };
		D2D1_POINT_2F centreLeft = { pWidth / 2.0f - 320, pHeight / 2.0f };
		D2D1_POINT_2F centreRight = { pWidth / 2.0f + 320, pHeight / 2.0f };

		D2D1_POINT_2F centre0 = { pWidth / 2.0f, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreLeft0 = { pWidth / 2.0f - 320, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreRight0 = { pWidth / 2.0f + 320, pHeight / 2.0f - 20 };



		IDWriteGdiInterop* pGDIInterop = nullptr;
		IDWritePixelSnapping* pPixelSnapping = nullptr;


		cout << "Creating A text Format for Gabriola Font" << endl;

		IDWriteFontFace* pFontFaceObject = nullptr;
		IDWriteFont* pFontObject = nullptr;







		std::cout << "Setting Text Layout Information 0" << endl;
		IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
		IDWriteTextLayout* pTextLayoutInformation1 = nullptr;
		IDWriteTextLayout* pTextLayoutInformation2 = nullptr;
		IDWriteBitmapRenderTarget* pTargetDWrite = nullptr;


		IDWriteTextFormat* pTextFormat = nullptr;
		hr = pFactoryObjectDWrite->CreateTextFormat(
			L"Times New Roman",              // Font family name.
			NULL,                       // Font collection (NULL sets it to use the system font collection).
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			pFontSize,
			L"en-us",
			&pTextFormat
			);

		static const WCHAR pText0[] = L"Hello World";
		UINT dpiScaleX = DEFAULT_DPI;
		UINT dpiScaleY = DEFAULT_DPI;
		std::cout << "Setting Text Alignment Object" << endl;
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
		DWORD dwFlags = 0;
		HMONITOR pMonitorValue = NULL;
		pMonitorValue = MonitorFromWindow(
			 hWnd,
			dwFlags
			);


	
		
		D2D1_RECT_F layoutRect = D2D1::RectF(
			static_cast<FLOAT>(rc.left) / dpiScaleX,
			static_cast<FLOAT>(rc.top) / dpiScaleY,
			static_cast<FLOAT>(rc.right - rc.left) / dpiScaleX,
			static_cast<FLOAT>(rc.bottom - rc.top) / dpiScaleY
			);

		


		hr = pFactoryObjectDWrite->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			DWRITE_FONT_STYLE_ITALIC,
			DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
			pFontSize,
			L"",
			&pTextFormat);

		std::cout << "Setting Text Alignment Object" << endl;
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);


		vector <IDWriteTextLayout*> pTextLayoutArray = vector<IDWriteTextLayout*>();


		for (WCHAR* pMessageToDraw : pMessageArray)
		{
			IDWriteTextLayout* pTextLayoutVar = nullptr;
			static const UINT stringLength0 = wcslen(pMessageToDraw) + 1;
			std::cout << "Setting Text Layout Information 0" << endl;
			hr = pFactoryObjectDWrite->CreateTextLayout(
				pMessageToDraw,
				stringLength0,
				pTextFormat,
				pWidth, // maxWidth
				pHeight, // maxHeight
				&pTextLayoutInformation0
				);
			pTextLayoutArray.push_back(pTextLayoutVar);

		}

		



		pFactoryObjectDWrite->GetGdiInterop(&pGDIInterop);

		IDWriteTextRenderer1* pTextRenderer = nullptr;
		

		hr = pGDIInterop->CreateBitmapRenderTarget(hDC, pWidth, pHeight,
			&pTargetDWrite);
	
		std::cout << "Setting Text Layout Information 0" << endl;
		pTextLayoutInformation0->SetMaxHeight(90);
		pTextLayoutInformation0->SetMaxWidth(90);
		pTextLayoutInformation0->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation0->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
		pTextLayoutInformation0->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

		RECT rcRect = { static_cast<FLOAT>(rc.left) / dpiScaleX,
			static_cast<FLOAT>(rc.top) / dpiScaleY,
			static_cast<FLOAT>(rc.right - rc.left) / dpiScaleX,
			static_cast<FLOAT>(rc.bottom - rc.top) / dpiScaleY };
	


		ID2D1Brush* pBaseBursh = nullptr;
		hr = pBlackBrush->QueryInterface(__uuidof(pBaseBursh), reinterpret_cast<void**>(&pBaseBursh));



		D2D1_POINT_2F pPointVar = D2D1::Point2F((rcRect.right - rcRect.left) / 2.0f, (rcRect.bottom - rcRect.top) / 2.0f);

	
		for (IDWriteTextLayout* pTextLayoutVar : pTextLayoutArray)
		{
			m_pD2DDeviceContext1->DrawTextLayout(centre, pTextLayoutInformation0, pBaseBursh);
			centre.y = centre.y + 30;
		}
		
	
		if (hr == D2DERR_RECREATE_TARGET)
		{
			SafeReleaseA(&m_pD2DBitmap);
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}
		m_pD2DDeviceContext1->EndDraw();


		pSwapChainObjectD2D->Present(0, 0);

		EndPaint(hWnd, &ps);
	}
		return hr;
	}

	HRESULT Direct2DManagementClass::GetDesktopDPI(HINSTANCE hInstance, HWND hWnd, float* pDPIX, float* pDPIY)
	{
		HRESULT hr = S_OK;
		float pDPIXValue = 0.0f;
		float pDPIYValue = 0.0f;
		if (m_pD2DFactory == nullptr)
		{
			CreateDeviceIndependentResources(hInstance, hWnd);
		}
		m_pD2DFactory->GetDesktopDpi(&pDPIXValue, &pDPIYValue);

		*pDPIX = pDPIXValue;
		*pDPIY = pDPIYValue;

		return hr;
	}



	HRESULT Direct2DManagementClass::DrawSampleTextOnTheScreenMultiple(HINSTANCE hInstance, HWND hWnd, WCHAR* pMessage0,
		WCHAR* pMessage1,
		UINT pFontSize)
	{
		HRESULT hr = S_OK;
		BYTE* selectedAreaData = NULL;
		PAINTSTRUCT ps;
		RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
		GetClientRect(hWnd, &rc);
		UINT pWidth = rc.right - rc.left;
		UINT pHeight = rc.bottom - rc.top;
		D2D1_POINT_2F centre = { pWidth / 2.0f, pHeight / 2.0f };
		D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
		D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
		int clippedAreaWidth = clippedArea.right - clippedArea.left;
		int clippedAreaHeight = clippedArea.bottom - clippedArea.top;
		hr = CreateDeviceResources(hInstance, hWnd);
		LPCWSTR pStringVar0 = (LPCWSTR)pMessage0;
		LPCWSTR pStringVar1 = (LPCWSTR)pMessage1;
		ID2D1SolidColorBrush* pGreenBrush = nullptr;
		ID2D1SolidColorBrush* pRedBrush = nullptr;
		ID2D1SolidColorBrush* pYellowBrush = nullptr;
		ID2D1SolidColorBrush* pBlackBrush = nullptr;
		ID2D1SolidColorBrush* pWhiteBrush = nullptr;
		IDWriteTextFormat* pTextFormat = nullptr;
		IDWriteTextLayout* pTextLayout = nullptr;
		IDWriteBitmapRenderTarget* pDWriteBitmapRenderTarget = nullptr;

		IDWriteFactory* pDwriteFactoryObjectVar = nullptr;


		HDC hDC = BeginPaint(hWnd, &ps);
		if (hDC)
		{

			if (m_pD2DDeviceContext1 == nullptr)
			{
				hr = CreateDeviceResources(hInstance, hWnd);
			}

			m_pD2DDeviceContext1->BeginDraw();
			m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));
			m_pD2DDeviceContext1->SetTransform(D2D1::IdentityMatrix());
			hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);


			ID2D1Brush* pBlackBrushInterface = nullptr;
			pBlackBrushInterface = pBlackBrush;



			D2D1_POINT_2F centre = { pWidth / 2.0f, pHeight / 2.0f };
			D2D1_POINT_2F centreTop = { pWidth / 2.0f, pHeight / 2.0f - 100 };

			D2D1_POINT_2F centreBottom = { pWidth / 2.0f, pHeight / 2.0f + 100};

			D2D1_POINT_2F centreLeft = { pWidth / 2.0f - 320, pHeight / 2.0f };
			D2D1_POINT_2F centreRight = { pWidth / 2.0f + 320, pHeight / 2.0f };




			D2D1_POINT_2F centre0 = { pWidth / 2.0f, pHeight / 2.0f - 20 };
			D2D1_POINT_2F centreLeft0 = { pWidth / 2.0f - 320, pHeight / 2.0f - 20 };
			D2D1_POINT_2F centreRight0 = { pWidth / 2.0f + 320, pHeight / 2.0f - 20 };



			IDWriteGdiInterop* pGDIInterop = nullptr;
			IDWritePixelSnapping* pPixelSnapping = nullptr;


			cout << "Creating A text Format for Gabriola Font" << endl;

			IDWriteFontFace* pFontFaceObject = nullptr;
			IDWriteFont* pFontObject = nullptr;







			std::cout << "Setting Text Layout Information 0" << endl;
			IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
			IDWriteTextLayout* pTextLayoutInformation1 = nullptr;
			IDWriteTextLayout* pTextLayoutInformation2 = nullptr;
			IDWriteBitmapRenderTarget* pTargetDWrite = nullptr;


			IDWriteTextFormat* pTextFormat = nullptr;
			hr = pFactoryObjectDWrite->CreateTextFormat(
				L"Times New Roman",              // Font family name.
				NULL,                       // Font collection (NULL sets it to use the system font collection).
				DWRITE_FONT_WEIGHT_REGULAR,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				pFontSize,
				L"en-us",
				&pTextFormat
				);

			static const WCHAR pText0[] = L"Hello World";
			UINT dpiScaleX = DEFAULT_DPI;
			UINT dpiScaleY = DEFAULT_DPI;
			std::cout << "Setting Text Alignment Object" << endl;
			pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
			DWORD dwFlags = 0;
			HMONITOR pMonitorValue = NULL;
			pMonitorValue = MonitorFromWindow(
				hWnd,
				dwFlags
				);



			static const UINT stringLength0 = wcslen(pMessage0) + 1;
			static const UINT stringLength1 = wcslen(pMessage1) + 1;

			D2D1_RECT_F layoutRect = D2D1::RectF(
				static_cast<FLOAT>(rc.left) / dpiScaleX,
				static_cast<FLOAT>(rc.top) / dpiScaleY,
				static_cast<FLOAT>(rc.right - rc.left) / dpiScaleX,
				static_cast<FLOAT>(rc.bottom - rc.top) / dpiScaleY
				);




			hr = pFactoryObjectDWrite->CreateTextFormat(
				L"Times New Roman",
				NULL,
				DWRITE_FONT_WEIGHT_EXTRA_BOLD,
				DWRITE_FONT_STYLE_ITALIC,
				DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
				pFontSize,
				L"",
				&pTextFormat);

			std::cout << "Setting Text Alignment Object" << endl;
			pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);


			std::cout << "Setting Text Layout Information 0" << endl;
			hr = pFactoryObjectDWrite->CreateTextLayout(
				pMessage0,
				stringLength0,
				pTextFormat,
				pWidth, // maxWidth
				pHeight, // maxHeight
				&pTextLayoutInformation0
				);

			std::cout << "Setting Text Layout Information 0" << endl;
			hr = pFactoryObjectDWrite->CreateTextLayout(
				pMessage1,
				stringLength1,
				pTextFormat,
				pWidth, // maxWidth
				pHeight, // maxHeight
				&pTextLayoutInformation1
				);

			pFactoryObjectDWrite->GetGdiInterop(&pGDIInterop);

			IDWriteTextRenderer1* pTextRenderer = nullptr;


			hr = pGDIInterop->CreateBitmapRenderTarget(hDC, pWidth, pHeight,
				&pTargetDWrite);

			std::cout << "Setting Text Layout Information 0" << endl;
			pTextLayoutInformation0->SetMaxHeight(90);
			pTextLayoutInformation0->SetMaxWidth(500);
			pTextLayoutInformation0->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
			pTextLayoutInformation0->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
			pTextLayoutInformation0->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);


			std::cout << "Setting Text Layout Information 0" << endl;
			pTextLayoutInformation1->SetMaxHeight(90);
			pTextLayoutInformation1->SetMaxWidth(500);
			pTextLayoutInformation1->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
			pTextLayoutInformation1->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
			pTextLayoutInformation1->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

			RECT rcRect = { static_cast<FLOAT>(rc.left) / dpiScaleX,
				static_cast<FLOAT>(rc.top) / dpiScaleY,
				static_cast<FLOAT>(rc.right - rc.left) / dpiScaleX,
				static_cast<FLOAT>(rc.bottom - rc.top) / dpiScaleY };



			ID2D1Brush* pBaseBursh = nullptr;
			hr = pBlackBrush->QueryInterface(__uuidof(pBaseBursh), reinterpret_cast<void**>(&pBaseBursh));



			D2D1_POINT_2F pPointVar = D2D1::Point2F((rcRect.right - rcRect.left) / 2.0f, (rcRect.bottom - rcRect.top) / 2.0f);

			UINT32 cTextLength0 = (UINT32)wcslen(pMessage0);
			UINT32 cTextLength1 = (UINT32)wcslen(pMessage1);



			m_pD2DDeviceContext1->DrawTextLayout(centre, pTextLayoutInformation0, pBaseBursh);
			m_pD2DDeviceContext1->DrawTextLayout(centreBottom, pTextLayoutInformation1, pBaseBursh);

			if (hr == D2DERR_RECREATE_TARGET)
			{
				SafeReleaseA(&m_pD2DBitmap);
				hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
			}
			m_pD2DDeviceContext1->EndDraw();


			pSwapChainObjectD2D->Present(0, 0);

			EndPaint(hWnd, &ps);
		}
		return hr;
	}




	HRESULT Direct2DManagementClass::DrawRoadSignImageComparisonPercents(HINSTANCE hInstance, HWND hWnd, UINT pAmountOfIdenticalPixel, UINT pAmountOfDifferentPixels)
	{
		HRESULT hr = S_OK;
		ClearRoadSignImageComparisonPercents(hInstance, hWnd);
		BYTE* selectedAreaData = NULL;
		PAINTSTRUCT ps;

		RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
		GetClientRect(hWnd, &rc);
		UINT pWidth = rc.right - rc.left;
		UINT pHeight = rc.bottom - rc.top;
		D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
		D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
		int clippedAreaWidth = clippedArea.right - clippedArea.left;
		int clippedAreaHeight = clippedArea.bottom - clippedArea.top;

		ID2D1SolidColorBrush* pGreenBrush = nullptr;
		ID2D1SolidColorBrush* pRedBrush = nullptr;
		ID2D1SolidColorBrush* pYellowBrush = nullptr;
		ID2D1SolidColorBrush* pBlackBrush = nullptr;
		ID2D1SolidColorBrush* pWhiteBrush = nullptr;

		vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();
		static const WCHAR pText0[] = L"33 Percents";
		static const WCHAR pText1[] = L"66 Percents";
		static const WCHAR pText2[] = L"100 Percents";
		static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
		static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
		static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;


		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);


		HDC hDC = BeginPaint(hWnd, &ps);
		if (hDC)
		{
			if (m_pD2DDeviceContext1 == nullptr)
			{
				hr = CreateDeviceResources(hInstance, hWnd);
			}



			hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenBrush);
			hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedBrush);
			hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteBrush);
			hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &pYellowBrush);



			D2D1_POINT_2F centre = { pWidth / 2.0f, pHeight / 2.0f };
			D2D1_POINT_2F centreLeft = { pWidth / 2.0f - 320, pHeight / 2.0f };
			D2D1_POINT_2F centreRight = { pWidth / 2.0f + 320, pHeight / 2.0f };

			D2D1_POINT_2F centre0 = { pWidth / 2.0f, pHeight / 2.0f - 20 };
			D2D1_POINT_2F centreLeft0 = { pWidth / 2.0f - 320, pHeight / 2.0f - 20 };
			D2D1_POINT_2F centreRight0 = { pWidth / 2.0f + 320, pHeight / 2.0f - 20 };

			D2D1_ELLIPSE ellipse = { centre, 150, 150 };
			D2D1_ELLIPSE ellipse0 = { centreLeft, 150, 150 };
			D2D1_ELLIPSE ellipse1 = { centreRight, 150, 150 };

			pEllipseArray.push_back(ellipse);
			pEllipseArray.push_back(ellipse0);
			pEllipseArray.push_back(ellipse1);
			m_pD2DDeviceContext1->BeginDraw();

			// draw the ellipses
			m_pD2DDeviceContext1->DrawEllipse(ellipse, pBlackBrush, 25);
			m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
			m_pD2DDeviceContext1->DrawEllipse(ellipse0, pBlackBrush, 25);
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
			m_pD2DDeviceContext1->DrawEllipse(ellipse1, pBlackBrush, 25);
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);

			IDWriteTextFormat* pTextFormat = nullptr;
			hr = m_pDWriteFactory->CreateTextFormat(
				L"Times New Roman",
				NULL,
				DWRITE_FONT_WEIGHT_EXTRA_BOLD,
				DWRITE_FONT_STYLE_ITALIC,
				DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
				24,
				L"",
				&pTextFormat);

			std::cout << "Setting Text Alignment Object" << endl;
			pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);


			std::cout << "Setting Text Layout Information 0" << endl;
			IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
			hr = m_pDWriteFactory->CreateTextLayout(
				pText0,
				stringLength0,
				pTextFormat,
				40, // maxWidth
				40, // maxHeight
				&pTextLayoutInformation0
				);

			std::cout << "Setting Text Layout Information 0" << endl;
			pTextLayoutInformation0->SetMaxHeight(40);
			pTextLayoutInformation0->SetMaxWidth(40);
			pTextLayoutInformation0->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
			pTextLayoutInformation0->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

			IDWriteTextLayout* pTextLayoutInformation1 = nullptr;
			hr = m_pDWriteFactory->CreateTextLayout(
				pText1,
				stringLength1,
				pTextFormat,
				40, // maxWidth
				40, // maxHeight
				&pTextLayoutInformation1
				);
			pTextLayoutInformation1->SetMaxHeight(40);
			pTextLayoutInformation1->SetMaxWidth(40);
			pTextLayoutInformation1->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
			pTextLayoutInformation1->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

			std::cout << "Setting Text Layout Information 1" << endl;
			IDWriteTextLayout* pTextLayoutInformation2 = nullptr;
			hr = m_pDWriteFactory->CreateTextLayout(
				pText2,
				stringLength2,
				pTextFormat,
				40, // maxWidth
				40, // maxHeight
				&pTextLayoutInformation2
				);
			pTextLayoutInformation2->SetMaxHeight(40);
			pTextLayoutInformation2->SetMaxWidth(40);
			pTextLayoutInformation2->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
			pTextLayoutInformation2->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);






			std::cout << "Filling The Relevant Circle With Color" << endl;

			if ((pAmountOfIdenticalPixel > 0) && (pAmountOfDifferentPixels < pAmountOfIdenticalPixel))
			{
				m_pD2DDeviceContext1->FillEllipse(ellipse0, pGreenBrush);
				m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
				m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);
			};

			if ((pAmountOfIdenticalPixel > 0) && (pAmountOfDifferentPixels == pAmountOfIdenticalPixel))
			{
				m_pD2DDeviceContext1->FillEllipse(ellipse, pYellowBrush);
				m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
				m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);
			}

			if ((pAmountOfDifferentPixels > 0) && (pAmountOfDifferentPixels > pAmountOfIdenticalPixel))
			{
				m_pD2DDeviceContext1->FillEllipse(ellipse1, pRedBrush);
				m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
				m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
			};

			m_pD2DDeviceContext1->DrawTextLayout(
				centreLeft,
				pTextLayoutInformation0,
				pBlackBrush,
				D2D1_DRAW_TEXT_OPTIONS_NONE);


			m_pD2DDeviceContext1->DrawTextLayout(
				centre,
				pTextLayoutInformation1,
				pBlackBrush,
				D2D1_DRAW_TEXT_OPTIONS_NONE);


			m_pD2DDeviceContext1->DrawTextLayout(
				centreRight,
				pTextLayoutInformation2,
				pBlackBrush,
				D2D1_DRAW_TEXT_OPTIONS_NONE);




			D2D1_SIZE_F renderTargetSize = m_pD2DDeviceContext1->GetSize();

			m_pD2DDeviceContext1->EndDraw();


			// Retrieve D2D Device.



			if (hr == D2DERR_RECREATE_TARGET)
			{
				hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
			}






			pSwapChainObjectD2D->Present(0, 0);

			EndPaint(hWnd, &ps);


		}
		return SUCCEEDED(hr) ? 0 : 1;
	}





HRESULT Direct2DManagementClass::DrawRoadSignImageComparison(HINSTANCE hInstance, HWND hWnd, UINT pAmountOfIdenticalPixel, UINT pAmountOfDifferentPixels)
{
	HRESULT hr = S_OK;
	ClearRoadSignImageComparison(hInstance, hWnd);
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;

	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
	D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
	int clippedAreaWidth = clippedArea.right - clippedArea.left;
	int clippedAreaHeight = clippedArea.bottom - clippedArea.top;

	ID2D1SolidColorBrush* pGreenBrush = nullptr;
	ID2D1SolidColorBrush* pRedBrush = nullptr;
	ID2D1SolidColorBrush* pYellowBrush = nullptr;
	ID2D1SolidColorBrush* pBlackBrush = nullptr;
	ID2D1SolidColorBrush* pWhiteBrush = nullptr;

	vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();
	static const WCHAR pText0[] = L"Identical";
	static const WCHAR pText1[] = L"Mixed";
	static const WCHAR pText2[] = L"Different";
	static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
	static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
	static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;


	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);


	HDC hDC = BeginPaint(hWnd, &ps);
	if (hDC)
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}



		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &pYellowBrush);



		D2D1_POINT_2F centre = { pWidth / 2.0f, pHeight / 2.0f };
		D2D1_POINT_2F centreLeft = { pWidth / 2.0f - 320, pHeight / 2.0f };
		D2D1_POINT_2F centreRight = { pWidth / 2.0f + 320, pHeight / 2.0f };

		D2D1_POINT_2F centre0 = { pWidth / 2.0f, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreLeft0 = { pWidth / 2.0f - 320, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreRight0 = { pWidth / 2.0f + 320, pHeight / 2.0f - 20 };

		D2D1_ELLIPSE ellipse = { centre, 150, 150 };
		D2D1_ELLIPSE ellipse0 = { centreLeft, 150, 150 };
		D2D1_ELLIPSE ellipse1 = { centreRight, 150, 150 };

		pEllipseArray.push_back(ellipse);
		pEllipseArray.push_back(ellipse0);
		pEllipseArray.push_back(ellipse1);
		m_pD2DDeviceContext1->BeginDraw();

		// draw the ellipses
		m_pD2DDeviceContext1->DrawEllipse(ellipse, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse0, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse1, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);

		IDWriteTextFormat* pTextFormat = nullptr;
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			DWRITE_FONT_STYLE_ITALIC,
			DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
			24,
			L"",
			&pTextFormat);

		std::cout << "Setting Text Alignment Object" << endl;
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);


		std::cout << "Setting Text Layout Information 0" << endl;
		IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText0,
			stringLength0,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation0
			);

		std::cout << "Setting Text Layout Information 0" << endl;
		pTextLayoutInformation0->SetMaxHeight(40);
		pTextLayoutInformation0->SetMaxWidth(40);
		pTextLayoutInformation0->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation0->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		IDWriteTextLayout* pTextLayoutInformation1 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText1,
			stringLength1,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation1
			);
		pTextLayoutInformation1->SetMaxHeight(40);
		pTextLayoutInformation1->SetMaxWidth(40);
		pTextLayoutInformation1->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation1->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		std::cout << "Setting Text Layout Information 1" << endl;
		IDWriteTextLayout* pTextLayoutInformation2 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText2,
			stringLength2,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation2
			);
		pTextLayoutInformation2->SetMaxHeight(40);
		pTextLayoutInformation2->SetMaxWidth(40);
		pTextLayoutInformation2->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation2->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);






		std::cout << "Filling The Relevant Circle With Color" << endl;

		if ((pAmountOfIdenticalPixel > 0) && (pAmountOfDifferentPixels < pAmountOfIdenticalPixel))
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pGreenBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);
		};

		if ((pAmountOfIdenticalPixel > 0) && (pAmountOfDifferentPixels == pAmountOfIdenticalPixel))
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse, pYellowBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);
		}

		if ((pAmountOfDifferentPixels > 0) && (pAmountOfDifferentPixels > pAmountOfIdenticalPixel))
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pRedBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
		};

		m_pD2DDeviceContext1->DrawTextLayout(
			centreLeft,
			pTextLayoutInformation0,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);


		m_pD2DDeviceContext1->DrawTextLayout(
			centre,
			pTextLayoutInformation1,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);


		m_pD2DDeviceContext1->DrawTextLayout(
			centreRight,
			pTextLayoutInformation2,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);




		D2D1_SIZE_F renderTargetSize = m_pD2DDeviceContext1->GetSize();

		m_pD2DDeviceContext1->EndDraw();


		// Retrieve D2D Device.



		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}






		pSwapChainObjectD2D->Present(0, 0);

		EndPaint(hWnd, &ps);


	}
	return SUCCEEDED(hr) ? 0 : 1;
}






HRESULT Direct2DManagementClass::ClipImageAgainstABitmapMask(HINSTANCE hInstance,
	HWND hWnd, LPWSTR szFileNameOne,
	LPWSTR szFileNameTwo,
	float pXPosition,
	float pYPosition,
	float pWidth,
	float pHeight,
	vector<PixelObject> *pPixelObjectVector)
{
	HRESULT hr = S_OK;

	ID2D1Bitmap* pOriginalBitmapObject = nullptr;
	ID2D1Bitmap* pBitmapMaskObject = nullptr;
	ID2D1BitmapBrush* pBitmapBrush = nullptr;

	std::cout << "Loading Image One for A Clipping Image" << endl;

	vector<PixelObject> pPixelObjectArrayOne = vector<PixelObject>();
	hr = CreateBitmapDrawingClipObjectUsingClipperWithFileNamesForMasking(hInstance, hWnd, szFileNameOne, pXPosition, pYPosition,  pWidth, pHeight, &pPixelObjectArrayOne, &pOriginalBitmapObject);

	fstream pixelDataFile;
	pixelDataFile.open("C:\\pixels\\ImageClipperUsingMasks.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);

	ClearRenderTargetContent(hInstance, hWnd);

	ID2D1Bitmap* pOriginalBitmapMask = nullptr;

	std::cout << "Loading Image One for A Clipping Image" << endl;

	vector<PixelObject> pPixelObjectArrayTwo = vector<PixelObject>();
	hr = CreateBitmapDrawingClipObjectUsingClipperWithFileNamesForMasking(hInstance, hWnd, szFileNameTwo, pXPosition, pYPosition, pWidth, pHeight, &pPixelObjectArrayTwo, &pBitmapMaskObject);



	std::cout << "Creating Bitmap Brush Object" << endl;

	if (SUCCEEDED(hr))
	{
		D2D1_BITMAP_BRUSH_PROPERTIES bitmapMapProperties = D2D1::BitmapBrushProperties(D2D1_EXTEND_MODE_WRAP,
			D2D1_EXTEND_MODE_WRAP,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);

		hr = m_pD2DDeviceContext1->CreateBitmapBrush(pOriginalBitmapObject, &pBitmapBrush);

		if (FAILED(hr))
		{
			int pLastError = GetLastError();
			std::cout << "Bitmap Brush Creation TERMINATED WITH ERROR: " << pLastError << endl;
			return hr;
		}
	}

	

	if (BeginPaint(hWnd, &ps))
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}

		RECT outputRect = { pXPosition, pYPosition, pWidth, pHeight };
		
		D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);


		LPCSTR pFileName = nullptr;


		m_pD2DDeviceContext1->BeginDraw();

	
		m_pD2DDeviceContext1->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

		m_pD2DDeviceContext1->FillRectangle(&clippedArea, pBitmapBrush);

		m_pD2DDeviceContext1->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);


		hr = m_pD2DDeviceContext1->EndDraw();


		if (hr == D2DERR_RECREATE_TARGET)
		{
			SafeReleaseA(&m_pD2DBitmap);
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}
		pSwapChainObjectD2D->Present(0, 0);



		EndPaint(hWnd, &ps);
	}
	
	LPWSTR pScreenImage = L"E:\\RobotData\\ImageScreenFile.jpg";
	CaptureScreenShotToFile(hInstance, hWnd, pScreenImage);

	vector<PixelObject> pPixelObjectArrayOutput = vector<PixelObject>();

	hr = CreateBitmapDrawingClipObjectUsingClipperWithFileNames(hInstance, hWnd, pScreenImage,
		pXPosition, pYPosition, pWidth, pHeight, &pPixelObjectArrayOutput);
	

	std::cout << "The size of pixel buffer = " << pPixelObjectArrayOutput.size() << endl;
	int pAmountOfPixelsLeft = 0;
	pAmountOfPixelsLeft = pPixelObjectArrayOutput.size();

	pixelDataFile.open("C:\\pixels\\ImageClipperTestsMasks.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);

	pixelDataFile.clear();
	pixelDataFile << " " << endl;
	pixelDataFile << " " << endl;
	pixelDataFile << "New Operation Begin" << endl;
	time_t currentTime = time(0);
	pixelDataFile << "current time" << currentTime << endl;
	for (PixelObject currentPixel : pPixelObjectArrayOutput)
	{



		std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
			"X Position = " << currentPixel.xPosition << " " <<
			"Y Position = " << currentPixel.yPosition << " " <<
			"The Value Of Red = " << currentPixel.RMask << " "
			<< "The Value Of Green = " << currentPixel.GMask << " " <<
			"The Value Of Blue = " << currentPixel.BMask << endl;

		pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
		pixelDataFile << "X Position = " << pXPosition << endl;
		pixelDataFile << "Y Position = " << pYPosition << endl;
		pixelDataFile << "The Value Of Red = " << currentPixel.RMask << endl;
		pixelDataFile << "The Value Of Green = " << currentPixel.GMask << endl;
		pixelDataFile << "The Value Of Blue = " << currentPixel.BMask << endl;

		pAmountOfPixelsLeft--;

	}










	pixelDataFile << "New Operation End" << endl;
	pixelDataFile << " " << endl;
	pixelDataFile << " " << endl;
	pixelDataFile.close();
	std::cout << "Pixels END" << endl;




	
	
	*pPixelObjectVector = pPixelObjectArrayOutput;
	
	return SUCCEEDED(hr) ? 0 : 1;
	
}







HRESULT Direct2DManagementClass::CreateBitmapDrawingClipObjectUsingClipper(HINSTANCE hInstance, HWND hWnd, float pXPosition, float pYPosition, float pWidth, float pHeight,
	vector<PixelObject> *ppPixelObject)
{
	ofstream myfile("imagebuffer.txt");
	myfile << "Please write this text to a file.\n this text is written using C++\n";
	IWICBitmapSource* pSource = nullptr;
	IWICBitmap* pBitmapObject = nullptr;
	IWICBitmapLock* pBitmapLock = nullptr;
	IWICBitmapLock* pBitmapLock1 = nullptr;
	IWICBitmapSource* pSourceObject = nullptr;
	IWICBitmapClipper *pIClipper = nullptr;
	BYTE* pixels = nullptr;
	HRESULT hr = S_OK;
	WCHAR szFileName[MAX_PATH];
	WICRect rcLock = { pXPosition, pYPosition, pWidth, pHeight };
	ID2D1Image* outputImage = nullptr;
	vector <PixelObject> pixelDataVector = vector <PixelObject>();
	HDC hDC;
	HDC src;
	WICPixelFormatGUID pixelFormat;
	IWICBitmapSource* piConverter = nullptr;
	UINT pImageHeight = NULL;
	UINT pImageWidth = NULL;
	BYTE* pixelArrayPointer = nullptr;
	D2D1_RECT_F rectangle = D2D1::RectF(pXPosition, pYPosition, pWidth, pHeight);
	UINT cbBufferSize = 0;
	BYTE* pixelDataArray = NULL;
	hInstance = GetModuleHandle(NULL);
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapDecoder* pImageDecoder = nullptr;
	IWICBitmapFrameDecode *pFrame = NULL;
	UINT uiWidth = 0;
	UINT uiHeight = 0;
	WICRect rcClip = { pXPosition, pYPosition, pWidth, pHeight };

	if (LocateImageFile(hWnd, szFileName, ARRAYSIZE(szFileName)))
	{

		// Prepare iterator pairs to iterate the file content!



		hr = m_pWICFactory->CreateDecoderFromFilename(
			szFileName,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&pImageDecoder);




		if (SUCCEEDED(hr))
		{
			hr = pImageDecoder->GetFrame(0, &pFrame);
		}

		IWICBitmapSource* pSourceObject = pFrame;

		if (SUCCEEDED(hr))
		{
			hr = m_pWICFactory->CreateBitmapClipper(&pIClipper);
		}

		WICRect rcClip = { pXPosition, pYPosition, pWidth, pHeight };

		// Initialize the clipper with the given rectangle of the frame's image data.
		if (SUCCEEDED(hr))
		{
			hr = pIClipper->Initialize(pFrame, &rcClip);
		}

		

		
		if (SUCCEEDED(hr))
		{
			
			hr = m_pWICFactory->CreateBitmapFromSource(pIClipper,
				WICBitmapCacheOnDemand,
				&pBitmapObject);

				IWICPalette* pImagePalette = nullptr;
				pBitmapObject->CopyPalette(pImagePalette);
		}






		if (SUCCEEDED(hr))
		{
			SafeReleaseA(&m_pConvertedSourceBitmap);
			hr = m_pWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pConvertedSourceBitmap->Initialize(
				pBitmapObject,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeCustom);

		}
		if (SUCCEEDED(hr))
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}
		if (SUCCEEDED(hr))
		{

			SafeReleaseA(&m_pD2DBitmap);
			hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);

		}



		if (SUCCEEDED(hr))
		{
			D2D1_SIZE_F pSize = m_pD2DBitmap->GetSize();
			pImageWidth = pSize.width;
			pImageHeight = pSize.height;

			hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidColorBrush);
		}

		if (SUCCEEDED(hr))
		{


			hr = DrawD2DContentAxisAlignedClip(hInstance, hWnd, m_pD2DBitmap, rectangle);

		}


		IWICBitmapSource* pConverter = nullptr;
		WICPixelFormatGUID pixelFormat;

		if (SUCCEEDED(hr))
		{

			pSourceObject = pFrame;
			pSourceObject->AddRef();
			pSourceObject->GetSize(&uiWidth, &uiHeight);
		}

		IWICPixelFormatInfo* pixelFormatInfo = nullptr;




		if (SUCCEEDED(hr))
		{
			pSourceObject->GetPixelFormat(&pixelFormat);
		}



		if (!IsEqualGUID(pixelFormat, GUID_WICPixelFormat128bppPRGBAFloat))
		{

			hr = WICConvertBitmapSource(GUID_WICPixelFormat128bppPRGBAFloat, pSourceObject, &pConverter);
			IWICBitmapLock* pBitmapLock = nullptr;


			if (SUCCEEDED(hr))
			{
				pSourceObject->Release();     // the converter has a reference to the source
				pSourceObject = NULL;         // so we don't need it anymore.
				pSourceObject = pConverter;   // let's treat the 128bppPABGR converter as the source
			}


			IWICPalette* pImagePalette = nullptr;
			pSourceObject->CopyPalette(pImagePalette);
		}



		CImageFileData* pImageClass = new CImageFileData((char*)pixelArrayPointer);



		if (pConverter)
		{
			UINT cbStride = pWidth * sizeof(float) * 4;
			UINT cbBufferSize = cbStride;

			float *pixels = new float[cbBufferSize / sizeof(float)];

		}
	}


	UINT bSize = 0;




	UINT rowPitch1 = 4 * pWidth;
	UINT stride = 0; // stride  = bytes per row					 // (width * (bpp/8)) + padding and round to the nearest DWORD
	stride = (((((UINT)pWidth * 32) + 31) & ~31) >> 3);
	bSize = pHeight * pWidth;
	BYTE *pixelArray = new BYTE[(UINT)pHeight * stride];


	pIClipper->CopyPixels(NULL, stride, bSize, pixelArray);
	

	hDC = GetDC(hWnd);

	HBITMAP map = CreateBitmap((UINT)pWidth,// width. 512 in my case
		(UINT)pHeight, // height
		1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
		8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
		(void*)pixelArray); // pointer to array
	// Temp HDC to copy picture
	src = CreateCompatibleDC(hDC); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
	// Copy image from temp HDC to window
	// Рисуем изображение bitmap
	BitBlt(hDC, pXPosition, pYPosition, pWidth, pHeight,
		src, pXPosition, pYPosition, SRCCOPY);




	std::cout << "Pixels Begin" << endl;

	int pixelCounter = 0;


	//	pBitmapObj->GetResolution(&pWidth, &pHeight);

	UINT pYVal = 0;
	UINT pXVal = 0;
	for (pXVal = pXPosition; pXVal < pXPosition + pWidth; ++pXVal)
	{
		for (pYVal = pYPosition; pYVal < pYPosition + pHeight; ++pYVal)
			// Your code here
		{
			std::cout << "Current X Value: " << pXVal << " " << "Current Y Value: " << pYVal << endl;
			COLORREF pCurrentPixel = GetPixel(hDC, pXVal, pYVal);
			int pRedValue = GetRValue(pCurrentPixel);
			int pGreenValue = GetGValue(pCurrentPixel);
			int pBlueValue = GetBValue(pCurrentPixel);
			PixelObject pixelObj = PixelObject(pixelCounter, pXVal, pYVal, pRedValue, pGreenValue, pBlueValue);
			pixelDataVector.push_back(pixelObj);
			std::cout << "Amount of total pixels processed  = " << pixelCounter << endl;
			pixelCounter = pixelCounter + 1;

		}
	}

	std::cout << "The size of pixel buffer = " << pixelDataVector.size() << endl;
	int pAmountOfPixelsLeft = 0;
	pAmountOfPixelsLeft = pixelDataVector.size();


	fstream pixelDataFile;
	pixelDataFile.open("C:\\pixels\\ImageClipperTests.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);

	pixelDataFile.clear();
	pixelDataFile << " " << endl;
	pixelDataFile << " " << endl;
	pixelDataFile << "New Operation Begin" << endl;
	time_t currentTime = time(0);
	pixelDataFile << "current time" << currentTime << endl;
	for (PixelObject currentPixel : pixelDataVector)
	{



		std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
			"X Position = " << currentPixel.xPosition << " " <<
			"Y Position = " << currentPixel.yPosition << " " <<
			"The Value Of Red = " << currentPixel.RMask << " "
			<< "The Value Of Green = " << currentPixel.GMask << " " <<
			"The Value Of Blue = " << currentPixel.BMask << endl;

		pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
		pixelDataFile << "X Position = " << pXPosition << endl;
		pixelDataFile << "Y Position = " << pYPosition << endl;
		pixelDataFile << "The Value Of Red = " << currentPixel.RMask << endl;
		pixelDataFile << "The Value Of Green = " << currentPixel.GMask << endl;
		pixelDataFile << "The Value Of Blue = " << currentPixel.BMask << endl;
		pXPosition++;
		pYPosition++;
		pAmountOfPixelsLeft--;

	}
	pixelDataFile << "New Operation End" << endl;
	pixelDataFile << " " << endl;
	pixelDataFile << " " << endl;
	pixelDataFile.close();
	std::cout << "Pixels END" << endl;












	SafeReleaseA(&pDecoder);
	SafeReleaseA(&pFrame);
	ReleaseDC(hWnd, src);
	ReleaseDC(hWnd, hDC);
	DeleteDC(src);
	DeleteDC(hDC);
	myfile.close();
	*ppPixelObject = pixelDataVector;

	return hr;
}










HRESULT Direct2DManagementClass::CreatePieChartDiagram(HWND hWnd)
{
	HRESULT hr = S_OK;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	PAINTSTRUCT ps = {};
	RECT rc;
	GetClientRect(hWnd, &rc);
	// Create the DC render target.
	hr = CreateDeviceResources(hInstance, hWnd);

	if (SUCCEEDED(hr))
	{
		ID2D1DCRenderTarget* m_pD2DDCRenderTarget = nullptr;
		// Bind the DC to the DC render target.
	


	
		m_pD2DDeviceContext->BeginDraw();

		m_pD2DDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());

		m_pD2DDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::White));

		m_pD2DDeviceContext->DrawEllipse(
			D2D1::Ellipse(
			D2D1::Point2F(150.0f, 150.0f),
			100.0f,
			100.0f),
			m_pBlackBrush,
			3.0
			);

		m_pD2DDeviceContext->DrawLine(
			D2D1::Point2F(150.0f, 150.0f),
			D2D1::Point2F(
			(150.0f + 100.0f * 0.15425f),
			(150.0f - 100.0f * 0.988f)),
			m_pBlackBrush,
			3.0
			);

		m_pD2DDeviceContext->DrawLine(
			D2D1::Point2F(150.0f, 150.0f),
			D2D1::Point2F(
			(150.0f + 100.0f * 0.525f),
			(150.0f + 100.0f * 0.8509f)),
			m_pBlackBrush,
			3.0
			);

		m_pD2DDeviceContext->DrawLine(
			D2D1::Point2F(150.0f, 150.0f),
			D2D1::Point2F(
			(150.0f - 100.0f * 0.988f),
			(150.0f - 100.0f * 0.15425f)),
			m_pBlackBrush,
			3.0
			);

		hr = m_pD2DDeviceContext->EndDraw();
		if (SUCCEEDED(hr))
		{
			//
			// Draw the pie chart with GDI.
			//

			// Save the original object.
			HGDIOBJ original = NULL;
			original = SelectObject(
				ps.hdc,
				GetStockObject(DC_PEN)
				);

			HPEN blackPen = CreatePen(PS_SOLID, 3, 0);
			SelectObject(ps.hdc, blackPen);

			Ellipse(ps.hdc, 300, 50, 500, 250);

			POINT pntArray1[2];
			pntArray1[0].x = 400;
			pntArray1[0].y = 150;
			pntArray1[1].x = static_cast<LONG>(400 + 100 * 0.15425);
			pntArray1[1].y = static_cast<LONG>(150 - 100 * 0.9885);

			POINT pntArray2[2];
			pntArray2[0].x = 400;
			pntArray2[0].y = 150;
			pntArray2[1].x = static_cast<LONG>(400 + 100 * 0.525);
			pntArray2[1].y = static_cast<LONG>(150 + 100 * 0.8509);


			POINT pntArray3[2];
			pntArray3[0].x = 400;
			pntArray3[0].y = 150;
			pntArray3[1].x = static_cast<LONG>(400 - 100 * 0.988);
			pntArray3[1].y = static_cast<LONG>(150 - 100 * 0.15425);

			Polyline(ps.hdc, pntArray1, 2);
			Polyline(ps.hdc, pntArray2, 2);
			Polyline(ps.hdc, pntArray3, 2);

			DeleteObject(blackPen);

			// Restore the original object.
			SelectObject(ps.hdc, original);
		}
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}

	return hr;
}


void Direct2DManagementClass::OnResize(D2D1_SIZE_U size)
{
	if (m_pD2DDeviceContext1)
	{
		return;
	}
}


void Direct2DManagementClass::SetFullscreenMode(HWND hWnd)
{
	
		MONITORINFO mi = { sizeof(mi) };
		static WINDOWPLACEMENT g_wpPrev = { sizeof(g_wpPrev) };
		DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	
		if (dwStyle & WS_OVERLAPPEDWINDOW) 
		{

			
			
			if (GetWindowPlacement(hWnd, &g_wpPrev) &&
				GetMonitorInfo(MonitorFromWindow(hWnd,
				MONITOR_DEFAULTTOPRIMARY), &mi))
			{
					
					LONG monitorWidth = mi.rcMonitor.right - mi.rcMonitor.left;
					LONG monitorHeight = mi.rcMonitor.bottom - mi.rcMonitor.top;

					LONG coeffMonitor = monitorWidth / monitorHeight;

					LONG newMonitorWidth = monitorWidth * coeffMonitor / 16;
					LONG newMonitorHeight = monitorHeight * coeffMonitor / 16;

					SetWindowLong(hWnd, GWL_STYLE,
						dwStyle & ~WS_OVERLAPPEDWINDOW);
					SetWindowPos(hWnd, HWND_TOP,
						mi.rcMonitor.left, mi.rcMonitor.top,
						newMonitorWidth,
						newMonitorHeight,
						SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
			}
		} 
}


HRESULT Direct2DManagementClass::DrawRoadSign(HINSTANCE hInstance, HWND hWnd, UINT pLevelOfIntensity)
{
	HRESULT hr = S_OK;
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;

	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
	D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
	int clippedAreaWidth = clippedArea.right - clippedArea.left;
	int clippedAreaHeight = clippedArea.bottom - clippedArea.top;

	ID2D1SolidColorBrush* pGreenBrush = nullptr;
	ID2D1SolidColorBrush* pRedBrush = nullptr;
	ID2D1SolidColorBrush* pYellowBrush = nullptr;
	ID2D1SolidColorBrush* pBlackBrush = nullptr;
	ID2D1SolidColorBrush* pWhiteBrush = nullptr;

	vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();



	static const WCHAR pText0[] = L"33";
	static const WCHAR pText1[] = L"66";
	static const WCHAR pText2[] = L"100";



	static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
	static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
	static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;


	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);
	

	HDC hDC = BeginPaint(hWnd, &ps);
	if (hDC)
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}



		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &pYellowBrush);
	


		D2D1_POINT_2F centre = { pWidth / 2.0f, pHeight / 2.0f };
		D2D1_POINT_2F centreLeft = { pWidth / 2.0f - 320, pHeight / 2.0f };
		D2D1_POINT_2F centreRight = { pWidth / 2.0f + 320, pHeight / 2.0f };

		D2D1_POINT_2F centre0 = { pWidth / 2.0f, pHeight / 2.0f - 20};
		D2D1_POINT_2F centreLeft0 = { pWidth / 2.0f - 320, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreRight0 = { pWidth / 2.0f + 320, pHeight / 2.0f - 20 };

		D2D1_ELLIPSE ellipse = { centre, 130, 130 };
		D2D1_ELLIPSE ellipse0 = { centreLeft, 130, 130 };
		D2D1_ELLIPSE ellipse1 = { centreRight, 130, 130 };
		pEllipseArray.push_back(ellipse);
		pEllipseArray.push_back(ellipse0);
		pEllipseArray.push_back(ellipse1);
		m_pD2DDeviceContext1->BeginDraw();
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));


		// draw the ellipses
		m_pD2DDeviceContext1->DrawEllipse(ellipse, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse0, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse1, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);
		
		IDWriteTextFormat* pTextFormat = nullptr;
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			DWRITE_FONT_STYLE_ITALIC,
			DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
			24,
			L"",
			&pTextFormat);
		
		std::cout << "Setting Text Alignment Object" << endl;
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		std::cout << "Setting Text Layout Information 0" << endl;
		IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText0,
			stringLength0,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation0
			);

		std::cout << "Setting Text Layout Information 0" << endl;
		pTextLayoutInformation0->SetMaxHeight(40);
		pTextLayoutInformation0->SetMaxWidth(40);
		pTextLayoutInformation0->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation0->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		IDWriteTextLayout* pTextLayoutInformation1 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText1,
			stringLength1,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation1
			);
		pTextLayoutInformation1->SetMaxHeight(40);
		pTextLayoutInformation1->SetMaxWidth(40);
		pTextLayoutInformation1->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation1->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		std::cout << "Setting Text Layout Information 1" << endl;
		IDWriteTextLayout* pTextLayoutInformation2 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText2,
			stringLength2,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation2
			);
		pTextLayoutInformation2->SetMaxHeight(40);
		pTextLayoutInformation2->SetMaxWidth(40);
		pTextLayoutInformation2->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation2->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);


		
		std::cout << "Filling The Relevant Circle With Color" << endl;

		if (pLevelOfIntensity < 33)
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pGreenBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse, pBlackBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pBlackBrush);


			m_pD2DDeviceContext1->DrawTextLayout(
				centre,
				pTextLayoutInformation1,
				pWhiteBrush,
				D2D1_DRAW_TEXT_OPTIONS_NONE);


		}

		else if (pLevelOfIntensity >= 33 && pLevelOfIntensity <= 66)
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse, pYellowBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pBlackBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pBlackBrush);

			m_pD2DDeviceContext1->DrawTextLayout(
				centreRight,
				pTextLayoutInformation2,
				pWhiteBrush,
				D2D1_DRAW_TEXT_OPTIONS_NONE);



		}
		
		else if (pLevelOfIntensity > 66 && pLevelOfIntensity <= 100)
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pRedBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pBlackBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse, pBlackBrush);


			m_pD2DDeviceContext1->DrawTextLayout(
				centreLeft,
				pTextLayoutInformation0,
				pWhiteBrush,
				D2D1_DRAW_TEXT_OPTIONS_NONE);


		}

		D2D1_RECT_F rectangle0 = D2D1::RectF(centreLeft.x, centreLeft.y, 10, 10);
		D2D1_RECT_F rectangle = D2D1::RectF(centre.x, centre.y, 10, 10);
		D2D1_RECT_F rectangle1 = D2D1::RectF(centreRight.x, centreRight.y, 10, 10);
	
	
	

		D2D1_SIZE_F renderTargetSize = m_pD2DDeviceContext1->GetSize();
	
		m_pD2DDeviceContext1->EndDraw();

	
		// Retrieve D2D Device.



		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}






		pSwapChainObjectD2D->Present(0, 0);
		EndPaint(hWnd, &ps);


	}
	return SUCCEEDED(hr) ? 0 : 1;
}



HRESULT Direct2DManagementClass::DrawRoadSignIntensityDifference(HINSTANCE hInstance, HWND hWnd, float pIntensityDifference)
{
	HRESULT hr = S_OK;
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;
	ClearRoadSignIntensityDifference(hInstance, hWnd);

	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
	D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
	int clippedAreaWidth = clippedArea.right - clippedArea.left;
	int clippedAreaHeight = clippedArea.bottom - clippedArea.top;

	ID2D1SolidColorBrush* pGreenBrush = nullptr;
	ID2D1SolidColorBrush* pRedBrush = nullptr;
	ID2D1SolidColorBrush* pYellowBrush = nullptr;
	ID2D1SolidColorBrush* pBlackBrush = nullptr;
	ID2D1SolidColorBrush* pWhiteBrush = nullptr;

	vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();



	static const WCHAR pText0[] = L"Difference <= 85";
	static const WCHAR pText1[] = L"Difference > 85 && < pDifference <= 170";
	static const WCHAR pText2[] = L"Difference > 170 && < pDifference <= 255";



	static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
	static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
	static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;


	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);


	HDC hDC = BeginPaint(hWnd, &ps);
	if (hDC)
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}



		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &pYellowBrush);



		D2D1_POINT_2F centre = { pWidth / 2.0f, pHeight / 2.0f };
		D2D1_POINT_2F centreLeft = { pWidth / 2.0f - 320, pHeight / 2.0f };
		D2D1_POINT_2F centreRight = { pWidth / 2.0f + 320, pHeight / 2.0f };

		D2D1_POINT_2F centre0 = { pWidth / 2.0f, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreLeft0 = { pWidth / 2.0f - 320, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreRight0 = { pWidth / 2.0f + 320, pHeight / 2.0f - 20 };

		D2D1_ELLIPSE ellipse = { centre, 130, 130 };
		D2D1_ELLIPSE ellipse0 = { centreLeft, 130, 130 };
		D2D1_ELLIPSE ellipse1 = { centreRight, 130, 130 };
		pEllipseArray.push_back(ellipse);
		pEllipseArray.push_back(ellipse0);
		pEllipseArray.push_back(ellipse1);
		m_pD2DDeviceContext1->BeginDraw();
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));


		// draw the ellipses
		m_pD2DDeviceContext1->DrawEllipse(ellipse, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse0, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse1, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);

		IDWriteTextFormat* pTextFormat = nullptr;
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			DWRITE_FONT_STYLE_ITALIC,
			DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
			24,
			L"",
			&pTextFormat);

		std::cout << "Setting Text Alignment Object" << endl;
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		std::cout << "Setting Text Layout Information 0" << endl;
		IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText0,
			stringLength0,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation0
			);

		std::cout << "Setting Text Layout Information 0" << endl;
		pTextLayoutInformation0->SetMaxHeight(40);
		pTextLayoutInformation0->SetMaxWidth(40);
		pTextLayoutInformation0->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation0->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		IDWriteTextLayout* pTextLayoutInformation1 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText1,
			stringLength1,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation1
			);
		pTextLayoutInformation1->SetMaxHeight(40);
		pTextLayoutInformation1->SetMaxWidth(40);
		pTextLayoutInformation1->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation1->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		std::cout << "Setting Text Layout Information 1" << endl;
		IDWriteTextLayout* pTextLayoutInformation2 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText2,
			stringLength2,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation2
			);
		pTextLayoutInformation2->SetMaxHeight(40);
		pTextLayoutInformation2->SetMaxWidth(40);
		pTextLayoutInformation2->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation2->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);



		std::cout << "Filling The Relevant Circle With Color" << endl;

		if (pIntensityDifference <= 85.0f)
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pGreenBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);


			m_pD2DDeviceContext1->DrawTextLayout(
				centre,
				pTextLayoutInformation1,
				pBlackBrush,
				D2D1_DRAW_TEXT_OPTIONS_NONE);


		}

		else if ((pIntensityDifference > 85.0f) && (pIntensityDifference <= 170.0f))
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse, pYellowBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);

			m_pD2DDeviceContext1->DrawTextLayout(
				centreRight,
				pTextLayoutInformation2,
				pBlackBrush,
				D2D1_DRAW_TEXT_OPTIONS_NONE);



		}

		else if ((pIntensityDifference > 170.0f) && (pIntensityDifference <= 255.0f))
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pRedBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);


			m_pD2DDeviceContext1->DrawTextLayout(
				centreLeft,
				pTextLayoutInformation0,
				pBlackBrush,
				D2D1_DRAW_TEXT_OPTIONS_NONE);


		}

		D2D1_RECT_F rectangle0 = D2D1::RectF(centreLeft.x, centreLeft.y, 10, 10);
		D2D1_RECT_F rectangle = D2D1::RectF(centre.x, centre.y, 10, 10);
		D2D1_RECT_F rectangle1 = D2D1::RectF(centreRight.x, centreRight.y, 10, 10);




		D2D1_SIZE_F renderTargetSize = m_pD2DDeviceContext1->GetSize();

		m_pD2DDeviceContext1->EndDraw();


		// Retrieve D2D Device.



		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}






		pSwapChainObjectD2D->Present(0, 0);
		EndPaint(hWnd, &ps);


	}
	return SUCCEEDED(hr) ? 0 : 1;
}


HRESULT Direct2DManagementClass::DrawRoadSignMotionDetected(HINSTANCE hInstance, HWND hWnd, BOOL
	pIsMotionUndefined)
{
	HRESULT hr = S_OK;
	ClearRoadSignMotionDetected(hInstance, hWnd);
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;
	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
	D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.
		bottom);
	int clippedAreaWidth = clippedArea.right - clippedArea.left;
	int clippedAreaHeight = clippedArea.bottom - clippedArea.top;
	ID2D1SolidColorBrush* pGreenBrush = nullptr;
	ID2D1SolidColorBrush* pRedBrush = nullptr;
	ID2D1SolidColorBrush* pYellowBrush = nullptr;
	ID2D1SolidColorBrush* pBlackBrush = nullptr;
	ID2D1SolidColorBrush* pWhiteBrush = nullptr;
	vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();
	static const WCHAR pText0[] = L"Motion not Detected";
	static const WCHAR pText1[] = L"Motion Undefines = ";
	static const WCHAR pText2[] = L"Motion Detected Between Image";
	static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
	static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
	static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;
	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);
	ID2D1Bitmap1* pBitmapToSaveData = nullptr;
	HDC hDC = BeginPaint(hWnd, &ps);
	if (hDC)
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}

		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &pYellowBrush);
		D2D1_POINT_2F centre = { pWidth / 2.0f, pHeight / 2.0f };
		D2D1_POINT_2F centreLeft = { pWidth / 2.0f - 320, pHeight / 2.0f };
		D2D1_POINT_2F centreRight = { pWidth / 2.0f + 320, pHeight / 2.0f };
		D2D1_POINT_2F centre0 = { pWidth / 2.0f, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreLeft0 = { pWidth / 2.0f - 320, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreRight0 = { pWidth / 2.0f + 320, pHeight / 2.0f - 20 };
		D2D1_ELLIPSE ellipse = { centre, 130, 130 };
		D2D1_ELLIPSE ellipse0 = { centreLeft, 130, 130 };
		D2D1_ELLIPSE ellipse1 = { centreRight, 130, 130 };
		pEllipseArray.push_back(ellipse);
		pEllipseArray.push_back(ellipse0);
		pEllipseArray.push_back(ellipse1);
		m_pD2DDeviceContext1->BeginDraw();
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));
		// draw the ellipses
		m_pD2DDeviceContext1->DrawEllipse(ellipse, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse0, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse1, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);
		IDWriteTextFormat* pTextFormat = nullptr;
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			DWRITE_FONT_STYLE_ITALIC,
			DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
			12,
			L"",
			&pTextFormat);
		std::cout << "Setting Text Alignment Object" << endl;
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
		std::cout << "Setting Text Layout Information 0" << endl;
		IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText0,
			stringLength0,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation0
			);
		std::cout << "Setting Text Layout Information 0" << endl;
		pTextLayoutInformation0->SetMaxHeight(40);
		pTextLayoutInformation0->SetMaxWidth(40);
		pTextLayoutInformation0->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation0->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
		IDWriteTextLayout* pTextLayoutInformation1 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText1,
			stringLength1,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation1
			);
		pTextLayoutInformation1->SetMaxHeight(40);
		pTextLayoutInformation1->SetMaxWidth(40);
		pTextLayoutInformation1->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation1->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
		std::cout << "Setting Text Layout Information 1" << endl;
		IDWriteTextLayout* pTextLayoutInformation2 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText2,
			stringLength2,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation2
			);
		pTextLayoutInformation2->SetMaxHeight(40);
		pTextLayoutInformation2->SetMaxWidth(40);
		pTextLayoutInformation2->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation2->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
		std::cout << "Filling The Relevant Circle With Color" << endl;
		if (pIsMotionUndefined == false)
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pGreenBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);
		}
		else if (pIsMotionUndefined == true)
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pRedBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
		}

		D2D1_RECT_F rectangle0 = D2D1::RectF(centreLeft.x, centreLeft.y, 10, 10);
		D2D1_RECT_F rectangle = D2D1::RectF(centre.x, centre.y, 10, 10);
		D2D1_RECT_F rectangle1 = D2D1::RectF(centreRight.x, centreRight.y, 10, 10);

		m_pD2DDeviceContext1->DrawTextLayout(
			centreLeft,
			pTextLayoutInformation0,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);
		m_pD2DDeviceContext1->DrawTextLayout(
			centre,
			pTextLayoutInformation1,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);
		m_pD2DDeviceContext1->DrawTextLayout(
			centreRight,
			pTextLayoutInformation2,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);
		D2D1_SIZE_F renderTargetSize = m_pD2DDeviceContext1->GetSize();
		m_pD2DDeviceContext1->EndDraw();

		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}
		pSwapChainObjectD2D->Present(0, 0);
		EndPaint(hWnd, &ps);
	}
	return SUCCEEDED(hr) ? 0 : 1;
}

HRESULT Direct2DManagementClass::DrawRoadSignIntensityDifferenceValue(HINSTANCE hInstance, HWND hWnd, float pDifferenceValue)
{
	HRESULT hr = S_OK;
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;

	ClearRoadSignIntensityDifference(hInstance, hWnd);

	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
	D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
	int clippedAreaWidth = clippedArea.right - clippedArea.left;
	int clippedAreaHeight = clippedArea.bottom - clippedArea.top;

	ID2D1SolidColorBrush* pGreenBrush = nullptr;
	ID2D1SolidColorBrush* pRedBrush = nullptr;
	ID2D1SolidColorBrush* pYellowBrush = nullptr;
	ID2D1SolidColorBrush* pBlackBrush = nullptr;
	ID2D1SolidColorBrush* pWhiteBrush = nullptr;


	vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();
	static const WCHAR pText0[] = L"<= 85";
	static const WCHAR pText1[] = L" >= 85 && < 170 ";
	static const WCHAR pText2[] = L">= 170 && < 255";
	static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
	static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
	static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;


	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);


	HDC hDC = BeginPaint(hWnd, &ps);
	if (hDC)
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}



		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &pYellowBrush);



		D2D1_POINT_2F centre = { pWidth / 2.0f, pHeight / 2.0f };
		D2D1_POINT_2F centreLeft = { pWidth / 2.0f - 320, pHeight / 2.0f };
		D2D1_POINT_2F centreRight = { pWidth / 2.0f + 320, pHeight / 2.0f };

		D2D1_POINT_2F centre0 = { pWidth / 2.0f, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreLeft0 = { pWidth / 2.0f - 320, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreRight0 = { pWidth / 2.0f + 320, pHeight / 2.0f - 20 };

		D2D1_ELLIPSE ellipse = { centre, 150, 150 };
		D2D1_ELLIPSE ellipse0 = { centreLeft, 150, 150 };
		D2D1_ELLIPSE ellipse1 = { centreRight, 150, 150 };
		pEllipseArray.push_back(ellipse);
		pEllipseArray.push_back(ellipse0);
		pEllipseArray.push_back(ellipse1);
		m_pD2DDeviceContext1->BeginDraw();
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));


		// draw the ellipses
		m_pD2DDeviceContext1->DrawEllipse(ellipse, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse0, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse1, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);

		IDWriteTextFormat* pTextFormat = nullptr;
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			DWRITE_FONT_STYLE_ITALIC,
			DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
			24,
			L"",
			&pTextFormat);

		std::cout << "Setting Text Alignment Object" << endl;
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		std::cout << "Setting Text Layout Information 0" << endl;
		IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText0,
			stringLength0,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation0
			);

		std::cout << "Setting Text Layout Information 0" << endl;
		pTextLayoutInformation0->SetMaxHeight(40);
		pTextLayoutInformation0->SetMaxWidth(40);
		pTextLayoutInformation0->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation0->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		IDWriteTextLayout* pTextLayoutInformation1 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText1,
			stringLength1,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation1
			);
		pTextLayoutInformation1->SetMaxHeight(40);
		pTextLayoutInformation1->SetMaxWidth(40);
		pTextLayoutInformation1->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation1->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		std::cout << "Setting Text Layout Information 1" << endl;
		IDWriteTextLayout* pTextLayoutInformation2 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText2,
			stringLength2,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation2
			);
		pTextLayoutInformation2->SetMaxHeight(40);
		pTextLayoutInformation2->SetMaxWidth(40);
		pTextLayoutInformation2->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation2->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);



		std::cout << "Filling The Relevant Circle With Color" << endl;

		if (pDifferenceValue < 85)
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pGreenBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);
		}

		else if (pDifferenceValue >= 85 && pDifferenceValue <= 170)
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse, pYellowBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);
		}

		else if (pDifferenceValue > 170 && pDifferenceValue <= 255)
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pRedBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
		}

		D2D1_RECT_F rectangle0 = D2D1::RectF(centreLeft.x, centreLeft.y, 10, 10);
		D2D1_RECT_F rectangle = D2D1::RectF(centre.x, centre.y, 10, 10);
		D2D1_RECT_F rectangle1 = D2D1::RectF(centreRight.x, centreRight.y, 10, 10);

		m_pD2DDeviceContext1->DrawTextLayout(
			centreLeft,
			pTextLayoutInformation0,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);


		m_pD2DDeviceContext1->DrawTextLayout(
			centre,
			pTextLayoutInformation1,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);


		m_pD2DDeviceContext1->DrawTextLayout(
			centreRight,
			pTextLayoutInformation2,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);




		D2D1_SIZE_F renderTargetSize = m_pD2DDeviceContext1->GetSize();

		m_pD2DDeviceContext1->EndDraw();


		// Retrieve D2D Device.



		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}






		pSwapChainObjectD2D->Present(0, 0);
		EndPaint(hWnd, &ps);


	}
	return SUCCEEDED(hr) ? 0 : 1;
}





HRESULT Direct2DManagementClass::DrawRoadSignOddEven(HINSTANCE hInstance, HWND hWnd, UINT pInputNumner, UINT pLevelOfIntensity)
{
	HRESULT hr = S_OK;
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;
	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
	D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
	int clippedAreaWidth = clippedArea.right - clippedArea.left;
	int clippedAreaHeight = clippedArea.bottom - clippedArea.top;

	ID2D1SolidColorBrush* pGreenBrush = nullptr;
	ID2D1SolidColorBrush* pRedBrush = nullptr;
	ID2D1SolidColorBrush* pYellowBrush = nullptr;
	ID2D1SolidColorBrush* pBlackBrush = nullptr;
	ID2D1SolidColorBrush* pWhiteBrush = nullptr;


	stringstream ss;
	ss << pInputNumner << endl;
	string pStringVal = ss.str();
	vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();
	static const WCHAR pText0[] = L"Even Number";
	static const WCHAR pText1[] = L"Unknown Number = ";
	static const WCHAR pText2[] = L"Odd Number = ";
	static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
	static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
	static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;


	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);
	ID2D1Bitmap1* pBitmapToSaveData = nullptr;


	HDC hDC = BeginPaint(hWnd, &ps);
	if (hDC)
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}



		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &pYellowBrush);



		D2D1_POINT_2F centre = { pWidth / 2.0f, pHeight / 2.0f };
		D2D1_POINT_2F centreLeft = { pWidth / 2.0f - 320, pHeight / 2.0f };
		D2D1_POINT_2F centreRight = { pWidth / 2.0f + 320, pHeight / 2.0f };

		D2D1_POINT_2F centre0 = { pWidth / 2.0f, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreLeft0 = { pWidth / 2.0f - 320, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreRight0 = { pWidth / 2.0f + 320, pHeight / 2.0f - 20 };
	
		D2D1_ELLIPSE ellipse = { centre, 130, 130 };
		D2D1_ELLIPSE ellipse0 = { centreLeft, 130, 130 };
		D2D1_ELLIPSE ellipse1 = { centreRight, 130, 130 };
		pEllipseArray.push_back(ellipse);
		pEllipseArray.push_back(ellipse0);
		pEllipseArray.push_back(ellipse1);
		m_pD2DDeviceContext1->BeginDraw();
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));


		// draw the ellipses
		m_pD2DDeviceContext1->DrawEllipse(ellipse, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse0, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse1, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);

		IDWriteTextFormat* pTextFormat = nullptr;
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			DWRITE_FONT_STYLE_ITALIC,
			DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
			24,
			L"",
			&pTextFormat);

		std::cout << "Setting Text Alignment Object" << endl;
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);


		std::cout << "Setting Text Layout Information 0" << endl;
		IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText0,
			stringLength0,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation0
			);

		std::cout << "Setting Text Layout Information 0" << endl;
		pTextLayoutInformation0->SetMaxHeight(40);
		pTextLayoutInformation0->SetMaxWidth(40);
		pTextLayoutInformation0->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation0->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		IDWriteTextLayout* pTextLayoutInformation1 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText1,
			stringLength1,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation1
			);
		pTextLayoutInformation1->SetMaxHeight(40);
		pTextLayoutInformation1->SetMaxWidth(40);
		pTextLayoutInformation1->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation1->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		std::cout << "Setting Text Layout Information 1" << endl;
		IDWriteTextLayout* pTextLayoutInformation2 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText2,
			stringLength2,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation2
			);
		pTextLayoutInformation2->SetMaxHeight(40);
		pTextLayoutInformation2->SetMaxWidth(40);
		pTextLayoutInformation2->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation2->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);



		std::cout << "Filling The Relevant Circle With Color" << endl;

		if (pLevelOfIntensity < 33)
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pGreenBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);
		}

		else if (pLevelOfIntensity >= 33 && pLevelOfIntensity <= 66)
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse, pYellowBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);
		}

		else if (pLevelOfIntensity > 66 && pLevelOfIntensity <= 100)
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pRedBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
		}

		D2D1_RECT_F rectangle0 = D2D1::RectF(centreLeft.x, centreLeft.y, 10, 10);
		D2D1_RECT_F rectangle = D2D1::RectF(centre.x, centre.y, 10, 10);
		D2D1_RECT_F rectangle1 = D2D1::RectF(centreRight.x, centreRight.y, 10, 10);

		m_pD2DDeviceContext1->DrawTextLayout(
			centreLeft,
			pTextLayoutInformation0,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);


		m_pD2DDeviceContext1->DrawTextLayout(
			centre,
			pTextLayoutInformation1,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);


		m_pD2DDeviceContext1->DrawTextLayout(
			centreRight,
			pTextLayoutInformation2,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);




		D2D1_SIZE_F renderTargetSize = m_pD2DDeviceContext1->GetSize();

		m_pD2DDeviceContext1->EndDraw();


		ID2D1Bitmap* pBitmapObject = nullptr;

		// Retrieve D2D Device.



		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}






		pSwapChainObjectD2D->Present(0, 0);
		EndPaint(hWnd, &ps);


	}
	return SUCCEEDED(hr) ? 0 : 1;
}




HRESULT Direct2DManagementClass::DrawRoadSignVertical(HINSTANCE hInstance, HWND hWnd, UINT pPixelNumber, UINT pLevelOfIntensity)
{
	HRESULT hr = S_OK;

	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;

	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	std::cout << "Screen Width: " << pWidth << endl;
	std::cout << "Screen Height" << pHeight << endl;
	D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
	D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
	int clippedAreaWidth = clippedArea.right - clippedArea.left;
	int clippedAreaHeight = clippedArea.bottom - clippedArea.top;

	ID2D1SolidColorBrush* pGreenBrush = nullptr;
	ID2D1SolidColorBrush* pRedBrush = nullptr;
	ID2D1SolidColorBrush* pYellowBrush = nullptr;
	ID2D1SolidColorBrush* pBlackBrush = nullptr;
	ID2D1SolidColorBrush* pWhiteBrush = nullptr;

	vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();
	static const WCHAR pText0[] = L"33";
	static const WCHAR pText1[] = L"66";
	static const WCHAR pText2[] = L"100";
	static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
	static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
	static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;


	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);


	HDC hDC = BeginPaint(hWnd, &ps);
	if (hDC)
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}



		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &pYellowBrush);



		D2D1_POINT_2F centreMiddle = { pWidth / 2.0f + 100, pHeight / 2.0f };
		D2D1_POINT_2F centreTop = { pWidth / 2.0f + 100, pHeight / 2.0f - 320 };
		D2D1_POINT_2F centreBottom = { pWidth / 2.0f + 100, pHeight / 2.0f + 320 };

		D2D1_POINT_2F textBlockPoint = { pWidth / 2.0f, pHeight / 2.0f + 200 };




		D2D1_POINT_2F centre0 = { pWidth / 2.0f, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreLeft0 = { pWidth / 2.0f - 320, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreRight0 = { pWidth / 2.0f + 320, pHeight / 2.0f - 20 };

		D2D1_ELLIPSE ellipse0 = { centreTop, 135, 135 };
		D2D1_ELLIPSE ellipse = { centreMiddle, 135, 135 };
		D2D1_ELLIPSE ellipse1 = { centreBottom, 135, 135 };

		pEllipseArray.push_back(ellipse0);
		pEllipseArray.push_back(ellipse);
		pEllipseArray.push_back(ellipse1);
		m_pD2DDeviceContext1->BeginDraw();
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));


		D2D1_RECT_F pTextRect = D2D1::RectF(textBlockPoint.x, textBlockPoint.y, textBlockPoint.x + 100, textBlockPoint.y + 60);


		IDWriteTextFormat* pTextFormat = nullptr;
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			DWRITE_FONT_STYLE_ITALIC,
			DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
			72,
			L"",
			&pTextFormat);






		// draw the ellipses
		m_pD2DDeviceContext1->DrawEllipse(ellipse1, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse0, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);

		std::cout << "Setting Text Alignment Object" << endl;
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		std::cout << "Setting Text Layout Information 0" << endl;
		IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText0,
			stringLength0,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation0
			);

		std::cout << "Setting Text Layout Information 0" << endl;
		pTextLayoutInformation0->SetMaxHeight(40);
		pTextLayoutInformation0->SetMaxWidth(40);
		pTextLayoutInformation0->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation0->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		IDWriteTextLayout* pTextLayoutInformation1 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText1,
			stringLength1,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation1
			);
		pTextLayoutInformation1->SetMaxHeight(40);
		pTextLayoutInformation1->SetMaxWidth(40);
		pTextLayoutInformation1->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation1->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		std::cout << "Setting Text Layout Information 1" << endl;
		IDWriteTextLayout* pTextLayoutInformation2 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText2,
			stringLength2,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation2
			);
		pTextLayoutInformation2->SetMaxHeight(40);
		pTextLayoutInformation2->SetMaxWidth(40);
		pTextLayoutInformation2->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation2->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);



		std::cout << "Filling The Relevant Circle With Color" << endl;

		if (pLevelOfIntensity <= 33)
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pGreenBrush);
		}

		else if (pLevelOfIntensity > 33 && pLevelOfIntensity <= 66)
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse, pYellowBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);
		}

		else if (pLevelOfIntensity > 66 && pLevelOfIntensity <= 100)
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pRedBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
		}

		D2D1_RECT_F rectangle0 = D2D1::RectF(centreBottom.x, centreBottom.y, 10, 10);
		D2D1_RECT_F rectangle = D2D1::RectF(centreMiddle.x, centreMiddle.y, 10, 10);
		D2D1_RECT_F rectangle1 = D2D1::RectF(centreTop.x, centreTop.y, 10, 10);

		m_pD2DDeviceContext1->DrawTextLayout(
			centreTop,
			pTextLayoutInformation2,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);


		m_pD2DDeviceContext1->DrawTextLayout(
			centreMiddle,
			pTextLayoutInformation1,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);


		m_pD2DDeviceContext1->DrawTextLayout(
			centreBottom,
			pTextLayoutInformation0,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);




		D2D1_SIZE_F renderTargetSize = m_pD2DDeviceContext1->GetSize();

		m_pD2DDeviceContext1->EndDraw();


		// Retrieve D2D Device.



		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}






		pSwapChainObjectD2D->Present(0, 0);
		EndPaint(hWnd, &ps);


	}
	return SUCCEEDED(hr) ? 0 : 1;
}


HRESULT Direct2DManagementClass::ClearRoadSignVertical(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr = S_OK;
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;
	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
	D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
	int clippedAreaWidth = clippedArea.right - clippedArea.left;
	int clippedAreaHeight = clippedArea.bottom - clippedArea.top;

	ID2D1SolidColorBrush* pGreenBrush = nullptr;
	ID2D1SolidColorBrush* pRedBrush = nullptr;
	ID2D1SolidColorBrush* pYellowBrush = nullptr;
	ID2D1SolidColorBrush* pBlackBrush = nullptr;
	ID2D1SolidColorBrush* pWhiteBrush = nullptr;


	
	vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();
	static const WCHAR pText0[] = L"33";
	static const WCHAR pText1[] = L"66";
	static const WCHAR pText2[] = L"100";
	static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
	static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
	static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;


	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);
	ID2D1Bitmap1* pBitmapToSaveData = nullptr;


	HDC hDC = BeginPaint(hWnd, &ps);
	if (hDC)
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}



		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &pYellowBrush);



		D2D1_POINT_2F centre = { pWidth / 2.0f, pHeight / 2.0f };
		D2D1_POINT_2F centreLeft = { pWidth / 2.0f - 320, pHeight / 2.0f };
		D2D1_POINT_2F centreRight = { pWidth / 2.0f + 320, pHeight / 2.0f };

		D2D1_POINT_2F centre0 = { pWidth / 2.0f, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreLeft0 = { pWidth / 2.0f - 320, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreRight0 = { pWidth / 2.0f + 320, pHeight / 2.0f - 20 };

		D2D1_ELLIPSE ellipse = { centre, 130, 130 };
		D2D1_ELLIPSE ellipse0 = { centreLeft, 130, 130 };
		D2D1_ELLIPSE ellipse1 = { centreRight, 130, 130 };
		pEllipseArray.push_back(ellipse);
		pEllipseArray.push_back(ellipse0);
		pEllipseArray.push_back(ellipse1);
		m_pD2DDeviceContext1->BeginDraw();
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));


		// draw the ellipses
		m_pD2DDeviceContext1->DrawEllipse(ellipse, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse0, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse1, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);

		IDWriteTextFormat* pTextFormat = nullptr;
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			DWRITE_FONT_STYLE_ITALIC,
			DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
			24,
			L"",
			&pTextFormat);

		std::cout << "Setting Text Alignment Object" << endl;
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);


		std::cout << "Setting Text Layout Information 0" << endl;
		IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText0,
			stringLength0,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation0
			);

		std::cout << "Setting Text Layout Information 0" << endl;
		pTextLayoutInformation0->SetMaxHeight(40);
		pTextLayoutInformation0->SetMaxWidth(40);
		pTextLayoutInformation0->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation0->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		IDWriteTextLayout* pTextLayoutInformation1 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText1,
			stringLength1,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation1
			);
		pTextLayoutInformation1->SetMaxHeight(40);
		pTextLayoutInformation1->SetMaxWidth(40);
		pTextLayoutInformation1->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation1->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		std::cout << "Setting Text Layout Information 1" << endl;
		IDWriteTextLayout* pTextLayoutInformation2 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText2,
			stringLength2,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation2
			);
		pTextLayoutInformation2->SetMaxHeight(40);
		pTextLayoutInformation2->SetMaxWidth(40);
		pTextLayoutInformation2->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation2->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);



		std::cout << "Filling The Relevant Circle With Color" << endl;

		
		

		m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);
		m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
		m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
		

		D2D1_RECT_F rectangle0 = D2D1::RectF(centreLeft.x, centreLeft.y, 10, 10);
		D2D1_RECT_F rectangle = D2D1::RectF(centre.x, centre.y, 10, 10);
		D2D1_RECT_F rectangle1 = D2D1::RectF(centreRight.x, centreRight.y, 10, 10);

		m_pD2DDeviceContext1->DrawTextLayout(
			centreLeft,
			pTextLayoutInformation0,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);


		m_pD2DDeviceContext1->DrawTextLayout(
			centre,
			pTextLayoutInformation1,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);


		m_pD2DDeviceContext1->DrawTextLayout(
			centreRight,
			pTextLayoutInformation2,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);




		D2D1_SIZE_F renderTargetSize = m_pD2DDeviceContext1->GetSize();

		m_pD2DDeviceContext1->EndDraw();


		ID2D1Bitmap* pBitmapObject = nullptr;

		// Retrieve D2D Device.



		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}






		pSwapChainObjectD2D->Present(0, 0);
		EndPaint(hWnd, &ps);


	}
	return SUCCEEDED(hr) ? 0 : 1;
}



HRESULT Direct2DManagementClass::CreateBitmapDrawingClipObjectWithInts(HINSTANCE hInstance, HWND hWnd, int pXPosition, int pYPosition, int pWidth, int pHeight, vector<PixelObjectInt> *ppPixelObject)
{
	ofstream myfile("imagebuffer.txt");
	myfile << "Please write this text to a file.\n this text is written using C++\n";
	IWICBitmapSource* pSource = nullptr;
	IWICBitmapSource* pSecondarySource = nullptr;

	IWICBitmap* pBitmapObject = nullptr;
	IWICBitmapLock* pBitmapLock = nullptr;
	IWICBitmapLock* pBitmapLock1 = nullptr;
	IWICBitmapClipper *pIClipper = nullptr;
	BYTE* pixels = nullptr;
	HRESULT hr = S_OK;
	WCHAR szFileName[MAX_PATH];
	WICRect rcLock = { pXPosition, pYPosition, pWidth, pHeight };
	ID2D1Image* outputImage = nullptr;
	vector <PixelObjectInt> pixelDataVector = vector <PixelObjectInt>();
	HDC src = NULL;
	HDC hDC = NULL;
	WICPixelFormatGUID pixelFormat;
	IWICBitmapSource* piConverter = nullptr;
	UINT pImageHeight = NULL;
	UINT pImageWidth = NULL;
	BYTE* pixelArrayPointer = nullptr;
	D2D1_RECT_F rectangle = D2D1::RectF(0, 0, pXPosition + pWidth, pYPosition + pHeight);
	UINT cbBufferSize = 0;
	BYTE* pixelDataArray = NULL;
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapDecoder* pImageDecoder = nullptr;
	IWICBitmapFrameDecode *pFrame = NULL;
	UINT uiWidth = 0;
	UINT uiHeight = 0;
	WICRect rcClip = { pXPosition, pYPosition, pWidth, pHeight };

	if (LocateImageFile(hWnd, szFileName, ARRAYSIZE(szFileName)))
	{

		// Prepare iterator pairs to iterate the file content!



		hr = m_pWICFactory->CreateDecoderFromFilename(
			szFileName,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&pImageDecoder);




		if (SUCCEEDED(hr))
		{
			hr = pImageDecoder->GetFrame(0, &pFrame);
		}



		if (SUCCEEDED(hr))
		{
			pSource = pFrame;
			pSource->AddRef();
			pSource->GetSize(&uiWidth, &uiHeight);

			pSecondarySource = pFrame;
			pSecondarySource->AddRef();
			pSecondarySource->GetSize(&uiWidth, &uiHeight);

			IWICPalette* pImagePalette = nullptr;
			pSecondarySource->CopyPalette(pImagePalette);
		}

		WICRect rc = { pXPosition, pYPosition, pWidth, pHeight };


		if (SUCCEEDED(hr))
		{
			hr = m_pWICFactory->CreateBitmapClipper(&pIClipper);
		}


		// Initialize the clipper with the given rectangle of the frame's image data.
		if (SUCCEEDED(hr))
		{
			hr = pIClipper->Initialize(pSource, &rcClip);
		}


		if (SUCCEEDED(hr))
		{
			SafeReleaseA(&m_pConvertedSourceBitmap);
			hr = m_pWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pConvertedSourceBitmap->Initialize(
				pIClipper,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeCustom);

		}
		if (SUCCEEDED(hr))
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}
		if (SUCCEEDED(hr))
		{

			SafeReleaseA(&m_pD2DBitmap);
			hr = m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);

		}



		if (SUCCEEDED(hr))
		{
			D2D1_SIZE_F pSize = m_pD2DBitmap->GetSize();
			pImageWidth = pSize.width;
			pImageHeight = pSize.height;

			hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidColorBrush);
		}

		if (SUCCEEDED(hr))
		{


			hr = DrawD2DContentAxisAlignedClip(hInstance, hWnd, m_pD2DBitmap, rectangle);

		}


		IWICBitmapSource* pConverter = nullptr;
		WICPixelFormatGUID pixelFormat;

		if (SUCCEEDED(hr))
		{


			pSecondarySource->AddRef();
			pSecondarySource->GetSize(&uiWidth, &uiHeight);
		}

		IWICPixelFormatInfo* pixelFormatInfo = nullptr;
		if (SUCCEEDED(hr))
		{
			pSecondarySource->GetPixelFormat(&pixelFormat);
		}

		if (!IsEqualGUID(pixelFormat, GUID_WICPixelFormat128bppPRGBAFloat))
		{

			hr = WICConvertBitmapSource(GUID_WICPixelFormat128bppPRGBAFloat, pSecondarySource, &pConverter);
			IWICBitmapLock* pBitmapLock = nullptr;


			if (SUCCEEDED(hr))
			{
				pSecondarySource->Release();     // the converter has a reference to the source
				pSecondarySource = NULL;         // so we don't need it anymore.
				pSecondarySource = pConverter;   // let's treat the 128bppPABGR converter as the source
			}


			IWICPalette* pImagePalette = nullptr;
			pSecondarySource->CopyPalette(pImagePalette);
		}




		CImageFileData* pImageClass = new CImageFileData((char*)pixelArrayPointer);

		if (pConverter)
		{
			UINT cbStride = pWidth * sizeof(float) * 4;
			UINT cbBufferSize = cbStride;
			float *pixels = new float[cbBufferSize / sizeof(float)];
			if (pHeight < rc.Height && pWidth < rc.Width)
			{
				rc.Height = pHeight;
				rc.Width = pWidth;
			}
			else
			{
				pWidth = rc.Width;
				pHeight = rc.Height;
			}
		}


		UINT bSize = 0;




		UINT rowPitch1 = 4 * pWidth;
		UINT stride = 0; // stride  = bytes per row					 // (width * (bpp/8)) + padding and round to the nearest DWORD
		stride = (((((UINT)pWidth * 32) + 31) & ~31) >> 3);
		bSize = pHeight * pWidth;
		BYTE *pixelArray = new BYTE[(UINT)pHeight * stride];


		pSecondarySource->CopyPixels(NULL, stride, bSize, pixelArray);
		HDC hDC = NULL;
		hDC = GetDC(hWnd);

		COLORREF bkColor = RGB(255, 255, 255);

		SetBkColor(hDC, bkColor);

		HBITMAP map = CreateBitmap((UINT)pWidth,// width. 512 in my case
			(UINT)pHeight, // height
			1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
			8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
			(void*)pixelArray); // pointer to array
		// Temp HDC to copy picture
		src = CreateCompatibleDC(hDC); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
		SelectObject(src, map); // Inserting picture into our temp HDC
		// Copy image from temp HDC to window




		std::cout << "Pixels Begin" << endl;

		int pixelCounter = 0;


		//	pBitmapObj->GetResolution(&pWidth, &pHeight);

		UINT pYVal = 0;
		UINT pXVal = 0;
		for (pXVal = pXPosition; pXVal < pXPosition + pWidth; ++pXVal)
		{
			for (pYVal = pYPosition; pYVal < pYPosition + pHeight; ++pYVal)
				// Your code here
			{

				std::cout << "Current X Value: " << pXVal << " " << "Current Y Value: " << pYVal << endl;
				COLORREF pCurrentPixel = GetPixel(hDC, pXVal, pYVal);
				int pRedValue = GetRValue(pCurrentPixel);
				int pGreenValue = GetGValue(pCurrentPixel);
				int pBlueValue = GetBValue(pCurrentPixel);
				PixelObjectInt pixelObj = PixelObjectInt(pixelCounter, pXVal, pYVal, pRedValue, pGreenValue, pBlueValue);
				pixelDataVector.push_back(pixelObj);
				std::cout << "Amount of total pixels processed  = " << pixelCounter << endl;
				pixelCounter = pixelCounter + 1;

			}
		}

		std::cout << "The size of pixel buffer = " << pixelDataVector.size() << endl;
		int pAmountOfPixelsLeft = 0;
		pAmountOfPixelsLeft = pixelDataVector.size();


		fstream pixelDataFile;
		pixelDataFile.open("C:\\pixels\\ImageClipperTests.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);

		pixelDataFile.clear();
		pixelDataFile << " " << endl;
		pixelDataFile << " " << endl;
		pixelDataFile << "New Operation Begin" << endl;
		time_t currentTime = time(0);
		pixelDataFile << "current time" << currentTime << endl;
		for (PixelObjectInt currentPixel : pixelDataVector)
		{



			std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
				"X Position = " << currentPixel.xPosition << " " <<
				"Y Position = " << currentPixel.yPosition << " " <<
				"The Value Of Red = " << currentPixel.RMask << " "
				<< "The Value Of Green = " << currentPixel.GMask << " " <<
				"The Value Of Blue = " << currentPixel.BMask << endl;

			pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
			pixelDataFile << "X Position = " << pXPosition << endl;
			pixelDataFile << "Y Position = " << pYPosition << endl;
			pixelDataFile << "The Value Of Red = " << currentPixel.RMask << endl;
			pixelDataFile << "The Value Of Green = " << currentPixel.GMask << endl;
			pixelDataFile << "The Value Of Blue = " << currentPixel.BMask << endl;

			pAmountOfPixelsLeft--;

		}










		pixelDataFile << "New Operation End" << endl;
		pixelDataFile << " " << endl;
		pixelDataFile << " " << endl;
		pixelDataFile.close();
		std::cout << "Pixels END" << endl;










	}

	ReleaseDC(hWnd, src);
	ReleaseDC(hWnd, hDC);
	DeleteDC(src);
	DeleteDC(hDC);
	SafeReleaseA(&pDecoder);
	SafeReleaseA(&pFrame);
	myfile.close();
	*ppPixelObject = pixelDataVector;

	return hr;
}


HRESULT Direct2DManagementClass::ClearRoadSignMotionDetected(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr = S_OK;
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;
	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
	D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
	int clippedAreaWidth = clippedArea.right - clippedArea.left;
	int clippedAreaHeight = clippedArea.bottom - clippedArea.top;

	ID2D1SolidColorBrush* pGreenBrush = nullptr;
	ID2D1SolidColorBrush* pRedBrush = nullptr;
	ID2D1SolidColorBrush* pYellowBrush = nullptr;
	ID2D1SolidColorBrush* pBlackBrush = nullptr;
	ID2D1SolidColorBrush* pWhiteBrush = nullptr;


	vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();
	static const WCHAR pText0[] = L"Motion not Detected";
	static const WCHAR pText1[] = L"Motion Undefines = ";
	static const WCHAR pText2[] = L"Motion Detected Between Image";
	static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
	static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
	static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;


	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);
	ID2D1Bitmap1* pBitmapToSaveData = nullptr;


	HDC hDC = BeginPaint(hWnd, &ps);
	if (hDC)
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}



		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &pYellowBrush);



		D2D1_POINT_2F centre = { pWidth / 2.0f, pHeight / 2.0f };
		D2D1_POINT_2F centreLeft = { pWidth / 2.0f,  pHeight / 2.0f - 320, };
		D2D1_POINT_2F centreRight = { pWidth / 2.0f, pHeight / 2.0f + 320};

		D2D1_POINT_2F centre0 = { pWidth / 2.0f, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreLeft0 = { pWidth / 2.0f - 320, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreRight0 = { pWidth / 2.0f + 320, pHeight / 2.0f - 20 };

		D2D1_ELLIPSE ellipse = { centre, 150, 150 };
		D2D1_ELLIPSE ellipse0 = { centreLeft, 150, 150 };
		D2D1_ELLIPSE ellipse1 = { centreRight, 150, 150 };
		pEllipseArray.push_back(ellipse);
		pEllipseArray.push_back(ellipse0);
		pEllipseArray.push_back(ellipse1);
		m_pD2DDeviceContext1->BeginDraw();
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));


		// draw the ellipses
		m_pD2DDeviceContext1->DrawEllipse(ellipse, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse0, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse1, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);

		IDWriteTextFormat* pTextFormat = nullptr;
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			DWRITE_FONT_STYLE_ITALIC,
			DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
			24,
			L"",
			&pTextFormat);


		std::cout << "Setting Text Alignment Object" << endl;
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		std::cout << "Setting Text Layout Information 0" << endl;
		IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText0,
			stringLength0,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation0
			);

		std::cout << "Setting Text Layout Information 0" << endl;
		pTextLayoutInformation0->SetMaxHeight(40);
		pTextLayoutInformation0->SetMaxWidth(40);
		pTextLayoutInformation0->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation0->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		IDWriteTextLayout* pTextLayoutInformation1 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText1,
			stringLength1,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation1
			);
		pTextLayoutInformation1->SetMaxHeight(40);
		pTextLayoutInformation1->SetMaxWidth(40);
		pTextLayoutInformation1->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation1->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		std::cout << "Setting Text Layout Information 1" << endl;
		IDWriteTextLayout* pTextLayoutInformation2 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText2,
			stringLength2,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation2
			);
		pTextLayoutInformation2->SetMaxHeight(40);
		pTextLayoutInformation2->SetMaxWidth(40);
		pTextLayoutInformation2->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation2->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);


		std::cout << "Filling The Relevant Circle With Color" << endl;


		D2D1_RECT_F rectangle0 = D2D1::RectF(centreLeft.x, centreLeft.y, 10, 10);
		D2D1_RECT_F rectangle = D2D1::RectF(centre.x, centre.y, 10, 10);
		D2D1_RECT_F rectangle1 = D2D1::RectF(centreRight.x, centreRight.y, 10, 10);

		m_pD2DDeviceContext1->DrawTextLayout(
			centreLeft,
			pTextLayoutInformation0,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);


		m_pD2DDeviceContext1->DrawTextLayout(
			centre,
			pTextLayoutInformation1,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);


		m_pD2DDeviceContext1->DrawTextLayout(
			centreRight,
			pTextLayoutInformation2,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);




		D2D1_SIZE_F renderTargetSize = m_pD2DDeviceContext1->GetSize();

		m_pD2DDeviceContext1->EndDraw();


		ID2D1Bitmap* pBitmapObject = nullptr;
		// Retrieve D2D Device.



		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}






		pSwapChainObjectD2D->Present(0, 0);
		EndPaint(hWnd, &ps);


	}
	return SUCCEEDED(hr) ? 0 : 1;
}



HRESULT Direct2DManagementClass::ClearRoadSignMotionDetectedVertical(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr = S_OK;
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;
	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
	D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.
		bottom);
	int clippedAreaWidth = clippedArea.right - clippedArea.left;
	int clippedAreaHeight = clippedArea.bottom - clippedArea.top;
	ID2D1SolidColorBrush* pGreenBrush = nullptr;
	ID2D1SolidColorBrush* pRedBrush = nullptr;
	ID2D1SolidColorBrush* pYellowBrush = nullptr;
	ID2D1SolidColorBrush* pBlackBrush = nullptr;
	ID2D1SolidColorBrush* pWhiteBrush = nullptr;
	vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();
	static const WCHAR pText0[] = L"33";
	static const WCHAR pText1[] = L"66";
	static const WCHAR pText2[] = L"100";
	static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
	static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
	static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;
	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);
	ID2D1Bitmap1* pBitmapToSaveData = nullptr;
	HDC hDC = BeginPaint(hWnd, &ps);
	if (hDC)
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &pYellowBrush);
		D2D1_POINT_2F centre = D2D1::Point2F((pWidth / 2.0f + 650), (pHeight / 2.0f));
		D2D1_POINT_2F centreLeft = D2D1::Point2F((pWidth / 2.0f + 650), (pHeight / 2.0f - 310));
		D2D1_POINT_2F centreRight = D2D1::Point2F((pWidth / 2.0f + 650), (pHeight / 2.0f + 310));

		D2D1_ELLIPSE ellipse = { centre, 150, 150 };
		D2D1_ELLIPSE ellipse0 = { centreLeft, 150, 150 };

		D2D1_ELLIPSE ellipse1 = { centreRight, 150, 150 };
		pEllipseArray.push_back(ellipse);
		pEllipseArray.push_back(ellipse0);
		pEllipseArray.push_back(ellipse1);
		m_pD2DDeviceContext1->BeginDraw();
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));
		// draw the ellipses
		m_pD2DDeviceContext1->DrawEllipse(ellipse, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse0, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse1, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);
		IDWriteTextFormat* pTextFormat = nullptr;
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			DWRITE_FONT_STYLE_ITALIC,
			DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
			32,
			L"",
			&pTextFormat);
		std::cout << "Setting Text Alignment Object" << endl;
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
		pTextFormat->SetFlowDirection(DWRITE_FLOW_DIRECTION_LEFT_TO_RIGHT);
		std::cout << "Setting Text Layout Information 0" << endl;
		IDWriteTextLayout* pTextLayoutInformation0 = nullptr;

		float pMaxWidth = 0.0f;
		float pMaxHeight = 0.0f;

		int pStrLen0 = (wcslen(pText0));


		if (pStrLen0 == 2)
		{
			pMaxWidth = 60;
			pMaxHeight = 60;
		}
		
		else if (pStrLen0 == 3)
		{
			pMaxWidth = 90;
			pMaxHeight = 90;
		}



		hr = m_pDWriteFactory->CreateTextLayout(
			pText0,
			stringLength0,
			pTextFormat,
			pMaxWidth, // maxWidth
			pMaxHeight, // maxHeight
			&pTextLayoutInformation0
			);
		std::cout << "Setting Text Layout Information 0" << endl;
		pTextLayoutInformation0->SetMaxHeight(60);
		pTextLayoutInformation0->SetMaxWidth(60);
		pTextLayoutInformation0->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation0->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
		IDWriteTextLayout* pTextLayoutInformation1 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText1,
			stringLength1,
			pTextFormat,
			pMaxWidth, // maxWidth
			pMaxHeight, // maxHeight
			&pTextLayoutInformation1
			);
		pTextLayoutInformation1->SetMaxHeight(60);
		pTextLayoutInformation1->SetMaxWidth(60);
		pTextLayoutInformation1->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation1->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
		std::cout << "Setting Text Layout Information 1" << endl;
		IDWriteTextLayout* pTextLayoutInformation2 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText2,
			stringLength2,
			pTextFormat,
			pMaxWidth, // maxWidth
			pMaxHeight, // maxHeight
			&pTextLayoutInformation2
			);

		pTextLayoutInformation2->SetMaxHeight(60);
		pTextLayoutInformation2->SetMaxWidth(60);
		pTextLayoutInformation2->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation2->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
		
		std::cout << "Filling The Relevant Circle With Color" << endl;
		D2D1_RECT_F rectangle0 = D2D1::RectF(ellipse0.point.x - 200, ellipse0.point.y, ellipse0.point.x - 180,  ellipse0.point.y + 20);
		D2D1_RECT_F rectangle = D2D1::RectF(ellipse.point.x - 200, ellipse.point.y, ellipse.point.x - 180, ellipse.point.y + 20);
		D2D1_RECT_F rectangle1 = D2D1::RectF(ellipse0.point.x - 200, ellipse0.point.y, ellipse0.point.x - 180, ellipse0.point.y + 20);
		D2D1_POINT_2F pTextRect0 = ellipse0.point;
		D2D1_POINT_2F pTextRect = ellipse0.point;
		D2D1_POINT_2F pTextRect1 = ellipse1.point;
		D2D1_POINT_2F pPoint0 = ellipse0.point;
		D2D1_POINT_2F pPoint1 = ellipse0.point;
		D2D1_POINT_2F pPoint2 = ellipse0.point;
		
		float pXPosition0 = pPoint0.x - 600;
		float pXPosition1 = pPoint1.x - 600;
		float pXPosition2 = pPoint2.x - 600;
		float pYPosition0 = pPoint0.y;
		float pYPosition1 = pPoint1.y;
		float pYPosition2 = pPoint2.y;
		
		pTextRect0.x = pXPosition0;
		pTextRect0.y = pYPosition0;
		pTextRect.x = pXPosition1;
		pTextRect.y = pYPosition1;
		pTextRect1.x = pXPosition2;
		pTextRect1.y = pYPosition2;

		m_pD2DDeviceContext1->DrawTextLayout(
			pTextRect0,
			pTextLayoutInformation0,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);
		m_pD2DDeviceContext1->DrawTextLayout(
			pTextRect,
			pTextLayoutInformation1,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);
		m_pD2DDeviceContext1->DrawTextLayout(
			pTextRect1,
			pTextLayoutInformation2,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);
		D2D1_SIZE_F renderTargetSize = m_pD2DDeviceContext1->GetSize();
		m_pD2DDeviceContext1->EndDraw();

		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}
		pSwapChainObjectD2D->Present(0, 0);
		EndPaint(hWnd, &ps);
}
return SUCCEEDED(hr) ? 0 : 1;
}



HRESULT Direct2DManagementClass::DrawRoadSignMotionDetectedVertical(HINSTANCE hInstance, HWND hWnd, BOOL
	pIsMotionUndefined)
{
	HRESULT hr = S_OK;
	ClearRoadSignMotionDetectedVertical(hInstance, hWnd);
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;
	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
	D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.
		bottom);
	int clippedAreaWidth = clippedArea.right - clippedArea.left;
	int clippedAreaHeight = clippedArea.bottom - clippedArea.top;
	ID2D1SolidColorBrush* pGreenBrush = nullptr;
	ID2D1SolidColorBrush* pRedBrush = nullptr;
	ID2D1SolidColorBrush* pYellowBrush = nullptr;
	ID2D1SolidColorBrush* pBlackBrush = nullptr;
	ID2D1SolidColorBrush* pWhiteBrush = nullptr;
	vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();
	static const WCHAR pText0[] = L"33";
	static const WCHAR pText1[] = L"66";
	static const WCHAR pText2[] = L"100";
	static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
	static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
	static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;
	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);
	ID2D1Bitmap1* pBitmapToSaveData = nullptr;
	HDC hDC = BeginPaint(hWnd, &ps);
	if (hDC)
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}

		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &pYellowBrush);
		D2D1_POINT_2F centre = D2D1::Point2F((pWidth / 2.0f + 650), (pHeight / 2.0f));
		D2D1_POINT_2F centreLeft = D2D1::Point2F((pWidth / 2.0f + 650), (pHeight / 2.0f - 320));
		D2D1_POINT_2F centreRight = D2D1::Point2F((pWidth / 2.0f + 650), (pHeight / 2.0f + 320));
		D2D1_ELLIPSE ellipse = { centre, 140, 140 };
		D2D1_ELLIPSE ellipse0 = { centreLeft, 140, 140 };
		D2D1_ELLIPSE ellipse1 = { centreRight, 140, 140 };
		pEllipseArray.push_back(ellipse);
		pEllipseArray.push_back(ellipse0);
		pEllipseArray.push_back(ellipse1);
		m_pD2DDeviceContext1->BeginDraw();
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));
		// draw the ellipses
		m_pD2DDeviceContext1->DrawEllipse(ellipse, pBlackBrush, 23);
		m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse0, pBlackBrush, 23);
		m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse1, pBlackBrush, 23);
		m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);
		IDWriteTextFormat* pTextFormat = nullptr;
		int pFontSize = 60;
		float pMaxWidth = 100;
		float pMaxHeight = 100;
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			DWRITE_FONT_STYLE_ITALIC,
			DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
			pFontSize,
			L"",
			&pTextFormat);
		std::cout << "Setting Text Alignment Object" << endl;
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
		std::cout << "Setting Text Layout Information 0" << endl;
		IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText0,
			stringLength0,
			pTextFormat,
			pMaxWidth, // maxWidth
			pMaxHeight, // maxHeight
			&pTextLayoutInformation0
			);
		std::cout << "Setting Text Layout Information 0" << endl;
		pTextLayoutInformation0->SetMaxWidth(pMaxWidth);
		pTextLayoutInformation0->SetMaxHeight(pMaxHeight);
		pTextLayoutInformation0->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation0->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
		IDWriteTextLayout* pTextLayoutInformation1 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText1,
			stringLength1,
			pTextFormat,
			pMaxWidth, // maxWidth
			pMaxHeight, // maxHeight
			&pTextLayoutInformation1
			);
		pTextLayoutInformation1->SetMaxWidth(pMaxWidth);
		pTextLayoutInformation1->SetMaxHeight(pMaxHeight);
		pTextLayoutInformation1->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation1->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
		std::cout << "Setting Text Layout Information 1" << endl;
		IDWriteTextLayout* pTextLayoutInformation2 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText2,
			stringLength2,
			pTextFormat,
			pMaxWidth, // maxWidth
			pMaxHeight, // maxHeight
			&pTextLayoutInformation2
			);
		pTextLayoutInformation2->SetMaxWidth(pMaxWidth);
		pTextLayoutInformation2->SetMaxHeight(pMaxHeight);
		pTextLayoutInformation2->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation2->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
		std::cout << "Filling The Relevant Circle With Color" << endl;
		if (pIsMotionUndefined == false)
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pGreenBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
		}
		else if (pIsMotionUndefined == true)
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pRedBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
		} 
		
		D2D1_RECT_F rectangle0 = D2D1::RectF(ellipse0.point.x, ellipse0.point.y, 20, 20);
		D2D1_RECT_F rectangle = D2D1::RectF(ellipse.point.x, ellipse.point.y, 20, 20);
		D2D1_RECT_F rectangle1 = D2D1::RectF(ellipse1.point.x, ellipse1.point.y, 20, 20);
	
		
		float pNewX0 = centreLeft.x - 45;
		float pNewX = centre.x - 45;
		float pNewX1 = centreRight.x - 45;

		float pNewY0 = centreLeft.y - 50;
		float pNewY = centre.y - 50;
		float pNewY1 = centreRight.y - 50;





		D2D1_POINT_2F pTextOnePoint = D2D1::Point2F(pNewX0, pNewY0);
		D2D1_POINT_2F pTextTwoPoint = D2D1::Point2F(pNewX,  pNewY);
		D2D1_POINT_2F pTextThreePoint = D2D1::Point2F(pNewX1, pNewY1);
			
		
		m_pD2DDeviceContext1->DrawTextLayout(
			pTextOnePoint,
			pTextLayoutInformation2,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);
		m_pD2DDeviceContext1->DrawTextLayout(
			pTextTwoPoint,
			pTextLayoutInformation1,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);
		m_pD2DDeviceContext1->DrawTextLayout(
			pTextThreePoint,
			pTextLayoutInformation0,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);

		D2D1_SIZE_F renderTargetSize = m_pD2DDeviceContext1->GetSize();
		m_pD2DDeviceContext1->EndDraw();
		ID2D1Bitmap* pBitmapObject = nullptr;
		// Retrieve D2D Device.
		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}
		pSwapChainObjectD2D->Present(0, 0);
		EndPaint(hWnd, &ps);
}
	return SUCCEEDED(hr) ? 0 : 1;
}




HRESULT Direct2DManagementClass::DrawRoadSignMotionDetectedVertical(HINSTANCE hInstance, HWND hWnd, 
	float pAmountOfDifferentPixels,
	float pLowerGateValueBottom,
	float pMiddleGateValueBottom,
	float pTopGateValueBottom)
{
	HRESULT hr = S_OK;
	ClearRoadSignMotionDetectedVertical(hInstance, hWnd);
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;
	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
	D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.
		bottom);
	int clippedAreaWidth = clippedArea.right - clippedArea.left;
	int clippedAreaHeight = clippedArea.bottom - clippedArea.top;
	ID2D1SolidColorBrush* pGreenBrush = nullptr;
	ID2D1SolidColorBrush* pRedBrush = nullptr;
	ID2D1SolidColorBrush* pYellowBrush = nullptr;
	ID2D1SolidColorBrush* pBlackBrush = nullptr;
	ID2D1SolidColorBrush* pWhiteBrush = nullptr;
	vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();
	static const WCHAR pText0[] = L"Bottom";
	static const WCHAR pText1[] = L"Middle";
	static const WCHAR pText2[] = L"Top";
	static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
	static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
	static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;
	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);
	ID2D1Bitmap1* pBitmapToSaveData = nullptr;
	HDC hDC = BeginPaint(hWnd, &ps);
	if (hDC)
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}

		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &pYellowBrush);
		D2D1_POINT_2F centre = D2D1::Point2F((pWidth / 2.0f + 650), (pHeight / 2.0f));
		D2D1_POINT_2F centreBottom = D2D1::Point2F((pWidth / 2.0f + 650), (pHeight / 2.0f + 310));
		D2D1_POINT_2F centreTop = D2D1::Point2F((pWidth / 2.0f + 650), (pHeight / 2.0f - 310));
		D2D1_ELLIPSE ellipseCentre = { centre, 140, 140 };
		D2D1_ELLIPSE ellipseBottom = { centreBottom, 140, 140 };
		D2D1_ELLIPSE ellipseTop = { centreTop, 140, 140 };
		pEllipseArray.push_back(ellipseCentre);
		pEllipseArray.push_back(ellipseBottom);
		pEllipseArray.push_back(ellipseTop);
		m_pD2DDeviceContext1->BeginDraw();
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));
		// draw the ellipses
		m_pD2DDeviceContext1->DrawEllipse(ellipseCentre, pBlackBrush, 23);
		m_pD2DDeviceContext1->FillEllipse(ellipseCentre, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipseBottom, pBlackBrush, 23);
		m_pD2DDeviceContext1->FillEllipse(ellipseBottom, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipseTop, pBlackBrush, 23);
		m_pD2DDeviceContext1->FillEllipse(ellipseTop, pWhiteBrush);
		IDWriteTextFormat* pTextFormat = nullptr;
		int pFontSize = 32;
		float pMaxWidth = 100;
		float pMaxHeight = 100;
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			DWRITE_FONT_STYLE_ITALIC,
			DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
			pFontSize,
			L"",
			&pTextFormat);
		std::cout << "Setting Text Alignment Object" << endl;
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
		std::cout << "Setting Text Layout Information 0" << endl;
		IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText0,
			stringLength0,
			pTextFormat,
			pMaxWidth, // maxWidth
			pMaxHeight, // maxHeight
			&pTextLayoutInformation0
			);
		std::cout << "Setting Text Layout Information 0" << endl;
		pTextLayoutInformation0->SetMaxWidth(pMaxWidth);
		pTextLayoutInformation0->SetMaxHeight(pMaxHeight);
		pTextLayoutInformation0->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation0->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
		IDWriteTextLayout* pTextLayoutInformation1 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText1,
			stringLength1,
			pTextFormat,
			pMaxWidth, // maxWidth
			pMaxHeight, // maxHeight
			&pTextLayoutInformation1
			);
		pTextLayoutInformation1->SetMaxWidth(pMaxWidth);
		pTextLayoutInformation1->SetMaxHeight(pMaxHeight);
		pTextLayoutInformation1->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation1->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
		std::cout << "Setting Text Layout Information 1" << endl;
		IDWriteTextLayout* pTextLayoutInformation2 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText2,
			stringLength2,
			pTextFormat,
			pMaxWidth, // maxWidth
			pMaxHeight, // maxHeight
			&pTextLayoutInformation2
			);
		pTextLayoutInformation2->SetMaxWidth(pMaxWidth);
		pTextLayoutInformation2->SetMaxHeight(pMaxHeight);
		pTextLayoutInformation2->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation2->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
		std::cout << "Filling The Relevant Circle With Color" << endl;
		if (pAmountOfDifferentPixels <= pLowerGateValueBottom)
		{
			m_pD2DDeviceContext1->FillEllipse(ellipseBottom, pGreenBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipseTop, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipseCentre, pWhiteBrush);
		}
		else if ((pAmountOfDifferentPixels > pLowerGateValueBottom) && (pAmountOfDifferentPixels <= pMiddleGateValueBottom))
		{
			m_pD2DDeviceContext1->FillEllipse(ellipseCentre, pYellowBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipseTop, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipseBottom, pWhiteBrush);
		}
		else if ((pAmountOfDifferentPixels > pMiddleGateValueBottom) && (pAmountOfDifferentPixels <= pTopGateValueBottom))
		{
			m_pD2DDeviceContext1->FillEllipse(ellipseTop, pRedBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipseCentre, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipseBottom, pWhiteBrush);
		}
		D2D1_RECT_F rectangle0 = D2D1::RectF(ellipseBottom.point.x, ellipseBottom.point.y, 20, 20);
		D2D1_RECT_F rectangle = D2D1::RectF(ellipseCentre.point.x, ellipseCentre.point.y, 20, 20);
		D2D1_RECT_F rectangle1 = D2D1::RectF(ellipseTop.point.x, ellipseTop.point.y, 20, 20);


		float pNewX0 = centreBottom.x - 45;
		float pNewX = centre.x - 45;
		float pNewX1 = centreTop.x - 45;

		float pNewY0 = centreBottom.y - 50;
		float pNewY = centre.y - 50;
		float pNewY1 = centreTop.y - 50;





		D2D1_POINT_2F pTextOnePoint = D2D1::Point2F(pNewX0, pNewY0);
		D2D1_POINT_2F pTextTwoPoint = D2D1::Point2F(pNewX, pNewY);
		D2D1_POINT_2F pTextThreePoint = D2D1::Point2F(pNewX1, pNewY1);


		m_pD2DDeviceContext1->DrawTextLayout(
			pTextOnePoint,
			pTextLayoutInformation0,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);
		m_pD2DDeviceContext1->DrawTextLayout(
			pTextTwoPoint,
			pTextLayoutInformation1,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);
		m_pD2DDeviceContext1->DrawTextLayout(
			pTextThreePoint,
			pTextLayoutInformation2,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);

		D2D1_SIZE_F renderTargetSize = m_pD2DDeviceContext1->GetSize();
		m_pD2DDeviceContext1->EndDraw();
		ID2D1Bitmap* pBitmapObject = nullptr;
		// Retrieve D2D Device.
		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}
		pSwapChainObjectD2D->Present(0, 0);
		EndPaint(hWnd, &ps);
		Sleep(1000);
	}
	return SUCCEEDED(hr) ? 0 : 1;
}





HRESULT Direct2DManagementClass::ClearRoadSignMotionDetectedVerticalState(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr = S_OK;
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;
	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
	D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.
		bottom);
	int clippedAreaWidth = clippedArea.right - clippedArea.left;
	int clippedAreaHeight = clippedArea.bottom - clippedArea.top;
	ID2D1SolidColorBrush* pGreenBrush = nullptr;
	ID2D1SolidColorBrush* pRedBrush = nullptr;
	ID2D1SolidColorBrush* pYellowBrush = nullptr;
	ID2D1SolidColorBrush* pBlackBrush = nullptr;
	ID2D1SolidColorBrush* pWhiteBrush = nullptr;
	vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();
	static const WCHAR pText0[] = L"Bottom Range";
	static const WCHAR pText1[] = L"Middle Range";
	static const WCHAR pText2[] = L"Top Range";
	static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
	static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
	static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;
	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);
	ID2D1Bitmap1* pBitmapToSaveData = nullptr;
	HDC hDC = BeginPaint(hWnd, &ps);
	if (hDC)
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}

		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &pYellowBrush);
		D2D1_POINT_2F centre = D2D1::Point2F((pWidth / 2.0f + 650), (pHeight / 2.0f));
		D2D1_POINT_2F centreBottom = D2D1::Point2F((pWidth / 2.0f + 650), (pHeight / 2.0f - 320));
		D2D1_POINT_2F centreTop = D2D1::Point2F((pWidth / 2.0f + 650), (pHeight / 2.0f + 320));
		D2D1_ELLIPSE ellipseCentre = { centre, 140, 140 };
		D2D1_ELLIPSE ellipseBottom = { centreBottom, 140, 140 };
		D2D1_ELLIPSE ellipseTop = { centreTop, 140, 140 };
		pEllipseArray.push_back(ellipseCentre);
		pEllipseArray.push_back(ellipseBottom);
		pEllipseArray.push_back(ellipseTop);
		m_pD2DDeviceContext1->BeginDraw();
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));
		// draw the ellipses
		m_pD2DDeviceContext1->DrawEllipse(ellipseCentre, pBlackBrush, 23);
		m_pD2DDeviceContext1->FillEllipse(ellipseCentre, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipseBottom, pBlackBrush, 23);
		m_pD2DDeviceContext1->FillEllipse(ellipseBottom, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipseTop, pBlackBrush, 23);
		m_pD2DDeviceContext1->FillEllipse(ellipseTop, pWhiteBrush);
		IDWriteTextFormat* pTextFormat = nullptr;
		int pFontSize = 32;
		float pMaxWidth = 100;
		float pMaxHeight = 100;
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			DWRITE_FONT_STYLE_ITALIC,
			DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
			pFontSize,
			L"",
			&pTextFormat);
		std::cout << "Setting Text Alignment Object" << endl;
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
		std::cout << "Setting Text Layout Information 0" << endl;
		IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText0,
			stringLength0,
			pTextFormat,
			pMaxWidth, // maxWidth
			pMaxHeight, // maxHeight
			&pTextLayoutInformation0
			);
		std::cout << "Setting Text Layout Information 0" << endl;
		pTextLayoutInformation0->SetMaxWidth(pMaxWidth);
		pTextLayoutInformation0->SetMaxHeight(pMaxHeight);
		pTextLayoutInformation0->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation0->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
		IDWriteTextLayout* pTextLayoutInformation1 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText1,
			stringLength1,
			pTextFormat,
			pMaxWidth, // maxWidth
			pMaxHeight, // maxHeight
			&pTextLayoutInformation1
			);
		pTextLayoutInformation1->SetMaxWidth(pMaxWidth);
		pTextLayoutInformation1->SetMaxHeight(pMaxHeight);
		pTextLayoutInformation1->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation1->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
		std::cout << "Setting Text Layout Information 1" << endl;
		IDWriteTextLayout* pTextLayoutInformation2 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText2,
			stringLength2,
			pTextFormat,
			pMaxWidth, // maxWidth
			pMaxHeight, // maxHeight
			&pTextLayoutInformation2
			);
		pTextLayoutInformation2->SetMaxWidth(pMaxWidth);
		pTextLayoutInformation2->SetMaxHeight(pMaxHeight);
		pTextLayoutInformation2->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation2->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
		std::cout << "Filling The Relevant Circle With Color" << endl;
		



	

		float pNewX0 = centreBottom.x - 45;
		float pNewX = centre.x - 45;
		float pNewX1 = centreTop.x - 45;

		float pNewY0 = centreBottom.y - 50;
		float pNewY = centre.y - 50;
		float pNewY1 = centreTop.y - 50;





		D2D1_POINT_2F pTextOnePoint = D2D1::Point2F(pNewX0, pNewY0);
		D2D1_POINT_2F pTextTwoPoint = D2D1::Point2F(pNewX, pNewY);
		D2D1_POINT_2F pTextThreePoint = D2D1::Point2F(pNewX1, pNewY1);


		m_pD2DDeviceContext1->DrawTextLayout(
			pTextOnePoint,
			pTextLayoutInformation2,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);
		m_pD2DDeviceContext1->DrawTextLayout(
			pTextTwoPoint,
			pTextLayoutInformation1,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);
		m_pD2DDeviceContext1->DrawTextLayout(
			pTextThreePoint,
			pTextLayoutInformation0,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);

		D2D1_SIZE_F renderTargetSize = m_pD2DDeviceContext1->GetSize();
		m_pD2DDeviceContext1->EndDraw();
		ID2D1Bitmap* pBitmapObject = nullptr;
		// Retrieve D2D Device.
		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}
		pSwapChainObjectD2D->Present(0, 0);
		EndPaint(hWnd, &ps);
	}
	return SUCCEEDED(hr) ? 0 : 1;
}



HRESULT Direct2DManagementClass::DrawRoadSignImageProcessingFSM(HINSTANCE hInstance, HWND hWnd, UINT pLevelOfIntensity)
{
	HRESULT hr = S_OK;
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;

	ClearRoadSignFSM(hInstance, hWnd);

	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
	D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
	int clippedAreaWidth = clippedArea.right - clippedArea.left;
	int clippedAreaHeight = clippedArea.bottom - clippedArea.top;

	ID2D1SolidColorBrush* pGreenBrush = nullptr;
	ID2D1SolidColorBrush* pRedBrush = nullptr;
	ID2D1SolidColorBrush* pYellowBrush = nullptr;
	ID2D1SolidColorBrush* pBlackBrush = nullptr;
	ID2D1SolidColorBrush* pWhiteBrush = nullptr;


	vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();
	static const WCHAR pText0[] = L"Image State No Change";
	static const WCHAR pText1[] = L"Unknown State ";
	static const WCHAR pText2[] = L"Image State Changed ";
	static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
	static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
	static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;


	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);


	HDC hDC = BeginPaint(hWnd, &ps);
	if (hDC)
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}



		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &pYellowBrush);



		D2D1_POINT_2F centre = { pWidth / 2.0f, pHeight / 2.0f };
		D2D1_POINT_2F centreLeft = { pWidth / 2.0f - 320, pHeight / 2.0f };
		D2D1_POINT_2F centreRight = { pWidth / 2.0f + 320, pHeight / 2.0f };

		D2D1_POINT_2F centre0 = { pWidth / 2.0f, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreLeft0 = { pWidth / 2.0f - 320, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreRight0 = { pWidth / 2.0f + 320, pHeight / 2.0f - 20 };

		D2D1_ELLIPSE ellipse = { centre, 150, 150 };
		D2D1_ELLIPSE ellipse0 = { centreLeft, 150, 150 };
		D2D1_ELLIPSE ellipse1 = { centreRight, 150, 150 };
		pEllipseArray.push_back(ellipse);
		pEllipseArray.push_back(ellipse0);
		pEllipseArray.push_back(ellipse1);
		m_pD2DDeviceContext1->BeginDraw();
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));


		// draw the ellipses
		m_pD2DDeviceContext1->DrawEllipse(ellipse, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse0, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse1, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);

		IDWriteTextFormat* pTextFormat = nullptr;
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			DWRITE_FONT_STYLE_ITALIC,
			DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
			24,
			L"",
			&pTextFormat);

		std::cout << "Setting Text Alignment Object" << endl;
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		std::cout << "Setting Text Layout Information 0" << endl;
		IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText0,
			stringLength0,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation0
			);

		std::cout << "Setting Text Layout Information 0" << endl;
		pTextLayoutInformation0->SetMaxHeight(40);
		pTextLayoutInformation0->SetMaxWidth(40);
		pTextLayoutInformation0->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation0->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		IDWriteTextLayout* pTextLayoutInformation1 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText1,
			stringLength1,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation1
			);
		pTextLayoutInformation1->SetMaxHeight(40);
		pTextLayoutInformation1->SetMaxWidth(40);
		pTextLayoutInformation1->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation1->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		std::cout << "Setting Text Layout Information 1" << endl;
		IDWriteTextLayout* pTextLayoutInformation2 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText2,
			stringLength2,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation2
			);
		pTextLayoutInformation2->SetMaxHeight(40);
		pTextLayoutInformation2->SetMaxWidth(40);
		pTextLayoutInformation2->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation2->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);



		std::cout << "Filling The Relevant Circle With Color" << endl;

		if (pLevelOfIntensity < 33)
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pGreenBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);
		}

		else if (pLevelOfIntensity >= 33 && pLevelOfIntensity <= 66)
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse, pYellowBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);
		}

		else if (pLevelOfIntensity > 66 && pLevelOfIntensity <= 100)
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pRedBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
		}

		D2D1_RECT_F rectangle0 = D2D1::RectF(centreLeft.x, centreLeft.y, 10, 10);
		D2D1_RECT_F rectangle = D2D1::RectF(centre.x, centre.y, 10, 10);
		D2D1_RECT_F rectangle1 = D2D1::RectF(centreRight.x, centreRight.y, 10, 10);

		m_pD2DDeviceContext1->DrawTextLayout(
			centreLeft,
			pTextLayoutInformation0,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);


		m_pD2DDeviceContext1->DrawTextLayout(
			centre,
			pTextLayoutInformation1,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);


		m_pD2DDeviceContext1->DrawTextLayout(
			centreRight,
			pTextLayoutInformation2,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);




		D2D1_SIZE_F renderTargetSize = m_pD2DDeviceContext1->GetSize();

		m_pD2DDeviceContext1->EndDraw();


		// Retrieve D2D Device.



		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}






		pSwapChainObjectD2D->Present(0, 0);
		EndPaint(hWnd, &ps);


	}
	return SUCCEEDED(hr) ? 0 : 1;
}


		
	
	


HRESULT Direct2DManagementClass::DrawRoadSignStateMachineOddEven(HINSTANCE hInstance, HWND hWnd, UINT pOutputIntensity)
{

	HRESULT hr = S_OK;
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);
	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
	D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
	int clippedAreaWidth = clippedArea.right - clippedArea.left;
	int clippedAreaHeight = clippedArea.bottom - clippedArea.top;

	ID2D1SolidColorBrush* pGreenBrush = nullptr;
	ID2D1SolidColorBrush* pRedBrush = nullptr;
	ID2D1SolidColorBrush* pYellowBrush = nullptr;
	ID2D1SolidColorBrush* pBlackBrush = nullptr;
	ID2D1SolidColorBrush* pWhiteBrush = nullptr;

	vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();
	static const WCHAR pText0[] = L"Odd Number";
	static const WCHAR pText1[] = L"Even Number";
	static const WCHAR pText2[] = L"Unknown Type";
	static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
	static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
	static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;



	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenBrush);
	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedBrush);
	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteBrush);
	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &pYellowBrush);
	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);



	D2D1_POINT_2F centre = { pWidth / 2.0f, pHeight / 2.0f };
	D2D1_POINT_2F centreLeft = { pWidth / 2.0f - 320, pHeight / 2.0f };
	D2D1_POINT_2F centreRight = { pWidth / 2.0f + 320, pHeight / 2.0f };

	D2D1_POINT_2F centre0 = { pWidth / 2.0f, pHeight / 2.0f - 20 };
	D2D1_POINT_2F centreLeft0 = { pWidth / 2.0f - 320, pHeight / 2.0f - 20 };
	D2D1_POINT_2F centreRight0 = { pWidth / 2.0f + 320, pHeight / 2.0f - 20 };

	D2D1_ELLIPSE ellipse = { centre, 130, 130 };
	D2D1_ELLIPSE ellipse0 = { centreLeft, 130, 130 };
	D2D1_ELLIPSE ellipse1 = { centreRight, 130, 130 };
	pEllipseArray.push_back(ellipse);
	pEllipseArray.push_back(ellipse0);
	pEllipseArray.push_back(ellipse1);
	m_pD2DDeviceContext1->BeginDraw();

	// draw the ellipses
	m_pD2DDeviceContext1->DrawEllipse(ellipse0, pBlackBrush, 25);
	m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);
	m_pD2DDeviceContext1->DrawEllipse(ellipse1, pBlackBrush, 25);
	m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);

	IDWriteTextFormat* pTextFormat = nullptr;
	hr = m_pDWriteFactory->CreateTextFormat(
		L"Times New Roman",
		NULL,
		DWRITE_FONT_WEIGHT_EXTRA_BOLD,
		DWRITE_FONT_STYLE_ITALIC,
		DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
		24,
		L"",
		&pTextFormat);

	std::cout << "Setting Text Alignment Object" << endl;
	pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);


	std::cout << "Setting Text Layout Information 0" << endl;
	IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
	IDWriteTextLayout* pTextLayoutInformation1 = nullptr;

	hr = m_pDWriteFactory->CreateTextLayout(
		pText0,
		stringLength0,
		pTextFormat,
		20, // maxWidth
		20, // maxHeight
		&pTextLayoutInformation0
		);


	std::cout << "Setting Text Layout Information 1" << endl;
	IDWriteTextLayout* pTextLayoutInformation2 = nullptr;
	hr = m_pDWriteFactory->CreateTextLayout(
		pText1,
		stringLength1,
		pTextFormat,
		20, // maxWidth
		20, // maxHeight
		&pTextLayoutInformation2
		);


	std::cout << "Filling The Relevant Circle With Color" << endl;

	if (pOutputIntensity == 0)
	{
		m_pD2DDeviceContext1->FillEllipse(ellipse0, pGreenBrush);
		m_pD2DDeviceContext1->FillEllipse(ellipse, pBlackBrush);
		m_pD2DDeviceContext1->FillEllipse(ellipse1, pBlackBrush);
	}


	else if (pOutputIntensity == 1)
	{
		m_pD2DDeviceContext1->FillEllipse(ellipse1, pRedBrush);
		m_pD2DDeviceContext1->FillEllipse(ellipse0, pBlackBrush);
		m_pD2DDeviceContext1->FillEllipse(ellipse, pBlackBrush);
	}



	D2D1_RECT_F rectangle0 = D2D1::RectF(centreLeft.x, centreLeft.y, 10, 10);
	D2D1_RECT_F rectangle = D2D1::RectF(centre.x, centre.y, 10, 10);
	D2D1_RECT_F rectangle1 = D2D1::RectF(centreRight.x, centreRight.y, 10, 10);

	m_pD2DDeviceContext1->DrawTextLayout(
		centreLeft,
		pTextLayoutInformation0,
		pBlackBrush,
		D2D1_DRAW_TEXT_OPTIONS_NONE);



	m_pD2DDeviceContext1->DrawTextLayout(
		centreRight,
		pTextLayoutInformation2,
		pBlackBrush,
		D2D1_DRAW_TEXT_OPTIONS_NONE);




	D2D1_SIZE_F renderTargetSize = m_pD2DDeviceContext1->GetSize();

	m_pD2DDeviceContext1->EndDraw();


	// Retrieve D2D Device.



	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
	}






	pSwapChainObjectD2D->Present(0, 0);

	EndPaint(hWnd, &ps);
	return hr;
}





HRESULT Direct2DManagementClass::DrawRoadSign(HINSTANCE hInstance, HWND hWnd, UINT pAmountOfIdenticalPixel, UINT pAmountOfDifferentPixels)
{
	HRESULT hr = S_OK;
	ClearRoadSignImageComparison(hInstance, hWnd);
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;

	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	D2D1_RECT_F outputRect = D2D1::RectF(0.0f, 0.0f, pWidth, pHeight);
	D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
	int clippedAreaWidth = clippedArea.right - clippedArea.left;
	int clippedAreaHeight = clippedArea.bottom - clippedArea.top;

	ID2D1SolidColorBrush* pGreenBrush = nullptr;
	ID2D1SolidColorBrush* pRedBrush = nullptr;
	ID2D1SolidColorBrush* pYellowBrush = nullptr;
	ID2D1SolidColorBrush* pBlackBrush = nullptr;
	ID2D1SolidColorBrush* pWhiteBrush = nullptr;

	vector<D2D1_ELLIPSE> pEllipseArray = vector<D2D1_ELLIPSE>();
	static const WCHAR pText0[] = L"Identical";
	static const WCHAR pText1[] = L"Mixed";
	static const WCHAR pText2[] = L"Different";
	static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
	static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
	static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;


	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);


	HDC hDC = BeginPaint(hWnd, &ps);
	if (hDC)
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}



		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteBrush);
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &pYellowBrush);



		D2D1_POINT_2F centre = { pWidth / 2.0f, pHeight / 2.0f };
		D2D1_POINT_2F centreLeft = { pWidth / 2.0f - 320, pHeight / 2.0f };
		D2D1_POINT_2F centreRight = { pWidth / 2.0f + 320, pHeight / 2.0f };

		D2D1_POINT_2F centre0 = { pWidth / 2.0f, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreLeft0 = { pWidth / 2.0f - 320, pHeight / 2.0f - 20 };
		D2D1_POINT_2F centreRight0 = { pWidth / 2.0f + 320, pHeight / 2.0f - 20 };

		D2D1_ELLIPSE ellipse = { centre, 150, 150 };
		D2D1_ELLIPSE ellipse0 = { centreLeft, 150, 150 };
		D2D1_ELLIPSE ellipse1 = { centreRight, 150, 150 };

		pEllipseArray.push_back(ellipse);
		pEllipseArray.push_back(ellipse0);
		pEllipseArray.push_back(ellipse1);
		m_pD2DDeviceContext1->BeginDraw();

		// draw the ellipses
		m_pD2DDeviceContext1->DrawEllipse(ellipse, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse0, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
		m_pD2DDeviceContext1->DrawEllipse(ellipse1, pBlackBrush, 25);
		m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);

		IDWriteTextFormat* pTextFormat = nullptr;
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			DWRITE_FONT_STYLE_ITALIC,
			DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
			24,
			L"",
			&pTextFormat);

		std::cout << "Setting Text Alignment Object" << endl;
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);


		std::cout << "Setting Text Layout Information 0" << endl;
		IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText0,
			stringLength0,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation0
			);

		std::cout << "Setting Text Layout Information 0" << endl;
		pTextLayoutInformation0->SetMaxHeight(40);
		pTextLayoutInformation0->SetMaxWidth(40);
		pTextLayoutInformation0->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation0->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		IDWriteTextLayout* pTextLayoutInformation1 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText1,
			stringLength1,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation1
			);
		pTextLayoutInformation1->SetMaxHeight(40);
		pTextLayoutInformation1->SetMaxWidth(40);
		pTextLayoutInformation1->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation1->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

		std::cout << "Setting Text Layout Information 1" << endl;
		IDWriteTextLayout* pTextLayoutInformation2 = nullptr;
		hr = m_pDWriteFactory->CreateTextLayout(
			pText2,
			stringLength2,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation2
			);
		pTextLayoutInformation2->SetMaxHeight(40);
		pTextLayoutInformation2->SetMaxWidth(40);
		pTextLayoutInformation2->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation2->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);






		std::cout << "Filling The Relevant Circle With Color" << endl;

		if ((pAmountOfIdenticalPixel > 0) && (pAmountOfDifferentPixels < pAmountOfIdenticalPixel))
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pGreenBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);
		};

		if ((pAmountOfIdenticalPixel > 0) && (pAmountOfDifferentPixels == pAmountOfIdenticalPixel))
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse, pYellowBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pWhiteBrush);
		}

		if ((pAmountOfDifferentPixels > 0) && (pAmountOfDifferentPixels > pAmountOfIdenticalPixel))
		{
			m_pD2DDeviceContext1->FillEllipse(ellipse1, pRedBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse0, pWhiteBrush);
			m_pD2DDeviceContext1->FillEllipse(ellipse, pWhiteBrush);
		};

		m_pD2DDeviceContext1->DrawTextLayout(
			centreLeft,
			pTextLayoutInformation0,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);


		m_pD2DDeviceContext1->DrawTextLayout(
			centre,
			pTextLayoutInformation1,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);


		m_pD2DDeviceContext1->DrawTextLayout(
			centreRight,
			pTextLayoutInformation2,
			pBlackBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);




		D2D1_SIZE_F renderTargetSize = m_pD2DDeviceContext1->GetSize();

		m_pD2DDeviceContext1->EndDraw();


		// Retrieve D2D Device.



		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}






		pSwapChainObjectD2D->Present(0, 0);

		EndPaint(hWnd, &ps);


	}
	return SUCCEEDED(hr) ? 0 : 1;
}










HRESULT Direct2DManagementClass::DrawD2DContentAxisAlignedClip(HINSTANCE hInstance, HWND hWnd, ID2D1Bitmap* pBitmapToDraw, D2D1_RECT_F outputRect)
{
	HRESULT hr = S_OK;
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;

	IWICBitmapLock* pLockObject = nullptr;
	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	ID2D1BitmapBrush* pBitmapBrush = nullptr;
	D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
	int clippedAreaWidth = clippedArea.right - clippedArea.left;
	int clippedAreaHeight = clippedArea.bottom - clippedArea.top;
	

	if (BeginPaint(hWnd, &ps))
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}



		LPCSTR pFileName = nullptr;

		hr = m_pD2DDeviceContext1->CreateBitmapBrush(pBitmapToDraw, &pBitmapBrush);

        

		m_pD2DDeviceContext1->BeginDraw();
		m_pD2DDeviceContext1->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));
		m_pD2DDeviceContext1->FillRectangle(outputRect, pBitmapBrush);
		hr = m_pD2DDeviceContext1->EndDraw();


		

		

		 // Retrieve D2D Device.
	


		if (hr == D2DERR_RECREATE_TARGET)
		{
			SafeReleaseA(&m_pD2DBitmap);
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}



	

	
		pSwapChainObjectD2D->Present(0, 0);

		EndPaint(hWnd, &ps);
	}
	return SUCCEEDED(hr) ? 0 : 1;
}





HRESULT Direct2DManagementClass::DrawD2DContentAxisAlignedClipWithRectangleGeometry(HINSTANCE hInstance, HWND hWnd,
	ID2D1Bitmap* pBitmapToDraw,
	ID2D1PathGeometry* pPathGeometryObject,
	D2D1_RECT_F outputRect)
{
	HRESULT hr = S_OK;
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;

	IWICBitmapLock* pLockObject = nullptr;
	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	ID2D1BitmapBrush* pBitmapBrush = nullptr;
	D2D1_RECT_F clippedArea = D2D1::RectF(outputRect.left, outputRect.top, outputRect.right, outputRect.bottom);
	int clippedAreaWidth = clippedArea.right - clippedArea.left;
	int clippedAreaHeight = clippedArea.bottom - clippedArea.top;


	if (BeginPaint(hWnd, &ps))
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}


		ID2D1Layer* pLayerObject = nullptr;

		
		hr = m_pD2DDeviceContext1->CreateBitmapBrush(pBitmapToDraw, &pBitmapBrush);
		m_pD2DDeviceContext1->CreateLayer(&pLayerObject);

		m_pD2DDeviceContext1->BeginDraw();
		m_pD2DDeviceContext1->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pD2DDeviceContext1->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), pPathGeometryObject),
			pLayerObject);

		m_pD2DDeviceContext1->PopLayer();
		hr = m_pD2DDeviceContext1->EndDraw();






		// Retrieve D2D Device.



		if (hr == D2DERR_RECREATE_TARGET)
		{
			SafeReleaseA(&m_pD2DBitmap);
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}






		pSwapChainObjectD2D->Present(0, 0);

		EndPaint(hWnd, &ps);
	}
	return SUCCEEDED(hr) ? 0 : 1;
}

HRESULT Direct2DManagementClass::CreateAndDrawBitmapFromPixelVector(HINSTANCE hInstance, HWND hWnd,
	int pImageWidth, int pImageHeight,
	vector<PixelObject> pCorePixelArray)
{
	HRESULT hr = S_OK;

	std::cout << "We will Start By Prinitng Our Array" << endl;


	fstream pixelDataFile;
	pixelDataFile.open("C:\\pixels\\Image Restoration Test.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);


	for (PixelObject currentPixel : pCorePixelArray)
	{



		std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
			"X Position = " << currentPixel.xPosition << " " <<
			"Y Position = " << currentPixel.xPosition << " " <<
			"The Value Of Red = " << currentPixel.RMask << " "
			<< "The Value Of Green = " << currentPixel.GMask << " " <<
			"The Value Of Blue = " << currentPixel.BMask << endl;

		pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
		pixelDataFile << "X Position = " << currentPixel.xPosition << endl;
		pixelDataFile << "Y Position = " << currentPixel.xPosition << endl;
		pixelDataFile << "The Value Of Red = " << currentPixel.RMask << endl;
		pixelDataFile << "The Value Of Green = " << currentPixel.GMask << endl;
		pixelDataFile << "The Value Of Blue = " << currentPixel.BMask << endl;


	}






	PAINTSTRUCT ps;
	RECT rc{ 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);


	HDC hdc = CreateCompatibleDC(NULL);
	HDC myMemHDC2 = CreateCompatibleDC(hdc);

	for (PixelObject currentPixel : pCorePixelArray)
	{

		SetPixel(hdc, currentPixel.xPosition, currentPixel.yPosition, RGB(currentPixel.RMask, currentPixel.GMask, currentPixel.BMask));
	}


	HBITMAP pBitmapObject = nullptr;
	pBitmapObject = CreateCompatibleBitmap(hdc, pImageWidth, pImageHeight);

	HGDIOBJ prevHBITMAP = SelectObject(hdc, pBitmapObject);
	BitBlt(myMemHDC2, 0, 0, pImageWidth, pImageHeight, hdc, 0, 0, SRCCOPY);
	SelectObject(myMemHDC2, prevHBITMAP);


	hr = CreateDeviceResources(hInstance, hWnd);

	ID2D1Bitmap* pBitmapObjectD2D = nullptr;
	IWICBitmap* pWicBmp = nullptr;


	hr = m_pWICFactory->CreateBitmapFromHBITMAP(pBitmapObject, NULL, WICBitmapUsePremultipliedAlpha,
		&pWicBmp);

	IWICBitmapSource* pSourceObject = nullptr;

	pWicBmp->QueryInterface(__uuidof(pSourceObject), reinterpret_cast<void**>(&pSourceObject));

   

	

	if (SUCCEEDED(hr))
	{
		SafeReleaseA(&m_pConvertedSourceBitmap);
		hr = m_pWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pConvertedSourceBitmap->Initialize(
			pSourceObject,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeCustom);

	}



	if (SUCCEEDED(hr))
	{
		m_pD2DDeviceContext1->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &pBitmapObjectD2D);
	}

	if (SUCCEEDED(hr))
	{
		hr = DrawD2DContent(hInstance, hWnd, pBitmapObjectD2D);
	}

	EndPaint(hWnd, &ps);

	return hr;
}



HRESULT Direct2DManagementClass::DrawD2DContentClip(HINSTANCE hInstance, HWND hWnd, ID2D1Bitmap* pBitmapToDraw, D2D1_RECT_F outputRect)
{
	HRESULT hr = S_OK;
	PAINTSTRUCT ps;
	BYTE* selectedAreaData = NULL;
	IWICBitmapLock* pLockObject = nullptr;
	



	if (BeginPaint(hWnd, &ps))
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}
		
	
		ID2D1BitmapBrush1* pBitmapBrush = nullptr;
		LPCSTR pFileName = nullptr;
		
		hr = m_pD2DDeviceContext1->CreateBitmapBrush(pBitmapToDraw, &pBitmapBrush);



		m_pD2DDeviceContext1->BeginDraw();

		m_pD2DDeviceContext1->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));
		m_pD2DDeviceContext1->FillRectangle(outputRect, pBitmapBrush);
		hr = m_pD2DDeviceContext1->EndDraw();


		if (hr == D2DERR_RECREATE_TARGET)
		{
			SafeReleaseA(&m_pD2DBitmap);
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}
		pSwapChainObjectD2D->Present(0, 0);



		EndPaint(hWnd, &ps);
	}
	return SUCCEEDED(hr) ? 0 : 1;
}





HRESULT Direct2DManagementClass::DrawRenctangleSet(HINSTANCE hInstance, HWND hWnd,
	vector<D2D1_RECT_F> *ppHouseArrayOne, vector<D2D1_RECT_F> *ppHouseArrayTwo)
{
	HRESULT hr = S_OK;
	PAINTSTRUCT ps;
	BYTE* selectedAreaData = NULL;
	IWICBitmapLock* pLockObject = nullptr;

	vector<D2D1_RECT_F> pHouseArrayOne = vector<D2D1_RECT_F>();
	vector<D2D1_RECT_F> pHouseArrayTwo = vector<D2D1_RECT_F>();
	pHouseArrayTwo.clear();
	pHouseArrayOne.clear();

	HMONITOR monitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFO info;
	info.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(monitor, &info);
	int monitorWidth = info.rcMonitor.right - info.rcMonitor.left;
	int monitorHeight = info.rcMonitor.bottom - info.rcMonitor.top;
	int pXPosition0 = (monitorWidth / 2) - 400;
	int pYPosition0 = monitorHeight / 4;


	int pXPosition1 = (monitorWidth / 2) + 200;
	int pYPosition1 = monitorHeight / 4;




	int pImageWidth = 90;
	int pImageHeight = 100;


	std::cout << "X Position Rect 0: " << pXPosition0 << endl;
	std::cout << "Y Position Rect 0: " << pYPosition0 << endl;


	std::cout << "X Position Rect 1: " << pXPosition1 << endl;
	std::cout << "Y Position Rect 1: " << pYPosition1 << endl;




	if (BeginPaint(hWnd, &ps))
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}


		ID2D1BitmapBrush1* pBitmapBrush = nullptr;
		LPCSTR pFileName = nullptr;

		static const WCHAR pText0[] = L"Image State No Change";
		static const WCHAR pText1[] = L"Unknown State ";
		static const WCHAR pText2[] = L"Image State Changed ";

		static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
		static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
		static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;




	
		std::cout << "Setting Text Layout Information 0" << endl;
		IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
		IDWriteTextLayout* pTextLayoutInformation1 = nullptr;
		IDWriteTextLayout* pTextLayoutInformation2 = nullptr;



		IDWriteTextFormat* pTextFormat = nullptr;
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			DWRITE_FONT_STYLE_ITALIC,
			DWRITE_FONT_STRETCH_NORMAL,
			18,
			L"",
			&pTextFormat);


		hr = m_pDWriteFactory->CreateTextLayout(
			pText0,
			stringLength0,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation0
			);



		hr = m_pDWriteFactory->CreateTextLayout(
			pText1,
			stringLength0,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation1
			);



		hr = m_pDWriteFactory->CreateTextLayout(
			pText2,
			stringLength0,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation2
			);

		std::cout << "Setting Text Layout Information 0" << endl;
		pTextLayoutInformation0->SetMaxHeight(40);
		pTextLayoutInformation0->SetMaxWidth(40);
		pTextLayoutInformation0->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation0->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);



		m_pD2DDeviceContext1->BeginDraw();

		m_pD2DDeviceContext1->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));

		D2D1_RECT_F pImageRectangle0 = D2D1::RectF(pXPosition0, pYPosition0, (pXPosition0 + 4 * pImageWidth), (pYPosition0 + pImageHeight));
		D2D1_RECT_F pImageRectangle1 = D2D1::RectF(pXPosition0, pYPosition0 + pImageHeight, pXPosition0 + 4 * pImageWidth, pYPosition0 + (2 * pImageHeight));
		D2D1_RECT_F pImageRectangle2 = D2D1::RectF(pXPosition0, pYPosition0 + 2 * pImageHeight, pXPosition0 + 4 * pImageWidth, pYPosition0 + (3 * pImageHeight));
		D2D1_RECT_F pImageRectangle3 = D2D1::RectF(pXPosition0, pYPosition0 + 3 * pImageHeight, pXPosition0 + 4 * pImageWidth, pYPosition0 + (4 * pImageHeight));
		D2D1_RECT_F pImageRectangle4 = D2D1::RectF(pXPosition0, pYPosition0 + 4 * pImageHeight, pXPosition0 + 4 * pImageWidth, pYPosition0 + (5 * pImageHeight));


		D2D1_RECT_F pImageRectangle5 = D2D1::RectF(pXPosition1, pYPosition0, (pXPosition1 + 4 * pImageWidth), (pYPosition0 + pImageHeight));
		D2D1_RECT_F pImageRectangle6 = D2D1::RectF(pXPosition1, pYPosition0 + pImageHeight, pXPosition1 + 4 * pImageWidth, pYPosition0 + (2 * pImageHeight));
		D2D1_RECT_F pImageRectangle7 = D2D1::RectF(pXPosition1, pYPosition0 + 2 * pImageHeight, pXPosition1 + 4 * pImageWidth, pYPosition0 + (3 * pImageHeight));
		D2D1_RECT_F pImageRectangle8 = D2D1::RectF(pXPosition1, pYPosition0 + 3 * pImageHeight, pXPosition1 + 4 * pImageWidth, pYPosition0 + (4 * pImageHeight));
		D2D1_RECT_F pImageRectangle9 = D2D1::RectF(pXPosition1, pYPosition0 + 4 * pImageHeight, pXPosition1 + 4 * pImageWidth, pYPosition0 + (5 * pImageHeight));


		pHouseArrayOne.push_back(pImageRectangle0);
		pHouseArrayOne.push_back(pImageRectangle1);
		pHouseArrayOne.push_back(pImageRectangle2);
		pHouseArrayOne.push_back(pImageRectangle3);
		pHouseArrayOne.push_back(pImageRectangle4);

		pHouseArrayTwo.push_back(pImageRectangle5);
		pHouseArrayTwo.push_back(pImageRectangle6);
		pHouseArrayTwo.push_back(pImageRectangle7);
		pHouseArrayTwo.push_back(pImageRectangle8);
		pHouseArrayTwo.push_back(pImageRectangle9);





		ID2D1SolidColorBrush* pBlackColorBrush = nullptr;
		ID2D1SolidColorBrush* pRedColorBrush = nullptr;
		ID2D1SolidColorBrush* pGreenColorBrush = nullptr;

		std::cout << "Creating Black Brush" << endl;
		m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackColorBrush);

		std::cout << "Creating Green Brush" << endl;
		m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenColorBrush);

		std::cout << "Creating Red Brush Brush" << endl;
		m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedColorBrush);



		std::cout << "Drawing Image Rects for House One" << endl;
		
		float pTextXRecX0 = pXPosition0;
		float pTextXRecY0 = pYPosition0 - 200;

		float pTextXRecX1 = pXPosition1;
		float pTextXRecY1 = pYPosition1 - 200;


		D2D1_POINT_2F pRecordOne = D2D1::Point2F(pTextXRecX0, pTextXRecY0);
		D2D1_POINT_2F pRecordTwo = D2D1::Point2F(pTextXRecX1, pTextXRecY0);


		
		ID2D1Brush* pBlackBrushInterface = nullptr;
		pBlackBrushInterface = pBlackColorBrush;

		m_pD2DDeviceContext1->DrawTextLayout(
			pRecordOne,
			pTextLayoutInformation0,
			pBlackBrushInterface,
			D2D1_DRAW_TEXT_OPTIONS_NONE);

		m_pD2DDeviceContext1->DrawTextLayout(
				pRecordTwo,
				pTextLayoutInformation2,
				pBlackBrushInterface,
				D2D1_DRAW_TEXT_OPTIONS_NONE);

		
		for (D2D1_RECT_F pHouseTwoRect : pHouseArrayTwo)
		{
			m_pD2DDeviceContext1->DrawRectangle(&pHouseTwoRect, pBlackColorBrush, 10.0f, NULL);
			m_pD2DDeviceContext1->FillRectangle(&pHouseTwoRect, pRedColorBrush);
			
		}
		
		for (D2D1_RECT_F pHouseOneRect : pHouseArrayOne)
		{
			m_pD2DDeviceContext1->DrawRectangle(&pHouseOneRect, pBlackColorBrush, 10.0f, NULL);
			m_pD2DDeviceContext1->FillRectangle(&pHouseOneRect, pGreenColorBrush);
			
		}

	



		hr = m_pD2DDeviceContext1->EndDraw();


		if (hr == D2DERR_RECREATE_TARGET)
		{
			SafeReleaseA(&m_pD2DBitmap);
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}
		pSwapChainObjectD2D->Present(0, 0);



		EndPaint(hWnd, &ps);
	}
	
	*ppHouseArrayOne = pHouseArrayOne;
	*ppHouseArrayTwo = pHouseArrayTwo;

	return hr;
}




HRESULT Direct2DManagementClass::ClearRenctangleSet(HINSTANCE hInstance, HWND hWnd,
	vector<D2D1_RECT_F> *ppHouseArrayOne, vector<D2D1_RECT_F> *ppHouseArrayTwo)
{
	HRESULT hr = S_OK;
	PAINTSTRUCT ps;
	BYTE* selectedAreaData = NULL;
	IWICBitmapLock* pLockObject = nullptr;


	 pHouseArrayOne = vector<D2D1_RECT_F>();
	 pHouseArrayTwo = vector<D2D1_RECT_F>();
	 pRectangleStructureArrayOne = vector<RectangleStructure>();
	 pRectangleStructureArrayTwo = vector<RectangleStructure>();

	pHouseArrayTwo.clear();
	pHouseArrayOne.clear();

	HMONITOR monitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFO info;
	info.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(monitor, &info);
	int monitorWidth = info.rcMonitor.right - info.rcMonitor.left;
	int monitorHeight = info.rcMonitor.bottom - info.rcMonitor.top;
	int pXPosition0 = (monitorWidth / 2) - 400;
	int pYPosition0 = monitorHeight / 4;


	int pXPosition1 = (monitorWidth / 2) + 200;
	int pYPosition1 = monitorHeight / 4;




	int pImageWidth = 90;
	int pImageHeight = 100;


	std::cout << "X Position Rect 0: " << pXPosition0 << endl;
	std::cout << "Y Position Rect 0: " << pYPosition0 << endl;


	std::cout << "X Position Rect 1: " << pXPosition1 << endl;
	std::cout << "Y Position Rect 1: " << pYPosition1 << endl;




	if (BeginPaint(hWnd, &ps))
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}


		ID2D1BitmapBrush1* pBitmapBrush = nullptr;
		LPCSTR pFileName = nullptr;

		static const WCHAR pText0[] = L"Image State No Change";
		static const WCHAR pText1[] = L"Unknown State ";
		static const WCHAR pText2[] = L"Image State Changed ";

		static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
		static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
		static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;





		std::cout << "Setting Text Layout Information 0" << endl;
		IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
		IDWriteTextLayout* pTextLayoutInformation1 = nullptr;
		IDWriteTextLayout* pTextLayoutInformation2 = nullptr;



		IDWriteTextFormat* pTextFormat = nullptr;
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			DWRITE_FONT_STYLE_ITALIC,
			DWRITE_FONT_STRETCH_NORMAL,
			12,
			L"",
			&pTextFormat);


		hr = m_pDWriteFactory->CreateTextLayout(
			pText0,
			stringLength0,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation0
			);



		hr = m_pDWriteFactory->CreateTextLayout(
			pText1,
			stringLength0,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation1
			);



		hr = m_pDWriteFactory->CreateTextLayout(
			pText2,
			stringLength0,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation2
			);

		std::cout << "Setting Text Layout Information 0" << endl;
		pTextLayoutInformation0->SetMaxHeight(40);
		pTextLayoutInformation0->SetMaxWidth(40);
		pTextLayoutInformation0->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation0->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);



		m_pD2DDeviceContext1->BeginDraw();

		m_pD2DDeviceContext1->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));

		D2D1_RECT_F pImageRectangle0 = D2D1::RectF(pXPosition0, pYPosition0, (pXPosition0 + 4 * pImageWidth), (pYPosition0 + pImageHeight));
		D2D1_RECT_F pImageRectangle1 = D2D1::RectF(pXPosition0, pYPosition0 + pImageHeight, pXPosition0 + 4 * pImageWidth, pYPosition0 + (2 * pImageHeight));
		D2D1_RECT_F pImageRectangle2 = D2D1::RectF(pXPosition0, pYPosition0 + 2 * pImageHeight, pXPosition0 + 4 * pImageWidth, pYPosition0 + (3 * pImageHeight));
		D2D1_RECT_F pImageRectangle3 = D2D1::RectF(pXPosition0, pYPosition0 + 3 * pImageHeight, pXPosition0 + 4 * pImageWidth, pYPosition0 + (4 * pImageHeight));
		D2D1_RECT_F pImageRectangle4 = D2D1::RectF(pXPosition0, pYPosition0 + 4 * pImageHeight, pXPosition0 + 4 * pImageWidth, pYPosition0 + (5 * pImageHeight));


		D2D1_RECT_F pImageRectangle5 = D2D1::RectF(pXPosition1, pYPosition0, (pXPosition1 + 4 * pImageWidth), (pYPosition0 + pImageHeight));
		D2D1_RECT_F pImageRectangle6 = D2D1::RectF(pXPosition1, pYPosition0 + pImageHeight, pXPosition1 + 4 * pImageWidth, pYPosition0 + (2 * pImageHeight));
		D2D1_RECT_F pImageRectangle7 = D2D1::RectF(pXPosition1, pYPosition0 + 2 * pImageHeight, pXPosition1 + 4 * pImageWidth, pYPosition0 + (3 * pImageHeight));
		D2D1_RECT_F pImageRectangle8 = D2D1::RectF(pXPosition1, pYPosition0 + 3 * pImageHeight, pXPosition1 + 4 * pImageWidth, pYPosition0 + (4 * pImageHeight));
		D2D1_RECT_F pImageRectangle9 = D2D1::RectF(pXPosition1, pYPosition0 + 4 * pImageHeight, pXPosition1 + 4 * pImageWidth, pYPosition0 + (5 * pImageHeight));


		pHouseArrayOne.push_back(pImageRectangle0);
		pHouseArrayOne.push_back(pImageRectangle1);
		pHouseArrayOne.push_back(pImageRectangle2);
		pHouseArrayOne.push_back(pImageRectangle3);
		pHouseArrayOne.push_back(pImageRectangle4);

		pHouseArrayTwo.push_back(pImageRectangle5);
		pHouseArrayTwo.push_back(pImageRectangle6);
		pHouseArrayTwo.push_back(pImageRectangle7);
		pHouseArrayTwo.push_back(pImageRectangle8);
		pHouseArrayTwo.push_back(pImageRectangle9);





		ID2D1SolidColorBrush* pBlackColorBrush = nullptr;
		ID2D1SolidColorBrush* pWhiteColorBrush = nullptr;


		std::cout << "Creating Black Brush Brush" << endl;
		m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackColorBrush);

	
		std::cout << "Creating White Brush Brush" << endl;
		m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteColorBrush);



		std::cout << "Drawing Image Rects for House One" << endl;

		float pTextXRecX0 = pXPosition0;
		float pTextXRecY0 = pYPosition0 - 200;

		float pTextXRecX1 = pXPosition1;
		float pTextXRecY1 = pYPosition1 - 200;


		D2D1_POINT_2F pRecordOne = D2D1::Point2F(pTextXRecX0, pTextXRecY0);
		D2D1_POINT_2F pRecordTwo = D2D1::Point2F(pTextXRecX1, pTextXRecY0);



		ID2D1Brush* pBlackBrushInterface = nullptr;
		pBlackBrushInterface = pBlackColorBrush;

		m_pD2DDeviceContext1->DrawTextLayout(
			pRecordOne,
			pTextLayoutInformation0,
			pBlackBrushInterface,
			D2D1_DRAW_TEXT_OPTIONS_NONE);

		m_pD2DDeviceContext1->DrawTextLayout(
			pRecordTwo,
			pTextLayoutInformation2,
			pBlackBrushInterface,
			D2D1_DRAW_TEXT_OPTIONS_NONE);


		for (D2D1_RECT_F pHouseTwoRect : pHouseArrayTwo)
		{
			m_pD2DDeviceContext1->DrawRectangle(&pHouseTwoRect, pBlackColorBrush, 10.0f, NULL);
			m_pD2DDeviceContext1->FillRectangle(&pHouseTwoRect, pWhiteColorBrush);
		}

		for (D2D1_RECT_F pHouseOneRect : pHouseArrayOne)
		{
			m_pD2DDeviceContext1->DrawRectangle(&pHouseOneRect, pBlackColorBrush, 10.0f, NULL);
			m_pD2DDeviceContext1->FillRectangle(&pHouseOneRect, pWhiteColorBrush);
		}





		hr = m_pD2DDeviceContext1->EndDraw();


		if (hr == D2DERR_RECREATE_TARGET)
		{
			SafeReleaseA(&m_pD2DBitmap);
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}
		pSwapChainObjectD2D->Present(0, 0);



		EndPaint(hWnd, &ps);
	}

	*ppHouseArrayOne = pHouseArrayOne;
	*ppHouseArrayTwo = pHouseArrayTwo;

	return hr;
}



bool Direct2DManagementClass::HDCToFile(const char* FilePath, HDC Context, RECT Area, uint16_t BitsPerPixel)
{
	uint32_t Width = Area.right - Area.left;
	uint32_t Height = Area.bottom - Area.top;
	BITMAPINFO Info;
	BITMAPFILEHEADER Header;
	memset(&Info, 0, sizeof(Info));
	memset(&Header, 0, sizeof(Header));
	Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	Info.bmiHeader.biWidth = Width;
	Info.bmiHeader.biHeight = Height;
	Info.bmiHeader.biPlanes = 1;
	Info.bmiHeader.biBitCount = BitsPerPixel;
	Info.bmiHeader.biCompression = BI_RGB;
	Info.bmiHeader.biSizeImage = Width * Height * (BitsPerPixel > 24 ? 4 : 3);
	Header.bfType = 0x4D42;
	Header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	char* Pixels = NULL;
	HDC MemDC = CreateCompatibleDC(Context);
	HBITMAP Section = CreateDIBSection(Context, &Info, DIB_RGB_COLORS, (void**)&Pixels, 0, 0);
	DeleteObject(SelectObject(MemDC, Section));
	BitBlt(MemDC, 0, 0, Width, Height, Context, Area.left, Area.top, SRCCOPY);
	DeleteDC(MemDC);
	std::fstream hFile(FilePath, std::ios::out | std::ios::binary);
	if (hFile.is_open())
	{
		hFile.write((char*)&Header, sizeof(Header));
		hFile.write((char*)&Info.bmiHeader, sizeof(Info.bmiHeader));
		hFile.write(Pixels, (((BitsPerPixel * Width + 31) & ~31) / 8) * Height);
		hFile.close();
		DeleteObject(Section);
		return true;
	}
	DeleteObject(Section);
	return false;
}




HRESULT Direct2DManagementClass::DrawPixelArray(HINSTANCE hInstance, HWND hWnd, vector<PixelObject> pPixelObjectArray)
{
	HRESULT hr = S_OK;
	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pScreenWidth = rc.right - rc.top;
	UINT pScreenHeight = rc.bottom - rc.top;

	const char* pFilePath = "E:\\RobotData\\my_pixel_array.bmp";
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);
	HDC hDC = NULL;
	HDC src = NULL;	fstream pixelDataFile;
	pixelDataFile.open("C:\\pixels\\SetPixelTextFile.txt", fstream::in | fstream::out | fstream::trunc);
	pixelDataFile.clear();

	hDC = NULL;
	ReleaseDC(hWnd, hDC);
	hDC = GetDC(hWnd);
	src = CreateCompatibleDC(hDC);

	HBITMAP pBitmapObj;

	HGDIOBJ hPen = GetStockObject(BLACK_PEN);
	HGDIOBJ hOldPen = SelectObject(hDC, hPen);

	if (hDC)
	{
		for (PixelObject currentPixel : pPixelObjectArray)
		{


			COLORREF pColor = RGB(255, 0, 0);

			SetPixel(hDC, currentPixel.xPosition, currentPixel.yPosition, pColor);
			SetPixel(src, currentPixel.xPosition, currentPixel.yPosition, pColor);



			std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
				"X Position = " << currentPixel.xPosition << " " <<
				"Y Position = " << currentPixel.yPosition << " " <<
				"The Value Of Red = " << currentPixel.RMask << " "
				<< "The Value Of Green = " << currentPixel.GMask << " " <<
				"The Value Of Blue = " << currentPixel.BMask << endl;

			pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
			pixelDataFile << "X Position = " << currentPixel.xPosition << endl;
			pixelDataFile << "Y Position = " << currentPixel.yPosition << endl;
			pixelDataFile << "The Value Of Red = " << currentPixel.RMask << endl;
			pixelDataFile << "The Value Of Green = " << currentPixel.GMask << endl;
			pixelDataFile << "The Value Of Blue = " << currentPixel.BMask << endl;

		}

	}


	return hr;


}




HRESULT Direct2DManagementClass::DrawPixelArrayOutput(HINSTANCE hInstance, HWND hWnd, vector<OutputPixelObjectWithAngleValuesD2D> pPixelObjectArray)
{
	HRESULT hr = S_OK;
	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pScreenWidth = rc.right - rc.top;
	UINT pScreenHeight = rc.bottom - rc.top;

	const char* pFilePath = "E:\\RobotData\\ImageGeometryOutput.bmp";
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);
	HDC hDC = NULL;
	HDC src = NULL;	fstream pixelDataFile;
	pixelDataFile.open("C:\\pixels\\SetPixelTextFile.txt", fstream::in | fstream::out | fstream::trunc);
	pixelDataFile.clear();

	hDC = NULL;
	ReleaseDC(hWnd, hDC);
	hDC = GetDC(hWnd);
	src = CreateCompatibleDC(hDC);

	HBITMAP pBitmapObj;

	HGDIOBJ hPen = GetStockObject(BLACK_PEN);
	HGDIOBJ hOldPen = SelectObject(hDC, hPen);

	if (hDC)
	{
		int iIndex = 0;

		vector<POINT> pPointArray = vector<POINT>();
		for (OutputPixelObjectWithAngleValuesD2D currentPixel : pPixelObjectArray)
		{
					
			std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
				"X Position = " << currentPixel.xPosition << " " <<
				"Y Position = " << currentPixel.yPosition << " " <<
				"The Value Of Red = " << currentPixel.rValue << " "
				<< "The Value Of Green = " << currentPixel.gValue << " " <<
				"The Value Of Blue = " << currentPixel.bValue << endl;

			pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
			pixelDataFile << "X Position = " << currentPixel.xPosition << endl;
			pixelDataFile << "Y Position = " << currentPixel.yPosition << endl;
			pixelDataFile << "The Value Of Red = " << currentPixel.rValue << endl;
			pixelDataFile << "The Value Of Green = " << currentPixel.gValue << endl;
			pixelDataFile << "The Value Of Blue = " << currentPixel.bValue << endl;

			POINT pPoint;
			pPoint.x = currentPixel.xPosition;
			pPoint.y = currentPixel.yPosition;
			pPointArray.push_back(pPoint);

		}

		POINT* pPolyData = &pPointArray[0];
		PolylineTo(hDC, pPolyData, pPointArray.size());

	}

	
	BITMAPINFO Info;
	BITMAPFILEHEADER Header;
	memset(&Info, 0, sizeof(Info));
	memset(&Header, 0, sizeof(Header));
	Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	Info.bmiHeader.biWidth = pScreenWidth;
	Info.bmiHeader.biHeight = pScreenHeight;
	Info.bmiHeader.biPlanes = 1;
	Info.bmiHeader.biBitCount = 24;
	Info.bmiHeader.biCompression = BI_RGB;
	Info.bmiHeader.biSizeImage = pScreenWidth * pScreenHeight * 24;
	Header.bfType = 0x4D42;
	Header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	char* Pixels = NULL;
	HDC MemDC = CreateCompatibleDC(hDC);
	HBITMAP Section = CreateDIBSection(src, &Info, DIB_RGB_COLORS, (void**)&Pixels, 0, 0);
	DeleteObject(SelectObject(MemDC, Section));
	BitBlt(MemDC, 0, 0, pScreenWidth, pScreenHeight, hDC, rc.left, rc.top, SRCCOPY);
	DeleteDC(MemDC);
	std::fstream hFile(pFilePath, std::ios::out | std::ios::binary);
	if (hFile.is_open())
	{
		hFile.write((char*)&Header, sizeof(Header));
		hFile.write((char*)&Info.bmiHeader, sizeof(Info.bmiHeader));
		hFile.write(Pixels, (((24 * pScreenWidth + 31) & ~31) / 8) * pScreenHeight);
		hFile.close();
		DeleteObject(Section);
		return true;
	}
	DeleteObject(Section);

	return hr;


}











	




HRESULT Direct2DManagementClass::DrawPixelBuffer(HINSTANCE hInstance, HWND hWnd, vector<PixelObject> pPixelObjectArray, UINT pXPosition, UINT pYPosition, UINT pImageWidth, UINT pImageHeight)
{
	HRESULT hr = S_OK;
	D2D1_RECT_F rectangle = D2D1::RectF(0, 0, pXPosition + pImageWidth, pYPosition + pImageHeight);
	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	const char* pFilePath = "E:\\RobotData\\my_pixel_array.bmp";
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);
	HDC hDC = NULL;
	HDC src = NULL;
	fstream pixelDataFile;
	pixelDataFile.open("C:\\pixels\\SetPixelTextFile.txt", fstream::in | fstream::out | fstream::trunc);
	pixelDataFile.clear();

	hDC = NULL;
	ReleaseDC(hWnd, hDC);
	hDC = GetDC(hWnd);
	src = CreateCompatibleDC(hDC);

	HBITMAP pBitmapObj;

	


	COLORREF bkColor = RGB(0, 0, 0);

	SetBkColor(hDC, bkColor);

	if (hDC)
	{
		for (PixelObject currentPixel : pPixelObjectArray)
		{





	//			std::cout << "PixelNumber  = " << currentPixel.pixelNumber << " " <<
	//				"X Position = " << currentPixel.xPosition << " " <<
	//				"Y Position = " << currentPixel.yPosition << " " <<
	//				"The Value Of Red = " << currentPixel.RMask << " "
	//				<< "The Value Of Green = " << currentPixel.GMask << " " <<
	//				"The Value Of Blue = " << currentPixel.BMask << endl;
//
//				pixelDataFile << "PixelNumber  = " << currentPixel.pixelNumber << endl;
//				pixelDataFile << "X Position = " << currentPixel.xPosition << endl;
//				pixelDataFile << "Y Position = " << currentPixel.yPosition << endl;
//				pixelDataFile << "The Value Of Red = " << currentPixel.RMask << endl;
//				pixelDataFile << "The Value Of Green = " << currentPixel.GMask << endl;
//				pixelDataFile << "The Value Of Blue = " << currentPixel.BMask << endl;

				COLORREF pColor = RGB((int)currentPixel.RMask, (int)currentPixel.GMask, (int)currentPixel.BMask);
				
				SetPixel(hDC, currentPixel.xPosition, currentPixel.yPosition, pColor);
				SetPixel(src, currentPixel.xPosition, currentPixel.yPosition, pColor);

		
		
		}

	
		
		BITMAPINFO Info;
		BITMAPFILEHEADER Header;
		memset(&Info, 0, sizeof(Info));
		memset(&Header, 0, sizeof(Header));
		Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		Info.bmiHeader.biWidth = pImageWidth;
		Info.bmiHeader.biHeight = pImageHeight;
		Info.bmiHeader.biPlanes = 1;
		Info.bmiHeader.biBitCount = 24;
		Info.bmiHeader.biCompression = BI_RGB;
		Info.bmiHeader.biSizeImage = pImageWidth * pImageHeight * 24;
		Header.bfType = 0x4D42;
		Header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		char* Pixels = NULL;
		HDC MemDC = CreateCompatibleDC(hDC);
		HBITMAP Section = CreateDIBSection(src, &Info, DIB_RGB_COLORS, (void**)&Pixels, 0, 0);
		DeleteObject(SelectObject(MemDC, Section));
		BitBlt(MemDC, 0, 0, pImageWidth, pImageHeight, hDC, rc.left, rc.top, SRCCOPY);
		DeleteDC(MemDC);
		std::fstream hFile(pFilePath, std::ios::out | std::ios::binary);
		if (hFile.is_open())
		{
			hFile.write((char*)&Header, sizeof(Header));
			hFile.write((char*)&Info.bmiHeader, sizeof(Info.bmiHeader));
			hFile.write(Pixels, (((24 * pImageWidth + 31) & ~31) / 8) * pImageHeight);
			hFile.close();
			DeleteObject(Section);
			return true;
		}
		DeleteObject(Section);






	


	}
	
	return hr;

	
}





HRESULT Direct2DManagementClass::DestroyFilePermanently(HINSTANCE hInstance, HWND hWnd, LPWSTR pFileName)
{
	HRESULT hr = S_OK;
	SHFILEOPSTRUCT sfo = { 0 };
	ZeroMemory(&sfo, sizeof(&sfo));
	sfo.wFunc = FO_DELETE;
	sfo.pFrom = pFileName;
	sfo.fFlags = FOF_ALLOWUNDO | FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;
	int result = SHFileOperation(&sfo);
	return hr;
}


HRESULT Direct2DManagementClass::DrawD2DContent(HINSTANCE hInstance, HWND hWnd, ID2D1Bitmap* pBitmapToDraw)
{
	HRESULT hr;
	PAINTSTRUCT ps;
	
	if (BeginPaint(hWnd, &ps))
	{
			if (m_pD2DDeviceContext1 == nullptr)
			{
				hr = CreateDeviceResources(hInstance, hWnd);
			}
			
			
		
			
			m_pD2DDeviceContext1->BeginDraw();
	
			m_pD2DDeviceContext1->SetTransform(D2D1::Matrix3x2F::Identity());
			m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::Black));

			D2D1_SIZE_F rtSize = m_pD2DDeviceContext->GetSize();
			D2D1_RECT_F rectangle = D2D1::RectF(0, 0, rtSize.width, rtSize.height);

			
			if (pBitmapToDraw)
			{
				m_pD2DDeviceContext1->DrawBitmap(pBitmapToDraw, rectangle);
			}
		
			hr = m_pD2DDeviceContext1->EndDraw();

			if (hr == D2DERR_RECREATE_TARGET)
			{
				SafeReleaseA(&m_pD2DBitmap);
				hr = InvalidateRect(hWnd, NULL, TRUE)? S_OK : E_FAIL;
			}
			pSwapChainObjectD2D->Present(0, 0);
		EndPaint(hWnd, &ps);
	}
	return SUCCEEDED(hr) ? 0 : 1;
}





HRESULT Direct2DManagementClass::DrawD2DContentWithPixels(HINSTANCE hInstance, HWND hWnd, vector<PixelObject> pPixelObjectArray, ID2D1Bitmap* pBrushToFill)
{
	HRESULT hr;
	PAINTSTRUCT ps;

	ID2D1BitmapBrush* pBitmapBrush = nullptr;
	vector<D2D1_POINT_2F> pPointArrayToDraw = vector<D2D1_POINT_2F>();
    
	for (PixelObject pPixelObject : pPixelObjectArray)
	{
		D2D1_POINT_2F pPointD2D = D2D1::Point2F(pPixelObject.xPosition, pPixelObject.yPosition);
		pPointArrayToDraw.push_back(pPointD2D);
	}


	ID2D1PathGeometry* pPathGeometry = nullptr;
	m_pD2DFactory->CreatePathGeometry(&pPathGeometry);

	ID2D1GeometrySink* pGeometrySink = nullptr;
	pPathGeometry->Open(&pGeometrySink);
	D2D1_POINT_2F* pPointArray = &pPointArrayToDraw[0];

	pGeometrySink->AddLines(pPointArray, pPointArrayToDraw.size());

	pGeometrySink->Close();


	if (BeginPaint(hWnd, &ps))
	{
		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}

        
		D2D1_BITMAP_BRUSH_PROPERTIES1 props = D2D1::BitmapBrushProperties1();
		ID2D1Bitmap* pBrushToDraw = nullptr;


       
		hr = m_pD2DDeviceContext1->CreateBitmapBrush(pBrushToFill,
			&pBitmapBrush);

		pBitmapBrush->QueryInterface(__uuidof(pBrushToDraw), reinterpret_cast<void**>(&pBrushToDraw));


		m_pD2DDeviceContext1->BeginDraw();

		m_pD2DDeviceContext1->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::Black));

		D2D1_SIZE_F rtSize = m_pD2DDeviceContext->GetSize();
		D2D1_RECT_F rectangle = D2D1::RectF(0, 0, rtSize.width, rtSize.height);

		
		
		m_pD2DDeviceContext1->DrawGeometry(pPathGeometry, pBitmapBrush);

		if (hr == D2DERR_RECREATE_TARGET)
		{
			SafeReleaseA(&m_pD2DBitmap);
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}
		pSwapChainObjectD2D->Present(0, 0);
		EndPaint(hWnd, &ps);
	    
	}
	return SUCCEEDED(hr) ? 0 : 1;
}





void Direct2DManagementClass::ValidateD3D11Device(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr = S_OK;
	IDXGIDevice1* pD2DGraphicsDevice;
	IDXGIAdapter* pDXGIAdapter = nullptr;
	hr = md3ddevice->QueryInterface(__uuidof(pD2DGraphicsDevice), reinterpret_cast<void**>(&pD2DGraphicsDevice));
	
	if (FAILED(hr))
	{
		return;
	}
	
	hr = pD2DGraphicsDevice->GetAdapter(&pDXGIAdapter);
	
	if (FAILED(hr))
	{
		return;
	}

	DXGI_ADAPTER_DESC deviceDesc;
	hr = pDXGIAdapter->GetDesc(&deviceDesc);
	if (FAILED(hr))
	{
		return;
	}

	IDXGIFactory2* dxgiFactory;
	IDXGIAdapter1* currentAdapter;
	DXGI_ADAPTER_DESC currentDesc;
	
	hr = CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)(&pDXGIFactory));
	if (FAILED(hr))
	{
		return;
	}

	hr = pDXGIFactory->QueryInterface(__uuidof(dxgiFactory), (void**)&dxgiFactory);
	if (FAILED(hr))
	{
		return;
	}


	hr = dxgiFactory->EnumAdapters1(0, &currentAdapter);
	if (FAILED(hr))
	{
		return;
	}
	hr = currentAdapter->GetDesc(&currentDesc);
	if (FAILED(hr))
	{
		return;
	}
	if (deviceDesc.AdapterLuid.LowPart != currentDesc.AdapterLuid.LowPart ||
		deviceDesc.AdapterLuid.HighPart != currentDesc.AdapterLuid.HighPart ||
		FAILED(md3ddevice->GetDeviceRemovedReason()))
	{
		m_pDXGIDevice = nullptr;
		pDXGIAdapter = nullptr;

		HandleDeviceLost();
	}
}

void Direct2DManagementClass::HandleDeviceLost()
{
	float m_dpi = -1.0f;
	float dpi = m_dpi;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	HWND hWnd = NULL;
	GetClientRect(hWnd, &rc);
	SetDpi(dpi);
}

void Direct2DManagementClass::UpdateForWindowSizeChange()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	HWND hWnd = NULL;
	GetClientRect(hWnd, &rc);

	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	FLOAT dpiX, dpiY;
	HDC screen = GetDC(0);
	dpiX = static_cast<FLOAT>(GetDeviceCaps(screen, LOGPIXELSX));
	dpiY = static_cast<FLOAT>(GetDeviceCaps(screen, LOGPIXELSY));
	ReleaseDC(0, screen);
	

	if (width != dpiX ||
		height != dpiY)
	{
		ID3D11RenderTargetView* nullViews[] = { nullptr };
		md3dcontext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
		m_pD2DDeviceContext->SetTarget(nullptr);
		pBitmapRenderTarget = nullptr;
		backBufferTarget = nullptr;
		pDepthView = nullptr;
		md3dcontext->Flush();
		CreateWindowSizeDependentResources(hInstance, hWnd);
	}


}





HRESULT Direct2DManagementClass::CreateWindowSizeDependentResources(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr = S_OK;
	hInstance = GetModuleHandle(NULL);
	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	float pWidth = rc.right - rc.left;
	float pHeight = rc.bottom - rc.top;
	FLOAT dpiX = 0;
	FLOAT dpiY = 0;
	m_pD2DDeviceContext1->GetDpi(&dpiX, &dpiY);
	ID3D11Texture2D* backBufferTexture2D = nullptr;
	IDXGIDevice1* dxgiDevice = nullptr;
	hr = pD3DGraphicsDevice->QueryInterface(__uuidof(pDXGIDevice), (void**)&pDXGIDevice);
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pDXGIAdapter);




	backBufferTexture2D = nullptr;
	hr = pSwapChainObjectD2D->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture2D);

	
	// After the render target view is created, specify that the viewport,
	// which describes what portion of the window to draw to, should cover
	// the entire window.

	D3D11_TEXTURE2D_DESC backBufferDesc = { 0 };
	backBufferTexture2D->GetDesc(&backBufferDesc);
	UINT width = backBufferDesc.Width;
	UINT height = backBufferDesc.Height;

	if (pSwapChainObjectD2D != NULL)
	{
		hr = pSwapChainObjectD2D->ResizeBuffers(2,
			static_cast<UINT>(width),
			static_cast<UINT>(height),
			DXGI_FORMAT_B8G8R8A8_UNORM,
			0);

		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			HandleDeviceLost();
			return hr;
		}
		else
		{
			hr = E_FAIL;
			return hr;
		}
	}
	else
	{

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc1;
		ZeroMemory(&swapChainDesc1, sizeof(swapChainDesc1));
		swapChainDesc1.BufferCount = 1;
		swapChainDesc1.Width = width;
		swapChainDesc1.Height = height;
		swapChainDesc1.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS | DXGI_USAGE_SHADER_INPUT;
		swapChainDesc1.SampleDesc.Count = 1;
		swapChainDesc1.SampleDesc.Quality = 0;
		swapChainDesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc;
		fullScreenDesc.Windowed = TRUE;
		fullScreenDesc.RefreshRate.Numerator = 1;
		fullScreenDesc.RefreshRate.Denominator = 0;
		fullScreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;



		hr = pD3DGraphicsContext->QueryInterface(__uuidof(pDXGIDevice), (void**)&pDXGIDevice);
		
		if (FAILED(hr))
		{
			return hr;
		}

	


		hr = pDXGIDevice->GetAdapter(&pDXGIAdapter);

		if (FAILED(hr))
		{
			return hr;
		}

		hr = pDXGIAdapter->GetParent(__uuidof(m_pDXGIFactory2), (void**)&m_pDXGIFactory2);

		hr = m_pDXGIFactory2->CreateSwapChainForHwnd(mdxgidevice, hWnd, &swapChainDesc,
			nullptr, nullptr, &pSwapChainObjectD2D);

		dxgiDevice->SetMaximumFrameLatency(1);
		if (FAILED(hr))
		{
			return hr;
		}

		DXGI_MODE_ROTATION rotation = DXGI_MODE_ROTATION_UNSPECIFIED;
		

		// Create a Direct3D render target view of the swap chain back buffer.

		hr = pSwapChainObjectD2D->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture2D);
		if (FAILED(hr))
		{
			return hr;
		}

	
		hr = pD3DGraphicsDevice->CreateRenderTargetView(
			backBufferTexture2D,
			nullptr,
			&backBufferTargetObject
			);

		if (FAILED(hr))
		{
			return hr;
		}

		// Create a depth stencil view for use with 3D rendering if needed.
		CD3D11_TEXTURE2D_DESC depthStencilDesc(
			DXGI_FORMAT_D24_UNORM_S8_UINT,
			static_cast<UINT>(width),
			static_cast<UINT>(height),
			1,
			1,
			D3D11_BIND_DEPTH_STENCIL
			);

		ID3D11Texture2D* depthStencil = nullptr;
		hr = pD3DGraphicsDevice->CreateTexture2D(
			&depthStencilDesc,
			nullptr,
			&depthStencil
			);
		
		if (FAILED(hr))
		{
			return hr;
		}

		CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
		hr = pD3DGraphicsDevice->CreateDepthStencilView(
			depthStencil,
			&depthStencilViewDesc,
			&pDepthView
			);

		if (FAILED(hr))
		{
			return hr;
		}

		// Set the 3D rendering viewport to target the entire window.
		CD3D11_VIEWPORT viewport(
			0.0f,
			0.0f,
			width,
			height
			);

	pD3DGraphicsContext->RSSetViewports(1, &viewport);

		// Create a Direct2D target bitmap associated with the
		// swap chain back buffer and set it as the current target.
		D2D1_BITMAP_PROPERTIES1 bitmapProperties =
			D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpiX,
			dpiX
			);

		IDXGISurface* dxgiBackBuffer = nullptr;
		hr = pSwapChainObjectD2D->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));

		if (FAILED(hr))
		{
			return hr;
		}

		hr = m_pD2DDeviceContext1->CreateBitmapFromDxgiSurface(dxgiBackBuffer,
			&bitmapProperties,
			&pBitmapRenderTarget);
		

		m_pD2DDeviceContext1->SetTarget(pBitmapRenderTarget);

		// Grayscale text anti-aliasing is recommended for all Windows Store apps.
		m_pD2DDeviceContext1->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
		
		return hr;

	}

}

void Direct2DManagementClass::SetDpi(float dpiX)
{

}

// Method to deliver the final image to the display.
void Direct2DManagementClass::Present()
{
	HRESULT hr = S_OK;
	// The application may optionally specify "dirty" or "scroll"
	// rects to improve efficiency in certain scenarios.
	DXGI_PRESENT_PARAMETERS parameters = { 0 };
	parameters.DirtyRectsCount = 0;
	parameters.pDirtyRects = nullptr;
	parameters.pScrollRect = nullptr;
	parameters.pScrollOffset = nullptr;

	// The first argument instructs DXGI to block until VSync, putting the application
// to sleep until the next VSync. This ensures we don't waste any cycles rendering
// frames that will never be displayed to the screen.
hr = pSwapChainObjectD2D->Present(1, 0);

// Discard the contents of the render target.
// This is a valid operation only when the existing contents will be entirely
// overwritten. If dirty or scroll rects are used, this call should be removed.
md3dcontext1->DiscardView(backBufferTargetObject);

// Discard the contents of the depth stencil.
md3dcontext1->DiscardView(pDepthView);

// If the device was removed either by a disconnect or a driver upgrade, we 
// must recreate all device resources.
if (hr == DXGI_ERROR_DEVICE_REMOVED)
{
	HandleDeviceLost();
}

}


HRESULT Direct2DManagementClass::PlotAGraphGeometryForGivenQuadraticData(HINSTANCE hInstance, HWND hWnd, vector<OutputStructureForGraph2D> pPixelDataArray)
{
	HRESULT hr = S_OK;
	std::cout << "This Function Will be Used to Produce the Graphical Representation for Our Pixel Data Set" << endl;

	fstream pixelDataFileFinal;
	pixelDataFileFinal.open("C:\\pixels\\PathGeometryDataFileFinal.txt", std::fstream::in | std::fstream::out | std::fstream::
		trunc);
	pixelDataFileFinal.clear();
	pixelDataFileFinal << " " << endl;
	pixelDataFileFinal << " " << endl;
	pixelDataFileFinal << "New Operation Begin" << endl;
	time_t currentTime = time(0);
	pixelDataFileFinal << "current time" << currentTime << endl;

	for (OutputStructureForGraph2D pOutputStructure : pPixelDataArray)
	{
		std::cout << "Item Index = " << pOutputStructure.pItemIndex << endl;
		std::cout << "Input Function Value =" << pOutputStructure.pInputStructureForGraph << endl;
		std::cout << "Output Function Value = " << pOutputStructure.pOutputStructureForGraph << endl;

		pixelDataFileFinal << "Item Index = " << pOutputStructure.pItemIndex << endl;
		pixelDataFileFinal << "Input Function Value =" << pOutputStructure.pInputStructureForGraph << endl;
		pixelDataFileFinal << "Output Function Value = " << pOutputStructure.pOutputStructureForGraph << endl;
	}

	std::cout << "Creating the Instances for Graphics Drawing Factories" << endl;
	pixelDataFileFinal << "Creating the Instances for Graphics Drawing Factories" << endl;



	std::cout << "Preparing the Devices For Graph Drawing Operation" << endl;
	std::cout << "We Will Need the Following " << endl;
	std::cout << "1 Factory Object " << endl;
	std::cout << "1 Geometry Sink Object" << endl;
	std::cout << "1 Path Geometry Object" << endl;
	std::cout << "1 White Solid Color Brush Object For BackGround " << endl;
	std::cout << "1 White Solid Color Brush Object" << endl;

	pixelDataFileFinal << "Preparing the Devices For Graph Drawing Operation" << endl;
	pixelDataFileFinal << "We Will Need the Following " << endl;
	pixelDataFileFinal << "1 Factory Object " << endl;
	pixelDataFileFinal << "1 Geometry Sink Object" << endl;
	pixelDataFileFinal << "1 Path Geometry Object" << endl;
	pixelDataFileFinal << "1 White Solid Color Brush Object For BackGround " << endl;
	pixelDataFileFinal << "1 White Solid Color Brush Object" << endl;

	ID2D1GeometrySink* pGeometrySinkObject = nullptr;
	ID2D1PathGeometry* pPathGeomtryObject = nullptr;
	ID2D1SolidColorBrush* pBlackBrushObject = nullptr;
	ID2D1SolidColorBrush* pWhiteBrushObject = nullptr;

	std::cout << "Generating Path Geometry Object Resources" << endl;
	pixelDataFileFinal << "Generating Path Geometry Object Resources" << endl;

	hr = GeneratePathGeometryForQuadraticFunctuinData(hInstance,
		hWnd, pPixelDataArray, m_pD2DFactory,
		&pGeometrySinkObject, &pPathGeomtryObject, &pBlackBrushObject, &pWhiteBrushObject);

	std::cout << "Rendering the Geometry Data" << endl;
	pixelDataFileFinal << "Rendering the Geometry Data" << endl;


	if (SUCCEEDED(hr))
	{
		hr = RenderGraphGeometryAlongThePlane(hInstance,
			hWnd, 20, pWhiteBrushObject, pBlackBrushObject, pPathGeomtryObject);
	}

	SafeReleaseA(&pGeometrySinkObject);
	SafeReleaseA(&pPathGeomtryObject);
	SafeReleaseA(&pBlackBrushObject);
	SafeReleaseA(&pWhiteBrushObject);

	return hr;
}

HRESULT Direct2DManagementClass::DrawPathGeometryForGivenDataSetOfPixels(HINSTANCE hInstance, HWND hWnd, vector<PixelObject> pPixelObjectArray)
{
	HRESULT hr = S_OK;
	PAINTSTRUCT ps;
	PixelObject pPixelObject = pPixelObjectArray[0];
	float pXPosition0 = 0.0f;
	float pYPosition0 = 0.0f;
	float pXPositionConst = 0.0f;
	float pYPositionConst = 0.0f;

	HDC hDC = NULL;
	BeginPaint(hWnd, &ps);
	ReleaseDC(hWnd, hDC);
	hDC = GetDC(hWnd);
	HDC src = CreateCompatibleDC(hDC);
	HGDIOBJ hPen = GetStockObject(BLACK_PEN);
	HGDIOBJ hOldPen = SelectObject(hDC, hPen);

	
	PixelObject pPixelObjectFirst = pPixelObjectArray[0];
	PixelObject pPixelObjectLast = pPixelObjectArray[pPixelObjectArray.size() - 1];

	pXPosition0 = pPixelObjectFirst.xPosition;
	pYPosition0 = pPixelObjectFirst.yPosition;
	pXPositionConst = pXPosition0;
	pYPositionConst = pYPosition0;

	ID2D1GeometrySink* pSinkObject = nullptr;
	ID2D1PathGeometry* pGeometryObject = nullptr;
	ID2D1SolidColorBrush* pBlackBrush = nullptr;
	ID2D1SolidColorBrush* pWhiteBrush = nullptr;

	hr = GeneratePathGeometryForGivenRGBPixels(hInstance,
		hWnd, pPixelObjectArray, m_pD2DFactory,
		&pSinkObject, &pGeometryObject,
		&pBlackBrush, &pWhiteBrush);

	if (SUCCEEDED(hr))
	{
		hr = RenderGraphGeometryAlongThePlane(hInstance, hWnd, 10.0f, pWhiteBrush, pBlackBrush, pGeometryObject);
	}
	return hr;

}
	


HRESULT Direct2DManagementClass::DrawPathGeometryForGivenDataSet(HINSTANCE hInstance, HWND hWnd,
	float pCentreX, float pCentreY,
	 vector<PixelObject> pPixelObjectArray)
{
	HRESULT hr = S_OK;
	PAINTSTRUCT ps;
	PixelObject pPixelObject = pPixelObjectArray[0];
	float pXPosition0 = 0.0f;
	float pYPosition0 = 0.0f;
	float pXPositionConst = 0.0f;
	float pYPositionConst = 0.0f;

	HDC hDC = NULL;
	BeginPaint(hWnd, &ps);
	ReleaseDC(hWnd, hDC);
	hDC = GetDC(hWnd);
	HDC src = CreateCompatibleDC(hDC);
	HGDIOBJ hPen = GetStockObject(BLACK_PEN);
	HGDIOBJ hOldPen = SelectObject(hDC, hPen);

	


    hDC = BeginPaint(hWnd, &ps);
	
	vector<POINT> pPointGraphics = vector<POINT>();
	for (PixelObject pixelObject : pPixelObjectArray)
	{
		POINT pPointObject;
		pPointObject.x = pixelObject.xPosition;
		pPointObject.y = pixelObject.yPosition;
		pPointGraphics.push_back(pPointObject);
	}

	POINT pPointOne = pPointGraphics[0];
	BeginPath(hDC);
	for (POINT pNewPoint : pPointGraphics)
	{
		LineTo(hDC, pNewPoint.x, pNewPoint.y);
		MoveToEx(hDC, pNewPoint.x, pNewPoint.y, &pNewPoint);
	}
	EndPath(hDC);
	
	EndPaint(hWnd, &ps);


	
	return hr;
}



HRESULT  Direct2DManagementClass::DrawPathGeometryForGivenRGBPixels(HINSTANCE hInstance,
	HWND hWnd,
	vector<PixelObject> ppixelDataArray)
{
	HRESULT hr = S_OK;
	hr = CreateDeviceResources(hInstance, hWnd);


	ID2D1PathGeometry* pPathGeometryObject = nullptr;
	ID2D1GeometrySink* pGeometrySinkObject = nullptr;
	vector<D2D1_POINT_2F> pVectorOfPoints = vector<D2D1_POINT_2F>();
	fstream pixelDataFileSetup;
	pixelDataFileSetup.open("C:\\pixels\\PathGeometryDataFileSetup.txt", std::fstream::in | std::fstream::out | std::fstream::
		trunc);
	pixelDataFileSetup.clear();
	pixelDataFileSetup << " " << endl;
	pixelDataFileSetup << " " << endl;
	pixelDataFileSetup << "New Operation Begin" << endl;
	time_t currentTime = time(0);
	pixelDataFileSetup << "current time" << currentTime << endl;

	HMONITOR monitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFO info;
	info.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(monitor, &info);
	int monitorWidth = info.rcMonitor.right - info.rcMonitor.left;
	int monitorHeight = info.rcMonitor.bottom - info.rcMonitor.top;
	int pXPosition0 = (monitorWidth / 2) - 400;
	int pYPosition0 = monitorHeight / 4;


	int pXPosition1 = (monitorWidth / 2) + 200;
	int pYPosition1 = monitorHeight / 4;




	int pImageWidth = 90;
	int pImageHeight = 100;


	std::cout << "X Position Rect 0: " << pXPosition0 << endl;
	std::cout << "Y Position Rect 0: " << pYPosition0 << endl;


	std::cout << "X Position Rect 1: " << pXPosition1 << endl;
	std::cout << "Y Position Rect 1: " << pYPosition1 << endl;



		if (m_pD2DDeviceContext1 == nullptr)
		{
			hr = CreateDeviceResources(hInstance, hWnd);
		}


		ID2D1BitmapBrush1* pBitmapBrush = nullptr;
		LPCSTR pFileName = nullptr;

		static const WCHAR pText0[] = L"Image State No Change";
		static const WCHAR pText1[] = L"Unknown State ";
		static const WCHAR pText2[] = L"Image State Changed ";

		static const UINT stringLength0 = ARRAYSIZE(pText0) - 1;
		static const UINT stringLength1 = ARRAYSIZE(pText1) - 1;
		static const UINT stringLength2 = ARRAYSIZE(pText2) - 1;

		ID2D1Factory* pFactoryObject = nullptr;
		pFactoryObject = m_pD2DFactory;



		std::cout << "Setting Text Layout Information 0" << endl;
		IDWriteTextLayout* pTextLayoutInformation0 = nullptr;
		IDWriteTextLayout* pTextLayoutInformation1 = nullptr;
		IDWriteTextLayout* pTextLayoutInformation2 = nullptr;



		IDWriteTextFormat* pTextFormat = nullptr;
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			DWRITE_FONT_STYLE_ITALIC,
			DWRITE_FONT_STRETCH_NORMAL,
			18,
			L"",
			&pTextFormat);


		hr = m_pDWriteFactory->CreateTextLayout(
			pText0,
			stringLength0,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation0
			);



		hr = m_pDWriteFactory->CreateTextLayout(
			pText1,
			stringLength0,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation1
			);



		hr = m_pDWriteFactory->CreateTextLayout(
			pText2,
			stringLength0,
			pTextFormat,
			40, // maxWidth
			40, // maxHeight
			&pTextLayoutInformation2
			);

		std::cout << "Setting Text Layout Information 0" << endl;
		pTextLayoutInformation0->SetMaxHeight(40);
		pTextLayoutInformation0->SetMaxWidth(40);
		pTextLayoutInformation0->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTextLayoutInformation0->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);


		std::cout << "This Function will be used to create path geometry for a given data" << endl;
		std::cout << "First We have to ensure the all are devices are set up" << endl;
		pixelDataFileSetup << "This Function will be used to create path geometry for a given data" << endl;
		pixelDataFileSetup << "First We have to ensure the all are devices are set up" << endl;

		std::cout << "Creating the Black Color Brush" << endl;
		ID2D1SolidColorBrush* pBlackColorBrush = nullptr;
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackColorBrush);

		std::cout << "Creating the White Coler Brush" << endl;
		ID2D1SolidColorBrush* pWhiteColorBrush = nullptr;
		hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pWhiteColorBrush);



		std::cout << "Creating the Path Geometry Objects" << endl;
		pixelDataFileSetup << "Creating the Path Geometry Objects" << endl;

		hr = pFactoryObject->CreatePathGeometry(&pPathGeometryObject);

		if (FAILED(hr))
		{
			int pLastError = GetLastError();
			std::cout << "Unable to Create the Path Geometry Object" << pLastError << endl;
			pixelDataFileSetup << "Unable to Create the Path Geometry Object" << pLastError << endl;
			return hr;
		}
		std::cout << "Opening the Geometry Sink Object" << endl;
		hr = pPathGeometryObject->Open(&pGeometrySinkObject);

		if (FAILED(hr))
		{
			int pLastError = GetLastError();
			std::cout << "Unable to Open the Geometry Sync Objects" << pLastError << endl;
			pixelDataFileSetup << "Unable to Create the Path Geometry Object" << pLastError << endl;
			return hr;
		}
		std::cout << "Now We will Have To Output The Data for our Graph Plot" << endl;
		std::cout << "This Function Will Start at The Beginning of the Vector Element" << endl;
		std::cout << "Firs We have to Initialize Figure Creatinon" << endl;
		std::cout << "Our Graph Will start the first pixel elements" << endl;

		pixelDataFileSetup << "Now We will Have To Output The Data for our Graph Plot" << endl;
		pixelDataFileSetup << "This Function Will Start at The Beginning of the Vector Element" << endl;
		pixelDataFileSetup << "Firs We have to Initialize Figure Creatinon" << endl;
		pixelDataFileSetup << "Our Graph Will start the first pixel elements" << endl;


		pXPosition0 = 0.0f;
		float pIntensityValue = 0.0f;
		std::cout << "Locating the Pixel Intensity Value at X = 0, Y = 0" << endl;
		pixelDataFileSetup << "Locating the Pixel Intensity Value at X = 0, Y = 0" << endl;


		D2D1_POINT_2F beginPoint = D2D1::Point2F(ppixelDataArray[0].xPosition, ppixelDataArray[0].yPosition);
		std::cout << "First Graph Point X Position: " << ppixelDataArray[0].xPosition << endl;
		pixelDataFileSetup << "First Graph Point Intensity Value: " << ppixelDataArray[0].xPosition << endl;

		pGeometrySinkObject->BeginFigure(beginPoint, D2D1_FIGURE_BEGIN_HOLLOW);
		pGeometrySinkObject->SetFillMode(D2D1_FILL_MODE::D2D1_FILL_MODE_ALTERNATE);
		float pIndexValue = 0.0f;
		float pXValue = 0.0f;
		float pYValue = 0.0f;
		float pHueValue = 0.0f;
		float pSaturationValue = 0.0f;
	
		vector<D2D1_POINT_2F> pPointVector = vector<D2D1_POINT_2F>();
		for (PixelObject pPixelObjectHSI : ppixelDataArray)
		{
			float pIndexValue = pPixelObjectHSI.pixelNumber;
			float pXValue = pPixelObjectHSI.xPosition;
			float pYValue = pPixelObjectHSI.yPosition;
			float pHueValue = pPixelObjectHSI.RMask;
			float pSaturationValue = pPixelObjectHSI.GMask;
			float pIntensityValue = pPixelObjectHSI.BMask;

			std::cout << "X Value = " << pXValue << endl;
			std::cout << "Y Value = " << pYValue << endl;
			std::cout << "Red Value = " << pHueValue << endl;
			std::cout << "Green Value = " << pSaturationValue << endl;
			std::cout << "Blue Value = " << pIntensityValue << endl;

			pixelDataFileSetup << "Step: " << pIndexValue << endl;
			pixelDataFileSetup << "X Value = " << pXValue << endl;
			pixelDataFileSetup << "Y Value = " << pYValue << endl;
			pixelDataFileSetup << "Red Value = " << pHueValue << endl;
			pixelDataFileSetup << "Green Value = " << pSaturationValue << endl;
			pixelDataFileSetup << "Blue Value = " << pIntensityValue << endl;

			D2D1_POINT_2F pNewPoint = D2D1::Point2F(pXValue * 20, pYValue * 20);
			pPointVector.push_back(pNewPoint);

		}


			pGeometrySinkObject->SetFillMode(D2D1_FILL_MODE::D2D1_FILL_MODE_ALTERNATE);
			// Set the start point and specify the figure is to be filled
			
			int count = pPointVector.size();
			// Add the other points


			for (D2D1_POINT_2F pPointVar : pPointVector)
			{
				pGeometrySinkObject->AddLine(pPointVar);
			}
			
		
		pGeometrySinkObject->EndFigure(D2D1_FIGURE_END_CLOSED);

		pGeometrySinkObject->Close();

		ID2D1SolidColorBrush* pRedColorBrush = nullptr;
		ID2D1SolidColorBrush* pGreenColorBrush = nullptr;

		std::cout << "Creating Black Brush" << endl;
		m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackColorBrush);

		std::cout << "Creating Green Brush" << endl;
		m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenColorBrush);

		std::cout << "Creating Red Brush Brush" << endl;
		m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedColorBrush);


		BeginPaint(hWnd, &ps);

		m_pD2DDeviceContext1->BeginDraw();

		D2D1_POINT_2F point;
		D2D1_POINT_2F tangent;
		D2D1_MATRIX_3X2_F triangleMatrix;
		D2D1_SIZE_F rtSize = m_pD2DDeviceContext1->GetSize();
		float minWidthHeightScale = min(rtSize.width, rtSize.height) / 512;

		D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(
			minWidthHeightScale,
			minWidthHeightScale
			);

		D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation(
			rtSize.width / 2,
			rtSize.height / 2
			);


		m_pD2DDeviceContext1->SetTransform(D2D1::Matrix3x2F::Identity());

		// Reset to identity transform

		//clear the render target contents
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));

		//center the path
		m_pD2DDeviceContext1->SetTransform(scale * translation);
		
		//draw the path in red
		m_pD2DDeviceContext1->DrawGeometry(pPathGeometryObject, pBlackColorBrush);
		//draw the path in red
		m_pD2DDeviceContext1->FillGeometry(pPathGeometryObject, pBlackColorBrush);

		static float float_time = 0.0f;
		m_pD2DDeviceContext1->EndDraw();
	
		
		pSwapChainObjectD2D->Present(0, 0);

		EndPaint(hWnd, &ps);

		return hr;

	}
	



HRESULT Direct2DManagementClass::CreateBMPFile(HINSTANCE hInstance, HWND hWnd, LPTSTR pFileName, PBITMAPINFO pBitmapInfoObject, HBITMAP pBitmapObject, HDC hDC)
{
	HRESULT hr = S_OK;

	HANDLE hf;                 // file handle  
	BITMAPFILEHEADER hdr;       // bitmap file-header  
	PBITMAPINFOHEADER pbih;     // bitmap info-header  
	LPBYTE lpBits;              // memory pointer  
	DWORD dwTotal;              // total count of bytes  
	DWORD cb;                   // incremental count of bytes  
	BYTE *hp;                   // byte pointer  
	DWORD dwTmp;
	
	pbih = (PBITMAPINFOHEADER)pBitmapInfoObject;
	lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

	if (!lpBits)
	{
		std::cout << "GLOBAL ALLOC FAILED" << endl;
	}

	// Retrieve the color table (RGBQUAD array) and the bits  
	// (array of palette indices) from the DIB.  
	if (!GetDIBits(hDC, pBitmapObject, 0, (WORD)pbih->biHeight, lpBits, pBitmapInfoObject,
		DIB_RGB_COLORS))
	{
		std::cout << "DIB FAILED" << endl;
	}

	// Create the .BMP file.  
	hf = CreateFile(pFileName,
		GENERIC_READ | GENERIC_WRITE,
		(DWORD)0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		(HANDLE)NULL);
	if (hf == INVALID_HANDLE_VALUE)
		return hr;
	hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"  
	// Compute the size of the entire file.  
	hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
		pbih->biSize + pbih->biClrUsed
		* sizeof(RGBQUAD) + pbih->biSizeImage);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;

	// Compute the offset to the array of color indices.  
	hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +
		pbih->biSize + pbih->biClrUsed
		* sizeof(RGBQUAD);

	// Copy the BITMAPFILEHEADER into the .BMP file.  
	if (!WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER),
		(LPDWORD)&dwTmp, NULL))
	{
		return hr;
	}

	// Copy the BITMAPINFOHEADER and RGBQUAD array into the file.  
	if (!WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER)
		+ pbih->biClrUsed * sizeof(RGBQUAD),
		(LPDWORD)&dwTmp, (NULL)))
	{
		return hr;
	}

	// Copy the array of color indices into the .BMP file.  
	dwTotal = cb = pbih->biSizeImage;
	hp = lpBits;
	if (!WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL))
	{
		return hr;
	}

	// Close the .BMP file.  
	if (!CloseHandle(hf))
	{
		return hr;
	}

	// Free memory.  
	GlobalFree((HGLOBAL)lpBits);
}



PBITMAPINFO Direct2DManagementClass::CreateBitmapInfoStructure(HINSTANCE hInstance, HWND hWnd, HBITMAP hBmp)
{
	BITMAP pBitmapINfoStructure;
	WORD cClrBits;
	PBITMAPINFO pBitmapInfo;
	int pResult = 0;
	pResult = GetObject(hBmp, sizeof(BITMAP), (LPSTR)&pBitmapINfoStructure);

	cClrBits = (WORD)(pBitmapINfoStructure.bmPlanes + pBitmapINfoStructure.bmBitsPixel);

	if (cClrBits == 1)
	{
		cClrBits = 1;
	}
	else if (cClrBits <= 4)
	{
		cClrBits = 4;
	}
	else if (cClrBits <= 8)
	{
		cClrBits = 8;
	}
	else if (cClrBits <= 16)
	{
		cClrBits = 16;
	}
	else if (cClrBits <= 24)
	{
		cClrBits = 24;
	}
	else
	{
		cClrBits = 32;
	}

	if (cClrBits < 24)
	{
		pBitmapInfo = (PBITMAPINFO)LocalAlloc(LPTR,
			sizeof(BITMAPINFOHEADER) +
			sizeof(RGBQUAD) * (1 << cClrBits));
	}

	else
	{
		pBitmapInfo = (PBITMAPINFO)LocalAlloc(LPTR,
			sizeof(BITMAPINFOHEADER));
	}


	pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pBitmapInfo->bmiHeader.biWidth = pBitmapINfoStructure.bmWidth;
	pBitmapInfo->bmiHeader.biHeight = pBitmapINfoStructure.bmHeight;
	pBitmapInfo->bmiHeader.biPlanes = pBitmapINfoStructure.bmPlanes;
	pBitmapInfo->bmiHeader.biBitCount = pBitmapINfoStructure.bmBitsPixel;
	if (cClrBits < 24)
		pBitmapInfo->bmiHeader.biClrUsed = (1 << cClrBits);

	// If the bitmap is not compressed, set the BI_RGB flag.  
	pBitmapInfo->bmiHeader.biCompression = BI_RGB;

	// Compute the number of bytes in the array of color  
	// indices and store the result in biSizeImage.  
	// The width must be DWORD aligned unless the bitmap is RLE 
	// compressed. 
	pBitmapInfo->bmiHeader.biSizeImage = ((pBitmapInfo->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8
		* pBitmapInfo->bmiHeader.biHeight;
	// Set biClrImportant to 0, indicating that all of the  
	// device colors are important.  
	pBitmapInfo->bmiHeader.biClrImportant = 0;
	return pBitmapInfo;
}


HRESULT  Direct2DManagementClass::GeneratePathGeometryForGivenIntensityDataAlongX(HINSTANCE hInstance,
	HWND hWnd,
	vector<PixelObjectHSI> ppixelDataArray,
	ID2D1Factory* pFactoryObject,
	ID2D1GeometrySink** pGeometrySink,
	ID2D1PathGeometry** pGraphicsPathGeometry,
	ID2D1SolidColorBrush** pSolidColorBrushBlack,
	ID2D1SolidColorBrush** pSolidColorBrushWhite)
{
	HRESULT hr = S_OK;
	ID2D1PathGeometry* pPathGeometryObject = nullptr;
	ID2D1GeometrySink* pGeometrySinkObject = nullptr;

	fstream pixelDataFileSetup;
	pixelDataFileSetup.open("C:\\pixels\\PathGeometryDataFileSetup.txt", std::fstream::in | std::fstream::out | std::fstream::
		trunc);
	pixelDataFileSetup.clear();
	pixelDataFileSetup << " " << endl;
	pixelDataFileSetup << " " << endl;
	pixelDataFileSetup << "New Operation Begin" << endl;
	time_t currentTime = time(0);
	pixelDataFileSetup << "current time" << currentTime << endl;


	std::cout << "This Function will be used to create path geometry for a given data" << endl;
	std::cout << "First We have to ensure the all are devices are set up" << endl;
	pixelDataFileSetup << "This Function will be used to create path geometry for a given data" << endl;
	pixelDataFileSetup << "First We have to ensure the all are devices are set up" << endl;

	std::cout << "Creating the Black Color Brush" << endl;
	ID2D1SolidColorBrush* pBlackColorBrush = nullptr;
	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackColorBrush);

	std::cout << "Creating the White Coler Brush" << endl;
	ID2D1SolidColorBrush* pWhiteColorBrush = nullptr;
	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pWhiteColorBrush);


	pFactoryObject = m_pD2DFactory;


	std::cout << "Creating the Path Geometry Objects" << endl;
	pixelDataFileSetup << "Creating the Path Geometry Objects" << endl;

	hr = pFactoryObject->CreatePathGeometry(&pPathGeometryObject);

	if (FAILED(hr))
	{
		int pLastError = GetLastError();
		std::cout << "Unable to Create the Path Geometry Object" << pLastError << endl;
		pixelDataFileSetup << "Unable to Create the Path Geometry Object" << pLastError << endl;
		return hr;
	}
	std::cout << "Opening the Geometry Sink Object" << endl;
	hr = pPathGeometryObject->Open(&pGeometrySinkObject);

	if (FAILED(hr))
	{
		int pLastError = GetLastError();
		std::cout << "Unable to Open the Geometry Sync Objects" << pLastError << endl;
		pixelDataFileSetup << "Unable to Create the Path Geometry Object" << pLastError << endl;
		return hr;
	}
	std::cout << "Now We will Have To Output The Data for our Graph Plot" << endl;
	std::cout << "This Function Will Start at The Beginning of the Vector Element" << endl;
	std::cout << "Firs We have to Initialize Figure Creatinon" << endl;
	std::cout << "Our Graph Will start the first pixel elements" << endl;

	pixelDataFileSetup << "Now We will Have To Output The Data for our Graph Plot" << endl;
	pixelDataFileSetup << "This Function Will Start at The Beginning of the Vector Element" << endl;
	pixelDataFileSetup << "Firs We have to Initialize Figure Creatinon" << endl;
	pixelDataFileSetup << "Our Graph Will start the first pixel elements" << endl;


	float pXPosition0 = 0.0f;
	float pIntensityValue = 0.0f;
	std::cout << "Locating the Pixel Intensity Value at X = 0, Y = 0" << endl;
	pixelDataFileSetup << "Locating the Pixel Intensity Value at X = 0, Y = 0" << endl;


	D2D1_POINT_2F beginPoint = D2D1::Point2F(ppixelDataArray[0].pXValue, ppixelDataArray[0].pIntensityValue);
	std::cout << "First Graph Point X Position: " << ppixelDataArray[0].pXValue << endl;
	pixelDataFileSetup << "First Graph Point Intensity Value: " << ppixelDataArray[0].pIntensityValue << endl;

	pGeometrySinkObject->BeginFigure(beginPoint, D2D1_FIGURE_BEGIN_HOLLOW);

	for (PixelObjectHSI pPixelObjectHSI : ppixelDataArray)
	{
		float pIndexValue = pPixelObjectHSI.pixelNumber;
		float pXValue = pPixelObjectHSI.pXValue;
		float pYValue = pPixelObjectHSI.pYValue;
		float pHueValue = pPixelObjectHSI.pHueValue;
		float pSaturationValue = pPixelObjectHSI.pSaturationValue;
		float pIntensityValue = pPixelObjectHSI.pIntensityValue;

		std::cout << "X Value = " << pXValue << endl;
		std::cout << "Y Value = " << pYValue << endl;
		std::cout << "Hue Value = " << pHueValue << endl;
		std::cout << "Saturation Value = " << pSaturationValue << endl;
		std::cout << "Intensity Value = " << pIntensityValue << endl;

		pixelDataFileSetup << "Step: " << pIndexValue << endl;
		pixelDataFileSetup << "X Value = " << pXValue << endl;
		pixelDataFileSetup << "Y Value = " << pYValue << endl;
		pixelDataFileSetup << "Hue Value = " << pHueValue << endl;
		pixelDataFileSetup << "Saturation Value = " << pSaturationValue << endl;
		pixelDataFileSetup << "Intensity Value = " << pIntensityValue << endl;

		D2D1_POINT_2F pGraphPoint = D2D1::Point2F(pXValue, pIntensityValue);
		pGeometrySinkObject->AddLine(pGraphPoint);
	}

	pGeometrySinkObject->EndFigure(D2D1_FIGURE_END_OPEN);
	pGeometrySinkObject->Close();
	pixelDataFileSetup.close();
	*pGraphicsPathGeometry = pPathGeometryObject;
	*pGeometrySink = pGeometrySinkObject;
	*pSolidColorBrushBlack = pBlackColorBrush;
	*pSolidColorBrushWhite = pWhiteColorBrush;

	return hr;
}



HRESULT  Direct2DManagementClass::GeneratePathGeometryForQuadraticFunctuinData(HINSTANCE hInstance,
	HWND hWnd,
	vector<OutputStructureForGraph2D> pDataArrayForGeometry,
	ID2D1Factory* pFactoryObject,
	ID2D1GeometrySink** pGeometrySink,
	ID2D1PathGeometry** pGraphicsPathGeometry,
	ID2D1SolidColorBrush** pSolidColorBrushBlack,
	ID2D1SolidColorBrush** pSolidColorBrushWhite)
{
	HRESULT hr = S_OK;
	ID2D1PathGeometry* pPathGeometryObject = nullptr;
	ID2D1GeometrySink* pGeometrySinkObject = nullptr;

	OutputStructureForGraph2D pBeginningPoint = pDataArrayForGeometry[0];
	D2D1_POINT_2F pStartPoint = D2D1::Point2F(pBeginningPoint.pInputStructureForGraph, pBeginningPoint.pOutputStructureForGraph);
	vector<D2D1_POINT_2F> pPointArrayObject = vector<D2D1_POINT_2F>();



	fstream pixelDataFileSetup;
	pixelDataFileSetup.open("C:\\pixels\\PathGeometryDataFileSetup.txt", std::fstream::in | std::fstream::out | std::fstream::
		trunc);
	pixelDataFileSetup.clear();
	pixelDataFileSetup << " " << endl;
	pixelDataFileSetup << " " << endl;
	pixelDataFileSetup << "New Operation Begin" << endl;
	time_t currentTime = time(0);
	pixelDataFileSetup << "current time" << currentTime << endl;


	std::cout << "This Function will be used to create path geometry for a given data" << endl;
	std::cout << "First We have to ensure the all are devices are set up" << endl;
	pixelDataFileSetup << "This Function will be used to create path geometry for a given data" << endl;
	pixelDataFileSetup << "First We have to ensure the all are devices are set up" << endl;

	std::cout << "Creating the Black Color Brush" << endl;
	ID2D1SolidColorBrush* pBlackColorBrush = nullptr;
	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackColorBrush);

	std::cout << "Creating the White Coler Brush" << endl;
	ID2D1SolidColorBrush* pWhiteColorBrush = nullptr;
	hr = m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pWhiteColorBrush);


	pFactoryObject = m_pD2DFactory;


	std::cout << "Creating the Path Geometry Objects" << endl;
	pixelDataFileSetup << "Creating the Path Geometry Objects" << endl;

	hr = pFactoryObject->CreatePathGeometry(&pPathGeometryObject);

	if (FAILED(hr))
	{
		int pLastError = GetLastError();
		std::cout << "Unable to Create the Path Geometry Object" << pLastError << endl;
		pixelDataFileSetup << "Unable to Create the Path Geometry Object" << pLastError << endl;
		return hr;
	}
	std::cout << "Opening the Geometry Sink Object" << endl;
	hr = pPathGeometryObject->Open(&pGeometrySinkObject);

	if (FAILED(hr))
	{
		int pLastError = GetLastError();
		std::cout << "Unable to Open the Geometry Sync Objects" << pLastError << endl;
		pixelDataFileSetup << "Unable to Create the Path Geometry Object" << pLastError << endl;
		return hr;
	}
	std::cout << "Now We will Have To Output The Data for our Graph Plot" << endl;
	std::cout << "This Function Will Start at The Beginning of the Vector Element" << endl;
	std::cout << "Firs We have to Initialize Figure Creatinon" << endl;
	std::cout << "Our Graph Will start the first pixel elements" << endl;

	pixelDataFileSetup << "Now We will Have To Output The Data for our Graph Plot" << endl;
	pixelDataFileSetup << "This Function Will Start at The Beginning of the Vector Element" << endl;
	pixelDataFileSetup << "Firs We have to Initialize Figure Creatinon" << endl;
	pixelDataFileSetup << "Our Graph Will start the first pixel elements" << endl;


	float pXPosition0 = 0.0f;
	float pIntensityValue = 0.0f;
	std::cout << "Locating the Pixel Intensity Value at X = 0, Y = 0" << endl;
	pixelDataFileSetup << "Locating the Pixel Intensity Value at X = 0, Y = 0" << endl;


	D2D1_POINT_2F beginPoint = D2D1::Point2F(pDataArrayForGeometry[0].pInputStructureForGraph, pDataArrayForGeometry[0].pOutputStructureForGraph);
	std::cout << "First Graph Point X Position: " << pDataArrayForGeometry[0].pInputStructureForGraph << endl;
	pixelDataFileSetup << "First Graph Point Intensity Value: " << pDataArrayForGeometry[0].pOutputStructureForGraph << endl;

	pGeometrySinkObject->BeginFigure(beginPoint, D2D1_FIGURE_BEGIN_HOLLOW);

	for (OutputStructureForGraph2D pGraphObject : pDataArrayForGeometry)
	{
		float pIndexValue = pGraphObject.pItemIndex;
		float pXValue = pGraphObject.pInputStructureForGraph;
		float pYValue = pGraphObject.pOutputStructureForGraph;

		std::cout << "Item Index = " << pIndexValue << endl;
		std::cout << "X Value = " << pXValue << endl;
		std::cout << "Y Value = " << pYValue << endl;

		pixelDataFileSetup << "Item Index = " << pIndexValue << endl;
		pixelDataFileSetup << "X Value = " << pXValue << endl;
		pixelDataFileSetup << "Y Value = " << pYValue << endl;

		D2D1_POINT_2F pGraphPoint = D2D1::Point2F(pXValue, pYValue);
		pPointArrayObject.push_back(pGraphPoint);
	}
	D2D1_POINT_2F* pDataPointArray = &pPointArrayObject[0];
	pGeometrySinkObject->AddLines(pDataPointArray, pDataArrayForGeometry.size());
	pGeometrySinkObject->EndFigure(D2D1_FIGURE_END_CLOSED);
	pGeometrySinkObject->Close();
	pixelDataFileSetup.close();
	*pGraphicsPathGeometry = pPathGeometryObject;
	*pGeometrySink = pGeometrySinkObject;
	*pSolidColorBrushBlack = pBlackColorBrush;
	*pSolidColorBrushWhite = pWhiteColorBrush;

	return hr;
}





HRESULT  Direct2DManagementClass::RenderGraphGeometryQuadAlongThePlane(HINSTANCE hInstance, HWND hWnd,
	float pLineThicknessValue,
	ID2D1SolidColorBrush* pSolidColorBrushObjectBackground,
	ID2D1SolidColorBrush* pSolidColorBrushObjectGraphDrawing,
	ID2D1PathGeometry* pPathGeometryObjectOne,
	ID2D1PathGeometry* pPathGeometryObjectTwo, 
	ID2D1PathGeometry* pPathGeometryObjectThree, 
	ID2D1PathGeometry* pPathGeometryObjectFour)
{


	HRESULT hr = S_OK;
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;

	IWICBitmapLock* pLockObject = nullptr;
	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	ID2D1BitmapBrush* pBitmapBrush = nullptr;
	D2D1_RECT_F clippedArea = D2D1::RectF(rc.left, rc.top, rc.right, rc.bottom);
	int clippedAreaWidth = pWidth;
	int clippedAreaHeight = pHeight;




	if (BeginPaint(hWnd, &ps))
	{
		std::cout << "First We Will Have to Generate our coordinate plane" << endl;

		D2D1_SIZE_F pSizeValue = m_pD2DDeviceContext1->GetSize();

		m_pD2DDeviceContext1->BeginDraw();
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));

		ID2D1SolidColorBrush* pBlackBrush = nullptr;
		m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);



		// Retrieve the 2D transform that accounts for device orientation.
		if (SUCCEEDED(hr))
		{
			m_pD2DDeviceContext1->DrawGeometry(pPathGeometryObjectOne, pSolidColorBrushObjectGraphDrawing);
			m_pD2DDeviceContext1->DrawGeometry(pPathGeometryObjectTwo, pSolidColorBrushObjectGraphDrawing);
			m_pD2DDeviceContext1->DrawGeometry(pPathGeometryObjectThree, pSolidColorBrushObjectGraphDrawing);
			m_pD2DDeviceContext1->DrawGeometry(pPathGeometryObjectFour, pSolidColorBrushObjectGraphDrawing);
		}

		// We ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
		// is lost. It will be handled during the next call to Present.
		hr = m_pD2DDeviceContext1->EndDraw();






		// Retrieve D2D Device.



		if (hr == D2DERR_RECREATE_TARGET)
		{
			SafeReleaseA(&m_pD2DBitmap);
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}






		pSwapChainObjectD2D->Present(0, 0);




	}
	EndPaint(hWnd, &ps);
	return hr;

}





HRESULT  Direct2DManagementClass::RenderGraphGeometryAlongThePlane(HINSTANCE hInstance, HWND hWnd,
	float pLineThicknessValue,
	ID2D1SolidColorBrush* pSolidColorBrushObjectBackground,
	ID2D1SolidColorBrush* pSolidColorBrushObjectGraphDrawing,
	ID2D1PathGeometry* pPathGeometryObject)
{


	HRESULT hr = S_OK;
	BYTE* selectedAreaData = NULL;
	PAINTSTRUCT ps;

	IWICBitmapLock* pLockObject = nullptr;
	RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetClientRect(hWnd, &rc);
	UINT pWidth = rc.right - rc.left;
	UINT pHeight = rc.bottom - rc.top;
	ID2D1BitmapBrush* pBitmapBrush = nullptr;
	D2D1_RECT_F clippedArea = D2D1::RectF(rc.left, rc.top, rc.right, rc.bottom);
	int clippedAreaWidth = pWidth;
	int clippedAreaHeight = pHeight;




	if (BeginPaint(hWnd, &ps))
	{
		std::cout << "First We Will Have to Generate our coordinate plane" << endl;

		D2D1_SIZE_F pSizeValue = m_pD2DDeviceContext1->GetSize();

		m_pD2DDeviceContext1->BeginDraw();
		m_pD2DDeviceContext1->Clear(D2D1::ColorF(D2D1::ColorF::White));

		ID2D1SolidColorBrush* pBlackBrush = nullptr;
		m_pD2DDeviceContext1->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);



		// Retrieve the 2D transform that accounts for device orientation.
		if (SUCCEEDED(hr))
		{
			m_pD2DDeviceContext1->FillGeometry(pPathGeometryObject, pSolidColorBrushObjectGraphDrawing);
		}

		// We ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
		// is lost. It will be handled during the next call to Present.
		hr = m_pD2DDeviceContext1->EndDraw();






		// Retrieve D2D Device.



		if (hr == D2DERR_RECREATE_TARGET)
		{
			SafeReleaseA(&m_pD2DBitmap);
			hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
		}




	  

		pSwapChainObjectD2D->Present(0, 0);

		

    
    }
	EndPaint(hWnd, &ps);
	return hr;

}
