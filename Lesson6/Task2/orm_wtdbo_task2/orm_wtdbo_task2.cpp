
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
		//Wt::Dbo::field(a, id, "id");
		Wt::Dbo::field(a, count, "count");
		Wt::Dbo::belongsTo(a, book);
		Wt::Dbo::belongsTo(a, shop);
		Wt::Dbo::hasMany(a, sale, Wt::Dbo::ManyToOne);
	}

};




struct Shop {
	//int id;
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
	//int id;
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
	

	/*0*/publisher_vector.push_back(std::make_unique<Publisher>("Drofa"));
	/*1*/publisher_vector.push_back(std::make_unique<Publisher>("EKSMO"));
	/*2*/publisher_vector.push_back(std::make_unique<Publisher>("Lenizdat"));
	


	/*0*/book_vector.push_back(std::make_unique <Book>("Book number 1"));
	/*1*/book_vector.push_back(std::make_unique <Book>("Book number 2"));
	/*2*/book_vector.push_back(std::make_unique <Book>("Book number 3"));
	/*3*/book_vector.push_back(std::make_unique <Book>("Book number 4"));
	/*4*/book_vector.push_back(std::make_unique <Book>("Book number 5"));
	/*5*/book_vector.push_back(std::make_unique <Book>("Book number 6"));
	/*6*/book_vector.push_back(std::make_unique <Book>("Book number 7"));
	/*7*/book_vector.push_back(std::make_unique <Book>("Book number 8"));
	/*8*/book_vector.push_back(std::make_unique <Book>("Book number 9"));


	
	
	/*0*/shop_vector.push_back(std::make_unique <Shop>("Shop number 1"));
	/*1*/shop_vector.push_back(std::make_unique <Shop>("Shop number 2"));
	/*2*/shop_vector.push_back(std::make_unique <Shop>("Shop number 3"));
	/*3*/shop_vector.push_back(std::make_unique <Shop>("Shop number 4"));
	/*4*/shop_vector.push_back(std::make_unique <Shop>("Shop number 5"));
	/*5*/shop_vector.push_back(std::make_unique <Shop>("Shop number 6"));

	
	/*0*/stock_vector.push_back(std::make_unique <Stock>(3));
	/*1*/stock_vector.push_back(std::make_unique <Stock>(7));
	/*2*/stock_vector.push_back(std::make_unique <Stock>(5));
	/*3*/stock_vector.push_back(std::make_unique <Stock>(14));
	/*4*/stock_vector.push_back(std::make_unique <Stock>(33));
	/*5*/stock_vector.push_back(std::make_unique <Stock>(54));
	/*6*/stock_vector.push_back(std::make_unique <Stock>(84));
	/*7*/stock_vector.push_back(std::make_unique <Stock>(120));
	/*8*/stock_vector.push_back(std::make_unique <Stock>(87));
	/*9*/stock_vector.push_back(std::make_unique <Stock>(23));
	/*10*/stock_vector.push_back(std::make_unique <Stock>(98));
	/*11*/stock_vector.push_back(std::make_unique <Stock>(16));
	/*12*/stock_vector.push_back(std::make_unique <Stock>(36));
	/*13*/stock_vector.push_back(std::make_unique <Stock>(87));
	/*14*/stock_vector.push_back(std::make_unique <Stock>(99));
	/*15*/stock_vector.push_back(std::make_unique <Stock>(116));
	/*16*/stock_vector.push_back(std::make_unique <Stock>(29));
	/*17*/stock_vector.push_back(std::make_unique <Stock>(36));

	
	/*0*/sale_vector.push_back(std::make_unique<Sale>(5, 560, "2023-11-07"));
	/*1*/sale_vector.push_back(std::make_unique<Sale>(52, 320, "2023-11-08"));
	/*2*/sale_vector.push_back(std::make_unique<Sale>(10, 562, "2023-11-09"));
	/*3*/sale_vector.push_back(std::make_unique<Sale>(17, 489, "2023-10-17"));
	/*4*/sale_vector.push_back(std::make_unique<Sale>(31, 987, "2024-01-07"));
	/*5*/sale_vector.push_back(std::make_unique<Sale>(89, 364, "2024-02-14"));
	/*6*/sale_vector.push_back(std::make_unique<Sale>(48, 477, "2024-03-05"));
	/*7*/sale_vector.push_back(std::make_unique<Sale>(81, 632, "2023-06-23"));
	/*8*/sale_vector.push_back(std::make_unique<Sale>(78, 355, "2023-03-09"));
	/*9*/sale_vector.push_back(std::make_unique<Sale>(32, 563, "2023-05-19"));
	/*10*/sale_vector.push_back(std::make_unique<Sale>(97, 589, "2023-08-12"));
	/*11*/sale_vector.push_back(std::make_unique<Sale>(61, 497, "2023-09-03"));
	/*12*/sale_vector.push_back(std::make_unique<Sale>(50, 425, "2024-01-01"));
	/*13*/sale_vector.push_back(std::make_unique<Sale>(46, 310, "2024-04-04"));
	/*14*/sale_vector.push_back(std::make_unique<Sale>(1, 511, "2024-02-22"));
	/*15*/sale_vector.push_back(std::make_unique<Sale>(71, 577, "2023-04-14"));
	/*16*/sale_vector.push_back(std::make_unique<Sale>(13, 433, "2023-06-24"));
	/*17*/sale_vector.push_back(std::make_unique<Sale>(99, 510, "2024-02-28"));
	

	

	

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

