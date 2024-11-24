#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <math.h>

#define NRM  "\x1B[0m"
#define RED  "\x1B[31m"
#define YLW  "\x1B[33m"
#define MGN  "\x1B[35m"
#define CYN  "\x1B[36m"


const char* male = "Male";
const char* fmale = "Female";

typedef struct {
    char nic[20];
    int status;
    char name[20];
    int age;
    char gender[10];
    char admissionDate[20];
} Patient;

char ui (int *recordCount, int patientCount) {
    // The User Interface of the Program with instructions
    char option;

    printf("%s----------------------------------------\n", CYN);
    printf("%s   COVID-19 PATIENT MANAGEMENT SYSTEM   \n", CYN);
    printf(" * * * * * * * * * * * * * * * * * * * *\n");
    printf("#Records: %d | #Patients: %d | #Removed: %d\n", *recordCount, patientCount, *recordCount-patientCount);
    printf("%s----------------------------------------\n", CYN);
    printf("%s[+]%s: Add a new patient record\n", MGN, YLW);
    printf("%s----------------------------------------\n", CYN);
    printf("%s[-]%s: Delete a patient record\n", MGN, YLW);
    printf("%s----------------------------------------\n", CYN);
    printf("%s[*]%s: Change a record\n", MGN, YLW);
    printf("%s----------------------------------------\n", CYN);
    printf("%s[P]%s: Print a patient record\n", MGN, YLW);
    printf("%s----------------------------------------\n", CYN);
    printf("%s[A]%s: Print all the patient records\n", MGN, YLW);
    printf("%s----------------------------------------\n", CYN);
    printf("%s[Q]%s: Delete all record and quit\n", MGN, YLW);
    printf("%s----------------------------------------\n", CYN);
    printf("%s              ************              \n", CYN);
    printf("%s            ENTER THE OPTION            \n", CYN);
    printf("%s                   ", YLW);

    scanf(" %c", &option);      // Taking the operation Command

    if (tolower(option) == 'q') {  
        // For terminating the program 
        return option;
    }
    
    for (int i = 0; i < 3; i++) {
        printf("%s                   .\n", CYN);
    }
    printf("%s * * * * * Request Processing * * * * * %s\n", CYN, NRM);

    return option;      // Returning the operation command
}

int checkRepeat (Patient* list, char *nic, int *index, int recordCount) {
    int cmd, check = 0;
    for (int i = 0; i < recordCount; i++) {
        if (strcmp(list[i].nic, nic) == 0) {
            *index = i;     // Keeping the repeated patinet index
            if (list[i].status == 1) {
                // If the patient is marked as admitted
                printf(">>> %sThis Patient is already in the records\n", MGN);
                printf("%sRemove the patient record before making any changes\n", CYN);
                return 0;
            }
            else {
                // If the Patient was admitted and then removed later
                printf(">>> %sThis Patient was removed from the records recently.\n", MGN);
                printf("%sDo you want to [0] Abort?\n", CYN);
                printf("               [1] Renew the record?\n");
                printf("               [2] Overwrite the record? \n");
                printf("Choose an option(0 / 1 / 2): ");
                scanf("%d", &cmd);
                if (cmd == 0) {
                    printf(">>> %sABORTED.\n", MGN);
                }
                return cmd;
            }
        }
    }
    return 3;   // When not a repeatetion
}

int changeCheck (Patient* list, char *nic, int *index, int recordCount) {
    int cmd, check = 0;
    for (int i = 0; i < recordCount; i++) {
        if (strcmp(list[i].nic, nic) == 0) {
            *index = i;     // Keeping the repeated patinet index
            printf("%sDo you want to [0] Abort?\n", CYN);
            printf("               [1] Renew the record if removed?\n");
            printf("               [2] Overwrite the record? \n");
            printf("               [4] Delete and add new? \n");
            printf("Choose an option(0 / 1 / 2 / 4): ");
            scanf("%d", &cmd);
            if (cmd == 0) {
                printf(">>> %sABORTED.\n", MGN);
            }
            return cmd;
        }
    }
    printf("%s>>> %sRecord does not exist.\n", CYN, MGN);
    return 0;   // When not a repeatetion
}

void printPatient(Patient patient) {
    printf("Name: %s\n", patient.name);
    printf("NIC: %s\n", patient.nic);
    printf("Age: %d\n", patient.age);
}

