#pragma once
#include "windows.h"
#include "phone_book.hpp"
#include <stdlib.h>
#include <iostream>
#include <string>

#include <pqxx/pqxx>
#include "pqxx/nontransaction"
#include "user_interconnection.hpp"

int main() {
	//setlocale(LC_ALL, "");
	SetConsoleCP(1251);// установка кодовой страницы win-cp 1251 в поток ввода
	SetConsoleOutputCP(1251); // установка кодовой страницы win-cp 1251 в поток вывода

	PhoneBook my_phone_book;
	my_phone_book.set_db_connection_parameters();

	std::string connection_string =
		"host=" + my_phone_book._host_name()
		+ " port=" + my_phone_book._port()
		+ " dbname=" + my_phone_book._dbname()
		+ " user=" + my_phone_book._user()
		+ " password=" + my_phone_book._password();

	std::cout << std::endl;



	//tx.exec("CREATE TABLE IF NOT EXISTS Phone_Book(id integer not null primary key, name varchar(50), surname varchar(50), email varchar(50), mobile_phone varchar(50));");

	//std::string connection_string = "host=localhost port=5432 dbname=phone_book user=postgres password=Cnjkbwf1988";

	try {

		pqxx::connection c(connection_string);
		

		pqxx::nontransaction tx(c);

		std::cout << std::endl;

		
		

		//создаем буферную переменную для хранения ответов на всякие каверзные вопросы
		std::string answer1;
		//спрашиваем как будет называться или уже должна называться наша таблица (телефонная книга)
		int answer2; //переменная для ответа на вопрос о дальнейших действиях
		while (true) {
			//system("CLS");
			std::cout << "Введите название телефонной книги (название таблицы),\nкоторую будем создавать или которая уже должна существовать в БД: ";
			std::cin >> answer1;
			if (!my_phone_book.is_name_valid(answer1)) {
				std::cout << "Введенное имя таблицы содержит недопустимые символы.\n1.Ввести название заново\n2.Покинуть чат\n";
				while (true) {
					std::cin >> answer2;
					if (answer2 == 1 || answer2 == 2) {
						if (answer2 == 2) {
							std::cout << "Всех благ";
							
							return -1;
						}
						else break;
					}
					else {
						std::cout << "Введите нормальный ответ: "; continue;
					}
				}
			}
			else break;

		}

		//

		//std::tuple используется т.к. tx.query<std::sring> возвращает std::tuple
		std::vector<std::tuple<std::string>> already_existing_tables_in_db;
		std::vector<std::tuple<std::string, std::string, std::string>> already_existing_columns_in_table_in_db;
		//std::vector<std::tuple<std::string>> temp_vector_with_current_column_names;
		//имя и фамилия для проверки дубликата клиента в БД
		std::string poped_name_surname_email_has_mobile_phone_from_table[4] = {"","","",""};
		std::vector<std::string> poped_mobile_phones_from_table;
		std::vector<int> poped_client_id_from_table;
		//очищаем вектора хранения имен в таблице
		already_existing_tables_in_db.clear();


		//сперва проверяем есть ли в базе какие-нибудь таблицы, имена складываем в вектор
		for (std::tuple<std::string> name : tx.query<std::string>("select table_name from information_schema.tables where table_schema = 'public'")) {
			//std::cout << name << std::endl;
			already_existing_tables_in_db.push_back(name);
		}
		if (already_existing_tables_in_db.size() == 0) {
			//запушим в класс имя таблицы чтобы потом легко использовать его
			my_phone_book.push_table_name(answer1);
			system("CLS");
			tx.exec(my_phone_book.create_new_main_table());
			//tx.commit();
			tx.exec(my_phone_book.create_new_aux_table());
			//tx.commit();
			//tx.query_value<std::string>(my_phone_book.create_new_main_table());
			//tx.query_value<std::string>(my_phone_book.create_new_aux_table());
			my_phone_book.is_main_table_created = true; //switch_way = 1; //- основная таблица создана
			my_phone_book.is_aux_table_created = true; //switch_way = 1; //- вспомогательная таблица создана
			//std::cout << "Основная таблица создана" << std::endl;
			//std::cout << "Вспомогательная таблица создана" << std::endl;
		}
		else {
			//проверка чтобы обязательно одну из таблиц звали как в answer1
			for (auto it = already_existing_tables_in_db.begin(); it != already_existing_tables_in_db.end(); ++it) { /*начало цикла*/
				//если найдено название основной таблицы
				if (std::get<0>(*it) == answer1) {
					//запушим в класс имя таблицы чтобы потом легко использовать его
					my_phone_book.push_table_name(answer1);
					my_phone_book.main_table_column_count = tx.query_value<int>("SELECT COUNT(*) FROM INFORMATION_SCHEMA.COLUMNS WHERE table_name = '" + std::get<0>(*it) + "'");
					//запрашиваем сколько столбцов в таблице
					if (my_phone_book.main_table_column_count == 5) {
						already_existing_columns_in_table_in_db.clear();
						for (auto [column_name, is_nullable, udt_name] : tx.query<std::string, std::string, std::string>(my_phone_book.is_main_table_valid())) {
							already_existing_columns_in_table_in_db.push_back(std::make_tuple(column_name, is_nullable, udt_name));
						}

						if ((std::get<0>(already_existing_columns_in_table_in_db.at(0)) != "id"
							|| std::get<1>(already_existing_columns_in_table_in_db.at(0)) != "NO"
							|| std::get<2>(already_existing_columns_in_table_in_db.at(0)) != "int4") && !my_phone_book.is_main_table_created) {

							system("CLS");
							std::cout << "В БД таблица с таким названием присутствует. Cтолбец \"id\" некорректен. Выберите дальнейшие действия:" << std::endl;
							std::cout << "1.Удалить текущую таблицу и создать новую структуру телефонной книги (таблицу)" << std::endl;
							std::cout << "2.Покинуть чат" << std::endl;
							while (true) {
								std::cin >> answer2;
								if (!my_phone_book.ask_ans_cycle_request(answer2)) {
									std::cout << "Введите корректный ответ: ";
									continue;
								}
								else if (my_phone_book.ask_ans_cycle_request(answer2) == 1) {
									tx.exec(my_phone_book.delete_main_table());
									//tx.commit();
									tx.exec(my_phone_book.create_new_main_table());
									//tx.commit();
									//tx.query_value<std::string>(my_phone_book.delete_main_table());
									//tx.query_value<std::string>(my_phone_book.create_new_main_table());
									my_phone_book.is_main_table_created = true; //switch_way = 1; //- основная таблица создана
									//system("CLS");
									//std::cout << "Основная таблица создана" << std::endl;
									break;
								}
								else if (my_phone_book.ask_ans_cycle_request(answer2) == 2) return -1;
							}
						}
						if ((std::get<0>(already_existing_columns_in_table_in_db.at(1)) != "name"
							|| std::get<1>(already_existing_columns_in_table_in_db.at(1)) != "NO"
							|| std::get<2>(already_existing_columns_in_table_in_db.at(1)) != "varchar") && !my_phone_book.is_main_table_created) {

							system("CLS");
							std::cout << "В БД таблица с таким названием присутствует. Cтолбец \"name\" столбец некорректен. Выберите дальнейшие действия:" << std::endl;
							std::cout << "1.Удалить текущую таблицу и создать новую структуру телефонной книги (таблицу)" << std::endl;
							std::cout << "2.Покинуть чат" << std::endl;
							while (true) {
								std::cin >> answer2;
								if (!my_phone_book.ask_ans_cycle_request(answer2)) {
									std::cout << "Введите корректный ответ: ";
									continue;
								}
								else if (my_phone_book.ask_ans_cycle_request(answer2) == 1) {
									tx.exec(my_phone_book.delete_main_table());
									//tx.commit();
									tx.exec(my_phone_book.create_new_main_table());
									//tx.commit();
									//tx.query_value<std::string>(my_phone_book.delete_main_table());
									//tx.query_value<std::string>(my_phone_book.create_new_main_table());
									my_phone_book.is_main_table_created = true; //switch_way = 1; //- основная таблица создана
									//system("CLS");
									//std::cout << "Основная таблица создана" << std::endl;
									break;
								}
								else if (my_phone_book.ask_ans_cycle_request(answer2) == 2) return -1;
							}
						}

						if ((std::get<0>(already_existing_columns_in_table_in_db.at(2)) != "surname"
							|| std::get<1>(already_existing_columns_in_table_in_db.at(2)) != "NO"
							|| std::get<2>(already_existing_columns_in_table_in_db.at(2)) != "varchar") && !my_phone_book.is_main_table_created) {
							system("CLS");
							std::cout << "В БД таблица с таким названием присутствует. Cтолбец \"surname\" некорректен. Выберите дальнейшие действия:" << std::endl;
							std::cout << "1.Удалить текущую таблицу и создать новую структуру телефонной книги (таблицу)" << std::endl;
							std::cout << "2.Покинуть чат" << std::endl;
							while (true) {
								std::cin >> answer2;
								if (!my_phone_book.ask_ans_cycle_request(answer2)) {
									std::cout << "Введите корректный ответ: ";
									continue;
								}
								else if (my_phone_book.ask_ans_cycle_request(answer2) == 1) {
									tx.exec(my_phone_book.delete_main_table());
									//tx.commit();
									tx.exec(my_phone_book.create_new_main_table());
									//tx.commit();
									//tx.query_value<std::string>(my_phone_book.delete_main_table());
									//tx.query_value<std::string>(my_phone_book.create_new_main_table());
									my_phone_book.is_main_table_created = true; //switch_way = 1; //- основная таблица создана
									//system("CLS");
									//std::cout << "Основная таблица создана" << std::endl;
									break;
								}
								else if (my_phone_book.ask_ans_cycle_request(answer2) == 2) return -1;
							}

						}

						if ((std::get<0>(already_existing_columns_in_table_in_db.at(3)) != "email"
							|| std::get<1>(already_existing_columns_in_table_in_db.at(3)) != "NO"
							|| std::get<2>(already_existing_columns_in_table_in_db.at(3)) != "varchar") && !my_phone_book.is_main_table_created) {
							system("CLS");
							std::cout << "В БД таблица с таким названием присутствует. Cтолбец \"email\" некорректен. Выберите дальнейшие действия:" << std::endl;
							std::cout << "1.Удалить текущую таблицу и создать новую структуру телефонной книги (таблицу)" << std::endl;
							std::cout << "2.Покинуть чат" << std::endl;
							while (true) {
								std::cin >> answer2;
								if (!my_phone_book.ask_ans_cycle_request(answer2)) {
									std::cout << "Введите корректный ответ: ";
									continue;
								}
								else if (my_phone_book.ask_ans_cycle_request(answer2) == 1) {
									tx.exec(my_phone_book.delete_main_table());
									//tx.commit();
									tx.exec(my_phone_book.create_new_main_table());
									//tx.commit();
									//tx.query_value<std::string>(my_phone_book.delete_main_table());
									//tx.query_value<std::string>(my_phone_book.create_new_main_table());
									my_phone_book.is_main_table_created = true; //switch_way = 1; //- основная таблица создана
									//system("CLS");
									//std::cout << "Основная таблица создана" << std::endl;
									break;
								}
								else if (my_phone_book.ask_ans_cycle_request(answer2) == 2) return -1;
							}

						}

						if ((std::get<0>(already_existing_columns_in_table_in_db.at(4)) != "has_mobile_phone"
							|| std::get<1>(already_existing_columns_in_table_in_db.at(4)) != "NO"
							|| std::get<2>(already_existing_columns_in_table_in_db.at(4)) != "varchar") && !my_phone_book.is_main_table_created) {

							system("CLS");
							std::cout << "В БД таблица с таким названием присутствует. Cтолбец \"has_mobile_phone\" некорректен. Выберите дальнейшие действия:" << std::endl;
							std::cout << "1.Удалить текущую таблицу и создать новую структуру телефонной книги (таблицу)" << std::endl;
							std::cout << "2.Покинуть чат" << std::endl;
							while (true) {
								std::cin >> answer2;
								if (!my_phone_book.ask_ans_cycle_request(answer2)) {
									std::cout << "Введите корректный ответ: ";
									continue;
								}
								else if (my_phone_book.ask_ans_cycle_request(answer2) == 1) {
									tx.exec(my_phone_book.delete_main_table());
									//tx.commit();
									tx.exec(my_phone_book.create_new_main_table());
									//tx.commit();
									//tx.query_value<std::string>(my_phone_book.delete_main_table());
									//tx.query_value<std::string>(my_phone_book.create_new_main_table());
									my_phone_book.is_main_table_created = true; //switch_way = 1; //- основная таблица создана
									//system("CLS");
									//std::cout << "Основная таблица создана" << std::endl;
									break;
								}
								else if (my_phone_book.ask_ans_cycle_request(answer2) == 2) return -1;
							}
						}








						//если прошли все вышеперечисленные условия, взводим флаг что таблица создана (есть в БД и все стобцы валидны)
						if (!my_phone_book.is_main_table_created) my_phone_book.is_main_table_created = true;


					}
					else {
						system("CLS");
						std::cout << "В БД таблица с таким названием присутствует. Количество столбцов некорректно. Выберите дальнейшие действия:" << std::endl;
						std::cout << "1.Удалить текущую таблицу и создать новую структуру телефонной книги (таблицу)" << std::endl;
						std::cout << "2.Покинуть чат" << std::endl;
						while (true) {
							std::cin >> answer2;
							if (!my_phone_book.ask_ans_cycle_request(answer2)) {
								std::cout << "Введите корректный ответ: ";
								continue;
							}
							else if (my_phone_book.ask_ans_cycle_request(answer2) == 1) {
								tx.exec(my_phone_book.delete_main_table());
								//tx.commit();
								tx.exec(my_phone_book.create_new_main_table());
								//tx.commit();
								//tx.query_value<std::string>(my_phone_book.delete_main_table());
								//tx.query_value<std::string>(my_phone_book.create_new_main_table());
								my_phone_book.is_main_table_created = true; //switch_way = 1; //- основная таблица создана
								//system("CLS");
								//std::cout << "Основная таблица создана" << std::endl;
								break;
							}
							else if (my_phone_book.ask_ans_cycle_request(answer2) == 2) return -1;
						}
					}
				}
				//если найдено название вспомогательной таблицы
				else if (std::get<0>(*it) == answer1 + "_aux") {
					//запушим в класс имя таблицы чтобы потом легко использовать его
					if (my_phone_book.get_table_name() == "") my_phone_book.push_table_name(answer1);
					my_phone_book.aux_table_column_count = tx.query_value<int>("SELECT COUNT(*) FROM INFORMATION_SCHEMA.COLUMNS WHERE table_name = '" + std::get<0>(*it) + "'");
					if (my_phone_book.aux_table_column_count == 3) {
						already_existing_columns_in_table_in_db.clear();
						for (auto [column_name, is_nullable, udt_name] : tx.query<std::string, std::string, std::string>(my_phone_book.is_aux_table_valid())) {
							already_existing_columns_in_table_in_db.push_back(std::make_tuple(column_name, is_nullable, udt_name));
						}
						if ((std::get<0>(already_existing_columns_in_table_in_db.at(0)) != "id"
							|| std::get<1>(already_existing_columns_in_table_in_db.at(0)) != "NO"
							|| std::get<2>(already_existing_columns_in_table_in_db.at(0)) != "int4") && !my_phone_book.is_aux_table_created) {

							system("CLS");
							std::cout << "В БД вспомогательная таблица для основной таблицы присутствует. Cтолбец \"id\" некорректен. Выберите дальнейшие действия:" << std::endl;
							std::cout << "1.Удалить текущую вспомогательную таблицу и создать новую структуру вспомогательной таблицы" << std::endl;
							std::cout << "2.Покинуть чат" << std::endl;
							while (true) {
								std::cin >> answer2;
								if (!my_phone_book.ask_ans_cycle_request(answer2)) {
									std::cout << "Введите корректный ответ: ";
									continue;
								}
								else if (my_phone_book.ask_ans_cycle_request(answer2) == 1) {
									tx.exec(my_phone_book.delete_aux_table());
									//tx.commit();
									tx.exec(my_phone_book.create_new_aux_table());
									//tx.commit();
									//tx.query_value<std::string>(my_phone_book.delete_aux_table());
									//tx.query_value<std::string>(my_phone_book.create_new_aux_table());
									my_phone_book.is_aux_table_created = true; //switch_way = 1; //- вспомогательная таблица создана
									//my_phone_book.switch_way = 1; //switch_way = 1; //- основная таблица создана
									//system("CLS");
									//std::cout << "Вспомогательная таблица создана" << std::endl;
									break;
								}
								else if (my_phone_book.ask_ans_cycle_request(answer2) == 2) return -1;
							}
						}

						if ((std::get<0>(already_existing_columns_in_table_in_db.at(1)) != "phone_book_id"
							|| std::get<1>(already_existing_columns_in_table_in_db.at(1)) != "YES"
							|| std::get<2>(already_existing_columns_in_table_in_db.at(1)) != "int4") && !my_phone_book.is_aux_table_created) {

							system("CLS");
							std::cout << "В БД вспомогательная таблица для основной таблицы присутствует. Cтолбец \"phone_book_id\" некорректен. Выберите дальнейшие действия:" << std::endl;
							std::cout << "1.Удалить текущую вспомогательную таблицу и создать новую структуру вспомогательной таблицы" << std::endl;
							std::cout << "2.Покинуть чат" << std::endl;
							while (true) {
								std::cin >> answer2;
								if (!my_phone_book.ask_ans_cycle_request(answer2)) {
									std::cout << "Введите корректный ответ: ";
									continue;
								}
								else if (my_phone_book.ask_ans_cycle_request(answer2) == 1) {
									tx.exec(my_phone_book.delete_aux_table());
									//tx.commit();
									tx.exec(my_phone_book.create_new_aux_table());
									//tx.commit();
									//tx.query_value<std::string>(my_phone_book.delete_aux_table());
									//tx.query_value<std::string>(my_phone_book.create_new_aux_table());
									my_phone_book.is_aux_table_created = true; //switch_way = 1; //- вспомогательная таблица создана
									//my_phone_book.switch_way = 1; //switch_way = 1; //- основная таблица создана
									//system("CLS");
									//std::cout << "Вспомогательная таблица создана" << std::endl;
									break;
								}
								else if (my_phone_book.ask_ans_cycle_request(answer2) == 2) return -1;
							}
						}

						if ((std::get<0>(already_existing_columns_in_table_in_db.at(2)) != "mobile_phone"
							|| std::get<1>(already_existing_columns_in_table_in_db.at(2)) != "YES"
							|| std::get<2>(already_existing_columns_in_table_in_db.at(2)) != "varchar") && !my_phone_book.is_aux_table_created) {

							system("CLS");
							std::cout << "В БД вспомогательная таблица для основной таблицы присутствует. Cтолбец \"mobile_phone\" некорректен. Выберите дальнейшие действия:" << std::endl;
							std::cout << "1.Удалить текущую вспомогательную таблицу и создать новую структуру вспомогательной таблицы" << std::endl;
							std::cout << "2.Покинуть чат" << std::endl;
							while (true) {
								std::cin >> answer2;
								if (!my_phone_book.ask_ans_cycle_request(answer2)) {
									std::cout << "Введите корректный ответ: ";
									continue;
								}
								else if (my_phone_book.ask_ans_cycle_request(answer2) == 1) {
									tx.exec(my_phone_book.delete_aux_table());
									//tx.commit();
									tx.exec(my_phone_book.create_new_aux_table());
									//tx.commit();
									//tx.query_value<std::string>(my_phone_book.delete_aux_table());
									//tx.query_value<std::string>(my_phone_book.create_new_aux_table());
									my_phone_book.is_aux_table_created = true; //switch_way = 1; //- вспомогательная таблица создана
									//my_phone_book.switch_way = 1; //switch_way = 1; //- основная таблица создана
									//system("CLS");
									//std::cout << "Вспомогательная таблица создана" << std::endl;
									break;
								}
								else if (my_phone_book.ask_ans_cycle_request(answer2) == 2) return -1;
							}
						}

						// если прошли все вышеперечисленные условия, взводим флаг что таблица создана(есть в БД и все стобцы валидны)
						if (!my_phone_book.is_aux_table_created) my_phone_book.is_aux_table_created = true;
					}
					else {
						system("CLS");
						std::cout << "В БД вспомогательная таблица для основной таблицы присутствует. Количество столбцов некорректно. Выберите дальнейшие действия:" << std::endl;
						std::cout << "1.Удалить текущую вспомогательную  таблицу и создать новую структуру вспомогательной таблицы" << std::endl;
						std::cout << "2.Покинуть чат" << std::endl;
						while (true) {
							std::cin >> answer2;
							if (my_phone_book.ask_ans_cycle_request(answer2) == -1) {
								std::cout << "Введите корректный ответ: ";
								continue;
							}
							else if (my_phone_book.ask_ans_cycle_request(answer2) == 1) {
								tx.exec(my_phone_book.delete_aux_table());
								//tx.commit();
								tx.exec(my_phone_book.create_new_aux_table());
								//tx.commit();
								//tx.query_value<std::string>(my_phone_book.delete_aux_table());
								//tx.query_value<std::string>(my_phone_book.create_new_aux_table());
								my_phone_book.is_aux_table_created = true; //switch_way = 1; //- вспомогательная таблица создана
								//system("CLS");
								//std::cout << "Вспомогательная таблица создана" << std::endl;
								break;
							}
							else if (my_phone_book.ask_ans_cycle_request(answer2) == 2) return -1;
						}
					}
				}
				//если перебрали все имена таблиц в базе и phone_book нет, сообщаем об этом
				if (it == already_existing_tables_in_db.end() - 1) {
					my_phone_book.push_table_name(answer1);
					if (!my_phone_book.is_main_table_created) {
						system("CLS");
						std::cout << "В БД отсутствует основная таблица с таким названием. Выберите дальнейшие действия:" << std::endl;
						std::cout << "1.Создать структуру телефонной книги (основную таблицу)" << std::endl;
						std::cout << "2.Покинуть чат" << std::endl;
						while (true) {
							std::cin >> answer2;
							if (!my_phone_book.ask_ans_cycle_request(answer2)) {
								std::cout << "Введите корректный ответ: ";
								continue;
							}
							else if (my_phone_book.ask_ans_cycle_request(answer2) == 1) {
								tx.exec(my_phone_book.create_new_main_table());
								//tx.commit();
								//tx.query_value<std::string>(my_phone_book.create_new_main_table());
								//tx.query_value<std::string>(my_phone_book.create_new_aux_table());
								my_phone_book.is_main_table_created = true; //switch_way = 1; //- основная таблица создана
								//system("CLS");
								//std::cout << "Основная таблица создана" << std::endl;
								//my_phone_book.is_aux_table_created = true; //switch_way = 1; //- вспомогательная таблица создана
								break;
							}
							else if (my_phone_book.ask_ans_cycle_request(answer2) == 2) return -1;
						}
					}
					if (!my_phone_book.is_aux_table_created) {
						system("CLS");
						std::cout << "В БД отсутствует вспомогательная таблица для основной. Выберите дальнейшие действия:" << std::endl;
						std::cout << "1.Создать структуру вспомогательной таблицы" << std::endl;
						std::cout << "2.Покинуть чат" << std::endl;
						while (true) {
							std::cin >> answer2;
							if (!my_phone_book.ask_ans_cycle_request(answer2)) {
								std::cout << "Введите корректный ответ: ";
								continue;
							}
							else if (my_phone_book.ask_ans_cycle_request(answer2) == 1) {
								tx.exec(my_phone_book.create_new_aux_table());
								//tx.commit();

								//tx.query_value<std::string>(my_phone_book.create_new_aux_table());
								my_phone_book.is_aux_table_created = true; //switch_way = 1; //- основная таблица создана
								//system("CLS");
								//std::cout << "Вспомогательная таблица создана" << std::endl;
								//my_phone_book.is_aux_table_created = true; //switch_way = 1; //- вспомогательная таблица создана
								break;
							}
							else if (my_phone_book.ask_ans_cycle_request(answer2) == 2) return -1;
						}
					}
				}
			} /*конец цикла*/
		}
		system("CLS");
		if (my_phone_book.is_main_table_created) std::cout << "Основная таблица создана" << std::endl;
		if (my_phone_book.is_aux_table_created) std::cout << "Вспомогательная таблица создана" << std::endl;
		c.set_client_encoding("WIN1251");
		std::cout << c.get_client_encoding() << "\n";
		/*Работа с данными в таблицах БД */
		
		UserAction ua;
		int operation_index = -1;
		while (operation_index != 7) {
			
			ua.user_interconnection(operation_index);
			
			
			switch (operation_index) {
			case(1):
				for (auto& n : poped_name_surname_email_has_mobile_phone_from_table) n = "";
				for (auto [poped_name, poped_surname] : tx.query<std::string, std::string>(my_phone_book.check_duplicate_client(ua.input_data.at(0), ua.input_data.at(1)))) {
					poped_name_surname_email_has_mobile_phone_from_table[0] = poped_name;
					poped_name_surname_email_has_mobile_phone_from_table[1] = poped_surname;
				}
				if (poped_name_surname_email_has_mobile_phone_from_table[0] != "") {
					std::cout << "\nКлиент с таким именем и фамилией уже существует в базе\n\n"; 
					operation_index = -1;
					break;
				}
				tx.exec(my_phone_book.add_new_client(ua.input_data));
				if (ua.input_data.size() > 4) {
					//int client_id = tx.query_value<int>("select id from " + my_phone_book.get_table_name() + " where name = '" + ua.input_data.at(0) + "' and surname = '" + ua.input_data.at(1) + "'");
					for (int i = 4; i < ua.input_data.size(); i++) {
						tx.exec(my_phone_book.insert_value_into_aux_table(tx.query_value<int>(my_phone_book.get_client_id(ua.input_data.at(0), ua.input_data.at(1))), ua.input_data.at(i)));
					}
				}
				std::cout << std::endl;
				std::cout << "Клиент успешно добавлен в телефонную книгу" << std::endl; 
				operation_index = -1;
				break;
			case(2):
				for (auto& n : poped_name_surname_email_has_mobile_phone_from_table) n = "";
				poped_mobile_phones_from_table.erase(poped_mobile_phones_from_table.begin(), poped_mobile_phones_from_table.end());
				for (auto [poped_name, poped_surname] : tx.query<std::string, std::string>(my_phone_book.check_duplicate_client(ua.input_data.at(0), ua.input_data.at(1)))) {
					poped_name_surname_email_has_mobile_phone_from_table[0] = poped_name;
					poped_name_surname_email_has_mobile_phone_from_table[1] = poped_surname;
				}
				if (poped_name_surname_email_has_mobile_phone_from_table[0] == "") {
					std::cout << "\nКлиента с таким именем и фамилией в базе не найдено\n\n";
					operation_index = -1;
					break;
				}
				else {
					for (const auto& n : tx.query<std::string>(my_phone_book.get_mobile_phone(ua.input_data.at(0), ua.input_data.at(1), tx.query_value<int>(my_phone_book.get_client_id(ua.input_data.at(0), ua.input_data.at(1)))))) {
						poped_mobile_phones_from_table.push_back(std::get<0>(n));
					}
					
					if (poped_mobile_phones_from_table.size() == 0) {
						for (int i = 2; i < ua.input_data.size(); i++) {
							tx.exec(my_phone_book.insert_value_into_aux_table(tx.query_value<int>(my_phone_book.get_client_id(ua.input_data.at(0), ua.input_data.at(1))), ua.input_data.at(i)));
						}
					}
					else {
						for (int i = 2; i < ua.input_data.size(); i++) {
							if (std::find(poped_mobile_phones_from_table.begin(), poped_mobile_phones_from_table.end(), ua.input_data.at(i)) == poped_mobile_phones_from_table.end())

								tx.exec(my_phone_book.insert_value_into_aux_table(tx.query_value<int>(my_phone_book.get_client_id(ua.input_data.at(0), ua.input_data.at(1))), ua.input_data.at(i)));
							else {
								std::cout << "\nНомер телефона " + ua.input_data.at(i) + " уже записан в телефонной книге и повторно записан не будет\n";
								continue;
							}
						}
					}
					//int client_id = tx.query_value<int>("select id from " + my_phone_book.get_table_name() + " where name = '" + ua.input_data.at(0) + "' and surname = '" + ua.input_data.at(1) + "'");
					
					if (tx.query_value<std::string>(my_phone_book.get_is_client_has_mobile_phone(ua.input_data.at(0), ua.input_data.at(1))) == "no")
						tx.exec(my_phone_book.put_is_client_has_mobile_phone(ua.input_data.at(0), ua.input_data.at(1), "yes"));
					std::cout << "Введенные номера телефонов успешно добавлены к клиенту " + ua.input_data.at(0) + " " + ua.input_data.at(1) + "\n";
					operation_index = -1;

				}
				
				break;
			case(3):
				
					for (auto& n : poped_name_surname_email_has_mobile_phone_from_table) n = "";
					poped_mobile_phones_from_table.erase(poped_mobile_phones_from_table.begin(), poped_mobile_phones_from_table.end());
					for (auto [poped_name, poped_surname] : tx.query<std::string, std::string>(my_phone_book.check_duplicate_client(ua.input_data.at(0), ua.input_data.at(1)))) {
						poped_name_surname_email_has_mobile_phone_from_table[0] = poped_name;
						poped_name_surname_email_has_mobile_phone_from_table[1] = poped_surname;
					}
					if (poped_name_surname_email_has_mobile_phone_from_table[0] == "") {
						std::cout << "\nКлиента с таким именем и фамилией в базе не найдено\n\n";
						operation_index = -1;
						//break;
					}
					else {
						poped_name_surname_email_has_mobile_phone_from_table[2] = tx.query_value<std::string>(my_phone_book.get_client_email(ua.input_data.at(0), ua.input_data.at(1)));
						poped_name_surname_email_has_mobile_phone_from_table[3] = tx.query_value<std::string>(my_phone_book.get_is_client_has_mobile_phone(ua.input_data.at(0), ua.input_data.at(1)));
						std::cout << "\nДанные клиента:\n";
						//for (int i = 0; i < 4; i++) {
							std::cout << "Имя: " << poped_name_surname_email_has_mobile_phone_from_table[0] << "\n";
							std::cout << "Фамилия: " << poped_name_surname_email_has_mobile_phone_from_table[1] << "\n";
							std::cout << "email: " << poped_name_surname_email_has_mobile_phone_from_table[2] << "\n";
						//}
						if (poped_name_surname_email_has_mobile_phone_from_table[3] == "yes") {
							for (const auto& n : tx.query<std::string>(my_phone_book.get_mobile_phone(ua.input_data.at(0), ua.input_data.at(1), tx.query_value<int>(my_phone_book.get_client_id(poped_name_surname_email_has_mobile_phone_from_table[0], poped_name_surname_email_has_mobile_phone_from_table[1]))))) {
								poped_mobile_phones_from_table.push_back(std::get<0>(n));
							}
							for (int i = 0; i < poped_mobile_phones_from_table.size(); i++) {
								std::cout << std::to_string(i + 1) + "-й телефон: " << poped_mobile_phones_from_table[i] << "\n";
								ua.mobile_phone_count++;
							}
						}
						ua.change_client_data_flag = true;
					}
				
				
				
				
				break;
			case(4):
				for (auto& n : poped_name_surname_email_has_mobile_phone_from_table) n = "";
				poped_mobile_phones_from_table.erase(poped_mobile_phones_from_table.begin(), poped_mobile_phones_from_table.end());
				for (auto [poped_name, poped_surname] : tx.query<std::string, std::string>(my_phone_book.check_duplicate_client(ua.input_data.at(0), ua.input_data.at(1)))) {
					poped_name_surname_email_has_mobile_phone_from_table[0] = poped_name;
					poped_name_surname_email_has_mobile_phone_from_table[1] = poped_surname;
				}
				if (poped_name_surname_email_has_mobile_phone_from_table[0] == "") {
					std::cout << "\nКлиента с таким именем и фамилией в базе не найдено\n\n";
					operation_index = -1;
					//break;
				}
				else {
					poped_name_surname_email_has_mobile_phone_from_table[3] = tx.query_value<std::string>(my_phone_book.get_is_client_has_mobile_phone(ua.input_data.at(0), ua.input_data.at(1)));
					if (poped_name_surname_email_has_mobile_phone_from_table[3] == "no") {
						std::cout << "\nУ клиента нет ни одного номера телефона\n\n";
						operation_index = -1;
					}
					else {
						std::cout << "\nНомера телефонов клиента:\n";
						for (const auto& n : tx.query<std::string>(my_phone_book.get_mobile_phone(ua.input_data.at(0), ua.input_data.at(1), tx.query_value<int>(my_phone_book.get_client_id(poped_name_surname_email_has_mobile_phone_from_table[0], poped_name_surname_email_has_mobile_phone_from_table[1]))))) {
							poped_mobile_phones_from_table.push_back(std::get<0>(n));
						}
						for (int i = 0; i < poped_mobile_phones_from_table.size(); i++) {
							std::cout << std::to_string(i + 1) + "-й телефон: " << poped_mobile_phones_from_table[i] << "\n";
							ua.mobile_phone_count++;
						}
					}
					ua.delete_existing_client_mobile_phone = true;
				}
				break;
			case(5):
				for (auto& n : poped_name_surname_email_has_mobile_phone_from_table) n = "";
				for (auto [poped_name, poped_surname] : tx.query<std::string, std::string>(my_phone_book.check_duplicate_client(ua.input_data.at(0), ua.input_data.at(1)))) {
					poped_name_surname_email_has_mobile_phone_from_table[0] = poped_name;
					poped_name_surname_email_has_mobile_phone_from_table[1] = poped_surname;
				}
				if (poped_name_surname_email_has_mobile_phone_from_table[0] == "") {
					std::cout << "\nКлиента с таким именем и фамилией в базе не найдено\n\n";
					operation_index = -1;
					//break;
				}
				else {
					tx.exec(my_phone_book.delete_client(ua.input_data.at(0), ua.input_data.at(1), tx.query_value<int>(my_phone_book.get_client_id(ua.input_data.at(0), ua.input_data.at(1)))));
					std::cout << "Клиент удален из телефонной книги\n";
					operation_index = -1;
					
				}
				break;
			case(6):
				for (auto& n : poped_name_surname_email_has_mobile_phone_from_table) n = "";
				poped_mobile_phones_from_table.erase(poped_mobile_phones_from_table.begin(), poped_mobile_phones_from_table.end());
				poped_client_id_from_table.erase(poped_client_id_from_table.begin(), poped_client_id_from_table.end());
				if (ua.b1[3].second) {
					poped_mobile_phones_from_table.push_back(ua.input_data[0]);
					for (const auto& n : tx.query<int>(my_phone_book.find_client_id_by_mobile_phone(ua.input_data))) {
						poped_client_id_from_table.push_back(std::get<0>(n));
						}
					if (poped_client_id_from_table.size() == 0) {
						std::cout << "\nКлиента с телефоном " + ua.input_data[0] + " в телефонной книге не найдено\n";
						operation_index = -1;
						

					}
					else {
						std::cout << "\nВ телефонной книге записаны следующие клиенты с телефоном " + ua.input_data[0] + ":\n";
						for (const auto& m : poped_client_id_from_table) {
							for (auto [name, surname, email] : tx.query<std::string, std::string, std::string>(my_phone_book.find_client_data_by_client_id(m))) {
								std::cout << "Имя: " + name + "\n";
								std::cout << "Фамилия: " + surname + "\n";
								std::cout << "email: " + email + "\n";
								std::cout << "\n";
							}
						}
						operation_index = -1;
					}
						
				}
				else {
					
					auto result = tx.exec(my_phone_book.find_client_by_name_surname_email(ua.input_data, ua.b1));
					std::cout << "\nРезультаты поиска:\n";
					result.for_each([&tx, &my_phone_book](const std::string& name, const std::string& surname, const std::string& email) {
						std::cout << "Имя: " + name + "\n";
						std::cout << "Фамилия: " + surname + "\n";
						std::cout << "email: " + email + "\n";
						if (tx.query_value<std::string>(my_phone_book.get_is_client_has_mobile_phone(name, surname)) == "no") {
							std::cout << "Клиент не имеет ни одного мобильного телефона\n";
							std::cout << "\n";

						}
						else if (tx.query_value<std::string>(my_phone_book.get_is_client_has_mobile_phone(name, surname)) == "yes") {
							int k = 1;
							
							for (const auto& t : tx.query<std::string>(my_phone_book.get_mobile_phone(name, surname, tx.query_value<int>(my_phone_book.get_client_id(name, surname))))) {
								std::cout << std::to_string(k) + "-й телефон: " + std::get<0>(t) + "\n";
								k++;
							}
							std::cout << "\n";
						}
						});


					
					operation_index = -1;
				}
				
				break;
			case(7):
				std::cout << "\nРабота с телефонной книгой завершена\n";
				break;
			case(13):
				if (ua.answer > 3) {
					tx.exec(my_phone_book.update_client_data(poped_name_surname_email_has_mobile_phone_from_table[0], poped_name_surname_email_has_mobile_phone_from_table[1], 4, ua.input_data.at(3), tx.query_value<int>(my_phone_book.get_client_id(poped_name_surname_email_has_mobile_phone_from_table[0], poped_name_surname_email_has_mobile_phone_from_table[1])), poped_mobile_phones_from_table[ua.answer - 4]));
					std::cout << "Выбранный номер мобильного телефона изменен\n";
				}
				else {
					tx.exec(my_phone_book.update_client_data(poped_name_surname_email_has_mobile_phone_from_table[0], poped_name_surname_email_has_mobile_phone_from_table[1], ua.answer, ua.input_data.at(ua.answer - 1), tx.query_value<int>(my_phone_book.get_client_id(poped_name_surname_email_has_mobile_phone_from_table[0], poped_name_surname_email_has_mobile_phone_from_table[1]))));
					std::cout << "Данные изменены\n";
				}
				ua.change_client_data_flag = false;
				operation_index = -1;
				break;
			case(15):
				tx.exec(my_phone_book.delete_existing_mobile_phone(poped_name_surname_email_has_mobile_phone_from_table[0], poped_name_surname_email_has_mobile_phone_from_table[1], poped_mobile_phones_from_table.at(ua.answer - 1), tx.query_value<int>(my_phone_book.get_client_id(poped_name_surname_email_has_mobile_phone_from_table[0], poped_name_surname_email_has_mobile_phone_from_table[1]))));
				if (poped_mobile_phones_from_table.size() == 1) tx.exec(my_phone_book.put_is_client_has_mobile_phone(poped_name_surname_email_has_mobile_phone_from_table[0], poped_name_surname_email_has_mobile_phone_from_table[1], "no"));

				std::cout << "Выбранный номер мобильного телефона удален\n";
				ua.delete_existing_client_mobile_phone = false;
				operation_index = -1;
				break;
					
			}
		}
		
		

		
		

		/*-------------*/

	}
	
		catch (pqxx::sql_error& e) {

			std::cout << "При выполнеии запроса к БД произошло нечто ужасное. Вот код этого безобразия - " + e.sqlstate() << std::endl;
			return -1;
		}

		catch (std::invalid_argument& e) {
			std::cout << e.what() << std::endl;
			return -1;
		}
		catch (pqxx::broken_connection& e) {
			std::cout << "При попытке установить соединение с БД произошло нечто ужасное. Перезапуститесь и введите данные повторно. Код ошибки - ";
			std::cout << e.what() << std::endl;
			return -1;
		}
		catch (pqxx::unexpected_rows& e) {
			std::cout  << e.what() << std::endl;
			return -1;
		}


		
	
	return 0;
}