#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/stat.h>
#include "CRelationHeaderFile.h"
#include <errno.h>
#include <error.h>

CRelationHeaderFile::CRelationHeaderFile()
{
	m_pListFile=new CBaseList<s_RELA_HFILE>();
	m_pMapFile=new map<string, s_RELA_HFILE*>();
}

CRelationHeaderFile::~CRelationHeaderFile()
{
	s_RELA_HFILE *pFile;
	
	m_pMapFile->clear();
	delete m_pMapFile;
	
	pFile=m_pListFile->GetHead();
	while(pFile) {
		pFile->pIncluded->Clear();
		pFile->pIncluding->Clear();
		delete pFile->pIncluded;
		delete pFile->pIncluding;
		delete pFile;
		pFile=m_pListFile->GetNext();
	}
}

void CRelationHeaderFile::SolveDir(const char* absPath, const char* indPath)
{
	DIR *dp;
	struct dirent *dpelem;
	struct stat buf_stat;
	char fullpath[512], fnm[512], indPath2[256];
	
	strcpy(fullpath, absPath);
	strcat(fullpath, "/");
	strcat(fullpath, indPath);
	
	printf("%s\n", fullpath);
	
	dp=NULL;
	dp=opendir(fullpath);
	if(dp==NULL)
		return;
	
	while(dpelem=readdir(dp)) {
		strcpy(fnm, fullpath);
		strcat(fnm, "/");
		strcat(fnm, dpelem->d_name);
		stat(fnm, &buf_stat);
		if(dpelem->d_name[0]=='.')
			continue;
		if(buf_stat.st_mode & S_IFDIR) {
			strcpy(indPath2, indPath);
			strcat(indPath2, "/");
			strcat(indPath2, dpelem->d_name);
			SolveDir(absPath, indPath2);
		}
		if(buf_stat.st_mode & S_IFREG) {
			// printf("%s/%s/%s\n", absPath, indPath, dpelem->d_name);
			strcpy(fnm, indPath);
			strcat(fnm, "/");
			strcat(fnm, dpelem->d_name);
			AddFile(absPath, fnm);
		}
	}
	closedir(dp);
}

void CRelationHeaderFile::AddFile(const char* path, const char* name)
{
	s_RELA_HFILE *pRelaHFile;
	
	pRelaHFile=(s_RELA_HFILE *)malloc(sizeof(s_RELA_HFILE));
	strcpy(pRelaHFile->path, path);
	strcpy(pRelaHFile->name, name);
	pRelaHFile->bRealFile=true;
	pRelaHFile->pIncluded=new CBaseList<s_RELA_HFILE>();
	pRelaHFile->pIncluding=new CBaseList<s_RELA_HFILE>();
	
	m_pListFile->Append(pRelaHFile);
	m_pMapFile->insert(pair<string, s_RELA_HFILE *>(name, pRelaHFile));
}

void CRelationHeaderFile::BuildRelation()
{
	s_RELA_HFILE *pRelaHFile;
	
	pRelaHFile=m_pListFile->GetHead();
	while(pRelaHFile) {
		BuildRelationFile(pRelaHFile);
		pRelaHFile=m_pListFile->GetNext();
	}
}

void CRelationHeaderFile::BuildRelationFile(s_RELA_HFILE* pFile)
{
	FILE *fp;
	char line[256], fnm[PATH_MAX], incfile[256];
	map<string, s_RELA_HFILE *>::iterator iter;
	s_RELA_HFILE *pRelaHFile;
	
	if(pFile->bRealFile==false)
		return;
	
	strcpy(fnm, pFile->path);
	strcat(fnm, "/");
	strcat(fnm, pFile->name);
	
	fp=fopen(fnm, "r");
	while(1) {
		line[0]=0;
		fgets(line, 256, fp);
		if(feof(fp) || line[0]==0)
			break;
		if(_SolveIncludeLine(line, incfile)==true && strcmp(incfile, "")) {
			// #include行
			iter=m_pMapFile->find(incfile);
			if(iter==m_pMapFile->end()) {
				// 无此文件
				pRelaHFile=(s_RELA_HFILE *)malloc(sizeof(s_RELA_HFILE));
				strcpy(pRelaHFile->path, "");
				strcpy(pRelaHFile->name, incfile);
				pRelaHFile->bRealFile=false;
				pRelaHFile->pIncluded=new CBaseList<s_RELA_HFILE>();
				pRelaHFile->pIncluding=new CBaseList<s_RELA_HFILE>();
				
				m_pListFile->Append(pRelaHFile);
				m_pMapFile->insert(pair<string, s_RELA_HFILE *>(incfile, pRelaHFile));
			} else {
				// 有此文件
				pRelaHFile=iter->second;
			}
			pRelaHFile->pIncluded->Append(pFile);
			pFile->pIncluding->Append(pRelaHFile);
		}
	}
	fclose(fp);
}

bool CRelationHeaderFile::_SolveIncludeLine(const char* pLine, char* incfile)
{
	int i, j, len;
	bool flag, flag2;
	
	len=strlen(pLine);
	
	// 首字母判断
	for(i=0; i<len; i++) {
		if(pLine[i]==' ' || pLine[i]=='\t')
			continue;
		else if(pLine[i]=='\n')
			return false;
		else if(pLine[i]!='#')
			return false;
		else
			break;
	}
	if(strncmp(pLine+i, "#include", 8)!=0)
		return false;
	
	// 解析include行
	j=0;
	flag=false;
	flag2=false;
	for(i+=7; i<len; i++) {
		if(flag==true) {
			if(pLine[i]=='>' || pLine[i]=='"')
				break;
			if(pLine[i]!=' ' || pLine[i]!='\t')
				flag2=true;
			if(flag2==true) {
				if(pLine[i]==' ' || pLine[i]=='\t')
					break;
				incfile[j++]=pLine[i];
			}
		}
		if(pLine[i]=='<' || pLine[i]=='"')
			flag=true;
	}
	incfile[j]=0;
	return true;
}

void CRelationHeaderFile::_RelesaeRelaHFile(s_RELA_HFILE* pFile)
{
	pFile->pIncluded->Clear();
	pFile->pIncluding->Clear();
}

void CRelationHeaderFile::GenGraphvizForIncluding(const char* name, FILE *fp)
{
	map<string, s_RELA_HFILE*>::iterator iter;
	s_RELA_HFILE *pFile, *pFile2;
	
	iter=m_pMapFile->find(name);
	if(iter!=m_pMapFile->end()) {
		pFile=iter->second;
		if(pFile->bVisited==true)
			return;
		if(pFile->pIncluding->Length()==0)
			fprintf(fp, "    \"%s\" [style=\"filled\", fillcolor=\"green\"];\n", pFile->name);
		pFile->bVisited=true;
		fprintf(fp, "// %s\n", name);
		pFile2=pFile->pIncluding->GetHead();
		while(pFile2) {
			fprintf(fp, "    \"%s\" -> \"%s\";\n", pFile->name, pFile2->name);
			pFile2=pFile->pIncluding->GetNext();
		}
		
		pFile2=pFile->pIncluding->GetHead();
		while(pFile2) {
			GenGraphvizForIncluding(pFile2->name, fp);
			pFile2=pFile->pIncluding->GetNext();
		}
	}
}

void CRelationHeaderFile::ClearVisitInfo()
{
	s_RELA_HFILE *pFile;
	
	pFile=m_pListFile->GetHead();
	while(pFile) {
		pFile->bVisited=false;
		pFile=m_pListFile->GetNext();
	}
}
