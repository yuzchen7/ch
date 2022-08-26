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
#include <time.h>
#define VERSION "v 1.4.0 Beta"
#define AUTHOR "Yuz.Chen(Yuzhuang Chen)"

/* if under windows system, will include <direct.h> from libray
   and dependency source, else under unix system, will
   include <unistd.h> from libray. PATH_SIGN use for modify 
   the path. HOME_PATH use in getenv function to get the home
   path. */
#ifdef _WIN32
    #include <direct.h> // _getcwd()
    #define getCurrentDri _getcwd
    #include "../Dependency/wingetopt/src/getopt.h" // getopt()
    #define PATH_SIGN "\\"
    #define HOME_PATH "PATH"
#else
    #include <unistd.h> // getcwd() getopt()
    #define getCurrentDri getcwd
    #define PATH_SIGN "/"
    #define HOME_PATH "HOME"
#endif

// open input file and check if input file is found
void openInputFile(std::ifstream& inputFile, std::string& fileName);

// remove all the space from the string
void removeSpace(std::string& str);

// remove the sub-string from a string if and only if subStr is exits in the string
void removeSubString(std::string& str, const std::string& subStr);

// close both input file stream
void inputFileStreamClose(std::ifstream& expectedIN, std::ifstream& myoutputIN);

// getting the driectory with assgin to the parameter resultPath
void getCurrentDriectory(std::string& resultPath);

// check if path starting with the home dri
bool startWithHomeDriectory(const std::string& path);

// print the man Manu
void printMan();

// print the ch tool version info
void printVersion();

// print the ch tool usage line for tell the user command formatting
int printUsage();

// NOTE: Core code; to check two file contents if is the same
int CoreChecker(int argc, char **argv, bool isIgnoreSpace, const std::string& subStringIgnore);


int main(int argc, char *argv[]) {
    // time_t start, end;
    // time(&start);

    std::string lastArgument = std::string(argv[argc - 1]);
    if (argc <= 1 || (argc >= 3 && lastArgument.find("-") != std::string::npos)) {
        return printUsage();
    }

    bool isIgnoreSpace = false;
    int optResult = -1;
    std::string subStringIgnore = "";

    if (argc <= 2) {
        if ((optResult = getopt(argc, argv, "mv")) != -1) {
            switch (optResult) {
            case 'm':
                printMan();
                break;
            
            case 'v':
                printVersion();
                break;

            case '?':
                std::cout << "having Unidentifiable operation parameter." << std::endl;
                printUsage();
                return 1;
            }
            return 0;
        }

    } else if (argc >= 4) {
        while ((optResult = getopt(argc, argv, "c:s")) != -1) {
            switch (optResult) {
            case 'c' :
                subStringIgnore = optarg;
                break;

            case 's': 
                isIgnoreSpace = !isIgnoreSpace;
                break;

            case '?':
                std::cout << "having Unidentifiable operation parameter." << std::endl;
                printUsage();
                return 1;
            }
        }
    }

    int result = CoreChecker(argc, argv, isIgnoreSpace, subStringIgnore);

    // time(&end);
    // std::cout << "run with: " << difftime(end, start) << " seconds" << std::endl;

    return result;
}

