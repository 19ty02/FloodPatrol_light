#include<cstdio>
#include<iostream>
using namespace std;
double d=0.1;
int a[4]={0,11,11,13};
int b[4]={0,6,6,5};
double c[4]={0,175.6,194.9,210.5};
// x=t, y=T, z=1/v
struct node{
	double a,b;  // z >= ax+by
	int sgn;     // 1: >= | -1: <=
	void prt(){
		printf("u");
		if(sgn==1)printf(" >= ");
		else printf(" <= ");
		printf("%.5lft+%.5lfT",a,b);
	}
}; 

void work(){
	node ans;
	if(a[1]!=a[2]){
		if(c[1]<c[2])ans.sgn=1;
		else ans.sgn=-1;
		ans.a=(a[1]-a[2])/(c[2]-c[1]);
		ans.b=(b[1]-b[2])/(c[2]-c[1]);
		ans.prt();
		printf(" and ");
	}
	
	if(a[2]!=a[3]){
		if(c[2]<c[3])ans.sgn=1;
		else ans.sgn=-1;
		ans.a=(a[2]-a[3])/(c[3]-c[2]);
		ans.b=(b[2]-b[3])/(c[3]-c[2]);
		ans.prt();
		printf(" and ");
	}
	
	if(d*c[3]-c[3]+c[1]>0)ans.sgn=1;
	else ans.sgn=-1;
	ans.a=(a[3]-d*a[3]-a[1])/(d*c[3]-c[3]+c[1]);
	ans.b=(b[3]-d*b[3]-b[1])/(d*c[3]-c[3]+c[1]);
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
