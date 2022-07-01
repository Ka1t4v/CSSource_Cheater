
#include "gdi.h"
#include "tools.h"


int main(int argc, char const* argv[])
{
    //分辨率
    float width = 1024;
    float height = 768;

    //人物模型高度
    float tall = 70;

    //获取进程pid
    DWORD pid = getpidbyname("hl2.exe");

    //获取client模块句柄
    HMODULE module_clientdll = getmodulehandlebyname("client.dll", pid);
    if (module_clientdll == NULL)
    {
        {
            cout << "open client.dll failed" << endl;
            return -1;
        }
    }

    //获取server模块句柄
    HMODULE module_serverdll = getmodulehandlebyname("server.dll", pid);
    if (module_serverdll == NULL)
    {
        {
            cout << "open server.dll failed" << endl;
            return -1;
        }
    }

    //获得进程句柄
    HANDLE hp = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (hp == NULL)
    {
        cout << "open process failed" << endl;
        return -1;
    }

    //获取窗口句柄
    HWND hw = FindWindow("Valve001", "Counter-Strike Source");
    if (hw == NULL)
    {
        cout << "open window failed" << endl;
        return -1;
    }

    //取数据循环
    while (1) {
        SIZE_T dwRead = 0;

        float fov, fov_y;
        UINT64 fov_addr = (UINT64)module_clientdll + 0x506848;
        UINT64 fov_y_addr = (UINT64)module_clientdll + 0x50684C;
        ReadProcessMemory(hp, (LPCVOID)fov_addr, &fov, 4, &dwRead);
        ReadProcessMemory(hp, (LPCVOID)fov_y_addr, &fov_y, 4, &dwRead);
        //printf("fov:%f fov_y:%f\n", fov, fov_y);


        float px, py, pz;
        UINT64 player_x = (UINT64)module_clientdll + 0x4FCB04;
        UINT64 player_y = (UINT64)module_clientdll + 0x4FCB08;
        UINT64 player_z = (UINT64)module_clientdll + 0x4FCB0C;
        ReadProcessMemory(hp, (LPCVOID)player_x, &px, 4, &dwRead);
        ReadProcessMemory(hp, (LPCVOID)player_y, &py, 4, &dwRead);
        ReadProcessMemory(hp, (LPCVOID)player_z, &pz, 4, &dwRead);
        //printf("player x:%f y:%f z:%f\n", px, py, pz);

        float aimy, aimx;
        UINT64 aim_y = (UINT64)module_clientdll + 0x49D504;
        UINT64 aim_x = (UINT64)module_clientdll + 0x49D508;
        ReadProcessMemory(hp, (LPCVOID)aim_y, &aimy, 4, &dwRead);
        ReadProcessMemory(hp, (LPCVOID)aim_x, &aimx, 4, &dwRead);
        // printf("aim x:%f y:%f\n", aimx, aimy);

        int botsum;
        UINT64 bot_sum = (UINT64)module_serverdll + 0x58BA08;
        ReadProcessMemory(hp, (LPCVOID)bot_sum, &botsum, 4, &dwRead);
        //printf("botsum:%i\n", botsum);

        UINT64 bot_base = (UINT64)module_serverdll + 0x54DD24;
        for (int i = 0; i < botsum; i++)
        {
            float x, y, z;
            float b = 0; //鼠标准星和bot的水平夹角
            float c = 0; //鼠标准星和bot的垂直夹角
            float d = 0; //鼠标准星和bot头的垂直夹角
            float GDI_x, GDI_y;
            UINT64 bot_x = bot_base;
            UINT64 bot_y = bot_base + 0x4;
            UINT64 bot_z = bot_base + 0x8;
            ReadProcessMemory(hp, (LPCVOID)bot_x, &x, 4, &dwRead);
            ReadProcessMemory(hp, (LPCVOID)bot_y, &y, 4, &dwRead);
            ReadProcessMemory(hp, (LPCVOID)bot_z, &z, 4, &dwRead);
            //printf("bot[%i] x:%f y:%f z:%f\n", i, x, y, z);

            float subx = x - px, suby = y - py, subz = z - pz;
            float dis_xy = sqrt(pow(subx, 2) + pow(suby, 2));
            float dis_xyz = sqrt(pow(subx, 2) + pow(suby, 2) + pow(subz, 2));
            //printf("dis_xy:%f\n", dis_xy);

            b = gethorizontalgap(aimx, getbotangle_horizontal(subx, suby));
            GDI_x = getxpoint(b, fov, width);

            c = getverticalgap(aimy, getbotangle_vertical(subz, dis_xy));
            GDI_y = getypoint(c, fov_y, width, height);

            float subhead = z + tall - pz;
            d = getverticalgap(aimy, getbotangle_vertical(subhead, dis_xy));
            float GDI_y_head = getypoint(d, fov_y, width, height);
            float bot_tall = fabs(GDI_y_head - GDI_y);

            if (GDI_x != -1 && GDI_y != -1) {
                paint(hw, GDI_x, GDI_y, bot_tall);
            }
            bot_base += 0x24;  //将偏移定位到下一个bot的起始地址
        }
    }


    return 0;
}
