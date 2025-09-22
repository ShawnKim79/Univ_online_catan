// link_type.h: interface for the link_type class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINK_TYPE_H__D36C1AD6_1736_4B7E_8F1D_0BB366B5E083__INCLUDED_)
#define AFX_LINK_TYPE_H__D36C1AD6_1736_4B7E_8F1D_0BB366B5E083__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class link_type  
{
public:
	int road_num(link_type *p);
	void add_point(link_type *p);
	
	link_type *link[3];				// �������� ����� �ٸ� ��������
	int number;						// �������� ������ȣ
	
	int road[3];					// ������ ������ ��

	int x;							// �������� X ��ǥ
	int y;							// �������� Y ��ǥ

	int link_max;					// �������� ����� �ٸ� �������� �ִ��
	
	unsigned char city;				// �������� �����ϴ� �ǹ�
									// 1~4 �� ��Ʋ����Ʈ
									// 6~9 �� ��Ƽ

	link_type();
	virtual ~link_type();

};

#endif // !defined(AFX_LINK_TYPE_H__D36C1AD6_1736_4B7E_8F1D_0BB366B5E083__INCLUDED_)
