#ifndef __CRELATION_H__
#define __CRELATION_H__

#include <map>
#include <iterator>
#include <string>
#include "CBaseList.h"

using std::map;
using std::iterator;
using std::string;

typedef struct {
	char path[256];
	char filename[256];
	int id;
	bool bRealFile;
} s_RELATION_FILEINFO;

typedef struct {
	s_RELATION_FILEINFO info;
	CBaseList<s_RELATION_FILEINFO> *pIncluded;
	CBaseList<s_RELATION_FILEINFO> *pIncluding;
} s_RELATION_FILE;

class CRelation
{
public:
	CRelation();
	~CRelation();
	
	void SolveDir(const char *path);
	
protected:
	void _AddRelationFile(const char *path, const char *filename);
	void _FileExt(const char *filename, const char *ex);
	
protected:
	CBaseList<s_RELATION_FILE> *m_pFiles;
	int m_iMaxID;
};

#endif
