/**
 * @file		CBaseList.h
 * @author		Luo, Yin
 * @version	V1.0
 * @date		2016-01-25
 * @brief		XKANAL.SYSTEM--工具类--基本链表(头文件)
 */

#ifndef __CBASELIST_H__
#define __CBASELIST_H__

#include <stdio.h>
#include <malloc.h>
#include <string.h>

template <typename T>
class CBaseList
{
public:
	static const int NOEXIST=-1;
	static const int HEAD=-2;
	static const int TAIL=-3;
	
	static const int ERR_OK=0;
	static const int ERR_NOEXIST=1;
	static const int ERR_FORBIDDEN=2;
	
protected:
	static const int tHEAD=0;
	static const int tTAIL=1;
	static const int tELEM=2;
	
	typedef struct _LIST {
		T *ptr;
		int nodeType;
		_LIST *prev;
		_LIST *next;
	} LIST, *PLIST;
	
public:
	explicit CBaseList();
	explicit CBaseList(const T* s[], const int count);
	~CBaseList();
	void Append(T* elem);
	void AppendCopy(const T* elem);
	void AppendCopyEx(const T* elem, void (*funcCopy)(T*, T*));
	void Show(void (*funcShow)(T *)) const;
	int GetIndex(const T* elem) const;
	int InsertBefore(T* elem, const int index);
	int InsertBeforeCopy(const T* elem, const int index);
	int InsertBeforeCopyEx(const T* elem, const int index, void (*funcCopy)(T*, T*));
	int InsertBefore(T* elem, const T* ref);
	int InsertAfter(T* elem, const int index);
	int InsertAfterCopy(const T* elem, const int index);
	int InsertAfterCopyEx(const T* elem, const int index, void (*funcCopy)(T*, T*));
	int InsertAfter(T* elem, const T* ref);
	T* GetElemAt(const int index) const;
	T* RemoveElemAt(const int index);
	void RemoveElem(T* elem);
	void Clear();
	T* GetHead();
	T* GetNext();
	bool Exist(T *ptr);
	int Length() { return m_iMember; }
// 	void Sort(int (*funcSort)(T*, T*));
// 	int InsertSort(T* elem, int (*funcSort)(T*, T*));
	
protected:
	PLIST m_pHeader, m_pTail;
	PLIST m_pCur;
	int m_iMember;
	
private:
	void* GetPListAt(const int index) const;
	void InsertBeforeNoCheck(PLIST pIns, const T* elem);
	void InsertAfterNoCheck(PLIST pIns, const T* elem);
};

template <typename T>
CBaseList<T>::CBaseList()
{
	m_pHeader=new LIST;
	m_pHeader->ptr=(T*)0;
	m_pHeader->nodeType=tHEAD;
	m_pHeader->prev=(PLIST)0;
	m_pHeader->next=(PLIST)0;
	
	m_pTail=new LIST;
	m_pTail->ptr=(T*)0;
	m_pHeader->nodeType=tTAIL;
	m_pHeader->prev=(PLIST)0;
	m_pHeader->next=(PLIST)0;
	
	m_pHeader->next=m_pTail;
	m_pTail->prev=m_pHeader;
	
	m_iMember=0;
	m_pCur=m_pTail;
}

template <typename T>
CBaseList<T>::CBaseList(const T* s[], const int count)
{
	CBaseList();
	for(int i=0; i<count; ++i) {
		Append(s[i]);
	}
}

template <typename T>
CBaseList<T>::~CBaseList()
{
	Clear();
	delete m_pHeader;
	delete m_pTail;
}

template <typename T>
void CBaseList<T>::Append(T* elem)
{
	PLIST pnew=new LIST;
	PLIST pLast;
	pnew->ptr=elem;
	pnew->nodeType=tELEM;
	
	pLast=m_pTail->prev;
	pLast->next=pnew;
	pnew->prev=pLast;
	pnew->next=m_pTail;
	m_pTail->prev=pnew;
	
	m_iMember++;
}

template <typename T>
void CBaseList<T>::AppendCopy(const T* elem)
{
	T* pElem;
	pElem=(T *)malloc(sizeof(T));
	memcpy(pElem, elem, sizeof(T));
	Append(pElem);
}

template <typename T>
void CBaseList<T>::AppendCopyEx(const T* elem, void (*funcCopy)(T*, T*))
{
	T* pElem;
	pElem=(T *)malloc(sizeof(T));
	(*funcCopy)(pElem, elem);
	Append(pElem);
}

