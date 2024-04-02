
#include <iostream>
#include <windows.h>
#pragma execution_character_set("utf-8")
#include <wt/dbo/dbo.h>
#include <wt/Dbo/backend/Postgres.h>

struct User {

	std::string name = "";
	std::string phone = "";
	int karma = 0;

	template<typename Action>
	void persist(Action& a) {
		Wt::Dbo::field(a, name, "name");
		Wt::Dbo::field(a, phone, "phone");
		Wt::Dbo::field(a, karma, "karma");
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
		session.mapClass<User>("user");
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
	
	std::unique_ptr<User> roma (new User);
	roma->name = "Roman";
	roma->phone = "+79218498053";
	roma->karma = 100;
	Wt::Dbo::Transaction t{session};
	session.add<User>(std::move(roma));
	t.commit();
	return 0;
}

