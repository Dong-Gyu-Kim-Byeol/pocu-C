#include <assert.h>
#include <stdio.h>

#include "cyber_spaceship.h"

#define CAB_LENGTH (50)

void test_cab_length_upto_100(void);
void test_etc(void);

int main(void)
{
    {
        char cab[CAB_LENGTH];
        const char* cluster_start_addresses[5];
        size_t cluster_lengths[5];

        size_t out_longest_safe_area_length = 0;
        size_t* out_longest_safe_area_length_p = &out_longest_safe_area_length;
        const char* longest_safe_cluster_start_address;
        int time_in_mins;
        const char* cab_start_address = cab;

        cluster_start_addresses[0] = &cab[8];
        cluster_start_addresses[1] = &cab[1];
        cluster_start_addresses[2] = &cab[13];
        cluster_start_addresses[3] = &cab[33];
        cluster_start_addresses[4] = &cab[38];

        cluster_lengths[0] = 10U;
        cluster_lengths[1] = 22U;
        cluster_lengths[2] = 10U;
        cluster_lengths[3] = 13U;
        cluster_lengths[4] = 10U;

        {
            longest_safe_cluster_start_address = get_longest_safe_zone_or_null(cab_start_address, CAB_LENGTH, cluster_start_addresses, cluster_lengths, 5, out_longest_safe_area_length_p);

            assert(out_longest_safe_area_length == 15);
            assert(longest_safe_cluster_start_address == cab_start_address + 18);
        }

        {
            time_in_mins = get_travel_time(cab_start_address, CAB_LENGTH, cluster_start_addresses, cluster_lengths, 5);

            assert(time_in_mins == 7);
        }
    }

    test_cab_length_upto_100();

    test_etc();

    return 0;
}

void test_cab_length_upto_100(void)
{
    /* 0 cluster */
    {
        const char cyber_asteroid_belt[100];
        int time_in_mins;
        size_t out_longest_safe_area_length = 0;
        size_t* out_longest_safe_area_length_p = &out_longest_safe_area_length;
        const char* longest_safe_cluster_start_address;
        size_t i;
        longest_safe_cluster_start_address = get_longest_safe_zone_or_null(cyber_asteroid_belt, 0, NULL, NULL, 0, out_longest_safe_area_length_p);
        assert(longest_safe_cluster_start_address == NULL);
        assert(*out_longest_safe_area_length_p == 0);
        time_in_mins = get_travel_time(cyber_asteroid_belt, 0, NULL, NULL, 0);
        assert(time_in_mins == 0);
        for (i = 1; i <= 100; i++) {
            longest_safe_cluster_start_address = get_longest_safe_zone_or_null(cyber_asteroid_belt, i, NULL, NULL, 0, out_longest_safe_area_length_p);
            assert(longest_safe_cluster_start_address == cyber_asteroid_belt);
            assert(*out_longest_safe_area_length_p == i);
            time_in_mins = get_travel_time(cyber_asteroid_belt, i, NULL, NULL, 0);
            assert(time_in_mins == (i + 5) / 10); /* check if your double value is like x.4999... < x.5 */
        }
    }
    /* 2 clusters */
    {
        const char cyber_asteroid_belt[100];
        int time_in_mins;
        const char* cluster_start_addresses[2];
        size_t cluster_lengths[2];
        size_t out_longest_safe_area_length = 0;
        size_t* out_longest_safe_area_length_p = &out_longest_safe_area_length;
        const char* longest_safe_cluster_start_address;
        size_t i;
        cluster_start_addresses[0] = &cyber_asteroid_belt[0];
        cluster_start_addresses[1] = &cyber_asteroid_belt[0];
        for (i = 1; i <= 100; i++) {
            cluster_lengths[0] = i;
            cluster_lengths[1] = i;
            longest_safe_cluster_start_address = get_longest_safe_zone_or_null(cyber_asteroid_belt, i, cluster_start_addresses, cluster_lengths, 2, out_longest_safe_area_length_p);
            assert(longest_safe_cluster_start_address == cyber_asteroid_belt);
            assert(*out_longest_safe_area_length_p == i);
            time_in_mins = get_travel_time(cyber_asteroid_belt, i, cluster_start_addresses, cluster_lengths, 2);
            assert(time_in_mins == (i + 5) / 10);
        }
    }
    /* 1 cluster */
    {
        const char cyber_asteroid_belt[100];
        int time_in_mins;
        const char* cluster_start_addresses[1];
        size_t cluster_lengths[1];
        size_t out_longest_safe_area_length = 0;
        size_t* out_longest_safe_area_length_p = &out_longest_safe_area_length;
        const char* longest_safe_cluster_start_address;
        size_t i;
        cluster_start_addresses[0] = &cyber_asteroid_belt[0];
        for (i = 1; i <= 100; i++) {
            cluster_lengths[0] = i;
            longest_safe_cluster_start_address = get_longest_safe_zone_or_null(cyber_asteroid_belt, i, cluster_start_addresses, cluster_lengths, 1, out_longest_safe_area_length_p);

            assert(*out_longest_safe_area_length_p == 0);
            assert(longest_safe_cluster_start_address == NULL);
            time_in_mins = get_travel_time(cyber_asteroid_belt, i, cluster_start_addresses, cluster_lengths, 1);
            assert(time_in_mins == (i * 2 + 5) / 10); /* check if your double value is like x.9999... < x+1 */
        }
    }
    printf("test_cab_length_upto_100 clear\n\n");
}

