/*	��Ŀ���ƣ�		����N������
	��ⷶΧ��		1~10,000,000,000 
	������			������
	ѧ�ţ�			PB14203189
	ʱ�䣺			2016.10 
*/
#include<iostream>
#include<cmath>
#include<string>
#include<sstream>
#include<ctime>
#define MAX 1000000000000			//��(x)��x��������ֵΪ1����
#define MAXSIEVE 1000010			//ӦԤ��������������Χ
#define MAXPRIME 100000				//���Χ������������
#define MAXN 1000					//׼����ά�������N
#define MAXM 200000					//׼����ά�������M
int prime[MAXPRIME], num[MAXSIEVE] = {0};//ǰ�ߴ������������߱�ʾ�±귶Χ����������
long long tempres[MAXN][MAXM];		//׼������
bool isNotPrime[MAXSIEVE] = { 1,1 };//������־����
double ratio[7] = {12.74, 15.047, 17.357, 20.667, 22.975, 25.283, 26.59 };//�����ܶȴ��·ֲ�������΢����ϵ��
long long my_prime[6] = { 2, 3, 5, 233, 331 };//�����ޱ��㷨������ֵ
long len = 0;						//��������
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
void EulerSieve()//ŷ��ɸ��
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
void my_pre()//׼������
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
long long fai(long long m, long long n)//�������ļ���
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
long long primeNum(long long m)//�У�x���ļ���
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
int get_len(long long numth)//�ж���������λ��
{
	int length = 0;
	while (numth)
	{
		numth /= 10;
		length++;
	}
	return length;
}
long long findPrime(long long numth)//�ҵ���numth������
{
	if (numth <= len)
	{
		return prime[numth - 1];
	}
	long long tempnum=0,errnum=5,lasterror;
	int num_lenth = get_len(numth);
	tempnum = ratio[num_lenth - 6] * numth;
	//cout << num_lenth << endl;
	while (abs(errnum)>3)								//����΢������numth
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
		
	/*while (true)							//�����ޱ��㷨
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
		cout <<"��"<<i<<"��������"<< findPrime(i) << endl;
		time_t end=clock();
		cout << "��ʱ��" << end - start << "ms" << endl;

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
