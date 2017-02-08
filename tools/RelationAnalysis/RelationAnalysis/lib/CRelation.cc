#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/stat.h>
#include "CRelation.h"

CRelation::CRelation()
{
	m_pFiles=new CBaseList<s_RELATION_FILE>();
	m_iMaxID=0;
}

CRelation::~CRelation()
{
	
}

void CRelation::SolveDir(const char* path)
{
	DIR *dp;
	struct dirent *filename;
	struct stat buf_stat;
	char ex[256], fnm[PATH_MAX];
	
	dp=NULL;
	dp=opendir(path);
	if(dp==NULL)
		return;
	
	while(filename=readdir(dp)) {
		strcpy(fnm, path);
		strcat(fnm, "/");
		strcat(fnm, filename->d_name);
		stat(fnm, &buf_stat);
		if(buf_stat.st_mode & S_IFREG) {
			// 常规文件
			_FileExt(filename->d_name, ex);
			if(strcmp(ex, "h")==0) {
				// .h文件
				_AddRelationFile(path, filename->d_name);
			}
		}
	}
	closedir(dp);
}

void CRelation::_AddRelationFile(const char* path, const char* filename)
{
	s_RELATION_FILE *pRelation;
	
	pRelation=(s_RELATION_FILE *)malloc(sizeof(s_RELATION_FILE));
	strcpy(pRelation->info.path, path);
	strcpy(pRelation->info.filename, filename);
	pRelation->info.bRealFile=true;
	pRelation->info.id=m_iMaxID++;
	pRelation->pIncluded=new CBaseList<s_RELATION_FILE>();
	pRelation->pIncluding=new CBaseList<s_RELATION_FILE>();
	
	m_pFiles->Append(pRelation);
	
}

void CRelation::_FileExt(const char* filename, const char* ex)
{
	int i, j, len;
	bool flag=false;
	
	j=0;
	len=strlen(filename);
	for(i=0; i<len; i++) {
		if(flag==true) {
			ex[j++]=filename[i];
		}
		if(filename[i]=='.')
			flag=true;
	}
	ex[j]=0;
}
