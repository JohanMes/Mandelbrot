#ifndef BROT_H
#define BROT_H

#include <complex>
#include <vector>
using std::complex;
using std::vector;

class Brot {
	public:		
		vector<vector<int> > points;
		int height; // pixels
		int width;
		float left;
		float top;
		float right;
		float bottom;
		float xpixel; // map coords per pixel
		float ypixel;

		int Check(complex<float> c,int maxiterations);
		void Fill();
		void SetSize(int width,int height);
		void SetViewport(float left,float top,float right,float bottom);
};

#endif
