#include "gdi.h"

/*
GDI���Ʒ���
����ͬһ��ͼ���������л滭
������˸�������
*/
void paint(HWND hwnd, float x, float y, float stall)
{
    HDC hdc;  // DC(�ɻ�ͼ���ڴ����) �ľ��
    HPEN hpen;  // ����
    hdc = GetDC(hwnd);
    hpen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
    HGDIOBJ oldpen = SelectObject(hdc, hpen); //���»��ʣ����ұ���ɻ���

    MoveToEx(hdc, x - stall / 4.0f, y, NULL);
    LineTo(hdc, x - stall / 4.0f, y - stall);
    LineTo(hdc, x + stall / 4.0f, y - stall);
    LineTo(hdc, x + stall / 4.0f, y);
    LineTo(hdc, x - stall / 4.0f, y);

    SelectObject(hdc, oldpen); //�󶨾ɻ���
    DeleteObject(hpen);
    ReleaseDC(hwnd, hdc);
}