template <typename T>
void CBaseList<T>::Show(void (*funcShow)(T *)) const
{
	if(m_iMember==0) {
		printf("List is empty.\n");
	} else {
		printf("List of members: \n");
		PLIST p1;
		p1=m_pHeader->next;
		while(p1!=m_pTail) {
			(*funcShow)(p1->ptr);
			p1=p1->next;
		}
	}
}

template <typename T>
int CBaseList<T>::GetIndex(const T* elem) const
{
	PLIST p1;
	int index=0;
	
	p1=m_pHeader->next;
	while(p1!=m_pTail) {
		if(elem==p1->ptr)
			return index;
		index++;
		p1=p1->next;
	}
	return NOEXIST;
}

template <typename T>
void CBaseList<T>::InsertBeforeNoCheck(PLIST pIns, const T* elem)
{	
	PLIST pnew, pBIns;
	
	pnew=new LIST;
	pnew->ptr=(T *)elem;
	pnew->nodeType=tELEM;
	
	pBIns=pIns->prev;
	// pBIns -- pnew -- pIns
	pBIns->next=pnew;
	pnew->prev=pBIns;
	pnew->next=pIns;
	pIns->prev=pnew;
}

template <typename T>
int CBaseList<T>::InsertBefore(T* elem, const int index)
{
	PLIST pIns;
	
	if(index==HEAD)
		return ERR_FORBIDDEN;
	if(index>=m_iMember)
		return ERR_NOEXIST;
	
	pIns=(PLIST)GetPListAt(index);
	
	if(pIns==NULL)
		return ERR_NOEXIST;
	
	InsertBeforeNoCheck(pIns, elem);
	m_iMember++;
	return ERR_OK;
}

template <typename T>
int CBaseList<T>::InsertBeforeCopy(const T* elem, const int index)
{
	PLIST pIns;
	T* pElem;
	
	if(index==HEAD)
		return ERR_FORBIDDEN;
	if(index>=m_iMember)
		return ERR_NOEXIST;
	
	pIns=(PLIST)GetPListAt(index);
	
	if(pIns==NULL)
		return ERR_NOEXIST;
	
	pElem=(T *)malloc(sizeof(T));
	memcpy(pElem, elem, sizeof(T));
	InsertBeforeNoCheck(pIns, pElem);
	m_iMember++;
	return ERR_OK;
}

template <typename T>
int CBaseList<T>::InsertBeforeCopyEx(const T* elem, const int index, void (*funcCopy)(T*, T*))
{
	PLIST pIns;
	T* pElem;
	
	if(index==HEAD)
		return ERR_FORBIDDEN;
	if(index>=m_iMember)
		return ERR_NOEXIST;
	
	pIns=(PLIST)GetPListAt(index);
	
	if(pIns==NULL)
		return ERR_NOEXIST;
	
	pElem=(T *)malloc(sizeof(T));
	(*funcCopy)(pElem, elem);
	InsertBeforeNoCheck(pIns, pElem);
	m_iMember++;
	return ERR_OK;
}

template <typename T>
int CBaseList<T>::InsertBefore(T* elem, const T* ref)
{
	return InsertBefore(elem, GetIndex(ref));
}

template <typename T>
void CBaseList<T>::InsertAfterNoCheck(PLIST pIns, const T* elem)
{	
	PLIST pnew, pAIns;
	
	pnew=new LIST;
	pnew->ptr=(T *)elem;
	pnew->nodeType=tELEM;
	
	pAIns=pIns->next;
	// pIns -- pnew -- pAIns
	pIns->next=pnew;
	pnew->prev=pIns;
	pnew->next=pAIns;
	pAIns->prev=pnew;
}

template <typename T>
int CBaseList<T>::InsertAfter(T* elem, const int index)
{
	PLIST p1, pIns, pAIns, pnew;
	int i=0;
	
	if(index==TAIL)
		return ERR_FORBIDDEN;
	if(index>=m_iMember)
		return ERR_NOEXIST;
	
	pIns=(PLIST)GetPListAt(index);
	
	if(pIns==NULL)
		return ERR_NOEXIST;
	
	InsertAfterNoCheck(pIns, elem);
	m_iMember++;
	return ERR_OK;
}

