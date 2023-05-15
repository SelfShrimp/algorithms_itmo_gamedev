#include "gtest/gtest.h"
#include <iostream>
#include <windows.h>
#define MY_ARRAY_H
#define DEFAULT_CAPACITY 8
#define MEMORY_MULT 2

template <typename T>
class MyArray final
{
private:

	T* dataptr_ = nullptr;
	int capacity_ = DEFAULT_CAPACITY;
	int size_ = 0;

	class ConstIterator
	{

	protected:

		MyArray<T>* arrptr_;
		T* elemptr_;
		int pos_;
		bool isDirect_;

	public:

		ConstIterator(MyArray<T>* arrptr, bool isDirect)
		{
			arrptr_ = arrptr;
			isDirect_ = isDirect;
			if (isDirect) pos_ = 0;
			else pos_ = arrptr_->size() - 1;
			elemptr_ = arrptr_->dataptr_ + pos_;
		}

		bool hasNext()
		{
			if (isDirect_) return !(pos_ == arrptr_->size());
			else return !(pos_ == -1);
		}

		void next()
		{
			if (isDirect_) pos_++;
			else pos_--;
			elemptr_ = arrptr_->dataptr_ + pos_;
		}

		const T& get() const
		{
			return *elemptr_;
		}
	};

	class Iterator : public ConstIterator
	{

	public:

		Iterator(MyArray<T>* arrptr, bool isDirect) : ConstIterator(arrptr, isDirect)
		{

		}

		void set(const T& value)
		{
			*(ConstIterator::elemptr_) = value;
		}
	};

public:

	MyArray()
	{
		dataptr_ = (T*)malloc(DEFAULT_CAPACITY * sizeof(T));
	}

	MyArray(int capacity)
	{
		dataptr_ = (T*)malloc(capacity * sizeof(T));
		capacity_ = capacity;
	}

	MyArray(const MyArray& otherArr)
	{
		capacity_ = otherArr.capacity_;
		size_ = otherArr.size_;
		dataptr_ = (T*)malloc(capacity_ * sizeof(T));
		for (int i = 0; i < size_; i++) {
			new(dataptr_ + i) T(otherArr.dataptr_[i]);
		}
	}

	MyArray(MyArray&& otherArr)
	{
		capacity_ = otherArr.capacity_;
		size_ = otherArr.size_;
		free(dataptr_);
		dataptr_ = otherArr.dataptr_;
		otherArr.size_ = 0;
		otherArr.capacity_ = DEFAULT_CAPACITY;
		otherArr.dataptr_ = nullptr;
	}

	~MyArray()
	{
		for (int i = 0; i < size_; i++) dataptr_[i].~T();
		free(dataptr_);
	}

	int insert(const T& value)
	{
		if (size_ == capacity_)
		{
			T* ndataptr_ = dataptr_;
			dataptr_ = (T*)malloc(capacity_ * MEMORY_MULT * sizeof(T));
			for (int i = 0; i < capacity_; i++) new (dataptr_ + i) T(std::move(ndataptr_[i]));
			capacity_ *= MEMORY_MULT;
			for (int i = 0; i < size_; i++) ndataptr_[i].~T();
			ndataptr_ = nullptr;
		}
		new (dataptr_ + size_) T(value);
		size_++;
		return size_ - 1;
	}

	/*int insert(int index, const T& value) {
		if (size_ == capacity_)
		{
			T* ndataptr_ = dataptr_;
			dataptr_ = (T*)malloc(capacity_ * MEMORY_MULT * sizeof(T));
			for (int i = capacity_ - 1; i >= index; i--) new (dataptr_ + i + 1) T(std::move(ndataptr_[i]));
			for (int i = 0; i < index; i++) new (dataptr_ + i) T(std::move(ndataptr_[i]));
			capacity_ *= MEMORY_MULT;
			for (int i = 0; i < size_; i++) ndataptr_[i].~T();
			ndataptr_ = nullptr;
		}
		if (index < 0 || index >= size_)
			return -1;

		new(dataptr_ + size_) T(std::move(dataptr_[size_ - 1]));

		for (int i = size_ - 1; i > index; i--)
			dataptr_[i] = T(std::move(dataptr_[i - 1]));

		dataptr_[index].~T();
		new(dataptr_ + index) T(value);
		size_++;

		return index;
	}*/

