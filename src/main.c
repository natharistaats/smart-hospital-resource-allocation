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

//billing data
double waitingTime[MAX_PATIENTS];
double emergencySurcharge[MAX_PATIENTS];
double wardCost[MAX_PATIENTS];
double totalGross[MAX_PATIENTS];
double discount[MAX_PATIENTS];
double finalPayable[MAX_PATIENTS];

int patientCount = 0;

//function prototypes
void displaySpecialties(void);
void displayWards(void);
void displayBedAvailability(void);
void patientIDgenerator(int index);
void registerPatient(int *patientCount);

double calculatingWaitingTime(int specialtyIndex);
double calculatingEmergencySurcharge(double basefee, int urgency);
double calculatingWardCost(int wardIndex, int days);
double calculatingTotalGross(double consultationFee, double surcharge, double wardCost);
double calculatingSubsidyDiscount(double gross, int age);

void calculatingBill(int index);
void displayPatientBill(int index);
void findPatient(void); //to help display the patient bill by the patient number
void displayPatientsByPriority(void);
void generateReport(void);

void saveBedStatus(void);
void loadBedStatus(void);
void savePatientRecord(int index);

//I add this extra function to make the smart hospital system works smoothely
//Patient discharge function
void dischargePatient(void); 


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

//calculating total gross
double calculatingTotalGross(double consultaionFee, double surcharge, double wardCost){
    return consultaionFee + surcharge + wardCost;
}


//calculating age subsidy discount
double calculatingSubsidyDiscount(double gross, int age){
    if(age < 5 || age > 65){
        return gross * 0.15;
    }

    return 0.0;
}

//===========================================================================================================
//calculating complete patient bill
void calculatingBill(int index){
    int speicaltyIndex;
    int wardIndex;

    speicaltyIndex = specialtyRequested[index] - 1;

    //waiting time
    waitingTime[index] = calculatingWaitingTime(speicaltyIndex);

    //emergency surcharge
    emergencySurcharge[index] = calculatingEmergencySurcharge(baseFee[speicaltyIndex], urgencyLevel[index]);

    //ward cost
    if(isAdmitted[index] == 1){
        wardIndex = patientWard[index] - 1;

        wardCost[index] = calculatingWardCost(wardIndex, daysAdmitted[index]);

    } else{
        wardCost[index] = 0.0;
    }

    // total gross
    totalGross[index] = calculatingTotalGross(baseFee[speicaltyIndex], emergencySurcharge[index], wardCost[index]);

    //age subsidy
    discount[index] = calculatingSubsidyDiscount(totalGross[index], patientAge[index]);

    //final payable amount
    finalPayable[index] = totalGross[index] - discount[index];
}

//==========================================================================================================================
//displaying patient bill

void displayPatientBill(int index){
    int specialtyIndex = specialtyRequested[index] - 1;

    printf("\n\n=============================================================\n");
    printf("        SMART HOSPITAL ADMISSION & BILL\n");
    printf("-------------------------------------------------------------\n");
    printf("Patient ID                 : PAT-%d\n", 1001 + index);
    printf("Patient Name               : %s\n", patientName[index]);

    if(patientAge[index] < 5 || patientAge[index] > 65){
        printf("Age                        : %d years (15%% Subsidy Eligible)\n", patientAge[index]);
    }
    else{
        printf("Age                        : %d years\n", patientAge[index]);
    }
    printf("Specialty                  : %s\n", specialtyName[specialtyIndex]);

    if(isAdmitted[index] == 1){
        int wardIndex = patientWard[index] - 1;
        printf("Assigned Ward              : %s (Bed #%02d)\n", wardName[wardIndex], assignedBed[index] + 1);
    }
    else{
        printf("Assigned Ward              : Outpatient\n");
    }

    if(urgencyLevel[index] == 1){
        printf("Urgency Level              : Level 1 (Normal)\n");
    }
    else if(urgencyLevel[index] == 2){
        printf("Urgency Level              : Level 2 (Urgent)\n");
    }
    else{
        printf("Urgency Level              : Level 3 (Critical)\n");
    }

    printf("-------------------------------------------------------------\n");

    printf("Base Concultation Fee      : LKR %10.2f\n", baseFee[specialtyIndex]);
    printf("Emergency Surcharge        : LKR %10.2f", emergencySurcharge[index]);

    if(urgencyLevel[index] == 2){
        printf(" (20%%)\n");
    }
    else if(urgencyLevel[index] == 3){
        printf(" (50%%)\n");
    }
    else{
        printf(" (0%%)\n");
    }

    if(isAdmitted[index] == 1){
        printf("Ward Stay Cost (%d Day%s)    : LKR %10.2f\n", daysAdmitted[index], daysAdmitted[index] == 1 ? "" : "s", wardCost[index]);
    }
    else{
        printf("Ward Stay Cost           : LKR %10.2f\n", wardCost[index]);
    }
    printf("------------------------------------------------------------\n");

    printf("Gross Total Bill           : LKR %10.2f\n", totalGross[index]);
    printf("Age Subsidy Discount       : LKR %10.2f", discount[index]);

    if(discount[index] > 0){ 
        printf(" (15%%)\n");
    }
    else{
        printf("\n");
    }
    printf("------------------------------------------------------------\n");

    printf("Final Payable Amount       : LKR %10.2f\n", finalPayable[index]);
    
    if(waitingTime[index] == 0){
        printf("Estimated Waiting Time     : %.2f mins (Immidiate Attention)\n", waitingTime[index]);
    }
    else{
        printf("Estimated Waiting Time     : %.2f mins\n", waitingTime[index]);
    }

    printf("============================================================\n");
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
    scanf(" %[^\n]", patientName[*patientCount]);

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

    calculatingBill(*patientCount);

    specialtyQcount[specialtyIndex]++;

    //calling the function to save the patient billing log in a file
    savePatientRecord(*patientCount);

    (*patientCount)++;

    printf("\nPatient successfully registered.\n");

    displayPatientBill(*patientCount - 1);

}

