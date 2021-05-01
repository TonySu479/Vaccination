#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Operacios rendszerek beadando A1SXTS Su Ruilong*/

#define DATA_SIZE 100

typedef struct Patient{
    size_t id;
    char name[DATA_SIZE];
    char year[DATA_SIZE];
    char phoneNumber[DATA_SIZE];
    char service[DATA_SIZE];
} Patient;

typedef struct PatientGroup{
    size_t size;
    Patient* everyPatient;
} PatientGroup;

PatientGroup* patient_group;

PatientGroup* initPatients(const char* fileName){
    PatientGroup* patients = (PatientGroup*)malloc(sizeof(PatientGroup));
    patients->size = 0;
    FILE *fPtr;
    fPtr = fopen(fileName, "r");
    if (fPtr == NULL){
        printf("Error opening file\n");
        exit(1);
    }
    char ch;
    size_t lines = 0;
    while(!feof(fPtr))
    {
        ch = fgetc(fPtr);
        if(ch == '\n'){
            lines++;
        }
    }

    fseek(fPtr, 0, SEEK_SET);
    patients->everyPatient = (Patient*)malloc(sizeof(Patient)*(lines/4));
    for(size_t i = 0; i < lines/4; i++){
		patients->everyPatient[i].id = i;
		fscanf(fPtr, "%[^\n]", patients->everyPatient[i].name);
		fseek ( fPtr , 1 , SEEK_CUR );
		fscanf(fPtr, "%[^\n]", patients->everyPatient[i].year);
		fseek ( fPtr , 1 , SEEK_CUR );
		fscanf(fPtr, "%[^\n]", patients->everyPatient[i].phoneNumber);
		fseek ( fPtr , 1 , SEEK_CUR );
		fscanf(fPtr, "%[^\n]", patients->everyPatient[i].service);
		fseek ( fPtr , 1 , SEEK_CUR );
		patients->size++;
    }
	fclose(fPtr);
	return patients;
}

void selectOption();
void showMenu();

void updateIDs(PatientGroup* patients)
{
		for(size_t i = 0; i < patients->size; i++)
		{
			patients->everyPatient[i].id = i;
		}
}

void addPatient()
{
    printf("\nAdding patient to the database...");
    printf("\nPlease enter your name, date of birth, phone number and whether you have free service, Please press enter after each data");
    printf("\nPatient data adding order: Name -> Year -> Phone Number -> Yes/No\n");
    char name[100], year[100], phoneNumber[100], service[100];
    char c;
    while((c=getchar()) != '\n' && c != EOF);
	if (!(fgets(name, sizeof name, stdin) != NULL))
	{
		fprintf(stderr, "Error reading Name.\n");
	}
	char *pos;
	if ((pos=strchr(name, '\n')) != NULL)
		*pos = '\0';

    if (!(fgets(year, sizeof name, stdin) != NULL))
	{
		fprintf(stderr, "Error reading Year.\n");
	}
	if ((pos=strchr(year, '\n')) != NULL)
		*pos = '\0';

	if (!(fgets(phoneNumber, sizeof name, stdin) != NULL))
	{
		fprintf(stderr, "Error reading Phone number.\n");
	}
	if ((pos=strchr(phoneNumber, '\n')) != NULL)
		*pos = '\0';

	if (!(fgets(service, sizeof name, stdin) != NULL))
	{
		fprintf(stderr, "Error reading Service.\n");
	}
	if ((pos=strchr(service, '\n')) != NULL)
		*pos = '\0';

    FILE*fPtr = fopen("patients.txt", "a");
    if(!fPtr){
        printf("Unable to open patients.txt\n");
        return;
    }
	fputs(name,fPtr);
	fputs("\n", fPtr);
	fputs(year,fPtr);
	fputs("\n", fPtr);
	fputs(phoneNumber,fPtr);
	fputs("\n", fPtr);
	fputs(service,fPtr);
	fputs("\n", fPtr);
	fclose(fPtr);
    patient_group = initPatients("patients.txt");
    printf("Patient added!\n");
	showMenu();
}

void deletePatient(){
    int del_patient_id = -1;
    printf("\nWrite the ID of the patient that you would like to delete!\n");
    scanf("%d", &del_patient_id);
    if(del_patient_id < 1 || del_patient_id > patient_group->size){
        printf("Invalid Patient ID!\n");
    }
    else{
        Patient* everyPC = (Patient*)malloc(sizeof(Patient)*(patient_group->size-1));
        int j = 0;
        for(size_t i = 0; i < patient_group->size; i++){
            if(i != del_patient_id-1)
            {
                everyPC[j] = patient_group->everyPatient[i];
                j++;
            }
        }
        free(patient_group->everyPatient);
        patient_group->everyPatient = everyPC;
        patient_group->size -= 1;
        updateIDs(patient_group);
        FILE*fPtr = fopen("temp.txt", "w");
        if (!fPtr){
            printf("unable to open a temp file to write in\n");
            return;
        }
        for(size_t i = 0; i < patient_group->size; i++){
            fputs(patient_group->everyPatient[i].name, fPtr);
            fputs("\n", fPtr);
            fputs(patient_group->everyPatient[i].year, fPtr);
            fputs("\n", fPtr);
            fputs(patient_group->everyPatient[i].phoneNumber, fPtr);
            fputs("\n", fPtr);
            fputs(patient_group->everyPatient[i].service, fPtr);
            fputs("\n", fPtr);
        }
        fclose(fPtr);
        rename("patients.txt", "_patients.txt");
        rename("temp.txt", "patients.txt");
        free(patient_group->everyPatient);
        free(patient_group);
        patient_group = initPatients("patients.txt");
        remove("_patients.txt");
        remove("temp.txt");
        printf("patient deleted!\n");
    }
    showMenu();
}

