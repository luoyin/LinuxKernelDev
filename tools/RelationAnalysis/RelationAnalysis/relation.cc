#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/stat.h>
#include "lib/CRelationHeaderFile.h"

int main(int argc, char **argv)
{
	CRelationHeaderFile *pRelation;
	FILE *fp;
	
	pRelation=new CRelationHeaderFile();
	
	pRelation->SolveDir("/usr/src/linux-3.12.53-40/include", "linux");
	pRelation->BuildRelation();
	
	fp=fopen("g_pci.dot", "w");
	fprintf(fp, "digraph linux_pci_h {\n");
	pRelation->ClearVisitInfo();
	pRelation->GenGraphvizForIncluding("linux/pci.h", fp);
	fprintf(fp, "}\n");
	fclose(fp);
	
	return 0;
}

