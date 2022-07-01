#include "gdi.h"

/*
GDI绘制方框
基于同一画图对象句柄进行绘画
会有闪烁现象出现
*/
void paint(HWND hwnd, float x, float y, float stall)
{
    HDC hdc;  // DC(可画图的内存对象) 的句柄
    HPEN hpen;  // 画笔
    hdc = GetDC(hwnd);
    hpen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
    HGDIOBJ oldpen = SelectObject(hdc, hpen); //绑定新画笔，并且保存旧画笔

    MoveToEx(hdc, x - stall / 4.0f, y, NULL);
    LineTo(hdc, x - stall / 4.0f, y - stall);
    LineTo(hdc, x + stall / 4.0f, y - stall);
    LineTo(hdc, x + stall / 4.0f, y);
    LineTo(hdc, x - stall / 4.0f, y);

    SelectObject(hdc, oldpen); //绑定旧画笔
    DeleteObject(hpen);
    ReleaseDC(hwnd, hdc);
}