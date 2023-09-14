#include <string>
#include <vector>
#include <iostream>

#include  <conio.h>
#include <algorithm>
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