void editPatient(){
    int edit_patient_id = -1;
    int option = -1;
    printf("Please enter the ID of the worker you would like to edit\n");
    scanf("%d", &edit_patient_id);
    if(edit_patient_id < 1 || edit_patient_id > patient_group->size){
        printf("Invalid Patient ID");
    } else {
        printf("Please select which data you would like to edit for this Patient\n");
        printf("1 - edit name\n");
        printf("2 - edit year\n");
        printf("3 - edit phone number\n");
        printf("4 - edit service\n");
        scanf("%d", &option);
        char buff[100];
        char c;
		while ((c=getchar()) != '\n' && c != EOF);
		switch(option){
        case 1:
            printf("Please enter the new name:\n");
            if (!(fgets(buff, sizeof buff, stdin) != NULL))
            {
                fprintf(stderr, "Error reading Name.\n");
            }
            char *pos;
            if ((pos=strchr(buff, '\n')) != NULL)
                *pos = '\0';
            strcpy(patient_group->everyPatient[edit_patient_id-1].name,buff);
            break;
        case 2:
            printf("Please enter the new Year:\n");
            if (!(fgets(buff, sizeof buff, stdin) != NULL))
            {
                fprintf(stderr, "Error reading Year.\n");
            }
            if ((pos=strchr(buff, '\n')) != NULL)
                *pos = '\0';
            strcpy(patient_group->everyPatient[edit_patient_id-1].year,buff);
            break;
        case 3:
            printf("Please enter the new Phone number:\n");
            if (!(fgets(buff, sizeof buff, stdin) != NULL))
            {
                fprintf(stderr, "Error reading Phone Number.\n");
            }
            if ((pos=strchr(buff, '\n')) != NULL)
                *pos = '\0';
            strcpy(patient_group->everyPatient[edit_patient_id-1].phoneNumber,buff);
            break;
       case 4:
            printf("Please enter the new Service Status:\n");
            if (!(fgets(buff, sizeof buff, stdin) != NULL))
            {
                fprintf(stderr, "Error reading Service Status.\n");
            }
            if ((pos=strchr(buff, '\n')) != NULL)
                *pos = '\0';
            strcpy(patient_group->everyPatient[edit_patient_id-1].service,buff);
            break;
		}
		FILE*fPtr = fopen("temp.txt", "w");
		if(!fPtr)
        {
            printf("Unable to open a temp file to write in\n");
            return;
        }
        for(size_t i = 0; i< patient_group->size; i++){
            fputs(patient_group->everyPatient[i].name, fPtr);
			fputs("\n", fPtr);
			fputs(patient_group->everyPatient[i].year, fPtr);
			fputs("\n", fPtr);
			fputs(patient_group->everyPatient[i].phoneNumber, fPtr);
			fputs("\n", fPtr);
			fputs(patient_group->everyPatient[i].service, fPtr);
			fputs("\n", fPtr);
        }
        fclose(fPtr);
        rename("patients.txt", "_patients.txt");
		rename("temp.txt", "patients.txt");
        free(patient_group->everyPatient);
        free(patient_group);
        patient_group = initPatients("patients.txt");
        printf("Patient edited!\n");
        remove("_patients.txt");
        remove("temp.txt");
    }
    showMenu();
}

void printPatientGroup(PatientGroup* patients){
    patient_group = initPatients("patients.txt");
	for(size_t i =0; i < patients->size; i++)
	{
		printf("\n%zu. Patient:\nName: %s\nYear: %s\nPhoneNumber: %s\nService: %s\n", patients->everyPatient[i].id+1, patients->everyPatient[i].name,patients->everyPatient[i].year, patients->everyPatient[i].phoneNumber, patients->everyPatient[i].service);
		printf("\n");
	}
}

void selectOption(){
    int i = -1;
    scanf("%d", &i);
    switch(i){
    case 1:
        patient_group = initPatients("patients.txt");
        printPatientGroup(patient_group);
        showMenu();
        break;
    case 2:
        addPatient();
        break;
    case 3:
        printPatientGroup(patient_group);
        deletePatient();
        break;
    case 4:
        editPatient();
        break;
    }
}

void showMenu(){
    printf("1 - Show Patients data\n");
    printf("2 - Create Patient\n");
    printf("3 - Delete Patient data\n");
    printf("4 - Edit Patient\n");
    printf("5 - Exit\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~\n\n");
    selectOption();
}

int main()
{
    patient_group = initPatients("patients.txt");
    showMenu();
    return 0;
}
