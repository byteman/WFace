#ifndef RESTOOLS_H
#define RESTOOLS_H
#include <Windows.h>
#include <QPixmap>
class ResTools
{
public:
    ResTools();
};
bool ChangeExeIcon(LPCSTR IconFile, LPCSTR ExeFile);
QPixmap GetFileIcon(QString path,QString savePath);

#endif // RESTOOLS_H
