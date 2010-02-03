#define private public
#define protected public

#include "Vector3D.h"

#include <assert.h>
#include <iostream>

int main(void) {
	Vector3D a,b,c,d;
	a = Vector3D(1,2,3);
	b = Vector3D(5,4,6,7,8,9);
	
	// constructor tests
	std::cout << "Testing constructors" << std::endl;
	assert(a.x == 1);
	assert(a.y == 2);
	assert(a.z == 3);

	assert(b.x == 5);
	assert(b.y == 4);
	assert(b.z == 6);
	assert(b.xPos == 7);
	assert(b.yPos == 8);
	assert(b.zPos == 9);

	// Testing ==
	a = Vector3D(1,2,3);
	b = Vector3D(1,2,3);
	assert(a == a);
	assert(a == b);

	// += Tests
	a = Vector3D(1,2,3);
	b = Vector3D(4,7,10);
	c = Vector3D(20,100,244);
	d = Vector3D(25, 109, 257);

	a += b + c;
	assert(a == d);



	// Length tests
	a = Vector3D(4,0,3);
	assert(a.length() == 5.0);

	// dotProd Tests
	std::cout << "Testing dot product" << std::endl;
	a = Vector3D(1,2,3);
	b = Vector3D(5,7,9);
	assert(a.dotProd(b) == 46);

	// perpindicular
	std::cout << "   ...Perpindicular Case" << std::endl;
	a = Vector3D(1,0,0);
	b = Vector3D(0,1,0);
	assert(a.dotProd(b) == 0);

	// parallel
	std::cout << "   ...Parallel Case" << std::endl;
	a = Vector3D(1,2,3);
	assert(a.dotProd(a) == 14);

	// crossProd Tests
	std::cout << "Testing cross product" << std::endl;
	a = Vector3D(1,2,3);
	b = Vector3D(5,7,9);
	c = a.crossProd(b);
	d = Vector3D(-3, 6, -3);
	assert(c == d);

	// perpindicular
	std::cout << "   ...Perpindicular Case" << std::endl;
	a = Vector3D(1,0,0);
	b = Vector3D(0,1,0);
	c = a.crossProd(b);
	d = Vector3D(0,0,1);
	assert(c == d);
	
	// parallel
	std::cout << "   ...Parallel Case" << std::endl;
	a = Vector3D(1,2,3);
	b = a.crossProd(a);
	c = Vector3D(0,0,0);
	assert(b == c);

}

