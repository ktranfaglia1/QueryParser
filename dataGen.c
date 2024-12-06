/* Author: Kyle Tranfaglia
*  COSC420: Parallel Database Query Processing System Project
*  Last updated 11/13/24
*  This program randomly generates a txt car inventory database
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_CARS 1600000
#define NUM_MODELS 225
#define NUM_DEALERS 100
#define NUM_COLORS 15

// Constant array of car models
const char* car_models[NUM_MODELS] = {
    "accord", "civic", "corolla", "camry", "mustang", "f-150", "silverado", "ram 1500", "tacoma", "explorer",
    "escape", "rogue", "altima", "sentra", "pathfinder", "highlander", "pilot", "cr-v", "cx-5", "forester",
    "outback", "impreza", "cherokee", "wrangler", "grand cherokee", "model 3", "model s", "model x", "model y",
    "leaf", "bolt ev", "blazer", "equinox", "traverse", "tahoe", "malibu", "fusion", "focus", "edge", "expedition",
    "avalon", "prius", "4runner", "land cruiser", "sierra 1500", "sierra 2500", "ranger", "titan", "maverick",
    "frontier", "x5", "x3", "q5", "a4", "a6", "q7", "rx 350", "es 350", "nx 300", "is 300", "gx 460",
    "mdx", "tlx", "ilx", "rdx", "encore", "envision", "regal", "velar", "range rover", "discovery", "xc90",
    "xc60", "s60", "v60", "tiguan", "atlas", "golf", "passat", "jetta", "glc", "gle", "c-class", "e-class",
    "s-class", "macan", "cayenne", "outlander", "mirage", "telluride", "sorento", "soul", "forte", "k5",
    "elantra", "sonata", "tucson", "santa fe", "rio", "stinger", "optima", "genesis g70", "genesis g80",
    "genesis g90", "spark", "impala", "sonic", "cruze", "trax", "captiva", "avalon", "corvette", "venza",
    "rav4", "sequoia", "tundra", "matrix", "yaris", "crosstrek", "ascent", "palisade", "venue", "ioniq",
    "niro", "kona", "santa cruz", "q3", "a3", "a5", "a7", "q8", "tt", "r8", "xc40", "xc70", "s90", "v90",
    "gla", "glb", "gls", "cla", "slk", "amg gt", "f-type", "xj", "xf", "xe", "e-pace", "i-pace", "f-pace",
    "db11", "dbx", "vantage", "rapide", "ghost", "phantom", "wraith", "cullinan", "bentayga", "continental gt",
    "mulsanne", "cayman", "911", "panamera", "aventador", "huracan", "urus", "chiron", "veyron", "enzo",
    "f12", "488 gtb", "812 superfast", "portofino", "roma", "ff", "vanquish", "cts", "ct5", "xt4", "xt5",
    "xt6", "escalade", "srx", "aviator", "navigator", "mkc", "mkz", "mkt", "corsair", "glk", "qashqai",
    "murano", "maxima", "armada", "versa", "500x", "500l", "giulia", "stelvio", "levante", "ghibli", "quattroporte",
    "mc20", "kizashi", "vitara", "celerio", "jimny", "swift", "baleno", "ertiga", "school bus", "ambulance", "m1 abrams",
    "jing-mobile", "batmobile", "the mystery machine", "the invisible boatmobile", "ice cream truck", "wienermobile",
    "delorean", "ecto-1", "lada", "the magic school bus", "lightning mcqueen (kachow)"
};

// Constant array of car dealers
const char* car_dealers[NUM_DEALERS] = {
    "smith auto", "classic motors", "pioneer cars", "city auto sales", "peak automotive", "valley view cars",
    "sunny road motors", "greenfield dealership", "elite autos", "sunrise car co.", "prime vehicles", "power motors",
    "north side autos", "westside cars", "autoland", "riverside auto group", "mega auto sales", "easy drive autos",
    "budget cars", "premier car sales", "all-wheel autos", "smooth ride autos", "superb motors", "champion motors",
    "prime choice motors", "econo autos", "greenway cars", "east side car co.", "right choice autos", "best buy autos",
    "metro car dealers", "family drive autos", "zoom cars", "sunshine auto sales", "parkside motors", "top gear autos",
    "all seasons motors", "central auto sales", "value cars", "blue horizon autos", "redline motors", "luxury motors",
    "highland auto sales", "big city autos", "national car sales", "city center autos", "smart choice cars",
    "roadmaster auto", "ultimate ride autos", "fairway car sales", "south side autos", "sunset motors",
    "prestige autos", "downtown motors", "eagle motors", "titan auto", "high performance autos", "speedway motors",
    "drive right cars", "select cars", "auto superstore", "motor plaza", "auto gallery", "easy choice autos",
    "reliable ride autos", "adventure motors", "uptown auto", "victory auto", "ace autos", "fast track cars",
    "reliable autos", "next car dealership", "big deal autos", "pro auto sales", "dependable car sales",
    "journey motors", "value ride autos", "elite car deals", "premium autos", "legacy auto sales", "superior motors",
    "pro choice autos", "green auto sales", "urban autos", "choice ride autos", "grand motors", "royal motors",
    "economy autos", "auto nation", "dynamic motors", "optimum autos", "trusty motors", "golden motors",
    "modern autos", "galaxy car sales", "east end motors", "westward motors", "auto palace", "river auto", "auto haven"
};

// Constant array of car colors
const char* car_colors[NUM_COLORS] = {
    "black", "white", "gray", "silver", "blue", "red", "green", "yellow", "brown", "orange",
    "gold", "beige", "purple", "maroon", "cyan"
};


// Generate a random number in a given range
int get_random_in_range(int min, int max) {
    return rand() % (max - min + 1) + min;
}


// Generate the car inventory
void generate_car_inventory(const char* filename, int num_cars) {
    FILE* car_inventory_file = fopen(filename, "w"); 

    if (car_inventory_file == NULL) {
        printf("Error opening the file %s.\n", filename);
        return;
    }

    srand(time(NULL));  // Seed the random number generator

    // Generate desired amount of data entries for the car inventory file
    for (int i = 1; i <= num_cars; i++) {

        // Randomly select model, dealer, and color using the prefixed arrays
        const char* model = car_models[get_random_in_range(0, NUM_MODELS - 1)];
        const char* dealer = car_dealers[get_random_in_range(0, NUM_DEALERS - 1)];
        const char* color = car_colors[get_random_in_range(0, NUM_COLORS - 1)];
        
        // Randomly generate make year and price
        int make_year = get_random_in_range(1924, 2024);
        int price = get_random_in_range(10000, 50000);

        // Write the car's information to the file
        fprintf(car_inventory_file, "%d,%s,%d,%s,%d,%s\n", i, model, make_year, color, price, dealer);
    }

    fclose(car_inventory_file);
    printf("Car inventory generated and saved to %s\n", filename);
}

// Driver Program
int main() {
    clock_t start_time = clock();
    
    generate_car_inventory("car_inventory.txt", NUM_CARS);  // Generate car entries and save them to 'car_inventory.txt'

    clock_t end_time = clock();

    printf("Time: %.8f Seconds\n", ((double)(end_time - start_time)) / CLOCKS_PER_SEC);
    return 0;
}
