#include <initguid.h>
#include <dxgi.h>
#include <stdio.h>
#include <psapi.h>
#include <intrin.h>
#include <windows.h>
#include <locale.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "psapi.lib")

int Switch; int cpuInfo[4];

char vendor[13];char model[49];


void cpu_info() {
 
    __cpuid(cpuInfo, 0);
    *(int*)&vendor[0] = cpuInfo[1]; // EBX
    *(int*)&vendor[4] = cpuInfo[3]; // EDX
    *(int*)&vendor[8] = cpuInfo[2]; // ECX
    vendor[12] = '\0';

    printf("������������� CPU: %s\n", vendor);

    __cpuid(cpuInfo, 0x80000000);
    unsigned int maxExtId = cpuInfo[0];

    if (maxExtId >= 0x80000004) {
        int* modelPtr = (int*)model;
        for (unsigned int i = 0; i < 3; i++) {
            __cpuid(cpuInfo, 0x80000002 + i);
            modelPtr[0] = cpuInfo[0];
            modelPtr[1] = cpuInfo[1];
            modelPtr[2] = cpuInfo[2];
            modelPtr[3] = cpuInfo[3];
            modelPtr += 4;
        }
        model[48] = '\0';
        printf("������ CPU: %s\n", model);
    }

    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    printf("���������� ����: %u\n", sysInfo.dwNumberOfProcessors);
    return main();
}

void mem_Info() {
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);

    if (GlobalMemoryStatusEx(&statex)) {
        printf("����� ����� ���: %llu ��\n", statex.ullTotalPhys / (1024 * 1024));
        return main();
    }
    else {
        printf("������ ��������� ����������\n");
    }
   
}

void gpu_info() {
    IDXGIFactory* pFactory = NULL;
    HRESULT hr = CreateDXGIFactory(&IID_IDXGIFactory, (void**)&pFactory);
    if (FAILED(hr)) {
        printf("������: �� ������� ������� DXGI Factory\n");
        return;
    }

    IDXGIAdapter* pAdapter = NULL;
    hr = pFactory->lpVtbl->EnumAdapters(pFactory, 0, &pAdapter);
    if (SUCCEEDED(hr)) {
        DXGI_ADAPTER_DESC desc;
        hr = pAdapter->lpVtbl->GetDesc(pAdapter, &desc);
        if (SUCCEEDED(hr)) {
            wprintf(L"������������� GPU: %s\n", desc.Description);
            printf("�����������: %llu ��\n", desc.DedicatedVideoMemory / (1024 * 1024));
        }
        pAdapter->lpVtbl->Release(pAdapter);
    }
    else {
        printf("������: �� ������� �������� ������� GPU\n");
    }

    pFactory->lpVtbl->Release(pFactory);
    return main();
}

int main() {
    setlocale(LC_ALL, "Russian");

    printf("������� ��� �� ������ ����������\n1.���������\t2.���������� ������\t3.������ ����������\t0.�����\n");
    scanf_s("%d", &Switch);

    switch (Switch)
    {
    case 0: return 0;
    case 1:mem_Info();break;
    case 2: cpu_info(); break;
    case 3: gpu_info(); break;
    default:
        printf("������������ �����\n"); return main();
        break;
    }
    
    return 0;
}
