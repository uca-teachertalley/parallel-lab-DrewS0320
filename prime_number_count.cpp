/*-- prime_count_parallel.cpp-----------------------------------------------------------
   This file implements a program that fills an array with numbers and
   then counts the prime numbers in the array using OpenMP for parallelism.
---------------------------------------------------------------------------------------*/

#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>
#include <omp.h>  
using namespace std;

// gen_numbers
void gen_numbers(long int numbers[], long int how_many)
{
	for (int i = 0; i < how_many; i++)
	{
		numbers[i] = i;
	}
}

// This function determines if a number is a prime number
bool is_prime(long int n)
{
	if (n < 2)
		return false;

	for (int i = 2; i <= sqrt(n); i++)
	{
		if (n % i == 0)
			return false;
	}
	return true;
}

int count_prime_parallel(long int numbers[], long int how_many)
{
	int count = 0;
	int max_threads = omp_get_max_threads();
	std::vector<int> thread_work_count(max_threads, 0); 

#pragma omp parallel
	{
		int thread_num = omp_get_thread_num();

#pragma omp for reduction(+:count)
		for (int i = 0; i < how_many; i++)
		{
			if (is_prime(numbers[i]))
				count++;

			thread_work_count[thread_num]++;
		}
	}

	for (int i = 0; i < thread_work_count.size(); ++i) {
		cout << "Thread " << i << " processed " << thread_work_count[i] << " elements." << endl;
	}

	return count;
}

int main() {
	long int n = 1000000;
	long int* numbers = new long int[n];

	cout << "Generating numbers..." << endl;
	gen_numbers(numbers, n);

	cout << "Counting primes in parallel..." << endl;
	auto start = chrono::steady_clock::now();
	int count = count_prime_parallel(numbers, n);
	auto end = chrono::steady_clock::now();

	double compute_time = chrono::duration<double>(end - start).count();
	cout << "Total number of primes = " << count << endl;
	cout << "Total computation time = " << compute_time << endl;

	delete[] numbers;
	return 0;
}
