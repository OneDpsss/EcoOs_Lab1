# Разработка компонента блочной сортировки (Bucket Sort) для системы калькулятора

## 1. Постановка задачи

**Цель работы**: разработка нового компонента для системы калькулятора, реализующего интерфейс сортировки массивов методом блочной сортировки (Bucket Sort).

**Требования к компоненту**:
- Обеспечивать универсальность
- Поддерживать сортировку массивов типов данных:
  - `int`
  - `long` 
  - `float`
  - `double`
  - `long double`
- Оформление в виде отдельного модуля
- Интеграция в существующую систему через стандартные интерфейсы
- Единообразный метод сортировки для всех поддерживаемых типов

## 2. Теоретические основы: алгоритм Bucket Sort

Блочная сортировка относится к алгоритмам распределяющей сортировки.

**Принцип работы**:
1. Входной массив делится на несколько подмножеств — блоков (buckets)
2. Каждый блок заполняется элементами, значение которых попадает в определённый диапазон
3. После распределения данных каждый блок сортируется отдельно
4. Блоки последовательно объединяются в один отсортированный массив

**Порядок действий**:
1. Создать k пустых блоков
2. Для каждого элемента определить индекс соответствующего блока и поместить его туда
3. Применить сортировку внутри каждого блока (обычно вставками)
4. Объединить содержимое блоков в порядке их индексов

## 3. Оценка сложности

**Параметры**:
- `n` — количество элементов
- `k` — число блоков

**Временная сложность**:
- Лучший случай: `O(n + k)` — элементы равномерно распределены, блоки малы
- Средний случай: `O(n + k)` — распределение относительно равномерное  
- Худший случай: `O(n²)` — все элементы попали в один блок

**Память**: `O(n + k)` — для хранения исходных данных и блоков

**Преимущество**: возможность распараллеливания работы с блоками, хорошая масштабируемость для больших массивов с равномерным распределением данных.

## 4. Общая схема реализации
# Пример реализации для типа int:
```c
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
```

  **Аналогичным образом реализуются версии сортировки для:**
  
  -int
  
  -long
  
  -double
  
  -long double


## 5. Вывод
<img width="2117" height="801" alt="image" src="https://github.com/user-attachments/assets/a9acb7db-85a6-485e-98c2-0eeaf1936437" />

  
  







