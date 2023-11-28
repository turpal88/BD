#include"user_interconnection.hpp"
UserAction::UserAction() {
	answer = 0;
	step = 0;
	//iteration_count = 0;
	change_client_data_flag = false;
	delete_existing_client_mobile_phone = false;
	mobile_phone_count = 0;
	this->b1.push_back(std::make_pair("name", false));
	this->b1.push_back(std::make_pair("surname", false));
	this->b1.push_back(std::make_pair("email", false));
	this->b1.push_back(std::make_pair("mobile_phone", false));
	
	
}

bool UserAction::check_value(std::string value_type, std::string value) {
	std::regex txt_regex;
	if (value_type == "name" || value_type == "surname") txt_regex = "[А-ЯA-Z]{1}[a-zа-я]+[ -]?([А-ЯA-Z]{1}[a-zа-я]+)?";
	else if (value_type == "email") txt_regex = "([a-z0-9_-]+\.)+[a-z0-9_-]+@[a-z0-9-]+(\.[a-z]{2,3}){1,2}";
	else if (value_type == "mobile_phone" || value_type == "one_more_mobile_phone") txt_regex = "\\+[1-9]{1,2}[0-9]{3,4}[0-9]{6,7}";
	//else if (value_type == "answer_for_search") txt_regex = "(1?2?3?4?)?";
	else throw std::invalid_argument("Неверно передана строка");
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
				std::cout << "Введите имя: "; step = 5;
				break;
			case(2):
				std::cout << "Введите фамилию: "; step = 5;
				break;
			case(3):
				std::cout << "Введите email: "; step = 5;
				break;
			case(4):
				std::cout << "Введите номер телефона: "; step = 5;
				break;
			case(5):
				value.erase();
				
				
				std::getline(std::cin >> std::ws, value);
				
				step = 6;
				break;
			case(6):
				if (!this->check_value(value_type, value)) {
					if (value_type == "name") std::cout << "Имя невалидно. ";
					else if (value_type == "surname") std::cout << "Фамилия невалидна. ";
					else if (value_type == "email") std::cout << "email невалиден. ";
					else if (value_type == "mobile_phone" || value_type == "one_more_mobile_phone") std::cout << "Номер телефона невалиден. ";
					std::cout << "Что дальше?\n1.Ввести заново\n2.В меню действий\n";
					step = 7;
					
				}
				else { 
					if (!this->change_client_data_flag/* && !this->delete_existing_client_mobile_phone*/) this->input_data.push_back(value);
					else if(this->change_client_data_flag) {
						if(this->input_data.size() < 4) this->input_data.resize(4);
						if (value_type == "name") this->input_data.at(0) = value;
						else if(value_type == "surname") this->input_data.at(1) = value;
						else if (value_type == "email") this->input_data.at(2) = value;
						else if (value_type == "mobile_phone") this->input_data.at(3) = value;
					}
					/*
					else if (this->delete_existing_client_mobile_phone) {
						if (this->input_data.size() < 3) this->input_data.resize(3);
						if (value_type == "name") this->input_data.at(0) = value;
						else if (value_type == "surname") this->input_data.at(1) = value;
						else if (value_type == "mobile_phone") this->input_data.at(2) = value;
					}
					*/
					if (value_type != "one_more_mobile_phone") step = 0;
					else step = 10;
				}
				break;


			case(7):
				std::cin >> answer;
				if (answer != 1 && answer != 2) step = 8;
				else step = 9;
				break;
			case(8):
				std::cout << "Введите нормальный ответ" << std::endl;
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
					if(value_type != "one_more_mobile_phone") this->step = 0;

				}
				break;
			case(10):
				std::cout << "Будем вводить еще один номер телефона?(1 - yes/2 - no)";
				step = 7;
				break;
			
			
			}
		}
	}
	else if (value_type == "has_mobile_phone") {
		std::cout << "Клиент имеет мобильный телефон?(1 - yes/2 - no) ";
		while (true) {
			std::cin >> answer;
			if (answer != 1 && answer != 2) {
				std::cout << "Введите нормальный ответ" << std::endl;
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
	else throw std::invalid_argument("Типом проверяемой строки в метод передано что попало\n");

}

/*
void UserAction::search_client(std::string value_type, std::string& value) {
	int answer = 0;
	int step = 0;
}
*/

void UserAction::user_interconnection(int& operation_index) {
	//char c = ' ';
	
	if (!this->change_client_data_flag && !this->delete_existing_client_mobile_phone) this->step = 0;
	else if (this->change_client_data_flag) this->step = 10;
	else if (this->delete_existing_client_mobile_phone) this->step = 14;
	std::string value;
	
	while (this->step >= 0) {
		
		switch(this->step) {
		case(0):
			this->input_data.clear();
			this->change_client_data_flag = false;
			this->mobile_phone_count = 0;
			//очищаем вектор ответов и std::pair
			this->answer_for_search_client.clear();
			std::for_each(this->b1.begin(), this->b1.end(), [](auto& n) {
				n.second = false;
				});
			
			
				std::cout << "\nВыберите дальнейшие действия с телефонной книгой:\n1.Добавить клиента\n2.Добавить телефон для существующего клиента\n";
				std::cout << "3.Изменить данные о клиенте\n4.Удалить телефон у существующего клиента\n5.Удалить существующего клиента\n";
				std::cout << "6.Найти клиента по его данным - имени, фамилии, email или телефону\n";
				std::cout << "7.Завершить работу\n";
				this->step = 8;
		
			
			break;
		case(1):
			//int case_one_count = 0;
			for (int i = 0; i < 5; i++) {
				//case_one_count++;
				if (this->step > 0) {
					
					if (i == 0) { this->adding_new_client_data_validation("name", value); continue; }
					
					
					if (i == 1) { this->adding_new_client_data_validation("surname", value); continue; }
				
					
					if (i == 2) { this->adding_new_client_data_validation("email", value); continue; }
						
					
					
					if (i == 3) {
						this->adding_new_client_data_validation("has_mobile_phone", value);
						if (value == "yes") {
							this->adding_new_client_data_validation("mobile_phone", value); continue;


						}
						else {
							this->step = -1;
							break;
						}
					} 
					if (i == 4) {
						this->adding_new_client_data_validation("one_more_mobile_phone", value);
						if (this->step > 0) this->step = -1;
							
						 
					}
						
					
				}//else break;
				
				 
				
					
				
			}
			//this->step = -1;
			break;
		case(2):
			for (int i = 0; i < 3; i++) {
				if (this->step > 0) {
					if (i == 0) {
						this->adding_new_client_data_validation("name", value); continue;
					}
					if (i == 1) {
						this->adding_new_client_data_validation("surname", value); continue;
						//this->step = -1;
					}
					if (i == 2) {
						this->adding_new_client_data_validation("mobile_phone", value); 
						if (this->step > 0) this->adding_new_client_data_validation("one_more_mobile_phone", value);
						if (this->step > 0) this->step = -1;
						//
						//continue;
					}
				}
				
			}
			break;
		case(3):
			//if (this->change_client_data_flag) this->step = 10;
			//else {
				for (int i = 0; i < 2; i++) {
					if (this->step > 0) {
						if (i == 0) {
							this->adding_new_client_data_validation("name", value); continue;
						}
						if (i == 1) {
							this->adding_new_client_data_validation("surname", value); 
							if (this->step > 0) this->step = -1;
							continue;
						}
					}
				}
			//}
				
			
			
				
				break;
			
			
		case(4):
			for (int i = 0; i < 2; i++) {
				if (this->step > 0) {
					if (i == 0) {
						this->adding_new_client_data_validation("name", value); continue;
					}
					if (i == 1) {
						this->adding_new_client_data_validation("surname", value);
						if (this->step > 0) this->step = -1;
						continue;
					}
				}
				

			}
			break;
		
		case(6):
			std::cout << "\nПо каким данным будем искать клиента?(можно выбрать один или несколько вариантов в любом порядке)\n";
			std::cout << "1.Имя\n";
			std::cout << "2.Фамилия\n";
			std::cout << "3.email\n";
			std::cout << "4.Номер телефона\n";
			this->step = 16;
			
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
					if (this->answer == 4 || this->answer == 5) this->step = 4; else this->step = i;
					break;
				}
				if (i == 7 && this->answer != i) {
					std::cout << "Введите нормальный ответ\n"; 
					this->step = 8;
					break;
				}
			}
			break;
		case(10):
			std::cout << "\nКакие данные будем менять у клиента?\n";
			std::cout << "1.Имя\n";
			std::cout << "2.Фамилия\n";
			std::cout << "3.email\n";
			if (this->mobile_phone_count > 0) {
				for (int i = 1; i <= this->mobile_phone_count; i++) {
					std::cout << std::to_string(i + 3) + "." + std::to_string(i) + "-й телефон\n";
				}
			}
			this->step = 11;
			break;
		case(11):
			std::cin >> this->answer;
			this->step = 12;
			
			break;
		case(12):
			if (this->change_client_data_flag) {
				for (int i = 1; i <= 3 + this->mobile_phone_count; i++) {
					if (this->answer == i) {
						this->step = 13;
						break;
					}
					if (i == 3 + this->mobile_phone_count && this->answer != i) {
						std::cout << "Введите нормальный ответ\n";
						this->step = 11;
						break;
					}

				}
			}
			else if (this->delete_existing_client_mobile_phone) {
				for (int i = 0; i < this->mobile_phone_count; i++) {
					if (this->answer == i + 1) {
						this->step = 15;
						break;
					}
					if (i == this->mobile_phone_count-1 && this->answer != i+1) {
						std::cout << "Введите нормальный ответ\n";
						this->step = 11;
						break;
					}

				}
			}
			
			break;
		case(13):
			operation_index = 13;
			//while (this->step > 0) {
				if (this->answer == 1) {
					this->adding_new_client_data_validation("name", value);
					if (this->step > 0) this->step = -1;
					
				}
				else if (this->answer == 2) {
					this->adding_new_client_data_validation("surname", value);
					if (this->step > 0) this->step = -1;
				}
				else if (this->answer == 3) {
					this->adding_new_client_data_validation("email", value);
					if (this->step > 0) this->step = -1;
				}
				else {
					this->adding_new_client_data_validation("mobile_phone", value);
					if (this->step > 0) this->step = -1;
				}
			//}
			//this->step = -1;
			break;
		case(14):
			std::cout << "\nКакой номер телефона будем удалять у клиента?\n";
			for (int i = 0; i < this->mobile_phone_count; i++) {
				std::cout << std::to_string(i + 1) + "." + std::to_string(i + 1) + "-й телефон\n";
			}
			this->step = 11;
			break;
		case(15):
			operation_index = 15;
			//this->adding_new_client_data_validation("mobile_phone", value);
			//if (this->step > 0) this->step = -1;
			this->step = -1;
			break;
		case(16):
			//очищаем вектор ответов и std::pair
			this->answer_for_search_client.clear();
			std::for_each(this->b1.begin(), this->b1.end(), [](auto& n) {
				n.second = false;
				});
			//складываем цифры введенного ответа в строку
			std::cin >> this->answer_for_search_client;
			this->step = 17;
			
			break;
		case(17):
			if (this->answer_for_search_client.size() > 4) {
				std::cout << "\nВведено слишком много вариантов для поиска. Введите заново\n";
				this->step = 16;
				
			}
			else if (this->answer_for_search_client.size() == 0) {
				std::cout << "\nНе введено ни одного варианта для поиска. Введите заново\n";
				this->step = 16;
			}
			else {
				for (int i = 0; i < this->answer_for_search_client.size(); i++) {
					for (int j = i + 1; j < this->answer_for_search_client.size(); j++) {
						if (this->answer_for_search_client[i] == this->answer_for_search_client[j]) {
							std::cout << "\nВарианты поиска введены несколько раз. Введите заново\n";
							this->step = 16;
							break;
						}
					}
					if (this->step == 16) break; 
					else {
						if (this->answer_for_search_client[i] != '1' && this->answer_for_search_client[i] != '2' && this->answer_for_search_client[i] != '3' && this->answer_for_search_client[i] != '4') {
							std::cout << "\nВведен некорректный ответ. Введите снова\n";
							this->step = 16;
							break;
						}


						else {
							this->b1[static_cast<int>((this->answer_for_search_client[i] - '0')) - 1].second = true;
							if (i == this->answer_for_search_client.size() - 1) this->step = 18;
						}

					}
				


				}
			}
			
			break;
		case(18):
			for (std::vector<std::pair<std::string, bool>>::const_iterator it = this->b1.begin(); it != this->b1.end(); ++it) {
				if (this->step > 0) {
					if (std::get<bool>(*it) && !std::get<bool>(*(this->b1.end() - 1)) && it != this->b1.end() - 1) {
						this->adding_new_client_data_validation(std::get<std::string>(*it), value);
						
					}
					else if (std::get<bool>(*it) && it == this->b1.end() - 1) {
						this->adding_new_client_data_validation(std::get<std::string>(*it), value);
						break;
					} 
					
				}else break;
				
			}
			if (this->step > 0) this->step = -1;
			break;
			
			}
			
			
			

		
	}
}