//Patient lookup menu to display patient bill
void findPatient(void){
    int patientID;
    int index;

    printf("\nEnter Patient ID: PAT-");
    scanf("%d", &patientID);

    index = patientID - 1001;

    if(index < 0 || index >= patientCount){
        printf("\nPatient not found.\n");
        return;
    }

    displayPatientBill(index);

}

//display patients by priority
void displayPatientsByPriority(void){
    int order[MAX_PATIENTS];

    if(patientCount == 0){
        printf("\nNo patients are registered.\n");
        return;
    }

    for(int i = 0; i < patientCount; i++){
        order[i] = i;
    }
    for(int i = 0; i < patientCount - 1; i++){
        for(int j = 0; j < patientCount - 1 - i; j++){
            if(urgencyLevel[order[j]] < urgencyLevel[order[j + 1]]){
                int temp = order[j];
                order[j] = order[j + 1];
                order[j + 1] = temp;
            }
        }
    }

    //displaying the patients by priority

    for(int i = 0; i < patientCount; i++){
        int index = order[i];

        printf("\nPatient ID: PAT-%d", 1001 + index);
        printf("\nName: %s", patientName[index]);
        printf("\nUrgency Level: %d", urgencyLevel[index]);
        printf("\nSpecialty: %s", specialtyName[specialtyRequested[index] - 1]);
        printf("\nFinal Payable: %.2f\n", finalPayable[index]);
    }
}

//for performance reports and analytics
void generateReports(void){

    //for total patients registered categorized by the urgency level
    int normalCount = 0;
    int urgentCount = 0;
    int criticalCount = 0;

    //for total revenue earned and total discount granted
    double totalRevenue = 0;
    double totalDiscount = 0;

    //to find highest paying patient
    int highestPayingPatientIndex = 0;
    double highestBill = 0.0;

    if(patientCount == 0){
        printf("\nNp patients are registered. No reports available.\n");
        return;
    }

    for(int i = 0; i < patientCount; i++){
        if(urgencyLevel[i] == 1){
            normalCount++;
        }
        else if(urgencyLevel[i] == 2){
            urgentCount++;
        }
        else if(urgencyLevel[i] == 3){
            criticalCount++;
        }

        //calculating total revenues and discounts
        totalRevenue += finalPayable[i];
        totalDiscount += discount[i];

        //to calculate highest pay
        if(finalPayable[i] > highestBill){
            highestBill = finalPayable[i];
            highestPayingPatientIndex = i;
        }
    }

    printf("\n======== URGENCY OF PATIENTS =======\n");
    printf("Normal     : %d\n", normalCount);
    printf("Urgent     : %d\n", urgentCount);
    printf("Critical   : %d\n", criticalCount);
    printf("Total      : %d\n", patientCount);

    printf("\n==== TOTAL REVENUES AND DISCOUNTS ====\n");
    printf("Total Revenue   : LKR %.2f\n", totalRevenue);
    printf("Total Discounts : LKR %.2f\n", totalDiscount);


    printf("\n============ BED OCCUPANCY ============\n");
    for(int i = 0; i < WARD_COUNT; i++){
        int occupiedBeds = 0;

        for(int j = 0; j < bedCapacity[i]; j++){
            if(bedOccupancy[i][j] == 1){
                occupiedBeds++;
            }
        }

        double occupancyPercentage = ((double)occupiedBeds / bedCapacity[i]) * 100;

        printf("%s : %d/%d beds occupied (%.2f%%)\n", wardName[i], occupiedBeds, bedCapacity[i], occupancyPercentage);
    }


    printf("\n======= HIGHEST PAYING PATIENT ========\n");
    printf("Patient Name : %s\n", patientName[highestPayingPatientIndex]);
    printf("Total Bill   : LKR %.2f\n", finalPayable[highestPayingPatientIndex]);

}

