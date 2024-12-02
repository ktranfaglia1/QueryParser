Program Compilation and Execution Instructions:
  1. Make sure all files downloaded and in same directory
  2. TERMINAL COMMAND: gcc -o dataGen dataGen.c
  3. TERMINAL COMMAND: ./dataGen

This will populate a file containing a car database. The next step is to set up your SQLCommand.txt file.
A sample of which is provided already but in order to create your own the formatting is as follows:
  1. You need the keywords SELECT, FROM, and WHERE all on their own lines
  2. After SELECT you will want a list of features that will print out when the query locates the cars being searched for.
    a) The list of features is as follows: ID, Model, YearMake, Color, Price, Dealer
    ex. SELECT ID, Model, YearMake, Color 
  3. After FROM you will want to input the name of the database file
    ex. FROM car_inventory
    Accesses Data from car_inventory.txt
  4. After WHERE you will add conditional statements using:
    a) conditionals: '=', '!=', '>', '<', '<=', '>='
    b) logical operators: 'AND', 'OR'
    ex. WHERE Model="accord" AND YearMake="2015" AND (Color="blue" OR Color="gray")

  Full SQL Query Example:
  SELECT ID, Model, YearMake, Color 
  FROM car_inventory 
  WHERE Model="accord" AND YearMake="2015" AND (Color="blue" OR Color="gray");

Now you are ready to compile and run the program.
  1. make clean
    This will clean up any executables already present.
  2. make
    This will compile all helper files with the main and create an executable file named SerialDataBase.
  3. Run the executable on the SQL Queries found in SQLCommand.txt using the following command:
    ./SerialDataBase $(cat SQLCommand.txt)
    
