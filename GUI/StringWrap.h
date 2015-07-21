#ifndef STRING_WRAP_H
#define STRING_WRAP_H

class CStringWrap
{
public:
	enum{MAX_WIDTH=64};
private:
	const char *m_pSourceStr;
	char * m_pDestStr;
	int m_iMaxWidth;
	int m_iLines;
	int m_iWidth;
	int *m_pPositions;
	int *m_pWidths;
	char m_cTempBuf[MAX_WIDTH+1];
public:
	int Lines(){return m_iLines;};	
	int Width(){return m_iWidth;};
	char *Line(int iLineID);
	int   LineWidth(int iLineID);
	void Wrap();
	void Clear();
	CStringWrap(const char * sourStr,int maxWidth);
	~CStringWrap();

	static void stringwrap(const char *sourcestr, 
						char *deststr,
						const int maxwidth,  
						int &lines,  
						int &width ,
						int *positions,
						int *widths);

};
#endif


