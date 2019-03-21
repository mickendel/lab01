#include <iostream>
#include <fstream>

using namespace std;

struct data_workers
{
	string name;
	string surname;
	int year;
	int salary;
	int holidays;
	int children;
};

int size_final_array = 0;

int count_lines()
{
	ifstream ifile("workers.txt");
	char *str = new char [1024];
    	int i=0;
    	while (!ifile.eof())
    	{
        	ifile.getline(str, 1024, '\n');
        	i++;
    	}
    	delete str;
	ifile.close();
	return i;
}

data_workers * scan_file()
{
	int count = count_lines();
	data_workers *array = new data_workers[count];
	ifstream file("workers.txt");
	for(int i = 0; i < count; i++)
	{
		getline(file, array[i].name);
		getline(file, array[i].surname);
		string year;
		getline(file, year);
		array[i].year = atoi(year.c_str());
		string salary;
		getline(file, salary);
		array[i].salary = atoi(salary.c_str());
		string holidays;
		getline(file, holidays);
		array[i].holidays = atoi(holidays.c_str());
		string children;
		getline(file, children);
		array[i].children = atof(children.c_str());
	}
	file.close();
	return array;
}

data_workers * sort_array(data_workers *array, int ask)
{
	int k = 0;
	for(int i = 0; i < 10; i++)
	{
		if(array[i].children > ask)
		{
			size_final_array += 1;
		}
	}

	data_workers *final_array = new data_workers[size_final_array];

	for(int i = 0; i < 10; i++)
	{
		if(array[i].children > ask)
		{
			final_array[k] = array[i];
			k++;
		}
	}

	string vowels = "AaEeIiOoUuYy";
	int answers[size_final_array];
	
	for(int i = 0; i < size_final_array; i++)
	{
		answers[i] = 0;
	}
	


	for(int i = 0; i < size_final_array; i++)
	{
		for(int j = 0; j < final_array[i].surname.length(); j++)
		{
			for (int q = 0; q < vowels.length(); q++)
			{
				if (final_array[i].surname[j] == vowels[q])
				{
					answers[i] += 1;
				}
			}
		}
	}

	int numVow[size_final_array];

	for (int i = 0; i < size_final_array; i++)
	{
		numVow[i] = answers[i];
	}


	for (int i = 0; i < size_final_array - 1; i++)
	{
		for (int j = 0; j < size_final_array - 1 - i; j++)
		{
			if (numVow[j] < numVow[j+1])
			{
				data_workers temp = final_array[j];
				final_array[j] = final_array[j + 1];
				final_array[j + 1] = temp;
			}
		}
	}

	return final_array;
}

void record_bin(data_workers *arr)
{
	ofstream ofile("results.bin", ios::binary);
	ofile.write(reinterpret_cast<const char*>(&size_final_array), sizeof(size_final_array));

	for(int i = 0; i < size_final_array; i++)
	{
		int size = arr[i].name.size();
        ofile.write(reinterpret_cast<const char *>(&size), sizeof(size));
        ofile.write(arr[i].name.c_str(), size);

		int sizeLast = arr[i].surname.size();
        ofile.write(reinterpret_cast<const char *>(&sizeLast), sizeof(sizeLast));
        ofile.write(arr[i].surname.c_str(), sizeLast);
		
		ofile.write(reinterpret_cast<const char *>(&arr[i].year), sizeof(arr[i].year));
		ofile.write(reinterpret_cast<const char *>(&arr[i].salary), sizeof(arr[i].salary));
		ofile.write(reinterpret_cast<const char *>(&arr[i].holidays), sizeof(arr[i].holidays));
		ofile.write(reinterpret_cast<const char *>(&arr[i].children), sizeof(arr[i].children));
	}
	ofile.close();
	cout << "The operation was successful\n";
}

void read_bin()
{
	int y = 0;
	ifstream ifile("results.bin", ios::binary);
	ifile.read(reinterpret_cast<char*>(&y), sizeof(y));
	data_workers *arr = new data_workers[y];

	for (int i = 0; i < y; i++)
	{
		int s = arr[i].name.size();
		ifile.read(reinterpret_cast<char *>(&s), sizeof(s));
		arr[i].name.resize(s);
		ifile.read(&arr[i].name[0], s);

		int sLast = arr[i].surname.size();
		ifile.read(reinterpret_cast<char *>(&sLast), sizeof(sLast));
		arr[i].surname.resize(sLast);
		ifile.read(&arr[i].surname[0], sLast);

		ifile.read(reinterpret_cast<char *>(&arr[i].year), sizeof(arr[i].year));
		ifile.read(reinterpret_cast<char *>(&arr[i].salary), sizeof(arr[i].salary));
		ifile.read(reinterpret_cast<char *>(&arr[i].holidays), sizeof(arr[i].holidays));
		ifile.read(reinterpret_cast<char *>(&arr[i].children), sizeof(arr[i].children));
	}
	ifile.close();

	for(int i = 0; i < y; i++)
	{
		cout << arr[i].name << " ";
		cout << arr[i].surname << " ";
		cout << arr[i].year << " ";
		cout << arr[i].salary << " ";
		cout << arr[i].holidays << " ";
		cout << arr[i].children << "\n";
	}

	delete [] arr;

}


int main(int argc, char **argv)
{
	string value_h = "-h";
	string value_a = "-a";
	string value_o = "-o";

	if(argc == 1)
	{
		cout << "main.exe -a <param_value> - read workers.txt, sort and write to binary file\n";
		cout << "main.exe -o - read binary file\n";
	}
	else if(argv[1] == value_h)
	{
		cout << "main.exe -a <param_value> - read workers.txt, sort and write to binary file\n";
		cout << "main.exe -o - read binary file\n";
	}
	else if(argc == 3)
	{
		char *value = argv[2];
		int number = atoi(value);
		data_workers *scan_arr = scan_file();
		data_workers *sort_arr = sort_array(scan_arr, number);
		delete[] scan_arr;
		record_bin(sort_arr);
		delete[] sort_arr;
	}
	else if(argv[1] == value_o)
	{
		read_bin();
	}
	else
	{
		cout << "Error.Command not found\n";
	}
	return 0;
}
