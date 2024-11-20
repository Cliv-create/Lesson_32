#include <iostream>
#include <windows.h>
#include <vector>
#include <memory.h> // For memcpy_s. Or change for string.h if needed (should include both)
using namespace std;

class Student {
	static int students_count;

	int* rates = nullptr;
	char* name = nullptr;
	int rates_count = 0;
	int age;

public:
	static int GetStudentsCount() {
		return students_count;
	}

	void AddRate(int rate) {
		if (rate < 1 || rate > 12)
			throw "Incorrect value.";
		if (rates_count == 0) {
			rates = new int[1];
			rates[0] = rate;
		}
		else {
			int* temp = new int[rates_count + 1];
			for (int i = 0; i < rates_count; i++)
				temp[i] = rates[i];
			temp[rates_count] = rate;
			delete[] rates;
			rates = temp;
		}
		rates_count++;
	}

	vector<int> GetRates() const {
		vector<int> temp;
		for (int i = 0; i < rates_count; i++)
			temp.push_back(rates[i]);
		return temp;
	}

	int GetRateByIndex(unsigned int index) {
		if (index >= rates_count) throw "Incorrect index!\n";
		return rates[index];
	}

	void SetName(const char* name) {
		if (this->name != nullptr)
			delete[] this->name;
		int length = strlen(name) + 1;
		this->name = new char[length];
		strcpy_s(this->name, length, name);
	}

	string GetName() const {
		return string(name);
	}

	explicit Student(string name) : Student(name.c_str(), 18) {}

	explicit Student(int value) : Student("Alex", value) {}

	explicit Student() : Student(35) {}

	explicit Student(int* ar) : Student("Alex", 35) {
		rates = ar;
		rates_count = _msize(ar) / sizeof(int); // 5
		cout << rates_count << "\n";
	}

	Student(const Student& original) {
		cout << "KK\n";
		SetName(original.name);
		age = original.age;
		for (int rate : original.GetRates())
			AddRate(rate);
		students_count++;
	}

	explicit Student(const char* name, int age) {
		SetName(name);
		SetAge(age);
		students_count++;
	}

	void SetAge(int age) {
		this->age = age;
	}

	int GetAge() const {
		return age;
	}

	int GetRatesCount() const {
		return rates_count;
	}

	void Print() const {
		cout << name << ", " << age << "\n";
		for (int i = 0; i < rates_count; i++) {
			cout << rates[i] << ", ";
		}
		cout << "\n\n";
	}

	~Student() {
		cout << "DEST\n";
		if (name != nullptr) {
			delete[] name;
			name = nullptr;
		}
		if (rates != nullptr) {
			delete[] rates;
			rates = nullptr;
		}
		students_count--;
	}

	// возврат Student& позволит на стороне клиента делать множественные присваивания по синтаксису a = b = c = d = a = e; 
	// если не хочется такие вещи разрешать клиенту, то возвращается просто void
	Student& operator = (/*const Student* this, */const Student& original)
	{ // неявный нулевой параметр this позволит понять, где объект-копия
		// единственный явный параметр - ссылка на оригинал
		cout << "OPERATOR =\n";
		// проверка на самоприсваивание
		if (this == &original) { // объект один и тот же что слева, что справа от =
			// cout << "Ты шо, с дуба рухнул? Это же самоприсваивание!!!\n";
			// throw "OOPS! Error! operator =\n";
			return *this; // возвращаем тот же самый объект
		}

		// очистка "старого" состояния копии
		/*if (this->name != nullptr) {
			delete[] this->name;
			this->name = nullptr;
		}
		if (this->rates != nullptr) {
			delete[] this->rates;
			this->rates = nullptr;
		}*/
		Student::~Student(); // деструктор можно вызывать явно!!!

		// большую часть логики конструктора копирования можно позаимствовать (без явного вызова, потому что в КК есть логика по изменению количества объектов)
		/*SetName(original.name);
		age = original.age;
		for (int rate : original.GetRates())
			AddRate(rate);
			*/
		Student::Student(original);
	}
};

int Student::students_count = 0;

// пример на ситуацию номер 2
void Exam(Student s) {

}

// пример ситуации номер 3 (КК)
Student Create() {
	Student temp;
	return temp;
}

template <typename T>
class Vector {
	unsigned int size = 0; // количество действительно присутствующих элементов в контейнере
	unsigned int capacity = 10; // ёмкость (вместительность, запас памяти)
	T* data = nullptr; // указатель на динамический массив целых чисел

	void EnsureCapacity() {
		// если весь резерв памяти исчерпан
		if (size == capacity) {
			capacity *= 2; // собираемся увеличить резерв памяти в 2 раза
			T* temp = new T[capacity];
			/*
			for (int i = 0; i < size; ++i) { // memcpy_s
				temp[i] = data[i];
			}
			*/
			memcpy_s(temp, _msize(temp), data, size * _msize(temp));
			delete[] data;
			data = temp;
		}
	}

public:
	Vector() : Vector(10)
	{
		// cout << "C-TOR WITHOUT PARAMS!\n";
	}

	Vector(unsigned int capacity)
	{
		if (capacity < 10)
		{
			capacity = 10;
		}
		this->capacity = capacity;
		data = new T[capacity];
		// cout << "C-TOR WITH PARAMS!\n";
	}

