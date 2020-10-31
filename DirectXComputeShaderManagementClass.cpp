#include "stdafx.h"
#include "DirectXComputeShaderManagementClass.h"
#include "BasicPlayback.h"
#include <cstdio>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

typedef struct
{
	DWORD dwParameter;          // DWORD parameter to fiber (unused)
	DWORD dwFiberResultCode;    // GetLastError() result code
	HANDLE hFile;               // handle to operate on
	DWORD dwBytesProcessed;     // number of bytes processed
} FIBERDATASTRUCT, *PFIBERDATASTRUCT, *LPFIBERDATASTRUCT;

#define RTN_OK 0
#define RTN_USAGE 1
#define RTN_ERROR 13

#define BUFFER_SIZE 32768   // read/write buffer size
#define FIBER_COUNT 3       // max fibers (including primary)

#define PRIMARY_FIBER 0 // array index to primary fiber
#define READ_FIBER 1    // array index to read fiber
#define WRITE_FIBER 2   // array index to write fiber

LPVOID g_lpFiber[FIBER_COUNT];
LPBYTE g_lpBuffer;
DWORD g_dwBytesRead;
struct TemperatureData
{
	float pTemeratureValue;
};

struct SpeedData
{
	float pSpeedValue;
};


DWORD WINAPI ProcessComputeShaderData(UINT nShaderInstance, DirectXComputeShaderManagementClass* pComputeShaderClass);
void ErrorExit(PTSTR lpszFunction);

void __stdcall DisplayFiberInfo(LPVOID lParam);
VOID __stdcall ReadFiberFunction(LPVOID lpParameter);
void __stdcall WriteFiberProcedure(LPVOID lpParameter);
static DirectXComputeShaderManagementClass pThreadInstance;







VOID __stdcall ReadFiberFunction(LPVOID lpParameter)

{
	DirectXComputeShaderManagementClass* pComputeShaderClass = (DirectXComputeShaderManagementClass*)lpParameter;

	DirectXComputeShaderManagementClass::LPFIBERDATASTRUCT fds = (DirectXComputeShaderManagementClass::LPFIBERDATASTRUCT)lpParameter;
	// If this fiber was passed NULL for fiber data, just return,
	// causing the current thread to exit
	//
	if (fds == NULL)
	{
		printf("Passed NULL fiber data; exiting current thread.\n");
		return;
	}

	//
	// Display some information pertaining to the current fiber
	//
	DisplayFiberInfo(lpParameter);

	fds->dwBytesProcessed = 0;

	while (1)
	{
		//
		// Read data from file specified in the READ_FIBER structure
		//
		if (!ReadFile(fds->hFile, pComputeShaderClass->pInternalProcessesCollection._Mylast, BUFFER_SIZE,
			&pComputeShaderClass->dwBytesRead, NULL))
		{
			break;
		}

		//
		// if we reached EOF, break
		//
		if (pComputeShaderClass->dwBytesRead == 0) break;

		//
		// Update number of bytes processed in the fiber data structure
		//
		fds->dwBytesProcessed += pComputeShaderClass->dwBytesRead;

		//
		// Switch to the write fiber
		//
		SwitchToFiber(pComputeShaderClass->pInternalProcessesCollection[WRITE_FIBER]);
	} // while

	//
	// Update the fiber result code
	//
	fds->dwFiberResultCode = GetLastError();

	//
	// Switch back to the primary fiber
	//
	SwitchToFiber(pComputeShaderClass->pInternalProcessesCollection[PRIMARY_FIBER]);
}

VOID __stdcall WriteFiberFunction(LPVOID lpParameter)
{
	DirectXComputeShaderManagementClass* pComputeShaderClass = (DirectXComputeShaderManagementClass*)lpParameter;

	LPFIBERDATASTRUCT fds = (LPFIBERDATASTRUCT)lpParameter;
	DWORD dwBytesWritten;

	//
	// If this fiber was passed NULL for fiber data, just return,
	// causing the current thread to exit
	//
	if (fds == NULL)
	{
		printf("Passed NULL fiber data; exiting current thread.\n");
		return;
	}

	//
	// Display some information pertaining to the current fiber
	//
	DisplayFiberInfo(lpParameter);

	//
	// Assume all writes succeeded.  If a write fails, the fiber
	// result code will be updated to reflect the reason for failure
	//
	fds->dwBytesProcessed = 0;
	fds->dwFiberResultCode = ERROR_SUCCESS;

	while (1)
	{
		//
		// Write data to the file specified in the WRITE_FIBER structure
		//
		if (!WriteFile(fds->hFile, &pComputeShaderClass->pInternalProcessesCollection.end(), dwBytesWritten,
			&dwBytesWritten, NULL))
		{
			//
			// If an error occurred writing, break
			//
			break;
		}

		//
		// Update number of bytes processed in the fiber data structure
		//
		fds->dwBytesProcessed += dwBytesWritten;

		//
		// Switch back to the read fiber
		//

		SwitchToFiber(pComputeShaderClass->pInternalProcessesCollection[0]);
	}  // while

	//
	// If an error occurred, update the fiber result code...
	//
	fds->dwFiberResultCode = GetLastError();

	//
	// ...and switch to the primary fiber
	//
	SwitchToFiber(pComputeShaderClass->pInternalProcessesCollection[0]);
}


void __stdcall DisplayFiberInfo(LPVOID lParam)
{
	DirectXComputeShaderManagementClass pComputeShaderClass = (DirectXComputeShaderManagementClass&)lParam;

	DirectXComputeShaderManagementClass::LPFIBERDATASTRUCT fds = (DirectXComputeShaderManagementClass::LPFIBERDATASTRUCT)GetFiberData();
	LPVOID lpCurrentFiber = GetCurrentFiber();

	//
	// Determine which fiber is executing, based on the fiber address
	//
	if (lpCurrentFiber == pComputeShaderClass.pInternalProcessesCollection[READ_FIBER])
		printf("Read fiber entered");
	else
	{
		if (lpCurrentFiber == pComputeShaderClass.pInternalProcessesCollection[WRITE_FIBER])
			printf("Write fiber entered");
		else
		{
			if (lpCurrentFiber == pComputeShaderClass.pInternalProcessesCollection[PRIMARY_FIBER])
				printf("Primary fiber entered");
			else
				printf("Unknown fiber entered");
		}
	}

	//
	// Display dwParameter from the current fiber data structure
	//
	printf(" (dwParameter is 0x%lx)\n", fds->dwParameter);
}





DWORD WINAPI ProcessComputeShaderData(UINT nShaderInstance,  DirectXComputeShaderManagementClass* pComputeShaderClass)
{
	HRESULT hr = S_OK;

	DirectXComputeShaderManagementClass* ppComputeShaderClass = pComputeShaderClass;
	ID3D11DeviceContext* pD3DContext = ppComputeShaderClass->m_pD3DCompContext;

	ID3D11Buffer* ppReadbackBuffer = nullptr;



	ID3D11ComputeShader* pComputeShader = ppComputeShaderClass->m_pComputeShaderOutputCollectiion[nShaderInstance];

	pD3DContext->CSGetConstantBuffers(0, 1, &ppReadbackBuffer);

	// Download the data
	D3D11_MAPPED_SUBRESOURCE MappedResource = {0}; 
	
	pD3DContext->CopyResource( ppReadbackBuffer, ppComputeShaderClass->m_pConstantBuffer);

	hr = pD3DContext->Map( ppReadbackBuffer, 0, D3D11_MAP_READ, 0, &MappedResource );

	if( SUCCEEDED( hr ) )
	{
		_Analysis_assume_( MappedResource.pData);
		assert( MappedResource.pData );
		memcpy( &pComputeShaderClass->results[0], MappedResource.pData, MAX_NUMBER_OF_GAME_LEVELS * sizeof(UINT) );
		pD3DContext->Unmap( ppReadbackBuffer, 0 );
	}

	return ppComputeShaderClass->results[0];
	
	
}



ComputeShader::ComputeShader(void)
	: m_pD3DDevice(nullptr),
	m_pD3DDeviceContext(nullptr),
	m_pdD3DComputeShader(nullptr)
{

}


ComputeShader::~ComputeShader()
{
	SafeReleaseA(&m_pdD3DComputeShader);
}

void DirectXComputeShaderManagementClass::MainComputationalLoop(HINSTANCE hInstance, HWND hWnd)
{
	
}

bool ComputeShader::InitializeComputeShader(TCHAR* shaderFile, TCHAR* blobFileAppendix, char* pFunctionName, D3D10_SHADER_MACRO* pDefines, ID3D11Device* d3dDevice, ID3D11DeviceContext*d3dContext)
{
	HRESULT hr = S_OK;
	m_pD3DDevice = d3dDevice;
	m_pD3DDeviceContext = d3dContext;


	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrorBlob = nullptr;

	FILE* fShaderBlob = nullptr;

	TCHAR blobFileName[300];

	if (blobFileAppendix != nullptr)
	{
		size_t l1 = _tcslen(shaderFile);
		size_t l2 = _tcslen(_tcsrchr(shaderFile, _T('.')));
		_tcsncpy_s(blobFileName, shaderFile, l1 - l2);

		_tcscat_s(blobFileName, _T("_"));
		_tcscat_s(blobFileName, blobFileAppendix);
		_tcscat_s(blobFileName, _T(".blob"));

		_tfopen_s(&fShaderBlob, blobFileName, _T("rb"));
	}


	DWORD dwShaderFlags =	D3DCOMPILE_ENABLE_STRICTNESS |
		D3DCOMPILE_IEEE_STRICTNESS |
		//D3DCOMPILE_WARNINGS_ARE_ERRORS |
		D3DCOMPILE_PREFER_FLOW_CONTROL;


	#if defined(DEBUG) || defined(_DEBUG)
		dwShaderFlags |= D3DCOMPILE_DEBUG;
		dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	#else
		dwShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
	#endif

	if (fShaderBlob == nullptr)
	{
		hr = D3DCompileFromFile(shaderFile, pDefines, D3D_COMPILE_STANDARD_FILE_INCLUDE, pFunctionName, "cs_5_0",
			dwShaderFlags, NULL, &pCompiledShader, &pErrorBlob);
		if (hr == S_OK)
		{
			if (blobFileAppendix != nullptr)
			{
				_tfopen_s(&fShaderBlob, blobFileName, _T("wb"));
				
				if (fShaderBlob != nullptr)
				{
					size_t size = pCompiledShader->GetBufferSize();
					fwrite(&size, sizeof(size_t), 1, fShaderBlob);
					fwrite(pCompiledShader->GetBufferPointer(), size, 1, fShaderBlob);
					fclose(fShaderBlob);
				}
			}
		}
	}
	else
	{
		int size = 0;
		fread_s(&size, sizeof(int), sizeof(int), 1, fShaderBlob);

		hr = D3DCreateBlob(size, &pCompiledShader);

		if (hr == S_OK)
		{
			fread_s(pCompiledShader->GetBufferPointer(), size, size, 1, fShaderBlob);
		}

		fclose(fShaderBlob);
	}

	if (pErrorBlob)
	{
		OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
	}

	if (hr == S_OK)
	{
		hr = m_pD3DDevice->CreateComputeShader(pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(), nullptr, &m_pdD3DComputeShader);

			SafeReleaseA(&pErrorBlob);
			SafeReleaseA(&pCompiledShader);

	}

	return true;
}


HRESULT DirectXComputeShaderManagementClass::ConnectToKernelNamedPipe(HINSTANCE hInstance, HWND hWnd, LPTSTR pipeName)
{
	HRESULT hr = S_OK;
	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\GPUDigitalLabCore");
	HANDLE pNamedPipeHandle = INVALID_HANDLE_VALUE;
	TCHAR chBuf[BUFSIZE];
	BOOL   fSuccess = FALSE;
	DWORD  cbRead, cbToWrite, cbWritten, dwMode;
	LPTSTR lpvMessage = TEXT("Default message from client.");

	while (1)
	{
		pNamedPipeHandle = CreateFile(pipeName,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);

		if (pNamedPipeHandle != INVALID_HANDLE_VALUE)
			break;

		if (GetLastError() == ERROR_PIPE_BUSY)
		{
			_tprintf(TEXT("Could not open pipe. GLE=%d\n"), GetLastError());
			hr = E_FAIL;
			return hr;
		}

		fSuccess = WaitNamedPipe(lpszPipename, 2000);

		if (!fSuccess)
		{
			printf("Could not open pipe: 20 second wait timed out.");
			return -1;
		}
	}

	// The pipe connected; change to message-read mode. 

	dwMode = PIPE_READMODE_MESSAGE;
	fSuccess = SetNamedPipeHandleState(
		pNamedPipeHandle,    // pipe handle 
		&dwMode,  // new pipe mode 
		NULL,     // don't set maximum bytes 
		NULL);    // don't set maximum time 
	if (!fSuccess)
	{
		_tprintf(TEXT("SetNamedPipeHandleState failed. GLE=%d\n"), GetLastError());
		return -1;
	}

	// Send a message to the pipe server. 

	cbToWrite = (lstrlen(lpvMessage) + 1)*sizeof(TCHAR);
	_tprintf(TEXT("Sending %d byte message: \"%s\"\n"), cbToWrite, lpvMessage);

	fSuccess = WriteFile(
		pNamedPipeHandle,                  // pipe handle 
		lpvMessage,             // message 
		cbToWrite,              // message length 
		&cbWritten,             // bytes written 
		NULL);                  // not overlapped 

	if (!fSuccess)
	{
		_tprintf(TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError());
		return -1;
	}

	printf("\nMessage sent to server, receiving reply as follows:\n");
}

ID3D11Buffer* DirectXComputeShaderManagementClass::CreateAndCopyToDebugBuf(ID3D11Device* pDevice, ID3D11DeviceContext* pd3dImmediateContext, ID3D11Buffer* pInputBuffer, ID3D11Buffer* pBuffer)
{

	HRESULT hr = S_OK;

	ID3D11Buffer* debugBuf = nullptr;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	pInputBuffer->GetDesc(&desc);

	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;

	hr = pDevice->CreateBuffer(&desc, nullptr, &debugBuf);
	if (SUCCEEDED(hr))
	{
		pd3dImmediateContext->CopyResource(debugBuf, pInputBuffer);
	}

	return debugBuf;
}