void test_etc(void)
{
    /* multiple clusters & safe + dangerous area */
    {
        const char cyber_asteroid_belt[100];
        const char* cluster_start_addresses[8];
        size_t cluster_lengths[8];
        size_t out_longest_safe_area_length = 0;
        size_t* out_longest_safe_area_length_p = &out_longest_safe_area_length;
        const char* longest_safe_cluster_start_address;
        int time_in_mins;
        cluster_start_addresses[0] = &cyber_asteroid_belt[0];
        cluster_start_addresses[1] = &cyber_asteroid_belt[1];
        cluster_start_addresses[2] = &cyber_asteroid_belt[2];
        cluster_start_addresses[3] = &cyber_asteroid_belt[3];
        cluster_start_addresses[4] = &cyber_asteroid_belt[4];
        cluster_start_addresses[5] = &cyber_asteroid_belt[5];
        cluster_start_addresses[6] = &cyber_asteroid_belt[68];
        cluster_start_addresses[7] = &cyber_asteroid_belt[69];
        cluster_lengths[0] = 50U;
        cluster_lengths[1] = 50U;
        cluster_lengths[2] = 50U;
        cluster_lengths[3] = 50U;
        cluster_lengths[4] = 50U;
        cluster_lengths[5] = 50U;
        cluster_lengths[6] = 30U;
        cluster_lengths[7] = 30U;
        longest_safe_cluster_start_address = get_longest_safe_zone_or_null(cyber_asteroid_belt, 100, cluster_start_addresses, cluster_lengths, 8, out_longest_safe_area_length_p);
        assert(longest_safe_cluster_start_address == cyber_asteroid_belt + 5);
        assert(*out_longest_safe_area_length_p == 45);
        time_in_mins = get_travel_time(cyber_asteroid_belt, 100, cluster_start_addresses, cluster_lengths, 8);
        assert(time_in_mins == 11);
    }
    /* two same length safe areas -> following area should be result */
    {
        const char cyber_asteroid_belt[100];
        const char* cluster_start_addresses[2];
        size_t cluster_lengths[2];
        size_t out_longest_safe_area_length = 0;
        size_t* out_longest_safe_area_length_p = &out_longest_safe_area_length;
        const char* longest_safe_cluster_start_address;
        int time_in_mins;

        cluster_start_addresses[0] = &cyber_asteroid_belt[0];
        cluster_start_addresses[1] = &cyber_asteroid_belt[50];

        cluster_lengths[0] = 25U;
        cluster_lengths[1] = 25U;

        longest_safe_cluster_start_address = get_longest_safe_zone_or_null(cyber_asteroid_belt, 100, cluster_start_addresses, cluster_lengths, 2, out_longest_safe_area_length_p);
        assert(longest_safe_cluster_start_address == cyber_asteroid_belt + 75);
        assert(*out_longest_safe_area_length_p == 25);
        time_in_mins = get_travel_time(cyber_asteroid_belt, 100, cluster_start_addresses, cluster_lengths, 2);
        assert(time_in_mins == 15);
    }
    /* start with 1 cluster but end with 2 clusters */
    {
        const char cyber_asteroid_belt[100];
        const char* cluster_start_addresses[3];
        size_t cluster_lengths[3];
        size_t out_longest_safe_area_length = 0;
        size_t* out_longest_safe_area_length_p = &out_longest_safe_area_length;
        const char* longest_safe_cluster_start_address;
        int time_in_mins;

        cluster_start_addresses[0] = &cyber_asteroid_belt[0];
        cluster_start_addresses[1] = &cyber_asteroid_belt[50];
        cluster_start_addresses[2] = &cyber_asteroid_belt[50];

        cluster_lengths[0] = 50U;
        cluster_lengths[1] = 50U;
        cluster_lengths[2] = 50U;

        longest_safe_cluster_start_address = get_longest_safe_zone_or_null(cyber_asteroid_belt, 100, cluster_start_addresses, cluster_lengths, 3, out_longest_safe_area_length_p);
        assert(longest_safe_cluster_start_address == cyber_asteroid_belt + 50);
        assert(*out_longest_safe_area_length_p == 50);
        time_in_mins = get_travel_time(cyber_asteroid_belt, 100, cluster_start_addresses, cluster_lengths, 3);
        assert(time_in_mins == 15);
    }
    /* start with 2 clusters but end with 1 cluster */
    {
        const char cyber_asteroid_belt[100];
        const char* cluster_start_addresses[3];
        size_t cluster_lengths[3];
        size_t out_longest_safe_area_length = 0;
        size_t* out_longest_safe_area_length_p = &out_longest_safe_area_length;
        const char* longest_safe_cluster_start_address;
        int time_in_mins;

        cluster_start_addresses[0] = &cyber_asteroid_belt[0];
        cluster_start_addresses[1] = &cyber_asteroid_belt[0];
        cluster_start_addresses[2] = &cyber_asteroid_belt[50];

        cluster_lengths[0] = 50U;
        cluster_lengths[1] = 50U;
        cluster_lengths[2] = 50U;

        longest_safe_cluster_start_address = get_longest_safe_zone_or_null(cyber_asteroid_belt, 100, cluster_start_addresses, cluster_lengths, 3, out_longest_safe_area_length_p);
        assert(longest_safe_cluster_start_address == cyber_asteroid_belt);
        assert(*out_longest_safe_area_length_p == 50);
        time_in_mins = get_travel_time(cyber_asteroid_belt, 100, cluster_start_addresses, cluster_lengths, 3);
        assert(time_in_mins == 15);
    }
    printf("test_etc clear\n\n");
}
