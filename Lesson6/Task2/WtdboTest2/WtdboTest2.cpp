// WtdboTest.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <ctime>

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>
namespace dbo = Wt::Dbo;
/*
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
*/
/*
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
*/
/*
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
*/
/*
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
*/
/*
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

*/
class User {
public:
    std::string name = "";
    std::string phone = "";
    int karma = 0;
    template<class Action>
    void persist(Action& a) {
        dbo::field(a, name, "name");
        dbo::field(a, phone, "phone");
        dbo::field(a, karma, "karma");

    }

};

int main()
{
    try {
        std::string conn =
            "host=localhost"
            " port=5432"
            " dbname=book_store_db"
            " user=postgres"
            " password=Cnjkbwf1988!";

        auto postgres = std::make_unique<dbo::backend::Postgres>(conn);

        dbo::Session session;
        session.setConnection(std::move(postgres));
        session.mapClass<User>("user");
        //session.mapClass<Book>("book");
        //session.mapClass<Publisher>("publisher");

        //session.mapClass<Stock>("stock");
        //session.mapClass<Shop>("shop");
        //session.mapClass<Sale>("sale");
        session.createTables();

    }
    catch (const dbo::Exception& e) {

        std::cout << "\n" << e.what() << "\n";
    }




    return 0;

}