HRESULT DirectXComputeShaderManagementClass::CreateComputationalFiber(HINSTANCE hInstance, HWND hWnd, LPCSTR fiberName, LPCSTR fileRead, LPCSTR fileWrite)
{
	HRESULT hr = S_OK;
	LPFIBERDATASTRUCT fs;
	std::ofstream out("c:\\GPUDigitalLab.txt");
	out << "Welcome To Log Implementation" << endl;


	//
	// Allocate storage for our fiber data structures
	//
	fs = (LPFIBERDATASTRUCT)HeapAlloc(
		GetProcessHeap(), 0,
		sizeof(FIBERDATASTRUCT) * FIBER_COUNT);

	if (fs == NULL)
	{
		printf("HeapAlloc error (%d)\n", GetLastError());
		out << "HeapAlloc error (%d)\n" << GetLastError() << endl;
		hr = E_FAIL;
		return hr;
	}

	//
	// Allocate storage for the read/write buffer
	//
	g_lpBuffer = (LPBYTE)HeapAlloc(GetProcessHeap(), 0, BUFFER_SIZE);
	if (g_lpBuffer == NULL)
	{
		printf("HeapAlloc error (%d)\n", GetLastError());
		out << "HeapAlloc error (%d)\n" << GetLastError() << endl;

		hr = E_FAIL;
		return hr;
	}

	//
	// Open the source file
	//
	fs[READ_FIBER].hFile = CreateFile(
		(LPCWSTR)fileRead,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_SHARE_WRITE | FILE_SHARE_WRITE,
		NULL
		);

	if (fs[READ_FIBER].hFile == INVALID_HANDLE_VALUE)
	{
		printf("CreateFile error (%d)\n", GetLastError());
		out << "HeapAlloc error (%d)\n" << GetLastError() << endl;
		return hr;
	}

	//
	// Open the destination file
	//
	fs[WRITE_FIBER].hFile = CreateFile(
		(LPCWSTR)fileWrite,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_SHARE_WRITE | FILE_SHARE_WRITE,
		NULL
		);

	if (fs[WRITE_FIBER].hFile == INVALID_HANDLE_VALUE)
	{
		printf("CreateFile error (%d)\n", GetLastError());
		out << "CreateFile error (%d)\n" << GetLastError() << endl;
		return hr;

		
	}

	//
	// Convert thread to a fiber, to allow scheduling other fibers
	//
	g_lpFiber[PRIMARY_FIBER] = ConvertThreadToFiber(&fs[PRIMARY_FIBER]);

	if (g_lpFiber[PRIMARY_FIBER] == NULL)
	{
		printf("ConvertThreadToFiber error (%d)\n", GetLastError());
		out << "ConvertThreadToFiber error (%d)\n" << GetLastError() << endl;
		hr = E_FAIL;
		return hr;
	}

	//
	// Initialize the primary fiber data structure.  We don't use
	// the primary fiber data structure for anything in this sample.
	//
	fs[PRIMARY_FIBER].dwParameter = 0;
	fs[PRIMARY_FIBER].dwFiberResultCode = 0;
	fs[PRIMARY_FIBER].hFile = INVALID_HANDLE_VALUE;

	//
	// Create the Read fiber
	//
	g_lpFiber[READ_FIBER] = CreateFiber(0, ReadFiberFunction, &fs[READ_FIBER]);

	if (g_lpFiber[READ_FIBER] == NULL)
	{
		printf("CreateFiber error (%d)\n", GetLastError());
		out << "CreateFiber error (%d)\n" << GetLastError() << endl;

		hr = E_FAIL;
		return hr;
	}

	fs[READ_FIBER].dwParameter = 0x12345678;

	//
	// Create the Write fiber
	//
	g_lpFiber[WRITE_FIBER] = CreateFiber(0, WriteFiberFunction, &fs[WRITE_FIBER]);

	if (g_lpFiber[WRITE_FIBER] == NULL)
	{
		printf("CreateFiber error (%d)\n", GetLastError());
		hr = E_FAIL;
		return hr;
	}

	fs[WRITE_FIBER].dwParameter = 0x54545454;

	//
	// Switch to the read fiber
	//
	SwitchToFiber(g_lpFiber[0]);

	//
	// We have been scheduled again. Display results from the 
	// read/write fibers
	//
	printf("ReadFiber: result code is %lu, %lu bytes processed\n",
		fs[READ_FIBER].dwFiberResultCode, fs[READ_FIBER].dwBytesProcessed);
	out << "ReadFiber: result code is %lu, %lu bytes processed\n" << fs[READ_FIBER].dwFiberResultCode << fs[READ_FIBER].dwBytesProcessed << endl;


	printf("WriteFiber: result code is %lu, %lu bytes processed\n",
		fs[WRITE_FIBER].dwFiberResultCode, fs[WRITE_FIBER].dwBytesProcessed);
	out << "WriteFiber: result code is %lu, %lu bytes processed\n" << fs[WRITE_FIBER].dwFiberResultCode << fs[WRITE_FIBER].dwBytesProcessed << endl;


	//
	// Delete the fibers
	//
	DeleteFiber(g_lpFiber[READ_FIBER]);
	DeleteFiber(g_lpFiber[WRITE_FIBER]);

	//
	// Close handles
	//
	CloseHandle(fs[READ_FIBER].hFile);
	CloseHandle(fs[WRITE_FIBER].hFile);

	//
	// Free allocated memory
	//
	HeapFree(GetProcessHeap(), 0, g_lpBuffer);
	HeapFree(GetProcessHeap(), 0, fs);
	out.close();
	
	return hr;
}



HRESULT DirectXComputeShaderManagementClass::InitializeDeamon(HINSTANCE hInstance, HWND hWnd, LPCWSTR deamonName, UINT iIndex)
{
	
	HRESULT hr = S_OK;
	BOOL result = false;
	SECURITY_ATTRIBUTES securityAttributes;
	printf("\n->Start of parent execution.\n");
	
	HANDLE pChildStdInReader = NULL;
	HANDLE pChildStdInWriter = NULL;
	HANDLE pChildStdOutReader = NULL;
	HANDLE pChildStdOutWriter = NULL;
	HANDLE pInputHandle = NULL;

	securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	securityAttributes.bInheritHandle = TRUE;
	securityAttributes.lpSecurityDescriptor = NULL;

	result = CreatePipe(&pChildStdOutReader, &pChildStdOutWriter,
		&securityAttributes, 0);

	if (!result)
	{
		ErrorExit(TEXT("StdoutRd CreatePipe"));
		hr = E_FAIL;
		return hr;
	}

	result = SetHandleInformation(pChildStdOutReader, HANDLE_FLAG_INHERIT, 0);

	if (!result)
	{
		ErrorExit(TEXT("Stdout SetHandleInformation")); 
		hr = E_FAIL;
		return hr;
	}

	result = CreatePipe(&pChildStdInReader, &pChildStdInWriter,
		&securityAttributes, 0);

	if (!result)
	{
		ErrorExit(TEXT("Stdin CreatePipe")); 
		hr = E_FAIL;
		return hr;
	}

	result = SetHandleInformation(pChildStdInReader, HANDLE_FLAG_INHERIT, 0);
	if (!result)
	{
		ErrorExit(TEXT("Stdin SetHandleInformation")); 
		hr = E_FAIL;
		return hr;
	}
	
	CreateModuleChildProcess(pChildStdInReader, pChildStdInWriter, pChildStdOutReader, pChildStdOutWriter);
	LPCWSTR strIndex = (LPCWSTR)iIndex;

	pInputHandle = CreateFile(
		deamonName, 
		GENERIC_READ, 
		0, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_READONLY, 
		NULL); 

	if (pInputHandle == INVALID_HANDLE_VALUE)
	{
		ErrorExit(TEXT("CreateFile"));
	}

	DaemonObject* pMyDaemonObject = new DaemonObject(pChildStdInReader,
		pChildStdInWriter,
		pChildStdOutReader,
		pChildStdOutWriter);

	pDaemonObjectCollection.push_back(pMyDaemonObject);

	CHAR buffData[BUFSIZE];
	printf( "\n->Contents of %s written to child STDIN pipe.\n", buffData);
	printf( "\n->Contents of child process STDOUT:\n\n", buffData);
	
	return hr;
}


void DirectXComputeShaderManagementClass::ErrorExit(PTSTR lpszFunction)
{
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError(); 

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
		(lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)lpszFunction)+40)*sizeof(TCHAR)); 
	StringCchPrintf((LPTSTR)lpDisplayBuf, 
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"), 
		lpszFunction, dw, lpMsgBuf); 
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	
}


void DirectXComputeShaderManagementClass::ReadDataFromPipe(CHAR chBuf[BUFSIZE])
{
	DWORD dwRead, dwWritten; 
	BOOL bSuccess = FALSE;
	HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	for (;;) 
	{ 
		bSuccess = ReadFile( g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
		if( ! bSuccess || dwRead == 0 ) break; 

		bSuccess = WriteFile(hParentStdOut, chBuf, 
			dwRead, &dwWritten, NULL);
		if (! bSuccess ) break; 
	} 
}





void DirectXComputeShaderManagementClass::WriteDataToPipeW(CHAR chBuf[BUFSIZE])
{
	DWORD dwRead, dwWritten; 
	BOOL bSuccess = FALSE;

	
		bSuccess = ReadFile(g_hChildStd_IN_Wr, chBuf, BUFSIZE, &dwRead, NULL);
		if ( ! bSuccess || dwRead == 0 ) 
		{
			return;
		}

		bSuccess = WriteFile(g_hChildStd_IN_Wr, chBuf, dwRead, &dwWritten, NULL);
		if ( ! bSuccess ) 
		{
			return;
		}

	// Close the pipe handle so the child process stops reading. 

	if ( ! CloseHandle(g_hChildStd_IN_Wr) ) 
		ErrorExit(TEXT("StdInWr CloseHandle"));
}




void DirectXComputeShaderManagementClass::CreateModuleChildProcess(HANDLE pStdInRead,
																   HANDLE pStdInWrite,
																   HANDLE pStdOutRead,
																   HANDLE pStdOutWrite)
{
	TCHAR szCmdline[]=TEXT("child");
	PROCESS_INFORMATION piProcInfo; 
	STARTUPINFO siStartInfo;
	BOOL bSuccess = FALSE; 

	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));

	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdError = pStdOutWrite;
	siStartInfo.hStdOutput = pStdOutWrite;
	siStartInfo.hStdInput = pStdInRead;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	bSuccess = CreateProcess(NULL,
		szCmdline,     // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		0,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&piProcInfo);  // receives PROCESS_INFORMATION 

	if (!bSuccess)
	{
		ErrorExit(TEXT("CreateProcess"));
	}
	else
	{
		CloseHandle(piProcInfo.hProcess);
		CloseHandle(piProcInfo.hThread);
	}

}

void ErrorExit(PTSTR lpszFunction) 

	// Format a readable error message, display a message box, 
	// and exit from the application.
{ 
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError(); 

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
		(lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)lpszFunction)+40)*sizeof(TCHAR)); 
	StringCchPrintf((LPTSTR)lpDisplayBuf, 
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"), 
		lpszFunction, dw, lpMsgBuf); 
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(1);
}


void DirectXComputeShaderManagementClass::DestroyComputeShaderBuffers(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pD3DCompDevice != nullptr)
	{
		SafeReleaseA(&m_pD3DCompDevice);
	}
	if (m_pD3DCompContext != nullptr)
	{
		SafeReleaseA(&m_pD3DCompContext);
	}
	
}


HRESULT DirectXComputeShaderManagementClass::LoadShaderModuleProgram(HINSTANCE hInstance, HWND hWND, LPCWSTR shaderFile, LPCSTR shaderType, LPCSTR shaderSuffix, LPCSTR entryPoint, ID3DBlob* pShaderBlob, ID3DBlob* pErrorBlob)
{
	HRESULT hr = S_OK;

	ID3D11Module* pNoduleObject = nullptr;
	hr = D3DCompileFromFile(shaderFile, NULL, NULL, NULL,
		0, 0, 0, &pShaderBlob, &pErrorBlob);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = D3DLoadModule(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), &m_pShaderModule);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = m_pShaderModule->CreateInstance("", &m_pShaderModuleInstance);
	if (FAILED(hr))
	{
		return hr;
	}

	m_pShaderModuleInstance->BindResource(0, 0, 1);
	m_pShaderModuleInstance->BindSampler(0, 0, 0);
	m_pShaderModuleInstance->BindConstantBuffer(0, 0, 0);
	m_pShaderModuleInstance->BindConstantBuffer(1, 1, 0);
	m_pShaderModuleInstance->BindConstantBuffer(2, 2, 0);

	hr = D3DCreateFunctionLinkingGraph(0, &m_pShaderFunctionLinkingGraph);

	if (FAILED(hr))
	{
		return hr;
	}

	static const D3D11_PARAMETER_DESC shaderInputParameters[] =
	{
		{ "inputPos", "POSITION0", D3D_SVT_FLOAT, D3D_SVC_VECTOR, 1, 3, D3D_INTERPOLATION_LINEAR, D3D_PF_IN, 0, 0, 0, 0 },
		{ "inputTex", "TEXCOORD0", D3D_SVT_FLOAT, D3D_SVC_VECTOR, 1, 2, D3D_INTERPOLATION_LINEAR, D3D_PF_IN, 0, 0, 0, 0 },
		{ "inputNorm", "NORMAL0", D3D_SVT_FLOAT, D3D_SVC_VECTOR, 1, 3, D3D_INTERPOLATION_LINEAR, D3D_PF_IN, 0, 0, 0, 0 }
	};

	hr = m_pShaderFunctionLinkingGraph->SetInputSignature(shaderInputParameters, ARRAYSIZE(shaderInputParameters),
		&pComputeShaderNode);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = m_pShaderFunctionLinkingGraph->CallFunction("", m_pShaderModule, "",
		&pComputeShaderNode);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = m_pShaderFunctionLinkingGraph->PassValue(pComputeShaderNode, 0, pComputeShaderNode, 0);
	
	if (FAILED(hr))
	{
		return hr;
	}

	D3D11_PARAMETER_DESC computeShaderOutputParameters[] =
	{
		{ "outputColor", "SV_TARGET", D3D_SVT_FLOAT, D3D_SVC_VECTOR, 1, 4, D3D_INTERPOLATION_UNDEFINED, D3D_PF_OUT, 0, 0, 0, 0 }
	};

	hr = m_pShaderFunctionLinkingGraph->SetOutputSignature(computeShaderOutputParameters, ARRAYSIZE(computeShaderOutputParameters),
		&pComputeShaderLinker);
	
	if (FAILED(hr))
	{
		return hr;
	}

	hr = m_pShaderFunctionLinkingGraph->PassValue(pComputeShaderNode, D3D_RETURN_PARAMETER_INDEX, pComputeShaderLinker, 0);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = m_pShaderFunctionLinkingGraph->CreateModuleInstance(&m_pShaderModuleInstance, &pErrorBlob);

	if (FAILED(hr))
	{
		return hr;
	}
	
	hr = D3DCreateLinker(&pDataLinkerObject);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = pDataLinkerObject->UseLibrary(m_pShaderModuleInstance);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = pDataLinkerObject->Link(m_pShaderModuleInstance, entryPoint, shaderSuffix, 0, &pShaderBlob, &pErrorBlob);



	return hr;
}	


