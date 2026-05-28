#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace chrono;

bool isSorted(vector<int> arr)
{
    for (int i = 0; i < arr.size(); i++)
    {
        for (int j = i + 1; j < arr.size(); j++)
        {
            if (arr[i] > arr[j])
            {
                return false;
            }
        }
    }

    return true;
}

void insertionSort(vector<int>& arr)
{
    int n = arr.size();

    for (int i = 1; i < n; i++)
    {
        int temp = arr[i];
        int j = i - 1;

        while (j >= 0)
        {
            if (arr[j] > temp)
            {
                arr[j + 1] = arr[j];
                j--;
            }
            else
            {
                break;
            }
        }

        arr[j + 1] = temp;
    }
}

void insertionSortRange(vector<int>& arr, int left, int right)
{
    for (int i = left + 1; i <= right; i++)
    {
        int temp = arr[i];
        int j = i - 1;

        while (j >= left && arr[j] > temp)
        {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = temp;
    }
}

int medianOfThree(vector<int>& arr, int left, int right)
{
    int mid = (left + right) / 2;

    if (arr[left] > arr[mid])
    {
        swap(arr[left], arr[mid]);
    }

    if (arr[left] > arr[right])
    {
        swap(arr[left], arr[right]);
    }

    if (arr[mid] > arr[right])
    {
        swap(arr[mid], arr[right]);
    }

    swap(arr[mid], arr[right - 1]);

    return arr[right - 1];
}

void quickSortRecursive(vector<int>& arr, int left, int right)
{
    if (left >= right)
    {
        return;
    }

    if (right - left <= 20)
    {
        insertionSortRange(arr, left, right);
        return;
    }

    int pivot = medianOfThree(arr, left, right);

    int i = left;
    int j = right - 1;

    while (true)
    {
        while (arr[++i] < pivot)
        {
        }

        while (arr[--j] > pivot)
        {
        }

        if (i < j)
        {
            swap(arr[i], arr[j]);
        }
        else
        {
            break;
        }
    }

    swap(arr[i], arr[right - 1]);

    quickSortRecursive(arr, left, i - 1);
    quickSortRecursive(arr, i + 1, right);
}

void quickSort(vector<int>& arr)
{
    if (arr.size() <= 1)
    {
        return;
    }

    quickSortRecursive(arr, 0, arr.size() - 1);
}

void mergeParts(vector<int>& arr, int left, int mid, int right)
{
    vector<int> leftArray;
    vector<int> rightArray;

    for (int i = left; i <= mid; i++)
    {
        leftArray.push_back(arr[i]);
    }

    for (int i = mid + 1; i <= right; i++)
    {
        rightArray.push_back(arr[i]);
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < leftArray.size() && j < rightArray.size())
    {
        if (leftArray[i] <= rightArray[j])
        {
            arr[k] = leftArray[i];
            i++;
        }
        else
        {
            arr[k] = rightArray[j];
            j++;
        }

        k++;
    }

    while (i < leftArray.size())
    {
        arr[k] = leftArray[i];
        i++;
        k++;
    }

    while (j < rightArray.size())
    {
        arr[k] = rightArray[j];
        j++;
        k++;
    }
}

void mergeSort(vector<int>& arr)
{
    int n = arr.size();

    int size = 1;

    while (size < n)
    {
        int left = 0;

        while (left < n - 1)
        {
            int mid = left + size - 1;
            int right = left + 2 * size - 1;

            if (mid >= n)
            {
                mid = n - 1;
            }

            if (right >= n)
            {
                right = n - 1;
            }

            if (mid < right)
            {
                mergeParts(arr, left, mid, right);
            }

            left = left + 2 * size;
        }

        size = size * 2;
    }
}

void heapify(vector<int>& arr, int n, int i)
{
    int largest = i;

    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n)
    {
        if (arr[left] > arr[largest])
        {
            largest = left;
        }
    }

    if (right < n)
    {
        if (arr[right] > arr[largest])
        {
            largest = right;
        }
    }

    if (largest != i)
    {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;

        heapify(arr, n, largest);
    }
}

void heapSort(vector<int>& arr)
{
    int n = arr.size();

    for (int i = n / 2 - 1; i >= 0; i--)
    {
        heapify(arr, n, i);
    }

    for (int i = n - 1; i >= 1; i--)
    {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        heapify(arr, i, 0);
    }
}

vector<int> generateSortedData(int n)
{
    vector<int> arr;

    for (int i = 1; i <= n; i++)
    {
        arr.push_back(i);
    }

    return arr;
}

vector<int> generateReverseData(int n)
{
    vector<int> arr;

    for (int i = n; i >= 1; i--)
    {
        arr.push_back(i);
    }

    return arr;
}

