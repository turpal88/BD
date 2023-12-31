﻿#include <string>
#include <vector>
#include <iostream>

#include  <conio.h>
#include <algorithm>
#include <iostream>

class PhoneBook {
	public:
		PhoneBook();
		PhoneBook(const PhoneBook& pb) = delete; //запрет конструктора копирования
		PhoneBook& operator=(const PhoneBook& pb) = delete; //запрет оператора присваивания
		void set_db_connection_parameters(); //метод запроса параметров для соединения с базой данных
		std::string _host_name();
		std::string _port();
		std::string _dbname();
		std::string _user();
		std::string _password();
		//void set_db_fields();
		std::vector<std::string> db_column_names;
		bool is_needed_to_define_own_column_names; //флаг что будес названия столбцов писать свои. Изначально ставим в false - Будут стандартные поля как по заданию

		bool is_name_valid(std::string table_name);
		void push_table_name(std::string table_name);
		std::string get_table_name();
		std::string create_new_main_table(); //метод создания новой таблицы телефонной книги
		std::string create_new_aux_table(); //метод создания новой вспомогательной таблицы
		std::string delete_main_table(); //метод удаления основной таблицы если она некорректна
		std::string delete_aux_table(); //метод удаления вспомогательной таблицы если она некорректна
		std::string is_main_table_valid();
		std::string is_aux_table_valid();
		int ask_ans_cycle_request(int answer2); // метод для задавания вопросов пользователю и получения ответов
		//std::string get_last_row_number();
		//std::string add_new_client();
		bool is_main_table_created; //флаг создания корректной основной таблицы в БД
		bool is_aux_table_created; //флаг создания корректной вспомогательной таблицы в БД
		int main_table_column_count;
		int aux_table_column_count;
		//bool check_value(std::string value_type, std::string value);
		std::string check_duplicate_client(std::string name, std::string surname);
		//std::string check_client_name(std::string name);
		//std::string check_client_surname(std::string surname);
		//std::string check_client_email(std::string email);
		//std::string check_client_mobile_phone(std::string mobile_phone);
		std::string insert_value_into_main_table(std::string column, std::string value);
		std::string insert_value_into_aux_table(int id, std::string value);
		std::string get_client_id(std::string name, std::string surname);
		std::string get_is_client_has_mobile_phone(std::string name, std::string surname);
		std::string get_is_client_has_mobile_phone(int client_id);
		std::string put_is_client_has_mobile_phone(std::string name, std::string surname, std::string has_mobile_phone_flag);
		//void adding_new_client_data_validation(std::string value_type, std::string& value);
		std::string add_new_client(std::vector<std::string> input_data);
		std::string get_client_email(std::string name, std::string surname);
		//std::string get_mobile_phone_count(std::string name, std::string surname);
		std::string get_mobile_phone(std::string name, std::string surname, int client_id);
		std::string update_client_data(std::string name, std::string surname, int updated_value_type, std::string updated_value, int client_id, std::string old_mobile_phone="");
		std::string delete_client(std::string name, std::string surname, int client_id);
		/*
		std::string get_client_data(std::string name = "", std::string surname = "", std::string email = "");
		std::string get_client_data(int client_id);
		*/

		std::string delete_existing_mobile_phone(std::string name, std::string surname, std::string deleted_mobile_phone, int client_id);
		std::string find_client_by_name_surname_email(std::vector<std::string>& input_data, std::vector<std::pair<std::string, bool>>& b1);
		std::string find_client_id_by_mobile_phone(std::vector<std::string>& input_data);
		std::string find_client_data_by_client_id(int client_id);

    private:
		std::string host_name;
		std::string port;
		std::string dbname;
		std::string user;
		std::string password;
		
		
		std::vector<int> allowed_ascii_values;
		std::vector<int> allowed_ascii_values_for_table_values;
		std::string table_name;

		
		//std::vector<int> db_column_count;
		
		//void push_columns_in_db();
		

};