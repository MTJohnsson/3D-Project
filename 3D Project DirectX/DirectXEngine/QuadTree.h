//#pragma once
//struct Rectangle {
//	float x, y;
//	float halfWidth;
//	float halflength;
//};
//template<typename  T>
//class QuadTree
//{
//private:
//	const int Capacity = 2;
//	int nrOfElements = 0;
//
//	bool subdivided = false;
//
//	QuadTree *NW = nullptr;
//	QuadTree *NE = nullptr;
//	QuadTree *SW = nullptr;
//	QuadTree *SE = nullptr;
//	T *data[Capacity];
//	//Rectangle *boundaries = nullptr;
//public:
//	QuadTree(){}
//	~QuadTree(){}
//
//	bool CreateQuadTree(std::vector<T> box);
//private:
//	//bool insert(T data, T currentPos);
//	bool subdivide(T currentPos, T* data);
//
//	bool exist(T)
//};
//
//template<typename T>
//bool QuadTree<T>::CreateQuadTree(std::vector<T> box)
//{
//	//check if its included
//
//	//check if capacity < 2 && !subdivided , then its inside this tree
//
//	//if its 0 check nw,ne,sw,se for to append the data
//
//
//
//	return false;
//}
//
//template<typename T>
//bool QuadTree<T>::subdivide(T currentPos, T * data)
//{
//	if (subdivide) {
//		return true;
//	}
//	this->subdivided = true;
//
//
//
//	return subdivided;
//}
