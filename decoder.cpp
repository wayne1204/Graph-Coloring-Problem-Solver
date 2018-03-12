/****************************************************************************
  FileName     [ decoder.cpp ]
  PackageName  [ Graph ]
  Synopsis     [ translate the result from minisat ]
  Author       [ Ping-Wei Hunag ]
    106-1  EDA project
****************************************************************************/
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
using namespace std;

size_t
myStrGetTok(const string& str, string& tok, size_t pos = 0,
            const char del = ' ')
{
   size_t begin = str.find_first_not_of(del, pos);
   if (begin == string::npos) { tok = ""; return begin; }
   size_t end = str.find_first_of(del, begin);
   tok = str.substr(begin, end - begin);
   return end;
}
bool myStr2Int(const string& str, int& num);

int main(int argc, char**argv){
	if(argc  != 3){
		cerr<<"[ERROR] Usage: ./decoder <CNF files> <result files> > "<<endl;
		return -1;
	}
	// initilaize node& color
	ifstream ifs,ifs2;
	string str,token;
	size_t pos = 0;
	int node, color = 0;

	ifs.open(argv[1],ios::in);
	if(!ifs.is_open()){
		cerr<<"can't open file "<<argv[1]<<endl;
		return -1;
	}
	getline(ifs,str);
	for(int i =0;i<3;++i)
		pos = myStrGetTok(str,token,pos);
	myStr2Int(token,node);
	getline(ifs,str);
	pos =0;
	while(  myStrGetTok(str,token,pos) != string::npos){
		pos =  myStrGetTok(str,token,pos);
		color ++;
	}
	node /= color;
	vector<int> storing;
	ifs2.open(argv[2],ios::in);
	if(!ifs2.is_open()){
		cerr<<"can't open file "<<argv[2]<<endl;
		return -1;
	}
	getline(ifs2,str);
	if(str == "UNSAT")	{
		cout<<"result is unsatisfiable\n";
		return -1;
	}
	getline(ifs2,str);
	int num;
	pos = 0;
	while(  myStrGetTok(str,token,pos) != string::npos){
		pos =  myStrGetTok(str,token,pos);
		myStr2Int(token,num);
		storing.push_back(num);
	}
	for(int i=0; i<node; ++i){
		cout<<"node "<<i;
		int color_num = 0;
		for(int j=0;j<color;++j){
			if(storing[i*color+j] < 0){
				color_num += pow(2, j);
			}
		}
		cout<<": color #"<<color_num<<endl;
	}
}





bool
myStr2Int(const string& str, int& num)
{
   num = 0;
   size_t i = 0;
   int sign = 1;
   if (str[0] == '-') { sign = -1; i = 1; }
   bool valid = false;
   for (; i < str.size(); ++i) {
      if (isdigit(str[i])) {
         num *= 10;
         num += int(str[i] - '0');
         valid = true;
      }
      else return false;
   }
   num *= sign;
   return valid;
}
