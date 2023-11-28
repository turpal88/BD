#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <conio.h>
#include <algorithm>
#include <iterator>

class UserAction {
public:
	int answer;
	int step;
	//int iteration_count;
	//буферный вектор куда будут складываться вводимые пользователем значения
	std::vector<std::string> input_data;
	std::string answer_for_search_client;
	//std::pair для возможности поиска клиента по одному или нескольким параметрам
	std::vector<std::pair<std::string, bool>> b1;
	bool change_client_data_flag;
	bool delete_existing_client_mobile_phone;
	int mobile_phone_count;
	bool check_value(std::string value_type, std::string value);
	UserAction(const UserAction& ua) = delete; //запрет конструктора копирования
	UserAction& operator=(const UserAction& ua) = delete; //запрет оператора присваивания
	UserAction();
	void adding_new_client_data_validation(std::string value_type, std::string& value);
	//void search_client(std::string value_type, std::string& value);
	void user_interconnection(int& operation_index);
	

};
