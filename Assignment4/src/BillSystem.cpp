/********************************************************************
 File name: BillSystem.cpp

 Purpose: The source code file BillSystem.cpp provides suitable implementation of the
 public subroutines listed in the supplied BillSystem.h header file, and further adds to it. The
 class handles all anticipated functionality. Reading data into records of suitable type(s),
 calculating billing amounts, discounting records, and displaying them. It makes use of
 the classes BillRecord, ElectBillRecord, GasBillRecord and PhoneBillRecord internally to
 clinch all that it intends to.

 Date: 21 December 2018

 Author: Priya Aswani

 ID Number: 5925502

 Email: pa470@uowmail.edu.au

 ********************************************************************/
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include "BillSystem.h"
using namespace std;

//destructor to deallocate memory, previously allocated to store BillRecord instances
BillSystem::~BillSystem() {
	for (auto billRecordsIterator : BRecs) {
		delete billRecordsIterator;
	}
}

//method to read data from usage.txt file into instances of appropriate subclasses of the
//class BillRecord. Internally makes use of ReadCustDetails() and ReadUsageInfo() to read
//data into the specific instance
bool BillSystem::ReadFile(char *fname) {

	ifstream inFile;
	inFile.open(fname);
	if (!inFile.good()) {
		cerr << "Cannot find file! Program terminating." << endl;
		return false;
	}

	BillRecord* billRecordPointer;
	char serviceType;
	while (inFile.peek() != EOF) {
		serviceType = inFile.peek();
		switch (serviceType) {
		case 'P':
			billRecordPointer = new PhoneBillRecord();
			break;
		case 'E':
			billRecordPointer = new ElectBillRecord();
			break;
		case 'G':
			billRecordPointer = new GasBillRecord();
			break;
		}

		if (!billRecordPointer->ReadCustDetails(inFile))
			break;
		if (!billRecordPointer->ReadUsageInfo(inFile))
			break;
		string dummy;
		getline(inFile, dummy);
		getline(inFile, dummy);
		BRecs.push_back(billRecordPointer);
	}

	inFile.close();
	return true;
}

//method to return the number of instances in the BillSystem
int BillSystem::GetNumRecs() {
	return BRecs.size();
}

//method to display a specific BillRecord. Internally invokes DisplayCustDetails() and
//DisplayUsageInfo() for the instance concerned
void BillSystem::DisplayRec(int i) {
	BRecs[i]->DisplayCustDetails();
	BRecs[i]->DisplayUsageInfo();
	cout << endl;
}

//method to calculate discount for each billing record. Customers of Dodo and Alinta, who
//subscribe to all three services from either of the mentioned service providers, enjoy a
//15% and 20% discount respectively. The method sets the discount to a valid percent
void BillSystem::CalDiscounts() {

	int alintaDiscountedCustomers = 0;
	int dodoDiscountedCustomers = 0;

	for (auto billRecordsIterator : BRecs) {
		if (billRecordsIterator->getSupplier() == "Alinta"
				|| billRecordsIterator->getSupplier() == "Dodo") {
			discountedCustomers.insert(billRecordsIterator);
		}
	}

	for (auto discountedCustomersIterator = discountedCustomers.begin();
			discountedCustomersIterator != discountedCustomers.end();) {
		multiset<BillRecord*>::iterator lowerBound =
				discountedCustomers.lower_bound(*discountedCustomersIterator);
		multiset<BillRecord*>::iterator upperBound =
				discountedCustomers.upper_bound(*discountedCustomersIterator);

		int count = distance(lowerBound, upperBound);
		if (count != 3) {
			discountedCustomersIterator = discountedCustomers.erase(lowerBound,
					upperBound);
		} else {
			if ((*discountedCustomersIterator)->getSupplier() == "Alinta") {
				alintaDiscountedCustomers++;
				for (int counter = 0; counter < count; counter++) {
					(*discountedCustomersIterator)->SetDiscount(0.8);
					discountedCustomersIterator++;
				}
			} else {
				dodoDiscountedCustomers++;
				for (int counter = 0; counter < count; counter++) {
					(*discountedCustomersIterator)->SetDiscount(0.85);
					discountedCustomersIterator++;
				}
			}

		}

	}

	cout << "Number of discounted customers for Alinta: "
			<< alintaDiscountedCustomers << endl;
	cout << "Number of discounted customers for Dodo: "
			<< dodoDiscountedCustomers << endl;
	cout << endl << endl;

}

//method to calculate and set the billing amount for each record by invoking the UpdateBalance()
//method for each instance in the BillSystem
void BillSystem::CalBills() {
	for (auto BillRecordsIterator : BRecs) {
		BillRecordsIterator->UpdateBalance();
	}
}

//method to display five BillRecord instances with their respective billing amounts. It also
//displays the name and addresses of discounted customers of Dodo and Alinta
void BillSystem::PrintReport() {
	cout << setw(5) << "#" << setw(14) << "Service" << setw(10) << "Supplier"
			<< setw(17) << "Name" << setw(27) << "Address" << setw(10) << right
			<< "BillAmt" << left << endl << endl;
	for (int i = 0; i < 5; i++) {
		cout << setw(5) << i + 1 << flush;
		cout << left << setprecision(4) << setw(14) << fixed;
		switch (BRecs[i]->getBType()) {
		case ePhone:
			cout << "Phone" << flush;
			break;
		case eGas:
			cout << "Gas" << flush;
			break;
		case eElect:
			cout << "Electricity" << flush;
			break;
		}

		cout << setw(10) << BRecs[i]->getSupplier() << setw(17)
				<< BRecs[i]->getName() << setw(27) << BRecs[i]->getAddress()
				<< setw(10) << right << BRecs[i]->getBillAmount() << left
				<< endl;
		cout << endl;
	}

	cout << "Dodo & Alinta\'s Discounted Customers" << endl << endl;
	cout << setw(5) << "#" << setw(10) << "Supplier" << setw(17) << "Name"
			<< setw(27) << "Address" << endl << endl;
	int counter = 1;
	for (auto discountedCustomersIterator = discountedCustomers.begin();
			discountedCustomersIterator != discountedCustomers.end();
			counter++) {
		cout << setw(5) << counter << setw(10)
				<< (*discountedCustomersIterator)->getSupplier() << setw(17)
				<< (*discountedCustomersIterator)->getName() << setw(27)
				<< (*discountedCustomersIterator)->getAddress() << endl << endl;
		discountedCustomersIterator = discountedCustomers.upper_bound(
				*discountedCustomersIterator);

	}
}
