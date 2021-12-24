#include "maputlis.h"

int main() {
    location loc;
    read_location_from_file("island_default.csv", 5, 4, &loc);
    print_location(&loc);
}