int CoreChecker(int argc, char **argv, bool isIgnoreSpace, const std::string& subStringIgnore) {
    std::string expectedINFile, myoutputINFile;

    expectedINFile = std::string(argv[argc - 2]);
    std::ifstream expectedIN;
    openInputFile(expectedIN, expectedINFile);
    if (!expectedIN) {
        std::cout << "input file1 error : File not Found error" << std::endl;
        exit(-1);
    }

    myoutputINFile = std::string(argv[argc - 1]);
    std::ifstream myoutputIN;
    openInputFile(myoutputIN, myoutputINFile);
    if (!myoutputIN) {
        std::cout << "input file2 error : File not Found error" << std::endl;
        exit(-1);
    }

    std::cout << "result is -> ";

    // prepar variable for loop 
    std::string expectedInput, myOutputInput;
    int counterLine = 1;
    // check each line in both file, to see if is correct
    while (!expectedIN.eof() && !myoutputIN.eof()) {
        getline(expectedIN, expectedInput);
        getline(myoutputIN, myOutputInput);

        if (isIgnoreSpace) {
            removeSpace(expectedInput);
            removeSpace(myOutputInput);
        }

        if (!subStringIgnore.empty()) {
            if (expectedInput.find(subStringIgnore) == myOutputInput.find(subStringIgnore)) {
                continue;
            } else {
                removeSubString(expectedInput, subStringIgnore);
                removeSubString(myOutputInput, subStringIgnore);
            }
        }

        if (expectedInput != myOutputInput) {
            std::cout << "false : line " << std::to_string(counterLine) << std::endl;
            inputFileStreamClose(expectedIN, myoutputIN);
            return 0;
        }

        counterLine++;
    }

    // if one of the file is not reach the end, then the rest are error
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
        // Modify path: change the relative path to full path  with dynamically
        getCurrentDriectory(fullPath);
        if (std::to_string(fileName.at(0)) != PATH_SIGN) {
            fullPath.append(PATH_SIGN);
        }
        fullPath.append(fileName);
    }

    inputFile.open(fullPath, std::ios::in);
}

// close both input file stream
void inputFileStreamClose(std::ifstream& expectedIN, std::ifstream& myoutputIN) {
    expectedIN.close();
    myoutputIN.close();
}

// remove all the space from the string
void removeSpace(std::string& str) {
    str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
}

// remove the sub-string from a string if and only if subStr is exits in the string
void removeSubString(std::string& str, const std::string& subStr) {
    if (subStr.empty() || str.find(subStr) == std::string::npos) {
        return;
    }
    
    int index = 0;
    while ((index = str.find(subStr)) != std::string::npos) {
        str.erase(index, subStr.size());
    }
}

// getting the driectory with assgin to the parameter resultPath
void getCurrentDriectory(std::string& resultPath) {
    char result[PATH_MAX]; //PATH_MAX the max bytes for storage the path name
    // getCurrentDri = getcwd / _getcwd @see #ifdef _WIN32
    getCurrentDri(result, sizeof(result)); //cwd = Current Working Dri
    resultPath = std::string(result);
}

// check if path starting with HOME_PATH '~'
bool startWithHomeDriectory(const std::string& path) {
    std::string homePath = std::getenv(HOME_PATH); // env = Environment Variables
    if (path.find(homePath) != std::string::npos) {
        return true;
    }
    return false;
}

// print the man Manu
void printMan() {
    std::cout << "NAME\n\tch - chcek file contents\n\n"
              << "SYNOPSIS\n"
              << "\tch [-cs] source_file_1 source_file_2\n"
              << "\tch [-v | -m]\n\n"
              << "DESCRIPTION\n"
              << "\tIn the first form, the ch tool will check whether the contents of the two files, File_Path1 and File_Path2, are the same. The default comparison process excludes all spaces that appear in the content, but not the newline character '\\n'.\n\n"
              << "\tIn its second form, the ch tool's operation will operate on the command or on the path depending on the operation parameters.\n\n"
              << "\tThe following options are available:\n\n"
              << "\t-v\tDisplay the version of ch command\n\n"
              << "\t-m\tDisplay the help manu of the command; more info, @see README\n\n" 
              << "\t-s\tCompare the two file without(Ignore) the space\n\n"
              << "\t-c\tCompare the two file without(Ignore) the Substring\n\n" 
              << "\tIt is an error occurs if the target file or directory does not exist, and an error message is output.\n\n"
              << "EXIT STATUS\n"
              << "\tThe command \"mv File_Path1 File_Path2\" will be output with an error message.\n"
              << "\t\tFile not Found error\n\n"
              << "EXAMPLE\n"
              << "\tTo check file boo and far if the contents are the same if both file are exists:\n\n"
              << "\t\t$ch boo far\n\n"
              << std::endl; 
}

// print the ch tool version info
void printVersion() {
    std::cout << "ch " << VERSION << "\n\n" 
              << "Author: " << AUTHOR 
              << std::endl;
}

// print the ch tool usage line for tell the user command formatting
int printUsage() {
    std::cout << "usage: ch [-cs] source_file_1 source_file_2\n"
                 "       ch [-m | -v]"
              << std::endl;
    return -1;
}