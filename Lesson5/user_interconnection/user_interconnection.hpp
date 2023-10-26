#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <conio.h>
#include <algorithm>
class UserAction {
public:
	int answer;
	int step;
	int iteration_count;
	//�������� ������ ���� ����� ������������ �������� ������������� ��������
	std::vector<std::string> input_data;
	
	bool check_value(std::string value_type, std::string value);
	UserAction(const UserAction& ua) = delete; //������ ������������ �����������
	UserAction& operator=(const UserAction& ua) = delete; //������ ��������� ������������
	UserAction();
	void adding_new_client_data_validation(std::string value_type, std::string& value);
	void user_interconnection(int& operation_index);

};
