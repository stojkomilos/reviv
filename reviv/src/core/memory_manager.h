#pragma once

#include<memory>

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T>
using Ref = std::shared_ptr<T>;

/*
template<typename T>
class Ref
{

};*/



/*
#include<cstddef>
#include<iostream>
#include<assert.h>

class MemoryManager {
public:
	template <typename T>
	class Array2D {
	public:
		size_t height, width;
		T* p;
		int index(size_t a, size_t b);
		T* adr(size_t a, size_t b);
		Array2D(size_t height, size_t width);
		void setUp(size_t sizeY, size_t sizeX);
		Array2D();
		~Array2D();
	};

	template <typename T>
	class Array3D {
	public:
		size_t depth, height, width;
		T* p;
		int index(size_t a, size_t b, size_t c);
		T* adr(size_t a, size_t b, size_t c);
		Array3D(size_t depth, size_t height, size_t width);
		void setUp(size_t sizeZ, size_t sizeY, size_t sizeX);
		Array3D();
		~Array3D();
	};
};














template <typename T>
T* MemoryManager::Array2D<T>::adr(size_t a, size_t b) {
	return p + index(a, b);
}

template <typename T>
int MemoryManager::Array2D<T>::index(size_t a, size_t b) {
	if (a >= height or b >= width) {
		assert(false);
		std::cout << "ERROR IN HEAP 2D INT ARRAY\n";
	}
	return a * width + b;
}

template <typename T>
MemoryManager::Array2D<T>::Array2D(size_t height, size_t width) : height(height), width(width) {
	p = new T[height * width];
}

template<typename T>
void MemoryManager::Array2D<T>::setUp(size_t sizeY, size_t sizeX) {
	if (p != nullptr) {
		assert(false);
		std::cout << "ERROR WHILE SETING UP Int2D\n";
	}
	height = sizeY;
	width = sizeX;
	p = new T[height * width];
}

template<typename T>
MemoryManager::Array2D<T>::Array2D(): p(nullptr), height(0), width(0) {
	
}

template<typename T>
MemoryManager::Array2D<T>::~Array2D() {
	delete[] p;
}








template<typename T>
T* MemoryManager::Array3D<T>::adr(size_t a, size_t b, size_t c) {
	return p + index(a, b, c);
}

template<typename T>
int MemoryManager::Array3D<T>::index(size_t a, size_t b, size_t c) {
	if (a >= depth or b >= height or c >= width) {
		assert(false);
		std::cout << "ERROR IN HEAP 2D INT ARRAY\n";
	}
	return a * width * height + b * width + c;
}

template<typename T>
MemoryManager::Array3D<T>::Array3D(size_t depth, size_t height, size_t width) : depth(depth), height(height), width(width) {
	p = new T[depth * height * width];
}

template<typename T>
void MemoryManager::Array3D<T>::setUp(size_t sizeZ, size_t sizeY, size_t sizeX) {
	if (p != nullptr) {
		assert(false);
		std::cout << "ERROR WHILE SETING UP Array3D\n";
	}
	depth = sizeZ;
	height = sizeY;
	width = sizeX;
	p = new T[depth * height * width];
}

template<typename T>
MemoryManager::Array3D<T>::Array3D(): p(nullptr), depth(0), height(0), width(0) {
}

template<typename T>
MemoryManager::Array3D<T>::~Array3D() {
	delete[] p;
}
*/