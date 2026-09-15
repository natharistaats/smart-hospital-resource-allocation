#include <stdio.h>
#include <string.h>

#define MAX_PATIENTS 100
#define SPECIALTY_COUNT 4
#define WARD_COUNT 4
#define MAX_BEDS 20

// Doctor specialty lookup data
const int specialtyID[SPECIALTY_COUNT] = {1, 2, 3, 4};

const char specialtyName[SPECIALTY_COUNT][30] = {
    "General Practice (OPD)",
    "Paediatrics",
    "Cardiology",
    "Neurology"
};

const double baseFee[SPECIALTY_COUNT] = {
    1500.00,
    2500.00,
    4500.00,
    5000.00
};

const int consultationTime[SPECIALTY_COUNT] = {
    15,
    20,
    30,
    30
};

const int dailyPatientCap[SPECIALTY_COUNT] = {
    30,
    20,
    12,
    10
};



//Hospital ward lookup data
const int wardID[WARD_COUNT] = {1, 2, 3, 4};

const char wardName[WARD_COUNT][30] = {
    "General Ward", 
    "Paediatric Ward",
    "Surgical Ward", 
    "ICU (Intensive Care Unit)"
};

const double dailyBedRate[WARD_COUNT] = {
    3000.00,
    6000.00,
    12000.00,
    25000.00
};
const int bedCapacity[WARD_COUNT] = {
    20,
    10,
    10,
    05
};

// Bed occupancy tracking
// 0 = Available
// 1 = Occupied
int bedOccupancy[WARD_COUNT][MAX_BEDS] = {0}; //initially set all beds available
int specialtyQcount[SPECIALTY_COUNT] = {0}; //tracking patients that are currently in queue for each specialty


//patient data intake
char patientName[MAX_PATIENTS][50];
int patientAge[MAX_PATIENTS];
int urgencyLevel[MAX_PATIENTS];
int specialtyRequested[MAX_PATIENTS];
int isAdmitted[MAX_PATIENTS];
int patientWard[MAX_PATIENTS];
int daysAdmitted[MAX_PATIENTS];
int assignedBed[MAX_PATIENTS];

int patientCount = 0;

//function prototypes
void displaySpecialties(void);
void displayWards(void);
void displayBedAvailability(void);

void patientIDgenerator(int index);
void registerPatient(int *patientCount);


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

    for (int i = 0; i < SPECIALTY_COUNT; i++)
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

    for (int i = 0; i < WARD_COUNT; i++)
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

    for (int i = 0; i < WARD_COUNT; i++)
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

void patientIDgenerator(int index){
    printf("Patient ID : PAT-%d\n", 1001 + index);
}


//Calculating waiting time
double calculatingWaitingTime(int specialtyIndex){
    return specialtyQcount[specialtyIndex] * consultationTime[specialtyIndex];

}

//calculating emergency surcharge
double calculatingEmergencySurcharge(double basefee, int urgency){
    if (urgency == 1){
        return 0.0;
    }
    else if(urgency == 2){
        return basefee * 0.20;
    }
    else if(urgency == 3){
        return basefee * 0.50;
    }

    return 0.0;
}


//calculating ward cost
double calculatingWardCost(int wardIndex, int days){
    if(wardIndex == -1 || days <= 0){
        return 0.0;
    }

    return days * dailyBedRate[wardIndex];
}


//calculating age subsidy discount
double calculatingSubsidyDiscount(double gross, int age){
    if(age < 5 || age > 65){
        return gross * 0.15;
    }

    return 0.0;
}



