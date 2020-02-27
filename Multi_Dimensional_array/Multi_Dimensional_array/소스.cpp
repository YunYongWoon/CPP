#include<iostream>
using namespace std;


namespace MyArray {
	class Array;
	class Int;

	class Array {
		friend Int;

	private:
		const int dim; // 몇 차원 배열인지
		int* size; //size[0] ~ size[dim-1] 까지의 배열

		struct Address {
			int level;
			// 마지막 레벨(dim - 1 레벨) 은 데이터 배열을 가리키고, 그 위 상위
			// 레벨에서는 다음 Address 배열을 가리킨다.
			void* next;	 // 필요할 때 적당한 포인터 타입으로 형변환
		};

		Address* top;

	public:
		class Iterator {
			int* location;
			Array* arr;

			friend Int;
		public:
			Iterator(Array* arr, int* loc = NULL) : arr(arr) {
				location = new int[arr->dim];
				for (int i = 0; i != arr->dim; i++) 
					location[i] = (loc != NULL ? loc[i] : 0);			
			}

			Iterator(const Iterator& itr) : arr(itr.arr) {
				location = new int[arr->dim];
				for (int i = 0; i != arr->dim; i++) 
					location[i] = itr.location[i];			
			}

			~Iterator() {
				delete[] location;
			}

			Iterator& operator++() {
				if (location[0] >= arr->size[0])
					return (*this);

				bool carry = false;
				int i = arr->dim - 1;

				do { 
					location[i]++;
					if (location[i] >= arr->size[i] && i >= 1) {
						location[i] -= arr->size[i];
						carry = true;
						i--;
					}
					else 
						carry = false;
					
				} while (i >= 0 && carry);

				return (*this);
			}

			Iterator& operator=(const Iterator& itr) {
				arr = itr.arr;
				location = new int[itr.arr->dim];
				for (int i = 0; i != arr->dim; i++) 
					location[i] = itr.location[i];

				return (*this);			
			}

			Iterator operator++(int) {
				Iterator itr(*this);
				++(*this);
				return itr;
			}

			bool operator!=(const Iterator& itr) {
				if (itr.arr->dim != arr->dim)
					return true;

				for (int i = 0; i != arr->dim; i++) {
					if (itr.location[i] != location[i])
						return true;
				}

				return false;
			}

			Int operator*();
		};

		friend Iterator;
		Array(int dim, int* array_size) : dim(dim) {
			size = new int[dim];
			for (int i = 0; i < dim; i++) {
				size[i] = array_size[i];
			}

			top = new Address;
			top->level = 0;

			initialize_address(top);
		}

		Array(const Array& arr) : dim(arr.dim) {
			size = new int[dim];

			for (int i = 0; i < dim; i++) {
				size[i] = arr.size[i];
			}

			top = new Address;
			top->level = 0;

			initialize_address(top);
		}

		// address를 초기화 하는 함수. 재귀 호출로 구성되어 있다. (깊이탐색)
		void initialize_address(Address* current) {
			if (!current) // current가 NULL일시 예외처리
				return;

			if (current->level == dim - 1) { // current가 마지막 레벨일시 int 배열 할당.
				current->next = new int[size[current->level]];
				return;
			}

			current->next = new Address[size[current->level]]; // current가 마지막 level이 아닐 시 다음 포인터 배열 생성
			for (int i = 0; i != size[current->level]; i++) { // 다음 단계로 넘어가는 과정
				(static_cast<Address*>(current->next) + i)->level = current->level + 1;
				initialize_address(static_cast<Address *>(current->next) + i);
			}
		}

		// address를 할당 해제하는 함수.
		void delete_address(Address* current) {
			if (!current)
				return;
			for (int i = 0; current->level < dim - 1 && i < size[current->level]; i++) {
				delete_address(static_cast<Address*>(current->next) + i);
			}

			delete[] current->next;
		}
		Int operator[](const int index);

		~Array() {
			delete_address(top);
			delete[] size;
		}

		Iterator begin() { // {0,0,...,0}을 반환
			int *arr = new int[dim];
			for (int i = 0; i != dim; i++)
				arr[i] = 0;

			Iterator temp(this, arr);
			delete[] arr;

			return temp;
		}

		Iterator end() { // {size[0],0,....,0}을 반환
			int* arr = new int[dim];
			arr[0] = size[0];
			for (int i = 1; i < dim; i++)
				arr[i] = 0;

			Iterator temp(this, arr);
			delete[] arr;

			return temp;
		}
	};

	class Int {
	private:
		void* data;

		int level;
		Array* array;
	public:
		Int(int index, int _level = 0, void *_data = NULL, Array *_array = NULL) : level(_level), data(_data), array(_array) {
			if (_level < 1 || index >= array->size[_level - 1]) {
				data = NULL;
				return;
			}

			if (level == array->dim) { // 이제 data에 int 자료형을 저장하도록 해야 한다.
				data = static_cast<void*>((static_cast<int*>(static_cast<Array::Address*>(data)->next) + index));
			}
			else { // 그렇지 않을 경우 data에 그냥 다음 addr을 넣어준다.
				data = static_cast<void*>(static_cast<Array::Address*>(static_cast<Array::Address*>(data)->next) + index);
			}
		};

		Int(const Int& i) : data(i.data), level(i.level), array(i.array) {};

		operator int() { //Int가 Wrapper 클래스로서 동작하기에 가장 필수적인 요소인 타입 변환 연산자
			if (data)
				return *static_cast<int*>(data);
			return 0;
		}

		Int& operator=(const int& a) {
			if (data)
				*static_cast<int*>(data) = a;
			return *this;
		}

		Int operator[](const int index) {
			if (!data)
				return 0;
			return Int(index, level + 1, data, array);
		}
	};

	Int Array::operator[] (const int index) {
		return Int(index, 1, static_cast<void*>(top), this);
	}

	Int Array::Iterator::operator*() {
		Int start = arr->operator[](location[0]);

		for (int i = 1; i <= arr->dim - 1; i++) {
			start = start.operator[](location[i]);
		}
		return start;
	}
}

int main() {
	int size[] = { 2, 3, 4 };
	MyArray::Array arr(3, size);

	MyArray::Array::Iterator itr = arr.begin();
	for (int i = 0; itr != arr.end(); itr++, i++) 
		(*itr) = i;
	for (itr = arr.begin(); itr != arr.end(); itr++)
		std::cout << *itr << std::endl;

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 4; k++) {
				arr[i][j][k] = (i + 1) * (j + 1) * (k + 1) + arr[i][j][k];
			}
		}
	}
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 4; k++) {
				std::cout << i << " " << j << " " << k << " " << arr[i][j][k]
					<< std::endl;
			}
		}
	}
}

