/**
 * This file is based on the DirectX.h from Amblone (amblone.com).
 * License: https://creativecommons.org/licenses/by-nc/3.0/
 *          https://creativecommons.org/licenses/by-nc/3.0/legalcode
*/
#pragma once
struct IDirect3D9;
struct IDirect3DDevice9;
struct IDirect3DSurface9;
struct IDirect3DTexture9;

class AmbloneDirectX {
public:
  AmbloneDirectX();
  ~AmbloneDirectX();
  /**returns false if init failed, true otherwise */
  bool initialize(int DeviceNum);
  /**Format: A8R8G8B8 */
  unsigned char*  getScreenBitmap();
  int getWidth() const;
  int getHeight() const;

private:
  bool initD3D(int DeviceNum);

  bool initialized;
  IDirect3D9*        pD3D;
  IDirect3DDevice9*  pd3dDevice;
  IDirect3DSurface9* pSurface;
  IDirect3DSurface9* pAvgSurface;
  int screenWidth;
  int screenHeight;
  long dataLength;
  unsigned char* pBits;
};
