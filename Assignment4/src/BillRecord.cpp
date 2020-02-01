/********************************************************************
 File name: wordstats.cpp

 Purpose: The source code file BillRecord.cpp provides suitable implementation of the
 public subroutines listed in the supplied BillStats.h header file, and further adds to it. The
 said source file contains method definitions of the classes BillRecord, ElectBillRecord,
 GasBillRecord and PhoneBillRecord. The functionality and state of instances of the
 aforementioned classes is made use of in the BillSystem class.

 Date: 21 December 2018

 Author: Priya Aswani

 ID Number: 5925502

 Email: pa470@uowmail.edu.au

 ********************************************************************/
#include <iostream>
#include <fstream>
#include <iomanip>
#include "BillRecord.h"
using namespace std;

// Default constructor initialises instance variables to appropriate values
BillRecord::BillRecord() {
	BType = eElect;
	AccountBalance = 0.0;
	DaysSinceLastReading = 0;
	BillAmount = 0.0;
	Supplier = "no supplier";
	Address = "no address";
	Name = "no name";
	discount = 1.0;

}

//method to read the customer details common to all customer types
bool BillRecord::ReadCustDetails(ifstream &fin) {
	string recordType;
	fin >> recordType;
	if (recordType == "Phone") {
		BType = ePhone;
	} else if (recordType == "Gas") {
		BType = eGas;
	} else {
		BType = eElect;
	}

	fin >> Supplier;
	fin.ignore();
	getline(fin, Name);
	getline(fin, Address);
	fin >> AccountBalance;
	fin >> DaysSinceLastReading;

	return true;
}

//method to display customer details common to all customers
void BillRecord::DisplayCustDetails() {
	cout << left << setprecision(4) << setw(14) << fixed;
	switch (BType) {
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

	cout << setw(10) << Supplier << setw(17) << Name << setw(27) << Address
			<< setw(10) << right << AccountBalance << setw(6)
			<< DaysSinceLastReading << left << endl;

}

//virtual function meant to read and discard additional customer details depending on the
//type of the customer (or billing record). The function is overridden in the subclasses
//to provide for suitable behaviour and actually store the data into required fields
bool BillRecord::ReadUsageInfo(ifstream &fin) {
	double temp;

	switch (BType) {
	case ePhone:
		for (int counter = 1; counter <= 5; counter++) {
			fin >> temp;
		}
		break;

	case eGas:
		for (int counter = 1; counter <= 5; counter++) {
			fin >> temp;
		}
		break;

	case eElect:
		for (int counter = 1; counter <= 6; counter++) {
			fin >> temp;
		}
		break;

	}
	return true;
}

//virtual function meant to display customer usage information. It has been specified
//that the function does not do anything, and hence no implementation has been provided
void BillRecord::DisplayUsageInfo() {

}

//method to read in details for an electricity billing record
bool ElectBillRecord::ReadUsageInfo(ifstream& fin) {
	fin >> previousReading;
	fin >> currentReading;
	fin >> rate1;
	fin >> rate1Threshold;
	fin >> rate2;
	fin >> supplyCharge;

	return true;
}

//method to read in details for a gas billing record
bool GasBillRecord::ReadUsageInfo(ifstream& fin) {
	fin >> previousReading;
	fin >> currentReading;
	fin >> heatingValue;
	fin >> rate;
	fin >> supplyCharge;

	return true;
}

//method to read in details for a phone billing record
bool PhoneBillRecord::ReadUsageInfo(ifstream& fin) {
	fin >> numberOfLocalCalls;
	fin >> localCallRate;
	fin >> longDistanceCallTime;
	fin >> longDistanceCallRate;
	fin >> lineRental;

	return true;
}

//method to display details for an electricity billing record
void ElectBillRecord::DisplayUsageInfo() {
	cout << "(Readings: " << previousReading << "," << currentReading << "\t"
			<< "R1: " << rate1 << " R1Th: " << rate1Threshold << "\t" << "R2: "
			<< rate2 << " SuppC: " << supplyCharge << ")" << flush;
}

//method to display details for a gas billing record
void GasBillRecord::DisplayUsageInfo() {
	cout << "(Readings: " << previousReading << "," << currentReading << "\t"
			<< "HV: " << heatingValue << " Rate: " << rate << "\t" << "SuppC: "
			<< supplyCharge << ")" << flush;
}

//method to display details for a phone billing record
void PhoneBillRecord::DisplayUsageInfo() {
	cout << "(LCalls: " << numberOfLocalCalls << "," << localCallRate << "\t"
			<< "DCalls: " << longDistanceCallTime << "," << longDistanceCallRate
			<< "\t" << "Rental: " << lineRental << ")" << flush;
}

//method to return the Supplier
string BillRecord::getSupplier() {
	return Supplier;
}

//method to set the discount
void BillRecord::SetDiscount(double discount) {
	if (discount > 1 || discount < 0) {
		cerr << "Invalid discount! Program terminating." << endl;
		exit(1);
	}
	this->discount = discount;
}

//method to return the name of the customer
string BillRecord::getName() {
	return Name;
}

//method to return the address of the customer
string BillRecord::getAddress() {
	return Address;
}

//method to set the billing amount (used internally by the BillSystem class to appropriately
//set the private field billing amount
void BillRecord::setBillAmount(double BillAmount) {
	if (BillAmount < 0) {
		cerr << "Invalid billing amount! Program terminating." << endl;
		exit(1);
	}
	this->BillAmount = BillAmount;
}

//method to return the billing amount
double BillRecord::getBillAmount() {
	return BillAmount;
}

//method to return the billing record type
BillType BillRecord::getBType() {
	return BType;
}

//overloaded < operator to compare two BillRecord instances
bool BillRecord::operator<(const BillRecord& otherBillRecord) const {
	if (Name != otherBillRecord.Name)
		return Name < otherBillRecord.Name;
	return Address < otherBillRecord.Address;
}

//method to calculate and appropriately set the billing amount for an electricity billing record
void ElectBillRecord::UpdateBalance() {
	int C = currentReading = previousReading;
	double P = supplyCharge * DaysSinceLastReading;
	if (C <= rate1Threshold) {
		setBillAmount((C * rate1 + P) * discount - AccountBalance);
	}

	else {
		setBillAmount(
				(rate1Threshold * rate1 + (C - rate1Threshold) * rate2 + P)
						* discount - AccountBalance);
	}

	AccountBalance = 0;

}

//method to calculate and appropriately set the billing amount for a gas billing record
void GasBillRecord::UpdateBalance() {
	int C = currentReading = previousReading;
	double P = supplyCharge * DaysSinceLastReading;
	setBillAmount((C * heatingValue * rate) * discount - AccountBalance);
	AccountBalance = 0;
}

//method to calculate and appropriately set the billing amount for a phone billing record
void PhoneBillRecord::UpdateBalance() {
	double L = localCallRate * numberOfLocalCalls;
	double D = longDistanceCallRate * longDistanceCallTime;
	double P = lineRental * DaysSinceLastReading;
	setBillAmount((L + D + P) * discount - AccountBalance);
	AccountBalance = 0;
}

