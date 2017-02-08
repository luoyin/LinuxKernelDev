#ifndef __CRELATIONHEADERFILE_H__
#define __CRELATIONHEADERFILE_H__

#include <string>
#include <list>
#include <map>
#include <iterator>
#include "CBaseList.h"

using std::string;
using std::list;
using std::map;
using std::iterator;
using std::pair;

typedef struct _s_RELA_HFILE{
	char path[256];
	char name[256];
	bool bRealFile;
	bool bVisited;
	CBaseList<struct _s_RELA_HFILE> *pIncluded;
	CBaseList<struct _s_RELA_HFILE> *pIncluding;
} s_RELA_HFILE;

class CRelationHeaderFile
{
public:
	CRelationHeaderFile();
	~CRelationHeaderFile();
	
	void SolveDir(const char *absPath, const char *indPath);
	void AddFile(const char *path, const char *name);
	void BuildRelation();
	void BuildRelationFile(s_RELA_HFILE *pFile);
	void GenGraphvizForIncluding(const char *name, FILE *fp);
	void ClearVisitInfo();
	
protected:
	bool _SolveIncludeLine(const char *pLine, char *incfile);
	void _RelesaeRelaHFile(s_RELA_HFILE *pFile);
	
protected:
	CBaseList<s_RELA_HFILE> *m_pListFile;
	map<string, s_RELA_HFILE *> *m_pMapFile;
};

#endif
