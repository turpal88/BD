#pragma once
#include "phone_book.hpp"

PhoneBook::PhoneBook() {
	
	for (int i = 65; i < 91; i++) { //заполняем вектор допустимыми значениями символов из ASCII-таблицы чтобы проверять что названия столбцов таблицы юзер вводит не абы какие
		allowed_ascii_values.push_back(i);
		allowed_ascii_values_for_table_values.push_back(i);
	}
	allowed_ascii_values.push_back(95);
	for (int i = 97; i < 123; i++) {
		allowed_ascii_values.push_back(i);
		allowed_ascii_values_for_table_values.push_back(i);
	}
	//allowed_ascii_values_for_table_values.push_back(32);
	allowed_ascii_values_for_table_values.push_back(95);
	/*
	for (int i = 128; i < 176; i++) {
		
		allowed_ascii_values_for_table_values.push_back(i);
	}
	*/
	/*
	for (int i = 224; i < 242; i++) {
		
		allowed_ascii_values_for_table_values.push_back(i);
	}
	*/
	//is_needed_to_define_own_column_names = true;
	is_main_table_created = false;
	is_aux_table_created = false;
	
}

void PhoneBook::set_db_connection_parameters() {
	std::vector<char> _password;
	char c = ' ';
	std::cout << "Введите параметры для входа в базу данных: " << std::endl;
	std::cout << "host: "; std::cin >> host_name;
	std::cout << "port: "; std::cin >> port;
	std::cout << "dbname: "; std::cin >> dbname;
	std::cout << "user: "; std::cin >> user;
	std::cout << "password: "; 
	while ((c = getch()) != '\r') {
		_password.push_back(c);
		putch('*');
	}
	std::for_each(_password.begin(), _password.end(), [&](const char k) {
		this->password += k;
		});
	
	
}
std::string PhoneBook::_host_name() {
	return this->host_name;
}
std::string PhoneBook::_port() {
	return this->port;
}
std::string PhoneBook::_dbname() {
	return this->dbname;
}
std::string PhoneBook::_user() {
	return this->user;
}
std::string PhoneBook::_password() {
	return this->password;
}

void PhoneBook::push_table_name(std::string table_name) {
	this->table_name = table_name;
}

std::string PhoneBook::get_table_name() {
	return this->table_name;
}

bool PhoneBook::is_name_valid(std::string table_name) {
	
	for (auto it = table_name.begin(); it != table_name.end(); ++it) {
		if (std::find(this->allowed_ascii_values.begin(), this->allowed_ascii_values.end(), static_cast<int>(*it)) != allowed_ascii_values.end()) continue;
		else return false;
	}
	return true;
}

//метод создания новой таблицы телефонной книги
std::string PhoneBook::create_new_main_table() {
		
	return	
		"create table " + this->get_table_name() + "(id serial primary key, "
		+ "name varchar(50) not null, "
		+ "surname varchar(50) not null, "
		+ "email varchar(50) unique, "
		+ "has_mobile_phone varchar(50));";
		 
		
		
	 
}

//метод создания новой вспомогательной таблицы
std::string PhoneBook::create_new_aux_table() {
	return "CREATE TABLE " + this->get_table_name() + "_aux" + "(id serial primary key, "
		+ "phone_book_id integer references " + this->get_table_name() + "_aux" + "(id), "
		+ "mobile_phone varchar(50));";
}


//метод удаления основной таблицы если она некорректна
std::string PhoneBook::delete_main_table() {
	return "drop table if exists " + this->get_table_name() + " cascade;";
}

//метод удаления вспомогательной таблицы если она некорректн
std::string PhoneBook::delete_aux_table() {
	return "drop table if exists " + this->get_table_name() + "_aux cascade;";
}

// метод для задавания вопросов пользователю и получения ответов
int PhoneBook::ask_ans_cycle_request(int answer2) {
	
		//std::cin >> answer2;
		if (answer2 == 1 || answer2 == 2) {
			if (answer2 == 1) return 1;
			//this->create_new_main_and_aux_table();
			//switch_way=1 - таблицы созданы с помощью метода, далее работа с данными таблицы(добавление, удаление, модификация)
			//this->switch_way = 1;
			//return 1;

		//в случае если таблица с нужной структурой в базе отсутствует и пользователь не хочет ее создавать, заканчиваем взаимодействие
			else {
				std::cout << "Всех благ" << std::endl;
				return 2;
			}
		}
		else return -1;
	
} 

