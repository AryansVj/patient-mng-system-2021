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

    printf("----------------------------------------\n");
    printf("   COVID-19 PATIENT MANAGEMENT SYSTEM   \n");
    printf(" * * * * * * * * * * * * * * * * * * * *\n");
    printf("#Records: %d | #Patients: %d | #Removed: %d\n", *recordCount, patientCount, *recordCount-patientCount);
    printf("----------------------------------------\n");
    printf("[+]: Add a new patient record\n");
    printf("----------------------------------------\n");
    printf("[-]: Delete a patient record\n");
    printf("----------------------------------------\n");
    printf("[*]: Change a record\n");
    printf("----------------------------------------\n");
    printf("[P]: Print a patient record\n");
    printf("----------------------------------------\n");
    printf("[A]: Print all the patient records\n");
    printf("----------------------------------------\n");
    printf("[Q]: Delete all record and quit\n");
    printf("----------------------------------------\n");
    printf("              ************              \n");
    printf("            ENTER THE OPTION            \n");
    printf("                   ");

    scanf(" %c", &option);      // Taking the operation Command

    if (tolower(option) == 'q') {  
        // For terminating the program 
        return option;
    }
    
    for (int i = 0; i < 3; i++) {
        printf("                   .\n");
    }
    printf(" * * * * * Request Processing * * * * * \n");

    return option;      // Returning the operation command
}

