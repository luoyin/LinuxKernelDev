/**
 * @file		CBaseList.cc
 * @author		Luo, Yin
 * @version	V1.0
 * @date		2016-01-25
 * @brief		XKANAL.SYSTEM--工具类--基本链表(源文件)
 */

#include "CBaseList.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>


// template <typename T>
// void CBaseList<T>::Sort(int (*funcSort)(T*, T*))
// {
// 	PLIST pHeader, pTail, p;
// 	
// 	if(m_iMember==0)
// 		return;
// 	
// 	pHeader=new LIST;
// 	pHeader->ptr=NULL;
// 	pHeader->nodeType=tHEAD;
// 	pHeader->prev=NULL;
// 	pHeader->next=NULL;
// 	
// 	pTail=new LIST;
// 	pTail->ptr=NULL;
// 	pTail->nodeType=tTAIL;
// 	pTail->prev=NULL;
// 	pTail->next=NULL;
// 	
// 	m_pHeader->next->prev=pHeader;
// 	pHeader->next=m_pHeader->next;
// 	m_pTail->prev->next=pTail;
// 	pTail->prev=m_pTail->prev;
// 	
// 	m_pHeader->next=m_pTail;
// 	m_pTail->prev=m_pHeader;
// 	m_iMember=0;
// 	
// 	p=pHeader->next;
// 	while(p!=pTail) {
// 		InsertSort(p->ptr, funcSort);
// 		m_iMember++;
// 		p=p->next;
// 	}
// }
// 
// template <typename T>
// int CBaseList<T>::InsertSort(T* elem, int (*funcSort)(T*, T*))
// {
// 	PLIST pIns, pBIns;
// 	
// 	if(m_iMember==0) {
// 		Append(elem);
// 	} else {
// 		pIns=m_pHeader->next;
// 		while(pIns!=m_pTail) {
// 			if((*funcSort)(pIns->ptr, elem)==1)
// 				break;
// 			pIns=pIns->next;
// 		}
// 		InsertBeforeNoCheck(pIns, elem);
// 	}
// }


