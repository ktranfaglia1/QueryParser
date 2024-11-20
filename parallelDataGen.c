/* Author: Kyle Tranfaglia
*  COSC420: Parallel Database Query Processing System Project
*  Last updated 11/13/24
*  This program randomly generates a txt car inventory database in parallel
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

#define NUM_CARS 1000000
#define NUM_MODELS 225
#define NUM_DEALERS 100
#define NUM_COLORS 15

// Constant array of car models
const char* car_models[NUM_MODELS] = {
    "Accord", "Civic", "Corolla", "Camry", "Mustang", "F-150", "Silverado", "Ram 1500", "Tacoma", "Explorer",
    "Escape", "Rogue", "Altima", "Sentra", "Pathfinder", "Highlander", "Pilot", "CR-V", "CX-5", "Forester",
    "Outback", "Impreza", "Cherokee", "Wrangler", "Grand Cherokee", "Model 3", "Model S", "Model X", "Model Y",
    "Leaf", "Bolt EV", "Blazer", "Equinox", "Traverse", "Tahoe", "Malibu", "Fusion", "Focus", "Edge", "Expedition",
    "Avalon", "Prius", "4Runner", "Land Cruiser", "Sierra 1500", "Sierra 2500", "Ranger", "Titan", "Maverick",
    "Frontier", "X5", "X3", "Q5", "A4", "A6", "Q7", "RX 350", "ES 350", "NX 300", "IS 300", "GX 460",
    "MDX", "TLX", "ILX", "RDX", "Encore", "Envision", "Regal", "Velar", "Range Rover", "Discovery", "XC90",
    "XC60", "S60", "V60", "Tiguan", "Atlas", "Golf", "Passat", "Jetta", "GLC", "GLE", "C-Class", "E-Class",
    "S-Class", "Macan", "Cayenne", "Outlander", "Mirage", "Telluride", "Sorento", "Soul", "Forte", "K5",
    "Elantra", "Sonata", "Tucson", "Santa Fe", "Rio", "Stinger", "Optima", "Genesis G70", "Genesis G80",
    "Genesis G90", "Spark", "Impala", "Sonic", "Cruze", "Trax", "Captiva", "Avalon", "Corvette", "Venza",
    "RAV4", "Sequoia", "Tundra", "Matrix", "Yaris", "Crosstrek", "Ascent", "Palisade", "Venue", "Ioniq",
    "Niro", "Kona", "Santa Cruz", "Q3", "A3", "A5", "A7", "Q8", "TT", "R8", "XC40", "XC70", "S90", "V90",
    "GLA", "GLB", "GLS", "CLA", "SLK", "AMG GT", "F-Type", "XJ", "XF", "XE", "E-Pace", "I-Pace", "F-Pace",
    "DB11", "DBX", "Vantage", "Rapide", "Ghost", "Phantom", "Wraith", "Cullinan", "Bentayga", "Continental GT",
    "Mulsanne", "Cayman", "911", "Panamera", "Aventador", "Huracan", "Urus", "Chiron", "Veyron", "Enzo", 
    "F12", "488 GTB", "812 Superfast", "Portofino", "Roma", "FF", "Vanquish", "CTS", "CT5", "XT4", "XT5",
    "XT6", "Escalade", "SRX", "Aviator", "Navigator", "MKC", "MKZ", "MKT", "Corsair", "GLK", "Qashqai",
    "Murano", "Maxima", "Armada", "Versa", "500X", "500L", "Giulia", "Stelvio", "Levante", "Ghibli", "Quattroporte",
    "MC20", "Kizashi", "Vitara", "Celerio", "Jimny", "Swift", "Baleno", "Cybertruck", "School Bus", "Ambulance", "M1 Abrams",
    "Jing-Mobile", "Batmobile", "The Mystery Machine", "The Invisible Boatmobile", "Ice Cream Truck", "Wienermobile",
    "DeLorean", "Ecto-1", "Lada", "The Magic School Bus", "Lightning McQueen (Kachow)"
};

// Constant array of car dealers
const char* car_dealers[NUM_DEALERS] = {
    "Smith Auto", "Classic Motors", "Pioneer Cars", "City Auto Sales", "Peak Automotive", "Valley View Cars",
    "Sunny Road Motors", "Greenfield Dealership", "Elite Autos", "Sunrise Car Co.", "Prime Vehicles", "Power Motors",
    "North Side Autos", "Westside Cars", "Autoland", "Riverside Auto Group", "Mega Auto Sales", "Easy Drive Autos",
    "Budget Cars", "Premier Car Sales", "All-Wheel Autos", "Smooth Ride Autos", "Superb Motors", "Champion Motors",
    "Prime Choice Motors", "Econo Autos", "Greenway Cars", "East Side Car Co.", "Right Choice Autos", "Best Buy Autos",
    "Metro Car Dealers", "Family Drive Autos", "Zoom Cars", "Sunshine Auto Sales", "Parkside Motors", "Top Gear Autos",
    "All Seasons Motors", "Central Auto Sales", "Value Cars", "Blue Horizon Autos", "Redline Motors", "Luxury Motors",
    "Highland Auto Sales", "Big City Autos", "National Car Sales", "City Center Autos", "Smart Choice Cars",
    "Roadmaster Auto", "Ultimate Ride Autos", "Fairway Car Sales", "South Side Autos", "Sunset Motors",
    "Prestige Autos", "Downtown Motors", "Eagle Motors", "Titan Auto", "High Performance Autos", "Speedway Motors",
    "Drive Right Cars", "Select Cars", "Auto Superstore", "Motor Plaza", "Auto Gallery", "Easy Choice Autos",
    "Reliable Ride Autos", "Adventure Motors", "Uptown Auto", "Victory Auto", "Ace Autos", "Fast Track Cars",
    "Reliable Autos", "Next Car Dealership", "Big Deal Autos", "Pro Auto Sales", "Dependable Car Sales",
    "Journey Motors", "Value Ride Autos", "Elite Car Deals", "Premium Autos", "Legacy Auto Sales", "Superior Motors",
    "Pro Choice Autos", "Green Auto Sales", "Urban Autos", "Choice Ride Autos", "Grand Motors", "Royal Motors",
    "Economy Autos", "Auto Nation", "Dynamic Motors", "Optimum Autos", "Trusty Motors", "Golden Motors",
    "Modern Autos", "Galaxy Car Sales", "East End Motors", "Westward Motors", "Auto Palace", "River Auto", "Auto Haven"
};

// Constant array of car colors
const char* car_colors[NUM_COLORS] = {
    "Black", "White", "Gray", "Silver", "Blue", "Red", "Green", "Yellow", "Brown", "Orange",
    "Gold", "Beige", "Purple", "Maroon", "Cyan"
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
    #pragma omp parallel for schedule(static)
    for (int i = 1; i <= num_cars; i++) {

        // Randomly select model, dealer, and color using the prefixed arrays
        const char* model = car_models[get_random_in_range(0, NUM_MODELS - 1)];
        const char* dealer = car_dealers[get_random_in_range(0, NUM_DEALERS - 1)];
        const char* color = car_colors[get_random_in_range(0, NUM_COLORS - 1)];
        
        // Randomly generate make year and price
        int make_year = get_random_in_range(1924, 2024);
        int price = get_random_in_range(10, 50) * 1000;

        // Write the car's information to the file
        fprintf(car_inventory_file, "%d,%s,%d,%s,%d,%s\n", i, model, make_year, color, price, dealer);
    }

    fclose(car_inventory_file);
    printf("Car inventory generated and saved to %s\n", filename);
}

// Driver Program
int main() {
    double start_time = omp_get_wtime();
    
    generate_car_inventory("car_inventory.txt", NUM_CARS);  // Generate car entries and save them to 'car_inventory.txt'

    double end_time = omp_get_wtime();

    printf("Time: %.8f Seconds\n", end_time - start_time);
    return 0;
}
