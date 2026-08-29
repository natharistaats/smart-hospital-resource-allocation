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


//Display doctor specialties
void displaySpecialties(void){
    char row1[20] = "ID";
    char row2[20] = "Specialty";
    char row3[10] = "Fee";
    char row4[25] = "Consultation Time(mins)";
    char row5[20] = "Daily Patient Cap";

    printf("\nDOCTOR SPECIALTIES\n");
    printf("------------------------------------------------------------------------------------------\n");
    printf("%-4s %-28s %-13s %-24s %s\n", row1, row2, row3, row4, row5);
    printf("------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < 4; i++)
    {
        printf("%-4d %-28s Rs. %-15.2f %-24d %-20d\n",
               specialtyID[i],
               specialtyName[i],
               baseFee[i],
               consultationTime[i],
               dailyPatientCap[i]);
    }

    printf("-------------------------------------------------------------------------------------------\n");
}


//Display hospital wards
void displayWards(void){
    char row1[20] = "ID";
    char row2[20] = "Ward Name";
    char row3[20] = "Daily Bed Rate";
    char row4[20] = "Bed Capacity";

    printf("\n\n\nHOSPITAL WARDS\n");
    printf("--------------------------------------------------------------------------\n");
    printf("%-4s %-28s %-24s %-14s\n", row1, row2, row3, row4);
    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < 4; i++)
    {
        printf("%-4d %-28s Rs. %-24.2f %d\n",
               wardID[i],
               wardName[i],
               dailyBedRate[i],
               bedCapacity[i]);
    }

    printf("--------------------------------------------------------------------------\n");
}


//Display bed availability
void displayBedAvailability(void)
{
    printf("\n\n\nBED AVAILABILITY\n");
    printf("-------------------------------------------------------------\n");
    printf("%-28s %-12s %-12s\n","Ward", "Capacity", "Available");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < 4; i++)
    {
        int availableBeds = 0;

        for (int j = 0; j < bedCapacity[i]; j++)
        {
            if (bedOccupancy[i][j] == 0)
            {
                availableBeds++;
            }
        }

        printf("%-28s %-12d %-12d\n",
               wardName[i],
               bedCapacity[i],
               availableBeds);
    }

    printf("-------------------------------------------------------------\n");
}



int main(void)
{
    printf("Smart Hospital & Resource Allocation System\n");
    displaySpecialties();
    displayWards();
    displayBedAvailability();

    return 0;
}