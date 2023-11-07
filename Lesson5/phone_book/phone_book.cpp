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
	table_name = "";

	
}

void PhoneBook::set_db_connection_parameters() {
	std::vector<char> _password;
	char c = ' ';
	std::cout << "Введите параметры для входа в БД: " << std::endl;
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
		+ "email varchar(50) unique not null, "
		+ "has_mobile_phone varchar(50) not null);";
		 
		
		
	 
}

//метод создания новой вспомогательной таблицы
std::string PhoneBook::create_new_aux_table() {
	return "CREATE TABLE " + this->get_table_name() + "_aux" + "(id serial primary key, "
		+ "phone_book_id integer references " + this->get_table_name() + "(id), "
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
	
		
		if (answer2 == 1 || answer2 == 2) {
			if (answer2 == 1) return 1;
			

		//в случае если таблица с нужной структурой в базе отсутствует и пользователь не хочет ее создавать, заканчиваем взаимодействие
			else {
				
				return 2;
			}
		}
		else return -1;
	
} 



std::string PhoneBook::is_main_table_valid() {
	return "select column_name, is_nullable, udt_name from information_schema.columns where table_name='" + this->get_table_name() + "'";
}

std::string PhoneBook::is_aux_table_valid() {
	return "select column_name, is_nullable, udt_name from information_schema.columns where table_name='" + this->get_table_name() + "_aux'";
}




std::string PhoneBook::check_duplicate_client(std::string name, std::string surname) {
	return
		"select name, surname from " + this->get_table_name()
		+ " where name='" + name + "' and surname='" + surname + "'";
}


std::string PhoneBook::insert_value_into_main_table(std::string column, std::string value) {
	return
		"insert into " + this->get_table_name() + "(" + column + ") values('" + value + "');";
}
std::string PhoneBook::insert_value_into_aux_table(int id, std::string value) {
	return
		"insert into " + this->get_table_name() + "_aux" + "(phone_book_id, mobile_phone) values('" + std::to_string(id) + "', '" + value + "');";
}

std::string PhoneBook::add_new_client(std::vector<std::string> input_data) {
	return
		"insert into " + this->get_table_name() + "(name, surname, email, has_mobile_phone) values('" + input_data.at(0) + "','" + input_data.at(1) + "','" + input_data.at(2) + "','" + input_data.at(3) + "');";
}