			std::vector<Wt::Dbo::ptr<Publisher>> publisher_ptrs;
			std::vector<Wt::Dbo::ptr<Book>> book_ptrs;
			std::vector<Wt::Dbo::ptr<Stock>> stock_ptrs;
			std::vector<Wt::Dbo::ptr<Shop>> shop_ptrs;
			std::vector<Wt::Dbo::ptr<Sale>> sale_ptrs;


			for (auto& n : publisher_vector) publisher_ptrs.push_back(session.add(std::move(n)));

			book_vector.at(0)->publisher = publisher_ptrs.at(1);
			book_vector.at(1)->publisher = publisher_ptrs.at(2);
			book_vector.at(2)->publisher = publisher_ptrs.at(1);
			book_vector.at(3)->publisher = publisher_ptrs.at(0);
			book_vector.at(4)->publisher = publisher_ptrs.at(2);
			book_vector.at(5)->publisher = publisher_ptrs.at(1);
			book_vector.at(6)->publisher = publisher_ptrs.at(0);
			book_vector.at(7)->publisher = publisher_ptrs.at(2);
			book_vector.at(8)->publisher = publisher_ptrs.at(0);

			for (auto& n : book_vector) book_ptrs.push_back(session.add(std::move(n)));
			
			for (auto& n : shop_vector) shop_ptrs.push_back(session.add(std::move(n)));

			stock_vector.at(0)->book = book_ptrs.at(3);
			stock_vector.at(1)->book = book_ptrs.at(8);
			stock_vector.at(2)->book = book_ptrs.at(0);
			stock_vector.at(3)->book = book_ptrs.at(1);
			stock_vector.at(4)->book = book_ptrs.at(2);
			stock_vector.at(5)->book = book_ptrs.at(6);
			stock_vector.at(6)->book = book_ptrs.at(3);
			stock_vector.at(7)->book = book_ptrs.at(5);
			stock_vector.at(8)->book = book_ptrs.at(4);
			stock_vector.at(9)->book = book_ptrs.at(7);
			stock_vector.at(10)->book = book_ptrs.at(8);
			stock_vector.at(11)->book = book_ptrs.at(4);
			stock_vector.at(12)->book = book_ptrs.at(2);
			stock_vector.at(13)->book = book_ptrs.at(4);
			stock_vector.at(14)->book = book_ptrs.at(1);
			stock_vector.at(15)->book = book_ptrs.at(7);
			stock_vector.at(16)->book = book_ptrs.at(6);
			stock_vector.at(17)->book = book_ptrs.at(5);

			stock_vector.at(0)->shop = shop_ptrs.at(3);
			stock_vector.at(1)->shop = shop_ptrs.at(4);
			stock_vector.at(2)->shop = shop_ptrs.at(0);
			stock_vector.at(3)->shop = shop_ptrs.at(1);
			stock_vector.at(4)->shop = shop_ptrs.at(2);
			stock_vector.at(5)->shop = shop_ptrs.at(5);
			stock_vector.at(6)->shop = shop_ptrs.at(3);
			stock_vector.at(7)->shop = shop_ptrs.at(5);
			stock_vector.at(8)->shop = shop_ptrs.at(4);
			stock_vector.at(9)->shop = shop_ptrs.at(5);
			stock_vector.at(10)->shop = shop_ptrs.at(5);
			stock_vector.at(11)->shop = shop_ptrs.at(4);
			stock_vector.at(12)->shop = shop_ptrs.at(2);
			stock_vector.at(13)->shop = shop_ptrs.at(4);
			stock_vector.at(14)->shop = shop_ptrs.at(1);
			stock_vector.at(15)->shop = shop_ptrs.at(5);
			stock_vector.at(16)->shop = shop_ptrs.at(5);
			stock_vector.at(17)->shop = shop_ptrs.at(5);


			for (auto& n : stock_vector) stock_ptrs.push_back(session.add(std::move(n)));

