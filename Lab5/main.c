#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#include "receipter.h"

int main(void)
{
    time_t time_now = 1569472997;
    /*time_t time_now = time(NULL);*/

    assert(TRUE == add_item("Magarita", 12.45));
    assert(TRUE == add_item("Sharkfin soup", 500.12));
    assert(TRUE == add_item("Kingcrab dip", 125.99));
    assert(TRUE == add_item("Deep fried scallops", 7.36));
    assert(TRUE == add_item("Tacoyaki", 6.89));
    assert(TRUE == add_item("Salmon sashimi", 16.54));
    assert(TRUE == add_item("Tuna sashimi", 15.42));
    assert(TRUE == add_item("Unagi soup", 25.48));
    assert(TRUE == add_item("Nestea ice tea", 5.99));
    assert(TRUE == add_item("Chocolate brownie fudge", 8.99));

    assert(FALSE == add_item("This won't get added lol", 2.55));

    set_tip(20.55);

    set_message("Thanks for dining with us!a123456789b123456789c123456789d123456789e123456789f123456789");
    assert(strlen("Thanks for dining with us!a123456789b123456789c123456789d123456789e12345678") == 75);

    assert(TRUE == print_receipt("receipt0.txt", time_now));

    assert(TRUE == add_item("Raw sea cucumber", 34.26));
    assert(TRUE == add_item("Pan fried Mackerel", 16.58));
    assert(TRUE == add_item("Pepsi", 5.99));

    assert(TRUE == print_receipt("receipt1.txt", time_now));

    set_message("My message");

    assert(FALSE == print_receipt("receipt2.txt", time_now));

    assert(TRUE == add_item("Strawberry Shortcake", 15));

    set_tip(5);

    assert(TRUE == print_receipt("receipt3.txt", time_now));

    printf("main end");
    return 0;
}
