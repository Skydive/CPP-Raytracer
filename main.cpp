
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

#include <functional>
#include <vector>

#include <glm/glm.hpp>

using namespace std;
using namespace glm;
#define IMAGE_WIDTH 1366
#define IMAGE_HEIGHT 786
const dvec2 IMAGE_RESOLUTION = dvec2(IMAGE_WIDTH, IMAGE_HEIGHT);

// char* -> [255, 255, 255]
using uchar_t=unsigned char;
using FragFunc=function<void(uchar_t*, dvec2)>;
void WritePixMap(string name, int width, int height, FragFunc func)
{
	ofstream PPMFile(name, std::ios::out);
	PPMFile << "P3\n"
		<< IMAGE_WIDTH << "\n"
		<< IMAGE_HEIGHT << "\n"
		<< "255\n";
	for(int i=0; i<IMAGE_HEIGHT; i++)
	{
		for(int j=0; j<IMAGE_WIDTH; j++)
		{
			int index = i*(3*IMAGE_WIDTH) + j*3;
			uchar_t color[3];
			func(color, dvec2(j, i));
			for(int k=0; k<3; k++)
			{
				PPMFile << (int)color[k] << " ";
			}
			PPMFile << "\t";
		}
		PPMFile << "\n";
	}
	PPMFile.close();
}

float depth = 1.0;
dvec3 o;
dvec3 light = dvec3(8.0, 8.0, 8.0);

void DrawSphere(uchar_t* fragColor, dvec2 fragCoord, dvec3 ce, double R)
{
	dvec2 fc = (fragCoord - IMAGE_RESOLUTION / 2.0) / IMAGE_RESOLUTION.x; fc.y = -fc.y;
	dvec3 s = dvec3(fc, depth);
	dvec3 d = normalize(s - o);
	dvec3 v = o - ce;

	double a = dot(d, d);
	double b = 2.0*dot(v, d);
	double c = dot(v, v) - pow(R, 2.0);

	double disc = b*b - 4.0*a*c;
	if(disc > 0)
	{
		double t1 = (-b + sqrt(disc)) / 2*a;
		double t2 = (-b - sqrt(disc)) / 2*a;

		dvec3 p = o + t2*d;
		dvec3 n = normalize(p - ce);
		dvec3 l = normalize(light - p);
		dvec3 v = normalize(p - o);

		double I = glm::max(dot(n, l), 0.0);

		dvec3 vhn = reflect(l, n);
		I += pow(glm::max(dot(v, vhn), 0.0), 64.0);

		uchar_t c = floor(255*glm::min(I, 1.0));
		fragColor[0] = c;
		fragColor[1] = c;
		fragColor[2] = c;
	}
}

void DrawPlane(uchar_t* fragColor, dvec2 fragCoord, dvec3 n, dvec3 ce, dvec2 dim)
{
	dvec2 fc = (fragCoord - IMAGE_RESOLUTION / 2.0) / IMAGE_RESOLUTION.x; fc.y = -fc.y;
	dvec3 s = dvec3(fc, depth);
	dvec3 d = normalize(s - o);
	dvec3 v = o - ce;

	double t = -dot(v, n)/dot(d, n);
	dvec3 p = o + t*d;
	if(t > 0
	&& p.x-ce.x > -dim.x/2.0 && p.x-ce.x < dim.x/2.0
	&& p.z-ce.z > -dim.y/2.0 && p.z-ce.z < dim.y/2.0)
	{
		dvec3 p = o + t*d;
		dvec3 l = normalize(light - p);
		v = normalize(p - o);

		double I = glm::abs(dot(n, l));

		dvec3 vhn = reflect(l, n);
		I += pow(glm::max(dot(v, vhn), 0.0), 64.0);

		uchar_t c = floor(255*glm::min(I, 1.0));
		fragColor[0] = c;
		fragColor[1] = c;
		fragColor[2] = c;
	}
}

void ApplyFragShader(uchar_t* fragColor, dvec2 fragCoord)
{
	DrawPlane(fragColor, fragCoord, dvec3(0.0, 1.0, 0.0), dvec3(0.0, -2.0, 10.0), dvec2(5.0, 5.0));
	//DrawSphere(fragColor, fragCoord, dvec3(0.0, 0.0, 10.0), 1.0);
	DrawSphere(fragColor, fragCoord, dvec3(0.0, 0.0, 10.0), 0.5);
}

int main(int argc, char** argv)
{
	WritePixMap("out.ppm", IMAGE_WIDTH, IMAGE_HEIGHT, [&](uchar_t* fragColor, dvec2 fragCoord)
	{
		fragColor[0] = 0;
		fragColor[1] = 0;
		fragColor[2] = 0;
		ApplyFragShader(fragColor, fragCoord);
	});
	return 0;
}
