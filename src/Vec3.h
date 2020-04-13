class Vec3 {
  public:
	Vec3();
	Vec3(float x,
		 float y,
		 float z);

	float length();
	Vec3 norm();

	float x,y,z;
};
