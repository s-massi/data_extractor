//Program for data extraction (maybe in the future)

#include<iostream>
#include<fstream>
#include<string>
#include<deque>
#include "strtk.hpp"
#include<cmath>

using namespace std;

int main()
{

ofstream nicedata;  //output file
nicedata.open("C:/Users/Massimiliano/OneDrive for Business/Multiphoton/Data/wanted_data.txt", ios_base::app);

int addresses_array[65537];  //array of integers (will be filled with addresses)
int dim=0;
int index=0;
bool pol[8]={0,0,0,0,0,0,0,0};  //used for writing the polarisation state, size equal to number of photons
                for (int g4=0; g4<2; g4++, pol[3] = !pol[3]) {
                    for (int g3=0; g3<2; g3++, pol[2] = !pol[2]) {
                        for (int g2=0; g2<2; g2++, pol[1] = !pol[1]) {
                            for (int g1=0; g1<2; g1++, pol[0] = !pol[0]) {
                                index=8 * g4 + 4 * g3 + 2 * g2 + g1;
                                addresses_array[index] = pow(2,6+g4) + pow(2,4+g3) + pow(2,2+g2) + pow(2,g1);
                                for (int ii=0; ii<4; ii++) {    //ii<8 if whole polarization states needed
                                    if (pol[ii]) nicedata<<"V";
                                    else nicedata<<"H";
                                }
                                nicedata<<"\t";
                                dim++;
                            }
                        }
                    }
                }


string addresses_line;
ifstream c ("C:/Users/Massimiliano/OneDrive for Business/Multiphoton/Data/columns.txt"); //file containing the wanted addresses
while(getline(c, addresses_line)) {
    strtk::parse(addresses_line," ",addresses_array); //all the integers in the line are parsed into the array
    }

nicedata<<"\n";
long long int total_counts[dim]={0};  //array of integers (will be filled with counts for the whole datafile)

for(int j=0;j<dim;j++) {     //write the addresses in the first line of the output file
    nicedata<<addresses_array[j]<<"\t";
    }
nicedata<<"\n";

int wrong_lines=0;  //counter of random measurements
int ok_lines=0;  //counter of proper measurements
string data_line;
ifstream f ("C:/Users/Massimiliano/OneDrive for Business/Multiphoton/Data/231115_HV_470mW_allPairs.dat");    //datafile
while(getline(f, data_line)) {
    deque<int> dataline_array;  //array of integers (will be filled with data)
    strtk::parse(data_line," ",dataline_array); //all the integers in the line are parsed into the array
    int useless_lines=0;
    int element=0;
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
    if (line_check) {    //extract measurements only if the line is valid
        for (int p=0;p<dim;p++) {
            useless_lines+=dataline_array[addresses_array[p]];  //eliminate all-0 lines
            if (dataline_array[addresses_array[p]]>500000)   element++;
        }
        if (useless_lines!=0 && element==0)   {
            for(int i=0;i<dim;i++) { //one cycle per address wanted=column in the output file
                nicedata<<dataline_array[addresses_array[i]]<<"\t"; //appends the desired columns from EVERY LINE to the output file
                total_counts[i]+=dataline_array[addresses_array[i]]; //increment total counts by counts in the address in the selected line
                }
        nicedata<<"\n";
        }
        ok_lines++;
    }
    else wrong_lines++; //otherwise count the line as a wrong measurement
    }

nicedata<<"\n";

for(int k=0;k<4;k++) {     //write the total counts in the output file
    for (int k2=0; k2<4; k2++) {
        nicedata<<total_counts[4*k + k2]<<"\t";
    }
    nicedata<<"\n";
}

nicedata<<"\nCorrect Measurements: "<<ok_lines;

nicedata<<"\nWrong Measurements: "<<wrong_lines;

}


