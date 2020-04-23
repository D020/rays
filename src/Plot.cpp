#include "Plot.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits>
#include <algorithm>
#include <math.h>
Plot::Plot(){
	width  = 0;
	height = 0;
	buf    = 0;
}

Plot::Plot(int width, int height){
	this->width  = width;
	this->height = height;
	buf = (float*) calloc(width*height*3,sizeof(float));
}

int Plot::getWidth(){
	return width;
}

int Plot::getHeight(){
	return height;
}

void Plot::plot(int x, int y, float r, float g, float b){
	bool insideWidth  = (0<=x && x<width);
	bool insideHeight = (0<=y && y<height);
	if(insideWidth && insideHeight){
		unsigned int index = (y*width + x)*3;
		buf[index+0] = r;
		buf[index+1] = g;
		buf[index+2] = b;
	}
	else
		printf("Invalid plot coordinates\n");
}

float clamp(float val, float a, float b){
	if(val<a)
		val = a;
	else if(b<val)
		val = b;
	return val;
}

void Plot::save(const char* path){

	FILE *fp = fopen(path,"wb");
	fprintf(fp,"P6\n%d %d\n255\n",width,height);

	float hdr_min = buf[0];		//
	float hdr_max = buf[0]; //Might as well take first element as max.

	for (int i=0; i<height; i++){
		for (int j=0; j<width; j++){
			unsigned int index = (i*width + j)*3;
			hdr_min = fminf(hdr_min,buf[index]);
			hdr_min = fminf(hdr_min,buf[index+1]);
			hdr_min = fminf(hdr_min,buf[index+2]);

			hdr_max = fmaxf(hdr_max,buf[index]);
			hdr_max = fmaxf(hdr_max,buf[index+1]);
			hdr_max = fmaxf(hdr_max,buf[index+2]);
		}
	}

	printf("hdr_min %f\n",hdr_min);
	printf("hdr_max %f\n",hdr_max);

	//float scalar = (255/(hdr_max-hdr_min));

	float a = 0;
	float b = 8.0;

	for (int i=0; i<height; i++){
		for (int j=0; j<width; j++){
			static unsigned char color[3];
			unsigned int index = (i*width + j)*3;
			color[0] = clamp(buf[index  ],a,b)/(b+0.01)*255;
			color[1] = clamp(buf[index+1],a,b)/(b+0.01)*255;
			color[2] = clamp(buf[index+2],a,b)/(b+0.01)*255;
			fwrite(color,1,3,fp);
		}
	}

	fclose(fp);

}
