#include<cstdio>
#include<iostream>
using namespace std;
const int d=10;  // ²»¾ùºâ¶È 1/d 
int a[4]={0,11,11,13};
int b[4]={0,6,6,5};
int c[4]={0,1756,1949,2105};
// x=t, y=T, z=1/v
struct node{
	int c,a,b;  // cz >= ax + by
	int sgn;     // 1: >= | -1: <=
	void trans_sgn(){
		if(c<0){
			c=-c;
			a=-a;
			b=-b;
			sgn=-sgn;
		}
	}
	void prt(){
		trans_sgn();
		printf("%du",c);
		if(sgn==1)printf(" >= ");
		else printf(" <= ");
		printf("%dt + %dT",a,b);
	}
}; 

void work(){
	node ans;
	if(a[2]!=a[3]){
		ans.sgn=1;
		ans.c=c[3]-c[2];
		ans.a=10*(a[2]-a[3]);
		ans.b=10*(b[2]-b[3]);
		ans.prt();
		printf(" and ");
	}
	
	if(a[1]!=a[2]){
		ans.sgn=1;
		ans.c=c[2]-c[1];
		ans.a=10*(a[1]-a[2]);
		ans.b=10*(b[1]-b[2]);
		ans.prt();
		printf(" and ");
	}
	ans.sgn=1;
	ans.c=c[3]-d*c[3]+d*c[1];
	ans.a=10*(d*a[3]-a[3]-d*a[1]);
	ans.b=10*(d*b[3]-b[3]-d*b[1]);
	ans.prt();
}
int main(){
	freopen("solution4.txt","w",stdout); 
	work();
	printf("\nor\n");
	swap(a[2],a[3]);
	swap(b[2],b[3]);
	swap(c[2],c[3]);
	work();
	printf("\nor\n");
	swap(a[2],a[1]);
	swap(b[2],b[1]);
	swap(c[2],c[1]);
	work();
	printf("\n\nwhere u = 1/v\n");
}
