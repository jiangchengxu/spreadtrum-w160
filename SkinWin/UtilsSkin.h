
#ifndef _UTILSSKIN_H_
#define _UTILSSKIN_H_

CString GetPathName( const TCHAR * filename );
CString GetFileName( const TCHAR * filename, int ext = 0);
TCHAR *next_token( TCHAR *buf, TCHAR *token, TCHAR *stopchars );

#endif // _UTILSSKIN_H_