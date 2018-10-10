//#ifndef ONLINE_JUDGE
#pragma comment (linker, "/STACK:9977721915")
//#endif

#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include <sys/timeb.h>
#include <ctime>
#include <Windows.h>
#include <stdint.h>
#include <Winbase.h>

#ifdef __GLIBC__
#if !defined(_POSIX_C_SOURCE) && !defined(_POSIX_SOURCE)
typedef struct _SYSTEMTIME {
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME;

#endif
#endif

#define  Max(a,b) ((a)>(b)?(a):(b))
#define  N   320/*768*/
double   maxeps = 0.1e-7;
int itmax = 10;
int i, j, k;
double eps;
double A[N][N][N], B[N][N][N];

void relax();
void resid();
void init();
void verify();
void wtime();

int main(int an, char **as)
{
	int it;
	double time0, time1;
	init();
	/* time0=omp_get_wtime (); */
	//wtime(&time0);
	for (it = 1; it <= itmax; it++)
	{
		eps = 0.;
		relax();
		resid();
		printf("it=%4i   eps=%f\n", it, eps);
		if (eps < maxeps) break;
	}
//	wtime(&time1);
	/* time1=omp_get_wtime (); */
	//printf("Time in seconds=%gs\t", time1 - time0);
	verify();
	
	system("pause");
	return 0;
}

void init()
{
	for (i = 0; i <= N - 1; i++)
		for (j = 0; j <= N - 1; j++)
			for (k = 0; k <= N - 1; k++)
			{
				if (i == 0 || i == N - 1 || j == 0 || j == N - 1 || k == 0 || k == N - 1) A[i][j][k] = 0.;
				else A[i][j][k] = (4. + i + j + k);
			}
}

void relax()
{
	for (i = 1; i <= N - 2; i++)
		for (j = 1; j <= N - 2; j++)
			for (k = 1; k <= N - 2; k++)
			{
				B[i][j][k] = (A[i - 1][j][k] + A[i + 1][j][k] + A[i][j - 1][k] + A[i][j + 1][k] + A[i][j][k - 1] + A[i][j][k + 1]) / 6.;
			}
}

void resid()
{
	for (i = 1; i <= N - 2; i++)
		for (j = 1; j <= N - 2; j++)
			for (k = 1; k <= N - 2; k++)
			{
				double e;
				e = fabs(A[i][j][k] - B[i][j][k]);
				A[i][j][k] = B[i][j][k];
				eps = Max(eps, e);
			}
}

void verify()
{
	double s;
	s = 0.;
	for (i = 0; i <= N - 1; i++)
		for (j = 0; j <= N - 1; j++)
			for (k = 0; k <= N - 1; k++)
			{
				s = s + A[i][j][k] * (i + 1)*(j + 1)*(k + 1) / (N*N*N);
			}
	printf("  S = %f\n", s);
}

void wtime(double *t)
{
	static int sec = -1;
	//struct timeval /*TIMEVAL*/ tv;
	SYSTEMTIME tv;
	/*gettimeofday(&tv, (void *)0);*/
	GetLocalTime(&tv);
	if (sec < 0) sec = tv.wSecond;
	*t = (tv.wSecond - sec) + 1.0e-6*tv.wMilliseconds;
}
