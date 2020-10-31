#pragma once
/**********************************************************************************************//**
	 * @fn
		*
	 * @brief	Creates structured buffer a.************
	 *
	 * @author	Губанов
	 * @date	19.09.2014
	 *
	 * @param	uElementSize		  	Size of the element.
	 * @param	uCount				  	Number of.
	 * @param	bSRV				  	true to server.
	 * @param	bUAV				  	true to uav.
	 * @param [in,out]	pInitData	  	If non-null, information describing the initialise.
	 * @param [in,out]	ppOutputBuffer	If non-null, buffer for output data.
	 *
	 * @return	The new structured buffer a.

	 **************************************************************************************************/
#include <iostream>
#include "stdio.h"
#include <io.h>
#include "Dx11DemoBase.h"
#include "CompulteBuffer.h"
#include "DaemonObject.h"
#include "stdafx.h"
#include <iostream>
#include "stdio.h"
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <tchar.h>
#include <d3dcompiler.h>
#include <tchar.h>
#include <strsafe.h>
#include <DirectXMath.h>
#include "ComputeTexture.h"
#include "D3D11Shader.h"

#pragma comment(lib,"d3dcompiler.lib")

using namespace std;
using namespace DirectX;


#define BUFFER_SIZE 32768   // read/write buffer size
#define FIBER_COUNT 3       // max fibers (including primary)

#define PRIMARY_FIBER 0 // array index to primary fiber
#define READ_FIBER 1    // array index to read fiber
#define WRITE_FIBER 2   // array index to write fiber

const int MAX_NUMBER_OF_THREADS = 100;
const int MAX_NUMBER_OF_MODELS = 2500;
const int MAX_NUMBER_OF_SKINNED_MODELS = 2500;
const int MAX_NUMBER_OF_TERRAIN_OBJECTS = 2500;
const int MAX_NUMBER_OF_PARTICLE_SYSTEM = 2500;
const int MAX_NUMBER_OF_SHADOW_MAP_EFFECTS = 2500;
const int NAX_NUMBER_OF_IMAGE_TEXTURES = 2500;
const int MAX_NUMBER_OF_AMBIENT_EFFECTS = 2500;
const int MAX_NUMBER_OF_GAME_LEVELS = 2500;
const int MAX_NUMBER_OF_COLLISIONS = 2500;
const int MAX_NUMBER_OF_SCENARIOS = 2500;

enum POSTPROCESS_MODE
{
	PM_COMPUTE_SHADER,
	PM_PIXEL_SHADER,
};


class ComputeShader
{
	friend class DirectXComputeShaderManagementClass;

	ID3D11Device* m_pD3DDevice;
	ID3D11DeviceContext* m_pD3DDeviceContext;
	ID3D11ComputeShader* m_pdD3DComputeShader;

private:
	explicit ComputeShader();

	bool InitializeComputeShader(TCHAR* shaderFile, TCHAR* blobFileAppendix, char* pFunctionName, D3D10_SHADER_MACRO* pDefines,
		ID3D11Device* d3dDevice, ID3D11DeviceContext*d3dContext);

public:
	~ComputeShader();

	void Set();
	void Unset();
};



enum COMPUTE_BUFFER_TYPE
{
	STRUCTURED_BUFFER,
	RAW_BUFFER
};




class ShaderModule
{
	LPCSTR shaderFile;
	ID3DBlob* ppShaderBlob;
	ID3DBlob* ppErrorBlob;
	ID3D11Module* m_pShaderModule;
	ID3D11ModuleInstance* m_pShaderModuleInstance;
	ID3D11FunctionLinkingGraph* m_pShaderFunctionLinkingGraph;
	ID3D11Linker* pDataLinkerObject;

public:
	ShaderModule(LPCSTR file,
		ID3DBlob* shaderBlob,
		ID3DBlob* errorBlob,
		ID3D11Module* shaderModule,
		ID3D11ModuleInstance* shaderModuleInstance,
		ID3D11FunctionLinkingGraph* linkingGraph,
		ID3D11Linker* linkerData)
	{
		shaderFile = file;
		ppShaderBlob = shaderBlob;
		ppErrorBlob = errorBlob;
		m_pShaderModule = shaderModule;
		m_pShaderModuleInstance = shaderModuleInstance;
		m_pShaderFunctionLinkingGraph = linkingGraph;
	}

	~ShaderModule()
	{
		shaderFile = NULL;
		ppShaderBlob = NULL;
		ppErrorBlob = NULL;
		m_pShaderModule = NULL;
		m_pShaderModuleInstance = NULL;
		m_pShaderFunctionLinkingGraph = NULL;
		pDataLinkerObject = NULL;
	}
};


struct StructuredBufferClass
{
	std::string bufferId;
	ID3D11Buffer* m_pInputBuffer;
	ID3D11ShaderResourceView* m_pShaderResourceView;
	ID3D11UnorderedAccessView* m_pUnordereredAccessView;

	StructuredBufferClass(ID3D11Buffer* pd3d11buffer, ID3D11ShaderResourceView* pd3d11shaderResource, ID3D11UnorderedAccessView* pd3dUnorderedAccess)
	{
		m_pInputBuffer = pd3d11buffer;
		m_pShaderResourceView = pd3d11shaderResource;
		m_pUnordereredAccessView = pd3dUnorderedAccess;
	}

};


