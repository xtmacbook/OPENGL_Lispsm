#ifndef HeightMap_H
#define HeightMap_H

#include <Types/DynamicField.h>
#include <Mathematic/Mathematic.h>
#include <Mathematic/Vector2.h>

template<class DATA>
struct HeightMap : public DynamicField<DATA> {
	HeightMap(const unsigned mapSize) {
		resize(mapSize,mapSize);
		fillWith(0);
	}
	
	template<class T, class I> void manipulate(const T& manip, const I& input) {
		if(input.size() != size()) {
			throw BaseException("HeightMap:assign different sizes");
		}
		for(unsigned i = 0; i < size(); i++) {
			operator[](i) = manip(operator[](i),input[i]);
		}
	}

	template<class T, class I> void manipulateUnchecked(const T& manip, const I& input) {
		//unchecked assignment if input is smaller than size() undefined behavior results
		for(unsigned i = 0; i < size(); i++) {
			operator[](i) = manip(operator[](i),input[i]);
		}
	}
	
	template<class T> void manipulateUnchecked(const T& manip, const char *input[]) {
		//unchecked assignment if input is smaller than size() undefined behavior results
		for(unsigned y = 0; y < sizeY(); y++) {
			for(unsigned x = 0; x < sizeX(); x++) {
				operator()(x,y) = manip(operator()(x,y),input[y][x]);
			}
		}
	}
	
	void lookUp(double samples[4], const HeightMap& input, const double x, const double y) {
		using namespace Math;
		const unsigned minX = clamp<unsigned>(floor(x),0,input.sizeX()-1);
		const unsigned minY = clamp<unsigned>(floor(y),0,input.sizeY()-1);
		const unsigned maxX = minX+1 < input.sizeX() ? minX+1 : minX;
		const unsigned maxY = minY+1 < input.sizeY() ? minY+1 : minY;
		samples[0] = input(minX,minY);
		samples[1] = input(maxX,minY);
		samples[2] = input(minX,maxY);
		samples[3] = input(maxX,maxY);
	}

	DATA bilinear(const HeightMap& input, const double x, const double y) {
		using namespace Math;

		double samples[4];
		lookUp(samples,input,x,y);
		const double factX = fract(x);
		const double factY = fract(y);

		return lerp(
			lerp(samples[0],samples[1],factX),
			lerp(samples[2],samples[3],factX),factY);
	}

	DATA biSin(const HeightMap& input, const double x, const double y) {
		using namespace Math;

		const double pi = Const<double>::pi();
		const double pi_2 = Const<double>::pi_2();

		double samples[4];
		lookUp(samples,input,x,y);
		const double factX = sin(fract(x)*pi-pi_2)*0.5+0.5;
		const double factY = sin(fract(y)*pi-pi_2)*0.5+0.5;

		return lerp(
			lerp(samples[0],samples[1],factX),
			lerp(samples[2],samples[3],factX),factY);
	}

	template<class C> void stretchManipulate(const C& manip, const HeightMap& input) {
		const double deltaX = double(input.sizeX())/sizeX();
		const double deltaY = double(input.sizeY())/sizeY();
		double fY(0.0);
		for(unsigned y = 0; y < sizeY(); y++, fY += deltaY) {
			double fX(0.0);
			for(unsigned x = 0; x < sizeX(); x++, fX += deltaX) {
				const DATA d = biSin(input,fX,fY);
				operator()(x,y) = manip(operator()(x,y),d);
			}
		}
	}

};


#endif