vector<int> generateRandomPermutation(int n)
{
    vector<int> arr;

    for (int i = 1; i <= n; i++)
    {
        arr.push_back(i);
    }

    for (int i = n - 1; i >= 1; i--)
    {
        int j = rand() % (i + 1);

        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    return arr;
}

vector<int> generateMergeWorstCase(vector<int> arr)
{
    int n = arr.size();

    if (n <= 1)
    {
        return arr;
    }

    vector<int> left;
    vector<int> right;

    for (int i = 0; i < n; i++)
    {
        if (i % 2 == 0)
        {
            left.push_back(arr[i]);
        }
        else
        {
            right.push_back(arr[i]);
        }
    }

    left = generateMergeWorstCase(left);
    right = generateMergeWorstCase(right);

    vector<int> result;

    for (int i = 0; i < left.size(); i++)
    {
        result.push_back(left[i]);
    }

    for (int i = 0; i < right.size(); i++)
    {   
        result.push_back(right[i]);
    }

    return result;
}

vector<int> generateMergeWorstCaseData(int n)
{
    vector<int> arr = generateSortedData(n);
    vector<int> result = generateMergeWorstCase(arr);

    return result;
}

double measureTime(void (*sortFunction)(vector<int>&), vector<int> data, int repeatTimes)
{
    double total = 0.0;

    for (int i = 0; i < repeatTimes; i++)
    {
        vector<int> copyData;

        for (int j = 0; j < data.size(); j++)
        {
            copyData.push_back(data[j]);
        }

        auto start = high_resolution_clock::now();

        sortFunction(copyData);

        auto end = high_resolution_clock::now();

        duration<double, milli> elapsed = end - start;

        total = total + elapsed.count();

        if (!isSorted(copyData))
        {
            cout << "Sorting error!" << endl;
        }
    }

    return total / repeatTimes;
}

double measureWorstRandomTime(void (*sortFunction)(vector<int>&), int n, int permutationCount)
{
    double maxTime = 0.0;

    for (int i = 0; i < permutationCount; i++)
    {
        vector<int> data = generateRandomPermutation(n);

        auto start = high_resolution_clock::now();

        sortFunction(data);

        auto end = high_resolution_clock::now();

        duration<double, milli> elapsed = end - start;

        if (elapsed.count() > maxTime)
        {
            maxTime = elapsed.count();
        }

        if (!isSorted(data))
        {
            cout << "Sorting error!" << endl;
        }
    }

    return maxTime;
}

void compositeSort(vector<int>& arr)
{
    int n = arr.size();

    if (n <= 100)
    {
        insertionSort(arr);
    }
    else if (n <= 2000)
    {
        quickSort(arr);
    }
    else if (n <= 4000)
    {
        mergeSort(arr);
    }
    else
    {
        heapSort(arr);
    }
}

int main()
{
    srand(time(0));

    vector<int> sizes;

    sizes.push_back(500);
    sizes.push_back(1000);
    sizes.push_back(2000);
    sizes.push_back(3000);
    sizes.push_back(4000);
    sizes.push_back(5000);

    int repeatTimes = 5;
    int permutationCount = 10;

    cout << fixed << setprecision(4);

    cout << "Clock used: C++ chrono high_resolution_clock" << endl;
    cout << "Repeat times: " << repeatTimes << endl;
    cout << "Random permutations: " << permutationCount << endl;
    cout << endl;

    cout << "================ Worst-Case Runtime Experiment ================" << endl;

    cout << setw(8) << "n"
         << setw(18) << "Insertion"
         << setw(18) << "Quick"
         << setw(18) << "Merge"
         << setw(18) << "Heap"
         << setw(18) << "Composite"
         << endl;

    cout << "--------------------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < sizes.size(); i++)
    {
        int n = sizes[i];

        vector<int> insertionData = generateReverseData(n);
        vector<int> mergeData = generateMergeWorstCaseData(n);
        vector<int> compositeData = generateReverseData(n);

        double insertionTime = measureTime(insertionSort, insertionData, repeatTimes);
        double quickTime = measureWorstRandomTime(quickSort, n, permutationCount);
        double mergeTime = measureTime(mergeSort, mergeData, repeatTimes);
        double heapTime = measureWorstRandomTime(heapSort, n, permutationCount);
        double compositeTime = measureTime(compositeSort, compositeData, repeatTimes);

        cout << setw(8) << n
             << setw(18) << insertionTime
             << setw(18) << quickTime
             << setw(18) << mergeTime
             << setw(18) << heapTime
             << setw(18) << compositeTime
             << endl;
    }

    cout << endl;
    cout << "Unit: milliseconds" << endl;

    return 0;
}