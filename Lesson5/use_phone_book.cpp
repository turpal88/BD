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

		/*Работа с данными в таблицах БД */
		
		UserAction ua;
		int operation_index = -1;
		while (operation_index != 7) {
			ua.user_interconnection(operation_index);
			switch (operation_index) {
			case(1):
				tx.exec(my_phone_book.add_new_client(ua.input_data));
				if (ua.input_data.size() > 4) {
					int client_id = tx.query_value<int>("select id from " + my_phone_book.get_table_name() + "where name = '" + ua.input_data.at(0) + "' and surname = '" + ua.input_data.at(1) + "'");
					for (int i = 4; i < ua.input_data.size(); i++) {
						tx.exec(my_phone_book.insert_value_into_aux_table(client_id, ua.input_data.at(i)));
					}
				}
				std::cout << std::endl;
				std::cout << "Клиент успешно добавлен в телефонную книгу" << std::endl;
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
			std::cout << "При попытке установить соединение с БД произошло нечто ужасное. Перезапуститесь и введите данные повторно" << std::endl;
			return -1;
		}
		catch (pqxx::unexpected_rows& e) {
			std::cout << "Ошибка удаления существующей таблицы" << std::endl;
			return -1;
		}


		
	
	return 0;
}