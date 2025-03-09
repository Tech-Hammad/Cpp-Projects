#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

class Student {
private:
	char Name[30];
	int rollNo;
	int Age;
	char Address[80];
	char enrolledCourse[5][10];  // Stores up to 5 course names (max 10 chars each)
	int courseGrade[5];          // Stores corresponding grades
	int courseCount;

public:
	Student() {
		Name[0] = '\0';
		Address[0] = '\0';
		rollNo = 0;
		Age = 0;
		courseCount = 0;
		for (int i = 0; i < 5; i++) {
			enrolledCourse[i][0] = '\0';
			courseGrade[i] = 0;
		}
	}

	void setName(const char n[]) {
		strcpy_s(Name, n);
	}

	void setRollNumber(int roll) {
		rollNo = roll;
	}

	void setAge(int age) {
		Age = age;
	}

	void setAddress(const char adr[]) {
		strcpy_s(Address, adr);
	}

	void enrollCourse(const char course[], int grade) {
		if (courseCount >= 5) {
			cout << "You cannot enroll in more than 5 courses.\n";
			return;
		}

		for (int i = 0; i < courseCount; i++) {
			if (strcmp(enrolledCourse[i], course) == 0) {
				cout << "Already enrolled in " << course << "\n";
				return;
			}
		}

		if (grade < 0 || grade > 100) {
			cout << "Invalid grade. Enter a grade between 0 and 100.\n";
			return;
		}

		strcpy_s(enrolledCourse[courseCount], course);
		courseGrade[courseCount] = grade;
		courseCount++;

		cout << "Course added successfully.\n";
	}

	double calculateGPA() {
		if (courseCount == 0) return 0.0;

		double totalGradePoints = 0;
		for (int i = 0; i < courseCount; i++) {
			int grade = courseGrade[i];

			if (grade >= 90) totalGradePoints += 4.0;
			else if (grade >= 80) totalGradePoints += 3.0;
			else if (grade >= 70) totalGradePoints += 2.0;
			else if (grade >= 60) totalGradePoints += 1.0;
			else totalGradePoints += 0.0; // Fail
		}

		return totalGradePoints / courseCount;
	}

	void display() {
		cout << "\nStudent Name: " << Name << "\n";
		cout << "Roll Number: " << rollNo << "\n";
		cout << "Age: " << Age << "\n";
		cout << "Address: " << Address << "\n";
		cout << "--- Courses and Grades ---\n";

		if (courseCount == 0) {
			cout << "No courses enrolled.\n";
		}
		else {
			for (int i = 0; i < courseCount; i++) {
				cout << "Course: " << enrolledCourse[i] << " | Grade: " << courseGrade[i] << "\n";
			}
		}
		cout << "GPA: " << calculateGPA() << "\n";
	}

	void saveToFile() {
		ofstream fout("studentData.txt", ios::app);
		fout << rollNo << "\n" << Name << "\n" << Age << "\n" << Address << "\n" << courseCount << "\n";

		for (int i = 0; i < courseCount; i++) {
			fout << enrolledCourse[i] << " " << courseGrade[i] << "\n";
		}

		fout.close();
		cout << "Data saved to file.\n";
	}

	void loadFromFile(int searchRollNo) {
		ifstream fin("studentData.txt");
		if (!fin) {
			cout << "Error opening file.\n";
			return;
		}

		while (fin >> rollNo) {
			fin.ignore();
			fin.getline(Name, 30);
			fin >> Age;
			fin.ignore();
			fin.getline(Address, 80);
			fin >> courseCount;
			fin.ignore();

			for (int i = 0; i < courseCount; i++) {
				fin >> enrolledCourse[i] >> courseGrade[i];
			}

			if (rollNo == searchRollNo) {
				display();
				fin.close();
				return;
			}
		}

		cout << "No record found for Roll Number " << searchRollNo << ".\n";
		fin.close();
	}

	void dropCourse(const char course[]) {
		bool found = false;
		for (int i = 0; i < courseCount; i++) 
		{
			if (strcmp(enrolledCourse[i], course) == 0) 
			{
				found = true;
				for (int j = i; j < courseCount - 1; j++)
				{
					strcpy_s(enrolledCourse[j], enrolledCourse[j + 1]);
					courseGrade[j] = courseGrade[j + 1];
				}
				courseCount--;
				cout << "Course " << course << " has been dropped.\n";
				return;
			}
		}
		if (!found)
		{
			cout << "Course " << course << " not found.\n";
		}
	}
};

int main()
{
	Student student;
	int choice;

	do
	{
		cout << "\n****** Student Management System ******\n";
		cout << "1. Add Student\n2. Enroll in Course\n3. Drop a Course\n4. Display Student Details\n";
		cout << "5. Save to File\n6. Load Data From File\n0. Exit\n";
		cout << "---------------------------------------\n";
		cout << "Enter your choice: ";
		cin >> choice;
		cin.ignore();

		if (choice == 1) 
		{
			char n[30], adr[80];
			int roll, age;

			cout << "Enter Student Name: ";
			cin.getline(n, 30);
			cout << "Enter Roll Number: ";
			cin >> roll;
			cout << "Enter Age: ";
			cin >> age;
			cin.ignore();
			cout << "Enter Address: ";
			cin.getline(adr, 80);

			student.setName(n);
			student.setRollNumber(roll);
			student.setAge(age);
			student.setAddress(adr);
		}
		else if (choice == 2)
		{
			char course[10];
			int grade;
			cout << "Enter Course Name: ";
			cin.getline(course, 10);
			cout << "Enter Grade (0-100): ";
			cin >> grade;
			cin.ignore();
			student.enrollCourse(course, grade);
		}
		else if (choice == 3)
		{
			char course[10];
			cout << "Enter Course Name to Drop: ";
			cin.getline(course, 10);
			student.dropCourse(course);
		}
		else if (choice == 4)
		{
			student.display();
		}
		else if (choice == 5)
		{
			student.saveToFile();
		}
		else if (choice == 6)
		{
			int rollNo;
			cout << "Enter Roll Number to Search: ";
			cin >> rollNo;
			student.loadFromFile(rollNo);
		}

	} while (choice != 0);

	cout << "Exiting Program...\n";
	system("pause");
	return 0;
}