//
// The following structs were copied out of the MSDN article
// "Icons in Win32".
// 
typedef struct { 
  BYTE  bWidth;        // Width of the image
  BYTE  bHeight;       // Height of the image (times 2)
  BYTE  bColorCount;   // Number of colors in image (0 if >=8bpp)
  BYTE  bReserved;     // Reserved
  WORD  wPlanes;       // Color Planes
  WORD  wBitCount;     // Bits per pixel
  DWORD dwBytesInRes;  // how many bytes in this resource?
  DWORD dwImageOffset; // where in the file is this image
} ICONDIRENTRY, *LPICONDIRENTRY;

typedef struct {
  WORD idReserved;           // Reserved
  WORD idType;               // resource type (1 for icons)
  WORD idCount;              // how many images?
  ICONDIRENTRY idEntries[1]; // the entries for each image
} ICONDIR, *LPICONDIR; 

typedef struct {
  BITMAPINFOHEADER  icHeader;      // DIB header
  RGBQUAD           icColors[1];   // Color table
  BYTE              icXOR[1];      // DIB bits for XOR mask
  BYTE              icAND[1];      // DIB bits for AND mask
} ICONIMAGE;

#ifdef _MSC_VER
#pragma pack(push)
#pragma pack(2)
#endif
typedef 
      struct {
  BYTE            bWidth;       // width, in pixels, of the image
  BYTE            bHeight;      // height, in pixels, of the image
  BYTE            bColorCount;  // Number of colors in image (0 if >= 8bpp)
  BYTE            bReserved;
  WORD            wPlanes;      // color planes
  WORD            wBitCount;    // bits per pixel
  DWORD           dwBytesInRes;
  WORD            nID;
} GRPICONDIRENTRY;
#ifdef _MSC_VER
#pragma pack(pop)

#pragma pack(push)
#pragma pack(2)
#endif
typedef 
     struct {
  WORD            idReserved;   // Reserved (must be 0).
  WORD            idType;       // Resource (must be 1 for icons)
  WORD            idCount;      // How many images?
  GRPICONDIRENTRY idEntries[1]; 
} GRPICONDIR;
#ifdef _MSC_VER
#pragma pack(pop)
#endif