struct BufType
{
	int i;
	float f;
	double d;
};

class DirectXComputeShaderManagementClass 
{

public:
	

	const int MAX_NUMBER_OF_MEMORY_BUFFERS = 20;
#define BUFSIZE 4096
#define BUFFER_SIZE 32768
#define FIBER_COUNT 100

	const std::string MODEL_BUFFER = "staticModels";
	const std::string SKINNED_MODEL_BUFFER = "skinnedModels";
	const std::string TERRAIN_BUFFER = "terrains";
	const std::string PARTICLE_SYSTEM_BUFFER = "particleSystem";
	const std::string SHADOW_MAP_BUFFER = "shadowMapBufferClass";
	const std::string AMBIENT_OCCLUSION_BUFFER = "ambientOcclusion";
	const std::string RENDERING_ENGINE_BUFFER = "renderingEngineBuffer";
	const std::string RENDERING_SCENE_BUFFER = "renderingSceneBuffer";
	const std::string GAME_LEVEL_MANAGER_BUFFER = "gameLevelManagerClass";
	const std::string GAME_LEVEL_OBJECT_PROCESSOR_BUFFER = "gameLevelObjectProcessor";
	const std::string GAME_LEVEL_MESH_RENDERER_BUFFER = "gameLevelMeshRendererBuffer";
	const std::string GAME_LEVEL_TESSELLATOR = "gameLevelTesselator";
	const std::string COLLISION_MODULE_BUFFER = "skinnedModels";
#define RTN_OK 0
#define RTN_USAGE 1
#define RTN_ERROR 13

	typedef struct
	{
		DWORD dwParameter;          // DWORD parameter to fiber (unused)
		DWORD dwFiberResultCode;    // GetLastError() result code
		HANDLE hFile;               // handle to operate on
		DWORD dwBytesProcessed;     // number of bytes processed
	} FIBERDATASTRUCT, *PFIBERDATASTRUCT, *LPFIBERDATASTRUCT;


	

	std::ofstream out;
	LPVOID g_lpFiber[FIBER_COUNT];
	LPBYTE g_lpBuffer;
	DWORD g_dwBytesRead;

	D3D_DRIVER_TYPE driverType;
	vector<StructuredBufferClass*> m_pStructuredBufferList;
	/*Буффера для обработки ComputeShader */

	ID3D11Buffer *m_pResourceBuffer;
	ID3D11ShaderResourceView* m_pComputeShaderResourceView;
	ID3D11ComputeShader* ppComputeShaderDataStorage;

	ID3D11Buffer* pInputBuffer;
	ID3D11Buffer* pOutputBuffer;
	ID3D11Buffer* pProcessingBuffer;
	ID3D11Buffer* pStagingBuffer;
	ID3D11Buffer* ppRawBuffer;
	//**//
	ID3D11Buffer* m_pScreenQuadVB;
	ID3D11Buffer* m_pScreenQuadIDV;

	DataLoggerClass* pDataLoggerClass;
	IDXGISwapChain* pSwapChainObject;
	IDXGIAdapter* pDXGIAdapter;
	ID3D11Buffer* pTextureConstantBuffer;
	ID3D11Buffer* pStaticModelConstantBuffer;
	ID3D11Buffer* pSkinnnedModelConstantBuffer;
	ID3D11Buffer* pTerrainObjectConstantBuffer;
	ID3D11Buffer* pParticleSystemCBuffer;
	ID3D11Buffer* pShadowMapBuffer;
	ID3D11Buffer* pAmbientOcclusionEffectBuffer;

	ID3D11Buffer* pTextureStagingBuffer;
	ID3D11Buffer* pStaticModelStagingBuffer;
	ID3D11Buffer* ppSkinnedModelStagingBuffer;
	ID3D11Buffer* ppTerrainObjectStagingBuffer;
	ID3D11Buffer* ppParticleSystemStagingBuffer;
	ID3D11Buffer* ppShadowMapStagingBuffer;
	ID3D11Buffer* ppAmbientOcclusionStagingBuffer;



	IDXGIFactory* pComputeDXGIFactory;
	IDXGIAdapter* pComputeDXGIAdaptor;
	IDXGISwapChain* pComputeDXGISwapChain;
	IDXGIOutput* pComputeDXGIOutput;

	ID3D11UnorderedAccessView* ppTextureRenderingUnorderedAccess;
	ID3D11UnorderedAccessView* ppStaticModelsUnorderedAccess;
	ID3D11UnorderedAccessView* ppSkinnedModelUnorderedAccess;
	ID3D11UnorderedAccessView* ppTerrainObjectUnorderedAccess;
	ID3D11UnorderedAccessView* ppParticleSystemUnordererdAccess;
	ID3D11UnorderedAccessView* ppAmbientOcclusionUnorderedAccess;
	ID3D11UnorderedAccessView* ppShadowMapUnorderedAccess;


	vector<IDXGIOutput*> pDXGIOutputs;
	vector<ID3D11Buffer*> m_pProgramConstantBufferCollection;
	vector<ID3D11ShaderResourceView*> m_pShaderResourceCollection;
	vector<ID3D11UnorderedAccessView*> m_pUnordereredAccessViewsCollection;
	vector<ID3D11Buffer*> m_pDebugBuffers;
	vector<ShaderModule> m_pLoadedShaders;


