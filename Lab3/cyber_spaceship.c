#include <assert.h>
#include <math.h>

#include "cyber_spaceship.h"

#ifndef NULL
#define NULL ((void*)0)
#endif /* !NULL */

#ifndef TRUE
#define TRUE (1)
#endif /* !TRUE */

#ifndef FALSE
#define FALSE (0)
#endif /* !FALSE */


int is_overlap(const char* const arr1, const size_t length1, const char* const arr2, const size_t length2)
{
    if (arr1 <= arr2) {
        return arr1 + length1 > arr2 ? TRUE : FALSE;
    }

    return arr2 + length2 > arr1 ? TRUE : FALSE;
}

const char* get_longest_safe_zone_or_null(
    const char* const cab_start_location,
    const size_t cab_length,
    const char* const cluster_start_locations[],
    const size_t cluster_lengths[],
    const size_t cluster_count,
    size_t* out_longest_safe_area_length)
{
    const char* longest_safe_zone_start_location = NULL;
    size_t longest_safe_zone_length = 0;

    if (cab_length == 0) {
        *out_longest_safe_area_length = 0;
        return NULL;
    }

    if (cluster_count == 0) {
        *out_longest_safe_area_length = cab_length;
        return cab_start_location;
    }

    {
        const char* temp_longest_safe_zone_start_location = NULL;
        size_t temp_longest_safe_zone_length = 0;

        size_t cab_i;
        size_t cluster_i;

        for (cab_i = 0; cab_i < cab_length; ++cab_i) {
            size_t overlap_count = 0;
            int is_safe;

            for (cluster_i = 0; cluster_i < cluster_count; ++cluster_i) {
                overlap_count += is_overlap(&cab_start_location[cab_i], 1, cluster_start_locations[cluster_i], cluster_lengths[cluster_i]);
            }
            is_safe = overlap_count % 2 == 0;

            if (is_safe == FALSE) {
                temp_longest_safe_zone_start_location = NULL;
                temp_longest_safe_zone_length = 0;
                continue;
            }

            assert(is_safe == TRUE);
            if (temp_longest_safe_zone_length == 0) {
                temp_longest_safe_zone_start_location = &cab_start_location[cab_i];
            }

            assert(temp_longest_safe_zone_start_location != NULL);
            ++temp_longest_safe_zone_length;

            if (longest_safe_zone_length <= temp_longest_safe_zone_length) {
                longest_safe_zone_start_location = temp_longest_safe_zone_start_location;
                longest_safe_zone_length = temp_longest_safe_zone_length;
            }
        }
    }

    *out_longest_safe_area_length = longest_safe_zone_length;
    return longest_safe_zone_start_location;
}

int get_travel_time(
    const char* const cab_start_location,
    const size_t cab_length,
    const char* const cluster_start_locations[],
    const size_t cluster_lengths[],
    const size_t cluster_count)
{
    const double SAFE_ZONE_DIV = 10.0;
    const double UNSAFE_ZONE_DIV = 5.0;

    if (cab_length == 0) {
        return 0.0;
    }

    if (cluster_count == 0) {
        return (int)round(cab_length / SAFE_ZONE_DIV);
    }

    {
        size_t cab_i;
        size_t cluster_i;
        size_t safa_zone_count = 0;

        for (cab_i = 0; cab_i < cab_length; ++cab_i) {
            size_t overlap_count = 0;
            int is_safe;

            for (cluster_i = 0; cluster_i < cluster_count; ++cluster_i) {
                overlap_count += is_overlap(&cab_start_location[cab_i], 1, cluster_start_locations[cluster_i], cluster_lengths[cluster_i]);
            }
            is_safe = overlap_count % 2 == 0;

            if (is_safe == TRUE) {
                ++safa_zone_count;;
            }
        }

        return (int)round(safa_zone_count / SAFE_ZONE_DIV + (cab_length - safa_zone_count) / UNSAFE_ZONE_DIV);
    }
}
