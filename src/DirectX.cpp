/**
 * This file is based on the DirectX.cpp from Amblone (amblone.com).
 * License: https://creativecommons.org/licenses/by-nc/3.0/
 *          https://creativecommons.org/licenses/by-nc/3.0/legalcode
*/

#include <d3d9.h>
#include "DirectX.h"
#include <QtGlobal>
using namespace std;

#define BITSPERPIXEL 32 //Format is A8R8G8B8 => 32 bit per pixel

Screen::Screen() : initialized(false), pD3D(nullptr),
                                   pd3dDevice(nullptr), pSurface(nullptr),
                                   screenWidth(0), screenHeight(0), dataLength(0)
{}

Screen::~Screen() {
  if(!initialized) return;
  if(pSurface) pSurface->Release();
  if(pd3dDevice) pd3dDevice->Release();
  if(pD3D) pD3D->Release();
}

bool Screen::initialize(int deviceNum) {
  if (initialized) return true;
  pD3D = nullptr;
  pd3dDevice = nullptr;
  pSurface = nullptr;
  initialized = true;
  return initD3D(deviceNum);
}

bool Screen::initD3D(int deviceNum) {
	D3DDISPLAYMODE	ddm;
	D3DPRESENT_PARAMETERS	d3dpp;

  if((pD3D=Direct3DCreate9(D3D_SDK_VERSION))== nullptr)
	{
    qCritical("Screen: Call Direct3DCreate9(%d) failed", D3D_SDK_VERSION);
		return false;
	}
  HRESULT result = pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT + deviceNum, &ddm);
  if(FAILED(result))
	{
    qCritical("Screen: GetAdapterDisplayMode() for device %d failed", D3DADAPTER_DEFAULT + deviceNum);
		return false;
	}

	ZeroMemory(&d3dpp,sizeof(D3DPRESENT_PARAMETERS));
  RECT gScreenRect = {0, 0, 0, 0};

  d3dpp.Windowed = true;
  d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
  d3dpp.BackBufferFormat = ddm.Format;
  d3dpp.BackBufferHeight = screenHeight = gScreenRect.bottom =ddm.Height;
  d3dpp.BackBufferWidth = screenWidth = gScreenRect.right =ddm.Width;
  d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
  d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  d3dpp.hDeviceWindow = NULL;//if this handle is NULL the focus window will be taken (FIXME what happens if the foucs window is closed)
  d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
  d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

  dataLength = screenWidth * screenHeight * (BITSPERPIXEL / 8);

  result = pD3D->CreateDevice(D3DADAPTER_DEFAULT + deviceNum,
                              D3DDEVTYPE_HAL, NULL,
                              D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                              &d3dpp, &pd3dDevice);
  if(FAILED(result))
	{
    qCritical("Screen: CreateDevice() failed");
		return false;
	}
  result = pd3dDevice->CreateOffscreenPlainSurface(ddm.Width, ddm.Height,
                                                   D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH,
                                                   &pSurface, nullptr);
  if(FAILED(result))
	{
    qCritical("Screen: CreateDevice() failed");
		return false;
	}
	return true;
}

void Screen::getScreenBitmap(Pixel* pOutData) const {

  pd3dDevice->GetFrontBufferData(0, pSurface);
  D3DLOCKED_RECT lockedRect;
  //FIXME use scoped locking
  pSurface->LockRect(&lockedRect, nullptr, D3DLOCK_NO_DIRTY_UPDATE|D3DLOCK_NOSYSLOCK|D3DLOCK_READONLY);
    memcpy((unsigned char*)pOutData, (unsigned char*) lockedRect.pBits, dataLength);
  pSurface->UnlockRect();
}

int Screen::getHeight() const
{
  return screenHeight;
}

int Screen::getWidth() const
{
  return screenWidth;
}