//register patient inputss
void registerPatient(int *patientCount){
    if (*patientCount >= MAX_PATIENTS){
        printf("\nError: Maximum system capacity reached!\n");
        return;
    }

    printf("\n-------Patient Admission Portal------\n");

    patientIDgenerator(*patientCount);

    printf("Enter Patient Name: ");
    scanf(" %49[^\n]", patientName[*patientCount]);

    do
    {
        printf("Enter Patient Age: ");
        scanf("%d", &patientAge[*patientCount]);
        
        if(patientAge[*patientCount] < 0){
            printf("Invalid Age. Retry!");
        }

    } while(patientAge[*patientCount] < 0);


    do
    {
        printf("Enter Emergency/Triage Level (1 = Normal, 2 = Urgent, 3 = Critical): ");
        scanf("%d", &urgencyLevel[*patientCount]);

        if(urgencyLevel[*patientCount] < 1 || urgencyLevel[*patientCount] > 3){
            printf("Invalid urgency level. Choose 1, 2 or 3.\n");
        }

    } while (urgencyLevel[*patientCount] < 1 || urgencyLevel[*patientCount] > 3);


    
    //specialty selection loop
    int specialtyChoice = 0;
    int specialtyIndex;

    while(1){
        displaySpecialties();

        printf("Select Specialty ID (1 to 4): ");
        scanf("%d", &specialtyChoice);

        if(specialtyChoice < 1 || specialtyChoice > 4 ){
            printf("Invalid Specialty ID. Please choose between 1 and 4.\n");
            continue;
        }

    specialtyIndex = specialtyChoice - 1;

    if(specialtyQcount[specialtyIndex] >= dailyPatientCap[specialtyIndex]){
        printf("\nWarning: Daily patient cap reached for %s.\n", specialtyName[specialtyIndex]);
        printf("Please select another specialty.\n");
        continue;

    }

    break;
}

specialtyRequested[*patientCount] = specialtyChoice;


    //ward admission

    do
    { 
        printf("\nIs the Patient Admitted to a ward? (1 = Yes, 0 = No): \n");
        scanf("%d", &isAdmitted[*patientCount]);

        if(isAdmitted[*patientCount] != 0 && isAdmitted[*patientCount] != 1){
            printf("Invalid choice. Please enter 1 or 0.\n");
        }

    } while (isAdmitted[*patientCount] != 0 && isAdmitted[*patientCount] != 1);


    if(isAdmitted[*patientCount] == 1){
        int wardChoice;
        int wardIndex;
        int assignedBedIndex = -1;

        while(assignedBedIndex == -1){
            displayWards();

            printf("Select Ward ID (1 to 4): ");
            scanf("%d", &wardChoice);

            if(wardChoice < 1 || wardChoice > 4){
                printf("Invalid Ward ID. Please choose between 1 and 4.");
                continue;
            }

            wardIndex = wardChoice - 1;

            //finding first available bed
            for(int i = 0; i < bedCapacity[wardIndex]; i++){
                if(bedOccupancy[wardIndex][i] == 0){
                    assignedBedIndex = i;
                    break;
                }
            }

            if(assignedBedIndex == -1){
                printf("Selected ward is full.\n");
                printf("Please select another ward.\n");
            }
            else{
                patientWard[*patientCount] = wardChoice;
                assignedBed[*patientCount] = assignedBedIndex;

                //marking bed as occupied
                bedOccupancy[wardIndex][assignedBedIndex] = 1;
            }
        }


        //days admitted
        do
        {
            printf("Enter Number of days admitted: ");
            scanf("%d", &daysAdmitted[*patientCount]);

            if(daysAdmitted[*patientCount] <= 0){
                printf("Invalid. Days admitted must be greater than 0.\n");
            }

        } while (daysAdmitted[*patientCount] <= 0);

    }
    else{
        patientWard[*patientCount] = 0;
        daysAdmitted[*patientCount] = 0;
        assignedBed[*patientCount] = -1;
    }

    specialtyQcount[specialtyIndex]++;

    (*patientCount)++;

    printf("\nPatient successfully registered.\n");

}



int main(void)
{
    printf("Smart Hospital & Resource Allocation System\n");
    displaySpecialties();
    displayWards();
    displayBedAvailability();

    registerPatient(&patientCount);

    printf("\nTotal registered patients: %d\n", patientCount);

    return 0;
}