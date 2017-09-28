#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cmath>

using namespace std;

static int PPMWriteImage(const string &filePath, const unsigned char *buffer,
		const unsigned width, const unsigned height);

inline void drawPoint(const unsigned &width, const unsigned &height,
		 const unsigned &cx, const unsigned &cy,
		 const unsigned &x, const unsigned &y,
		 unsigned char * &buffer,
		 const unsigned char &R,
		 const unsigned char &G,
		 const unsigned char &B)
{
	if(cx + x > width || cy + y > height)
		return;
	auto p = buffer + ((cy + y) * width + cx + x)*3;
	*p++ = R;
	*p++ = G;
	*p++ = B;

}

inline void plot(const unsigned &width, const unsigned &height,
		 const unsigned &cx, const unsigned &cy,
		 const unsigned &x, const unsigned &y,
		 unsigned char * &buffer)
{
	drawPoint(width, height, cx, cy, x, y, buffer, 0, 0, 0);
	drawPoint(width, height, cx, cy, -x, y, buffer, 0, 0, 0);
	drawPoint(width, height, cx, cy, x, -y, buffer, 0, 0, 0);
	drawPoint(width, height, cx, cy, -x, -y, buffer, 0, 0, 0);
}

static int drawEllipse(const unsigned cx, const unsigned cy,
		const unsigned a, const unsigned b)
{
	const unsigned width = cx + a;
	const unsigned height = cy + b;
	auto *image = new unsigned char[width*height*3u];
	memset(image, 255, width * height * 3u);
	unsigned asqr = a*a, bsqr = b*b;
	unsigned int x=a, y = 0u;
	plot(width, height, cx, cy, x, y, image);
	int x0 = asqr/sqrt(asqr+bsqr);
	int P = bsqr*((a<<1)-1)*((a<<1)-1) - ((asqr*(bsqr-1))<<2);
	while(x>=x0)
	{
		if(P>0)
		{
			P += bsqr*(8-(x<<3));
			--x;
		}
		P += asqr*(12+(y<<3));
		++y;
		plot(width, height, cx, cy, x, y, image);
	}
	x=0,y=b;
	P = asqr*((b<<1)-1)*((b<<1)-1)-((bsqr*(asqr-1))<<2);
	while(x<x0)
	{
		if(P>0)
		{
			P += asqr*(8-(y<<3));
			--y;
		}
		P += bsqr * ( 12 + (x<<3) );
		++x;
		plot(width, height, cx, cy, x, y, image);
	}
	auto result = PPMWriteImage("ellipse.ppm", image, width, height);
	delete [] image;
	return result;
}

int main(int argc, char *argv[])
{
	ios::sync_with_stdio(false);
	unsigned cx, cy, Ra, Rb;
	cout<<"Input the ellipse's cx, cy, Ra, and Rb, which should be seperated by spaces:"<<endl;
	cin>>cx>>cy>>Ra>>Rb;
	drawEllipse(cx, cy, Ra, Rb);
	return 0;
}

static int PPMWriteImage(const string &filePath, const unsigned char *buffer,
		const unsigned width, const unsigned height)
{
	auto size = width * height * 3;
	ofstream file(filePath, ios::out | ios::binary | ios::trunc);
	if(!file)
	{
		std::cerr<<"Open target image file "<<filePath<<" failed!"<<endl;
		return -1;
	}
	file << "P3 " << width << " " << height << " 255" << endl;

	while(size-- > 0)
		file<<" "<<(unsigned)(*buffer++);

	file.close();

	return 0;
}
