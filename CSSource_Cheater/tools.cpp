#include "tools.h"

/*
通过进程名获取进程pid
*/
DWORD getpidbyname(const char* ProcessName) {
    HANDLE Processes = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    PROCESSENTRY32 ProcessInfo = { 0 };
    ProcessInfo.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(Processes, &ProcessInfo) == false) {
        return -1;
    }
    do {
        if (strcmp((char*)ProcessInfo.szExeFile, ProcessName) == 0) {
            return ProcessInfo.th32ProcessID;
        }
        memset(&ProcessInfo, 0, sizeof(ProcessInfo));
        ProcessInfo.dwSize = sizeof(PROCESSENTRY32);
    } while (Process32Next(Processes, &ProcessInfo));
    return -1;
}

/*
通过模块名获取指定pid中的模块句柄
*/
HMODULE getmodulehandlebyname(const char* ModuleName, DWORD pid) {
    HANDLE Module = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    MODULEENTRY32 ModuleInfo = { 0 };
    ModuleInfo.dwSize = sizeof(MODULEENTRY32);
    if (Module32First(Module, &ModuleInfo) == false) {
        return NULL;
    }
    do {
        if (strcmp((char*)ModuleInfo.szModule, ModuleName) == 0) {
            return ModuleInfo.hModule;
        }
    } while (Module32Next(Module, &ModuleInfo));
    return NULL;
}

/*
获取bot水平方向和x轴正半轴的夹角
*/
float getbotangle_horizontal(float subx, float suby)
{
    float a = 0;
    if (subx > 0 && suby > 0) //one quadrant
    {
        a = atan(suby / subx) * 180.0f / PI;
    }
    else if (subx == 0 && suby > 0)  // y+
    {
        a = 90;
    }
    else if (subx < 0 && suby>0) //two quadrant
    {
        a = atan(suby / subx) * 180.0f / PI + 180;
    }
    else if (subx < 0 && suby == 0)  // x-
    {
        a = 180;
    }
    else if (subx < 0 && suby < 0) //three quadrant
    {
        a = atan(suby / subx) * 180.0f / PI + 180;
    }
    else if (subx == 0 && suby < 0) // y-
    {
        a = 270;
    }
    else if (subx > 0 && suby < 0) //four quadrant
    {
        a = atan(suby / subx) * 180.0f / PI + 360;
    }
    else if (subx > 0 && suby == 0)//x+
    {
        a = 0;
    }
    return a;
}

/*
获取bot垂直方向和z轴正半轴的夹角
*/
float getbotangle_vertical(float subz, float dis_xy)
{
    float a = 0;
    if (subz > 0)
    {
        a = atan((double)subz / (double)dis_xy) * 180.0f / PI;
    }
    else if (subz == 0)
    {
        a = 0;

    }
    else if (subz < 0) {
        a = atan((double)subz / (double)dis_xy) * 180.0f / PI;
    }
    return a;
}

/*
获取水平方向视角和bot之间的夹角
夹角在0-180之间
*/
float gethorizontalgap(float x, float y)
{
    float sub;
    sub = x - y;
    if (sub > 180) sub = 360 - sub;
    if (sub < -180) sub = 360 + sub;
    return sub;
}

/*
获取垂直方向视角和bot之间的夹角
夹角在0-90之间
*/
float getverticalgap(float x, float y)
{
    float sub;
    sub = x + y;
    return sub;
}

/*
获取水平方向的屏幕点坐标
*/
float getxpoint(float b, float fov, float image_x)
{
    if (b > -(fov / 2) && b < (fov / 2))
    {
        float t = tan(b * PI / 180.0f);
        float sum = image_x / 2 + image_x / 2 * t;
        //printf("x轴图像偏移:%f\n", sum);
        return  sum;
    }
    else
    {
        // printf("x轴不在图像内\n");
        return -1;
    }
}

/*
获取垂直方向的屏幕点坐标
*/
float getypoint(float c, float fov_y, float image_x, float image_y)
{
    if (c > -(fov_y / 2) && c < (fov_y / 2))
    {
        float t = tan(c * PI / 180.0f);
        float sum = image_y / 2 - image_x / 2 * t;
        //printf("y轴图像偏移:%f\n", sum);
        return  sum;
    }
    else
    {
        //printf("y轴不在图像内\n");
        return -1;
    }
}