
#pragma once

#if __VER >= 15 // __USING_CONTINENT_DATA

// �� ������ ���帮���� ����� �����մϴ�
// �� ������ ����� �߰��ɰ��  ����̸��� Index�� (�̸�)���ǵǾ�� �մϴ�.
// ����� ���ǵɰ�� �̸���Ģ�� �� ���Ѿ� �մϴ�. ( ���λ� CONT_ )

const BYTE CONT_NODATA			= 0;

const BYTE CONT_FLARIS			= 1;	// �ö󸮽� 
const BYTE CONT_SAINTMORNING		= 2; // ����Ʈ���
const BYTE CONT_RICIS			= 3; // ���ý��� ����
const BYTE CONT_ESTIA			= 4; // �̽�Ƽ�� ĳ�Ͼ�
const BYTE CONT_EAST			= 15; // ���� : ( 15 > n ) -> ���� �� ���

const BYTE CONT_DARKON12		= 241; // ���� 1, 2
const BYTE CONT_DARKON3			= 242; // ���� 3
const BYTE CONT_HARMONIN		= 243; // �ϸ����
const BYTE CONT_WEST			= 240; // ���� : ( n > 240 ) -> ���� �� ���

const BYTE CONT_ALL				= 255; // ����

#endif