template <typename T>
int CBaseList<T>::InsertAfterCopy(const T* elem, const int index)
{
	PLIST pIns;
	T* pElem;
	
	if(index==TAIL)
		return ERR_FORBIDDEN;
	if(index>=m_iMember)
		return ERR_NOEXIST;
	
	pIns=(PLIST)GetPListAt(index);
	
	if(pIns==NULL)
		return ERR_NOEXIST;
	
	pElem=(T *)malloc(sizeof(T));
	memcpy(pElem, elem, sizeof(T));
	InsertAfterNoCheck(pIns, pElem);
	m_iMember++;
	return ERR_OK;
}

template <typename T>
int CBaseList<T>::InsertAfterCopyEx(const T* elem, const int index, void (*funcCopy)(T*, T*))
{
	PLIST pIns;
	T* pElem;
	
	if(index==TAIL)
		return ERR_FORBIDDEN;
	if(index>=m_iMember)
		return ERR_NOEXIST;
	
	pIns=(PLIST)GetPListAt(index);
	
	if(pIns==NULL)
		return ERR_NOEXIST;
	
	pElem=(T *)malloc(sizeof(T));
	(*funcCopy)(pElem, elem);
	InsertAfterNoCheck(pIns, pElem);
	m_iMember++;
	return ERR_OK;
}

template <typename T>
int CBaseList<T>::InsertAfter(T* elem, const T* ref)
{
	return InsertAfter(elem, GetIndex(ref));
}

template <typename T>
T* CBaseList<T>::GetElemAt(const int index) const
{
	PLIST p1;
	
	if(index==HEAD)
		return NULL;
	
	if(index==TAIL)
		return NULL;
	
	if(index<0)
		return NULL;
	
	if(m_iMember==0)
		return NULL;
	
	if(index>=m_iMember)
		return NULL;
	
	p1=m_pHeader->next;
	for(int i=0; i<index; ++i) {
		p1=p1->next;
	}
	return p1->ptr;
}

template <typename T>
T* CBaseList<T>::RemoveElemAt(const int index)
{
	PLIST p1, p1p, p1n;
	T* pRet;
	
	if(m_iMember==0)
		return NULL;
	
	if(index>=m_iMember)
		return NULL;
	
	p1=m_pHeader->next;
	for(int i=0; i<index; ++i) {
		p1=p1->next;
	}
	p1p=p1->prev;
	p1n=p1->next;
	p1p->next=p1n;
	p1n->prev=p1p;
	
	pRet=p1->ptr;
	p1->prev=NULL;
	p1->next=NULL;
	p1->ptr=NULL;
	delete p1;
	m_iMember--;
	
	return pRet;
}

template <typename T>
void CBaseList<T>::RemoveElem(T* elem)
{
	if(m_iMember==0)
		return;
	
	PLIST p0, p1, p1p, p1n;
	
	p1=NULL;
	p0=m_pHeader->next;
	while(p0) {
		if(p0->ptr==elem) {
			p1=p0;
			break;
		}
		p0=p0->next;
	}
	
	if(p1) {
		p1p=p1->prev;
		p1n=p1->next;
		p1p->next=p1n;
		p1n->prev=p1p;
		
		p1->prev=NULL;
		p1->next=NULL;
		p1->ptr=NULL;
		delete p1;
		m_iMember--;
	}
	
	m_pCur=p1n;
}

template <typename T>
void CBaseList<T>::Clear()
{
	while(m_iMember>0)
		RemoveElemAt(0);
}

template <typename T>
void* CBaseList<T>::GetPListAt(const int index) const
{
	PLIST p, pFind;
	int i=0;
	
	pFind=NULL;
	if(index==HEAD) {
		pFind=m_pHeader;
	} else if(index==TAIL) {
		pFind=m_pTail;
	} else {
		p=m_pHeader->next;
		while(p!=m_pTail) {
			if(i==index) {
				pFind=p;
				break;
			}
			++i;
			p=p->next;
		}
	}
	return (void *)pFind;
}

template <typename T>
T* CBaseList<T>::GetHead()
{
	m_pCur=m_pHeader->next;
	if(m_pCur==m_pTail)
		return NULL;
	return m_pCur->ptr;
}

template <typename T>
T* CBaseList<T>::GetNext()
{
	if(m_pCur==m_pTail)
		return NULL;
	m_pCur=m_pCur->next;
	if(m_pCur==m_pTail)
		return NULL;
	return m_pCur->ptr;
}

template <typename T>
bool CBaseList<T>::Exist(T* ptr)
{
	T* p;
	p=GetHead();
	while(p) {
		if(p==ptr)
			return true;
		p=GetNext();
	}
	return false;
}

#endif

