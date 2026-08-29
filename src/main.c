#include <stdio.h>

// Doctor specialty lookup data
const int specialtyID[4] = {1, 2, 3, 4};

const char specialtyName[4][30] = {
    "General Practice (OPD)",
    "Paediatrics",
    "Cardiology",
    "Neurology"
};

const double baseFee[4] = {
    1500.00,
    2500.00,
    4500.00,
    5000.00
};

const int consultationTime[4] = {
    15,
    20,
    30,
    30
};

const int dailyPatientCap[4] = {
    30,
    20,
    12,
    10
};

//Hospital ward lookup data
const int wardID[4] = {1, 2, 3, 4};

const char wardName[4][30] = {
    "General Ward", 
    "Paediatric Ward",
    "Surgical Ward", 
    "ICU (Intensive Care Unit)"
};

const double dailyBedRate[4] = {
    3000.00,
    6000.00,
    12000.00,
    25000.00
};
const int bedCapacity[4] = {
    20,
    10,
    10,
    05
};

// Bed occupancy tracking
// 0 = Available
// 1 = Occupied
int bedOccupancy[4][20] = {0}; //initially set all beds available

int main(void)
{
    printf("Smart Hospital & Resource Allocation System\n");

    return 0;
}