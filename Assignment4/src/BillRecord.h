/********************************************************************
 File name: BillRecord.h

 Purpose: The header file BillRecord.h provides an overview of the classes
 designed to accomplish tasks associated with the billing system.
 The BillRecord class provides a template for other classes-ElectBillRecord, GasBillRecord
 and PhoneBillRecord to inherit from.


 Date: 21 December 2018

 Author: Priya Aswani

 ID Number: 5925502

 Email: pa470@uowmail.edu.au

 ********************************************************************/
#ifndef BILLRECORD_H
#define BILLRECORD_H

#include <string>
using namespace std;

enum BillType {
	eElect, eGas, ePhone
};

class BillRecord {
public:
	virtual ~BillRecord() {
	}
	;
	BillRecord();
	bool ReadCustDetails(ifstream &fin);
	virtual bool ReadUsageInfo(ifstream &fin);
	void DisplayCustDetails();
	virtual void DisplayUsageInfo();
	void SetDiscount(double discount);
	string getSupplier();
	string getName();
	string getAddress();
	BillType getBType();
	bool operator<(const BillRecord& otherBillRecord) const;
	virtual void UpdateBalance()=0;
	void setBillAmount(double BillAmount);
	double getBillAmount();

protected:
	double AccountBalance;
	int DaysSinceLastReading;
	double discount;

private:
	BillType BType;
	string Supplier;
	string Name, Address;
	double BillAmount;
};

class ElectBillRecord: public BillRecord {
private:
	int previousReading;
	int currentReading;
	double rate1;
	double rate1Threshold;
	double rate2;
	double supplyCharge;

public:
	bool ReadUsageInfo(ifstream& fin) override;
	void DisplayUsageInfo() override;
	void UpdateBalance() override;
};

class GasBillRecord: public BillRecord {
private:
	int previousReading;
	int currentReading;
	double heatingValue;
	double rate;
	double supplyCharge;

public:
	bool ReadUsageInfo(ifstream& fin) override;
	void DisplayUsageInfo() override;
	void UpdateBalance() override;

};

class PhoneBillRecord: public BillRecord {
private:
	int numberOfLocalCalls;
	double localCallRate;
	int longDistanceCallTime;
	double longDistanceCallRate;
	double lineRental;

public:
	bool ReadUsageInfo(ifstream& fin) override;
	void DisplayUsageInfo() override;
	void UpdateBalance() override;
};

#endif

