class Plot {
  public:
	Plot();
	Plot(int width,int height);
	
	void save(const char* path);
	void plot(int x, int y, int r, int g, int b);

	int width, height;
	unsigned char* buf;
};