	int insert(int index, const T& value)
	{
		if (index < capacity_ - 1)
		{
			if (size_ == capacity_)
			{
				T* ndataptr_ = dataptr_;
				dataptr_ = (T*)malloc(capacity_ * MEMORY_MULT * sizeof(T));
				for (int i = capacity_ - 1; i >= index; i--) new (dataptr_ + i + 1) T(std::move(ndataptr_[i]));
				for (int i = 0; i < index; i++) new (dataptr_ + i) T(std::move(ndataptr_[i]));
				capacity_ *= MEMORY_MULT;
				for (int i = 0; i < size_; i++) ndataptr_[i].~T();
				ndataptr_ = nullptr;
			}
			else for (int i = size_ - 1; i >= index; i--)
			{
				dataptr_[i + 1].~T();
				new (dataptr_ + i + 1) T(std::move(dataptr_[i]));
			}
			new (dataptr_ + index) T(value);
			size_++;
			return size_ - 1;
		}
		else return -1;
	}

	void remove(int index)
	{
		if (index < size_ - 1)
		{
			for (int i = index; i < size_ - 1; i++)
			{
				dataptr_[i].~T();
				new (dataptr_ + i) T(std::move(dataptr_[i + 1]));
			}
			dataptr_[size_ - 1].~T();
			size_--;
		}
	}

	int size() const
	{
		return size_;
	}

	Iterator iterator() { return Iterator(this, true); }
	ConstIterator cIterator() { return ConstIterator(this, true); }

	Iterator reverseIterator() { return Iterator(this, false); }
	ConstIterator cReverseIterator() { return ConstIterator(this, false); }

	const T& operator[](int index) const
	{
		return (dataptr_[index]);
	}

	T& operator[](int index)
	{
		return (dataptr_[index]);
	}

	MyArray<T>& operator=(MyArray<T> otherArray)
	{
		std::swap(dataptr_, otherArray.dataptr_);
		std::swap(size_, otherArray.size_);
		std::swap(capacity_, otherArray.capacity_);

		return *this;
	}

	MyArray<T>& operator=(MyArray<T>&& otherArray)
	{
		if (this != &otherArray)
		{
			for (int i = 0; i < size_; i++) dataptr_[i].~T();
			free(dataptr_);

			capacity_ = otherArray.capacity_;
			size_ = otherArray.size_;
			dataptr_ = otherArray.dataptr_;

			otherArray._itemsArray = nullptr;
			otherArray._currentCapacity = 0;
			otherArray._currentSize = 0;
		}
		return *this;
	}

	T* getPointer()
	{
		return dataptr_;
	}

	int getCapacity()
	{
		return capacity_;
	}
};

//TEST(TestCaseName, TestName) {
//	EXPECT_EQ(1, 1);
//	EXPECT_TRUE(false);
//}

TEST(DynamicArrayTest, InitInt) {
	MyArray<int> arr;
	EXPECT_EQ(arr.size(), 0);
}

TEST(DynamicArrayTest, ExpandInt) {
	MyArray<int> arr(2);
	int* firstPtr = arr.getPointer();
	int firstCap = arr.getCapacity();
	arr.insert(1);
	arr.insert(2);
	arr.insert(3);
	int* secPtr = arr.getPointer();
	int secCap = arr.getCapacity();
	EXPECT_EQ(firstCap, secCap);
	EXPECT_EQ(firstPtr, secPtr);
}

TEST(DynamicArrayTest, ContentInt) {
	MyArray<int> arr(7);
	arr.insert(1);
	arr.insert(2);
	arr.insert(3);
	arr.insert(4);
	arr.insert(5);
	arr.insert(6);
	arr.insert(7);
	EXPECT_EQ(arr[0], 1);
	EXPECT_EQ(arr[1], 2);
	EXPECT_EQ(arr[2], 3);
	EXPECT_EQ(arr[3], 4);
	EXPECT_EQ(arr[4], 5);
	EXPECT_EQ(arr[5], 6);
	EXPECT_EQ(arr[6], 7);
}

