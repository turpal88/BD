#include"user_interconnection.hpp"
UserAction::UserAction() {
	answer = 0;
	step = 0;
	iteration_count = 0;
	
	
	
}

bool UserAction::check_value(std::string value_type, std::string value) {
	std::regex txt_regex;
	if (value_type == "name" || value_type == "surname") txt_regex = "[�-�]{1}[�-�]+[ -]?([�-�]{1}[�-�]+)?";
	else if (value_type == "email") txt_regex = "([a-z0-9_-]+\.)+[a-z0-9_-]+@[a-z0-9-]+(\.[a-z]{2,3}){1,2}";
	else if (value_type == "mobile_phone" || value_type == "one_more_mobile_phone") txt_regex = "\\+[1-9]{1,2}[0-9]{3,4}[0-9]{6,7}";
	else throw std::invalid_argument("������� �������� ������");
	return std::regex_match(value, txt_regex);


}

void UserAction::adding_new_client_data_validation(std::string value_type, std::string& value) {
	int answer = 0;
	int step = 0;
	
	
	if (value_type == "name" || value_type == "surname" || value_type == "email" || value_type == "mobile_phone" || value_type == "one_more_mobile_phone") {

		if (value_type == "name") step = 1;
		else if (value_type == "surname") step = 2;
		else if (value_type == "email") step = 3;
		else if (value_type == "mobile_phone") step = 4;
		else if (value_type == "one_more_mobile_phone") step = 10;
		while (step > 0) {
			switch (step) {
			case(1):
				std::cout << "������� ���: "; step = 5;
				break;
			case(2):
				std::cout << "������� �������: "; step = 5;
				break;
			case(3):
				std::cout << "������� email: "; step = 5;
				break;
			case(4):
				std::cout << "������� ����� ��������: "; step = 5;
				break;
			case(5):
				value.erase();
				
				
				std::getline(std::cin >> std::ws, value);
				
				step = 6;
				break;
			case(6):
				if (!this->check_value(value_type, value)) {
					if (value_type == "name") std::cout << "��� ���������. ";
					else if (value_type == "surname") std::cout << "������� ���������. ";
					else if (value_type == "email") std::cout << "email ���������. ";
					else if (value_type == "mobile_phone" || value_type == "one_more_mobile_phone") std::cout << "����� �������� ���������. ";
					std::cout << "��� ������?\n1.������ ������\n2.� ���� ��������\n";
					step = 7;
					
				}
				else { 
					if (value_type != "one_more_mobile_phone") { this->input_data.push_back(value); step = 0; }
					else step = 10;
				}
				break;


			case(7):
				std::cin >> answer;
				if (answer != 1 && answer != 2) step = 8;
				else step = 9;
				break;
			case(8):
				std::cout << "������� ���������� �����" << std::endl;
				step = 7;
				break;
			case(9):
				if (answer == 1) {
					if (value_type == "name") step = 1;
					else if (value_type == "surname") step = 2;
					else if (value_type == "email") step = 3;
					else if (value_type == "mobile_phone" || value_type == "one_more_mobile_phone") step = 4;

				}
				else if (answer == 2) {
					step = 0;
					this->step = 0;

				}
				break;
			case(10):
				std::cout << "����� ������� ��� ���� ����� ��������?(1 - yes/2 - no)";
				step = 7;
				break;
			
			
			}
		}
	}
	else if (value_type == "has_mobile_phone") {
		std::cout << "������ ����� ��������� �������?(1 - yes/2 - no) ";
		while (true) {
			std::cin >> answer;
			if (answer != 1 && answer != 2) {
				std::cout << "������� ���������� �����" << std::endl;
				continue;
			}
			else {
				if (answer == 1) value = "yes";
				else if (answer == 2) value = "no";
				input_data.push_back(value);
				//this->is_input_validation_data_comleted = true;
				break;


			}
		}

	}
	else throw std::invalid_argument("����� ����������� ������ � ����� �������� ��� ������\n");

}

void UserAction::user_interconnection(int& operation_index) {
	this->step = 0;
	std::string value;
	
	while (this->step >= 0) {
		
		switch(this->step) {
		case(0):
			this->input_data.clear();
			std::cout << std::endl;
			std::cout << "�������� ���������� �������� � ���������� ������:\n1.�������� �������\n2.�������� ������� ��� ������������� �������\n";
			std::cout << "3.�������� ������ � �������\n4.������� ������� � ������������� �������\n5.������� ������������� �������\n";
			std::cout << "6.����� ������� �� ��� ������ - �����, �������, email ��� ��������\n";
			std::cout << "7.��������� ������\n";
			this->step = 8;
			break;
		case(1):
			for (int i = 0; i < 4; i++) {
				if (this->step > 0) {
					if (i == 0) {
						this->adding_new_client_data_validation("name", value); continue;
					}
					if (i == 1) {
						this->adding_new_client_data_validation("surname", value); continue;
					}
					if (i == 2) {
						this->adding_new_client_data_validation("email", value);
						continue;
					}
					if (i == 3) {
						this->adding_new_client_data_validation("has_mobile_phone", value);
						if (value == "yes") {
							this->adding_new_client_data_validation("mobile_phone", value);
							this->adding_new_client_data_validation("one_more_mobile_phone", value);

						}
						this->step = -1;
					}
				}else break;
				
				 
				
					
				
			}
			break;
		case(7):
			this->step = -1;
			break;
		case(8):
			std::cin >> this->answer;
			operation_index = this->answer;
			this->step = 9;
			break;
		case(9):
			for (int i = 1; i < 8; i++) {
				if (this->answer == i) {
					this->step = i;
					break;
				}
				if (i == 7 && this->answer != i) {
					std::cout << "������� ���������� �����\n"; 
					break;
				}
			}
			
		}
		
	}
}