HRESULT DirectXComputeShaderManagementClass::CompileD3DShaderFromFileW(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	if (!ppBlobOut)
	{
		hr = E_INVALIDARG;
		return hr;
	}


	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;

#endif

	ID3DBlob* pErrorBlob = nullptr;

	
	const D3D_SHADER_MACRO defines[] =
	{
		"NUM_THREADS", "256",
		NULL, NULL
	};



	hr = D3DCompileFromFile(szFileName, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);

	if (pErrorBlob)
	{
		OutputDebugString((LPCWSTR)pErrorBlob->GetBufferPointer());
		printf((char*)pErrorBlob->GetBufferPointer());
	}

	if (FAILED(hr))
	{
		int pError = GetLastError();



		if (pError == ERROR_ARITHMETIC_OVERFLOW)
		{
			MessageBoxA(NULL, (LPCSTR)L"При компиляции шейдера произошло переполнение памяти", (LPCSTR)"Message", 0);
		}


		SafeReleaseA(&pErrorBlob);

		return hr;
	}

	SafeReleaseA(&pErrorBlob);

	return S_OK;
}


HRESULT DirectXComputeShaderManagementClass::CompileD3DShaderFromFileWhithThreadMacro(const WCHAR* szFileName, LPCSTR szEntryPoint, char* pNumberOfThreadsX,
	char* pNumberOfThreadsY,
	char* pNumberOfThreadsZ,
	LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	if (!ppBlobOut)
	{
		hr = E_INVALIDARG;
		return hr;
	}


	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;

#endif

	ID3DBlob* pErrorBlob = nullptr;







	const _D3D_SHADER_MACRO defines[] =
	{
		"GROUP_SIZE_X", pNumberOfThreadsX,
		"GROUP_SIZE_Y", pNumberOfThreadsY,
		"GROUP_SIZE_Z", pNumberOfThreadsZ,
		NULL, NULL
	};



	hr = D3DCompileFromFile(szFileName, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);

	if (pErrorBlob)
	{
		OutputDebugString((LPCWSTR)pErrorBlob->GetBufferPointer());
		printf((char*)pErrorBlob->GetBufferPointer());
	}

	if (FAILED(hr))
	{
		int pError = GetLastError();



		if (pError == ERROR_ARITHMETIC_OVERFLOW)
		{
			MessageBoxA(NULL, (LPCSTR)L"При компиляции шейдера произошло переполнение памяти", (LPCSTR)"Message", 0);
		}


		SafeReleaseA(&pErrorBlob);

		return hr;
	}

	SafeReleaseA(&pErrorBlob);

	return S_OK;
}




HRESULT DirectXComputeShaderManagementClass::CompileD3DShaderFromFileWhithThreadMacro(const WCHAR* szFileName, LPCSTR szEntryPoint, char* pNumberOfThreads, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	if (!ppBlobOut)
	{
		hr = E_INVALIDARG;
		return hr;
	}


	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;

#endif

	ID3DBlob* pErrorBlob = nullptr;



	
	
	

	const _D3D_SHADER_MACRO defines[] =
	{
		"GROUP_SIZE_X", pNumberOfThreads,
		NULL, NULL
	};



	hr = D3DCompileFromFile(szFileName, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);

	if (pErrorBlob)
	{
		OutputDebugString((LPCWSTR)pErrorBlob->GetBufferPointer());
		printf((char*)pErrorBlob->GetBufferPointer());
	}

	if (FAILED(hr))
	{
		int pError = GetLastError();



		if (pError == ERROR_ARITHMETIC_OVERFLOW)
		{
			MessageBoxA(NULL, (LPCSTR)L"При компиляции шейдера произошло переполнение памяти", (LPCSTR)"Message", 0);
		}


		SafeReleaseA(&pErrorBlob);

		return hr;
	}

	SafeReleaseA(&pErrorBlob);

	return S_OK;
}







HRESULT DirectXComputeShaderManagementClass::CompileD3DShaderFromFileForMatrixMultiply(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, LPCSTR pGroupThreadX, LPCSTR pGroupThreadY, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	if (!ppBlobOut)
	{
		hr = E_INVALIDARG;
		return hr;
	}


	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	
#endif

	ID3DBlob* pErrorBlob = nullptr;

	D3D10_SHADER_MACRO ShaderMacro[3] = { { "GROUP_SIZE_X", pGroupThreadX }, { "GROUP_SIZE_Y", pGroupThreadY }, { NULL, NULL } };


	hr = D3DCompileFromFile(szFileName, ShaderMacro, D3D_COMPILE_STANDARD_FILE_INCLUDE, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);

	if (pErrorBlob)
	{
		OutputDebugString((LPCWSTR)pErrorBlob->GetBufferPointer());
		printf((char*)pErrorBlob->GetBufferPointer());
	}
	
	if (FAILED(hr))
	{
		int pError = GetLastError();
		std::cout << "Unable To Compile the Compute Shader Object Error: " << endl;
	
		
		if (pError == ERROR_ARITHMETIC_OVERFLOW)
		{
			MessageBoxA(NULL, (LPCSTR)L"При компиляции шейдера произошло переполнение памяти", (LPCSTR)"Message", 0);
		}
	

		SafeReleaseA(&pErrorBlob);

		return hr;
	}

	SafeReleaseA(&pErrorBlob);

	return S_OK;
}










void ComputeShader::Set()
{
	m_pD3DDeviceContext->CSSetShader(m_pdD3DComputeShader, nullptr, 0);
}

void ComputeShader::Unset()
{
	m_pD3DDeviceContext->CSSetShader(nullptr, nullptr, 0);
}


ID3D11Buffer* DirectXComputeShaderManagementClass::CreateRawBuffer(UINT uElementSize, void* pInitData, ID3D11Buffer** ppRawDataBuffer, char* debugName /* = nullptr */)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* ppDataBuffer = nullptr;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_INDEX_BUFFER | D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = uElementSize;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

	if (pInitData)
	{
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = pInitData;
		hr = m_pD3DCompDevice->CreateBuffer(&desc, &initData, &ppDataBuffer);
	}
	else
	{
		hr = m_pD3DCompDevice->CreateBuffer(&desc, nullptr, &ppDataBuffer);
	}

	*ppRawDataBuffer = ppDataBuffer;
	return ppDataBuffer;
}



ComputeBuffer* DirectXComputeShaderManagementClass::CreateBuffer(COMPUTE_BUFFER_TYPE uType, UINT uElementSize, UINT uCount, bool bSRV, bool bUAV, VOID* pInitData, bool bCreateStaging, char* debugName)
{
	HRESULT hr = S_OK;

	ComputeBuffer* m_pDataBuffer = new ComputeBuffer();
	m_pDataBuffer->m_pD3DDeviceContext = m_pD3DCompContext;
	UINT count = 1;

	if (uType == STRUCTURED_BUFFER)
	{
		hr = CreateStructuredBufferA(uElementSize, count, true,true, nullptr, m_pDataBuffer->m_pResourceBuffer,
			m_pDataBuffer->m_pShaderResource, m_pDataBuffer->m_pUnordereredAccess);

	}
	else if (uType == RAW_BUFFER)
	{
		m_pDataBuffer->m_pResourceBuffer = CreateRawBuffer(uElementSize, pInitData, &m_pDataBuffer->m_pResourceBuffer, debugName);
	}

	if (m_pDataBuffer->m_pResourceBuffer)
	{
		if (bSRV)
		{
			hr = CreateBufferSRV(m_pD3DCompDevice, m_pDataBuffer->m_pResourceBuffer, &m_pDataBuffer->m_pShaderResource);
		}

		if (bUAV)
		{
			hr = CreateBufferUnorderedAccessViewA(m_pDataBuffer->m_pResourceBuffer, &m_pDataBuffer->m_pUnordereredAccess);
		}

		if (bCreateStaging)
		{
			hr = CreateStagingBufferW(m_pDataBuffer->m_pResourceBuffer, m_pDataBuffer->m_pStagingBuffer, uElementSize);
		}
	}

	if (debugName)
	{
		if (m_pDataBuffer->m_pResourceBuffer) SetDebugName(m_pDataBuffer->m_pResourceBuffer, debugName);
		if (m_pDataBuffer->m_pShaderResource) SetDebugName(m_pDataBuffer->m_pShaderResource, debugName);
		if (m_pDataBuffer->m_pUnordereredAccess) SetDebugName(m_pDataBuffer->m_pUnordereredAccess, debugName);
		if (m_pDataBuffer->m_pStagingBuffer) SetDebugName(m_pDataBuffer->m_pStagingBuffer, debugName);

	}

	return m_pDataBuffer;
}




DirectXComputeShaderManagementClass::DirectXComputeShaderManagementClass(HINSTANCE hInstance, HWND hWnd)
{
	m_pD3DCompDevice = nullptr;
	m_pD3DCompContext = nullptr;
	m_pD3DCompDevice1 = nullptr;
	m_pD3DCompContext1 = nullptr;
	pDaemonObjectCollection = vector<DaemonObject*>();
	pDaemonObjectCollection.resize(MAX_NUMBER_OF_MODELS);
	pComputationalFiberCollection.resize(10);
}


DirectXComputeShaderManagementClass::DirectXComputeShaderManagementClass(const DirectXComputeShaderManagementClass& pdcomputeShaderManager)
{
	vector<IDXGIAdapter*> pAdaptersData = vector<IDXGIAdapter*>();
	pAdaptersData.resize(200);
}




HRESULT DirectXComputeShaderManagementClass::InitializeNewComputationalLoop(HINSTANCE hInstance, HWND hWnd, LPCWSTR readFiber, LPCWSTR writeFiber)
{
	HRESULT hr = S_OK;
	return hr;
}



HRESULT DirectXComputeShaderManagementClass::LaunchComputeShader(HINSTANCE hInstance, int adapterIndex, HWND hWnd, LPCSTR fName, LPCSTR functionName)
{
	HRESULT hr = S_OK;
	BYTE* pOutputData;
	void* pDataBuffer = nullptr;
	ID3D11Buffer* pConstantBuffer = nullptr;
	ID3D11ComputeShader* pDMOLComputeShader = nullptr;
	ID3D11ShaderResourceView* pDmolShaderResource = nullptr;
	ID3D11UnorderedAccessView* pDmolUnorderedAccess = nullptr;
	ID3D11Buffer* pDebugBuffer = nullptr;

	pOutputData = ExecuteComputeShaderProgram(hInstance, adapterIndex, hWnd, pDataBuffer,
		pConstantBuffer, fName, functionName, pDebugBuffer, pDMOLComputeShader,pDmolShaderResource, pDmolUnorderedAccess);

	return hr;
}

DirectXComputeShaderManagementClass::DirectXComputeShaderManagementClass()
{

}




