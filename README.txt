Program Compilation and Execution Instructions:
  1. Make sure all files downloaded and in same directory
  2. TERMINAL COMMAND: gcc -o dataGen dataGen.c
  3. TERMINAL COMMAND: ./dataGen
This will populate a file containing a car database. 

The next step is to set up your SQLCommand.txt file.
A sample of which is provided already but in order to create your own the formatting is as follows:

  1. You need the keywords SELECT, FROM, and WHERE all on their own lines

  2. After SELECT you will want a list of features that will print out when the query locates the cars being searched for.
    a) The list of features is as follows (Case Sensitive): ID, Model, YearMake, Color, Price, Dealer
    ex. SELECT ID, Model, YearMake, Color 

  3. After FROM you will want to input the name of the database file
    ex. FROM car_inventory
    Accesses Data from car_inventory.txt

  4. After WHERE you will add conditional statements using:
    a) conditionals: '=', '!=', '>', '<', '<=', '>='
    b) conditionals should be sandwiched between a feature and an attribute, with the feature on the left and the attribute on the right. 
       Quotations around the attribute are not necassary and the feature must match the case sensitive list of features displayed in
       SQLCommand.txt setup section 2. There should be no whitespace between the feature, conditional, and attribute.
       The combination of a feature, conditional, and attribute will be referred to as a "qualification" in this document.
       EXAMPLE: Color="gray"
    c) logical operators: 'AND', 'OR'
    d) Parentheticals can be included to prioritize specific logical operations over others.
    e) Whitespace is only permitted on either end of a logical operator or qualification, but parenthetical seperation between a 
       logical operator and a qualification is permitted. Parenthetical seperation between two qualifications or two logical operators is not allowed.
       EXAMPLE: YearMake="2015" AND(Color="blue" OR Color="gray");
       Notice with the above example that "AND(Color='blue'" is permitted because the parenthesis acts as a seperator like a whitespace would between the logical operator and qualification
    f) Last character in the line containing the WHERE keyword must be a semicolon.
    EXAMPLE: WHERE Model="accord" AND YearMake="2015" AND (Color="blue" OR Color="gray");

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


Included Files:
Submitted files will be the main requested files: dataGen.c, OPESeq.c, OPEOMP.c, OPEMPI.c, Proj2.pdf, and this file (README.txt) meeting all requirements outlined in project
Additionally the following helper files will be submitted:
theDatabaser.c and .h : includes all functionality for creating a database from the txt file and freeing that database when necassary
structStack.c and .h : includes functionality for loading our custom structure into a stack to push struct elements on and pop top one off
stringStack.c and .h : includes functionality for loading strings into a stack to push strings on and pop top one off
searchUtility.c and .h : Pairs with OPESeq.c to search database in sequential
searchUtilityOMP.c and .h : Pairs with OPEOMP to search database in parallel using OpenMP
(Deprecated) searchUtilityMPI.c and .h : Pairs with OPEMPI to search database in parallel using MPI
parse.c and .h : Takes in SQLCommand.txt and breaks it down into parameters for various other helper functions including one that takes instructions following WHERE keyword and convert to postfix operations
QueryParsing.c and .h : Takes postfix operation and uses it to query database for cars meeting query requirements
SQLCommand.txt : Contains sample SQL Query but this one really isn't necassary. If you wish to use another .txt file, make sure it contains that formatted query and is in the same directory as everything else, then
                 refer to the execution command in section 3 of the instructions for compiling and running the program. Swap SQLCommand.txt with whatever txt file you are using.


Team Contributions:
Timmy:
  - Parsed query prompt into separate operations
  - Converted where operation from Infix to Postfix
  - Lead and designed large scale code structure
Kyle:
  - Created Code to generate Text file of cars
  - Wrote algorithms such as union, intersect and find functions
  - Made parallelized versions of these functions
Dustin:
  - Created Main and functions to call operations
  - Handled database memory and leaks
  - Debugged and handled linking of files and various compilation and runtime issues
  - Created Second Version of MPI
Olivia:
  - Tested and calculated time complexity of algorithms
  - Created Hashmap structure and functions
  - Created stack structure and functions
Nick:
  - Created structs and functions designed for reading in the database text file
  - Created Database print and free functions necessary for other programs
  - Handled small scale memory leaks


Brief Discussion:
In order to optimize performance we split the task into smaller bits of functionality and charged each member of the team with different functionality and to make sure that functionality was optimized to the extent that it could be.
Splitting the project up like this enabled each member of the team to hone in on optimizing the portion of the code that they had worked on.
Each portion of the code was responsible for sending information to another portion of the code. In order to ensure correctness once we assigned each team member with the functionality they were to work on we had team members who were
working on functionality that needed to communicate confer amongst themselves as to how that information should be communicated. 
For example:
If Function A needed to pass information X along to Function B, Timmy was working on Function A and Dustin was working on Function B, Timmy and Dustin would communicate to decide how to structure information X and communicate it from Function A to Function B
