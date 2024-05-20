// OS 24 EX1

#include "memory_latency.h"
#include "measure.h"
#include <math.h>
#include <ctime>   // For time
//#include <thread> // For std::this_thread::sleep_for
//#include <chrono> // For std::chrono::seconds

#define GALOIS_POLYNOMIAL ((1ULL << 63) | (1ULL << 62) | (1ULL << 60) | (1ULL << 59))
#define NANOSECOND 1000000000ULL
#include <time.h>
/**
 * Converts the struct timespec to time in nano-seconds.
 * @param t - the struct timespec to convert.
 * @return - the value of time in nano-seconds.
 */
uint64_t nanosectime(struct timespec t)
{
	// Your code here
    return (t.tv_sec * NANOSECOND) + t.tv_nsec;
}

/**
* Measures the average latency of accessing a given array in a sequential order.
* @param repeat - the number of times to repeat the measurement for and average on.
* @param arr - an allocated (not empty) array to preform measurement on.
* @param arr_size - the length of the array arr.
* @param zero - a variable containing zero in a way that the compiler doesn't "know" it in compilation time.
* @return struct measurement containing the measurement with the following fields:
*      double baseline - the average time (ns) taken to preform the measured operation without memory access.
*      double access_time - the average time (ns) taken to preform the measured operation with memory access.
*      uint64_t rnd - the variable used to randomly access the array, returned to prevent compiler optimizations.
*/
struct measurement measure_sequential_latency(uint64_t repeat, array_element_t* arr, uint64_t arr_size, uint64_t zero)
{
    // Your code here
    repeat = arr_size > repeat ? arr_size:repeat; // Make sure repeat >= arr_size
	

    // Baseline measurement:
    struct timespec t0;
    timespec_get(&t0, TIME_UTC);
    register uint64_t rnd=12345;
    for (register uint64_t i = 0; i < repeat; i++)
    {
    register uint64_t index = rnd % arr_size;
        rnd ^= index & zero;
        rnd = (rnd >> 1) ^ ((0 - (rnd & 1)) & GALOIS_POLYNOMIAL);
    }
    struct timespec t1;
    timespec_get(&t1, TIME_UTC);

    // Memory access measurement:
    struct timespec t2;
    timespec_get(&t2, TIME_UTC);
    rnd=(rnd & zero) ^ 12345;
    for (register uint64_t i = 0; i < repeat; i++)
    {
        register uint64_t index = i % arr_size;
        rnd ^= arr[index] & zero;
        rnd = (rnd >> 1) ^ ((0 - (rnd & 1)) & GALOIS_POLYNOMIAL);  // Advance rnd pseudo-randomly (using Galois LFSR)
        
    }
    struct timespec t3;
    timespec_get(&t3, TIME_UTC);

    // Calculate baseline and memory access times:
    double baseline_per_cycle=(double)(nanosectime(t1)- nanosectime(t0))/(repeat);
    double memory_per_cycle=(double)(nanosectime(t3)- nanosectime(t2))/(repeat);
    struct measurement result;

    result.baseline = baseline_per_cycle;
    result.access_time = memory_per_cycle;
    result.rnd = rnd;
    return result;
}

/**
 * Runs the logic of the memory_latency program. Measures the access latency for random and sequential memory access
 * patterns.
 * Usage: './memory_latency max_size factor repeat' where:
 *      - max_size - the maximum size in bytes of the array to measure access latency for.
 *      - factor - the factor in the geometric series representing the array sizes to check.
 *      - repeat - the number of times each measurement should be repeated for and averaged on.
 * The program will print output to stdout in the following format:
 *      mem_size_1,offset_1,offset_sequential_1
 *      mem_size_2,offset_2,offset_sequential_2
 *              ...
 *              ...
 *              ...
 */
int main(int argc, char* argv[])
{
	std::srand(std::time(0));

    // zero==0, but the compiler doesn't know it. Use as the zero arg of measure_latency and measure_sequential_latency.
    struct timespec t_dummy;
    timespec_get(&t_dummy, TIME_UTC);
    const uint64_t zero = nanosectime(t_dummy)>1000000000ull?0:nanosectime(t_dummy);

    // Your code here
    if (argc != 4) {
        fprintf(stderr, "error: %s max_size factor repeat is incorrect\n", argv[0]);
        return -1;
    }
    int max_size = strtoull(argv[1], NULL, 10);
    float factor = atof(argv[2]);
    int repeat = strtoull(argv[3], NULL, 10);
    if (max_size < 100 || factor <= 1 || repeat <= 0) {
        fprintf(stderr, "Invalid arguments. max_size must be > 100, "
                        "factor must be > 1, and repeat must be >0.\n");
        return -1;
    }
    int current_size = 100;
    uint64_t arr_size = current_size / sizeof(array_element_t);
    while (current_size <= max_size) {
        array_element_t* arr = (array_element_t*) malloc(arr_size * sizeof(array_element_t));
        if (!arr) {
            printf("Failed to allocate memory");
            return -1;
        }
         //for (size_t i = 0; i < arr_size; ++i) {
        //arr[i] = std::rand() % 10000; // Random numbers between 0 and 99
        //printf("%d\n" , arr[i]);
        //std::this_thread::sleep_for(std::chrono::seconds(1));
    //}
        struct measurement measure_sequential_result  = measure_sequential_latency(repeat, arr, arr_size, zero);
        struct measurement measure_random_result = measure_latency(repeat, arr, arr_size, zero);

        double  offset1 = measure_sequential_result.access_time - measure_sequential_result.baseline;
        double  offset = measure_random_result.access_time - measure_random_result.baseline;
        printf("%d,%lf,%lf\n", current_size, offset, offset1);
        free(arr);
        current_size = ceil(current_size * factor);
        arr_size = (uint64_t)(current_size / sizeof(array_element_t));
    }
    return 0;
}
