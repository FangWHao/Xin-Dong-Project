#include<iostream>
#include<algorithm>
#include<cstring>
#include<cstdio>
#include<math.h>
#include<fstream>
using namespace std;
int map[105][105],map1[105][105],c[105];
int thm=100,size=100,pos=10;
int direction=1; // -1 left 1 right
int st=40,ed=100; //取样范围，判断角度
struct nod{
	int num;
	nod *l,*r;
	nod(){
		l=NULL;
		r=NULL;
	}
	nod(int x,nod *ll,nod *rr){
		num=x;
		l=ll;
		r=rr;
	}
};
nod *head_max=NULL,*tail_max=NULL;
nod *head_min=NULL,*tail_min=NULL;
int max_size=0,min_size=0;
int offset_min,offset_max;
void add_nod(nod *dst,nod *x){ //add after
	if(dst==NULL)return;
	x->l=dst;
	x->r=dst->r;
	if(dst->r!=NULL)
		(dst->r)->l=x;
	dst->r=x;
}
void add_min(int a){
	nod* x=new nod(a,NULL,NULL);
	if(min_size>=size){
		if(tail_min->num<=x->num)return;
		nod* tmp=tail_min;
		tail_min=tail_min->r;
		tail_min->l=NULL;
		delete tmp;
		min_size--;
	}
	min_size++;
	nod* pos=tail_min;
	while(pos!=NULL&&pos->r!=NULL){
		if(pos->num>x->num){
			pos=pos->r;
		}
		else break;
	}
	if(pos==head_min){
		add_nod(head_min,x);
		head_min=x;
	}
	else if(pos!=NULL){
		if(pos->l!=NULL)pos=pos->l;
		add_nod(pos,x);
	}
	if(min_size==1)tail_min=head_min;
}
void add_max(int a){
	nod* x=new nod(a,NULL,NULL);
	if(max_size>=size){
		if(tail_max->num>=x->num)return;
		nod* tmp=tail_max;
		tail_max=tail_max->r;
		tail_max->l=NULL;
		delete tmp;
		max_size--;
	}
	max_size++;
	nod* pos=tail_max;
	while(pos!=NULL&&pos->r!=NULL){
		if(pos->num<x->num){
			pos=pos->r;
		}
		else break;
	}
	if(pos==head_max){
		add_nod(head_max,x);
		head_max=x;
	}
	else if(pos!=NULL){
		if(pos->l!=NULL)pos=pos->l;
		add_nod(pos,x);
	}
	if(max_size==1)tail_max=head_max;
}
void exposure_adjust(){
	max_size=0,min_size=0;
	for(int i=1;i<=100;i++){
		for(int j=1;j<=100;j++){
			add_min(map[i][j]);
			add_max(map[i][j]);
		}
	}
	nod* pos;
	pos=tail_max;
	while(1){
		offset_max+=pos->num;
		pos=pos->r;
		if(pos==NULL)break;
	}
	pos=tail_min;
	while(1){
		offset_min+=pos->num;
		pos=pos->r;
		if(pos==NULL)break;
	}
	offset_max/=size;
	offset_min/=size;
	int offset=offset_max-offset_min;
	for(int i=1;i<=100;i++){
		for(int j=1;j<=100;j++){
			if(map[i][j]>offset_max)
				map[i][j]=255;
			else if(map[i][j]<offset_min)
				map[i][j]=0;
			else {
				map[i][j]=1.0*(map[i][j]-offset_min)/offset*255;
			}
		}
	}
}
int ostu(){
	int ans=0;
	double max=0;
	for(int t=1;t<=255;t++){
		int fp=0,bp=0;	//front/background pixels
		int ftot=0,btot=0;	//front/background pixels
		for(int i=1;i<=100;i++){
			for(int j=1;j<=100;j++){
				if(map[i][j]>t){
					fp++;
					ftot+=map[i][j];
				}
				else {
					bp++;
					btot+=map[i][j];
				}
			}
		}
		double w0=fp/10000.0;
		double w1=bp/10000.0;
		double u0=1.0*ftot/fp;
		double u1=1.0*btot/bp;
		double now=w0*w1*(u0-u1)*(u0-u1);
		if(now>max){
			max=now;
			ans=t;
		}
	}
	return ans;
}
void ave_filter(){
	for(int i=1;i<=100;i++){
		for(int j=1;j<=100;j++){
			map1[i][j]=map[i][j];
		}
	}
	for(int i=2;i<=99;i++){
		for(int j=2;j<=99;j++){
			int tmp=0;
			for(int x=-1;x<=1;x++){
				for(int y=-1;y<=1;y++){
					tmp+=map1[i+x][j+y];
				}
			}
			map[i][j]=tmp/9;
		}
	}
	for(int i=2;i<=99;i++){
		map[1][i]=map[2][i];
		map[100][i]=map[99][i];
		map[i][1]=map[i][2];
		map[i][100]=map[i][99];
	}
	map[1][1]=map[2][2];
	map[1][100]=map[2][99];
	map[100][100]=map[99][99];
	map[100][1]=map[99][2];
}
int a[10];
void mid_filter(){
	for(int i=1;i<=100;i++){
		for(int j=1;j<=100;j++){
			map1[i][j]=map[i][j];
		}
	}
	for(int i=2;i<=99;i++){
		for(int j=2;j<=99;j++){
			int cnt=0;
			for(int x=-1;x<=1;x++){
				for(int y=-1;y<=1;y++){
					a[cnt++]=map1[i+x][j+y];
				}
			}
			sort(a,a+9);
			map[i][j]=a[5];
		}
	}
	for(int i=2;i<=99;i++){
		map[1][i]=map[2][i];
		map[100][i]=map[99][i];
		map[i][1]=map[i][2];
		map[i][100]=map[i][99];
	}
	map[1][1]=map[2][2];
	map[1][100]=map[2][99];
	map[100][100]=map[99][99];
	map[100][1]=map[99][2];
}
void get_center(){
	for(int i=st;i<=ed;i++){
		int x=50;
		while(x>1&&x<101&&map[i][x]==0){
			cout<<i<<endl;
			x+=direction; //防止弯度过大，找到一个空点
		}
		int l=x,r=x;
		while(l>0&&map[i][l]==255)l--;
		while(r<101&&map[i][r]==255)r++;
		c[i]=(l+r)>>1;
	}
}
int cc[105];
double lsm(){ //最小二乘求斜率,负代表向左走
	int n=ed-st+1;
	double xx=n/2.0-1,yy=0;
	for(int i=st;i<=ed;i++){
		cc[100-i]=c[i]-50;
		yy+=cc[100-i];
	}
	yy/=n;
	cout<<xx<<' '<<yy<<endl;
	double u=0,d=0;
	for(int i=0;i<n;i++){
		u+=(i-xx)*(cc[i]-yy);
		d+=(i-xx)*(i-xx);
	}
	return u/d;
}
int main(){
	ifstream fin("in.txt");
	for(int i=1;i<=100;i++){
		for(int j=1;j<=100;j++){
			fin>>map[i][j];
		}
	}
	fin.close();
	/********image prossess********/
	//exposure_adjust();
	ave_filter();
	//mid_filter();
	thm=ostu();
	cout<<thm<<endl;
	ofstream fout("out.txt");
	for(int i=1;i<=100;i++){
		for(int j=1;j<=100;j++){
			if(map[i][j]>thm)map[i][j]=255;
			else map[i][j]=0;
		}
	}
	/*********lane detection*********/
	get_center();
	for(int i=st;i<=ed;i++){
		map[i][c[i]]=0;
	}
	double k=lsm();
	double deg=atan2(k,1)/3.1415*180;
	cout<<k<<' '<<deg<<endl;
	for(int i=1;i<=100;i++){
		for(int j=1;j<=100;j++){
			fout<<map[i][j]<<' ';
		}
		fout<<endl;
	}
	fout.close();
}