	~Vector()
	{
		// cout << "DESTRUCTOR!\n";
		if (data != nullptr)
		{
			delete[] data;
			data = nullptr;
		}
	}

	// Copy c-tor
	Vector(const Vector& original) {
		for (T value : original.GetVector()) {
			/*this->*/PushBack(value); // Adding value from original array copy to this.
		}
		// this->size = original.size;
		// this->capacity = original.capacity;
	}

	/// <summary>
	/// Returns copy of data at index value.
	/// </summary>
	/// <param name="index">Index from which data copy will return.</param>
	/// <returns>Data value copy from index.</returns>
	T operator [] (unsigned int index) {
		return data[index];
	}

	Vector& operator = (/*const Vector* this, */const Vector& original) {
		if (this == &original) {
			return *this;
		}
		Vector::~Vector();
		Vector::Vector(original);
	}

	void PushBack(T value)
	{
		EnsureCapacity(); // проверка, хватит ли места для нового элемента
		data[size] = value;
		size++;
	}

	void PushFront(T value)
	{
		EnsureCapacity();
		for (int i = size; i > 0; i--) // i = 1
		{
			data[i] = data[i - 1]; // data[1] = data[0]
		}
		data[0] = value;
		size++;
	}

	void PopBack() {
    	if (size > 0) {
    	    --size; // уменьшаем размер, освобождаем последний элемент логически
    	}
	}

	void PopFront(T value)
	{
		EnsureCapacity();
		for (int i = size; i > 0; i--) // i = 1
		{
			data[i] = data[i - 1]; // data[1] = data[0]
		}
		// data[0] = value; // First element will stay clear.
		if (size > 0) {
			--size;
		}
	}

	void TrimToSize() {
		capacity = size;
		T* temp = new T[capacity];
		/*
		for (int i = 0; i < size; ++i) { // memcpy_s
			temp[i] = data[i];
		}
		*/
		memcpy_s(temp, _msize(temp), data, size * _msize(temp));
		delete[] data;
		data = temp;
	}

	void Clear()
	{
		size = 0;
	}

	bool IsEmpty() const
	{
		return size == 0;
	}

	void Print() const
	{
		if (IsEmpty())
		{
			cout << "Vector is empty.\n";
			return;
		}

		for (int i = 0; i < size; i++)
		{
			cout << data[i] << " ";
		}
		cout << "\n";
	}

	

	int IndexOf(T value) {
		for (unsigned int i = 0; i < size; i++)
		{
			if (this->data[i] == value) {
				return i;
			}
		}
		return -1;
	}

	int LastIndexOf(T value) {
		for (int i = size - 1; i >= 0; i--)
		{
			if (this->data[i] == value) {
				return i;
			}
		}
		return -1;
	}

	// Clone function
	Vector<T> GetVector() const {
		Vector<T> temp;
		for (int i = 0; i < size; i++)
		{
			temp.PushBack(data[i]);
		}
		return temp;
	}

	T GetElementAt(unsigned int index) {
		if (index > 0 && index <= size) {
			return data[index];
		}
		else throw "Wrong index value. Expected >0 and <= size of vector.";
	}

	unsigned int GetSize() const {
		return size;
	}

	unsigned int GetCapacity() const {
		return capacity;
	}

};

ostream& operator << (ostream& os, const Vector& student);
istream& operator >> (istream& is, Vector& student);

ostream& operator << (ostream% os, const Vector& original) {
	original.Print();
	return os;
}

/*
istream& operator >> (istream& is, Vector& original) {

}
*/


int main() {
	setlocale(0, "");
	Student a;
	a.Print();
	Student b = a; // пример на ситуацию номер 1 = KK
	Exam(a);
	Create();

	// 4 ситуации копирования:
	// 1) создание НОВОГО объекта на основе уже существующего (решается КК)
	// 2) передача объекта в функцию/метод по значению (КК) - параметр будет НОВЫМ объектом
	// 3) возврат объекта из функции/метода по значению (КК) - то что возвращается, будет строиться с нуля
	// 4) запись в уже СУЩЕСТВУЮЩИЙ объект состояния другого уже существующего объекта (КК НЕ РЕШАЕТСЯ!!!)
	//a = a; // здесь вызовется не КК (потому что объект b уже есть, это "старый" объект), а вызовется операция = (которая в классе есть по дефолту, но она выполняет ПОБИТОВОЕ КОПИРОВАНИЕ)
	// a.operator= (a); // <<< для компилятора происходит следующий вызов
	// Student::operator=(&a, a);

	/*
	while (true) {
		a = b;
	}
	*/
	

	// проблема: дефолтовая перегрузка = скопирует значение указателя из оригинала в копию, получится 2 указателя на одну и ту же память, в итоге первый деструктор сработает как надо, а вот уже второй - попытается ПОВТОРНО очистить память, которой уже нет
	// что делать?
	// 1) реализовать перегрузку операции = ЯВНО в виде метода класса (это единственный вариант)
	// 2) в первую очередь, сделать проверку на самоприсваивание a = a;
	// 3) в перегрузке избавиться от "старого" состояния, полностью почистить память (если она была выделена)
	// 4) после чего выделить "новую" память для копируемого состояния с оригинала
	b.Print();
	cout << "HAPPY END!\n";
}