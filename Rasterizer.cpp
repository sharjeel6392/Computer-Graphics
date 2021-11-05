///
//  Rasterizer.cpp
//
//  Simple module that provides rasterization algorithms
//
//  Created by Joe Geigel on 11/30/11.
//  Updates: 2019/02/09, 2019/09/14 by wrc.
//  Copyright 2019 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  YOUR_NAME_HERE
///

#include <list>
#include <algorithm>
#include <iostream>

#include "Types.h"
#include "Rasterizer.h"
#include "Canvas.h"

#define INF 99999;

using namespace std;

truct Vertex{
	int x;
	int y;
};


struct bucket{
	int yMax, yMin;
	float x;
	float invSlope;
};

///
// Constructor
//
// @param n number of scanlines
// @param C The Canvas to use
///



Rasterizer::Rasterizer( int n, Canvas &canvas ) : n_scanlines(n), C(canvas)
{
}

///
// Draw a filled polygon.
//
// Implementation should use the scan-line polygon fill algorithm
// discussed in class.
//
// The polygon has n distinct vertices.  The coordinates of the vertices
// making up the polygon are supplied in the 'v' array parameter, such
// that the ith vertex is in v[i].
//
// You are to add the implementation here using only calls to the
// addPixel() function.
//
// @param n - number of vertices
// @param v - array of vertices
///



void printVertices(int n, const Vertex v[]){
	for(int i=0;i<n;i++){
		cout<<"("<<v[i].x<<","<<v[i].y<<")";
		cout<<"\n";
	}	
}

void createEdges(int n, int x[], int y[], bucket *B){
	int i;
	double m;

	for (i=0;i<n;i++){
		if(y[i] > y[(i+1)%n]){
			B[i].yMax = y[i];
			B[i].yMin = y[(i+1)%n];
			B[i].x = x[(i+1)%n];
			B[i].invSlope = static_cast<float>(x[(i+1)%n]-x[i])/(y[(i+1)%n]-y[i]);
		}
		else if(y[i] < y[(i+1)%n]){
			B[i].yMax = y[(i+1)%n];
			B[i].yMin = y[i];
			B[i].x = x[i];
			B[i].invSlope = static_cast<float>(x[(i+1)%n]-x[i])/(y[(i+1)%n]-y[i]);
		}
		else
			B[i].invSlope = INF;
			
	}
}

void sort(bucket *B, int n){
	int i,j;
	bucket key;
	for(i=1;i<n;i++){
		key = B[i];
		j = i-1;
		while(j>=0 && B[j].yMax > key.yMax || B[j].yMin > key.yMin || B[j].x > key.x){
			B[j+1] = B[j];
			j --;
		}
		B[j+1] = key;
	}
}

void drawPolygon(bucket *B, int n){
	
}

void Rasterizer::drawPolygon( int n, const Vertex v[] )
{
    bucket edgeTable[n];

	int x[n],y[n];
	for(int i=0;i<n;i++){
		x[i] = v[i].x;
		y[i] = v[i].y;
	}

	createEdges(n,x,y, edgeTable);
	sort(edgeTable, n);
	cout<<"Drawing"<<endl;
	int i;
	int parity = 0; // 0: even; 1: odd
	int scanLine = edgeTable[1].yMin;
	int count = 0;
	for(i=1;i<n;i++){
		if(edgeTable[i].yMin == scanLine){
			count++;
		}
	}
	bucket *AEL = nullptr;

	AEL = new bucket[count];
	int k = 0;
	for(i=1;i<n;i++){
		if(edgeTable[i].yMin == scanLine){
			AEL[k].yMin = edgeTable[i].yMin;
			AEL[k].yMax = edgeTable[i].yMax;
			AEL[k].x = edgeTable[i].x + edgeTable[i].invSlope;
			AEL[k].invSlope = edgeTable[i].invSlope;
			k++;
		}
	}	

	sort(AEL, count);
	for(int i=0;i<count;i++)
		cout<<AEL[i].yMin<<","<<AEL[i].yMax<<","<<AEL[i].x<<","<<AEL[i].invSlope<<"\n";


	Vertex p = {AEL[0].x,AEl[0].yMin}
	addPixel(p);

	delete[] AEL;
}
