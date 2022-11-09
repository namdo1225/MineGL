#ifndef COOR_PACK_H
#define COOR_PACK_H

struct CoorPack {
	int x;
	int y;
	int z;

	int scaleX;
	int scaleY;
	int scaleZ;

	CoorPack(int x, int y, int z, int scaleX, int scaleY, int scaleZ);
};

#endif