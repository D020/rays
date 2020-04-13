#include "Plot.h"
#include <stdio.h>
#include <stdlib.h>
Plot::Plot(){
	width  = 0;
	height = 0;
	buf    = 0;
}

Plot::Plot(int width, int height){
	this->width  = width;
	this->height = height;
	buf = (unsigned char*) calloc(width*height*3,sizeof(unsigned char));
}

void Plot::plot(int x, int y, int r, int g, int b){
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

void Plot::save(const char* path){

	FILE *fp = fopen(path,"wb");
	fprintf(fp,"P6\n%d %d\n255\n",width,height);
	
	for (int i=0; i<height; i++){
		for (int j=0; j<width; j++){
			static unsigned char color[3];
			unsigned int index = (i*width + j)*3;
			color[0] = buf[index];
			color[1] = buf[index+1];
			color[2] = buf[index+2];
			fwrite(color,1,3,fp);
		}
	}

	fclose(fp);

}
