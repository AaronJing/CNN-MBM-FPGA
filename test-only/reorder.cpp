#include <iostream>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <ctime>
#include <fstream>


using namespace std;

int main (){

	ifstream file("c5w.txt", ios::in);
	string line;
	string waste_buffer;
	double buffer[48000];

	if(file.is_open()){

		for(int i = 0; i < 48000; i++){
			getline(file, line);
			buffer[i] = stod(line);
		}
	}

	file.close();

	ofstream file_out("c5w-ro.txt");
	for (int i = 0; i < 120; i++){
		for (int j = 0; j < 16; j++){
			for (int k = 0; k < 25; k++){
				file_out << buffer[i*25+j*25*120+k] << "," << endl;
			}
		}
	}
	file_out.close();
	return 0;
}