	ID3D11Buffer* m_pMainConstantBuffer;
	ID3D11Buffer* m_pMainStagingBuffer;
	ID3D11ShaderResourceView* m_pMainShaderResourceView;
	ID3D11UnorderedAccessView* m_pMainUnordereredAccessView;
	ID3D11ComputeShader* m_pMainComputeShaderClass;

	ID3D11VertexShader* m_pSceneVertexShader;
	ID3D11PixelShader* m_pScenePixelShader;

	LPCSTR OnFileOpen(HINSTANCE hInstance, HWND hwnd1, HWND hWnd2);

	ID3D11ComputeShader* m_pStaticModelComputeShader;
	ID3D11ComputeShader* m_pSkinnedModelComputeShader;
	ID3D11ComputeShader* m_pTerrainLandscapeComputeShader;
	ID3D11ComputeShader* m_pParticleSystemComputeShader;
	ID3D11ComputeShader* m_pAmbientOcclusionComputeShader;
	ID3D11ComputeShader* m_pShadowMappingComputeShader;
	ID3D11ComputeShader* m_pGameLevelManagerComputeShader;
	ID3D11ComputeShader* m_pGameLevelObjectProcessorComputeShader;
	ID3D11ComputeShader* m_pGameLevelTesselatorComputeShader;
	ID3D11ComputeShader* m_pCollisionDataComputeShader;
	ID3D11ComputeShader* m_pScenarioRenderingComputeShader;
	ID3D11ComputeShader* m_pSkeletalAnimationComputeShader;

	ID3D11Buffer* m_pStaticModelConstantBuffer;
	ID3D11Buffer* m_pSkinnedModelConstantBuffer;
	ID3D11Buffer* m_pParticleSystemConstantBuffer;
	ID3D11Buffer* m_pAmbientOcclusionConstantBuffer;
	ID3D11Buffer* m_pTerrainLandscapeConstantBuffer;
	ID3D11Buffer* m_pShadowMappingConstantBuffer;
	ID3D11Buffer* m_pGameLevelManagementConstantBuffer;
	ID3D11Buffer* m_pGameLevelObjectProcessorcConstantBuffer;
	ID3D11Buffer* m_pGameLevelTesselatorConstantBuffer;
	ID3D11Buffer* m_pCollisionDataConstantBuffer;
	ID3D11Buffer* m_pScenarioRenderingConstantBuffer;

	ID3D11ShaderResourceView* m_pStaticModelSRV;
	ID3D11ShaderResourceView* m_pSkinnedModelSRV;
	ID3D11ShaderResourceView* m_pParticleSystemSRV;
	ID3D11ShaderResourceView* m_pAmbientOcclusionSRV;
	ID3D11ShaderResourceView* m_pTerrainLandscapeSRV;
	ID3D11ShaderResourceView* m_pShadowMappingSRV;
	ID3D11ShaderResourceView* m_pGameLevelSRV;
	ID3D11ShaderResourceView* m_pGameLevelObjectSRV;
	ID3D11ShaderResourceView* m_pGameLevelTesselatorSRV;
	ID3D11ShaderResourceView* m_pCollisionDetectionSRV;
	ID3D11ShaderResourceView* m_pScenarioRenderingSRV;

	ID3D11UnorderedAccessView* m_pStaticModelUAV;
	ID3D11UnorderedAccessView* m_pSkinnedModelUAV;
	ID3D11UnorderedAccessView* m_pParticleSystemUAV;
	ID3D11UnorderedAccessView* m_pAmbientOcclusionUAV;
	ID3D11UnorderedAccessView* m_pTerrainLandscapeUAV;
	ID3D11UnorderedAccessView* m_pShadowMappingUAV;
	ID3D11UnorderedAccessView* m_pGameLevelUAV;
	ID3D11UnorderedAccessView* m_pGameLevelObjectUAV;
	ID3D11UnorderedAccessView* m_pGameLevelTesselatorUAV;
	ID3D11UnorderedAccessView* m_pCollisionDetectionUAV;
	ID3D11UnorderedAccessView* m_pScenarioRenderingUAV;

	vector<ID3D11Buffer*> m_pConstantBufferCollection;
	vector<ID3D11ShaderResourceView*> m_pShaderResourceViewCollection;
	vector<ID3D11UnorderedAccessView*> m_pUnorderedAccessViewCollection;
	vector<ID3D11Buffer*> m_pOutputBufferCollection;

	ID3D11Module* pd3dmodule;
	ID3D11VertexShader* ppVertexShaderCS;
	ID3D11PixelShader* ppPicxelSHader;
	ID3D11InputLayout* ppInputLayout;
	ID3D11Buffer* m_pDebugBuffer;

	ID3D11SamplerState* m_pSamplerStateLinear;
	ID3D11SamplerState* m_pSamplerStateClamp;

	LPBYTE pGlpBuffer;

	LPVOID pFiberObjectHandle;
	LPVOID pFiberObjectWritingHandle;


