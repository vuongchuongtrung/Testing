/* Group Members:
* Ian Stafford			X00121946
* Chuong Trung Vuong	X00123743
*/

/***************
Usage: Based
Availability: http://www.cplusplus.com/forum/beginner/10292/
Date: 27/10/2016
****************/

#include <fstream>
#include <iostream>
#include <string>
#include <io.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iomanip>
#include <ostream>

#include <algorithm>
#include <vector>
#include <Windows.h>

#include "dirent.h"
#include "dirent.cpp"

using namespace std;

typedef struct
{
	string fName;
	string fExt;
	int cIf;
	int cFor;
	int cWhile;
	int cSwitch;
	int total;
	int noWord;
} similarityIndex_t;

/*
 * usage: used/modified
 * availability: http://stackoverflow.com/questions/10779195/sorting-struct-using-stlalgorithm
 * on 03/11/2016, at 11:30
 *
 * for all sort functions
 */
bool sortByfName(const similarityIndex_t &left, const similarityIndex_t &right);
bool sortByCountIf(const similarityIndex_t &left, const similarityIndex_t &right);
bool sortByCountFor(const similarityIndex_t &left, const similarityIndex_t &right);
bool sortByCountWhile(const similarityIndex_t &left, const similarityIndex_t &right);
bool sortByCountSwitch(const similarityIndex_t &left, const similarityIndex_t &right);
bool sortByCountTotal(const similarityIndex_t &left, const similarityIndex_t &right);
void printDetails(string &dir, vector<similarityIndex_t> &indexes, ostream &str, int &numFile);


void inputDir(string &dir);
void displayHeader();
void setColor(int value);
int getNoOfFile(string &dir, int &numFile);


int main()
{
	DIR *dp;
	struct dirent *dirp;
	struct stat filestat;
	ofstream outFile("report.txt");

	string stringWhile = "while";
	string stringFor = "for";
	string stringIf = "if";
	string stringSwitch = "switch";
	string candidate;

	ifstream fin;
	string dir;
	string filePath;
	string fileName;

	int i = 0; // index location of vector
	int numFile = 0;

	inputDir(dir); // read in an input directory
	system("cls");
	// after getting directory, determine how many files in the directory
	getNoOfFile(dir, numFile);
	// cout << numFile << endl; // used to test if it return correct number of file ==> correct

	// create vector that have number of file
	vector<similarityIndex_t> indexes(numFile);	

	dp = opendir(dir.c_str());
	if (dp == NULL)
	{
		cout << "Error(" << errno << ") opening " << dir << endl;
		return errno;
	}

	while ((dirp = readdir(dp)))
	{
		fileName = dirp->d_name;
		filePath = dir + "/" + fileName;

		// If the file is a directory (or is in some way invalid) skip it 
		if (stat(filePath.c_str(), &filestat))
		{
			continue;
		}
		if (S_ISDIR(filestat.st_mode))
		{
			continue;
		}
		// perform some iterative and selection count and display onto screen
		//similarityIndex(fin, filepath, fileName, id);

		int len = sizeof(fileName);
		string name;
		string extension;
		int index;
		int countFor = 0;
		int countWhile = 0;
		int countSwitch = 0;
		int countIf = 0;
		int wordCount = 0;

		for (int i = 0; i < len; i++)
		{
			if (fileName.at(i) == '.')
			{
				index = i;
				i = len;				
			}
		}
		name = fileName.substr(0, index);
		extension = fileName.substr(index, len);
		//cout << name << "             " << extension << endl; // testing only

		fin.open(filePath.c_str());
		if (fin.fail())
		{
			cout << "File opening failed!\n" << endl;
		}
		else
		{
			while (fin >> candidate) // for each candidate word read from the file 
			{
				wordCount++;
				if (candidate == stringIf)
				{
					countIf++;
				}
				else if (candidate == stringFor)
				{
					countFor++;
				}
				else if (candidate == stringWhile)
				{
					countWhile++;
				}
				else if (candidate == stringSwitch)
				{
					countSwitch++;
				}
			}			
		}
		indexes[i].fName = name;
		indexes[i].fExt = extension;
		indexes[i].cIf = countIf;
		indexes[i].cFor = countFor;
		indexes[i].cWhile = countWhile;
		indexes[i].cSwitch = countSwitch;
		indexes[i].total = countIf + countFor + countWhile + countSwitch;
		indexes[i].noWord = wordCount;
		i++;
		fin.close();
	}
	closedir(dp);
	
	/*
	 * usage: used/modified
	 * availability: http://stackoverflow.com/questions/10779195/sorting-struct-using-stlalgorithm
	 * on 03/11/2016, at 11:30
	 *
	 * for all sort functions
	 */
	//sort(indexes.begin(), indexes.end(), sortByfName);
	//sort(indexes.begin(), indexes.end(), sortByCountSwitch);
	//sort(indexes.begin(), indexes.end(), sortByCountWhile);
	//sort(indexes.begin(), indexes.end(), sortByCountFor);
	//sort(indexes.begin(), indexes.end(), sortByCountIf);
	sort(indexes.begin(), indexes.end(), sortByCountTotal);
	
	
	printDetails(dir, indexes, cout, numFile); // display to console
	printDetails(dir, indexes, outFile, numFile); // write to file

	//setColor(12);
	//for (int i = 0; i < numFile - 1; i++) // because (i + 1) is used in the loop
	//{
	//	if (indexes[i].total != 0 && (indexes[i].total == indexes[i + 1].total))
	//	{
	//		// do not want to display file 0 ==> (i+1)
	//		cout << "File " << (i+1)  << " is similar to file " << (i + 2) << endl;
	//	}
	//}

	//// reset color back to white
	//setColor(7);

	cout << endl; // print empty line
	//system("Color FO");
	system("pause");
	return 0;
}

