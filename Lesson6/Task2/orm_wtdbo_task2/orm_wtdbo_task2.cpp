
#include <iostream>
#include <windows.h>
#pragma execution_character_set("utf-8")
#include <wt/dbo/dbo.h>
#include <wt/Dbo/backend/Postgres.h>



struct Publisher;
struct Book;
struct Stock;
struct Publisher;
struct Shop;
struct Sale;



struct Publisher {
	//int id;
	std::string name;
	Wt::Dbo::collection<Wt::Dbo::ptr<Book>> book;

	template<typename Action>
	void persist(Action& a) {
		//Wt::Dbo::field(a, id, "id");
		Wt::Dbo::field(a, name, "name");
		Wt::Dbo::hasMany(a, book, Wt::Dbo::ManyToOne);
	}

};





struct Book {
	//int id;
	std::string title;
	Wt::Dbo::ptr<Publisher> publisher;
	Wt::Dbo::collection<Wt::Dbo::ptr<Stock>> stock;

	template<class Action>
	void persist(Action& a) {
		//Wt::Dbo::field(a, id, "id");
		Wt::Dbo::field(a, title, "title");
		Wt::Dbo::belongsTo(a, publisher);
		Wt::Dbo::hasMany(a, stock, Wt::Dbo::ManyToOne);
	}
};





struct Stock {
	//int id;
	Wt::Dbo::ptr<Book> book;
	Wt::Dbo::ptr<Shop> shop;
	Wt::Dbo::collection<Wt::Dbo::ptr<Sale>> sale;
	int count;

	template<typename Action>
	void persist(Action& a) {
		Wt::Dbo::field(a, count, "count");
		Wt::Dbo::belongsTo(a, book);
		Wt::Dbo::belongsTo(a, shop);
		Wt::Dbo::hasMany(a, sale, Wt::Dbo::ManyToOne);
	}

};




struct Shop {
	std::string name;
	Wt::Dbo::collection<Wt::Dbo::ptr<Stock>> stock;
	

	template<typename Action>
	void persist(Action& a) {
		//Wt::Dbo::field(a, id, "id");
		Wt::Dbo::field(a, name, "name");
		Wt::Dbo::hasMany(a, stock, Wt::Dbo::ManyToOne);
	}

};





struct Sale {
	int count;
	int price;
	std::string date_sale;
	Wt::Dbo::ptr<Stock> stock;

	template<typename Action>
	void persist(Action& a) {
		//Wt::Dbo::field(a, id, "id");
		Wt::Dbo::field(a, count, "count");
		Wt::Dbo::field(a, price, "price");
		Wt::Dbo::field(a, date_sale, "date_sale");
		Wt::Dbo::belongsTo(a, stock);
	}


};




int main()
{
	//setlocale(LC_ALL, "Russian");
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	Wt::Dbo::Session session;
	try {
		std::string connString =
			"host=127.0.0.1 "
			"port=5432 "
			"dbname=orm_wtdbo "
			"user=postgres "
			"password=Cnjkbwf1988!";

		std::unique_ptr<Wt::Dbo::backend::Postgres> conn;
		conn = std::make_unique<Wt::Dbo::backend::Postgres>(connString);

		session.setConnection(std::move(conn));
		
		session.mapClass<Book>("book");
		session.mapClass<Publisher>("publisher");

		session.mapClass<Shop>("shop");
		session.mapClass<Stock>("stock");
		session.mapClass<Sale>("sale");

		try {
			session.createTables();
		}
		catch (std::exception& e) {
			//std::cout << e.what() << std::endl;
		}

	}
	catch (std::exception& e) {
		std::cout << "Error = " << e.what() << std::endl;
	}

	/*
	std::unique_ptr<Publisher> drofa{ new Publisher{"Дрофа"}};
	std::unique_ptr<Publisher> power_mashines{ new Publisher{"Силовые машины"} };
	std::unique_ptr<Publisher> lenizdat{ new Publisher{"Лениздат"} };
	std::unique_ptr<Publisher> mosizdat{ new Publisher{"Мосиздат"} };
	std::unique_ptr<Publisher> ekatizdat{ new Publisher{"Екатиздат"} };
	std::unique_ptr<Publisher> kirovizdat{ new Publisher{"Кировиздат"} };

	Wt::Dbo::Transaction t{ session };
	Wt::Dbo::ptr<Publisher> publisherPtr1 = session.add<Publisher>(std::move(drofa));
	Wt::Dbo::ptr<Publisher> publisherPtr2 = session.add<Publisher>(std::move(power_mashines));
	Wt::Dbo::ptr<Publisher> publisherPtr3 = session.add<Publisher>(std::move(lenizdat));
	Wt::Dbo::ptr<Publisher> publisherPtr4 = session.add<Publisher>(std::move(mosizdat));
	Wt::Dbo::ptr<Publisher> publisherPtr5 = session.add<Publisher>(std::move(ekatizdat));
	Wt::Dbo::ptr<Publisher> publisherPtr6 = session.add<Publisher>(std::move(kirovizdat));
	t.commit();
	*/

	return 0;
}

