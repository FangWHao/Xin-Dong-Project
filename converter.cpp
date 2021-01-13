#include<iostream>
#include<cstring>
#include<algorithm>
#include<cstdio>
#include<fstream>
using namespace std;
int map[105][105];
int main(){
	//100*100
	cout<<"0: convert colored bmp to uncolored txt\n";
	cout<<"1: convert uncolored txt to bmp\n";
	bool flag=0;
	cin>>flag;
	ifstream fin("in.bmp",ios::in|ios::binary);
	if(!flag){
		char a;
		for(int i=1;i<=54;i++){
			fin.read((char*)&a,sizeof(char));
		}
		int b=0,g=0,r=0;
		//BGR
		for(int i=1;i<=100;i++){
			for(int j=1;j<=100;j++){
				fin.read((char*)&b,sizeof(char));
				fin.read((char*)&g,sizeof(char));
				fin.read((char*)&r,sizeof(char));
				int tmp=0.0+r*0.299+g*0.587+b*0.114;
				//cout<<r<<' '<<g<<' '<<b<<endl;
				map[101-i][j]=tmp;
			}
		}
		fin.close();
		ofstream fout1("in.txt");
		for(int i=1;i<=100;i++){
			for(int j=1;j<=100;j++){
				fout1<<map[i][j]<<' ';
			}
			fout1<<endl;
		}
		fout1.close();
	}
	else {
		ifstream fin1("out.txt",ios::in);
		char a;
		ofstream fout("out.bmp",ios::out|ios::binary);
		for(int i=1;i<=54;i++){
			fin.read((char*)&a,sizeof(char));
			if(flag){
				fout.write((char*)&a,sizeof(char));
			}
		}
		for(int i=1;i<=100;i++){
			for(int j=1;j<=100;j++){
				fin1>>map[i][j];
			}
		}
		for(int i=100;i>=1;i--){
			for(int j=1;j<=100;j++){
				fout.write((char*)&map[i][j],sizeof(char));
				fout.write((char*)&map[i][j],sizeof(char));
				fout.write((char*)&map[i][j],sizeof(char));
			}
		}
		fout.close();
	}
}
