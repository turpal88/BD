// WtdboTest.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <ctime>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>
namespace dbo = Wt::Dbo;
class Publisher;
class Book;
class Stock;
class Shop;
class Sale;
class Publisher {
public:
    int id;
    std::string name;
    dbo::ptr<Book> book;
    template<class Action>
    void persist(Action& a) {
        dbo::field(a, id, "id");
        dbo::field(a, name, "name");
        dbo::belongsTo(a, book);

    }
};

class Book {
public:
    int id;
    std::string title;
    dbo::weak_ptr<Publisher> publisher;
    dbo::ptr<Stock> stock;
    template<class Action>
    void persist(Action& a) {
        dbo::field(a, id, "id");
        dbo::field(a, title, "title");
        dbo::hasOne(a, publisher);
        dbo::belongsTo(a, stock);
    }

};

class Stock {
public:
    int id;
    dbo::collection<dbo::ptr<Book>> book_in_stock;
    dbo::collection<dbo::ptr<Shop>> shop_in_stock;
    dbo::ptr<Sale> sale;
    int count;
    template<class Action>
    void persist(Action& a) {
        dbo::field(a, id, "id");
        dbo::field(a, count, "count");
        dbo::hasMany(a, book_in_stock, dbo::ManyToOne, "book_in_stock");
        dbo::hasMany(a, shop_in_stock, dbo::ManyToOne, "shop_in_stock");
        dbo::belongsTo(a, sale, "sale");
    }

};

class Shop {
public:
    int id;
    std::string name;
    dbo::ptr<Stock> stock;
    template<class Action>
    void persist(Action& a) {
        dbo::field(a, id, "id");
        dbo::field(a, name, "name");
        dbo::belongsTo(a, stock);
    }

};

class Sale {
public:
    int id;
    float price;
    time_t date_sale;
    dbo::collection<dbo::ptr<Stock>> id_stock;
    int count;
    template<class Action>
    void persist(Action& a) {
        dbo::field(a, id, "id");
        dbo::field(a, price, "price");
        dbo::field(a, date_sale, "date_sale");
        dbo::hasMany(a, id_stock, dbo::ManyToOne, "id_stock");
    }

};

int main()
{

    return 0;

}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