int add (Patient* list, int *recordCount, int patientCount, int repeat) {
    char gender, nic[20];
    int index = 0, records = *recordCount;

    Patient patient;
    patient.status = 1;     //Setting up the status of the patient as 'Admitted'


    // Taking details for the new record
    printf("%sEnter Patient's NIC no: %s", CYN, YLW);
    scanf("%s", nic);

    // Checking if a patient with the same NIC is already in the records
    if (repeat == 0) {
        // For adding a record
        repeat = checkRepeat(list, nic, &index, *recordCount);
    }
    else {
        // For changing a record
        repeat = changeCheck(list, nic, &index, *recordCount);
    }

    if (repeat == 1) {
        // Renewing the patient status as Admitted
        list[index].status = 1;
        printf("%s>>> %sPatient status renewed as 'ADMITTED'.\n", CYN, MGN);
        return 1;
    }
    else if (repeat == 2 || repeat == 3 || repeat == 4) {
        // When overwriting or when the patient is not a repeated record
        if (repeat == 4) {
            printf("%sEnter Patient's NIC no: %s", CYN, YLW);
            scanf("%s", nic);
        }
        strcpy(patient.nic, nic);
        printf("%sEnter the Patient's Name: %s", CYN, YLW);
        scanf("%s", &patient.name);
        printf("%sEnter Patient's Age: %s", CYN, YLW);
        scanf("%d", &patient.age);
        printf("%sEnter Patient's Gender(M/F): %s", CYN, YLW);
        scanf(" %c", &gender);
        printf("%sEnter Patient's Admission Date(DD/MM/YYYY): %s", CYN, YLW);
        scanf("%s", &patient.admissionDate);

        if (tolower(gender) == 'm') {
            strcpy(patient.gender, male);
        }
        else if (tolower(gender) == 'f') {
            strcpy(patient.gender, fmale);
        }

        if (repeat == 3) {
            records++;      // Adding a new record if no match
            *recordCount+=1;
            printf("Records %d\n", records);
            
            // printPatient(patient);

            list[*recordCount-1] = patient;

            // printPatient(list[*recordCount-1]);

            printf(">>> %s%s successfully ADDED as a new patient record.\n", MGN, list[*recordCount-1].name);
        }
        else {
            list[index] = patient;
            // printPatient(list[index]);
            
            printf(">>> NIC: %s%s was OVERWRITTEN as an Admitted patient record.\n", MGN, list[*recordCount-1].nic);
        }

        return 1;
    }
    else {
        // When Aborted or the patient record is a repeat with status Admitted
        return 0;
    }
}

int delete (Patient *list, int recordCount, int patientCount) {
    if (patientCount < 1) {
        printf(">>> %sThere are no patient records in the database\n", MGN);
    }
    else {
        char nic[20], status;
        printf("%sEnter the NIC of the removing Patient: %s", CYN, YLW);
        scanf("%s", nic);
        // printf("%sDid the patient recover? (Y/N): \n%s", CYN, YLW);
        // scanf("%c", &status);

        for (int i = 0; i < recordCount; i++) {
            if (!strcmp(list[i].nic,nic)) {
                if (list[i].status == 1) {
                    list[i].status = 0;
                    printf("%s----------------------------------------\n", CYN);
                    printf(">>> %s%s was successfully removed from the Patient records\n", MGN,list[i].name);
                    return patientCount-1;
                }
                else {
                    printf("%s----------------------------------------\n", CYN);
                    printf(">>> %sThe patient had already been removed from the records\n", MGN);
                    return patientCount;
                }
            }
        }
        printf("%s----------------------------------------\n", CYN);
        printf(">>> ERROR %sThe requested patient was not found in the records\n", MGN);
        return patientCount;
    }
}