int checkRepeat (Patient* list, char *nic, int *index, int recordCount) {
    int cmd, check = 0;
    for (int i = 0; i < recordCount; i++) {
        if (strcmp(list[i].nic, nic) == 0) {
            *index = i;     // Keeping the repeated patinet index
            if (list[i].status == 1) {
                // If the patient is marked as admitted
                printf(">>> This Patient is already in the records\n");
                printf("Remove the patient record before making any changes\n");
                return 0;
            }
            else {
                // If the Patient was admitted and then removed later
                printf(">>> This Patient was removed from the records recently.\n");
                printf("Do you want to [0] Abort?\n");
                printf("               [1] Renew the record?\n");
                printf("               [2] Overwrite the record? \n");
                printf("Choose an option(0 / 1 / 2): ");
                scanf("%d", &cmd);
                if (cmd == 0) {
                    printf(">>> ABORTED.\n");
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
            printf("Do you want to [0] Abort?\n");
            printf("               [1] Renew the record if removed?\n");
            printf("               [2] Overwrite the record? \n");
            printf("               [4] Delete and add new? \n");
            printf("Choose an option(0 / 1 / 2 / 4): ");
            scanf("%d", &cmd);
            if (cmd == 0) {
                printf(">>> ABORTED.\n");
            }
            return cmd;
        }
    }
    printf(">>> Record does not exist.\n");
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
    printf("Enter Patient's NIC no: ");
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
        printf(">>> Patient status renewed as 'ADMITTED'.\n");
        return 1;
    }
    else if (repeat == 2 || repeat == 3 || repeat == 4) {
        // When overwriting or when the patient is not a repeated record
        if (repeat == 4) {
            printf("Enter Patient's NIC no: ");
            scanf("%s", nic);
        }
        strcpy(patient.nic, nic);
        printf("Enter the Patient's Name: ");
        scanf("%s", &patient.name);
        printf("Enter Patient's Age: ");
        scanf("%d", &patient.age);
        printf("Enter Patient's Gender(M/F): ");
        scanf(" %c", &gender);
        printf("Enter Patient's Admission Date(DD/MM/YYYY): ");
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

            printf(">>> %s successfully ADDED as a new patient record.\n", list[*recordCount-1].name);
        }
        else {
            list[index] = patient;
            // printPatient(list[index]);
            
            printf(">>> NIC: %s was OVERWRITTEN as an Admitted patient record.\n", list[*recordCount-1].nic);
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
        printf(">>> There are no patient records in the database\n");
    }
    else {
        char nic[20], status;
        printf("Enter the NIC of the removing Patient: ");
        scanf("%s", nic);
        // printf("Did the patient recover? (Y/N): \n");
        // scanf("%c", &status);

        for (int i = 0; i < recordCount; i++) {
            if (!strcmp(list[i].nic,nic)) {
                if (list[i].status == 1) {
                    list[i].status = 0;
                    printf("----------------------------------------\n");
                    printf(">>> %s was successfully removed from the Patient records\n",list[i].name);
                    return patientCount-1;
                }
                else {
                    printf("----------------------------------------\n");
                    printf(">>> The patient had already been removed from the records\n");
                    return patientCount;
                }
            }
        }
        printf("----------------------------------------\n");
        printf(">>> ERROR The requested patient was not found in the records\n");
        return patientCount;
    }
}

int showPatient (Patient *list, int recordCount, int patientCount) {
    if (patientCount < 1) {
        printf(">>> There are no patient records in the database\n");
    }
    else {
        char nic[20];
        printf("Enter the NIC of the Patient: ");
        scanf("%s", nic);

        for (int i = 0; i < recordCount; i++) {
            if (strcmp(list[i].nic, nic) == 0) {
                if (list[i].status == 1) {
                    printf("Name   : %s\n", list[i].name);
                    printf("Age    : %d\n", list[i].age);
                    printf("Gender : %s\n", list[i].gender);
                    printf("This patient was admitted on %s and is currently being treated\n", list[i].admissionDate);
                    printf("----------------------------------------\n");
                    return 0;
                }
                else if (list[i].status == 0) {
                    printf("----------------------------------------\n");
                    printf(">>> This patient has been removed from the records\n");
                    return 0;
                }
            }
        }
        printf("----------------------------------------\n");
        printf(">>> ERROR The requested patient was not found in the records\n");
        return 0;
    }
}

int printAll (Patient *list, int recordCount, int patientCount) {
    int check;
    printf("Admitted (1) or Removed (0): ");
    scanf("%d",&check);
    if (patientCount < 1) {
        printf(">>> There are no patient records in the database\n");
    }
    else {
        printf(">>> There are %d patients currently being treated.\n", patientCount);
        for (int i = 0; i < recordCount; i++) {
            if (list[i].status == check) {
                printf("NIC    : %s\n", list[i].nic);
                printf("Name   : %s\n", list[i].name);
                printf("Age    : %d\n", list[i].age);
                printf("Gender : %s\n", list[i].gender);
                printf("Status : %d\n", list[i].status);
                printf("This patient was admitted on %s and is currently being treated\n", list[i].admissionDate);
                printf("----------------------------------------\n\n");
            }
        }
    }
}


int main () {
    int quaries, incr;
    char cmd;
    int patientCount = 0, discharged = 0, ceased = 0, records = 0;
    int *recordCount = &records;
    
    printf("How many records do you want to store? \n");
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
                    printf("Record: %d/%d\n", *recordCount+1, quaries);
                    incr = add(arr, recordCount, patientCount, 0); // How much does the patient count increment
                    patientCount += incr;
                }
                else {
                    printf(">>> ERROR Can't add any more records\n");
                    sleep(1);
                }
                printf("----------------------------------------\n");
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
                sleep(1);
                break;
            default:
                printf(">>> ERROR Wrong Command Entry! Try Again.\n");
                break;
        }

        for (int i = 0; i < 3; i++) {
            printf("                   .\n");
        }

        if (quaries > *recordCount || quaries == 0) {
            cmd = ui(recordCount, patientCount);
        }
        else {
            printf("----------------------------------------\n");
            printf("  * %d Records were added succesfully *\n", quaries);
            printf("                 ****               \n");

            sleep(1);
            
            cmd = ui(recordCount, patientCount);
            quaries = 0;        
        }
        
    }
    
    for (int i = 0; i < 3; i++) {
        printf("                   .\n");
    }
    printf("----------------------------------------\n");
    printf("* * * * * Quitting the program * * * * * \n");
    printf("               THANK YOU! \n");
    printf("                 ****** \n");

    return 0;
}