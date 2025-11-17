#include <iostream>
#include <cstdlib>      // malloc, free
#include <new>          // std::nothrow
#ifdef _WIN32
#include <windows.h>    // Для GlobalMemoryStatusEx на Windows
#endif

using namespace std;

// https://github.com/it-school/CPP_Pointers_Practice

/* =====================================================
	Прототипы функций
	===================================================== */

	// Утилита: вывод текущего состояния памяти (только Windows)
static void printMemoryStatus(const string& message);

// Пример 1: Демонстрация передачи переменной по адресу
static void increment(int* pValue);
static void example1();

// Пример 2: Динамическое выделение одного числа с проверкой
static void example2();

// Пример 3: Изменение размера динамического массива
static void resizeArray(int*& array, int oldSize, int newSize);
static void example3();

// Пример 4: Передача массива в функцию и заполнение значениями
static void fillArray(int* arr, int size);
static void example4();

// Пример 5: Двойной указатель: выделение и запись значения
static void allocateAndSet(int** pp, int value);
static void example5();

// Пример 6: Передача адресов для изменения нескольких переменных
static void setTwoValues(int* a, int* b);
static void example6();

// Пример 7: Демонстрация утечки памяти и диагностика состояния
static void leakMemory();
static void example7();

// Пример 8: Сравнение malloc/free и new/delete
static void example8();

/* =====================================================
	Утилита: оценка доступной памяти
	===================================================== */
#ifdef _WIN32
static void printMemoryStatus(const string& message) {
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);

	if (GlobalMemoryStatusEx(&status)) {
		cout << message << " Physical memory available: " << status.ullAvailPhys / (1024 * 1024) << " МБ" << endl;
	}
	else {
		cout << "Can't get memory info." << endl;
	}
}
#else
static void printMemoryStatus(const string& message) {
	cout << message << " (Memory estimation for current OS is not available)" << endl;
}
#endif

static void increment(int* pValue) {
	(*pValue)++;
}

static void example1() {
	int x = 10;
	cout << endl << "Example 1: x before increment = " << x << endl;
	increment(&x);
	cout << "Example 1: x after increment = " << x << endl;
}

static void example2() {
	int* p = new (nothrow) int;

	if (!p)
	{
		cout << endl << "Example 2: Memory allocation failed!" << endl;
		return;
	}

	*p = 42;
	cout << endl << "Example 2: Value = " << *p << endl;
}

static void resizeArray(int*& array, int oldSize, int newSize) {
	int* newArray = new int[newSize] {0};

	int elementsToCopy = (oldSize < newSize) ? oldSize : newSize;
	for (int i = 0; i < elementsToCopy; i++) {
		newArray[i] = array[i];
	}
	delete[] array;
	array = newArray;
}

static void example3() {
	int size = 5;
	int* arr = new int[size] {9, 5, 1, 7, 3};
	cout << endl << "Example 3: Original array: ";
	for (int i = 0; i < size; i++) {
		cout << arr[i] << " ";
	}
	cout << endl;

	int resizedSize = 8;
	resizeArray(arr, size, resizedSize);

	cout << "Example 3: Resized array: ";
	for (int i = 0; i < resizedSize; i++) {
		cout << arr[i] << " ";
	}
	cout << endl;

	size = resizedSize;
	resizedSize = 3;
	resizeArray(arr, size, resizedSize);

	cout << "Example 3: Resized array one more time: ";
	for (int i = 0; i < resizedSize; i++) {
		cout << arr[i] << " ";
	}
	cout << endl;

	delete[] arr;
}

static void fillArray(int* arr, int size) {
	for (int i = 0; i < size; i++)
	{
		arr[i] = i + 1;
	}
}

static void example4() {
	int size = 10;
	int* arr = new (nothrow) int[size];
	if (!arr) {
		cout << endl << "Example 4: Memory allocation failed!" << endl;
		return;
	}

	fillArray(arr, size);

	cout << endl << "Example 4: Filled array: ";
	for (int i = 0; i < size; i++) {
		cout << arr[i] << " ";
	}
}

static void allocateAndSet(int** pp, int value) {
	*pp = new int;
	**pp = value;
}

static void example5() {
	int* p = nullptr;
	allocateAndSet(&p, 42);

	cout << endl << endl << "Example 5: Value = " << *p << endl;
	cout << "Example 5: Address in memory: " << p << endl;

	delete p;
}

static void setTwoValues(int* a, int* b) {
	*a = 10;
	*b = 20;
}

static void example6() {
	int x = 0, y = 0;
	setTwoValues(&x, &y);
	cout << endl << "Example 6: x = " << x << ", y = " << y << endl;
}

static void leakMemory() {
	printMemoryStatus("\nBefore memory leak:");
	const int leakSize = 1 * 1024 * 1024 * 1024; // 4 GB = 1_000_000_000 ints
	int* leak = new int[leakSize];
	printMemoryStatus("After memory leak:");
	
	// Note: Intentionally not deleting 'leak' to simulate memory leak
	// delete[] leak;
}

static void example7() {
	leakMemory();
	cout << endl << "Example 7: Memory leak simulated. Check memory status above." << endl;
	cin.get();
}

static void example8() {
	int size = 3;

	// C-ctyle
	int* a = (int*)malloc(sizeof(int) * size);
	if (!a) {
		cout << endl << "Example 8: malloc failed!" << endl;
		return;
	}
	a[0] = 1;
	a[1] = 2;
	a[2] = 3;

	// C++ style
	int* b = new (nothrow) int[size] {4, 5, 6};
	if (!b) {
		cout << endl << "Example 8: new failed!" << endl;
		free(a);
		return;
	}
	cout << endl << "Example 8: C-style array: ";
	for (int i = 0; i < size; i++) {
		cout << a[i] << " ";
	}
	cout << endl << "Example 8: C++-style array: ";
	for (int i = 0; i < size; i++) {
		cout << b[i] << " ";
	}

	free(a);  // Освобождение памяти, выделенной через malloc
	delete[] b;  // Освобождение памяти, выделенной через new
}

/* =====================================================
	Точка входа: main
	===================================================== */
int main() {
	//setlocale(LC_ALL, ""); // поддержка вывода на кириллице в консоли Windows

	example1();
	example2();
	example3();
	example4();
	example5();
	example6();
	example7();
	example8();

	cin.get();
	return 0;
}