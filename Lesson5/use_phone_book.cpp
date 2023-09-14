#pragma once
#include "windows.h"
#include "phone_book.hpp"
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <pqxx/pqxx>
#include "pqxx/nontransaction"


int main() {
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
			system("CLS");
			std::cout << "Введите название телефонной книги (название таблицы),\nкоторую будем создавать или которая уже должна существовать в БД: ";
			std::cin >> answer1;
			if (!my_phone_book.is_name_valid(answer1)) {
				std::cout << "Введенное имя таблицы содержит недопустимые символы.\n1.Покинуть чат\n2.Ввести название заново\n";
				while (true) {
					std::cin >> answer2;
					if (answer2 == 1 || answer2 == 2) {
						if (answer2 == 1) {
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
			std::cout << "Основная таблица создана" << std::endl;
			std::cout << "Вспомогательная таблица создана" << std::endl;
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

						if (std::get<0>(already_existing_columns_in_table_in_db.at(0)) != "id"
							|| std::get<1>(already_existing_columns_in_table_in_db.at(0)) != "NO"
							|| std::get<2>(already_existing_columns_in_table_in_db.at(0)) != "int4") {

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
									system("CLS");
									std::cout << "Основная таблица создана" << std::endl;
									break;
								}
								else if (my_phone_book.ask_ans_cycle_request(answer2) == 2) return -1;
							}
						}
						if (std::get<0>(already_existing_columns_in_table_in_db.at(1)) != "name"
							|| std::get<1>(already_existing_columns_in_table_in_db.at(1)) != "NO"
							|| std::get<2>(already_existing_columns_in_table_in_db.at(1)) != "varchar") {

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
									system("CLS");
									std::cout << "Основная таблица создана" << std::endl;
									break;
								}
								else if (my_phone_book.ask_ans_cycle_request(answer2) == 2) return -1;
							}
						}

						if (std::get<0>(already_existing_columns_in_table_in_db.at(2)) != "surname"
							|| std::get<1>(already_existing_columns_in_table_in_db.at(2)) != "NO"
							|| std::get<2>(already_existing_columns_in_table_in_db.at(2)) != "varchar") {
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
									system("CLS");
									std::cout << "Основная таблица создана" << std::endl;
									break;
								}
								else if (my_phone_book.ask_ans_cycle_request(answer2) == 2) return -1;
							}

						}

						if (std::get<0>(already_existing_columns_in_table_in_db.at(3)) != "email"
							|| std::get<1>(already_existing_columns_in_table_in_db.at(3)) != "YES"
							|| std::get<2>(already_existing_columns_in_table_in_db.at(3)) != "varchar") {
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
									system("CLS");
									std::cout << "Основная таблица создана" << std::endl;
									break;
								}
								else if (my_phone_book.ask_ans_cycle_request(answer2) == 2) return -1;
							}

						}

						if (std::get<0>(already_existing_columns_in_table_in_db.at(4)) != "has_mobile_phone"
							|| std::get<1>(already_existing_columns_in_table_in_db.at(4)) != "YES"
							|| std::get<2>(already_existing_columns_in_table_in_db.at(4)) != "varchar") {

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
									system("CLS");
									std::cout << "Основная таблица создана" << std::endl;
									break;
								}
								else if (my_phone_book.ask_ans_cycle_request(answer2) == 2) return -1;
							}
						}








						//есил прошли все вышеперечисленные условия, взводим флаг что таблица создана (есть в БД и все стобцы валидны)
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
								system("CLS");
								std::cout << "Основная таблица создана" << std::endl;
								break;
							}
							else if (my_phone_book.ask_ans_cycle_request(answer2) == 2) return -1;
						}
					}
				}
				//если найдено название вспомогательной таблицы
				else if (std::get<0>(*it) == answer1 + "_aux") {
					//запушим в класс имя таблицы чтобы потом легко использовать его
					my_phone_book.push_table_name(answer1);
					my_phone_book.aux_table_column_count = tx.query_value<int>("SELECT COUNT(*) FROM INFORMATION_SCHEMA.COLUMNS WHERE table_name = '" + std::get<0>(*it) + "_aux'");
					if (my_phone_book.aux_table_column_count == 3) {
						already_existing_columns_in_table_in_db.clear();
						for (auto [column_name, is_nullable, udt_name] : tx.query<std::string, std::string, std::string>(my_phone_book.is_aux_table_valid())) {
							already_existing_columns_in_table_in_db.push_back(std::make_tuple(column_name, is_nullable, udt_name));
						}
						if (std::get<0>(already_existing_columns_in_table_in_db.at(0)) != "id"
							|| std::get<1>(already_existing_columns_in_table_in_db.at(0)) != "NO"
							|| std::get<2>(already_existing_columns_in_table_in_db.at(0)) != "int4") {

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
									system("CLS");
									std::cout << "Вспомогательная таблица создана" << std::endl;
									break;
								}
								else if (my_phone_book.ask_ans_cycle_request(answer2) == 2) return -1;
							}
						}

						if (std::get<0>(already_existing_columns_in_table_in_db.at(1)) != "phone_book_id"
							|| std::get<1>(already_existing_columns_in_table_in_db.at(1)) != "YES"
							|| std::get<2>(already_existing_columns_in_table_in_db.at(1)) != "int4") {

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
									system("CLS");
									std::cout << "Вспомогательная таблица создана" << std::endl;
									break;
								}
								else if (my_phone_book.ask_ans_cycle_request(answer2) == 2) return -1;
							}
						}

						if (std::get<0>(already_existing_columns_in_table_in_db.at(2)) != "mobile_phone"
							|| std::get<1>(already_existing_columns_in_table_in_db.at(2)) != "YES"
							|| std::get<2>(already_existing_columns_in_table_in_db.at(2)) != "varchar") {

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
									system("CLS");
									std::cout << "Вспомогательная таблица создана" << std::endl;
									break;
								}
								else if (my_phone_book.ask_ans_cycle_request(answer2) == 2) return -1;
							}
						}
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
								system("CLS");
								std::cout << "Вспомогательная таблица создана" << std::endl;
								break;
							}
							else if (my_phone_book.ask_ans_cycle_request(answer2) == 2) return -1;
						}
					}
				}
				//если перебрали все имена таблиц в базе и phone_book нет, сообщаем об этом
				if (it == already_existing_tables_in_db.end() - 1) {
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
								system("CLS");
								std::cout << "Основная таблица создана" << std::endl;
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
								system("CLS");
								std::cout << "Вспомогательная таблица создана" << std::endl;
								//my_phone_book.is_aux_table_created = true; //switch_way = 1; //- вспомогательная таблица создана
								break;
							}
							else if (my_phone_book.ask_ans_cycle_request(answer2) == 2) return -1;
						}
					}
				}
			} /*конец цикла*/
		}

		
			








						//for (std::tuple<std::string> column_name : tx.query<std::string>(
						//	"SELECT column_name from information_schema.columns where table_name='" + std::get<0>(*it) + "'"
						//	))
						//{
						//	temp_vector_with_current_column_names.push_back(column_name);
							//std::cout << ordinal_position << "." << column_name << " (" << data_type << ")" << std::endl;
					//	}
						//std::cout << std::endl;
						//std::cout << "Данная таблица содержит " << tx.query_value<int>("SELECT COUNT(*) FROM " + std::get<0>(*it)) << " записей" << std::endl;



				//если таблицы с таким названием нет, сообщаем об этом






			

			/*
			//проверка что одна из таблиц - phone_book а вторая - связанная с ней
			for (auto it = already_existing_tables_in_db.begin(); it != already_existing_tables_in_db.end(); ++it) {
				std::cout << std::endl;
				std::cout << std::get<0>(*it) << std::endl; //вывод имени таблицы на печать

				int col_count = tx.query_value<int>("SELECT COUNT(*) FROM INFORMATION_SCHEMA.COLUMNS WHERE table_name = '" + std::get<0>(*it) + "'");
				if (col_count > 0) {
					/*
					for (auto [ordinal_position, column_name, data_type] : tx.query<int, std::string, std::string>(
						"SELECT ordinal_position, column_name, data_type from information_schema.columns where table_name='" + std::get<0>(*it) + "'"
						))
					{
						//temp_vector_with_current_column_names.push_back(column_name);
						std::cout << ordinal_position << "." << column_name << " (" << data_type << ")" << std::endl;
					}
					std::cout << std::endl;
					*/
					//std::string s = "SELECT COUNT(*) FROM INFORMATION_SCHEMA.COLUMNS WHERE table_name = '" + std::get<0>(*it) + "'";

			//	}

		//	}
			/*
			if (already_existing_tables_in_db.size() > 0) {

				std::cout << "БД уже содержит таблицы. Вот их список:" << std::endl;


			}
			//std::string s = "SELECT COUNT(*) FROM INFORMATION_SCHEMA.COLUMNS WHERE table_name = 'phone_book'";
			//char answer1;
			//int current_column_count = tx.query_value<int>(s);
			if (current_column_count > 0) {
				//std::vector<std::string> temp_vector_with_current_column_names;
				std::cout << "БД уже содержит столбцы. Вот их список:" << std::endl;
				for (auto [ordinal_position, column_name, data_type] : tx.query<int, std::string, std::string>(
					"SELECT ordinal_position, column_name, data_type from information_schema.columns where table_name='phone_book'"
				))
				{
					//temp_vector_with_current_column_names.push_back(column_name);
					std::cout << ordinal_position << "." << column_name << " (" << data_type << ")" << std::endl;
				}

				std::cout << std::endl;
				std::cout << "Будем удалять все столбцы и создавать заново или работать с тем что есть? \n('y' - удаляем столбцы и пишем заново, 'n' - оставляем как есть)";
				while (true) {
					std::cin >> answer1;
					if (answer1 == 'y') {
						//std::string drop_from_db_column;

						//	drop_from_db_column += "drop table if exists " + s + "cascade; ";

						tx.exec("drop table if exists phone_book cascade;");

						//tx.commit();
						if (tx.query_value<int>(s) == 0) {
							std::cout << std::endl;
							std::cout << "Столбцы грохнули, можем заполняться" << std::endl;
							break;
						}
						else throw std::invalid_argument("При удалении столбцов в БД произошло что-то ужасное. Останавливаю всю работу");

					}
					if (answer1 == 'n') {
						my_phone_book.is_needed_to_define_own_column_names = false;
						break;
					}
					else {
						std::cout << "Введите нормальный ответ: ";
						continue;
					}
				}
			}
			if (my_phone_book.is_needed_to_define_own_column_names) {
				my_phone_book.set_db_fields();

				//int mt = tx.query_value<int>(s);
				//std::cout << mt << std::endl;
				std::string create_db_query = "CREATE TABLE IF NOT EXISTS phone_book(";
				for (int i = 0; i < my_phone_book.db_column_names.size(); i++) {
					if (i == 0) create_db_query += "id INTEGER PRIMARY KEY NOT NULL, ";
					else if (i < my_phone_book.db_column_names.size() - 1) create_db_query += my_phone_book.db_column_names.at(i) + " VARCHAR(50),";
					else if (i == my_phone_book.db_column_names.size() - 1) create_db_query += my_phone_book.db_column_names.at(i) + " VARCHAR(50));";
				}
				tx.exec(create_db_query);
				std::string create_reference_table = "CREATE TABLE IF NOT EXISTS " + my_phone_book.db_column_names.at(0) + "_" + my_phone_book.db_column_names.at(my_phone_book.db_column_names.size() - 1) + "("
					+ my_phone_book.db_column_names.at(my_phone_book.db_column_names.size() - 1) + "_" + my_phone_book.db_column_names.at(0) + " INTEGER REFERENCES phone_book(id), "
					+ my_phone_book.db_column_names.at(my_phone_book.db_column_names.size() - 1) + " VARCHAR(50));";
				tx.exec(create_reference_table);
				//tx.commit();
				//create_db_query.clear();
			}




	*/
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
			std::cout << "При попытке установить соединение с БД произошло нечто ужасное. Перезапуститесь и введите данные повторно" << std::endl;
			return -1;
		}
		catch (pqxx::unexpected_rows& e) {
			std::cout << "Ошибка удаления сущетсвующей таблицы" << std::endl;
			return -1;
		}


		
	
	return 0;
}