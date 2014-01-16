/**
 * This file is based on the DirectX.h from Amblone (amblone.com).
 * License: https://creativecommons.org/licenses/by-nc/3.0/
 *          https://creativecommons.org/licenses/by-nc/3.0/legalcode
*/
#pragma once
struct IDirect3D9;
struct IDirect3DDevice9;
struct IDirect3DSurface9;

class Screen
{
public:

  struct Pixel
  {
    unsigned char A;
    unsigned char R;
    unsigned char G;
    unsigned char B;
  };

  Screen();
  ~Screen();
  /**returns false if init failed, true otherwise */
  bool initialize(int DeviceNum);

  /** Copies a bitmap of the current screen content into @p pOutData
   * @param pOutData a pointer to a block of memory that is at least screenWidth*screenHeight*sizeof(Screen::Pixel) bytes large.
   *                 Pixel format is A8R8G8B8.
   */
  void getScreenBitmap(Pixel* pOutData) const;

  /**Returns the width of the screen */
  int getWidth() const;

  /**Returns the height of the screen */
  int getHeight() const;

private:
  /**Initialize the direct3d device which will later be used to take screenshots **/
  bool initD3D(int DeviceNum);

  bool initialized; /**<True if initialize() has been called successfully */
  IDirect3D9        *pD3D; //FIXME comments
  IDirect3DDevice9  *pd3dDevice;
  IDirect3DSurface9 *pSurface;
  int screenWidth;
  int screenHeight;
  long dataLength;
};
