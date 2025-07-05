//Задан файл, который содержит информацию про туристические агентства. 
//О каждом туре известно: страна, города, место проживания, характе тура, 
// цена путёвки, периодичность. Выбрать коммерческие туры и туры отдыха.
// Напечатать в алфавитном порядке названия стран, в которые есть возможность поехать
// Выбрать три самые дорогие и три самые дещёвые туры 
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;
//Это структура,   описывающая один тур 
struct Tour {
    string country;          // страна
    vector<string> cities;    // города
    string accommodation;      // какое жильё
    string type;              // характер тура
    double price;              // цена
    string frequency;           //периодичность тура
};

// Ассоциативный список: страна >  
multimap<string, Tour> tours;    // используется для хранения всех туров с ключом — страной


void loadFromFile(const string& filename) {
    ifstream file(filename);        // Создаём входной файловый поток file, открывается файл по имени filename (tours.txt)
    string line;       // Переменная line хранит текущую строку из файла

    while (getline(file, line)) {    // Построчно читаем информацию (один тур = одна строка),   пока есть строки 
        stringstream ss(line);      // создаём поток из строки, чтобы извлекать поля
        string country, cities_str, accommodation, type, price_str, frequency;
        // Разбиваем строку на поля по символу ;
        // извлекаем каждое поле до символа ;
        // переменные получают значения из строки
        getline(ss, country, ';');
        getline(ss, cities_str, ';');
        getline(ss, accommodation, ';');
        getline(ss, type, ';');
        getline(ss, price_str, ';');
        getline(ss, frequency, ';');
        // 
        Tour tour;      // Объявили объект tour типа Тour,в него постепенно записываются все данные из строки
        tour.country = country;

        // Строка городов (cities_str) содержит несколько городов, разделённых запятой
        stringstream city_stream(cities_str);  // используем stringstream (city_stream) для разбора по ,
        string city;     // 
        while (getline(city_stream, city, ',')) {
            tour.cities.push_back(city);    // каждый город добавляется в tour.cities — это vector<string>.
        }

        // Прямое присваивание оставшихся полей
        tour.accommodation = accommodation;
        tour.type = type;
        tour.price = stod(price_str);  // перевод строки в double (цена)
        tour.frequency = frequency;

        tours.insert({ country, tour });    //Созданный объект Tour  добавляется   в tours  (multimap)
        // добавляется пара: first: country (ключ)  second: tour (значение)
        // используем multimap, чтобы можно было добавить несколько туров на одну страну.
    }

    file.close();
}
//  функция печатает все поля одного тура: страну, города, тип, цену и т.д.
void printTour(const Tour& t) {
    cout << t.country << " | ";
    for (const auto& c : t.cities) cout << c << ",";
    cout << " | " << t.accommodation << " | " << t.type << " | " << t.price << " | " << t.frequency << "\n";
}
// функция перебирает все элементы tours и вызывает printTour для каждого 
void showAllTours() {
    for (const auto& pair : tours)
        printTour(pair.second);
}
//  сортировка и вывод стран
void showSortedCountries() {
    set<string> countries;
    for (const auto& pair : tours)    // cобираем все названия стран из tours
        countries.insert(pair.first);

    cout << "Список стран (в алфавитном порядке):\n";
    for (const auto& c : countries)
        cout << c << "\n";
}
// отбор по типу тура 
void filterToursByType() {
    cout << "\nКоммерческие и оздоровительные туры:\n";
    for (const auto& pair : tours) {
        if (pair.second.type == "Commercial" || pair.second.type == "Health")
            printTour(pair.second);
    }
}


bool compareByPriceDesc(const Tour& a, const Tour& b) {  // функция-компаратор
    return a.price > b.price;  // по убыванию
}


void showTopTours() {
    vector<Tour> allTours;
    //Все туры собираются в вектор vector<Tour>
    for (const auto& pair : tours)
        allTours.push_back(pair.second);

    // Вектор сортируется по убыванию цены
   // используем функцию-компаратор compareByPriceDesc    
    sort(allTours.begin(), allTours.end(), compareByPriceDesc);

    cout << "\nТоп-3 самых дорогих тура:\n";
    for (int i = 0; i < 3 && i < allTours.size(); ++i)
        printTour(allTours[i]);

    cout << "\nТоп-3 самых дешёвых тура:\n";
    for (int i = 0; i < 3 && i < allTours.size(); ++i)
        printTour(allTours[allTours.size() - 1 - i]);
}
//записываем туры в текстовый файл в том же формате, в котором они читались:
void saveToFile(const string& filename) {   // функция  принимает имя файла (filename) в виде строки
    ofstream file(filename);                // создаётся объект file типа ofstream — выходной файловый поток
    // файл с именем filename открывается для записи
    // если файл существует — он перезаписывается; если не существует — будет создан.

    for (const auto& pair : tours) {     // перебор всех элементов ассоциативного списка tours

        const Tour& t = pair.second;   // извлекаем ссылку на тур t  из пары, это удобно для обращения к полям
        // pair.first — это страна (ключ), pair.second — структура Tour
        file << t.country << ";";     // записываем страну, за ней ставим ; (разделитель между полями).
        for (size_t i = 0; i < t.cities.size(); ++i) {   //  перебираем вектор городов t.cities.
            file << t.cities[i];                         // записываем каждый город
            if (i + 1 != t.cities.size()) file << ",";  // если это не последний город, добавляем , между ними
        }
        file << ";" << t.accommodation << ";" << t.type << ";" << t.price << ";" << t.frequency << "\n"; // записываем остальные поля
    }
    file.close();   // Закрываем файл
    cout << "\nИнформация записана в " << filename << "\n";
}

int main() {
    setlocale(0, "");

    loadFromFile("tours.txt");
    // Меню повторяется в цикле do...while, пока пользователь не выберет 0.
    int choice;
    do {
        cout << "\nМеню:\n"
            << "1. Показать все туры\n"
            << "2. Показать туры (коммерческие и оздоровительные)\n"
            << "3. Показать список стран по алфавиту\n"
            << "4. Топ-3 дорогих и дешёвых тура\n"
            << "5. Сохранить в файл\n"
            << "0. Выход\n"
            << "Выбор: ";
        cin >> choice;

        switch (choice) {
        case 1: showAllTours(); break;
        case 2: filterToursByType(); break;
        case 3: showSortedCountries(); break;
        case 4: showTopTours(); break;
        case 5: saveToFile("output.txt"); break;
        case 0: cout << "Выход...\n"; break;
        default:cout << "Неверный выбор\n"; break;
        }
    } while (choice != 0);

    return 0;
}