/*
std::string PhoneBook::get_last_row_number() {
	return "select count(*) from " + this->get_table_name() + ";";
}
*/

std::string PhoneBook::is_main_table_valid() {
	return "select column_name, is_nullable, udt_name from information_schema.columns where table_name='" + this->get_table_name() + "'";
}

std::string PhoneBook::is_aux_table_valid() {
	return "select column_name, is_nullable, udt_name from information_schema.columns where table_name='" + this->get_table_name() + "_aux'";
}


/*
std::string PhoneBook::add_new_client() {
	std::string name;
	std::cout << "name: "; 
	
}
*/

/*
void PhoneBook::set_db_fields() {
	
	char answer = ' ';
	std::cout << std::endl;
	std::cout << "В таблицу БД будем заносить столбцы \"id\" \"Имя\" \"Фамилия\" \"email\" \"Телефон\"? Или запишем свои?\n(Последний столбец - может быть больше одной записи или не быть вовсе на один id)\n('n' - не пишем свои 'y' - пишем свои)";
	
		while (true) {
			std::cin >> answer;
			if (answer == 'n') {
				db_column_names.push_back("id");
				db_column_names.push_back("Name");
				db_column_names.push_back("Surname");
				db_column_names.push_back("email");
				db_column_names.push_back("Mobile_phone");
				return;
			}
			if (answer == 'y') break;
			else {
				std::cout << "Введите нормальный ответ: ";
				continue;
			}


		}
	
	
		std::cout << std::endl;
	std::cout << "Пишу названия столбцов таблицы БД: ";
	//std::cin >> db_column_count;
	//std::cout << "Заносим в базу названия столбцов:" << std::endl;
	
	int count = 1;
	char c = ' ';
	bool failure; //флаг введенного хотя бы 1 символа в названии столбца вне значений вектора allowed_ascii_values, приведенных к char
	bool enough = false; //хватит столбцов флаг
	std::string column_name;
	//std::vector<std::string> db_column_names;
	while (true) {
		column_name = ""; //обнуляем буферную переменную названия столбца БД
		std::cout << count << ".";
		std::cin >> column_name;
		
		
		failure = false; //перед проверкой символов названия столбца сбрасываем флаг неправильно введенного в прошлую итерацию символа
		std::for_each(column_name.begin(), column_name.end(), [&](const char& x) {
			if (!failure) {
			auto it = std::find(allowed_ascii_values.begin(), allowed_ascii_values.end(), static_cast<int>(x));
			
				if (it == allowed_ascii_values.end()) {
					//std::string message = "Введено неадекватное название столбца. Так не пойдет. \nВводите заново или до свидания (o - еще раз, b - до свидания)";
					std::cout << "Введено неадекватное название столбца. Так не пойдет. \nВводите заново или до свидания (o - еще раз, b - до свидания)";
					while (true) {
						std::cin >> answer;
						if (answer == 'o' || answer == 'b') {
							if (answer == 'o') {
								failure = true; //ставим флаг что есть некорректный символ - чтобы цикл std::for_each дальше остальные символы названия столбца не анализировал
								break;
							} 
							else if (answer == 'b') throw std::invalid_argument("Всех благ");
							else {
								std::cout << "Введите нормальный ответ: ";
								continue;
							}
						}
					}

				}
				
				
			}
			
			});
		if (!failure) {
		
			db_column_names.push_back(column_name);
			count++;
			
			std::cout << "Название столбца принято. Столбцы еще будут? ('e' - харэ, 'm' - еще запишем)" << std::endl;
			while (true) {
				std::cin >> answer;
				if (answer == 'e' || answer == 'm') {
					if (answer == 'e') {
						enough = true;
						
						break;
					}
					else if (answer == 'm') break;
					
				}
				else {
					std::cout << "Введите нормальный ответ: ";
					continue;
				}
			}
		
		
		}
		if (enough) break;

	}
}
*/





