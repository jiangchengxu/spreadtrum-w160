// CPicture.h: interface for the CPicture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICTURE_H__081D008A_5029_4C56_B80C_37793E323DF6__INCLUDED_)
#define AFX_PICTURE_H__081D008A_5029_4C56_B80C_37793E323DF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////
#include <stdio.h>

//------------------------------------------

typedef enum {
    OFM_WRITE        = 0,
    OFM_READ         = 1
} FILE_OPENMODE;

#define HIMETRIC_INCH 2540

//=-----------------------------
class CFileProcess
{
private :
    FILE  *m_File;

public:
    CFileProcess();
    ~CFileProcess();
    BOOL	Open        (TCHAR * FileName , FILE_OPENMODE Open_Mode);
    BOOL	Write       (VOID * zBuffer, DWORD cSize);
    BOOL	Read        (VOID * zBuffer, DWORD cSize);
    LONG    GetSize     (VOID);
    VOID    Close       (VOID);
};

//-------------------------------------------------------------


//-----------------------------------------
class CPicture
{
public:
    CPicture();
    virtual ~CPicture();

private:
    IPicture *m_pic;
    HGLOBAL  hGlobal;

public:

    HBITMAP LoadPicture(TCHAR * FileName);
    HBITMAP LoadPicture(TCHAR * FileName, HDC hdc);
    VOID    FreePicture( VOID );
    HDC     _GetDC     ( VOID );
    HBITMAP _GetHandle ( VOID );
    DWORD   _GetWidth  ( VOID );
    DWORD   _GetHeight ( VOID );
    BOOL    DrawPicture(HDC hdc,long x,long y,long cx,long cy);


};

#endif // !defined(AFX_PICTURE_H__081D008A_5029_4C56_B80C_37793E323DF6__INCLUDED_)
