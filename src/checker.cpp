//
//  checker.cpp
//  ch (checker tool)
//
//  Created by yuz.chen on 6/23/22.
//

#include <iostream>
#include <string>
#include <fstream>
#include <ctype.h> // using for ::isspace
#include <algorithm> // using for std::remove_if()
#define VERSION "v 1.1.0 Beta"
#define AUTHOR "Yuz.Chen(Yuzhuang Chen)"

/* if under windows system, will include <direct.h> from libray
   else under unix system, will include <unistd.h> from libray 
   PATH_SIGN using for */
#ifdef WINDOWS
    #include <direct.h>
    #define PATH_SIGN "\\"
#else
    #include <unistd.h>
    #define PATH_SIGN "/"
#endif

// open input file and check if input file is found
void openInputFile(std::ifstream& inputFile, std::string& fileName);
//remove all the space from the string
void removeSpace(std::string& str);
//close both input file stream
void inputFileStreamClose(std::ifstream& expectedIN, std::ifstream& myoutputIN);
//getting the driectory with assgin to the parameter resultPath
void getCurrentDriectory(std::string& resultPath);
// check if path starting with the home dri
bool startWithHomeDriectory(const std::string& path);
// print the man Manu
void printMan();
// print the ch tool version info
void printVersion();
// NOTE: Core code; to check two file contents if is the same or not
int CoreChecker(int argc, char **argv);

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        std::cout << "check: no file arguments" << std::endl;
        return 0;
    }

    if (argc <= 2) {
        int result = getopt(argc, argv, "-mv");

        if (result != -1) {
            switch (result)
            {
            case 'm':
                printMan();
                break;
            
            case 'v':
                printVersion();
                break;

            case '?':
                std::cout << "having Unidentifiable operation parameter." << std::endl;
                break;
            }
        } else {
            std::cout << "check: needs two file argument" << std::endl;
        }

        return 0;
    }

    return CoreChecker(argc, argv);
}

int CoreChecker(int argc, char **argv) {
    std::string expectedINFile, myoutputINFile;
    expectedINFile = std::string(argv[argc - 2]);
    myoutputINFile = std::string(argv[argc - 1]);
    
    std::ifstream expectedIN;
    openInputFile(expectedIN, expectedINFile);
    if (!expectedIN) {
        std::cout << "input file1 error : File not Found error" << std::endl;
        exit(-1);
    }

    std::ifstream myoutputIN;
    openInputFile(myoutputIN, myoutputINFile);
    if (!myoutputIN) {
        std::cout << "input file2 error : File not Found error" << std::endl;
        exit(-1);
    }

    std::cout << "result is -> ";

    // prepar variable for loop 
    std::string expectedinput, myoutputinput;
    int counterLine = 1;
    // check each line in both file, to see if is correct
    while (!expectedIN.eof() && !myoutputIN.eof()) {
        getline(expectedIN, expectedinput);
        getline(myoutputIN, myoutputinput);

        removeSpace(expectedinput);
        removeSpace(myoutputinput);

        if (expectedinput != myoutputinput) {
            std::cout << "false : line " << std::to_string(counterLine) << std::endl;
            inputFileStreamClose(expectedIN, myoutputIN);
            return 0;
        }

        counterLine++;
    }

    // if one of the file is not reach the end, then reast are error
    if (!expectedIN.eof() || !myoutputIN.eof()) {
        std::cout << "false : line : " << counterLine << " and keep on" << std::endl;
        inputFileStreamClose(expectedIN, myoutputIN);
        return 0;
    }

    std::cout << "true : all line Match" << std::endl;
    inputFileStreamClose(expectedIN, myoutputIN);
    return 0;
}

// open input file and check if input file is found
void openInputFile(std::ifstream& inputFile, std::string& fileName) {
    bool flagHomeDri = startWithHomeDriectory(fileName);

    std::string fullPath;

    if (flagHomeDri) {
        fullPath = fileName;
    } else {
        getCurrentDriectory(fullPath);
        if (std::to_string(fileName.at(0)) != PATH_SIGN) {
            fullPath.append(PATH_SIGN);
        }
        fullPath.append(fileName);
    }

    inputFile.open(fullPath, std::ios::in);
}

//close both input file stream
void inputFileStreamClose(std::ifstream& expectedIN, std::ifstream& myoutputIN) {
    expectedIN.close();
    myoutputIN.close();
}

//remove all the space from the string
void removeSpace(std::string& str) {
    str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
}

//getting the driectory with assgin to the parameter resultPath
void getCurrentDriectory(std::string& resultPath) {
    char result[PATH_MAX]; //PATH_MAX the max bytes for storage the path name
    getcwd(result, sizeof(result)); //cwd = Current Working Dri
    resultPath = std::string(result);
}

// check if path starting with '~'
bool startWithHomeDriectory(const std::string& path) {
    std::string homePath = getenv("HOME"); // env = Environment Variables
    if (path.find(homePath) != std::string::npos) {
        return true;
    }
    return false;
}

void printMan() {
    std::cout << "NAME\n\tch - chcek file contents\n\n"
              << "SYNOPSIS\n"
              << "\tch File_Path1 File_Path2\n"
              << "\tch [-v | -m]\n\n"
              << "DESCRIPTION\n"
              << "\tIn the first form, the ch tool will check whether the contents of the two files, File_Path1 and File_Path2, are the same. The default comparison process excludes all spaces that appear in the content, but not the newline character '\\n'.\n\n"
              << "\tIn its second form, the ch tool's operation will operate on the command or on the path depending on the operation parameters.\n\n"
              << "\t\tNOTE: A command operation can have at most one operation parameter at a time. if more then 1 operation parameter, the first parameter is considered as the operation parameter of the current command\n\n"
              << "\tThe following options are available:\n\n"
              << "\t-v\tDisplay the version of ch command\n\n"
              << "\t-m\tDisplay the help manu of the command; more info, @see README\n\n" 
              << "\tIt is an error occurs if the target file or directory does not exist, and an error message is output.\n\n"
              << "EXIT STATUS\n"
              << "\tThe command \"mv File_Path1 File_Path2\" will be output with an error message.\n"
              << "\t\tFile not Found error\n\n"
              << "EXAMPLE\n"
              << "\tTo check file boo and far if the contents are the same if both file are exists:\n\n"
              << "\t\t$ch boo far\n\n"
              << std::endl; 
}

void printVersion() {
    std::cout << "ch" << VERSION << "\n\n" 
              << "Author: " << AUTHOR 
              << std::endl;
}