#include <iostream>
#include <fstream>
#include <cstring>  
using namespace std;

class Employee
{
private:
	int id;
	float hoursWorked, hourlyRate, grossSalary, taxDeduction, netSalary, bonus;

public:
	Employee() : id(0), hoursWorked(0), hourlyRate(0), grossSalary(0), taxDeduction(0), netSalary(0), bonus(0) {}

	void setEmployee(int empID, float hours, float rate)
	{
		id = empID;
		if (hours > 0)
		{
			hoursWorked = hours;
		}
		hours = 0;
		if (rate > 0)
		{
			hourlyRate = rate;
		}
		rate = 0;
		calculateSalary();
	}

	void calculateSalary()
	{
		grossSalary = hoursWorked * hourlyRate;
		taxDeduction = grossSalary * 0.10f;  // Used f to Avoid Conversion Error
		bonus = (hoursWorked > 40) ? grossSalary * 0.05f : 0;
		netSalary = grossSalary - taxDeduction + bonus;
	}
	// Getters
	int getID()
	{
		return id; 
	}
	float getNetSalary()
	{
		return netSalary; 
	}
	void displayEmployee()
	{
		cout << id << "   " << hoursWorked << "   " << hourlyRate
			<< "   " << grossSalary << "   " << taxDeduction
			<< "   " << bonus << "   " << netSalary << endl;
	}

	void saveToFile(ofstream &file)
	{
		file << id << " " << hoursWorked << " " << hourlyRate << endl;
	}
};

// File Handler Class
class FileHandler
{
public:
	static void saveToFile(Employee employees[], int count)
	{
		ofstream file("employees.txt");
		if (!file)
		{
			cout << "Error saving data!\n";
		}
		for (int i = 0; i < count; i++)
		{
			employees[i].saveToFile(file);
		}
		file.close();
		cout << "Data Saved to File Successfully!\n";
	}

	static int loadFromFile(Employee employees[])
	{
		ifstream file("employees.txt");
		if (!file)
		{
			cout << "No previous records found. Starting fresh!\n";
			return 0;
		}
		int count = 0, id;
		float hours, rate;
		while (file >> id >> hours >> rate)
		{
			employees[count].setEmployee(id, hours, rate);
			count++;
		}
		file.close();
		cout << "Data loaded successfully!\n";
		return count;
	}
};

// Admin Class for Authentication
class Admin
{
public:
	static bool login()
	{
		char username[20], password[20];
		cout << "\nAdmin Login Required\n";
		cout << "Username: ";
		cin >> username;
		cout << "Password: ";
		cin >> password;

		if (strcmp(username, "admin") != 0 || strcmp(password, "1234") != 0)
		{
			cout << "Access Denied!\n";
			return false;
		}
		cout << "Login Successful!\n";
		return true;
	}
};

// Payroll System Class
class PayrollSystem
{
private:
	Employee employees[100];
	int recordCount;

public:
	PayrollSystem() 
	{
		recordCount = FileHandler::loadFromFile(employees); 
	}

	void displayRecords()
	{
		if (recordCount == 0)
		{
			cout << "No Employees to Display!\n";
			return;
		}
		cout << "-----------------------------------------------\n";
		cout << "ID   Hours  Rate  Gross  Tax  Bonus  Net Salary\n";
		cout << "-----------------------------------------------\n";
		for (int i = 0; i < recordCount; i++)
		{
			employees[i].displayEmployee();
		}
		cout << "-----------------------------------------------\n";
	}

	void addEmployee()
	{
		if (recordCount >= 100)
		{
			cout << "Error: Employee list is full!\n";
			return;
		}
		int id;
		float hours, rate;
		cout << "Enter Employee ID: ";
		cin >> id;

		for (int i = 0; i < recordCount; i++)
		{
			if (employees[i].getID() == id)
			{
				cout << "Error: Employee ID already exists!\n";
				return;
			}
		}

		cout << "Enter Hours Worked: ";
		cin >> hours;
		cout << "Enter Hourly Rate: ";
		cin >> rate;

		if (hours < 0 || rate < 0)
		{
			cout << "Error: Hours and Rate must be positive values!\n";
		}

		employees[recordCount].setEmployee(id, hours, rate);
		recordCount++;
		cout << "New Employee Added Successfully!\n";
	}

	void searchEmployee()
	{
		int id;
		cout << "Enter Employee ID to Search: ";
		cin >> id;
		for (int i = 0; i < recordCount; i++)
		{
			if (employees[i].getID() == id)
			{
				cout << "Employee Found:-\n";

				cout << "-----------------------------------------------\n";
				cout << "ID   Hours  Rate  Gross  Tax  Bonus  Net Salary\n";
				cout << "-----------------------------------------------\n";
				
				employees[i].displayEmployee();
				return;
			}
		}
		cout << "Error: Employee Not Found!\n";
	}

	// Sort in Hihest Order
	void sortEmployees() 
	{
		if (recordCount == 0)
		{
			cout << "No Employees to Sort!\n";
			return;
		}
		for (int i = 0; i < recordCount - 1; i++)
		{
			for (int j = i + 1; j < recordCount; j++)
			{
				if (employees[i].getNetSalary() < employees[j].getNetSalary())
				{
					swap(employees[i], employees[j]);
				}
			}
		}
		cout << "Employees Sorted by Net Salary!\n";
	}

	void saveData() 
	{
		FileHandler::saveToFile(employees, recordCount); 
	}

	void menu()
	{
		if (!Admin::login())
		{
			return;
		}
		char choice;
		do
		{
			cout << "\nEmployee Payroll Management System\n";
			cout << "1. Display All Employees\n";
			cout << "2. Add a New Employee\n";
			cout << "3. Search Employee by ID\n";
			cout << "4. Sort Employees by Salary\n";
			cout << "5. Save & Exit\n";
			cout << "Enter Choice: ";
			cin >> choice;

			switch (choice)
			{
			case '1':
			{
				displayRecords(); 
				break; 
			}
			case '2':
			{
				addEmployee();
				break;
			}
			case '3':
			{
				searchEmployee();
				break;
			}
			case '4':
			{
				sortEmployees();
				break;
			}
			case '5':
			{ 
				saveData();
				cout << "Exiting...\n";
				break;
			}
			default:
			{
				cout << "Invalid Choice! Try again.\n";
			}
			}
		} while (choice != '5');
	}
};

int main()
{
	PayrollSystem payroll;
	payroll.menu();

	system("pause");
	return 0;
}
