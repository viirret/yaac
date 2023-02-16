#ifndef YAAC_COLOR_HH
#define YAAC_COLOR_HH

struct Color 
{
	Color(int R, int G, int B, int A) : R(R), G(G), B(B), A(A) {}
	int R, G, B, A;
};

#endif
