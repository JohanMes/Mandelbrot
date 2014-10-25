#include <complex>
#include <windows.h>
#include <vector>
#include <stdio.h>
#include <omp.h>
using std::complex;
using std::vector;

#include "brot.h"

int Brot::Check(complex<float> c,int maxiterations) {
	vector<complex<float> > points;
	points.resize(maxiterations);
	for(int i = 0;i < maxiterations - 1;i++) {
		points[i+1] = points[i] * points[i] + c;
		if(abs(points[i+1]) > 1e10) {
			return i;
		}
	}
	return 0;
}

void Brot::Fill() {
	
	// Check exact iedere pixel
	points.resize(width);
	for(int i = 0;i < points.size();i++) {
		points[i].resize(height);
	}
	
	// Voor iedere pixel even berekenen
	#pragma omp parallel for num_threads(16)
	for(int x = 0;x < width;x++) {
		for(int y = 0;y < height;y++) {
			
			// scale pixel to map coord
			float xbrot = left + (float)x/(float)width * (right - left);
			float ybrot = top + (float)y/(float)height * (bottom - top);
		
			points[x][y] = Check(complex<float>(xbrot,ybrot),100);
		}
	}
}

void Brot::SetSize(int width,int height) {
	this->width = width;
	this->height = height;
	
	xpixel = (right - left) / width;
	ypixel = (bottom - top) / height;
}

void Brot::SetViewport(float left,float top,float right,float bottom) {
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	
	xpixel = (right - left) / width;
	ypixel = (bottom - top) / height;
}
