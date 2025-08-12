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


void space() {
    for (int i = 0; i < 100; i++) {
        printf("\n");
    }
}

void cpu_info() {

    __cpuid(cpuInfo, 0);
    *(int*)&vendor[0] = cpuInfo[1]; // EBX
    *(int*)&vendor[4] = cpuInfo[3]; // EDX
    *(int*)&vendor[8] = cpuInfo[2]; // ECX
    vendor[12] = '\0';
    
    space();

    printf("Производитель CPU: %s\n", vendor);

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
        printf("Модель CPU: %s\n", model);
    }


    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    printf("Логических ядер: %u\n", sysInfo.dwNumberOfProcessors);
   
    return main();
}

void mem_Info() {
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);

    if (GlobalMemoryStatusEx(&statex)) {
        space();
        printf("Общий объем ОЗУ: %llu МБ\n", statex.ullTotalPhys / (1024 * 1024));
        return main();
    }
    else {
        printf("Ошибка получения информации\n");
    }
   
}

void gpu_info() {
    IDXGIFactory* pFactory = NULL;
    HRESULT hr = CreateDXGIFactory(&IID_IDXGIFactory, (void**)&pFactory);
    
    if (FAILED(hr)) {
        printf("Ошибка: не удалось создать DXGI Factory\n");
        return;
    }

    

    IDXGIAdapter* pAdapter = NULL;
    hr = pFactory->lpVtbl->EnumAdapters(pFactory, 0, &pAdapter);
    if (SUCCEEDED(hr)) {
        DXGI_ADAPTER_DESC desc;
        hr = pAdapter->lpVtbl->GetDesc(pAdapter, &desc);
        if (SUCCEEDED(hr)) {
            space();
            wprintf(L"Производитель GPU: %s\n", desc.Description);
            printf("Видеопамять: %llu МБ\n", desc.DedicatedVideoMemory / (1024 * 1024));
        }
        pAdapter->lpVtbl->Release(pAdapter);
    }
    else {
        printf("Ошибка: не удалось получить адаптер GPU\n");
    }

    pFactory->lpVtbl->Release(pFactory);
    return main();
}

int main() {
    setlocale(LC_ALL, "Russian");

    printf("\t\t\t\t\tВведите что вы хотите посмотреть\n\t1.Процессор\t\t2.Количество оперативной памяти\t\t3.Модель видеокарты\t0.Выход\n");
    scanf_s("%d", &Switch);

    switch (Switch)
    {
    case 0: return 0;
    case 1:cpu_info(); break;
    case 2:mem_Info(); break;
    case 3: gpu_info(); break;
    default: space();
        printf("Некорректный выбор\n"); return main();
        break;
    }
    
    return 0;
}
