#include <iostream>  
#include <sstream>  
#include <fstream>  
#include <string>
#include <cstring>
#include <iterator>  
#include <algorithm>  
#include <climits>  
#include <cmath>  
#include <cstdlib>  
  
using namespace std;  
  
const int nCities = 10;           //城市数量  
const double SPEED = 0.98;        //退火速度  
const int INITIAL_TEMP = 1000;    //初始温度  
const int L = 10 * nCities;       //Markov 链的长度  

struct unit                             //一个解  
{  
	double length;                      //代价，总长度  
	int path[nCities];                  //路径  
	bool operator < (const struct unit &other) const  
	{  
		return length < other.length;  
	}  
};  
unit bestone = {INT_MAX, {0} };         //最优解  

double length_table[nCities][nCities];  //distance  
 
class saTSP
{
	public:
		int init_dis();                  // create matrix to storage the Distance each city
		void SA_TSP();  
		void CalCulate_length(unit &p);  //计算长度  
		void print(unit &p);             //打印一个解  
		void getNewSolution(unit &p);    // 从邻域中获去一个新解  
		bool Accept(unit &bestone, unit &temp, double t);//新解以Metropolis 准则接受  
};
  
//stl 中 generate 的辅助函数对象  
class GenbyOne {  
    public:  
        GenbyOne (int _seed = -1): seed(_seed){}  
        int operator() (){return seed += 1;}  
    private:  
        int seed;  
};  
  
void saTSP::SA_TSP()  
{  
    srand((int)time(0));  
    int i = 0;  
    double r = SPEED;  
    double t = INITIAL_TEMP;  
    const double t_min = 0.001; //温度下限，若温度达到t_min ，则停止搜索  
  
    //choose an initial solution ~  
    unit temp;  
    generate(temp.path, temp.path + nCities, GenbyOne(0));  
    random_shuffle(temp.path, temp.path + nCities);  
    CalCulate_length(temp);  
    memcpy(&bestone, &temp, sizeof(temp));  

    // while the stop criterion is not yet satisfied do  
    while ( t > t_min )  
    {  
        for (i = 0; i < L; i++)   
        {  
  
            getNewSolution(temp);  
			//cout << "dkkd:" << bestone.length << endl;
            if(Accept(bestone,temp, t))  
            {  
                memcpy(&bestone, &temp, sizeof(unit));  
            }  
            else  
            {  
                memcpy(&temp, &bestone, sizeof(unit));  
            }  
        }  
        t *= r; //退火  
    }  
    return;  
}  
  
bool saTSP::Accept(unit &bestone, unit &temp, double t)  
{  
    if (bestone.length > temp.length) //获得更短的路径  
    {  
        return true;  
    }  
    else  
    {  
        if ((int)(exp((bestone.length - temp.length) / t) * 100) > (rand() % 101) )   
        {  
            return true;  
        }  
    }  
    return false;  
}  
  
void saTSP::getNewSolution(unit &p)  
{  
    int i = rand() % nCities;  
    int j = rand() % nCities;  
    if (i > j)   
    {  
        int t = i;  
        i = j;  
        j = t;  
    }  
    else if (i == j)      
    {  
        return;   
    }  
  
    int choose = rand() % 3;  
    if ( choose == 0)   
    {//交换  
        int temp = p.path[i];  
        p.path[i] = p.path[j];  
        p.path[j] = temp;  
    }  
    else if (choose == 1)   
    {//置逆  
        reverse(p.path + i, p.path + j);       
    }  
    else  
    {//移位  
        if (j + 1 == nCities) //边界处不处理  
        {  
            return;  
        }  
        rotate(p.path + i, p.path + j, p.path + j + 1);    
    }  
    CalCulate_length(p);  
}  
  
int saTSP::init_dis() // create matrix to storage the Distance each city  
{  
    int i = 0, j = 0;  
	string line;
	double word;
    ifstream infile("del2.txt");  
	if(!infile)
	{
		cout << "Cannot open the file" << endl;
		return 0;
	}
	
	while(getline(infile, line))
	{
		j = 0;
		istringstream instream(line);
		while(instream >> word)
		{
			length_table[i][j] = word;  
			++j;
		}
		++i;
	}
}  
  
void saTSP::CalCulate_length(unit &p)  
{  
    int j = 0;  
    p.length = 0;  
    for (j = 1; j < nCities; j++)   
    {  
        p.length += length_table[ p.path[j-1] ][ p.path[j] ];  
    }  
    p.length += length_table[p.path[ nCities - 1] ][ p.path[0] ];  
}  
  
void saTSP::print( unit &p)  
{  
    int i;  
    cout << "代价是：" << p.length << endl;  
    cout << "路径是：";  
    for (i = 0; i < nCities; i++)   
    {  
        cout << p.path[i] << " ";  
    }  
    cout << endl;  
}  

int main(int argc, char* argv[])  
{  
	saTSP sa;
    sa.init_dis();  
    sa.SA_TSP();  
    sa.CalCulate_length(bestone);  
    sa.print(bestone);  
    return 0;  
}  