//file handeling 
//file handeling of bed occupancy 1
//saving beds temporarily
void saveBedStatus(void){
    FILE *file = fopen("beds_status.txt", "w");

    if(file == NULL){
        printf("Error. Could not save the bed status.\n");
        return;
    }

    for(int i = 0; i < WARD_COUNT; i++){
        for(int j = 0; j < bedCapacity[i]; j++){
            fprintf(file, "%d ", bedOccupancy[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);

    printf("\nBed status saved successfully.\n");
}

//file handeling of bed occupancy 2
//loading previous bed occupancy status
void loadBedStatus(void){
    FILE *file = fopen("beds_status.txt", "r");

    if(file == NULL){
        printf("No previous bed status was found. Starting with all beds available.\n");
        return;
    }

    for(int i = 0; i < WARD_COUNT; i++){
        for(int j = 0; j < bedCapacity[i]; j++){
            fscanf(file, "%d", &bedOccupancy[i][j]);
        }
    }

    fclose(file);

    printf("Previous bed status loaded successfully.\n");
}

//saving patients record in a file
void savePatientRecord(int index){
    FILE *file = fopen("patient_records.txt", "a");

    if(file == NULL){
        printf("Error. Could not save patient record.\n");
        return;
    }

    int specialtyIndex = specialtyRequested[index] - 1;

    fprintf(file, "\n=======================================\n");
    fprintf(file, "Patient ID        : PAT-%d\n", 1001 + index);
    fprintf(file, "Patient Name      : %s\n", patientName[index]);
    fprintf(file, "Age               : %d\n", patientAge[index]);
    fprintf(file, "Urgency Level     : %d\n", urgencyLevel[index]);
    fprintf(file, "Specialty         : %s\n", specialtyName[specialtyIndex]);

    if(isAdmitted[index] == 1){
        int wardIndex = patientWard[index] - 1;

        fprintf(file, "Ward              : %s\n", wardName[wardIndex]);
        fprintf(file, "Days Admitted     : %d\n", daysAdmitted[index]);
    }
    else{
        fprintf(file, "Ward            : Outpatient\n");
    }

    fprintf(file, "Gross Bill          : LKR %.2f\n", totalGross[index]);
    fprintf(file, "Discount            : LKR %.2f\n", discount[index]);
    fprintf(file, "Final Payable       : LKR %.2f\n", finalPayable[index]);

    fclose(file);
}

//discharge function
void dischargePatient(void){
    int patientID;
    int index;

    printf("\nEnter Patient ID: PAT-");
    scanf("%d", &patientID);

    index = patientID - 1001;

    if(index < 0 || index >= patientCount){
        printf("\nPatient not found.\n");
        return;
    }

    if(isAdmitted[index] == 0){
        printf("\nThis patient is an outpatient and has no assigned bed.\n");
        return;
    }

    int wardIndex = patientWard[index] - 1;
    int bedIndex = assignedBed[index];

    bedOccupancy[wardIndex][bedIndex] = 0;

    printf("\nPatient PAT-%d has beed discharge successfully.\n", patientID);
    printf("Released Bed : %s, Bed #%02\n", wardName[wardIndex], bedIndex + 1);

    isAdmitted[index] = 0;
}


//=================================================================================
int main(void)
{
    int choice;

    printf("Smart Hospital & Resource Allocation System\n");

    //loading previous bed status
    loadBedStatus();

    while(1){
        printf("\n===============================================\n");
        printf("              MAIN MENU\n");
        printf("===============================================\n");
        printf("1. Register Patient\n");
        printf("2. Display Patient Bill\n");
        printf("3. Display Patients by Priority\n");
        printf("4. Performance reports & Analytics\n");
        printf("5. Discharge Patient\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if(choice == 1){
            registerPatient(&patientCount);
        }
        else if(choice == 2){
            findPatient();
        }
        else if(choice == 3){
            displayPatientsByPriority();
        }
        else if(choice == 4){
            generateReports();
        }
        else if(choice == 5){
            dischargePatient();
        }
        else if(choice == 6){
            saveBedStatus(); //saving bed status file when exiting the program
            printf("You are exiting the system...\n");
            break;
        }
        else{
            printf("\nInvalid choice. Please select a valid choice.\n");
        }
    }

    printf("\nTotal registered patients: %d\n", patientCount);

    return 0;
}