int showPatient (Patient *list, int recordCount, int patientCount) {
    if (patientCount < 1) {
        printf(">>> %sThere are no patient records in the database\n", MGN);
    }
    else {
        char nic[20];
        printf("%sEnter the NIC of the Patient: %s", CYN, YLW);
        scanf("%s", nic);

        for (int i = 0; i < recordCount; i++) {
            if (strcmp(list[i].nic, nic) == 0) {
                if (list[i].status == 1) {
                    printf("%sName   : %s%s\n", CYN, YLW, list[i].name);
                    printf("%sAge    : %s%d\n", CYN, YLW, list[i].age);
                    printf("%sGender : %s%s\n", CYN, YLW, list[i].gender);
                    printf("%sThis patient was admitted on %s%s %sand is currently being treated\n", CYN, YLW, list[i].admissionDate, CYN);
                    printf("%s----------------------------------------\n", CYN);
                    return 0;
                }
                else if (list[i].status == 0) {
                    printf("%s----------------------------------------\n", CYN);
                    printf(">>> %sThis patient has been removed from the records\n", MGN);
                    return 0;
                }
            }
        }
        printf("%s----------------------------------------\n", CYN);
        printf(">>> ERROR %sThe requested patient was not found in the records\n", MGN);
        return 0;
    }
}

int printAll (Patient *list, int recordCount, int patientCount) {
    int check;
    printf("%sAdmitted (1) or Removed (0): ", CYN);
    scanf("%d",&check);
    if (patientCount < 1) {
        printf(">>> %sThere are no patient records in the database\n", MGN);
    }
    else {
        printf(">>> %sThere are %d patients currently being treated.\n", MGN, patientCount);
        for (int i = 0; i < recordCount; i++) {
            if (list[i].status == check) {
                printf("%sNIC    : %s%s\n", CYN, YLW, list[i].nic);
                printf("%sName   : %s%s\n", CYN, YLW, list[i].name);
                printf("%sAge    : %s%d\n", CYN, YLW, list[i].age);
                printf("%sGender : %s%s\n", CYN, YLW, list[i].gender);
                printf("%sStatus : %s%d\n", CYN, YLW, list[i].status);
                printf("%sThis patient was admitted on %s%s %sand is currently being treated\n", CYN, YLW, list[i].admissionDate, CYN);
                printf("%s----------------------------------------\n\n", CYN);
            }
        }
    }
}


int main () {
    int quaries, incr;
    char cmd;
    int patientCount = 0, discharged = 0, ceased = 0, records = 0;
    int *recordCount = &records;
    
    printf("%sHow many records do you want to store? \n", CYN);
    scanf("%d", &quaries);

    Patient *arr = malloc(ceil(quaries/2)*sizeof(Patient));

    // printf("RecordsCouunt: \n");
    // scanf("%d", recordCount);
    // printf("PatientCouunt: \n");
    // scanf("%d", &patientCount);

    cmd = ui(recordCount, patientCount);

    while (tolower(cmd) != 'q') {

        switch (tolower(cmd)) {
            case '+':
                if (quaries > *recordCount) {
                    printf("%sRecord: %s%d/%d\n", CYN, MGN, *recordCount, quaries);
                    incr = add(arr, recordCount, patientCount, 0); // How much does the patient count increment
                    patientCount += incr;
                }
                else {
                    printf("%s>>> ERROR %sCan't add any more records\n", CYN, MGN);
                    sleep(1);
                }
                printf("%s----------------------------------------\n", CYN);
                break;
            case '-':
                patientCount = delete(arr, *recordCount, patientCount);
                sleep(1);
                break;
            case '*':
                incr = add(arr, recordCount, patientCount, 10); // When Changing, repeat is not 0
                sleep(1);
                break;
            case 'p':
                showPatient(arr, *recordCount, patientCount);
                sleep(1);
                break;
            case 'a':
                printAll(arr, *recordCount, patientCount);
                break;
            default:
                printf("%s>>> ERROR %sWrong Command Entry! Try Again.\n", CYN, MGN);
                break;
        }

        for (int i = 0; i < 3; i++) {
            printf("%s                   .\n", CYN);
        }

        if (quaries > *recordCount || quaries == 0) {
            cmd = ui(recordCount, patientCount);
        }
        else {
            printf("%s----------------------------------------\n", CYN);
            printf("  * %d Records were added succesfully *\n", quaries);
            printf("                 ****               \n");

            sleep(1);
            
            cmd = ui(recordCount, patientCount);
            quaries = 0;        
        }
        
    }
    
    for (int i = 0; i < 3; i++) {
        printf("%s                   .\n", CYN);
    }
    printf("%s----------------------------------------\n", CYN);
    printf("%s* * * * * Quitting the program * * * * * %s\n", CYN, NRM);
    printf("%s               THANK YOU! %s\n", CYN, NRM);
    printf("%s                 ****** %s\n", CYN, NRM);

    return 0;
}