DirectXComputeShaderManagementClass::DirectXComputeShaderManagementClass(HINSTANCE hInstance, HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		: m_pMainComputeShaderClass(nullptr),
		m_pResourceBuffer(nullptr),
		m_pComputeShaderResourceView(nullptr),
		ppComputeShaderDataStorage(nullptr),
		m_pDisplacementMap(nullptr),
		m_pDisplacementShaderResourceView(nullptr),
		m_pDisplacementUnorderedAccess(nullptr)
		
{
	m_pLoadedShaders = vector<ShaderModule>();
	m_pThreadPointerCollection = vector<HANDLE>();
	m_pThreadPointerCollection.resize(MAX_NUMBER_OF_MODELS);
	m_pD3DCompDevice = pDevice;
	m_pD3DCompContext = pContext;
	pAdapters = vector<IDXGIAdapter*>();
	pAdapters.resize(2);
	pComputationalFiberInstances = vector<LPVOID>();
	pComputationalFiberInstances.resize(5);
	pInternalProcessesCollection = vector<LPVOID>();
	pInternalProcessesCollection.resize(5);
	pInternalFibersCollection = vector<LPFIBERDATASTRUCT*>();
	pInternalFibersCollection.resize(5);
}



DirectXComputeShaderManagementClass::~DirectXComputeShaderManagementClass(void)
{
	SafeReleaseA(&m_pMainComputeShaderClass);
	SafeReleaseA(&m_pResourceBuffer);
	SafeReleaseA(&m_pComputeShaderResourceView);
	SafeReleaseA(&ppComputeShaderDataStorage);
	SafeReleaseA(&m_pDisplacementMap);
	SafeReleaseA(&m_pDisplacementShaderResourceView);
	SafeReleaseA(&m_pDisplacementUnorderedAccess);

}


void DirectXComputeShaderManagementClass::SetDebugName(ID3D11Resource* ppDeviceChild, char* debugName)
{
#if defined(DEBUG)
//	object->SetPrivateData(WKPDID_D3DDebugObjectName, (unsigned int)strlen(debugName), debugName); 
#endif
}

HRESULT DirectXComputeShaderManagementClass::CreateStructuredBufferA(UINT uElementSize, UINT uCount, bool bSRV, bool bUAV, void* pInitData, ID3D11Buffer* ppOutputBuffer, ID3D11ShaderResourceView* ppShaderResourceView, ID3D11UnorderedAccessView* ppUnordereredAccess)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = uElementSize * sizeof(D3D11_BUFFER_DESC);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = sizeof(pInitData);

	D3D11_SUBRESOURCE_DATA bufferInitData;
	ZeroMemory(&bufferInitData, sizeof(bufferInitData));
	bufferInitData.pSysMem = pInitData;
	hr = m_pD3DCompDevice->CreateBuffer(&bufferDesc, (pInitData) ? &bufferInitData : nullptr, &ppOutputBuffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.ElementWidth = uElementSize;
	hr = m_pD3DCompDevice->CreateShaderResourceView(ppOutputBuffer, &srvDesc, &ppShaderResourceView);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_APPEND;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.NumElements = uElementSize;

	hr = m_pD3DCompDevice->CreateUnorderedAccessView(ppOutputBuffer, &uavDesc, &ppUnordereredAccess);

	return hr;

}


void DirectXComputeShaderManagementClass::SetDebugName(ID3D11DeviceChild* object, char* debugName)
{
#if defined(DEBUG)
	object->SetPrivateData(WKPDID_D3DDebugObjectName, (unsigned int)strlen(debugName), debugName); 
#endif
}

ComputeShader* DirectXComputeShaderManagementClass::CreateComputeShaderObject(HWND hWnd, TCHAR* shaderFile, TCHAR* blobFileAppendix, char* pFunctionName, D3D10_SHADER_MACRO* pDefines, ID3D11ComputeShader* ppOutputComputeShader)
{
	ComputeShader* m_pComputeShader = new ComputeShader();

	bool bResult = m_pComputeShader->InitializeComputeShader(shaderFile,
		blobFileAppendix,
		pFunctionName, 
		pDefines, m_pD3DCompDevice, m_pD3DCompContext);

	if (m_pComputeShader && !bResult)
	{
		SafeDelete(m_pComputeShader);
	}

	return m_pComputeShader;
}





HRESULT DirectXComputeShaderManagementClass::StartModuleObject(HINSTANCE hInstance, HWND hWnd)
{

	HRESULT hr = S_OK;

	hr = InitializeComputeShaderInstances(hInstance, 4, hWnd, "StaticModelContainerCS.hlsl");

	if (SUCCEEDED(hr))
	{
		hr = StartComputeShaderExecution(hWnd);
	}

	MSG msg;

	while (GetMessage(&msg, hWnd, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	
	}
	
	return hr;
}


LPCSTR DirectXComputeShaderManagementClass::OnFileOpen(HINSTANCE hInstance, HWND hwnd1, HWND hWnd2)
{




	HRESULT hr = S_OK;


	// Show the File Open dialog.
	wchar_t path[MAX_PATH];
	path[0] = L'\0';

	OPENFILENAMEW ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd1;
	ofn.lpstrFilter = L"Windows Media\0*.wmv;*.wma;*.asf\0MP3\0*.mp3\0All files\0*.*\0";
	ofn.lpstrFile = path;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST;
	ofn.hInstance = hInstance;


	GetOpenFileNameW(&ofn);
	
	return (LPCSTR)ofn.lpstrFile;

}



HRESULT DirectXComputeShaderManagementClass::InitializeD3D11DeviceW()
{
	HRESULT hr = S_OK;




	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pDXGIFactory));




	UINT uCreationFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#ifdef _DEBUG
	uCreationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL flOut;
	static const D3D_FEATURE_LEVEL flvl[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };

	bool bNeedRefDevice = false;

	hr = D3D11CreateDevice(NULL,                        // Use default graphics card
		D3D_DRIVER_TYPE_HARDWARE,    // Try to create a hardware accelerated device
		nullptr,                        // Do not use external software rasterizer module
		uCreationFlags,              // Device creation flags
		flvl,
		sizeof(flvl) / sizeof(D3D_FEATURE_LEVEL),
		D3D11_SDK_VERSION,           // SDK version
		&m_pD3DCompDevice,               // Device out
		&flOut,                      // Actual feature level created
		&m_pD3DCompContext);              // Context out



	return hr;
}



BYTE* DirectXComputeShaderManagementClass::ExecuteComputeShader(HINSTANCE hInstance, int adapterIndex, HWND hWnd, LPCSTR fName, int x, int y, int z)
{
	HRESULT hr = S_OK;
	BYTE* pData = new BYTE[235];
	ID3D11Buffer* pConstantBuffer = nullptr;
	ID3D11ComputeShader* pComputeShader = nullptr;
	ID3D11ShaderResourceView* pComputeShaderSRV = nullptr;
	ID3D11UnorderedAccessView* pComputeShaderUAV = nullptr;
	ID3D11Buffer* pDebugBuffer = nullptr;

	hr = InitializeComputeShaderFromFileW(hWnd, adapterIndex, fName, "cs_5_0", "CS_Main", NULL,
		m_pD3DCompDevice,
		m_pD3DCompContext,
 		m_pMainComputeShaderClass);

	if (!pComputeShader)
	{
		MessageBoxA(hWnd, (LPCSTR)"Не удалось загрузить шейдер", (LPCSTR)"Message", 0);
	}

	if (SUCCEEDED(hr))
	{
		MessageBoxA(hWnd, (LPCSTR)"Compute Shader загружен в память", (LPCSTR)"Message", 0);

		hr = CreateStructuredBufferA(MAX_NUMBER_OF_AMBIENT_EFFECTS, 1, true, true, nullptr,
			m_pResourceBuffer, pComputeShaderSRV, pComputeShaderUAV);
	}

	if (FAILED(hr))
	{
		MessageBoxA(hWnd, (LPCSTR)"Не удалось создать константный буффер", (LPCSTR)"Message", 0);
	}

	if (SUCCEEDED(hr))
	{
		MessageBoxA(hWnd, (LPCSTR)"Создан константный буффер", (LPCSTR)"Message", 0);
		pConstantBuffer = CreateAndCopyToDebugBuf(m_pD3DCompDevice, m_pD3DCompContext, pDebugBuffer);
	}

	if (SUCCEEDED(hr))
	{
		RunComputeShaderSingle(m_pD3DCompContext, pComputeShader,
			pComputeShaderSRV, pConstantBuffer, nullptr, 0, pComputeShaderUAV,
			x, y, z);
	}

	
	ID3D11Buffer* pResultBuffer = nullptr;
	BYTE* pResultData = nullptr;

	m_pD3DCompContext->CopyResource(pResultBuffer, pDebugBuffer);

	BYTE* electrostaticsData;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	m_pD3DCompContext->Map(pDebugBuffer, NULL, D3D11_MAP_READ, NULL, &mappedResource);
	pResultData = (BYTE*)mappedResource.pData;

	StopComputeShaderExecution();


	return pResultData;

}







BYTE* DirectXComputeShaderManagementClass::ExecuteComputeShaderProgram(HINSTANCE hInstance, UINT adapterIndex, HWND hWnd, void* pCSData, ID3D11Buffer* pConstantBuffer, LPCSTR fName, LPCSTR functionName, ID3D11Buffer* pDebugBuffer, ID3D11ComputeShader* pComputeShader, ID3D11ShaderResourceView* pComputeSRV, ID3D11UnorderedAccessView* pComputeUAV)
{
	HRESULT hr = S_OK;
	ID3D11Buffer *pcbReadData;
	vector<BYTE*> ppOutputData;
	BYTE* pResultData;


	hr = InitializeComputeShaderFromFile(hWnd, adapterIndex, (TCHAR*)fName, _T("cs_5_0"), (char*)functionName, nullptr, pComputeShader);
	
	if (FAILED(hr))
	{
		MessageBoxA(hWnd, (LPCSTR)"Ваш Compute Shader Составлен Неверно", (LPCSTR)"Message", 0);
		pResultData = 0;
		return pResultData;
	}

	if (SUCCEEDED(hr))
	{
		hr = CreateStructuredBufferA(MAX_NUMBER_OF_AMBIENT_EFFECTS, 1, true, true, nullptr,
			pConstantBuffer, pComputeSRV, pComputeUAV);
	}

	if (SUCCEEDED(hr))
	{
		pDebugBuffer = CreateAndCopyToDebugBuf(m_pD3DCompDevice, m_pD3DCompContext, pConstantBuffer);
	}

	if (SUCCEEDED(hr))
	{
		RunComputeShaderSingle(m_pD3DCompContext,
			pComputeShader, pComputeSRV, pDebugBuffer, pCSData,
			sizeof(pDebugBuffer),
			pUnorderedAccessViewW, 0, 0, 0);

	}

	if (sizeof(ppOutputData) == 0)
	{
		MessageBoxA(hWnd, (LPCSTR)"Не удалось Выполнить вычисление", (LPCSTR)"Message", 0);
	}

	
	ID3D11Buffer* pResultBuffer = nullptr;

	m_pD3DCompContext->CopyResource(pResultBuffer, pDebugBuffer);

	BYTE* electrostaticsData;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	m_pD3DCompContext->Map(pDebugBuffer, NULL, D3D11_MAP_READ, NULL, &mappedResource);
	pResultData = (BYTE*)mappedResource.pData;

	StopComputeShaderExecution();


	return pResultData;
}


HRESULT DirectXComputeShaderManagementClass::InitializeComputeShaderFromFile(HWND hWnd, UINT adapterIndex, TCHAR* shaderFile, TCHAR* blobFileAppendix, char* pFunctionName, D3D10_SHADER_MACRO* pDefines, ID3D11ComputeShader* ppComputeShader)
{
	HRESULT hr = S_OK;


	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrorBlob = nullptr;

	FILE* fShaderBlob = nullptr;

	LPCSTR blobFileName[300];

	


	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS |
		D3DCOMPILE_IEEE_STRICTNESS |
		//D3DCOMPILE_WARNINGS_ARE_ERRORS |
		D3DCOMPILE_PREFER_FLOW_CONTROL;


#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	dwShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

	if (fShaderBlob == nullptr)
	{
		hr = D3DCompileFromFile(shaderFile, pDefines, D3D_COMPILE_STANDARD_FILE_INCLUDE, pFunctionName, "cs_5_0",
			dwShaderFlags, NULL, &pCompiledShader, &pErrorBlob);
		if (hr == S_OK)
		{
			if (blobFileAppendix != nullptr)
			{
			
				if (fShaderBlob != nullptr)
				{
					size_t size = pCompiledShader->GetBufferSize();
					fwrite(&size, sizeof(size_t), 1, fShaderBlob);
					fwrite(pCompiledShader->GetBufferPointer(), size, 1, fShaderBlob);
					fclose(fShaderBlob);
				}
			}
		}
	}
	else
	{
		int size = 0;
		fread_s(&size, sizeof(int), sizeof(int), 1, fShaderBlob);

		hr = D3DCreateBlob(size, &pCompiledShader);

		if (hr == S_OK)
		{
			fread_s(pCompiledShader->GetBufferPointer(), size, size, 1, fShaderBlob);
		}

		fclose(fShaderBlob);
	}

	if (pErrorBlob)
	{
		OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
	}

	if (hr == S_OK)
	{
		hr = m_pD3DCompDevice->CreateComputeShader(pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(), nullptr, &ppComputeShaderDataStorage);

		SafeReleaseA(&pErrorBlob);
		SafeReleaseA(&pCompiledShader);

	}

	return hr;
}