TEST(DynamicArrayTest, ContentString) {
	MyArray<std::string> arr(7);
	arr.insert("One");
	arr.insert("Two");
	arr.insert("Three");
	arr.insert("Four");
	arr.insert("Five");
	arr.insert("Six");
	arr.insert("Seven");
	EXPECT_EQ(arr[0], "One");
	EXPECT_EQ(arr[1], "Two");
	EXPECT_EQ(arr[2], "Three");
	EXPECT_EQ(arr[3], "Four");
	EXPECT_EQ(arr[4], "Five");
	EXPECT_EQ(arr[5], "Six");
	EXPECT_EQ(arr[6], "Seven");
}

TEST(DynamicArrayTest, InsertAndRemoveString) {
	MyArray<std::string> arr(5);
	arr.insert("One");
	arr.insert("Two");
	arr.insert("Three");
	arr.insert(1, "Four");
	EXPECT_EQ(arr[2],"Four");
	arr.insert(0, "Five");
	EXPECT_EQ(arr[0],"Five");
	EXPECT_EQ(arr[3],"Four");
	arr.remove(3);
	arr.remove(2);
	EXPECT_EQ(arr[0],"Five");
	EXPECT_EQ(arr[1],"One");
	EXPECT_EQ(arr[2],"Three");
}

TEST(DynamicArrayTest, IterSet) {
	MyArray<int> arr(4);
	arr.insert(1);
	arr.insert(2);
	arr.insert(3);
	arr.insert(4);
	auto iter = arr.iterator();
	for (auto iter = arr.iterator(); iter.hasNext(); iter.next()) iter.set(iter.get() + 1);
	EXPECT_EQ(arr[0], 2);
	EXPECT_EQ(arr[1], 3);
	EXPECT_EQ(arr[2], 4);
	EXPECT_EQ(arr[3], 5);
}

TEST(DynamicArrayTest, IterBorders) {
	MyArray<int> arr(8);
	arr.insert(1);
	arr.insert(2);
	arr.insert(3);
	arr.insert(4);
	arr.insert(5);
	arr.insert(6);
	arr.insert(7);
	arr.insert(8);

	auto iter = arr.iterator();
	EXPECT_EQ(iter.get(), arr[0]);
	for (int i = 0; i < arr.size() - 1; i++) iter.next();
	auto rIter = arr.reverseIterator();
	EXPECT_EQ(iter.get(), rIter.get());

	iter = arr.reverseIterator();
	EXPECT_EQ(iter.get(), arr[arr.size() - 1]);
	for (int i = 0; i < arr.size() - 1; i++) iter.next();
	rIter = arr.iterator();
	EXPECT_EQ(iter.get(), rIter.get());
}

TEST(DynamicArrayTest, IterLoop) {
	MyArray<int> arr(8);
	arr.insert(1);
	arr.insert(2);
	arr.insert(3);
	arr.insert(4);
	arr.insert(5);
	arr.insert(6);
	arr.insert(7);
	arr.insert(8);

	int i = 0;
	for (auto iter = arr.iterator(); iter.hasNext(); iter.next())
	{
		EXPECT_EQ(arr[i], iter.get());
		i++;
	}

	i = arr.size() - 1;
	for (auto iter = arr.reverseIterator(); iter.hasNext(); iter.next())
	{
		EXPECT_EQ(arr[i], iter.get());
		i--;
	}
}

TEST(DynamicArrayTest, InsertWrongIndex) {
	MyArray<int> arr(4);
	arr.insert(1);
	arr.insert(2);
	arr.insert(3);
	arr.insert(4);
	arr.insert(10, 5);
	EXPECT_EQ(arr.size(), 4);
	EXPECT_EQ(arr.getCapacity(), 4);
}

TEST(DynamicArrayTest, ConstIterLoop) {
	MyArray<int> arr(8);
	arr.insert(1);
	arr.insert(2);
	arr.insert(3);
	arr.insert(4);
	arr.insert(5);
	arr.insert(6);
	arr.insert(7);
	arr.insert(8);

	int i = 0;
	for (auto iter = arr.cIterator(); iter.hasNext(); iter.next())
	{
		EXPECT_EQ(arr[i], iter.get());
		i++;
	}

	i = arr.size() - 1;
	for (auto iter = arr.cReverseIterator(); iter.hasNext(); iter.next())
	{
		EXPECT_EQ(arr[i], iter.get());
		i--;
	}
}