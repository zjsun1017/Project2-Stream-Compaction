/**
 * @file      main.cpp
 * @brief     Stream compaction test program
 * @authors   Kai Ninomiya
 * @date      2015
 * @copyright University of Pennsylvania
 */

#include <cstdio>
#include <stream_compaction/cpu.h>
#include <stream_compaction/naive.h>
#include <stream_compaction/efficient.h>
#include <stream_compaction/thrust.h>
#include <stream_compaction/radix.h>
#include "testing_helpers.hpp"

 const int SIZE = 1 << 20; // feel free to change the size of array
 const int NPOT = SIZE - 3; // Non-Power-Of-Two
 int *a = new int[SIZE];
 int *b = new int[SIZE];
 int *c = new int[SIZE];

const int MAX_POW = 30;

int main(int argc, char* argv[]) {
	// Scan tests
	printf("\n");
	printf("****************\n");
	printf("** SCAN TESTS **\n");
	printf("****************\n");

	genArray(SIZE - 1, a, 50);  // Leave a 0 at the end to test that edge case
	a[SIZE - 1] = 0;
	printArray(SIZE, a, true);

	 /*initialize b using StreamCompaction::CPU::scan you implement
	 We use b for further comparison. Make sure your StreamCompaction::CPU::scan is correct.
	 At first all cases passed because b && c are all zeroes.*/
	zeroArray(SIZE, b);
	printDesc("cpu scan, power-of-two");
	StreamCompaction::CPU::scan(SIZE, b, a);
	printElapsedTime(StreamCompaction::CPU::timer().getCpuElapsedTimeForPreviousOperation(), "(std::chrono Measured)");
	printArray(SIZE, b, true);

	zeroArray(SIZE, c);
	printDesc("cpu scan, non-power-of-two");
	StreamCompaction::CPU::scan(NPOT, c, a);
	printElapsedTime(StreamCompaction::CPU::timer().getCpuElapsedTimeForPreviousOperation(), "(std::chrono Measured)");
	printArray(NPOT, b, true);
	printCmpResult(NPOT, b, c);

	zeroArray(SIZE, c);
	printDesc("naive scan, power-of-two");
	StreamCompaction::Naive::scan(SIZE, c, a);
	printElapsedTime(StreamCompaction::Naive::timer().getGpuElapsedTimeForPreviousOperation(), "(CUDA Measured)");
	//printArray(SIZE, c, true);
	printCmpResult(SIZE, b, c);

	/* For bug-finding only: Array of 1s to help find bugs in stream compaction or scan
	onesArray(SIZE, c);
	printDesc("1s array for finding bugs");
	StreamCompaction::Naive::scan(SIZE, c, a);
	printArray(SIZE, c, true); */

	zeroArray(SIZE, c);
	printDesc("naive scan, non-power-of-two");
	StreamCompaction::Naive::scan(NPOT, c, a);
	printElapsedTime(StreamCompaction::Naive::timer().getGpuElapsedTimeForPreviousOperation(), "(CUDA Measured)");
	//printArray(SIZE, c, true);
	printCmpResult(NPOT, b, c);

	zeroArray(SIZE, c);
	printDesc("work-efficient scan, power-of-two");
	StreamCompaction::Efficient::scan(SIZE, c, a);
	printElapsedTime(StreamCompaction::Efficient::timer().getGpuElapsedTimeForPreviousOperation(), "(CUDA Measured)");
	//printArray(SIZE, c, true);
	printCmpResult(SIZE, b, c);

	zeroArray(SIZE, c);
	printDesc("work-efficient scan, non-power-of-two");
	StreamCompaction::Efficient::scan(NPOT, c, a);
	printElapsedTime(StreamCompaction::Efficient::timer().getGpuElapsedTimeForPreviousOperation(), "(CUDA Measured)");
	//printArray(NPOT, c, true);
	printCmpResult(NPOT, b, c);

	zeroArray(SIZE, c);
	printDesc("thrust scan, power-of-two");
	StreamCompaction::Thrust::scan(SIZE, c, a);
	printElapsedTime(StreamCompaction::Thrust::timer().getGpuElapsedTimeForPreviousOperation(), "(CUDA Measured)");
	//printArray(SIZE, c, true);
	printCmpResult(SIZE, b, c);

	zeroArray(SIZE, c);
	printDesc("thrust scan, non-power-of-two");
	StreamCompaction::Thrust::scan(NPOT, c, a);
	printElapsedTime(StreamCompaction::Thrust::timer().getGpuElapsedTimeForPreviousOperation(), "(CUDA Measured)");
	//printArray(NPOT, c, true);
	printCmpResult(NPOT, b, c);

	printf("\n");
	printf("*****************************\n");
	printf("** STREAM COMPACTION TESTS **\n");
	printf("*****************************\n");

	// Compaction tests
	genArray(SIZE - 1, a, 4);  // Leave a 0 at the end to test that edge case
	a[SIZE - 1] = 0;
	printArray(SIZE, a, true);

	int count, expectedCount, expectedNPOT;

	// initialize b using StreamCompaction::CPU::compactWithoutScan you implement
	// We use b for further comparison. Make sure your StreamCompaction::CPU::compactWithoutScan is correct.
	zeroArray(SIZE, b);
	printDesc("cpu compact without scan, power-of-two");
	count = StreamCompaction::CPU::compactWithoutScan(SIZE, b, a);
	printElapsedTime(StreamCompaction::CPU::timer().getCpuElapsedTimeForPreviousOperation(), "(std::chrono Measured)");
	expectedCount = count;
	printArray(count, b, true);
	printCmpLenResult(count, expectedCount, b, b);

	zeroArray(SIZE, c);
	printDesc("cpu compact without scan, non-power-of-two");
	count = StreamCompaction::CPU::compactWithoutScan(NPOT, c, a);
	printElapsedTime(StreamCompaction::CPU::timer().getCpuElapsedTimeForPreviousOperation(), "(std::chrono Measured)");
	expectedNPOT = count;
	printArray(count, c, true);
	printCmpLenResult(count, expectedNPOT, b, c);

	zeroArray(SIZE, c);
	printDesc("cpu compact with scan");
	count = StreamCompaction::CPU::compactWithScan(SIZE, c, a);
	printElapsedTime(StreamCompaction::CPU::timer().getCpuElapsedTimeForPreviousOperation(), "(std::chrono Measured)");
	printArray(count, c, true);
	printCmpLenResult(count, expectedCount, b, c);

	zeroArray(SIZE, c);
	printDesc("work-efficient compact, power-of-two");
	count = StreamCompaction::Efficient::compact(SIZE, c, a);
	printElapsedTime(StreamCompaction::Efficient::timer().getGpuElapsedTimeForPreviousOperation(), "(CUDA Measured)");
	//printArray(count, c, true);
	printCmpLenResult(count, expectedCount, b, c);

	zeroArray(SIZE, c);
	printDesc("work-efficient compact, non-power-of-two");
	count = StreamCompaction::Efficient::compact(NPOT, c, a);
	printElapsedTime(StreamCompaction::Efficient::timer().getGpuElapsedTimeForPreviousOperation(), "(CUDA Measured)");
	//printArray(count, c, true);
	printCmpLenResult(count, expectedNPOT, b, c);

	//Sort tests
	printf("\n");
	printf("*****************************\n");
	printf("********* SORT TESTS ********\n");
	printf("*****************************\n");

	zeroArray(SIZE, c);
	printDesc("radix sort, power-of-two");
	StreamCompaction::Radix::sort(SIZE, c, a);
	printElapsedTime(StreamCompaction::Radix::timer().getGpuElapsedTimeForPreviousOperation(), "(CUDA Measured)");
	printArray(SIZE, c, true);
	if (std::is_sorted(c, c + SIZE)) printf("passed\n");
	else printf("failed\n");

	zeroArray(SIZE, c);
	printDesc("radix sort, non-power-of-two");
	StreamCompaction::Radix::sort(NPOT, c, a);
	printElapsedTime(StreamCompaction::Radix::timer().getGpuElapsedTimeForPreviousOperation(), "(CUDA Measured)");
	printArray(NPOT, c, true);
	if (std::is_sorted(c, c + NPOT)) printf("passed\n");
	else printf("failed\n");

	system("pause"); // stop Win32 console from closing on exit
	delete[] a;
	delete[] b;
	delete[] c;


	////test of scan and compact time
	//printf("Power of 2,CPU Scan Time (ms),Naive Scan Time (ms),Efficient Scan Time (ms),Thrust Scan Time (ms),CPU Compact without Scan Time (ms),CPU Compact with Scan Time (ms),Efficient GPU Compact Time (ms)\n");
	//for (int POW = 0; POW <= MAX_POW; POW++) {
	//	int SIZE = 1 << POW;
	//	int* a = new int[SIZE];
	//	int* b = new int[SIZE];
	//	int* c = new int[SIZE];

	//	genArray(SIZE - 1, a, 50);
	//	a[SIZE - 1] = 0;

	//	zeroArray(SIZE, b);
	//	StreamCompaction::CPU::scan(SIZE, b, a);
	//	double cpuScanTime = StreamCompaction::CPU::timer().getCpuElapsedTimeForPreviousOperation();

	//	zeroArray(SIZE, c);
	//	StreamCompaction::Naive::scan(SIZE, c, a);
	//	double naiveScanTime = StreamCompaction::Naive::timer().getGpuElapsedTimeForPreviousOperation();

	//	zeroArray(SIZE, c);
	//	StreamCompaction::Efficient::scan(SIZE, c, a);
	//	double efficientScanTime = StreamCompaction::Efficient::timer().getGpuElapsedTimeForPreviousOperation();

	//	zeroArray(SIZE, c);
	//	StreamCompaction::Thrust::scan(SIZE, c, a);
	//	double thrustScanTime = StreamCompaction::Thrust::timer().getGpuElapsedTimeForPreviousOperation();

	//	zeroArray(SIZE, c);
	//	int compactWithoutScanCount = StreamCompaction::CPU::compactWithoutScan(SIZE, c, a);
	//	double cpuCompactWithoutScanTime = StreamCompaction::CPU::timer().getCpuElapsedTimeForPreviousOperation();

	//	zeroArray(SIZE, c);
	//	int compactWithScanCount = StreamCompaction::CPU::compactWithScan(SIZE, c, a);
	//	double cpuCompactWithScanTime = StreamCompaction::CPU::timer().getCpuElapsedTimeForPreviousOperation();

	//	zeroArray(SIZE, c);
	//	int efficientCompactCount = StreamCompaction::Efficient::compact(SIZE, c, a);
	//	double efficientCompactTime = StreamCompaction::Efficient::timer().getGpuElapsedTimeForPreviousOperation();

	//	printf("%d,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n", POW, cpuScanTime, naiveScanTime, efficientScanTime, thrustScanTime, cpuCompactWithoutScanTime, cpuCompactWithScanTime, efficientCompactTime);

	//	delete[] a;
	//	delete[] b;
	//	delete[] c;
	//}

	return 0;
}