HRESULT DirectXComputeShaderManagementClass::InitializeComputeShaderInstances(HINSTANCE hInstance, HWND hWnd, UINT adapterIndex)
{
	HRESULT hr = S_OK;
	void* pCSData = nullptr;
	
	 adapterIndex = 3;
	try
	{
		hr = CreateSimulationBuffers(m_pD3DCompDevice);

		if (FAILED(hr))
		{
			return hr;
		}
		
		try
		{

			BYTE* pComputationOutput = nullptr;
			pComputationOutput = ExecuteComputeShaderProgram(hInstance, adapterIndex, hWnd, pCSData, pStaticModelConstantBuffer, "D:\DirectXGPUDigitalLabSoftware2015\DirectXGPUDigitalLabSoftware\Debug\EmptyComputeShaderCS.hlsl", "CS_Main",
				pStaticModelStagingBuffer, m_pStaticModelComputeShader, ppStaticModelShaderResource,
				ppStaticModelsUnorderedAccess);

		}

		catch (std::exception ex)
		{
			throw ex;
		}

		if (FAILED(hr))
		{
			return hr;
		}

		hr = InitializeComputeShaderFromFileW(hWnd, adapterIndex, (LPCSTR)"SkinnedModelContainerCS.hlsl",
			(LPCSTR)"cs_5_0", "CS_Main", nullptr, m_pD3DCompDevice, m_pD3DCompContext, m_pSkinnedModelComputeShader);
		
		if (FAILED(hr))
		{
			return hr;
		}

		hr = InitializeComputeShaderFromFileW(hWnd, adapterIndex, (LPCSTR)"TerrainContainerCS.hlsl",
			(LPCSTR)"cs_5_0", "CS_Main", nullptr, m_pD3DCompDevice, m_pD3DCompContext, m_pTerrainLandscapeComputeShader);

		if (FAILED(hr))
		{
			return hr;
		}

		hr = InitializeComputeShaderFromFileW(hWnd, adapterIndex, (LPCSTR)"ParticleSystemContainerCS.hlsl",
			(LPCSTR)"cs_5_0", "CS_Main", nullptr, m_pD3DCompDevice, m_pD3DCompContext, m_pParticleSystemComputeShader);

		if (FAILED(hr))
		{
			return hr;
		}

		hr = InitializeComputeShaderFromFileW(hWnd, adapterIndex, (LPCSTR)"ShadowMapContainerCS.hlsl",
			(LPCSTR)"cs_5_0", "CS_Main", nullptr, m_pD3DCompDevice, m_pD3DCompContext, m_pShadowMappingComputeShader);
		if (FAILED(hr))
		{
			return hr;
		}

		adapterIndex = adapterIndex + 1;

		hr = InitializeComputeShaderFromFileW(hWnd, adapterIndex, (LPCSTR)"AmbientOcclusionContainerCS.hlsl",
			(LPCSTR)"cs_5_0", "CS_Main", nullptr, m_pD3DCompDevice, m_pD3DCompContext, m_pAmbientOcclusionComputeShader);

		if (FAILED(hr))
		{
			return hr;
		}

		adapterIndex = adapterIndex + 1;

		hr = InitializeComputeShaderFromFileW(hWnd, adapterIndex, (LPCSTR)"GameLevelManagementCS.hlsl",
			(LPCSTR)"cs_5_0", "CS_Main", nullptr, m_pD3DCompDevice, m_pD3DCompContext, m_pGameLevelManagerComputeShader);
		
		if (FAILED(hr))
		{
			return hr;
		}

		adapterIndex = adapterIndex + 1;

		hr = InitializeComputeShaderFromFileW(hWnd, adapterIndex, (LPCSTR)"GameLevelManagementObjectProcessorCS.hlsl",
			(LPCSTR)"cs_5_0", "CS_Main", nullptr, m_pD3DCompDevice, m_pD3DCompContext, m_pGameLevelObjectProcessorComputeShader);
		
		if (FAILED(hr))
		{
			return hr;
		}

		adapterIndex = adapterIndex + 1;

		hr = InitializeComputeShaderFromFileW(hWnd, adapterIndex, (LPCSTR)"GameLevelManagementTesselatorCS.hlsl",
			(LPCSTR)"cs_5_0", "CS_Main", nullptr, m_pD3DCompDevice, m_pD3DCompContext, m_pGameLevelTesselatorComputeShader);

		if (FAILED(hr))
		{
			return hr;
		}

		adapterIndex = adapterIndex + 1;


		hr = InitializeComputeShaderFromFileW(hWnd, adapterIndex, (LPCSTR)"CollisionDetectionManagement.hlsl",
			(LPCSTR)"cs_5_0", "CS_Main", nullptr, m_pD3DCompDevice, m_pD3DCompContext, m_pCollisionDataComputeShader);

		if (FAILED(hr))
		{
			return hr;
		}

		adapterIndex = adapterIndex + 1;

		hr = InitializeComputeShaderFromFileW(hWnd, adapterIndex, (LPCSTR)"ScenarioRenderingCS.hlsl",
			(LPCSTR)"cs_5_0", "CS_Main", nullptr, m_pD3DCompDevice, m_pD3DCompContext, m_pScenarioRenderingComputeShader);

		if (FAILED(hr))
		{
			return hr;
		}

		m_pComputeShaderCollection.push_back(m_pStaticModelComputeShader);
		m_pComputeShaderCollection.push_back(m_pSkinnedModelComputeShader);
		m_pComputeShaderCollection.push_back(m_pAmbientOcclusionComputeShader);
		m_pComputeShaderCollection.push_back(m_pShadowMappingComputeShader);
		m_pComputeShaderCollection.push_back(m_pParticleSystemComputeShader);
		m_pComputeShaderCollection.push_back(m_pSkeletalAnimationComputeShader);
		m_pComputeShaderCollection.push_back(m_pTerrainLandscapeComputeShader);
		m_pComputeShaderCollection.push_back(m_pGameLevelManagerComputeShader);
		m_pComputeShaderCollection.push_back(m_pGameLevelObjectProcessorComputeShader);
		m_pComputeShaderCollection.push_back(m_pGameLevelTesselatorComputeShader);
		m_pComputeShaderCollection.push_back(m_pScenarioRenderingComputeShader);


	}
	catch (std::exception ex)
	{
		throw ex;
	}
	

}






HRESULT DirectXComputeShaderManagementClass::StartComputeShaderExecution(HWND hWnd)
{
	HRESULT hr = S_OK;

	MessageBoxA(hWnd, (LPCSTR)"Мы начинаем запуск рендерной машины", (LPCSTR)"Message", 0);

	for (UINT nIndex = 0; nIndex < MAX_NUMBER_OF_MODELS; ++nIndex)
	{
		
	


		m_pD3DCompContext->CSSetShader(m_pComputeShaderCollection[nIndex], nullptr, 0);
		m_pD3DCompContext->CSSetConstantBuffers(nIndex, 1, &m_pConstantBufferCollection[nIndex]);
		m_pD3DCompContext->CSSetShaderResources(nIndex, 1, &m_pShaderResourceViewCollection[nIndex]);
		m_pD3DCompContext->CSSetUnorderedAccessViews(nIndex, 1, &m_pUnorderedAccessViewCollection[nIndex], 0);

		try
		{
			m_pD3DCompContext->Dispatch(MAX_NUMBER_OF_THREADS, MAX_NUMBER_OF_THREADS, MAX_NUMBER_OF_THREADS);

			m_pMutexCollection[nIndex] = CreateMutex(NULL, FALSE, NULL);

			m_pThreadPointerCollection[nIndex] = CreateThread(nullptr,
				0,
				(LPTHREAD_START_ROUTINE)ProcessComputeShaderData,
				nullptr,
				0,
				(LPDWORD)nIndex);
		}
		catch (std::exception ex)
		{
			throw ex;
		}
	}

	
}


HRESULT DirectXComputeShaderManagementClass::StopComputeShaderExecution()
{
	HRESULT hr = S_OK;

	for (UINT nIndex = 0; nIndex < MAX_NUMBER_OF_THREADS; ++nIndex)
	{
		hr = CloseHandle(m_pThreadPointerCollection[nIndex]);
	}

	for (UINT nIndex = 0; nIndex < MAX_NUMBER_OF_THREADS; ++nIndex)
	{
		try
		{
		
			m_pD3DCompContext->CSSetShader(nullptr, nullptr, 0);
		
			ID3D11UnorderedAccessView* ppUAViewnullptr[1] = { nullptr };
			m_pD3DCompContext->CSSetUnorderedAccessViews( 0, 1, ppUAViewnullptr, nullptr );

			ID3D11ShaderResourceView* ppSRVnullptr[2] = { nullptr, nullptr };
			m_pD3DCompContext->CSSetShaderResources( 0, 2, ppSRVnullptr );

			ID3D11Buffer* ppCBnullptr[1] = { nullptr };
			m_pD3DCompContext->CSSetConstantBuffers( 0, 1, ppCBnullptr );
		}
		catch (std::exception ex)
		{
			throw ex;
		}
	}

}


HRESULT DirectXComputeShaderManagementClass::InitializeD3D11DeviceHardware(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr = S_OK;

	if (m_pD3DCompDevice != nullptr)
	{
		SafeRelease(&m_pD3DCompDevice);
	}
	if (m_pD3DCompContext != nullptr)
	{
		SafeRelease(&m_pD3DCompContext);
	}
	if (m_pD3DCompDevice1 != nullptr)
	{
		SafeRelease(&m_pD3DCompDevice1);
	}
	if (m_pD3DCompContext1 != nullptr)
	{
		SafeRelease(&m_pD3DCompDevice1);
	}


	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pDXGIFactory));
	pAdapters = vector<IDXGIAdapter*>();
	pAdapters.clear();
	if (SUCCEEDED(hr))
	{
		for (UINT i = 0;
			pDXGIFactory->EnumAdapters(i, &pDXGIAdapter) != DXGI_ERROR_NOT_FOUND;
			++i)
		{
			pAdapters.push_back(pDXGIAdapter);
		}

	}



	UINT uCreationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	uCreationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL flOut;
	static const D3D_FEATURE_LEVEL flvl[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };

	bool bNeedRefDevice = false;

	hr = D3D11CreateDevice(pAdapters[0],                        // Use default graphics card
		D3D_DRIVER_TYPE_UNKNOWN,    // Try to create a hardware accelerated device
		nullptr,                        // Do not use external software rasterizer module
		uCreationFlags,              // Device creation flags
		flvl,
		sizeof(flvl) / sizeof(D3D_FEATURE_LEVEL),
		D3D11_SDK_VERSION,           // SDK version
		&m_pD3DCompDevice,               // Device out
		&flOut,
		// Actual feature level created
		&m_pD3DCompContext);              // Context out
	if (FAILED(hr))
	{

		hr = D3D11CreateDevice(pAdapters[1],
			D3D_DRIVER_TYPE_UNKNOWN,
			0,
			uCreationFlags,
			flvl,
			ARRAYSIZE(flvl),
			D3D11_SDK_VERSION,
			&m_pD3DCompDevice,
			&flOut,
			&m_pD3DCompContext);
	}
	if (FAILED(hr))
	{
		hr = D3D11CreateDevice(pAdapters[2],
			D3D_DRIVER_TYPE_UNKNOWN,
			0,
			uCreationFlags,
			flvl,
			ARRAYSIZE(flvl),
			D3D11_SDK_VERSION,
			&m_pD3DCompDevice,
			&flOut,
			&m_pD3DCompContext);
	}


	return hr;
}



HRESULT DirectXComputeShaderManagementClass::InitializeD3D11Device(HWND hWnd, int adapterIndex)
{
	HRESULT hr = S_OK;

	if (m_pD3DCompDevice != nullptr)
	{
		SafeRelease(&m_pD3DCompDevice);
	}
	if (m_pD3DCompContext != nullptr)
	{
		SafeRelease(&m_pD3DCompContext);
	}
	if (m_pD3DCompDevice1 != nullptr)
	{
		SafeRelease(&m_pD3DCompDevice1);
	}
	if (m_pD3DCompContext1 != nullptr)
	{
		SafeRelease(&m_pD3DCompDevice1);
	}


	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pDXGIFactory));
	pAdapters = vector<IDXGIAdapter*>();
	pAdapters.clear();
	if (SUCCEEDED(hr))
	{
		for (UINT i = 0;
			pDXGIFactory->EnumAdapters(i, &pDXGIAdapter) != DXGI_ERROR_NOT_FOUND;
			++i)
		{
			pAdapters.push_back(pDXGIAdapter);
		}

	}



	UINT uCreationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	uCreationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL flOut;
	static const D3D_FEATURE_LEVEL flvl[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };

	bool bNeedRefDevice = false;

	hr = D3D11CreateDevice(NULL,                        // Use default graphics card
		D3D_DRIVER_TYPE_HARDWARE,    // Try to create a hardware accelerated device
		nullptr,                        // Do not use external software rasterizer module
		uCreationFlags,              // Device creation flags
		flvl,
		sizeof(flvl) / sizeof(D3D_FEATURE_LEVEL),
		D3D11_SDK_VERSION,           // SDK version
		&m_pD3DCompDevice,               // Device out
		&flOut,
		// Actual feature level created
		&m_pD3DCompContext);

	if (FAILED(hr))
	{
		hr = D3D11CreateDevice(pAdapters[0],                        // Use default graphics card
			D3D_DRIVER_TYPE_UNKNOWN,    // Try to create a hardware accelerated device
			nullptr,                        // Do not use external software rasterizer module
			uCreationFlags,              // Device creation flags
			flvl,
			sizeof(flvl) / sizeof(D3D_FEATURE_LEVEL),
			D3D11_SDK_VERSION,           // SDK version
			&m_pD3DCompDevice,               // Device out
			&flOut,
			// Actual feature level created
			&m_pD3DCompContext);
	}

	
	     // Context out
		if (FAILED(hr))
		{

			hr = D3D11CreateDevice(pAdapters[1],
				D3D_DRIVER_TYPE_UNKNOWN,
				0,
				uCreationFlags,
				flvl,
				ARRAYSIZE(flvl),
				D3D11_SDK_VERSION,
				&m_pD3DCompDevice,
				&flOut,
				&m_pD3DCompContext);
		}
		if (FAILED(hr))
		{
			hr = D3D11CreateDevice(pAdapters[2],
				D3D_DRIVER_TYPE_UNKNOWN,
				0,
				uCreationFlags,
				flvl,
				ARRAYSIZE(flvl),
				D3D11_SDK_VERSION,
				&m_pD3DCompDevice,
				&flOut,
				&m_pD3DCompContext);
		}


	return hr;
}





HRESULT DirectXComputeShaderManagementClass::InitializeComputeShaderInstances(HINSTANCE hInstance, UINT adapterIndex, HWND hWnd, LPCSTR fName)
{

	HRESULT hr = S_OK;

	ID3D11Buffer* m_pCostantBufferCS = nullptr;
	ID3D11Buffer* m_pOutputBuffer = nullptr;
	void* pCSData = nullptr;
	BYTE* pData = new BYTE[600];

	ID3D11ShaderResourceView* m_pShaderResourceViewCS = nullptr;
	ID3D11UnorderedAccessView* m_pUnordererdAccessViewCS = nullptr;
	ID3D11ComputeShader* m_pComputeShaderCS = nullptr;

	if (!m_pD3DCompDevice)
	{
		hr = InitializeD3D11Device(hWnd, adapterIndex);
	}

	pData = ExecuteComputeShaderProgram(hInstance, adapterIndex, hWnd, pCSData, m_pCostantBufferCS,
										fName, "CS_Main", m_pOutputBuffer,
										m_pComputeShaderCS, m_pShaderResourceViewCS,
										m_pUnordererdAccessViewCS);
	
	return hr;
 }







void DirectXComputeShaderManagementClass::SetComputeShader()
{
	m_pD3DCompContext->CSSetShader(m_pMainComputeShaderClass, nullptr, 0);
}

void DirectXComputeShaderManagementClass::UnsetComputeShader()
{
	m_pD3DCompContext->CSSetShader(nullptr, nullptr, 0);
}


