#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define DATABASE "humans.bin"

struct header
{
	int count;
};

struct human_s
{
	char firstname[32];
	char lastname[32];
	char patronymic[32];
	char address[256];
	char phonenumber[32];
	int age;
};

human_s humans[] =
{
#include "humans.h"
};

const int total_objects = sizeof(humans) / sizeof(humans[0]);

int current_users;
const int max_data = 20;
human_s humans_data[max_data];

void WriteInitData()
{
	FILE *fp = fopen(DATABASE, "wb");
	if (!fp) {
		printf("Failed to open file!\n");
		return;
	}

	fwrite(&total_objects, sizeof(int), 1, fp);

	if (fwrite(&humans, sizeof(humans[0]), total_objects, fp) != total_objects)
		printf("Error writing data to file!\n");

	fclose(fp);
}

FILE *fp = NULL;

bool Load(const char *filename)
{
	if (!(fp = fopen(filename, "rb"))) {
		printf("Load(): Failed open file!\n");
		return false;
	}

	header hdr;
	fread(&hdr, sizeof(hdr), 1, fp);
	if (hdr.count >= max_data) {
		printf("Load(): Too many entries. Reduced to %d\n", max_data);
		hdr.count = max_data;
	}

	if (fread(&humans_data, sizeof(humans_data[0]), hdr.count, fp) != hdr.count) {
		printf("Load(): Failed to load all data blocks!\n");
		fclose(fp);
		return false;
	}
	current_users = hdr.count;

	fclose(fp);
	fp = NULL;
	return true;
}

bool DeleteHumanByAge(int age)
{
	for (int i = 0; i < max_data; i++) {
		if (humans_data[i].age == age) {
			for (int j = i; j < max_data - 1; j++) {
				humans_data[j] = humans_data[j + 1];
			}
			current_users--;
			return true;
		}
	}
	return false;
}

bool InsertHuman(human_s *p_human_data, int index)
{
	if (current_users == max_data)
		return false;

	for (int i = index; i < max_data - 1; i += 2)
		humans_data[i + 1] = humans_data[i];

	humans_data[index] = *p_human_data;
	if (current_users < max_data) {
		current_users++;
		printf("current_users++;\n");
	}
	return true;
}

void PrintData()
{
	for (int i = 0; i < current_users; i++)
		printf("%d: %s %s %s %s %s %d\n",
			i,
			humans_data[i].lastname,
			humans_data[i].firstname,
			humans_data[i].patronymic,
			humans_data[i].address,
			humans_data[i].phonenumber,
			humans_data[i].age);
}

#define MENU 1
#define DELETE_ALL_USERS_BY_AGE 2
#define ADD_USER 3
#define PRINT_DATA 4

void DisplayMenu()
{
	printf("%d) Display menu\n", MENU);
	printf("%d) Delete user by age\n", DELETE_ALL_USERS_BY_AGE);
	printf("%d) Add new user after selected index\n", ADD_USER);
	printf("%d) Print all humans\n", PRINT_DATA);
}

int main()
{
	WriteInitData();
	Load(DATABASE);

	int id;
	DisplayMenu();
	while (true) {
		scanf_s("%d", &id);
		switch (id)
		{
		case ADD_USER:
			int index;
			printf("Enter index: ");
			scanf("%d", &index);
			index++;
			if (index < 0 || index > max_data) {
				printf("Selected invalid index!\n");
				continue;
			}

			human_s new_human;
			printf("Enter lastname: ");
			scanf("%s", new_human.lastname);
			printf("Enter firstname: ");
			scanf("%s", new_human.firstname);
			printf("Enter patronymic: ");
			scanf("%s", new_human.patronymic);
			printf("Enter address: ");
			scanf("%s", new_human.address);
			printf("Enter phonenumber: ");
			scanf("%s", new_human.phonenumber);
			printf("Enter age: ");
			scanf("%d", &new_human.age);

			printf("Human add %s\n", InsertHuman(&new_human, index) ? "sucessfully" : "failed");
			break;

		case DELETE_ALL_USERS_BY_AGE:
			int age;
			printf("\nEnter age for delete: ");
			scanf("%d", &age);

			int deleted;
			deleted = 0;
			for (int i = 0; i < current_users; i++) {
				if (humans_data[i].age == age) {
					printf("Deleted user %s!\n", humans_data[i].firstname);
					printf("Delete status: %s\n", DeleteHumanByAge(age) ? "SUCCESS" : "NOT FOUND");
					deleted++;
				}
			}
			printf("Deleted users: %d\n", deleted);
			PrintData();
			break;

		case PRINT_DATA:
			PrintData();
			break;

		case MENU:
			DisplayMenu();
			break;
		}
	}
	return 0;
}