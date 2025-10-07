#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoFileSystemManagement1.h"
#include "IdEcoLab1.h"
#include <stdio.h>

/* Новое оформление вывода тестов (кардинально другое) */
static void box_line(char c, int width) {
    int i;
    for (i = 0; i < width; ++i) putchar(c);
    putchar('\n');
}

static void box_title(const char* title) {
    int width = 76;
    int len = 0;
    const char* p = title;
    while (*p++) ++len;
    box_line('=', width);
    printf("|| %-*s ||\n", width - 6, title);
    box_line('=', width);
}

static void box_section(const char* label) {
    printf("-- %s --\n", label);
}

static void print_array(const int32_t* arr, int size) {
    int i;
    putchar('[');
    for (i = 0; i < size; ++i) {
        printf("%d%s", arr[i], (i < size - 1) ? ", " : "");
    }
    printf("]\n");
}

static void print_array_long(const int64_t* arr, int size) {
    int i;
    putchar('[');
    for (i = 0; i < size; ++i) {
        printf("%lld%s", arr[i], (i < size - 1) ? ", " : "");
    }
    printf("]\n");
}

static void print_array_float(const float* arr, int size) {
    int i;
    putchar('[');
    for (i = 0; i < size; ++i) {
        printf("%.3f%s", arr[i], (i < size - 1) ? ", " : "");
    }
    printf("]\n");
}

static void print_array_double(const double* arr, int size) {
    int i;
    putchar('[');
    for (i = 0; i < size; ++i) {
        printf("%.6f%s", arr[i], (i < size - 1) ? ", " : "");
    }
    printf("]\n");
}

static void print_array_longdouble(const long double* arr, int size) {
    int i;
    putchar('[');
    for (i = 0; i < size; ++i) {
        printf("%.6Lf%s", arr[i], (i < size - 1) ? ", " : "");
    }
    printf("]\n");
}

static int is_sorted_int32(const int32_t* arr, int size) {
    int i;
    for (i = 1; i < size; i++) {
        if (arr[i-1] > arr[i]) return 0;
    }
    return 1;
}

static int is_sorted_int64(const int64_t* arr, int size) {
    int i;
    for (i = 1; i < size; i++) {
        if (arr[i-1] > arr[i]) return 0;
    }
    return 1;
}

static int is_sorted_float(const float* arr, int size) {
    int i;
    for (i = 1; i < size; i++) {
        if (arr[i-1] > arr[i]) return 0;
    }
    return 1;
}

static int is_sorted_double(const double* arr, int size) {
    int i;
    for (i = 1; i < size; i++) {
        if (arr[i-1] > arr[i]) return 0;
    }
    return 1;
}

static int is_sorted_longdouble(const long double* arr, int size) {
    int i;
    for (i = 1; i < size; i++) {
        if (arr[i-1] > arr[i]) return 0;
    }
    return 1;
}