ComputeBuffer* DirectXComputeShaderManagementClass::CreateComputationalBuffer(COMPUTE_BUFFER_TYPE uType, UINT uElementSize, UINT uCount, bool bSRV, bool bUAV, VOID* pInitData, bool bCreateStaging /* = false */, char* debugName /* = nullptr */)
{
	HRESULT hr = S_OK;

	ComputeBuffer* m_pComputeBuffer = new ComputeBuffer();
	m_pComputeBuffer->m_pDefferedContext = m_pD3DCompContext;

	if (uType == STRUCTURED_BUFFER)
	{
		hr = CreateStructuredBufferA(uElementSize, uCount, bSRV, bUAV, pInitData, m_pComputeBuffer->m_pResourceBuffer, m_pComputeBuffer->m_pShaderResource, m_pComputeBuffer->m_pUnordereredAccess);
	}
	
	else if (uType == RAW_BUFFER)
	{
		hr = CreateRawDataBufferA(uElementSize, pInitData, &m_pComputeBuffer->m_pResourceBuffer);
	}

	if (m_pComputeBuffer->m_pResourceBuffer != nullptr)
	{

		if (bSRV)
		{
			hr = CreateBufferSRV(m_pD3DCompDevice, m_pComputeBuffer->m_pResourceBuffer, &m_pComputeBuffer->m_pShaderResource);
		}
		
		if (bUAV)
		{
			hr = CreateBufferUAV(m_pD3DCompDevice, m_pComputeBuffer->m_pResourceBuffer, 0, &m_pComputeBuffer->m_pUnordereredAccess);
		}

		if (bCreateStaging)
		{
			m_pComputeBuffer->m_pStagingBuffer = CreateStagingBuffer(uElementSize * uCount);
		}
	}

	if (debugName)
	{
		if (m_pComputeBuffer->m_pResourceBuffer) SetDebugName(m_pComputeBuffer->m_pResourceBuffer, debugName);
		if (m_pComputeBuffer->m_pStagingBuffer) SetDebugName(m_pComputeBuffer->m_pStagingBuffer, debugName);
		if (m_pComputeBuffer->m_pShaderResource) SetDebugName(m_pComputeBuffer->m_pShaderResource, debugName);
		if (m_pComputeBuffer->m_pUnordereredAccess) SetDebugName(m_pComputeBuffer->m_pUnordereredAccess, debugName);
	}
	
	return m_pComputeBuffer;
}


ID3D11Buffer* DirectXComputeShaderManagementClass::CreateStructuredBuffer(UINT uElementSize, UINT uCount,
												  bool bSRV, bool bUAV, VOID* pInitData)
{
	ID3D11Buffer* pOutputBuffer = nullptr;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = 0;

	if (bUAV)
	{
		desc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
	}

	if (bSRV)
	{
		desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
	}

	UINT bufferSize = uElementSize * uCount;
	desc.ByteWidth = bufferSize < 16 ? 16 : bufferSize;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = uElementSize;

	if (pInitData)
	{
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = pInitData;
		m_pD3DCompDevice->CreateBuffer(&desc, &initData, &pOutputBuffer);
	}
	else
	{
		m_pD3DCompDevice->CreateBuffer(&desc, nullptr, &pOutputBuffer);
	}

	return pOutputBuffer;
}


ID3D11ShaderResourceView* DirectXComputeShaderManagementClass::CreateBufferSRV(ID3D11Buffer* pBuffer)
{
	ID3D11ShaderResourceView* ppShaderResourceView = nullptr;

	D3D11_BUFFER_DESC descBuf;
	ZeroMemory(&descBuf, sizeof(descBuf));
	pBuffer->GetDesc(&descBuf);

	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	desc.BufferEx.FirstElement = 0;

	if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
	{
		desc.Format = DXGI_FORMAT_R32_TYPELESS;
		desc.BufferEx.Flags - D3D11_BUFFEREX_SRV_FLAG_RAW;
		desc.BufferEx.NumElements = descBuf.ByteWidth / 4;
	}
	else if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
	{
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.BufferEx.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;
	}

	else
	{
		return nullptr;
	}

	m_pD3DCompDevice->CreateShaderResourceView(m_pResourceBuffer, &desc, &ppShaderResourceView);

	return ppShaderResourceView;
}


HRESULT DirectXComputeShaderManagementClass::CreateBufferUnorderedAccessViewA(ID3D11Buffer* pBuffer, ID3D11UnorderedAccessView **ppUnordereredAccess)
{
	HRESULT hr = S_OK;

	ID3D11UnorderedAccessView* ppUnorderedAccessViewOut = nullptr;

	D3D11_BUFFER_DESC descBuf;
	ZeroMemory(&descBuf, sizeof(descBuf));
	pBuffer->GetDesc(&descBuf);

	D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	desc.Buffer.FirstElement = 0;

	if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
	{
		desc.Format = DXGI_FORMAT_R32_TYPELESS;
		desc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
		desc.Buffer.NumElements = descBuf.ByteWidth / 4;
	}
	else if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
	{
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.Buffer.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;
	}
	else
	{
		return hr;
	}

	hr = m_pD3DCompDevice->CreateUnorderedAccessView(m_pResourceBuffer, &desc, &ppUnorderedAccessViewOut);

	return hr;
}



HRESULT DirectXComputeShaderManagementClass::CreateBufferUAV(ID3D11Buffer* pBuffer, UINT pSize, ID3D11UnorderedAccessView** ppUAV)
{
	HRESULT hr = S_OK;
	ID3D11UnorderedAccessView* ppUnorderedAccessViewOut = nullptr;

	D3D11_BUFFER_DESC descBuf;
	ZeroMemory(&descBuf, sizeof(descBuf));
	pBuffer->GetDesc(&descBuf);

	D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	desc.Buffer.FirstElement = 23;

	if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
	{
		desc.Format = DXGI_FORMAT_R32_TYPELESS;
		desc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
		desc.Buffer.NumElements = descBuf.ByteWidth / 4;
	}
	else if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
	{
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.Buffer.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;
	}
	else
	{
		return hr;
	}

	m_pD3DCompDevice->CreateUnorderedAccessView(pBuffer, &desc, &ppUnorderedAccessViewOut);
	*ppUAV = ppUnorderedAccessViewOut;
	return hr;
}


ID3D11Buffer* DirectXComputeShaderManagementClass::CreateStagingBuffer(UINT uSize)
{
	ID3D11Buffer* debugbuf = nullptr;

	D3D11_BUFFER_DESC desc;

	ZeroMemory(&desc, sizeof(desc));

	desc.ByteWidth = uSize;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;

	m_pD3DCompDevice->CreateBuffer(&desc, nullptr, &debugbuf);

	return debugbuf;
}




HRESULT DirectXComputeShaderManagementClass::CreateStagingBufferW(ID3D11Buffer* ppInputBuffer, ID3D11Buffer* ppOutbutBuffer, UINT size)
{

	HRESULT hr = S_OK;
	ID3D11Buffer* pInputBuf = nullptr;
	ID3D11Buffer* debugBuf = nullptr;


	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	ppInputBuffer->GetDesc(&desc);

	desc.CPUAccessFlags - D3D11_CPU_ACCESS_READ;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;

	hr = m_pD3DCompDevice->CreateBuffer(&desc, nullptr, &debugBuf);

	if (SUCCEEDED(hr))
	{
		m_pD3DCompContext->CopyResource(debugBuf, ppInputBuffer);
	}


	return hr;
}




ComputeTexture* DirectXComputeShaderManagementClass::CreateComputationalTexture(DXGI_FORMAT dxFormat, UINT uWidth,
																				UINT uHeight, UINT uRowPitch,
																				VOID* pInitData, bool bCreateStaging, char* debugName)
{
	HRESULT hr = S_OK;
	ComputeTexture* m_pComputeTexture = new ComputeTexture();
	m_pComputeTexture->m_pDefferedContext = m_pD3DCompContext;
	m_pComputeTexture->m_pTextureResource = CreateTextureResource(dxFormat, uWidth, uHeight, uRowPitch, pInitData);

	if (m_pComputeTexture->m_pTextureResource != nullptr)
	{
		m_pComputeTexture->m_pShaderResourceView = CreateTextureSRV(m_pComputeTexture->m_pTextureResource);
		m_pComputeTexture->m_pUnorderedAccessView = CreateTextureUAV(m_pComputeTexture->m_pTextureResource);
	
		if (bCreateStaging)
		{
			m_pComputeTexture->m_pStagingTexture = CreateStagingTexture(m_pComputeTexture->m_pTextureResource);
		}
	}

	if (debugName)
	{
		if (m_pComputeTexture->m_pTextureResource)
		{
			SetDebugName(m_pComputeTexture->m_pTextureResource, debugName);
		}

		if (m_pComputeTexture->m_pStagingTexture)
		{
			SetDebugName(m_pComputeTexture->m_pStagingTexture, debugName);
		}

		if (m_pComputeTexture->m_pShaderResourceView)
		{
			SetDebugName(m_pComputeTexture->m_pShaderResourceView, debugName);
		}

		if (m_pComputeTexture->m_pUnorderedAccessView)
		{
			SetDebugName(m_pComputeTexture->m_pUnorderedAccessView, debugName);
		}
	}

	return m_pComputeTexture;


}


ComputeTexture* DirectXComputeShaderManagementClass::CreateComputationalTextureFromFile(TCHAR* textureFilename, char* debugName /* = nullptr */)
{
	ComputeTexture* m_pComputeTexture = new ComputeTexture();
	m_pComputeTexture->m_pDefferedContext = m_pD3DCompContext;

	return nullptr;
}


ID3D11Texture2D* DirectXComputeShaderManagementClass::CreateTextureResource(DXGI_FORMAT dxFormat,
																			UINT uWidth, UINT uHeight, UINT uRowPitch, VOID* pInitData)
{
	HRESULT hr = S_OK;

	ID3D11Texture2D* pTexture = nullptr;

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = uWidth;
	desc.Height = uHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = dxFormat;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = pInitData;
	data.SysMemPitch = uRowPitch;

	hr = m_pD3DCompDevice->CreateTexture2D(&desc, &data, &pTexture);

	if (FAILED(hr))
	{
		return nullptr;
	}

	return pTexture;
}


ID3D11ShaderResourceView* DirectXComputeShaderManagementClass::CreateTextureSRV(ID3D11Texture2D* pTexture)
{
	HRESULT hr = S_OK;

	ID3D11ShaderResourceView* pShaderResourceView = nullptr;

	D3D11_TEXTURE2D_DESC textureDesc;
	pTexture->GetDesc(&textureDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(viewDesc));

	viewDesc.Format = textureDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	viewDesc.Texture2D.MipLevels= textureDesc.MipLevels;

	hr = m_pD3DCompDevice->CreateShaderResourceView(pTexture, &viewDesc, &pShaderResourceView);

	if (FAILED(hr))
	{

	}

	return pShaderResourceView;
}


ID3D11UnorderedAccessView* DirectXComputeShaderManagementClass::CreateTextureUAV(ID3D11Texture2D* pTexture)
{
	HRESULT hr = S_OK;
	ID3D11UnorderedAccessView* pUnorderedAccessView = nullptr;
	
	hr = m_pD3DCompDevice->CreateUnorderedAccessView(pTexture, nullptr, &pUnorderedAccessView);
	SafeReleaseA(&pTexture);

	return pUnorderedAccessView;


}


ID3D11Texture2D* DirectXComputeShaderManagementClass::CreateStagingTexture(ID3D11Texture2D* pTexture)
{
	HRESULT hr = S_OK;

	ID3D11Texture2D* pStagingTexture = nullptr;

	D3D11_TEXTURE2D_DESC desc;
	pTexture->GetDesc(&desc);

	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;
	
	hr = m_pD3DCompDevice->CreateTexture2D(&desc, nullptr, &pStagingTexture);

	return pStagingTexture;
}


ID3D11Buffer* DirectXComputeShaderManagementClass::CreateConstantBuffer(UINT uSize, VOID* pInitData, char* debugName /* = nullptr */)
{
	
	ID3D11Buffer* pBuffer = nullptr;

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	bool addMod = uSize % 16 != 0 ? true : false;

	cbDesc.ByteWidth = uSize + (addMod ? (16 - uSize % 16) : 0);
	cbDesc.CPUAccessFlags = 0;
	
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;

	if (pInitData)
	{
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = pInitData;
		m_pD3DCompDevice->CreateBuffer(&cbDesc, &initData, &pBuffer);
	}
	else
	{
		m_pD3DCompDevice->CreateBuffer(&cbDesc, nullptr, &pBuffer);
	}

	if (debugName && pBuffer)
	{
		SetDebugName(pBuffer, debugName);
	}

	return pBuffer;

}



ID3D11Buffer* DirectXComputeShaderManagementClass::CreateDynamicBuffer(UINT uSize, VOID* pInitData, char* debugName /* = nullptr */)
{
	ID3D11Buffer* ppDynamicBuffer = nullptr;

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	bool addMod = uSize % 16 != 0 ? true : false;
	
	cbDesc.ByteWidth = uSize + (addMod ? (16 - uSize % 16) : 0);
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;

	if (pInitData)
	{
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = pInitData;
		m_pD3DCompDevice->CreateBuffer(&cbDesc, &initData, &ppDynamicBuffer);
	}
	else
	{
		m_pD3DCompDevice->CreateBuffer(&cbDesc, nullptr, &ppDynamicBuffer);
	}

	return ppDynamicBuffer;

}


HRESULT DirectXComputeShaderManagementClass::InitializeComputeShaderFromFileW(HWND hWnd, UINT adapterIndex, LPCSTR shaderFile, LPCSTR blobFileAppendix, LPCSTR pFunctionName, D3D_SHADER_MACRO pDefines[2], ID3D11Device* md3ddevice, ID3D11DeviceContext* ppDeviceContext, ID3D11ComputeShader* ppComputeShader)
{
	HRESULT hr = S_OK;
	
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(_DEBUG)
		dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

		LPCSTR pProfile = ( m_pD3DCompDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0 ) ? "cs_5_0" : "cs_4_0";

		ID3DBlob* ppShaderBlob = NULL;
		ID3DBlob* pErrorBlob = NULL;

		dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS |
			D3DCOMPILE_IEEE_STRICTNESS |
			D3DCOMPILE_PREFER_FLOW_CONTROL;

#if defined(DEBUG) || defined(_DEBUG)
		dwShaderFlags |= D3DCOMPILE_DEBUG;
		dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		dwShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;

#endif
		hr = D3DCompileFromFile((TCHAR*)shaderFile, pDefines, D3D_COMPILE_STANDARD_FILE_INCLUDE, "CS_Main", "cs_5_0", dwShaderFlags, NULL, &ppShaderBlob, &pErrorBlob);

		if (SUCCEEDED(hr))
		{
			MessageBoxA(hWnd, (LPCSTR)"Шейдер загружен в память", (LPCSTR)"Message", 0);
			std::cout << "Loading Data Completed" << endl;
		}

		if (FAILED(hr))
		{
			MessageBoxA(hWnd, (LPCSTR)"Не удалось скомилировать шейер", (LPCSTR)"Message", 0);
			if (pErrorBlob)
				OutputDebugString((LPCWSTR)pErrorBlob->GetBufferPointer());
			if (pErrorBlob)
				SafeReleaseA(&pErrorBlob);
			if (ppShaderBlob)
				SafeReleaseA(&ppShaderBlob);
			return hr;
		}
		else
		{
			hr = m_pD3DCompDevice->CreateComputeShader(ppShaderBlob->GetBufferPointer(), ppShaderBlob->GetBufferSize(),
				NULL, &ppComputeShader);
			if (pErrorBlob)
				SafeReleaseA(&pErrorBlob);
			if (ppShaderBlob)
				SafeReleaseA(&ppShaderBlob);
			return hr;
		}

}

