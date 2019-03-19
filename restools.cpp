#include "restools.h"
#include <stdio.h>
#include <windows.h>

#include <QPixmap>
#include <QString>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QtWinExtras/QtWinExtras>
QPixmap GetFileIcon(QString path,QString savePath)
{
    QFileInfo fileInfo(path);
    QFileIconProvider icon;
    QPixmap pixmap = icon.icon(fileInfo).pixmap(32,32);
    pixmap.save(savePath);
    return pixmap;
}

//QPixmap GetFileIconEx(QString path,QString savePath)
//{
//    Q_GUI_EXPORT QPixmap qt_pixmapFromWinHICON(HICON icon);
//    SHFILEINFOA psfi;
//    SHGetFileInfoA(path.toStdString().c_str(),NULL,&psfi,sizeof(SHFILEINFOA),SHGFI_ICON);
//    QPixmap pixmap = qt_pixmapFromWinHICON(psfi.hIcon);
//    pixmap.save(savePath);
//    DestroyIcon(psfi.hIcon);
//    return pixmap;
//}

struct ICONDIRENTRY {
    BYTE bWidth;
    BYTE bHeight;
    BYTE bColorCount;
    BYTE bReserved;
    WORD wPlanes;
    WORD wBitCount;
    DWORD dwBytesInRes;
    DWORD dwImageOffset;
};

struct ICONDIR {
    WORD idReserved;
    WORD idType;
    WORD idCount;
//ICONDIRENTRY idEntries;
};

struct GRPICONDIRENTRY {
    BYTE bWidth;
    BYTE bHeight;
    BYTE bColorCount;
    BYTE bReserved;
    WORD wPlanes;
    WORD wBitCount;
    DWORD dwBytesInRes;
    WORD nID;
};

struct GRPICONDIR {
    WORD idReserved;
    WORD idType;
    WORD idCount;
    GRPICONDIRENTRY idEntries;
};

//////////////////////////////////////////////
//函数说明：修改EXE图标
//
//参    数：IconFile 图标文件
//              ExeFile 被修改的EXE文件
//
//返回值： 成功为True，否则False
/////////////////////////////////////////////
bool ChangeExeIcon(LPCSTR IconFile, LPCSTR ExeFile) {
    ICONDIR stID;
    ICONDIRENTRY stIDE;
    GRPICONDIR stGID;
    HANDLE hFile;
    DWORD nSize, nGSize, dwReserved;
    HANDLE hUpdate;
    PBYTE pIcon, pGrpIcon;
    BOOL ret;

    hFile = CreateFileA(IconFile, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return false;
    }

    ZeroMemory(&stID, sizeof(ICONDIR));
    ret = ReadFile(hFile, &stID, sizeof(ICONDIR), &dwReserved, NULL);

    ZeroMemory(&stIDE, sizeof(ICONDIRENTRY));
    ret = ReadFile(hFile, &stIDE, sizeof(ICONDIRENTRY), &dwReserved, NULL);

    nSize = stIDE.dwBytesInRes;
    pIcon = (PBYTE) malloc(nSize);
    SetFilePointer(hFile, stIDE.dwImageOffset, NULL, FILE_BEGIN);
    ret = ReadFile(hFile, (LPVOID) pIcon, nSize, &dwReserved, NULL);
    if (!ret) {
        CloseHandle(hFile);
        return false;
    }

    ZeroMemory(&stGID, sizeof(GRPICONDIR));
    stGID.idCount = stID.idCount;
    stGID.idReserved = 0;
    stGID.idType = 1;
    CopyMemory(&stGID.idEntries, &stIDE, 12);
    stGID.idEntries.nID = 0;

    nGSize = sizeof(GRPICONDIR);
    pGrpIcon = (PBYTE) malloc(nGSize);
    CopyMemory(pGrpIcon, &stGID, nGSize);

    hUpdate = BeginUpdateResourceA(ExeFile, false);
    ret = UpdateResource(hUpdate, RT_GROUP_ICON, MAKEINTRESOURCE(1), 0, (LPVOID) pGrpIcon, nGSize);
    ret = UpdateResource(hUpdate, RT_ICON, MAKEINTRESOURCE(1), 0, (LPVOID) pIcon, nSize);
    EndUpdateResource(hUpdate, false);
    if (!ret) {
        CloseHandle(hFile);
        return false;
    }

    CloseHandle(hFile);
    return true;
}
