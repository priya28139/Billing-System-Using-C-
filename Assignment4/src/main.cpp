/********************************************************************
 File name: main.cpp

 Purpose: The source code file main.cpp provides an environment for
 the actual utilisation of the sub-routines implemented in the source code and
 header files included in the package, meant for managing a billing system

 Date: 21 December 2018

 Author: Priya Aswani

 ID Number: 5925502

 Email: pa470@uowmail.edu.au

 ********************************************************************/
#include <iostream>
#include <iomanip>
#include "BillSystem.h"
using namespace std;

int main() {

	BillSystem BS;

	cout << "Begin tests for BillSystem\n\n";

	if (!BS.ReadFile("usage.txt")) {
		cout << "File not found!\n\n";
		exit(1);
	}
	int n = BS.GetNumRecs();

	cout << "Num records read: " << n << endl << endl;
	cout << left;

	cout << setw(5) << "#" << setw(14) << "Service" << setw(10) << "Supplier"
			<< setw(17) << "Name" << setw(27) << "Address" << setw(10) << right
			<< "AccBal" << setw(6) << "Days" << left << endl << endl;
	for (int i = 0; i < n && i < 10; i++) {
		cout << setw(5) << i + 1 << flush;
		BS.DisplayRec(i);
		cout << endl;
	}

	BS.CalDiscounts();

	BS.CalBills();
	BS.PrintReport();

	cout << endl << "End tests for BillSystem\n";

	return 0;
}