			sale_vector.at(0)->stock = stock_ptrs.at(17);
			sale_vector.at(1)->stock = stock_ptrs.at(16);
			sale_vector.at(2)->stock = stock_ptrs.at(15);
			sale_vector.at(3)->stock = stock_ptrs.at(14);
			sale_vector.at(4)->stock = stock_ptrs.at(13);
			sale_vector.at(5)->stock = stock_ptrs.at(12);
			sale_vector.at(6)->stock = stock_ptrs.at(11);
			sale_vector.at(7)->stock = stock_ptrs.at(10);
			sale_vector.at(8)->stock = stock_ptrs.at(9);
			sale_vector.at(9)->stock = stock_ptrs.at(8);
			sale_vector.at(10)->stock = stock_ptrs.at(7);
			sale_vector.at(11)->stock = stock_ptrs.at(6);
			sale_vector.at(12)->stock = stock_ptrs.at(5);
			sale_vector.at(13)->stock = stock_ptrs.at(4);
			sale_vector.at(14)->stock = stock_ptrs.at(3);
			sale_vector.at(15)->stock = stock_ptrs.at(2);
			sale_vector.at(16)->stock = stock_ptrs.at(1);
			sale_vector.at(17)->stock = stock_ptrs.at(0);

			for (auto& n : sale_vector) sale_ptrs.push_back(session.add(std::move(n)));

			t1.commit();

			
		}
		catch (std::exception& e) {
			std::cout << e.what() << std::endl;
		}


		//Запрос от пользователя
		std::cout << "Поиск доступных магазинов по издателю" << std::endl << std::endl;
		Wt::Dbo::ptr<Publisher> pb_ptr;
		
		Wt::Dbo::collection<Wt::Dbo::ptr<Book>> bk_collection;
		std::vector<Wt::Dbo::collection<Wt::Dbo::ptr<Stock>>> stk_collection;
		
		std::vector<Wt::Dbo::ptr<Shop>> shp_collection;
		std::string pb_name_id = "";



		std::cout << "Введите наименование или идентификатор (id) издателя" << std::endl;
		std::cin >> pb_name_id;
		std::set<int> temp; //набор для складывания туда id объектов, полученных из запросов
		Wt::Dbo::Transaction t2{ session };
		for (auto& el : bk_collection) el.remove();
		for (auto& el : stk_collection) el.clear();
		temp.clear();
		//for (auto& el : shp_collection) el.clear();
		try {
			std::stoi(pb_name_id);
			pb_ptr = session.find<Publisher>().where("id=?").bind(pb_name_id);
			if (pb_ptr.id() >= 0) {

				try {
					
					bk_collection = session.find<Book>().where("publisher_id=?").bind(std::stoi(pb_name_id));
					
					for (auto& el : bk_collection) {
						temp.insert(el.id());
					}
					
					for(const int& el : temp) stk_collection.push_back(session.find<Stock>().where("book_id=?").bind(el));

					//for (auto& el : bk_collection) stk_collection.push_back(session.find<Stock>().where("book_id=?").bind(el.id()));
					temp.clear();
					for (auto& el : stk_collection) {
						for (auto t : el) {
							temp.insert(t->shop.id());

						}
					}
					
					for (const int& el : temp) shp_collection.push_back(session.find<Shop>().where("id=?").bind(el));
					

					std::cout << "Запрашиваемый издатель продается в следующих магазинах:\n";
					for (const Wt::Dbo::ptr<Shop>& i : shp_collection) std::cout << i->name << " ";
					std::cout << std::endl;
					
				}
				catch (Wt::Dbo::Exception& e) {
					std::cout << e.what() << std::endl;
				} 

				
				//std::cout << pb_ptr->name << std::endl;
			}
			else std::cout << "Издателя с таким наименованием или id в БД не найдено" << std::endl;
			
		}
		catch (std::exception& e) {
			pb_ptr = session.find<Publisher>().where("name=?").bind(pb_name_id);
			if(pb_ptr.id() >= 0) {
				
				try {
					Wt::Dbo::ptr<Publisher> pbl_ptr = session.find<Publisher>().where("name=?").bind(pb_name_id);
					int pbl_ptr_id = pbl_ptr.id();
					bk_collection = session.find<Book>().where("publisher_id=?").bind(pbl_ptr_id);
					
					for (auto& el : bk_collection) {
						temp.insert(el.id());
					}

					for (const int& el : temp) stk_collection.push_back(session.find<Stock>().where("book_id=?").bind(el));

					//for (auto& el : bk_collection) stk_collection.push_back(session.find<Stock>().where("book_id=?").bind(el.id()));
					temp.clear();
					for (auto& el : stk_collection) {
						for (auto t : el) {
							temp.insert(t->shop.id());

						}
					}

					for (const int& el : temp) shp_collection.push_back(session.find<Shop>().where("id=?").bind(el));


					std::cout << "Запрашиваемый издатель продается в следующих магазинах:\n";
					for (const Wt::Dbo::ptr<Shop>& i : shp_collection) std::cout << i->name << " ";
					std::cout << std::endl;
				}
				catch (Wt::Dbo::Exception& e) {
					std::cout << e.what() << std::endl;

				}
				
			}
			else std::cout << "Издателя с таким наименованием или id в БД не найдено" << std::endl;
			
		}





		t2.commit();



	}
	catch (std::exception& e) {
		std::cout << "Error = " << e.what() << std::endl;
	}

	
	
	
	
	
	
	return 0;
}