HRESULT DirectXComputeShaderManagementClass::CreateOutputBufferA(HWND hWnd, UINT uType, UINT uCount, bool bSRV, bool UAV, VOID* pInitData, bool bCreateStaging, ID3D11Buffer** ppOutputBuffer, char* debugName /* = nullptr */)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC desc;

	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS |
					D3D11_BIND_SHADER_RESOURCE;
	desc.ByteWidth = sizeof(m_pResourceBuffer);
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = 4;

	hr = m_pD3DCompDevice->CreateBuffer(&desc, NULL, ppOutputBuffer);
	
	if (FAILED(hr))
	{
		return hr;
	}

	D3D11_BUFFER_DESC descBuf;
	ZeroMemory(&descBuf, sizeof(descBuf));

	m_pResourceBuffer->GetDesc(&descBuf);

	D3D11_UNORDERED_ACCESS_VIEW_DESC descView;
	ZeroMemory(&descView, sizeof(descView));

	descView.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	descView.Buffer.FirstElement = 0;

	descView.Format = DXGI_FORMAT_UNKNOWN;
	descView.Buffer.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;

	hr = m_pD3DCompDevice->CreateUnorderedAccessView(pResource, nullptr, &ppUnordereredAccess);

	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}






HRESULT DirectXComputeShaderManagementClass::CreateStructuredBufferW(UINT uElementSize, UINT uCount, bool bSRV, bool bUAV, ID3D11Buffer** ppBuffer,
																	 ID3D11ShaderResourceView** ppSRV,
																	 ID3D11UnorderedAccessView** ppUAV,
																	 const D3D11_SUBRESOURCE_DATA* pInitData)

{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = uCount * sizeof(D3D11_BUFFER_DESC);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	D3D11_SUBRESOURCE_DATA bufferInitData;
	ZeroMemory(&bufferInitData, sizeof(bufferInitData));
	bufferDesc.StructureByteStride = sizeof(bufferInitData);

	bufferInitData.pSysMem = &bufferInitData;

	hr = m_pD3DCompDevice->CreateBuffer(&bufferDesc, (pInitData) ? &bufferInitData : nullptr, ppBuffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.ElementWidth = uElementSize;
	hr = m_pD3DCompDevice->CreateShaderResourceView(*ppBuffer, &srvDesc, ppSRV);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.NumElements = uElementSize;
	hr = m_pD3DCompDevice->CreateUnorderedAccessView(*ppBuffer, &uavDesc, ppUAV);

	return hr;
}




void DirectXComputeShaderManagementClass::DrawFullScreenQuad11(ID3D11DeviceContext* pd3dImmediateContext,
																  ID3D11PixelShader* pPixelShader,
																  UINT Width,
																  UINT Height)
{
	D3D11_VIEWPORT viewPorts[D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX];
	UINT nViewPorts = 1;
	pd3dImmediateContext->RSGetViewports(&nViewPorts, viewPorts);


	D3D11_VIEWPORT viewport;

	viewport.Width = (float)Width;
	viewport.Height = (float)Height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	
	pd3dImmediateContext->RSSetViewports(1, &viewport);

	UINT offsets = 0;
	ID3D11Buffer* pBufferCollection[1] = {m_pScreenQuadVB};

	pd3dImmediateContext->IASetInputLayout(m_pScreenQuadInputLayout);
	pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pd3dImmediateContext->VSSetShader(m_pSceneVertexShader, nullptr, 0);
	pd3dImmediateContext->PSSetShader(pPixelShader, nullptr, 0);
	pd3dImmediateContext->Draw(4, 0);
	
	pd3dImmediateContext->RSSetViewports(nViewPorts, viewPorts);

}



ID3D11Buffer* DirectXComputeShaderManagementClass::CreateAndCopyToDebugBuf(ID3D11Device* pDevice, ID3D11DeviceContext* pd3dImmediateContext, ID3D11Buffer* pBuffer)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* debugbuf = NULL;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	pBuffer->GetDesc(&desc);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;
	hr = m_pD3DCompDevice->CreateBuffer(&desc, NULL, &debugbuf);
	if (SUCCEEDED(hr))
	{
		m_pD3DCompContext->CopyResource(debugbuf, pBuffer);
	}
	

	return debugbuf;
}


_Use_decl_annotations_
ID3D11Buffer* DirectXComputeShaderManagementClass::CreateAndCopyToDebugBuf(ID3D11Device* pDevice, ID3D11DeviceContext* pd3dImmediateContext, UINT pElementAmount, ID3D11Buffer* pBuffer)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* debugbuf = NULL;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	pBuffer->GetDesc(&desc);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;

	hr = m_pD3DCompDevice->CreateBuffer(&desc, NULL, &debugbuf);
	if (SUCCEEDED(hr))
	{
		m_pD3DCompContext->CopyResource(debugbuf, pBuffer);
	}

	
	return debugbuf;
}

void DirectXComputeShaderManagementClass::SimulateComputeShader(ID3D11DeviceContext* pd3dImmediateContext,
	ID3D11ComputeShader* pComputeShader,
	UINT nNumViews, ID3D11ShaderResourceView** pShaderResourceViews,
	ID3D11Buffer* pCBCS, void* pCSData, DWORD dwNumDataBytes,
	ID3D11UnorderedAccessView* pUnorderedAccessView,
	UINT X, UINT Y, UINT Z)
{
	pd3dImmediateContext->CSSetShader(pComputeShader, nullptr, 0);
	pd3dImmediateContext->CSSetShaderResources(0, nNumViews, pShaderResourceViews);
	pd3dImmediateContext->CSSetUnorderedAccessViews(0, 1, &pUnorderedAccessView, nullptr);
	if (pCBCS && pCSData)
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		pd3dImmediateContext->Map(pCBCS, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		memcpy(MappedResource.pData, pCSData, dwNumDataBytes);
		pd3dImmediateContext->Unmap(pCBCS, 0);
		ID3D11Buffer* ppCB[1] = { pCBCS };
		pd3dImmediateContext->CSSetConstantBuffers(0, 1, ppCB);
	}

	pd3dImmediateContext->Dispatch(X, Y, Z);

	pd3dImmediateContext->CSSetShader(nullptr, nullptr, 0);

	ID3D11UnorderedAccessView* ppUAViewnullptr[1] = { nullptr };
	pd3dImmediateContext->CSSetUnorderedAccessViews(0, 1, ppUAViewnullptr, nullptr);

	ID3D11ShaderResourceView* ppSRVnullptr[2] = { nullptr, nullptr };
	pd3dImmediateContext->CSSetShaderResources(0, 2, ppSRVnullptr);

	ID3D11Buffer* ppCBnullptr[1] = { nullptr };
	pd3dImmediateContext->CSSetConstantBuffers(0, 1, ppCBnullptr);
}



HRESULT DirectXComputeShaderManagementClass::CreateBufferSRV( ID3D11Device* pDevice, ID3D11Buffer* pBuffer, ID3D11ShaderResourceView** ppSRVOut )
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC descBuf;
	ZeroMemory(&descBuf, sizeof(descBuf));
	pBuffer->GetDesc(&descBuf);

	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	desc.BufferEx.FirstElement = 0;
	

	if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
	{
		
		desc.Format = DXGI_FORMAT_R32_TYPELESS;
		desc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
		desc.BufferEx.NumElements = descBuf.ByteWidth / 4;
	}
	else if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
	{
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.BufferEx.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;
	}
	else
	{
		hr = E_INVALIDARG;
		return hr;
	}
	
	hr = pDevice->CreateShaderResourceView(pBuffer, &desc, ppSRVOut);

	return hr;

}






HRESULT DirectXComputeShaderManagementClass::CreateRawDataBufferA(UINT uElementSize, void* pInitData, ID3D11Buffer** ppRawDataBuffer, char* debugName /* = nullptr */)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* ppDataBuffer = nullptr;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_INDEX_BUFFER | D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = uElementSize;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

	if (pInitData)
	{
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = pInitData;
		hr = m_pD3DCompDevice->CreateBuffer(&desc, &initData, &ppDataBuffer);
	}
	else
	{
		hr = m_pD3DCompDevice->CreateBuffer(&desc, nullptr, &ppDataBuffer);
	}

	*ppRawDataBuffer = ppDataBuffer;
	return hr;
}


HRESULT DirectXComputeShaderManagementClass::CreateDynamicBufferA(HWND hWnd, UINT uType, UINT uElementSize, UINT uCount, bool* bSRV, bool bUAV, VOID* ppInitiData, bool bCreateStaging, ID3D11Buffer* pOutputBuffer, char* debugName)
{
	ID3D11UnorderedAccessView* m_destDataGPUBufferView = nullptr;
	HRESULT hr = S_OK;
	// The compute shader will need to output to some buffer so here we create a GPU buffer for that.
	D3D11_BUFFER_DESC descGPUBuffer;
	ZeroMemory(&descGPUBuffer, sizeof(descGPUBuffer));
	descGPUBuffer.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	descGPUBuffer.ByteWidth = uElementSize;
	descGPUBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	descGPUBuffer.StructureByteStride = 4;	// We assume the output data is in the RGBA format, 8 bits per channel

	hr = m_pD3DCompDevice->CreateBuffer(&descGPUBuffer, NULL, &pOutputBuffer);
		return false;

	// The view we need for the output is an unordered access view. This is to allow the compute shader to write anywhere in the buffer.
	D3D11_BUFFER_DESC descBuf;
	ZeroMemory(&descBuf, sizeof(descBuf));
	pOutputBuffer->GetDesc(&descBuf);

	D3D11_UNORDERED_ACCESS_VIEW_DESC descView;
	ZeroMemory(&descView, sizeof(descView));
	descView.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	descView.Buffer.FirstElement = 0;

	descView.Format = DXGI_FORMAT_UNKNOWN;      // Format must be must be DXGI_FORMAT_UNKNOWN, when creating a View of a Structured Buffer
	descView.Buffer.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;
	hr = m_pD3DCompDevice->CreateUnorderedAccessView(pOutputBuffer, &descView, &m_destDataGPUBufferView);

	

	return hr;
}








HRESULT DirectXComputeShaderManagementClass::CreateStructuredBuffer(ID3D11Device* pd3dDevice, 
																	UINT iNumElements,
																	ID3D11Buffer** ppBuffer,
																	ID3D11ShaderResourceView** ppSRV,
																	ID3D11UnorderedAccessView** ppUAV, 
																	const D3D11_SUBRESOURCE_DATA* pInitialData /* = nullptr */)
{
	HRESULT hr = S_OK;
	
	D3D11_BUFFER_DESC bufferDesc;
	
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = iNumElements * sizeof(D3D11_BUFFER_DESC);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = sizeof(pInitialData);

	D3D11_SUBRESOURCE_DATA bufferInitData;
	ZeroMemory(&bufferInitData, sizeof(bufferInitData));
	bufferInitData.pSysMem = pInitialData;
	hr = m_pD3DCompDevice->CreateBuffer(&bufferDesc, (pInitialData) ? &bufferInitData : nullptr, ppBuffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.ElementWidth = iNumElements;
	hr = m_pD3DCompDevice->CreateShaderResourceView(*ppBuffer, &srvDesc, ppSRV);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.NumElements = iNumElements;
	hr = m_pD3DCompDevice->CreateUnorderedAccessView(*ppBuffer, &uavDesc, ppUAV);

	


	return hr;


}


HRESULT DirectXComputeShaderManagementClass::CreateSimulationBuffers( ID3D11Device* pd3dDevice )
{
	HRESULT hr = S_OK;
	int numberOfBuffers = MAX_NUMBER_OF_MEMORY_BUFFERS;

	for (UINT iIndex = 0; iIndex < numberOfBuffers; ++iIndex)
	{
		ID3D11Buffer* pModelConstantBuffer = nullptr;
		ID3D11ShaderResourceView* pModelShaderResource = nullptr;
		ID3D11UnorderedAccessView* pModelUnordereredAccess = nullptr;
		hr = CreateStructuredBuffer(pd3dDevice, 3, &pModelConstantBuffer, &pModelShaderResource, &pModelUnordereredAccess, nullptr);

		if (SUCCEEDED(hr))
		{
			StructuredBufferClass* m_pModelStructuredBuffer = new StructuredBufferClass(
				pModelConstantBuffer, pModelShaderResource, pModelUnordereredAccess);

			m_pStructuredBufferList.push_back(m_pModelStructuredBuffer);
		}

		if (FAILED(hr))
		{
			break;
		}

		pModelConstantBuffer = nullptr;
		pModelShaderResource = nullptr;
		pModelUnordereredAccess = m_pMainUnordereredAccessView;
	}

	SetConstantBufferId(m_pStructuredBufferList[0], MODEL_BUFFER);
	SetConstantBufferId(m_pStructuredBufferList[1],  SKINNED_MODEL_BUFFER);
	SetConstantBufferId(m_pStructuredBufferList[2], TERRAIN_BUFFER);
	SetConstantBufferId(m_pStructuredBufferList[3], PARTICLE_SYSTEM_BUFFER);
	SetConstantBufferId(m_pStructuredBufferList[4], SHADOW_MAP_BUFFER);
	SetConstantBufferId(m_pStructuredBufferList[5], AMBIENT_OCCLUSION_BUFFER);
	SetConstantBufferId(m_pStructuredBufferList[6], GAME_LEVEL_MANAGER_BUFFER);
	SetConstantBufferId(m_pStructuredBufferList[7], GAME_LEVEL_OBJECT_PROCESSOR_BUFFER);
	SetConstantBufferId(m_pStructuredBufferList[8], GAME_LEVEL_MESH_RENDERER_BUFFER);
	SetConstantBufferId(m_pStructuredBufferList[7], GAME_LEVEL_OBJECT_PROCESSOR_BUFFER);

	return hr;
}



HRESULT DirectXComputeShaderManagementClass::SetConstantBufferId(StructuredBufferClass* pBuffer, std::string bufferId)
{
	HRESULT hr = S_OK;
	pBuffer->bufferId = bufferId;
	return hr;
}

HRESULT DirectXComputeShaderManagementClass::CreateConstantBufferA(HWND hWnd, UINT uSize, VOID* pInitData, ID3D11Buffer** ppOutputBufffer, char* debugName /* = nullptr */)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* pBuffer = nullptr;
	
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	bool addMod = uSize % 16 != 0 ? true : false;
	cbDesc.ByteWidth = uSize + (addMod ? (16 - uSize % 16) : 0);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	hr = m_pD3DCompDevice->CreateBuffer(&cbDesc, nullptr, ppOutputBufffer);

	return hr;
}



