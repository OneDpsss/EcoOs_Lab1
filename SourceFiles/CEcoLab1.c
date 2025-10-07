/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный исходный код описывает реализацию интерфейсов CEcoLab1
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "CEcoLab1.h"

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoLab1
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab1_QueryInterface(/* in */ IEcoLab1Ptr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &IID_IEcoLab1) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция AddRef для интерфейса IEcoLab1
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoLab1_AddRef(/* in */ IEcoLab1Ptr_t me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return ++pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoLab1
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoLab1_Release(/* in */ IEcoLab1Ptr_t me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoLab1((IEcoLab1*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция MyFunction
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab1_MyFunction(/* in */ IEcoLab1Ptr_t me, /* in */ char_t* Name, /* out */ char_t** copyName) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int16_t index = 0;

    /* Проверка указателей */
    if (me == 0 || Name == 0 || copyName == 0) {
        return ERR_ECO_POINTER;
    }

    /* Копирование строки */
    while(Name[index] != 0) {
        index++;
    }
    pCMe->m_Name = (char_t*)pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, index + 1);
    index = 0;
    while(Name[index] != 0) {
        pCMe->m_Name[index] = Name[index];
        index++;
    }
    *copyName = pCMe->m_Name;

    return ERR_ECO_SUCCESES;
}

/* Вспомогательные функции для сортировки вставками внутри корзин (bucket sort) */
static void CEcoLab1_insertion_sort_int32(int32_t* arr, uint32_t n) {
    uint32_t i;
    for (i = 1; i < n; ++i) {
        int32_t key = arr[i];
        int32_t j = (int32_t)i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

static void CEcoLab1_insertion_sort_int64(int64_t* arr, uint32_t n) {
    uint32_t i;
    for (i = 1; i < n; ++i) {
        int64_t key = arr[i];
        int32_t j = (int32_t)i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

static void CEcoLab1_insertion_sort_float(float* arr, uint32_t n) {
    uint32_t i;
    for (i = 1; i < n; ++i) {
        float key = arr[i];
        int32_t j = (int32_t)i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

static void CEcoLab1_insertion_sort_double(double* arr, uint32_t n) {
    uint32_t i;
    for (i = 1; i < n; ++i) {
        double key = arr[i];
        int32_t j = (int32_t)i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

static void CEcoLab1_insertion_sort_long_double(long double* arr, uint32_t n) {
    uint32_t i;
    for (i = 1; i < n; ++i) {
        long double key = arr[i];
        int32_t j = (int32_t)i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

/* Реализация bucket sort для разных типов данных. Используется распределитель памяти компонента. */
static int16_t CEcoLab1_bucket_sort_int32(IEcoLab1Ptr_t me, int32_t* data, uint32_t length) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    IEcoMemoryAllocator1* pIMem = pCMe->m_pIMem;
    uint32_t i;
    int32_t minVal, maxVal;
    uint64_t range;
    uint32_t numBuckets;
    uint32_t* sizes = 0;
    uint32_t* caps = 0;
    int32_t** buckets = 0;
    uint32_t out = 0;

    if (length <= 1) {
        return ERR_ECO_SUCCESES;
    }

    minVal = data[0];
    maxVal = data[0];
    for (i = 1; i < length; ++i) {
        if (data[i] < minVal) minVal = data[i];
        if (data[i] > maxVal) maxVal = data[i];
    }
    if (minVal == maxVal) {
        return ERR_ECO_SUCCESES;
    }

    range = (uint64_t)((int64_t)maxVal - (int64_t)minVal);
    numBuckets = length;
    if (numBuckets > 1024U) numBuckets = 1024U;
    if (numBuckets == 0U) numBuckets = 1U;

    sizes = (uint32_t*)pIMem->pVTbl->Alloc(pIMem, numBuckets * sizeof(uint32_t));
    caps = (uint32_t*)pIMem->pVTbl->Alloc(pIMem, numBuckets * sizeof(uint32_t));
    buckets = (int32_t**)pIMem->pVTbl->Alloc(pIMem, numBuckets * sizeof(int32_t*));
    if (sizes == 0 || caps == 0 || buckets == 0) {
        if (sizes) pIMem->pVTbl->Free(pIMem, sizes);
        if (caps) pIMem->pVTbl->Free(pIMem, caps);
        if (buckets) pIMem->pVTbl->Free(pIMem, buckets);
        return ERR_ECO_POINTER;
    }
    for (i = 0; i < numBuckets; ++i) {
        sizes[i] = 0U;
        caps[i] = 4U;
        buckets[i] = (int32_t*)pIMem->pVTbl->Alloc(pIMem, caps[i] * sizeof(int32_t));
        if (buckets[i] == 0) {
            uint32_t k;
            for (k = 0; k < i; ++k) {
                pIMem->pVTbl->Free(pIMem, buckets[k]);
            }
            pIMem->pVTbl->Free(pIMem, sizes);
            pIMem->pVTbl->Free(pIMem, caps);
            pIMem->pVTbl->Free(pIMem, buckets);
            return ERR_ECO_POINTER;
        }
    }

    for (i = 0; i < length; ++i) {
        uint64_t pos = (uint64_t)((int64_t)data[i] - (int64_t)minVal);
        uint32_t idx = (range == 0) ? 0U : (uint32_t)(pos * (numBuckets - 1U) / range);
        uint32_t sz = sizes[idx];
        uint32_t cap = caps[idx];
        if (sz >= cap) {
            uint32_t newCap = cap * 2U;
            int32_t* newBuf = (int32_t*)pIMem->pVTbl->Alloc(pIMem, newCap * sizeof(int32_t));
            uint32_t m;
            if (newBuf == 0) {
                uint32_t k;
                for (k = 0; k < numBuckets; ++k) {
                    pIMem->pVTbl->Free(pIMem, buckets[k]);
                }
                pIMem->pVTbl->Free(pIMem, sizes);
                pIMem->pVTbl->Free(pIMem, caps);
                pIMem->pVTbl->Free(pIMem, buckets);
                return ERR_ECO_POINTER;
            }
            for (m = 0; m < sz; ++m) newBuf[m] = buckets[idx][m];
            pIMem->pVTbl->Free(pIMem, buckets[idx]);
            buckets[idx] = newBuf;
            caps[idx] = newCap;
        }
        buckets[idx][sizes[idx]++] = data[i];
    }

    out = 0;
    for (i = 0; i < numBuckets; ++i) {
        if (sizes[i] > 1U) {
            CEcoLab1_insertion_sort_int32(buckets[i], sizes[i]);
        }
        if (sizes[i] > 0U) {
            uint32_t m;
            for (m = 0; m < sizes[i]; ++m) {
                data[out++] = buckets[i][m];
            }
        }
        pIMem->pVTbl->Free(pIMem, buckets[i]);
    }
    pIMem->pVTbl->Free(pIMem, sizes);
    pIMem->pVTbl->Free(pIMem, caps);
    pIMem->pVTbl->Free(pIMem, buckets);
    return ERR_ECO_SUCCESES;
}

static int16_t CEcoLab1_bucket_sort_int64(IEcoLab1Ptr_t me, int64_t* data, uint32_t length) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    IEcoMemoryAllocator1* pIMem = pCMe->m_pIMem;
    uint32_t i;
    int64_t minVal, maxVal;
    long double fRange;
    uint32_t numBuckets;
    uint32_t* sizes = 0;
    uint32_t* caps = 0;
    int64_t** buckets = 0;
    uint32_t out = 0;

    if (length <= 1) {
        return ERR_ECO_SUCCESES;
    }

    minVal = data[0];
    maxVal = data[0];
    for (i = 1; i < length; ++i) {
        if (data[i] < minVal) minVal = data[i];
        if (data[i] > maxVal) maxVal = data[i];
    }
    if (minVal == maxVal) {
        return ERR_ECO_SUCCESES;
    }

    numBuckets = length;
    if (numBuckets > 1024U) numBuckets = 1024U;
    if (numBuckets == 0U) numBuckets = 1U;

    sizes = (uint32_t*)pIMem->pVTbl->Alloc(pIMem, numBuckets * sizeof(uint32_t));
    caps = (uint32_t*)pIMem->pVTbl->Alloc(pIMem, numBuckets * sizeof(uint32_t));
    buckets = (int64_t**)pIMem->pVTbl->Alloc(pIMem, numBuckets * sizeof(int64_t*));
    if (sizes == 0 || caps == 0 || buckets == 0) {
        if (sizes) pIMem->pVTbl->Free(pIMem, sizes);
        if (caps) pIMem->pVTbl->Free(pIMem, caps);
        if (buckets) pIMem->pVTbl->Free(pIMem, buckets);
        return ERR_ECO_POINTER;
    }
    for (i = 0; i < numBuckets; ++i) {
        sizes[i] = 0U;
        caps[i] = 4U;
        buckets[i] = (int64_t*)pIMem->pVTbl->Alloc(pIMem, caps[i] * sizeof(int64_t));
        if (buckets[i] == 0) {
            uint32_t k;
            for (k = 0; k < i; ++k) {
                pIMem->pVTbl->Free(pIMem, buckets[k]);
            }
            pIMem->pVTbl->Free(pIMem, sizes);
            pIMem->pVTbl->Free(pIMem, caps);
            pIMem->pVTbl->Free(pIMem, buckets);
            return ERR_ECO_POINTER;
        }
    }

    fRange = (long double)maxVal - (long double)minVal;
    for (i = 0; i < length; ++i) {
        long double norm = ((long double)data[i] - (long double)minVal) / fRange;
        if (norm < 0.0L) norm = 0.0L;
        if (norm > 1.0L) norm = 1.0L;
        {
            uint32_t idx = (uint32_t)(norm * (long double)(numBuckets - 1U));
        uint32_t sz = sizes[idx];
        uint32_t cap = caps[idx];
        if (sz >= cap) {
            uint32_t newCap = cap * 2U;
            int64_t* newBuf = (int64_t*)pIMem->pVTbl->Alloc(pIMem, newCap * sizeof(int64_t));
            uint32_t m;
            if (newBuf == 0) {
                uint32_t k;
                for (k = 0; k < numBuckets; ++k) {
                    pIMem->pVTbl->Free(pIMem, buckets[k]);
                }
                pIMem->pVTbl->Free(pIMem, sizes);
                pIMem->pVTbl->Free(pIMem, caps);
                pIMem->pVTbl->Free(pIMem, buckets);
                return ERR_ECO_POINTER;
            }
            for (m = 0; m < sz; ++m) newBuf[m] = buckets[idx][m];
            pIMem->pVTbl->Free(pIMem, buckets[idx]);
            buckets[idx] = newBuf;
            caps[idx] = newCap;
        }
        buckets[idx][sizes[idx]++] = data[i];
        }
    }

    out = 0;
    for (i = 0; i < numBuckets; ++i) {
        if (sizes[i] > 1U) {
            CEcoLab1_insertion_sort_int64(buckets[i], sizes[i]);
        }
        if (sizes[i] > 0U) {
            uint32_t m;
            for (m = 0; m < sizes[i]; ++m) {
                data[out++] = buckets[i][m];
            }
        }
        pIMem->pVTbl->Free(pIMem, buckets[i]);
    }
    pIMem->pVTbl->Free(pIMem, sizes);
    pIMem->pVTbl->Free(pIMem, caps);
    pIMem->pVTbl->Free(pIMem, buckets);
    return ERR_ECO_SUCCESES;
}

static int16_t CEcoLab1_bucket_sort_float(IEcoLab1Ptr_t me, float* data, uint32_t length) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    IEcoMemoryAllocator1* pIMem = pCMe->m_pIMem;
    uint32_t i;
    float minVal, maxVal;
    float range;
    uint32_t numBuckets;
    uint32_t* sizes = 0;
    uint32_t* caps = 0;
    float** buckets = 0;
    uint32_t out = 0;

    if (length <= 1) {
        return ERR_ECO_SUCCESES;
    }

    minVal = data[0];
    maxVal = data[0];
    for (i = 1; i < length; ++i) {
        if (data[i] < minVal) minVal = data[i];
        if (data[i] > maxVal) maxVal = data[i];
    }
    range = maxVal - minVal;
    if (range == 0.0f) {
        return ERR_ECO_SUCCESES;
    }

    numBuckets = length;
    if (numBuckets > 1024U) numBuckets = 1024U;
    if (numBuckets == 0U) numBuckets = 1U;

    sizes = (uint32_t*)pIMem->pVTbl->Alloc(pIMem, numBuckets * sizeof(uint32_t));
    caps = (uint32_t*)pIMem->pVTbl->Alloc(pIMem, numBuckets * sizeof(uint32_t));
    buckets = (float**)pIMem->pVTbl->Alloc(pIMem, numBuckets * sizeof(float*));
    if (sizes == 0 || caps == 0 || buckets == 0) {
        if (sizes) pIMem->pVTbl->Free(pIMem, sizes);
        if (caps) pIMem->pVTbl->Free(pIMem, caps);
        if (buckets) pIMem->pVTbl->Free(pIMem, buckets);
        return ERR_ECO_POINTER;
    }
    for (i = 0; i < numBuckets; ++i) {
        sizes[i] = 0U;
        caps[i] = 4U;
        buckets[i] = (float*)pIMem->pVTbl->Alloc(pIMem, caps[i] * sizeof(float));
        if (buckets[i] == 0) {
            uint32_t k;
            for (k = 0; k < i; ++k) {
                pIMem->pVTbl->Free(pIMem, buckets[k]);
            }
            pIMem->pVTbl->Free(pIMem, sizes);
            pIMem->pVTbl->Free(pIMem, caps);
            pIMem->pVTbl->Free(pIMem, buckets);
            return ERR_ECO_POINTER;
        }
    }

    for (i = 0; i < length; ++i) {
        float norm = (data[i] - minVal) / range;
        if (norm < 0.0f) norm = 0.0f;
        if (norm > 1.0f) norm = 1.0f;
        {
            uint32_t idx = (uint32_t)(norm * (float)(numBuckets - 1U));
            uint32_t sz = sizes[idx];
            uint32_t cap = caps[idx];
            if (sz >= cap) {
                uint32_t newCap = cap * 2U;
                float* newBuf = (float*)pIMem->pVTbl->Alloc(pIMem, newCap * sizeof(float));
                uint32_t m;
                if (newBuf == 0) {
                    uint32_t k;
                    for (k = 0; k < numBuckets; ++k) {
                        pIMem->pVTbl->Free(pIMem, buckets[k]);
                    }
                    pIMem->pVTbl->Free(pIMem, sizes);
                    pIMem->pVTbl->Free(pIMem, caps);
                    pIMem->pVTbl->Free(pIMem, buckets);
                    return ERR_ECO_POINTER;
                }
                for (m = 0; m < sz; ++m) newBuf[m] = buckets[idx][m];
                pIMem->pVTbl->Free(pIMem, buckets[idx]);
                buckets[idx] = newBuf;
                caps[idx] = newCap;
            }
            buckets[idx][sizes[idx]++] = data[i];
        }
    }

    out = 0;
    for (i = 0; i < numBuckets; ++i) {
        if (sizes[i] > 1U) {
            CEcoLab1_insertion_sort_float(buckets[i], sizes[i]);
        }
        if (sizes[i] > 0U) {
            uint32_t m;
            for (m = 0; m < sizes[i]; ++m) {
                data[out++] = buckets[i][m];
            }
        }
        pIMem->pVTbl->Free(pIMem, buckets[i]);
    }
    pIMem->pVTbl->Free(pIMem, sizes);
    pIMem->pVTbl->Free(pIMem, caps);
    pIMem->pVTbl->Free(pIMem, buckets);
    return ERR_ECO_SUCCESES;
}

static int16_t CEcoLab1_bucket_sort_double(IEcoLab1Ptr_t me, double* data, uint32_t length) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    IEcoMemoryAllocator1* pIMem = pCMe->m_pIMem;
    uint32_t i;
    double minVal, maxVal;
    double range;
    uint32_t numBuckets;
    uint32_t* sizes = 0;
    uint32_t* caps = 0;
    double** buckets = 0;
    uint32_t out = 0;

    if (length <= 1) {
        return ERR_ECO_SUCCESES;
    }

    minVal = data[0];
    maxVal = data[0];
    for (i = 1; i < length; ++i) {
        if (data[i] < minVal) minVal = data[i];
        if (data[i] > maxVal) maxVal = data[i];
    }
    range = maxVal - minVal;
    if (range == 0.0) {
        return ERR_ECO_SUCCESES;
    }

    numBuckets = length;
    if (numBuckets > 1024U) numBuckets = 1024U;
    if (numBuckets == 0U) numBuckets = 1U;

    sizes = (uint32_t*)pIMem->pVTbl->Alloc(pIMem, numBuckets * sizeof(uint32_t));
    caps = (uint32_t*)pIMem->pVTbl->Alloc(pIMem, numBuckets * sizeof(uint32_t));
    buckets = (double**)pIMem->pVTbl->Alloc(pIMem, numBuckets * sizeof(double*));
    if (sizes == 0 || caps == 0 || buckets == 0) {
        if (sizes) pIMem->pVTbl->Free(pIMem, sizes);
        if (caps) pIMem->pVTbl->Free(pIMem, caps);
        if (buckets) pIMem->pVTbl->Free(pIMem, buckets);
        return ERR_ECO_POINTER;
    }
    for (i = 0; i < numBuckets; ++i) {
        sizes[i] = 0U;
        caps[i] = 4U;
        buckets[i] = (double*)pIMem->pVTbl->Alloc(pIMem, caps[i] * sizeof(double));
        if (buckets[i] == 0) {
            uint32_t k;
            for (k = 0; k < i; ++k) {
                pIMem->pVTbl->Free(pIMem, buckets[k]);
            }
            pIMem->pVTbl->Free(pIMem, sizes);
            pIMem->pVTbl->Free(pIMem, caps);
            pIMem->pVTbl->Free(pIMem, buckets);
            return ERR_ECO_POINTER;
        }
    }

    for (i = 0; i < length; ++i) {
        double norm = (data[i] - minVal) / range;
        if (norm < 0.0) norm = 0.0;
        if (norm > 1.0) norm = 1.0;
        {
            uint32_t idx = (uint32_t)(norm * (double)(numBuckets - 1U));
            uint32_t sz = sizes[idx];
            uint32_t cap = caps[idx];
            if (sz >= cap) {
                uint32_t newCap = cap * 2U;
                double* newBuf = (double*)pIMem->pVTbl->Alloc(pIMem, newCap * sizeof(double));
                uint32_t m;
                if (newBuf == 0) {
                    uint32_t k;
                    for (k = 0; k < numBuckets; ++k) {
                        pIMem->pVTbl->Free(pIMem, buckets[k]);
                    }
                    pIMem->pVTbl->Free(pIMem, sizes);
                    pIMem->pVTbl->Free(pIMem, caps);
                    pIMem->pVTbl->Free(pIMem, buckets);
                    return ERR_ECO_POINTER;
                }
                for (m = 0; m < sz; ++m) newBuf[m] = buckets[idx][m];
                pIMem->pVTbl->Free(pIMem, buckets[idx]);
                buckets[idx] = newBuf;
                caps[idx] = newCap;
            }
            buckets[idx][sizes[idx]++] = data[i];
        }
    }

    out = 0;
    for (i = 0; i < numBuckets; ++i) {
        if (sizes[i] > 1U) {
            CEcoLab1_insertion_sort_double(buckets[i], sizes[i]);
        }
        if (sizes[i] > 0U) {
            uint32_t m;
            for (m = 0; m < sizes[i]; ++m) {
                data[out++] = buckets[i][m];
            }
        }
        pIMem->pVTbl->Free(pIMem, buckets[i]);
    }
    pIMem->pVTbl->Free(pIMem, sizes);
    pIMem->pVTbl->Free(pIMem, caps);
    pIMem->pVTbl->Free(pIMem, buckets);
    return ERR_ECO_SUCCESES;
}

static int16_t CEcoLab1_bucket_sort_long_double(IEcoLab1Ptr_t me, long double* data, uint32_t length) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    IEcoMemoryAllocator1* pIMem = pCMe->m_pIMem;
    uint32_t i;
    long double minVal, maxVal;
    long double range;
    uint32_t numBuckets;
    uint32_t* sizes = 0;
    uint32_t* caps = 0;
    long double** buckets = 0;
    uint32_t out = 0;

    if (length <= 1) {
        return ERR_ECO_SUCCESES;
    }

    minVal = data[0];
    maxVal = data[0];
    for (i = 1; i < length; ++i) {
        if (data[i] < minVal) minVal = data[i];
        if (data[i] > maxVal) maxVal = data[i];
    }
    range = maxVal - minVal;
    if (range == 0.0L) {
        return ERR_ECO_SUCCESES;
    }

    numBuckets = length;
    if (numBuckets > 1024U) numBuckets = 1024U;
    if (numBuckets == 0U) numBuckets = 1U;

    sizes = (uint32_t*)pIMem->pVTbl->Alloc(pIMem, numBuckets * sizeof(uint32_t));
    caps = (uint32_t*)pIMem->pVTbl->Alloc(pIMem, numBuckets * sizeof(uint32_t));
    buckets = (long double**)pIMem->pVTbl->Alloc(pIMem, numBuckets * sizeof(long double*));
    if (sizes == 0 || caps == 0 || buckets == 0) {
        if (sizes) pIMem->pVTbl->Free(pIMem, sizes);
        if (caps) pIMem->pVTbl->Free(pIMem, caps);
        if (buckets) pIMem->pVTbl->Free(pIMem, buckets);
        return ERR_ECO_POINTER;
    }
    for (i = 0; i < numBuckets; ++i) {
        sizes[i] = 0U;
        caps[i] = 4U;
        buckets[i] = (long double*)pIMem->pVTbl->Alloc(pIMem, caps[i] * sizeof(long double));
        if (buckets[i] == 0) {
            uint32_t k;
            for (k = 0; k < i; ++k) {
                pIMem->pVTbl->Free(pIMem, buckets[k]);
            }
            pIMem->pVTbl->Free(pIMem, sizes);
            pIMem->pVTbl->Free(pIMem, caps);
            pIMem->pVTbl->Free(pIMem, buckets);
            return ERR_ECO_POINTER;
        }
    }

    for (i = 0; i < length; ++i) {
        long double norm = (data[i] - minVal) / range;
        if (norm < 0.0L) norm = 0.0L;
        if (norm > 1.0L) norm = 1.0L;
        {
            uint32_t idx = (uint32_t)(norm * (long double)(numBuckets - 1U));
            uint32_t sz = sizes[idx];
            uint32_t cap = caps[idx];
            if (sz >= cap) {
                uint32_t newCap = cap * 2U;
                long double* newBuf = (long double*)pIMem->pVTbl->Alloc(pIMem, newCap * sizeof(long double));
                uint32_t m;
                if (newBuf == 0) {
                    uint32_t k;
                    for (k = 0; k < numBuckets; ++k) {
                        pIMem->pVTbl->Free(pIMem, buckets[k]);
                    }
                    pIMem->pVTbl->Free(pIMem, sizes);
                    pIMem->pVTbl->Free(pIMem, caps);
                    pIMem->pVTbl->Free(pIMem, buckets);
                    return ERR_ECO_POINTER;
                }
                for (m = 0; m < sz; ++m) newBuf[m] = buckets[idx][m];
                pIMem->pVTbl->Free(pIMem, buckets[idx]);
                buckets[idx] = newBuf;
                caps[idx] = newCap;
            }
            buckets[idx][sizes[idx]++] = data[i];
        }
    }

    out = 0;
    for (i = 0; i < numBuckets; ++i) {
        if (sizes[i] > 1U) {
            CEcoLab1_insertion_sort_long_double(buckets[i], sizes[i]);
        }
        if (sizes[i] > 0U) {
            uint32_t m;
            for (m = 0; m < sizes[i]; ++m) {
                data[out++] = buckets[i][m];
            }
        }
        pIMem->pVTbl->Free(pIMem, buckets[i]);
    }
    pIMem->pVTbl->Free(pIMem, sizes);
    pIMem->pVTbl->Free(pIMem, caps);
    pIMem->pVTbl->Free(pIMem, buckets);
    return ERR_ECO_SUCCESES;
}

/* Публичные функции сортировки (реализация bucket sort при сохранении сигнатур) */
static int16_t ECOCALLMETHOD CEcoLab1_qsort_int(IEcoLab1Ptr_t me, int32_t* data, uint32_t length) {
    if (me == 0 || (length > 0 && data == 0)) {
        return ERR_ECO_POINTER;
    }
    return CEcoLab1_bucket_sort_int32(me, data, length);
}

static int16_t ECOCALLMETHOD CEcoLab1_qsort_long(IEcoLab1Ptr_t me, int64_t* data, uint32_t length) {
    if (me == 0 || (length > 0 && data == 0)) {
        return ERR_ECO_POINTER;
    }
    return CEcoLab1_bucket_sort_int64(me, data, length);
}

static int16_t ECOCALLMETHOD CEcoLab1_qsort_float(IEcoLab1Ptr_t me, float* data, uint32_t length) {
    if (me == 0 || (length > 0 && data == 0)) {
        return ERR_ECO_POINTER;
    }
    return CEcoLab1_bucket_sort_float(me, data, length);
}

static int16_t ECOCALLMETHOD CEcoLab1_qsort_double(IEcoLab1Ptr_t me, double* data, uint32_t length) {
    if (me == 0 || (length > 0 && data == 0)) {
        return ERR_ECO_POINTER;
    }
    return CEcoLab1_bucket_sort_double(me, data, length);
}

static int16_t ECOCALLMETHOD CEcoLab1_qsort_long_double(IEcoLab1Ptr_t me, long double* data, uint32_t length) {
    if (me == 0 || (length > 0 && data == 0)) {
        return ERR_ECO_POINTER;
    }
    return CEcoLab1_bucket_sort_long_double(me, data, length);
}

/*
 *
 * <сводка>
 *   Функция Init
 * </сводка>
 *
 * <описание>
 *   Функция инициализации экземпляра
 * </описание>
 *
 */
int16_t ECOCALLMETHOD initCEcoLab1(/*in*/ IEcoLab1Ptr_t me, /* in */ struct IEcoUnknown *pIUnkSystem) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    IEcoInterfaceBus1* pIBus = 0;
    int16_t result = -1;

    /* Проверка указателей */
    if (me == 0 ) {
        return result;
    }

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

    /* Проверка указателей */
    if (me == 0 ) {
        return result;
    }

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);

    return result;
}

/* Create Virtual Table IEcoLab1 */
IEcoLab1VTbl g_x277FC00C35624096AFCFC125B94EEC90VTbl = {
    CEcoLab1_QueryInterface,
    CEcoLab1_AddRef,
    CEcoLab1_Release,
    CEcoLab1_MyFunction,
    CEcoLab1_qsort_int,
    CEcoLab1_qsort_long,
    CEcoLab1_qsort_float,
    CEcoLab1_qsort_double,
    CEcoLab1_qsort_long_double
};

/*
 *
 * <сводка>
 *   Функция Create
 * </сводка>
 *
 * <описание>
 *   Функция создания экземпляра
 * </описание>
 *
 */
int16_t ECOCALLMETHOD createCEcoLab1(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoLab1** ppIEcoLab1) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    CEcoLab1* pCMe = 0;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;

    /* Проверка указателей */
    if (ppIEcoLab1 == 0 || pIUnkSystem == 0) {
        return result;
    }

    /* Получение системного интерфейса приложения */
    result = pIUnkSystem->pVTbl->QueryInterface(pIUnkSystem, &GID_IEcoSystem, (void **)&pISys);

    /* Проверка */
    if (result != 0 && pISys == 0) {
        return result;
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

    /* Получение идентификатора компонента для работы с памятью */
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*)pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    /* Получение интерфейса распределителя памяти */
    pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 && pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        pISys->pVTbl->Release(pISys);
        return result;
    }

    /* Выделение памяти для данных экземпляра */
    pCMe = (CEcoLab1*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoLab1));

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = pISys;

    /* Сохранение указателя на интерфейс для работы с памятью */
    pCMe->m_pIMem = pIMem;

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;

    /* Создание таблицы функций интерфейса IEcoLab1 */
    pCMe->m_pVTblIEcoLab1 = &g_x277FC00C35624096AFCFC125B94EEC90VTbl;

    /* Инициализация данных */
    pCMe->m_Name = 0;

    /* Возврат указателя на интерфейс */
    *ppIEcoLab1 = (IEcoLab1*)pCMe;

    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);

    return 0;
}

/*
 *
 * <сводка>
 *   Функция Delete
 * </сводка>
 *
 * <описание>
 *   Функция освобождения экземпляра
 * </описание>
 *
 */
void ECOCALLMETHOD deleteCEcoLab1(/* in */ IEcoLab1* pIEcoLab1) {
    CEcoLab1* pCMe = (CEcoLab1*)pIEcoLab1;
    IEcoMemoryAllocator1* pIMem = 0;

    if (pIEcoLab1 != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Освобождение */
        if ( pCMe->m_Name != 0 ) {
            pIMem->pVTbl->Free(pIMem, pCMe->m_Name);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}