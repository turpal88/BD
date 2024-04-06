
#include <iostream>
#include <windows.h>
#pragma execution_character_set("utf-8")
#include <vector>
#include <algorithm>
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
	Publisher() {}
	Publisher(std::string name) : name(name) {}
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
	Book() {}
	Book(std::string title) :title(title) {}
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
	int count;
	Stock() {}
	Stock(int count) : count(count) {}
	Wt::Dbo::ptr<Book> book;
	Wt::Dbo::ptr<Shop> shop;
	Wt::Dbo::collection<Wt::Dbo::ptr<Sale>> sale;
	

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
	Shop() {}
	Shop(std::string name) :name(name) {}
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
	Sale() {}
	Sale(int count, int price, std::string date_sale) :count(count), price(price), date_sale(date_sale) {}
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


	std::vector<std::unique_ptr<Publisher>> publisher_vector;
	std::vector<std::unique_ptr<Book>> book_vector;
	std::vector<std::unique_ptr<Shop>> shop_vector;
	std::vector<std::unique_ptr<Stock>> stock_vector;
	std::vector<std::unique_ptr<Sale>> sale_vector;
	//std::vector<Wt::Dbo::ptr<Publisher>> publisher_ptr_vector;
	//std::vector<Wt::Dbo::ptr<Book>> book_ptr_vector;
	//std::vector<Wt::Dbo::ptr<Shop>> shop_ptr_vector;
	//std::vector<Wt::Dbo::ptr<Stock>> stock_ptr_vector;
	//std::vector<Wt::Dbo::ptr<Sale>> sale_ptr_vector;

	publisher_vector.push_back(std::make_unique<Publisher>("Дрофа"));
	publisher_vector.push_back(std::make_unique<Publisher>("ЭКСМО"));
	publisher_vector.push_back(std::make_unique<Publisher>("Лениздат"));
	


	book_vector.push_back(std::make_unique <Book>("Электрические машины. Учебник для ВУЗов"));
	book_vector.push_back(std::make_unique <Book>("Сказка о рыбаке и рыбке"));
	book_vector.push_back(std::make_unique <Book>("Сказка о попе и работнике его Балде"));
	book_vector.push_back(std::make_unique <Book>("А.П.Чехов. Повести"));
	book_vector.push_back(std::make_unique <Book>("Стабилизации положения бутерброда при падении. Инструкция по настройке"));
	book_vector.push_back(std::make_unique <Book>("Инструкция по определению предельного давления воздуха при надувании шариков ртом"));
	book_vector.push_back(std::make_unique <Book>("Теория автоматического управления ледянкой"));
	book_vector.push_back(std::make_unique <Book>("Определение глубины лужи при следовании в резиновых сапогах. Инструкция для дошкольных учреждений"));
	book_vector.push_back(std::make_unique <Book>("Инструкция по распутыванию наушников в кармане"));


	
	
	shop_vector.push_back(std::make_unique <Shop>("Сельмаг"));
	shop_vector.push_back(std::make_unique <Shop>("Универмаг"));
	shop_vector.push_back(std::make_unique <Shop>("Буквоед"));
	shop_vector.push_back(std::make_unique <Shop>("Читай-ка"));
	shop_vector.push_back(std::make_unique <Shop>("Ларек-марек"));
	shop_vector.push_back(std::make_unique <Shop>("Лента"));

	
	stock_vector.push_back(std::make_unique <Stock>(3));
	stock_vector.push_back(std::make_unique <Stock>(7));
	stock_vector.push_back(std::make_unique <Stock>(5));
	stock_vector.push_back(std::make_unique <Stock>(14));
	stock_vector.push_back(std::make_unique <Stock>(33));
	stock_vector.push_back(std::make_unique <Stock>(54));
	stock_vector.push_back(std::make_unique <Stock>(84));
	stock_vector.push_back(std::make_unique <Stock>(120));
	stock_vector.push_back(std::make_unique <Stock>(87));
	stock_vector.push_back(std::make_unique <Stock>(23));
	stock_vector.push_back(std::make_unique <Stock>(98));
	stock_vector.push_back(std::make_unique <Stock>(16));
	stock_vector.push_back(std::make_unique <Stock>(36));
	stock_vector.push_back(std::make_unique <Stock>(87));
	stock_vector.push_back(std::make_unique <Stock>(99));
	stock_vector.push_back(std::make_unique <Stock>(116));
	stock_vector.push_back(std::make_unique <Stock>(29));
	stock_vector.push_back(std::make_unique <Stock>(36));

	
	sale_vector.push_back(std::make_unique<Sale>(5, 560, "2023-11-07"));
	sale_vector.push_back(std::make_unique<Sale>(52, 320, "2023-11-08"));
	sale_vector.push_back(std::make_unique<Sale>(10, 562, "2023-11-09"));
	sale_vector.push_back(std::make_unique<Sale>(17, 489, "2023-10-17"));
	sale_vector.push_back(std::make_unique<Sale>(31, 987, "2024-01-07"));
	sale_vector.push_back(std::make_unique<Sale>(89, 364, "2024-02-14"));
	sale_vector.push_back(std::make_unique<Sale>(48, 477, "2024-03-05"));
	sale_vector.push_back(std::make_unique<Sale>(81, 632, "2023-06-23"));
	sale_vector.push_back(std::make_unique<Sale>(78, 355, "2023-03-09"));
	sale_vector.push_back(std::make_unique<Sale>(32, 563, "2023-05-19"));
	sale_vector.push_back(std::make_unique<Sale>(97, 589, "2023-08-12"));
	sale_vector.push_back(std::make_unique<Sale>(61, 497, "2023-09-03"));
	sale_vector.push_back(std::make_unique<Sale>(50, 425, "2024-01-01"));
	sale_vector.push_back(std::make_unique<Sale>(46, 310, "2024-04-04"));
	sale_vector.push_back(std::make_unique<Sale>(1, 511, "2024-02-22"));
	sale_vector.push_back(std::make_unique<Sale>(71, 577, "2023-04-14"));
	sale_vector.push_back(std::make_unique<Sale>(13, 433, "2023-06-24"));
	sale_vector.push_back(std::make_unique<Sale>(99, 510, "2024-02-28"));
	

	/*
	book_vector.at(0)->publisher = std::move(publisher_vector.at(2));
	book_vector.at(1)->publisher = std::move(publisher_vector.at(0));
	book_vector.at(2)->publisher = std::move(publisher_vector.at(0));
	book_vector.at(3)->publisher = std::move(publisher_vector.at(0));
	book_vector.at(4)->publisher = std::move(publisher_vector.at(1));
	book_vector.at(5)->publisher = std::move(publisher_vector.at(1));
	book_vector.at(6)->publisher = std::move(publisher_vector.at(2));
	book_vector.at(7)->publisher = std::move(publisher_vector.at(2));
	book_vector.at(8)->publisher = std::move(publisher_vector.at(1));

	book_vector.at(0)->stock.insert(std::move(stock_vector.at(3)));
	book_vector.at(0)->stock.insert(std::move(stock_vector.at(5)));
	

	book_vector.at(1)->stock.insert(std::move(stock_vector.at(4)));
	book_vector.at(1)->stock.insert(std::move(stock_vector.at(2)));
	

	book_vector.at(2)->stock.insert(std::move(stock_vector.at(1)));
	book_vector.at(2)->stock.insert(std::move(stock_vector.at(8)));

	book_vector.at(3)->stock.insert(std::move(stock_vector.at(7)));
	book_vector.at(3)->stock.insert(std::move(stock_vector.at(6)));

	book_vector.at(4)->stock.insert(std::move(stock_vector.at(9)));
	book_vector.at(4)->stock.insert(std::move(stock_vector.at(11)));

	book_vector.at(5)->stock.insert(std::move(stock_vector.at(10)));
	book_vector.at(5)->stock.insert(std::move(stock_vector.at(12)));

	book_vector.at(6)->stock.insert(std::move(stock_vector.at(13)));
	book_vector.at(6)->stock.insert(std::move(stock_vector.at(15)));

	book_vector.at(7)->stock.insert(std::move(stock_vector.at(14)));
	book_vector.at(7)->stock.insert(std::move(stock_vector.at(16)));

	book_vector.at(8)->stock.insert(std::move(stock_vector.at(17)));
	book_vector.at(8)->stock.insert(std::move(stock_vector.at(0)));

	
	

	publisher_vector.at(0)->book.insert(std::move(book_vector.at(1)));
	publisher_vector.at(0)->book.insert(std::move(book_vector.at(2)));
	publisher_vector.at(0)->book.insert(std::move(book_vector.at(3)));

	publisher_vector.at(1)->book.insert(std::move(book_vector.at(8)));
	publisher_vector.at(1)->book.insert(std::move(book_vector.at(5)));
	publisher_vector.at(1)->book.insert(std::move(book_vector.at(4)));

	publisher_vector.at(2)->book.insert(std::move(book_vector.at(7)));
	publisher_vector.at(2)->book.insert(std::move(book_vector.at(6)));
	publisher_vector.at(2)->book.insert(std::move(book_vector.at(0)));

	stock_vector.at(0)->book = std::move(book_vector.at(8));
	stock_vector.at(1)->book = std::move(book_vector.at(2));
	stock_vector.at(2)->book = std::move(book_vector.at(1));
	stock_vector.at(3)->book = std::move(book_vector.at(0));
	stock_vector.at(4)->book = std::move(book_vector.at(1));
	stock_vector.at(5)->book = std::move(book_vector.at(0));
	stock_vector.at(6)->book = std::move(book_vector.at(3));
	stock_vector.at(7)->book = std::move(book_vector.at(3));
	stock_vector.at(8)->book = std::move(book_vector.at(2));
	stock_vector.at(9)->book = std::move(book_vector.at(4));
	stock_vector.at(10)->book = std::move(book_vector.at(5));
	stock_vector.at(11)->book = std::move(book_vector.at(4));
	stock_vector.at(12)->book = std::move(book_vector.at(5));
	stock_vector.at(13)->book = std::move(book_vector.at(6));
	stock_vector.at(14)->book = std::move(book_vector.at(7));
	stock_vector.at(15)->book = std::move(book_vector.at(6));
	stock_vector.at(16)->book = std::move(book_vector.at(7));
	stock_vector.at(17)->book = std::move(book_vector.at(8));
	
	stock_vector.at(0)->shop = std::move(shop_vector.at(5));
	stock_vector.at(1)->shop = std::move(shop_vector.at(2));
	stock_vector.at(2)->shop = std::move(shop_vector.at(1));
	stock_vector.at(3)->shop = std::move(shop_vector.at(1));
	stock_vector.at(4)->shop = std::move(shop_vector.at(1));
	stock_vector.at(5)->shop = std::move(shop_vector.at(0));
	stock_vector.at(6)->shop = std::move(shop_vector.at(6));
	stock_vector.at(7)->shop = std::move(shop_vector.at(3));
	stock_vector.at(8)->shop = std::move(shop_vector.at(2));
	stock_vector.at(9)->shop = std::move(shop_vector.at(3));
	stock_vector.at(10)->shop = std::move(shop_vector.at(3));
	stock_vector.at(11)->shop = std::move(shop_vector.at(5));
	stock_vector.at(12)->shop = std::move(shop_vector.at(4));
	stock_vector.at(13)->shop = std::move(shop_vector.at(5));
	stock_vector.at(14)->shop = std::move(shop_vector.at(0));
	stock_vector.at(15)->shop = std::move(shop_vector.at(4));
	stock_vector.at(16)->shop = std::move(shop_vector.at(0));
	stock_vector.at(17)->shop = std::move(shop_vector.at(4));
	
	stock_vector.at(0)->sale.insert(std::move(sale_vector.at(2)));
	stock_vector.at(1)->sale.insert(std::move(sale_vector.at(4)));
	stock_vector.at(2)->sale.insert(std::move(sale_vector.at(6)));
	stock_vector.at(3)->sale.insert(std::move(sale_vector.at(8)));
	stock_vector.at(4)->sale.insert(std::move(sale_vector.at(10)));
	stock_vector.at(5)->sale.insert(std::move(sale_vector.at(12)));
	stock_vector.at(6)->sale.insert(std::move(sale_vector.at(14)));
	stock_vector.at(7)->sale.insert(std::move(sale_vector.at(16)));
	stock_vector.at(8)->sale.insert(std::move(sale_vector.at(1)));
	stock_vector.at(9)->sale.insert(std::move(sale_vector.at(3)));
	stock_vector.at(10)->sale.insert(std::move(sale_vector.at(5)));
	stock_vector.at(11)->sale.insert(std::move(sale_vector.at(7)));
	stock_vector.at(12)->sale.insert(std::move(sale_vector.at(9)));
	stock_vector.at(13)->sale.insert(std::move(sale_vector.at(11)));
	stock_vector.at(14)->sale.insert(std::move(sale_vector.at(13)));
	stock_vector.at(15)->sale.insert(std::move(sale_vector.at(15)));
	stock_vector.at(16)->sale.insert(std::move(sale_vector.at(17)));
	stock_vector.at(17)->sale.insert(std::move(sale_vector.at(0)));

	shop_vector.at(0)->stock.insert(std::move(stock_vector.at(5)));
	shop_vector.at(0)->stock.insert(std::move(stock_vector.at(14)));
	shop_vector.at(0)->stock.insert(std::move(stock_vector.at(16)));

	shop_vector.at(1)->stock.insert(std::move(stock_vector.at(3)));
	shop_vector.at(1)->stock.insert(std::move(stock_vector.at(2)));
	shop_vector.at(1)->stock.insert(std::move(stock_vector.at(4)));

	shop_vector.at(2)->stock.insert(std::move(stock_vector.at(1)));
	shop_vector.at(2)->stock.insert(std::move(stock_vector.at(6)));
	shop_vector.at(2)->stock.insert(std::move(stock_vector.at(8)));

	shop_vector.at(3)->stock.insert(std::move(stock_vector.at(7)));
	shop_vector.at(3)->stock.insert(std::move(stock_vector.at(9)));
	shop_vector.at(3)->stock.insert(std::move(stock_vector.at(10)));

	shop_vector.at(4)->stock.insert(std::move(stock_vector.at(15)));
	shop_vector.at(4)->stock.insert(std::move(stock_vector.at(12)));
	shop_vector.at(4)->stock.insert(std::move(stock_vector.at(17)));

	shop_vector.at(5)->stock.insert(std::move(stock_vector.at(11)));
	shop_vector.at(5)->stock.insert(std::move(stock_vector.at(13)));
	shop_vector.at(5)->stock.insert(std::move(stock_vector.at(0)));

	sale_vector.at(0)->stock = std::move(stock_vector.at(17));
	sale_vector.at(1)->stock = std::move(stock_vector.at(8));
	sale_vector.at(2)->stock = std::move(stock_vector.at(0));
	sale_vector.at(3)->stock = std::move(stock_vector.at(9));
	sale_vector.at(4)->stock = std::move(stock_vector.at(1));
	sale_vector.at(5)->stock = std::move(stock_vector.at(10));
	sale_vector.at(6)->stock = std::move(stock_vector.at(2));
	sale_vector.at(7)->stock = std::move(stock_vector.at(11));
	sale_vector.at(8)->stock = std::move(stock_vector.at(3));
	sale_vector.at(9)->stock = std::move(stock_vector.at(12));
	sale_vector.at(10)->stock = std::move(stock_vector.at(4));
	sale_vector.at(11)->stock = std::move(stock_vector.at(13));
	sale_vector.at(12)->stock = std::move(stock_vector.at(5));
	sale_vector.at(13)->stock = std::move(stock_vector.at(14));
	sale_vector.at(14)->stock = std::move(stock_vector.at(6));
	sale_vector.at(15)->stock = std::move(stock_vector.at(15));
	sale_vector.at(16)->stock = std::move(stock_vector.at(7));
	sale_vector.at(17)->stock = std::move(stock_vector.at(16));
	*/

	

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
			Wt::Dbo::Transaction t1{ session };

			for (auto& n : publisher_vector) session.add(std::move(n));
			
			for (auto& n : book_vector) session.add(std::move(n));
			
			for (auto& n : stock_vector) session.add(std::move(n));
			
			for (auto& n : shop_vector) session.add(std::move(n));
			
			for (auto& n : sale_vector) session.add(std::move(n));
			


			t1.commit();

			Wt::Dbo::Transaction t2{ session };
			
			Wt::Dbo::collection<Wt::Dbo::ptr<Publisher>> publisher_ptrs = session.find<Publisher>();
			Wt::Dbo::collection<Wt::Dbo::ptr<Book>> book_ptrs = session.find<Book>();
			Wt::Dbo::collection<Wt::Dbo::ptr<Stock>> stock_ptrs = session.find<Stock>();
			Wt::Dbo::collection<Wt::Dbo::ptr<Shop>> shop_ptrs = session.find<Shop>();
			Wt::Dbo::collection<Wt::Dbo::ptr<Sale>> sale_ptrs = session.find<Sale>();

			
			publisher_vector.at(0)->book.insert(std::move(book_vector.at(1)));
			publisher_vector.at(0)->book.insert(std::move(book_vector.at(2)));
			publisher_vector.at(0)->book.insert(std::move(book_vector.at(3)));

			publisher_vector.at(1)->book.insert(std::move(book_vector.at(8)));
			publisher_vector.at(1)->book.insert(std::move(book_vector.at(5)));
			publisher_vector.at(1)->book.insert(std::move(book_vector.at(4)));

			publisher_vector.at(2)->book.insert(std::move(book_vector.at(7)));
			publisher_vector.at(2)->book.insert(std::move(book_vector.at(6)));
			publisher_vector.at(2)->book.insert(std::move(book_vector.at(0)));
			
			t2.commit();

		}
		catch (std::exception& e) {
			//std::cout << e.what() << std::endl;
		}

	}
	catch (std::exception& e) {
		std::cout << "Error = " << e.what() << std::endl;
	}

	
	
	
	
	
	return 0;
}

