#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <Windows.h>

using namespace std;

struct Student {
    string surname;
    vector<int> grades;
};

struct Faculty {
    string name;
    int groupNumber;
};

struct Course {
    string name;
};

struct Record {
    Faculty faculty;
    Course course;
    Student student;
};

// Функція для запису даних у файл
void writeToFile(const string& filename, const vector<Record>& records) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Не вдалося відкрити файл для запису.\n";
        return;
    }
    for (const auto& record : records) {
        file.write(reinterpret_cast<const char*>(&record), sizeof(Record));
    }
    file.close();
}

// Функція для додавання запису у файл
void appendToFile(const string& filename, const Record& record) {
    ofstream file(filename, ios::binary | ios::app);
    if (!file) {
        cerr << "Не вдалося відкрити файл для додавання.\n";
        return;
    }
    file.write(reinterpret_cast<const char*>(&record), sizeof(Record));
    file.close();
}

// Функція для заміни запису у файлі
void replaceInFile(const string& filename, const Record& record, int index) {
    fstream file(filename, ios::binary | ios::in | ios::out);
    if (!file) {
        cerr << "Не вдалося відкрити файл для заміни.\n";
        return;
    }
    file.seekp(index * sizeof(Record));
    file.write(reinterpret_cast<const char*>(&record), sizeof(Record));
    file.close();
}

// Функція для видалення даних з файлу
void deleteFromFile(const string& filename, const string& surname) {
    vector<Record> records;
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Не вдалося відкрити файл для читання.\n";
        return;
    }
    Record record;
    while (file.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
        if (record.student.surname != surname) {
            records.push_back(record);
        }
    }
    file.close();

    ofstream outFile(filename, ios::binary);
    if (!outFile) {
        cerr << "Не вдалося відкрити файл для запису.\n";
        return;
    }
    for (const auto& rec : records) {
        outFile.write(reinterpret_cast<const char*>(&rec), sizeof(Record));
    }
    outFile.close();
}

// Функція для знаходження студентів з середнім балом 4.5
void findStudentsByAverageGrade(const vector<Record>& records, double targetAverage) {
    cout << "Студенти з середнім балом " << targetAverage << ":\n";
    for (const auto& record : records) {
        double sum = 0;
        for (int grade : record.student.grades) {
            sum += grade;
        }
        double average = sum / record.student.grades.size();
        if (average == targetAverage) {
            cout << "Прізвище: " << record.student.surname << ", Група: " << record.faculty.groupNumber << ", Факультет: " << record.faculty.name << endl;
        }
    }
}

// Функція для видалення студентів першого курсу з трьома двійками
void deleteStudentsWithLowGrades(const string& filename) {
    vector<Record> records;
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Не вдалося відкрити файл для читання.\n";
        return;
    }
    Record record;
    while (file.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
        if (record.course.name == "Перший") {
            int countTwos = count(record.student.grades.begin(), record.student.grades.end(), 2);
            if (countTwos == 3) {
                continue;
            }
        }
        records.push_back(record);
    }
    file.close();

    ofstream outFile(filename, ios::binary);
    if (!outFile) {
        cerr << "Не вдалося відкрити файл для запису.\n";
        return;
    }
    for (const auto& rec : records) {
        outFile.write(reinterpret_cast<const char*>(&rec), sizeof(Record));
    }
    outFile.close();
}

// Функція для сортування назв факультетів за зростанням успішності студентів
void sortFacultiesBySuccess(const vector<Record>& records) {
    vector<string> facultyNames;
    for (const auto& record : records) {
        double sum = 0;
        for (int grade : record.student.grades) {
            sum += grade;
        }
        double average = sum / record.student.grades.size();
        facultyNames.push_back(record.faculty.name + ": " + to_string(average));
    }
    sort(facultyNames.begin(), facultyNames.end());

    cout << "Факультети відсортовані за успішністю:\n";
    for (const auto& name : facultyNames) {
        cout << name << endl;
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    vector<Record> records;

    while (true) {
        cout << "1. Додати запис до файлу\n";
        cout << "2. Замінити дані в файлі\n";
        cout << "3. Видалити студентів з низькими оцінками\n";
        cout << "4. Знайти студентів за середньою оцінкою\n";
        cout << "5. Відсортувати факультети за успішністю\n";
        cout << "6. Вихід\n";
        cout << "Оберіть опцію: ";

        int choice;
        cin >> choice;

        switch (choice) {
        case 1: {
            Record newRecord;
            cout << "Введіть дані для нового запису:" << endl;
            cout << "Назва факультету: ";
            cin >> newRecord.faculty.name;
            cout << "Номер групи: ";
            cin >> newRecord.faculty.groupNumber;
            cout << "Назва курсу: ";
            cin >> newRecord.course.name;
            cout << "Прізвище студента: ";
            cin >> newRecord.student.surname;
            cout << "Введіть оцінки з іспитів (введіть -1 щоб завершити): ";
            int grade;
            while (true) {
                cin >> grade;
                if (grade == -1) break;
                newRecord.student.grades.push_back(grade);
            }
            records.push_back(newRecord);
            appendToFile("records.bin", newRecord);
            break;
        }
        case 2: {
            int index;
            cout << "Введіть індекс запису для заміни: ";
            cin >> index;
            if (index >= 0 && index < records.size()) {
                Record newData;
                cout << "Введіть нові дані для запису:" << endl;
                cout << "Назва факультету: ";
                cin >> newData.faculty.name;
                cout << "Номер групи: ";
                cin >> newData.faculty.groupNumber;
                cout << "Назва курсу: ";
                cin >> newData.course.name;
                cout << "Прізвище студента: ";
                cin >> newData.student.surname;
                cout << "Введіть оцінки з іспитів (введіть -1 щоб завершити): ";
                int grade;
                while (true) {
                    cin >> grade;
                    if (grade == -1) break;
                    newData.student.grades.push_back(grade);
                }
                records[index] = newData;
                replaceInFile("records.bin", newData, index);
            }
            else {
                cout << "Неправильний індекс запису." << endl;
            }
            break;
        }
        case 3: {
            deleteStudentsWithLowGrades("records.bin");
            cout << "Студенти з низькими оцінками успішно видалені з файлу." << endl;
            break;
        }
        case 4: {
            double targetAverage;
            cout << "Введіть цільову середню оцінку: ";
            cin >> targetAverage;
            findStudentsByAverageGrade(records, targetAverage);
            break;
        }
        case 5: {
            sortFacultiesBySuccess(records);
            break;
        }
        case 6:
            return 0;
        default:
            cout << "Неправильний вибір. Будь ласка, спробуйте ще раз.\n";
        }
    }

    return 0;
}