int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    IEcoLab1* pIEcoLab1 = 0;


    if (pISys == 0) {
        result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void **)&pISys);
        if (result != 0 || pISys == 0) {
            printf("ERROR: Failed to create system interface.\n");
            goto Release;
        }
    }

    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        printf("ERROR: Failed to get interface bus.\n");
        goto Release;
    }

    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0,
        &IID_IEcoMemoryAllocator1, (void**)&pIMem);
    if (result != 0 || pIMem == 0) {
        printf("ERROR: Failed to get memory manager.\n");
        goto Release;
    }

    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1, 0,
        &IID_IEcoLab1, (void**)&pIEcoLab1);
    if (result != 0 || pIEcoLab1 == 0) {
        printf("ERROR: Failed to get IEcoLab1 interface.\n");
        goto Release;
    }

    box_title("STRESS TEST: int32_t (random 1000 elems)");
    {
        #define N1 1000
        int32_t arr[N1];
        int i;
        for (i = 0; i < N1; i++) {
            arr[i] = (rand() % 2000000) - 1000000;  /* [-1e6, 1e6] */
        }
        pIEcoLab1->pVTbl->qsort_int(pIEcoLab1, arr, N1);
        printf("Status: %s\n", is_sorted_int32(arr, N1) ? "OK" : "FAIL");
        printf("First 10 elems: ");
        print_array(arr, 10);
        putchar('\n');
    }

    box_title("STRESS TEST: int64_t (random 5000 elems)");
    {
        #define N2 5000
        int64_t arr[N2];
        int i;
        for (i = 0; i < N2; i++) {
            arr[i] = ((int64_t)rand() * (int64_t)rand()) % 1000000000 - 500000000;
        }
        pIEcoLab1->pVTbl->qsort_long(pIEcoLab1, arr, N2);
        printf("Status: %s\n", is_sorted_int64(arr, N2) ? "OK" : "FAIL");
        printf("First 10 elems: ");
        print_array_long(arr, 10);
        putchar('\n');
    }

    box_title("STRESS TEST: float (random 2000 elems)");
    {
        #define N3 2000
        float arr[N3];
        int i;
        for (i = 0; i < N3; i++) {
            arr[i] = ((float)rand() / (float)RAND_MAX) * 2000.0f - 1000.0f;
        }
        pIEcoLab1->pVTbl->qsort_float(pIEcoLab1, arr, N3);
        printf("Status: %s\n", is_sorted_float(arr, N3) ? "OK" : "FAIL");
        printf("First 10 elems: ");
        print_array_float(arr, 10);
        putchar('\n');
    }

    box_title("STRESS TEST: double (random 10000 elems)");
    {
        #define N4 10000
        double arr[N4];
        int i;
        for (i = 0; i < N4; i++) {
            arr[i] = ((double)rand() / (double)RAND_MAX) * 1e6 - 5e5;
        }
        pIEcoLab1->pVTbl->qsort_double(pIEcoLab1, arr, N4);
        printf("Status: %s\n", is_sorted_double(arr, N4) ? "OK" : "FAIL");
        printf("First 10 elems: ");
        print_array_double(arr, 10);
        putchar('\n');
    }

    box_title("STRESS TEST: long double (random 3000 elems)");
    {
        #define N5 3000
        long double arr[N5];
        int i;
        for (i = 0; i < N5; i++) {
            arr[i] = ((long double)rand() / (long double)RAND_MAX) * 1000.0 - 500.0;
        }
        pIEcoLab1->pVTbl->qsort_longdouble(pIEcoLab1, arr, N5);
        printf("Status: %s\n", is_sorted_longdouble(arr, N5) ? "OK" : "FAIL");
        printf("First 10 elems: ");
        print_array_longdouble(arr, 10);
        putchar('\n');
    }

    /* EDGE TEST: уже отсортированный массив */
    box_title("EDGE TEST: already sorted array (int32_t)");
    {
        #define N6 1000
        int32_t arr[N6];
        int i;
        for (i = 0; i < N6; i++) arr[i] = i;
        pIEcoLab1->pVTbl->qsort_int(pIEcoLab1, arr, N6);
        printf("Status: %s\n", is_sorted_int32(arr, N6) ? "OK" : "FAIL");
        printf("First 10 elems: ");
        print_array(arr, 10);
        putchar('\n');
    }

    /* EDGE TEST: массив в обратном порядке */
    box_title("EDGE TEST: reverse sorted array (double)");
    {
        #define N7 2000
        double arr[N7];
        int i;
        for (i = 0; i < N7; i++) arr[i] = (double)(N7 - i);
        pIEcoLab1->pVTbl->qsort_double(pIEcoLab1, arr, N7);
        printf("Status: %s\n", is_sorted_double(arr, N7) ? "OK" : "FAIL");
        printf("First 10 elems: ");
        print_array_double(arr, 10);
        putchar('\n');
    }
    result = 0;

Release:
    if (pIBus != 0) pIBus->pVTbl->Release(pIBus);
    if (pIMem != 0) pIMem->pVTbl->Release(pIMem);
    if (pIEcoLab1 != 0) pIEcoLab1->pVTbl->Release(pIEcoLab1);
    if (pISys != 0) pISys->pVTbl->Release(pISys);

    
    return result;
}