HRESULT DirectXComputeShaderManagementClass::CreateStructuredBufferForReading(UINT uElementSize, UINT uCount, void* pInitData, ID3D11Buffer** ppBufOut, ID3D11ShaderResourceView** pInputSRV)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* pInputBuffer = nullptr;
	ID3D11ShaderResourceView* pInputBufferSRV = nullptr;

	D3D11_BUFFER_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = uElementSize * sizeof(float);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags =  D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = 4;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	D3D11_SUBRESOURCE_DATA bufferInitData;
	ZeroMemory(&bufferInitData, sizeof(bufferInitData));
	bufferInitData.pSysMem = pInitData;

	if (bufferInitData.pSysMem)
	{
		hr = m_pD3DCompDevice->CreateBuffer(&bufferDesc, (bufferInitData.pSysMem) ? &bufferInitData : nullptr, &pInputBuffer);
	}
	else
	{
		hr = m_pD3DCompDevice->CreateBuffer(&bufferDesc, nullptr, &pInputBuffer);
	}

	D3D11_BUFFER_DESC descBuf;
	ZeroMemory(&descBuf, sizeof(descBuf));
	pInputBuffer->GetDesc(&descBuf);

	D3D11_SHADER_RESOURCE_VIEW_DESC descView;
	ZeroMemory(&descView, sizeof(descView));
	descView.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	descView.Buffer.FirstElement = 0;
	descView.Buffer.ElementWidth = 1;
	hr = m_pD3DCompDevice->CreateShaderResourceView(pInputBuffer, &descView, &pInputBufferSRV);

	if (SUCCEEDED(hr))
	{
		*ppBufOut = pInputBuffer;
		*pInputSRV = pInputBufferSRV;
	}

	return hr;
}





HRESULT DirectXComputeShaderManagementClass::CreateStructuredBufferForWriting(UINT uElementSize,  ID3D11Buffer** ppBufOut, ID3D11UnorderedAccessView** ppUAV)
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC destBufferDesc;
	ID3D11Buffer* pOutputBuffer = nullptr;
	ID3D11UnorderedAccessView* pUnorderedAccessView = nullptr;
	ZeroMemory(&destBufferDesc, sizeof(destBufferDesc));
	destBufferDesc.ByteWidth = uElementSize * sizeof(SpeedData);
	destBufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	destBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	destBufferDesc.StructureByteStride = 4;
	
	hr = m_pD3DCompDevice->CreateBuffer(&destBufferDesc, NULL, &pOutputBuffer);

	D3D11_BUFFER_DESC buffDesc;
	ZeroMemory(&buffDesc, sizeof(buffDesc));
	pOutputBuffer->GetDesc(&buffDesc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC descView;
	ZeroMemory(&descView, sizeof(descView));
	descView.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	descView.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_COUNTER;
	descView.Buffer.FirstElement = 0;
	descView.Buffer.NumElements = 1;
	descView.Format = DXGI_FORMAT_UNKNOWN;
	hr = m_pD3DCompDevice->CreateUnorderedAccessView(pOutputBuffer, &descView, &pUnorderedAccessView);

	if (SUCCEEDED(hr))
	{
		*ppBufOut = pOutputBuffer;
		*ppUAV = pUnorderedAccessView;
	}

	return hr;
}









HRESULT DirectXComputeShaderManagementClass::CreateStructuredBufferB(ID3D11Device* pDevice, UINT uElementSize, UINT uCount, void* pInitData, ID3D11Buffer** ppBufOut)
{
	ID3D11Buffer* pBufOut = nullptr;
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = uElementSize * uCount;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = uElementSize;



	D3D11_SUBRESOURCE_DATA bufferInitData;
	ZeroMemory(&bufferInitData, sizeof(bufferInitData));
	bufferInitData.pSysMem = pInitData;
	
	if (bufferInitData.pSysMem)
	{
		hr = m_pD3DCompDevice->CreateBuffer(&bufferDesc, (bufferInitData.pSysMem) ? &bufferInitData : nullptr, ppBufOut);
	}
	else
	{
		hr = pDevice->CreateBuffer(&bufferDesc, nullptr, ppBufOut);
	}
	*ppBufOut = pBufOut;
	return hr;
}


void DirectXComputeShaderManagementClass::RunComputeShader(ID3D11DeviceContext* pd3dImmediateContext,
	ID3D11ComputeShader* pComputeShader,
	UINT nNumViews, ID3D11ShaderResourceView** pShaderResourceViews,
	ID3D11Buffer* pCBCS, void* pCSData, DWORD dwNumDataBytes,
	ID3D11UnorderedAccessView* pUnorderedAccessView,
	UINT X, UINT Y, UINT Z)
{
	pd3dImmediateContext->CSSetShader(pComputeShader, nullptr, 0);
	pd3dImmediateContext->CSSetShaderResources(0, nNumViews, pShaderResourceViews);
	pd3dImmediateContext->CSSetUnorderedAccessViews(0, 1, &pUnorderedAccessView, nullptr);
	if (pCBCS && pCSData)
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		pd3dImmediateContext->Map(pCBCS, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		memcpy(MappedResource.pData, pCSData, dwNumDataBytes);
		pd3dImmediateContext->Unmap(pCBCS, 0);
		ID3D11Buffer* ppCB[1] = { pCBCS };
		pd3dImmediateContext->CSSetConstantBuffers(0, 1, ppCB);
	}

	pd3dImmediateContext->Dispatch(X, Y, Z);

	pd3dImmediateContext->CSSetShader(nullptr, nullptr, 0);

	ID3D11UnorderedAccessView* ppUAViewnullptr[1] = { nullptr };
	pd3dImmediateContext->CSSetUnorderedAccessViews(0, 1, ppUAViewnullptr, nullptr);

	ID3D11ShaderResourceView* ppSRVnullptr[2] = { nullptr, nullptr };
	pd3dImmediateContext->CSSetShaderResources(0, 2, ppSRVnullptr);

	ID3D11Buffer* ppCBnullptr[1] = { nullptr };
	pd3dImmediateContext->CSSetConstantBuffers(0, 1, ppCBnullptr);
}



HRESULT DirectXComputeShaderManagementClass::CreateBufferUAV( ID3D11Device* pDevice, ID3D11Buffer* pBuffer, UINT pSize, ID3D11UnorderedAccessView** ppUAVOut )
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC descBuf;
	ZeroMemory(&descBuf, sizeof(descBuf));
	pBuffer->GetDesc(&descBuf);
	ID3D11UnorderedAccessView* ppUnorderedAccessView = nullptr;
	D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_APPEND;
	desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	desc.Buffer.NumElements = pSize;
	desc.Buffer.FirstElement = 1;

	if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
	{
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_COUNTER;
		desc.Buffer.NumElements = pSize;
	}
	else if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
	{
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.Buffer.NumElements = pSize;
	}
	
	else if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
	{
		desc.Format = DXGI_FORMAT_UNKNOWN;
	}

	hr = pDevice->CreateUnorderedAccessView(pBuffer, &desc, &ppUnorderedAccessView);
	*ppUAVOut = ppUnorderedAccessView;
	return hr;
}

HRESULT DirectXComputeShaderManagementClass::DumpCurrentSceneToTexture( ID3D11DeviceContext* pd3dImmediateContext,
																	   DWORD dwWidth, 
																	   DWORD dwHeight, 
																	   ID3D11ShaderResourceView* pFromRV,
																	   ID3D11RenderTargetView* pToRTV )
{
	HRESULT hr = S_OK;
	
	ID3D11ShaderResourceView* pShaderResourceViews[1] = {pFromRV};
	pd3dImmediateContext->PSSetShaderResources(0, 1, pShaderResourceViews);
	
	ID3D11RenderTargetView* pRenderTargetViews[1] = {pToRTV};
	pd3dImmediateContext->OMSetRenderTargets(1, pRenderTargetViews, nullptr);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = pd3dImmediateContext->Map(pResource, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	UINT* pDataBuffer = reinterpret_cast<UINT*>(mappedResource.pData);

	pDataBuffer[0] = dwWidth;
	pDataBuffer[1] = dwHeight;

	pd3dImmediateContext->Unmap(pResource, 0);
	ID3D11Buffer* ppCurrentBuffers[1];
	pd3dImmediateContext->PSSetConstantBuffers(g_iBPSBind, 1, &m_pResourceBuffer);

	DrawFullScreenQuad11(pd3dImmediateContext, m_pScenePixelShader, dwWidth, dwWidth);

	return hr;
}







void DirectXComputeShaderManagementClass::RunComputeShader( ID3D11DeviceContext* pd3dImmediateContext,
														   vector<ID3D11ComputeShader*> pComputeShader,
														   vector<ID3D11ShaderResourceView*> pShaderResourceViews, 
														   vector<ID3D11Buffer*> pConstantBuffers, void* pCSData, DWORD dwNumDataBytes,
														   vector<ID3D11UnorderedAccessView*> pUnorderedAccessView,
														   UINT X, UINT Y, UINT Z )
{
	
	for (UINT pIndex = 0; pIndex < sizeof(pComputeShader); pIndex)
	{
		pd3dImmediateContext->CSSetShader(pComputeShader[pIndex], nullptr, 0);
		pd3dImmediateContext->CSSetConstantBuffers(0, sizeof(pConstantBuffers), &pConstantBuffers[pIndex]);
		pd3dImmediateContext->CSSetShaderResources(0, sizeof(pShaderResourceViews), &pShaderResourceViews[pIndex]);
		pd3dImmediateContext->CSSetUnorderedAccessViews(0, sizeof(ppUnorderedAccessViews), &ppUnorderedAccessViews[pIndex], nullptr);
	}
	

	for (DWORD pIndex = 0; pIndex < sizeof(pConstantBuffers); pIndex)
	{

		if (pCSData)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			pd3dImmediateContext->Map(pConstantBuffers[pIndex], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			memcpy(mappedResource.pData, pCSData, dwNumDataBytes);
			pd3dImmediateContext->Unmap(pConstantBuffers[pIndex], 0);
			ID3D11Buffer* ppCurrentBuffers[1] = {pConstantBuffers[pIndex]};
			pd3dImmediateContext->CSSetConstantBuffers(0, 1, ppCurrentBuffers);
		}

		pd3dImmediateContext->Dispatch(X, Y, Z);

		HANDLE pHandle = CreateThread(nullptr, 0,
			(LPTHREAD_START_ROUTINE)ProcessComputeShaderData,
			nullptr,
			0,
			&pIndex);

		pComputeShaderThreadHandlers.push_back(pHandle);
	}

	
}



void DirectXComputeShaderManagementClass::RunComputeShaderWithData(ID3D11DeviceContext* pd3dImmediateContext,
	ID3D11ComputeShader* pComputeShader,
	ID3D11ClassInstance* pClassInstance,
	ID3D11ShaderResourceView* pShaderResourceViews,
	ID3D11Buffer* pConstantBuffers,
	void* pCSData,
	DWORD dwNumDataBytes,
	ID3D11UnorderedAccessView* pUnorderedAccessView,
	UINT X, UINT Y, UINT Z)
{
	pd3dImmediateContext->CSSetShader(pComputeShader, &pClassInstance, 0);
	pd3dImmediateContext->CSSetShaderResources(0, 1, &pShaderResourceViews);
	pd3dImmediateContext->CSSetUnorderedAccessViews(0, 1, &pUnorderedAccessView, nullptr);

	if (pCSData)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		pd3dImmediateContext->Map(pConstantBuffers, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy(mappedResource.pData, pCSData, dwNumDataBytes);
		pd3dImmediateContext->Unmap(pConstantBuffers, 0);
		ID3D11Buffer* ppCurrentBuffers[1] = { pConstantBuffers };
		pd3dImmediateContext->CSSetConstantBuffers(0, 1, ppCurrentBuffers);
	}

	try
	{
		pd3dImmediateContext->Dispatch(X, Y, Z);
	}

	catch (std::exception ex)
	{
		throw ex;
	}

}

void DirectXComputeShaderManagementClass::RunComputeShaderSingle( ID3D11DeviceContext* pd3dImmediateContext,
																 ID3D11ComputeShader* pComputeShader,
																 ID3D11ShaderResourceView* pShaderResourceViews,
																 ID3D11Buffer* pConstantBuffers, 
																 void* pCSData,
																 DWORD dwNumDataBytes,
																 ID3D11UnorderedAccessView* pUnorderedAccessView, 
																 UINT X, UINT Y, UINT Z )
{
	pd3dImmediateContext->CSSetShader(pComputeShader, nullptr, 0);
	pd3dImmediateContext->CSSetShaderResources(0, sizeof(pShaderResourceViews), &pShaderResourceViews);
	pd3dImmediateContext->CSSetUnorderedAccessViews(0, sizeof(pUnorderedAccessView), &pUnorderedAccessView, nullptr);

	if (pCSData)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		pd3dImmediateContext->Map(pConstantBuffers, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy(mappedResource.pData, pCSData, dwNumDataBytes);
		pd3dImmediateContext->Unmap(pConstantBuffers, 0);
		ID3D11Buffer* ppCurrentBuffers[1] = {pConstantBuffers};
		pd3dImmediateContext->CSSetConstantBuffers(0, 1, ppCurrentBuffers);
	}

	try
	{
		pd3dImmediateContext->Dispatch(X, Y, Z);
	}
	
	catch (std::exception ex)
	{
		throw ex;
	}

}