void inputDir(string &dir)
{
	cout << "dir to get files of: " << flush;
	getline(cin, dir);  // gets everything the user ENTERs
	cout << "===========================================================================================" << endl;
}

void displayHeader()
{
	cout << setw(5) << "Id"
		<< setw(3) << ""
		<< left << setw(25) << "FileName"
		<< left << setw(10) << "Extension"
		<< right << setw(5) << "If"
		<< right << setw(6) << "For"
		<< right << setw(8) << "While"
		<< right << setw(9) << "Switch"
		<< right << setw(8) << "Total"
		<< right << setw(10) << "No.Word" << "\n"
		<< "==========================================================================================="
		<< endl;
}

/*
 * usage: used/modified
 * availability: http://www.cplusplus.com/forum/beginner/77879/
 * on 03/11/2016, at 13:30
 *
 * Color values:
 * 0 = Black			8 = Gray		
 * 1 = Blue				9 = Light Blue
 * 2 = Green			A = Light Green
 * 3 = Aqua				B = Light Aqua
 * 4 = Red				C = Light Red
 * 5 = Purple			D = Light Purple
 * 6 = Yellow			E = Light Yellow
 * 7 = White			F = Bright White
 */
void setColor(int value)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), value);
}

int getNoOfFile(string &dir, int &numFile)
{
	DIR *dp;
	struct dirent *dirp;
	struct stat filestat;
	string filePath;
	string fileName;
	
	dp = opendir(dir.c_str());
	if (dp == NULL)
	{
		cout << "Error(" << errno << ") opening " << dir << endl;
		return errno;
	}

	while ((dirp = readdir(dp)))
	{
		fileName = dirp->d_name;
		filePath = dir + "/" + fileName;

		// If the file is a directory (or is in some way invalid) skip it 
		if (stat(filePath.c_str(), &filestat))
		{
			continue;
		}
		if (S_ISDIR(filestat.st_mode))
		{
			continue;
		}
		numFile++;		
	}
	return numFile;
}

void printDetails(string &dir, vector<similarityIndex_t> &indexes, ostream &str, int &numFile)
{
	int id = 0;
	str << dir << "\n"
		<< "===========================================================================================" << "\n"
	    << setw(5) << "Id"
		<< setw(3) << ""
		<< left << setw(25) << "FileName"
		<< left << setw(10) << "Extension"
		<< right << setw(5) << "If"
		<< right << setw(6) << "For"
		<< right << setw(8) << "While"
		<< right << setw(9) << "Switch"
		<< right << setw(8) << "Total"
		<< right << setw(10) << "No.Word" << "\n"
		<< "==========================================================================================="
		<< endl;

	for (similarityIndex_t &sim : indexes)
	{
		str << right << setw(5) << ++id
			<< setw(3) << ""
			<< left << setw(25) << sim.fName
			<< left << setw(10) << sim.fExt
			<< right << setw(5) << sim.cIf
			<< right << setw(6) << sim.cFor
			<< right << setw(8) << sim.cWhile
			<< right << setw(9) << sim.cSwitch
			<< right << setw(8) << sim.total
			<< right << setw(10) << sim.noWord
			<< endl;
	}
	str << "===========================================================================================" << endl;
	setColor(12);
	for (int i = 0; i < numFile - 1; i++) // because (i + 1) is used in the loop
	{
		if (indexes[i].total != 0 && (indexes[i].total == indexes[i + 1].total))
		{
			// do not want to display file 0 ==> (i+1)
			str << "File " << (i + 1) << " is similar to file " << (i + 2) << endl;
		}
	}

	// reset color back to white
	setColor(7);
}



/*
 * usage: used/modified
 * availability: http://stackoverflow.com/questions/10779195/sorting-struct-using-stlalgorithm
 * on 03/11/2016, at 11:30
 *
 * for all sort functions
 */
bool sortByfName(const similarityIndex_t &left, const similarityIndex_t &right)
{
	return left.fName < right.fName;
}

bool sortByCountIf(const similarityIndex_t &left, const similarityIndex_t &right)
{
	return left.cIf < right.cIf;
}

bool sortByCountFor(const similarityIndex_t &left, const similarityIndex_t &right)
{
	return left.cFor < right.cFor;
}

bool sortByCountWhile(const similarityIndex_t &left, const similarityIndex_t &right)
{
	return left.cWhile < right.cWhile;
}

bool sortByCountSwitch(const similarityIndex_t &left, const similarityIndex_t &right)
{
	return left.cSwitch < right.cSwitch;
}

bool sortByCountTotal(const similarityIndex_t &left, const similarityIndex_t &right)
{
	return left.total < right.total;
}