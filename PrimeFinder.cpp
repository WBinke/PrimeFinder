/*	项目名称：		求解第N个素数
	求解范围：		1~10,000,000,000 
	姓名：			王秉科
	学号：			PB14203189
	时间：			2016.10 
*/
#include<iostream>
#include<cmath>
#include<string>
#include<sstream>
#include<ctime>
#define MAX 1000000000000			//π(x)中x可求的最大值为1万亿
#define MAXSIEVE 1000010			//应预先求的素数的最大范围
#define MAXPRIME 100000				//最大范围内素数最大个数
#define MAXN 1000					//准备二维数组最大N
#define MAXM 200000					//准备二维数组最大M
int prime[MAXPRIME], num[MAXSIEVE] = {0};//前者储存素数，后者表示下标范围内素数个数
long long tempres[MAXN][MAXM];		//准备数组
bool isNotPrime[MAXSIEVE] = { 1,1 };//素数标志数组
double ratio[7] = {12.74, 15.047, 17.357, 20.667, 22.975, 25.283, 26.59 };//素数密度大致分布，用于微分项系数
long long my_prime[6] = { 2, 3, 5, 233, 331 };//米勒罗宾算法素数的值
long len = 0;						//素数个数
using namespace std;
long long qmul(long long x, long long y, long long mod) 
{


	return (x * y - (long long)(x / (long double)mod * y + 1e-3) *mod + mod) % mod;
}
long long  qpow(long long  a, long long n, long long mod) 
{
	long long ret = 1;
	while (n) {
		if (n & 1) ret = qmul(ret, a, mod);
		a = qmul(a, a, mod);
		n >>= 1;
	}
	return ret;
}
bool Miller_Rabin(long long p) 
{
	if (p < 2) return 0;
	if (p != 2 && p % 2 == 0) return 0;
	long long s = p - 1;
	while (!(s & 1)) s >>= 1;
	for (int i = 0; i < 5; ++i) {
		if (p == my_prime[i]) return 1;
		long long t = s, m = qpow(my_prime[i], s, p);
		while (t != p - 1 && m != 1 && m != p - 1) {
			m = qmul(m, m, p);
			t <<= 1;
		}
		if (m != p - 1 && !(t & 1)) return 0;
	}
	return 1;
}
void EulerSieve()//欧拉筛法
{
	for (long  i = 2; i < MAXSIEVE; i++)
	{	
		
		num[i] = num[i - 1];
		if (!isNotPrime[i])
		{
			prime[len++] = i;
			num[i]++;
		}
		for (long j = 0; j < len && i*prime[j]<MAXSIEVE; j++)
		{
			isNotPrime[i * prime[j]] = 1;
			if (!(i%prime[j]))
				break;
		}
	}

}
void my_pre()//准备数组
{
	for (long long n = 0; n < MAXN; n++) {
		for (long long m = 0; m < MAXM; m++) {
			if (n)
			{
				tempres[n][m] = tempres[n - 1][m] - tempres[n - 1][m / prime[n - 1]];
			}
			else
				tempres[n][m] = m;
		}
	}
}
long long fai(long long m, long long n)//函数Φ的计算
{
	if (n==0)
	{
		return m;
	}
	if (prime[n-1]>=m)
	{
		return 1;
	}
	if (m < MAXM && n < MAXN)
	{
		return tempres[n][m];
	}
	return fai(m, n - 1) - fai(m / prime[n-1], n - 1);
}
long long primeNum(long long m)//π（x）的计算
{
	if (m<MAXSIEVE)
	{
		return num[m];
	}
	long long y, n,min_lim,max_lim,result;
	
	min_lim = cbrt(m);
	max_lim = sqrt(m);
	y = min_lim;
	n = num[y];
	result = fai(m, n) + n - 1;
	for (long long i = n ; prime[i] < max_lim+1; i++)
	{
		result = result - primeNum(m / prime[i]) + primeNum(prime[i] + 1)-1;
	}
	return result;
}
int get_len(long long numth)//判断输入数的位数
{
	int length = 0;
	while (numth)
	{
		numth /= 10;
		length++;
	}
	return length;
}
long long findPrime(long long numth)//找到第numth个素数
{
	if (numth <= len)
	{
		return prime[numth - 1];
	}
	long long tempnum=0,errnum=5,lasterror;
	int num_lenth = get_len(numth);
	tempnum = ratio[num_lenth - 6] * numth;
	//cout << num_lenth << endl;
	while (abs(errnum)>3)								//比例微分趋近numth
	{
		tempnum = tempnum + errnum*ratio[num_lenth - 6];
		errnum = numth - primeNum(tempnum);
		//cout << errnum << endl
	}
	
	while (errnum!=0)									
	{	
		if (tempnum%2!=0)
		{
			tempnum = tempnum + 2 * errnum;
		}
		else
		{
			tempnum += errnum;
		}
		
		errnum = numth - primeNum(tempnum);
	}
	tempnum = tempnum - 1;
	int flag = 0;
	while (primeNum(tempnum) != numth - 1)
	{
		if (tempnum % 2 == 0)
		{
			tempnum = tempnum - 2;
		}
			else
		{
			tempnum = tempnum - 1;
		}

		}
		return tempnum + 1;
		
	/*while (true)							//米勒罗宾算法
	{
		if (Miller_Rabin(tempnum))
		{
			return tempnum;
		}
		else
		{	
			//cout<<tempnum<<endl;
			tempnum--;
		}
	}
	*/
}
int main()
{	
	EulerSieve();
	my_pre();
	long long numth=0;
	string s_numth;
	char test;
	/*
	for (long long i = 1; i < 10000000001; i=i*10)
	{	
		time_t start=clock();
		cout <<"第"<<i<<"个素数："<< findPrime(i) << endl;
		time_t end=clock();
		cout << "耗时：" << end - start << "ms" << endl;

	}
	*/
		getline(cin,s_numth);
		stringstream temnum;
		temnum << s_numth;
		if (!(temnum>>numth))
		{
			cout << "ERROR INPUT" << endl;
		}
		else if (temnum>>test)
		{
			cout << "ERROR INPUT" << endl;
		}
		else if (numth<=0||numth>10000000000)
		{
			cout << "ERROR INPUT" << endl;
		}
		else
		{
			cout << findPrime(numth) << endl;
		}
	
	return 0;
}
