/********************************************************************
 File name: BillSystem.h

 Purpose: The source code file BillSystem.h provides an overview of the class BillSystem
 designed to accomplish tasks associated with the billing system.

 Date: 21 December 2018

 Author: Priya Aswani

 ID Number: 5925502

 Email: pa470@uowmail.edu.au

 ********************************************************************/
#ifndef BILLSYS_H
#define BILLSYS_H

#include <iostream>
#include <vector>
#include <set>
#include "BillRecord.h"
using namespace std;

class BillSystem {
public:
	~BillSystem();
	bool ReadFile(char *fname);
	int GetNumRecs();
	void DisplayRec(int i);
	void CalDiscounts();
	void CalBills();
	void PrintReport();

private:
	vector<BillRecord*> BRecs;
	struct BillRecordComparator {
		bool operator()(BillRecord const* record1, BillRecord const* record2) {
			return *record1 < *record2;
		}
	};
	multiset<BillRecord*, BillRecordComparator> discountedCustomers;
};

#endif