	void __stdcall WriteFiberProcecure(LPVOID lpParameter);
	void __stdcall DisplayFiberInfo();

private: 
	HRESULT InitializeD3D11DeviceHardware(HINSTANCE hInstance, HWND hWnd);
	ComputeBuffer CreateComputeBuffer(COMPUTE_BUFFER_TYPE uType,
		UINT uElementSize, UINT uCount, bool bSRV, bool bUAV, VOID* pInitData, bool bCreateStaging, char* debugName);
	ID3D11Buffer* CreateRawBuffer(UINT uElementSize, void* pInitData, ID3D11Buffer** ppRawDataBuffer, char* debugName /* = nullptr */);
	ID3D11ShaderResourceView* CreateBufferSRV(ID3D11Buffer* pBuffer);
	ID3D11UnorderedAccessView* CreateBufferUAV(ID3D11Buffer* pBuffer);
	ID3D11Buffer* CreateStagingBuffer(UINT uSize);

	//texture functions
	ID3D11Texture2D* CreateTextureResource(DXGI_FORMAT dxFormat,
		UINT uWidth, UINT uHeight, UINT uRowPitch, VOID* pInitData);
	HRESULT ConnectToKernelNamedPipe(HINSTANCE hInstance, HWND hWnd, LPTSTR pipeName);
	HRESULT LoadShaderModuleFromFile(HINSTANCE hInstance, HWND hWND, LPCWSTR shaderFile, LPCSTR shaderType, LPCSTR shaderSuffix);
	ID3D11Buffer* CreateRawBuffer(UINT uSize, VOID* pInitData);
	ID3D11ShaderResourceView* CreateTextureSRV(ID3D11Texture2D* pTexture);
	ID3D11UnorderedAccessView* CreateTextureUAV(ID3D11Texture2D* pTexture);
	ID3D11Texture2D* CreateStagingTexture(ID3D11Texture2D* pTexture);
	void SetDebugName(ID3D11DeviceChild* object, char* debugName);
	HRESULT InitializeDataProcessingThread(HINSTANCE hInstance, HWND hWnd, LPCSTR fiberName, LPCSTR fileRead, LPCSTR fileWrite);
	
public:
	HRESULT CompileD3DShaderFromFileWhithThreadMacro(const WCHAR* szFileName, LPCSTR szEntryPoint, char* pNumberOfThreads, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT InitializeD3D11DeviceW();
	ID3D11Buffer* CreateAndCopyToDebugBuf(ID3D11Device* pDevice, ID3D11DeviceContext* pd3dImmediateContext, UINT pElementAmount, ID3D11Buffer* pBuffer);
	ID3D11Buffer* CreateAndCopyToDebugBuf(ID3D11Device* pDevice, ID3D11DeviceContext* pd3dImmediateContext, ID3D11Buffer* pBuffer);
	void SimulateComputeShader(ID3D11DeviceContext* pd3dImmediateContext,
		ID3D11ComputeShader* pComputeShader,
		UINT nNumViews, ID3D11ShaderResourceView** pShaderResourceViews,
		ID3D11Buffer* pCBCS, void* pCSData, DWORD dwNumDataBytes,
		ID3D11UnorderedAccessView* pUnorderedAccessView,
		UINT X, UINT Y, UINT Z);
	void InitializeConsoleWindow();
	vector<LPFIBERDATASTRUCT*> pInternalFibersCollection;
	vector<LPVOID> pInternalProcessesCollection;
	IDXGIFactory* pDXGIFactory;
	LPFIBERDATASTRUCT pReadFiberStruct;
	LPFIBERDATASTRUCT pComputationalFiberStruct;
	LPFIBERDATASTRUCT pWriteFiberDataStruct;
	vector<FIBERDATASTRUCT> pComputationalFiberCollection;
	LPVOID pComputationalFiberRead;
	LPVOID pComputationalFiberWrite;
	vector<LPVOID> pComputationalFiberInstances;
	LPBYTE pComputationalMemoryBuff;
	DWORD dwBytesRead;

	void __stdcall ReadFiberProcedure(LPVOID lpParameter);
	ID3D11Buffer* CreateStructuredBuffer(UINT uElementSize, UINT uCount, bool bSRV, bool bUAV, VOID* pInitData);

	HRESULT CompileD3DShaderFromFileForMatrixMultiply(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, LPCSTR pGroupThreadX, LPCSTR pGroupThreadY, ID3DBlob** ppBlobOut);
	HRESULT CompileD3DShaderFromFileW(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT CompileD3DShaderFromFileWhithThreadMacro(const WCHAR* szFileName, LPCSTR szEntryPoint, char* pNumberOfThreadsX,
		char* pNumberOfThreadsY,
		char* pNumberOfThreadsZ,
		LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT CompileD3DShaderFromFileWithMacroW(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT CompileD3DShaderFromFileForMatrixMultiplyW(const WCHAR* szFileName, LPCSTR szEntryPoint, UINT pThreadX, UINT pThreadY,  LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT CompileShaderModule(HINSTANCE hInstance, HWND hWND, LPCSTR shaderFile, LPCSTR shaderType);
	HRESULT InitializeComputeShaderFromFile(HWND hWnd, UINT adapterIndex, TCHAR* shaderFile, TCHAR* blobFileAppendix, char* pFunctionName, D3D10_SHADER_MACRO* pDefines, ID3D11ComputeShader* ppComputeShader);
	HRESULT InitializeComputeShaderFromFileW(HWND hWnd, UINT adapterIndex, LPCSTR shaderFile, LPCSTR blobFileAppendix, LPCSTR pFunctionName, D3D_SHADER_MACRO pDefines[2], ID3D11Device* md3ddevice, ID3D11DeviceContext* ppDeviceContext, ID3D11ComputeShader* ppComputeShader);
	HRESULT InitializeNewComputationalLoop(HINSTANCE hInstance, HWND hWnd, LPCWSTR readFiber, LPCWSTR writeFiber);


	ID3DBlob* pShaderBlob;
	ID3DBlob* pErrorBlob;

	ID3D11Buffer* pTempBufVS;
	ID3D11Buffer* pTempBufCS;
	ID3D11ShaderResourceView* pSRVnullptr;
	ID3D11UnorderedAccessView* ppUnordereredAccess;
	vector<ID3D11Buffer*> ppContstantBufffers;
	vector<ID3D11ShaderResourceView*> ppShaderResourceCollection;
	vector<ID3D11UnorderedAccessView*> ppUnorderedAccessViews;
	vector<ID3D11ComputeShader*> ppComputeShaderCollection;
	ID3D11LinkingNode* pComputeShaderLinker;
	ID3D11LinkingNode* pComputeShaderNode;

	static DirectXComputeShaderManagementClass& Instance(HINSTANCE hInstance, HWND hWnd)
	{
		HRESULT hr = S_OK;
		static DirectXComputeShaderManagementClass* pSecondaryComputeShaderClass = nullptr;
		pSecondaryComputeShaderClass = new DirectXComputeShaderManagementClass(hInstance, hWnd);
		return *pSecondaryComputeShaderClass;
	}

	


	



	ID3D11Device* GetComputeDevice() {return m_pD3DCompDevice; }
	ID3D11DeviceContext* GetComputeContext() { return m_pD3DCompContext;}
	ID3D11UnorderedAccessView* pUnorderedAccessViewW;

	DirectXComputeShaderManagementClass();
	DirectXComputeShaderManagementClass(HINSTANCE hInstance, HWND hWnd);
	DirectXComputeShaderManagementClass(HINSTANCE hInstance, HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	DirectXComputeShaderManagementClass(const DirectXComputeShaderManagementClass& pdcomputeShaderManager);
	~DirectXComputeShaderManagementClass(void);
	vector<IDXGIAdapter*> pAdapters;
	vector<DaemonObject*> pDaemonObjectCollection;
	

	HRESULT InitializeDeamon(HINSTANCE hInstance, HWND hWnd, LPCWSTR deamonName, UINT iIndex);
	void CreateModuleChildProcess(HANDLE pStdInRead,
		HANDLE pStdInWrite,
		HANDLE pStdOutRead,
		HANDLE pStdOutWrite);

	void WriteDataToPipeW( CHAR chBuf[BUFSIZE]);
	void ReadDataFromPipe( CHAR chBuf[BUFSIZE]);
	void ErrorExit(PTSTR exitColde);



	HRESULT LoadShaderModuleProgram(HINSTANCE hInstance, HWND hWND, LPCWSTR shaderFile, LPCSTR shaderType, LPCSTR shaderSuffix, LPCSTR entryPoint, ID3DBlob* pShaderBlob, ID3DBlob* pErrorBlob);
	HRESULT LaunchComputeShader(HINSTANCE hInstance, int adapterIndex, HWND hWnd, LPCSTR fName, LPCSTR functionName);
	BYTE* ExecuteComputeShaderProgram(HINSTANCE hInstance, UINT adapterIndex, HWND hWnd, void* pCSData, ID3D11Buffer* pConstantBuffer, LPCSTR fName, LPCSTR functionName, ID3D11Buffer* pDebugBuffer, ID3D11ComputeShader* pComputeShader, ID3D11ShaderResourceView* pComputeSRV, ID3D11UnorderedAccessView* pComputeUAV);
	HRESULT InitializeComputeShaderInstances(HINSTANCE hInstance, HWND hWnd, UINT adapterIndex);
	BYTE* ExecuteComputeShader(HINSTANCE hInstance, int adapterIndex, HWND hWnd, LPCSTR fName, int x, int y, int z);

	ComputeShader* CreateComputeShaderObject(HWND hWnd, TCHAR* shaderFile, TCHAR* blobFileAppendix, char* pFunctionName, D3D10_SHADER_MACRO* pDefines, ID3D11ComputeShader* ppOutputComputeShader);
	ID3D11Buffer* CreateConstantBuffer(UINT uSize, VOID* pInitData,  char* debugName = nullptr);
	ID3D11Buffer* CreateDynamicBuffer(UINT uSize, VOID* pInitData, char* debugName = nullptr);

	HRESULT InitializeComputeShaderInstances(HINSTANCE hInstance, UINT adapterIndex, HWND hWnd, LPCSTR fName);


	ComputeBuffer* CreateComputationalBuffer(COMPUTE_BUFFER_TYPE uType, UINT uElementSize,
		UINT uCount, bool bSRV, bool bUAV, VOID* pInitData, bool bCreateStaging = false, char* debugName = nullptr);


	ComputeTexture* CreateComputationalTextureFromMemory(DXGI_FORMAT dxFormat,	UINT uWidth,
		UINT uHeight, UINT uRowPitch, VOID* pInitData, bool bCreateStaging = false, char* debugName = nullptr);

	ComputeTexture* CreateComputationalTextureFromFile(TCHAR* textureFilename, char* debugName = nullptr);

	D3D_DRIVER_TYPE computeDriverType;

	HANDLE g_hChildStd_IN_Rd;
	HANDLE g_hChildStd_IN_Wr;
	HANDLE g_hChildStd_OUT_Rd;
	HANDLE g_hChildStd_OUT_Wr;
	HANDLE g_hInputFile;
	IDXGIFactory* m_pDXGIFactory;
	IDXGIAdapter* m_pDXGIAdapter;

	ID3D11Buffer* m_pConstantBuffer;
	ID3D11ComputeShader* pBasicCS11;
	ID3D11Module* m_pShaderModule;
	ID3D11ModuleInstance* m_pShaderModuleInstance;
	ID3D11FunctionLinkingGraph* m_pShaderFunctionLinkingGraph;
	ID3D11Linker* pDataLinkerObject;



	void MainComputationalLoop(HINSTANCE hInstance, HWND hWnd);


	bool LoadContent() {return true; }
	void UnloadContent() {} 
	void UpdateScene() {}
	void UpdateScene(float dt) {}
	void DrawScene() {}
	void OnResize() {}
	void Update() {}
	void Render() {}

	ID3D11ShaderResourceView* ppTextureRenderingResourceView;
	ID3D11ShaderResourceView* ppStaticModelShaderResource;
	ID3D11ShaderResourceView* ppSkinnedModelShaderResource;
	ID3D11ShaderResourceView* ppTerrainObjectShaderResource;
	ID3D11ShaderResourceView* ppParticleSystemShaderResourceView;;
	ID3D11ShaderResourceView* ppAmbientOcclusionResource;
	ID3D11ShaderResourceView* ppShadowMapResource;
	std::vector<UINT> results;

	vector<ID3D11ComputeShader*> m_pComputeShaderOutputCollectiion;

	DirectX::XMFLOAT4X4 GetViewMatrix();

	ID3D11ComputeShader* ppStaticModelComputeShader;
	ID3D11ComputeShader* ppSkinnedModelComputeShader;
	ID3D11ComputeShader* ppTerrainObjectComputeShader;
	ID3D11ComputeShader* ppParticeSystemComputeShader;
	ID3D11ComputeShader* ppAmbientOcclusionComputeShader;
	ID3D11ComputeShader* ppShadowMapComputeShader;

	vector<ID3D11ComputeShader*> m_pComputeShaderCollection;



	UINT g_iBPSBind;

	ID3D11Device* m_pD3DCompDevice;
	ID3D11DeviceContext* m_pD3DCompContext;
	ID3D11Texture2D* m_pDisplacementMap;
	ID3D11ShaderResourceView* m_pDisplacementShaderResourceView;
	ID3D11RenderTargetView* m_pDisplacementRenderTarget;
	ID3D11UnorderedAccessView* m_pDisplacementUnorderedAccess;
	

public:
	ID3D11Device* m_pD3DCompDevice1;
	ID3D11DeviceContext* m_pD3DCompContext1;

	void DestroyComputeShaderBuffers(HINSTANCE hInstance, HWND hWnd);
	HRESULT CreateOutputBuffer(HINSTANCE hInstance, HWND hWnd, UINT pSize,
		ID3D11Buffer* ppOutputBuffer);
	HRESULT CreateRawDataBufferA(UINT uElementSize, void* pInitData, ID3D11Buffer** ppRawDataBuffer, char* debugName = nullptr);
	void RunComputeShaderWithData(ID3D11DeviceContext* pd3dImmediateContext,
		ID3D11ComputeShader* pComputeShader,
		ID3D11ClassInstance* pClassInstance,
		ID3D11ShaderResourceView* pShaderResourceViews,
		ID3D11Buffer* pConstantBuffers,
		void* pCSData,
		DWORD dwNumDataBytes,
		ID3D11UnorderedAccessView* pUnorderedAccessView,
		UINT X, UINT Y, UINT Z);
	HANDLE pFileHandle;
	HRESULT InitializeD3D11Device(HWND hWnd, int adapterIndex);
	void RunComputeShader(ID3D11DeviceContext* pd3dImmediateContext,
		ID3D11ComputeShader* pComputeShader,
		UINT nNumViews, ID3D11ShaderResourceView** pShaderResourceViews,
		ID3D11Buffer* pCBCS, void* pCSData, DWORD dwNumDataBytes,
		ID3D11UnorderedAccessView* pUnorderedAccessView,
		UINT X, UINT Y, UINT Z);
	ID3D11Buffer* CreateAndCopyToDebugBuf(ID3D11Device* pDevice, ID3D11DeviceContext* pd3dImmediateContext, ID3D11Buffer* pInputBuffer, ID3D11Buffer* pBuffer);
	HRESULT StartComputeShaderExecution(HWND hWnd);
	HRESULT StopComputeShaderExecution();
	HRESULT CreateBufferUAV(ID3D11Device* pDevice, ID3D11Buffer* pBuffer, UINT pSize, ID3D11UnorderedAccessView** ppUAVOut);
	HRESULT CreateStructuredBufferForReading(UINT uElementSize, UINT uCount, void* pInitData, ID3D11Buffer** ppBufOut, ID3D11ShaderResourceView** pInputSRV);
	HRESULT CreateStructuredBufferForWriting(UINT uElementSize, ID3D11Buffer** ppBufOut, ID3D11UnorderedAccessView** ppUAV);
	HRESULT CreateStructuredBufferB(ID3D11Device* pDevice, UINT uElementSize, UINT uCount, void* pInitData, ID3D11Buffer** ppBufOut);
	HRESULT CreateBufferUAV(ID3D11Buffer* pBuffer, UINT pSize, ID3D11UnorderedAccessView** ppUAV);

	HRESULT CreateConstantBufferA(HWND hWnd, UINT uSize, VOID* pInitData,  ID3D11Buffer** ppOutputBufffer, char* debugName = nullptr);
	HRESULT CreateDynamicBufferA(HWND hWnd, UINT uType, UINT uElementSize, UINT uCount,	bool* bSRV, bool bUAV, VOID* ppInitiData, bool bCreateStaging, ID3D11Buffer* pOutputBuffer, char* debugName = nullptr);
	HRESULT CreateStructuredBufferA(UINT uElementSize, UINT uCount, bool bSRV, bool bUAV, void* pInitData, ID3D11Buffer* ppOutputBuffer, ID3D11ShaderResourceView* ppShaderResourceView, ID3D11UnorderedAccessView* ppUnordereredAccess);
	
	HRESULT CreateStructuredBufferW(UINT uElementSize, UINT uCount, bool bSRV, bool bUAV, ID3D11Buffer** ppBuffer,
		ID3D11ShaderResourceView** ppSRV,
		ID3D11UnorderedAccessView** ppUAV, 
		const D3D11_SUBRESOURCE_DATA* pInitialData  = nullptr/* = nullptr */);
	HRESULT CreateComputationalFiber(HINSTANCE hInstance, HWND hWnd, LPCSTR fiberName, LPCSTR fileRead, LPCSTR fileWrite);

	void SetComputeShader();
	void UnsetComputeShader();
	HRESULT UpdateLightBuffers(ID3D11DeviceContext* ppContext);
protected:
//****************************
//General ComputeShader Functions that support all compute Shaders;
// They are used to compile average compute shader, attach data and render graphics
// You start by compiling shaders using d3dcompile funtions that load shader from file. After
// that you must create necessary buffers and shader resources that would be needed to render the scene.
// During rendering yo must attach buffers and resource to the compute shader and call the dispatch method to execture 
//****************************

	ID3D11Buffer* GetBufferResource();

	HRESULT SetConstantBufferId(StructuredBufferClass* pBuffer, std::string bufferId);
	ComputeBuffer* CreateBuffer(COMPUTE_BUFFER_TYPE uType,
		UINT uElementSize, UINT uCount, bool bSRV, bool bUAV, VOID* pInitData, bool bCreateStaging, char* debugName);

	
	ID3D11Buffer* CreateComputeStructuredBuffer(UINT uElementSize, UINT uCount,
		bool bSRV, bool bUAV, VOID* pInitData);
	
	ComputeTexture* CreateComputationalTexture(DXGI_FORMAT dxFormat, UINT uWidth,
		UINT uHeight, UINT uRowPitch, VOID* pInitData, bool bCreateStaging, char* debugName);



	ID3D11ShaderResourceView* GetShaderResourceView();
	ID3D11UnorderedAccessView* GetUnorderedAccessView();
	ID3D11Buffer* GetStagingBuffer();
	void CopyDataToStagingBuffer();
	void DiscardComputeShaderBuffer();
	ID3D11Texture2D* GetTextureResource();
	ID3D11ShaderResourceView* GetTexureShaderResourceView();
	ID3D11UnorderedAccessView* GetTextureUnordereredAccessView();
	ID3D11Texture2D* GetSTextureStagingBuffer();
	void CopyTextureStagingBuffer();
	void ReleaseTextureResources();
	
	HRESULT CreateStructuredBuffer(ID3D11Device* pd3dDevice,
		UINT iNumElements,
		ID3D11Buffer** ppBuffer,
		ID3D11ShaderResourceView** ppSRV,
		ID3D11UnorderedAccessView** ppUAV,
		const D3D11_SUBRESOURCE_DATA* pInitialData /* = nullptr */);

	HRESULT CreateOutputBufferA(HWND hWnd, UINT uType, UINT uCount, bool bSRV, bool UAV, VOID* pInitData, bool bCreateStaging, ID3D11Buffer** ppOutputBuffer, char* debugName = nullptr);
	HRESULT CreateEmptyTextureW(HWND hWnd, DXGI_FORMAT dxFormat, UINT uWidth, UINT uHeight, UINT uRowPitch);
	HRESULT CreateTextureFromFileW(HWND hWnd, TCHAR* textureFilename, ID3D11Texture2D** ppOutputTexture);
	HRESULT TerminateGraphicsDataStorage(HWND hWnd);
	HRESULT CreateBufferSRV( ID3D11Device* pDevice, ID3D11Buffer* pBuffer, ID3D11ShaderResourceView** ppSRVOut );
	HRESULT ResizeSwapChainBuffer(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
		const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	HRESULT CreateStagingBufferW(ID3D11Buffer* ppInputBuffer, ID3D11Buffer* ppOutbutBuffer, UINT size);

	

	vector<HANDLE> pComputeShaderThreadHandlers;
	void RunComputeShader( ID3D11DeviceContext* pd3dImmediateContext,
		vector<ID3D11ComputeShader*> pComputeShader,
		vector<ID3D11ShaderResourceView*> pShaderResourceViews, 
		vector<ID3D11Buffer*> pConstantBuffers, void* pCSData, DWORD dwNumDataBytes,
		vector<ID3D11UnorderedAccessView*> pUnorderedAccessView,
		UINT X, UINT Y, UINT Z );

	void RunComputeShaderSingle( ID3D11DeviceContext* pd3dImmediateContext,
		ID3D11ComputeShader* pComputeShader,
		ID3D11ShaderResourceView* pShaderResourceViews, 
		ID3D11Buffer* pConstantBuffers, void* pCSData, DWORD dwNumDataBytes,
		ID3D11UnorderedAccessView* pUnorderedAccessView,
		UINT X, UINT Y, UINT Z );

	
	
	
	void RenderCurrentGraphicsToTexture( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime,
		float fElapsedTime, void* pUserContext );
	HRESULT DumpCurrentSceneToTexture( ID3D11DeviceContext* pd3dImmediateContext, DWORD dwWidth, DWORD dwHeight,
		ID3D11ShaderResourceView* pFromRV, ID3D11RenderTargetView* pToRTV );
	void DrawFullScreenQuad11( ID3D11DeviceContext* pd3dImmediateContext, 
		ID3D11PixelShader* pPixelShader,
		UINT Width, UINT Height);
	HRESULT StartModuleObject(HINSTANCE hInstance, HWND hWnd);
	ID3D11PixelShader* m_pDumpBufferPS;

	

	HRESULT CreateSimulationBuffers( ID3D11Device* pd3dDevice );

	
private:
	//structured buffers
	
	HRESULT CreateBufferShaderResourceViewA(ID3D11Buffer* ppInputBuffer, ID3D11ShaderResourceView** ppShaderSRV);
	HRESULT CreateBufferUnorderedAccessViewA(ID3D11Buffer* ppInputBuffer, ID3D11UnorderedAccessView** ppUnorderedAccessView);
	HRESULT CreateStagingTextureA(ID3D11Texture2D* pTexture, ID3D11Texture2D** ppResulyTexture);


	void CreateTextureAndViews(ID3D11Device* pd3dDevice, UINT width, UINT height, DXGI_FORMAT format,
		ID3D11Texture2D** ppTex, ID3D11ShaderResourceView** ppSRV, ID3D11RenderTargetView** ppRTV);

	HRESULT DumpToTexture( ID3D11DeviceContext* pd3dImmediateContext, DWORD dwWidth, DWORD dwHeight,
		ID3D11ShaderResourceView* pFromRV, ID3D11RenderTargetView* pToRTV );

	


	//ComputeShader Render Functions


	//textured buffers
	HRESULT CreateTextureA(DXGI_FORMAT dxFormat, UINT uWidth, UINT uHeight, UINT uRowPitch, void* pInitData, char* debugName, ID3D11Texture2D** ppOutputTexture);
	HRESULT CreateRawTextureBufferA(UINT uSize, void* pInitData, ID3D11Buffer** ppOutputTextureBuffer);
	HRESULT CreateTextureShaderResourceViewA(ID3D11Texture2D* ppTextureInput, ID3D11ShaderResourceView** ppShaderResourceView);
	HRESULT CreateTextureUnorderedAccessViewA(ID3D11Texture2D* ppTextureInput, ID3D11UnorderedAccessView** ppUnorderedAccessView);
	void SetDebugName(ID3D11Resource* ppDeviceChild, char* debugName);
	HRESULT PresentGraphicsToTexture( ID3D11DeviceContext* pd3dImmediateContext, DWORD dwWidth, DWORD dwHeight,
		ID3D11ShaderResourceView* pFromRV, ID3D11RenderTargetView* pToRTV );



	ID3D11InputLayout* m_pScreenQuadInputLayout;
	ID3D11Resource* pResource;
	
	ID3D11VertexShader* m_pScreenQuadVS;
	ID3D11PixelShader* m_pScreenQuadPS;
	ID3D11ShaderResourceView* m_pCurrentBufferShaderResource;






	ID3D11Resource** ppTextureResource;
	ID3D11Texture2D* pComputeShaderTextureBuffer;
	ID3D11ShaderResourceView*  pComputeShaderResourceView;
	ID3D11UnorderedAccessView* ppTextureUnorderedAccessView;
	ID3D11Texture2D* ppTextureStagingBuffer;
	ID3D11ComputeShader* ppTextureRenderingComputeShader;
	ID3D11Device* pd3dcompDevice;
	ID3D11DeviceContext* pd3dcompContext;

	vector<HANDLE> m_pThreadPointerCollection;
	vector<HANDLE> m_pMutexCollection;


//*********************************************************************************
//End region 
//*********************************************************************************

protected:



};


