//Program for data extraction (maybe in the future)

#include<iostream>
#include<fstream>
#include<string>
#include<deque>
#include "strtk.hpp"

using namespace std;

int main()
{

ofstream nicedata;  //output file

string addresses_line;
deque<int> addresses_array;  //array of integers (will be filled with addresses)
ifstream c ("C:/Users/Massimiliano/OneDrive for Business/Multiphoton/Data/Singles_Coinc_columns.txt"); //file containing the wanted addresses
getline(c, addresses_line);
strtk::parse(addresses_line," ",addresses_array); //all the integers in the line are parsed into the array


long long int total_counts[addresses_array.size()]={0};  //array of integers (will be filled with counts for the whole datafile)

nicedata.open("C:/Users/Massimiliano/OneDrive for Business/Multiphoton/Data/wanted_data.txt", ios_base::app);
for(int j=0;j<addresses_array.size();j++) {     //write the addresses in the first line of the output file
    nicedata<<addresses_array[j]<<"\t";
    }
nicedata<<"\n";

int wrong_lines=0;  //counter of random measurements
int ok_lines=0;  //counter of proper measurements
string data_line;
ifstream f ("C:/Users/Massimiliano/OneDrive for Business/Multiphoton/Data/171115_HV_600mW_allPairs.dat");    //datafile
while(getline(f, data_line)) {
    deque<int> dataline_array;  //array of integers (will be filled with data)
    strtk::parse(data_line," ",dataline_array); //all the integers in the line are parsed into the array

    if (dataline_array[0]!=0)   {   //check that first element is 0
        wrong_lines++;
        continue;
        }
    bool line_check=true;   //will be used to check if the line is a valid measurement
    for (int l=1; l<dataline_array.size(); l++) {   //check if the line is valid
        if (dataline_array[l]>50000000)  {
            line_check=false;   //if it was a random measurement, we'll know!
            break;
            }
        }
    if (line_check && dataline_array[1]>100000) {    //extract measurements only if the line is valid
        for(int i=0;i<addresses_array.size();i++) { //one cycle per address wanted=column in the output file
//            nicedata<<dataline_array[addresses_array[i]]<<"\t"; //appends the desired columns from a line to the output file
            total_counts[i]+=dataline_array[addresses_array[i]]; //increment total counts by counts in the address in the selected line
            }
//        nicedata<<"\n";
        ok_lines++;
        }
    else wrong_lines++; //otherwise count the line as a wrong measurement
    }

nicedata<<"\n";

for(int k=0;k<addresses_array.size();k++) {     //write the total counts in the last line of the output file
    nicedata<<total_counts[k]<<"\t";
    }

nicedata<<"\nCorrect Measurements: "<<ok_lines;

nicedata<<"\nWrong Measurements: "<<wrong_lines;

}
