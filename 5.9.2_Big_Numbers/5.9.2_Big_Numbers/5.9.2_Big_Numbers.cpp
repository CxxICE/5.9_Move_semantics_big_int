#include <iostream>
#include <string>
#include <stdexcept>

#define PLUS 43		//ASCII +
#define MINUS 45	//ASCII -
#define ZERO 48		//ASCII 0
#define NINE 57		//ASCII 9

class NotInt : public std::domain_error
{
public:
	NotInt(std::string err) : std::domain_error(err) {};
};

class big_integer
{
public:

	big_integer()
	{
		negativ = false;
		_number = "";
	}

	big_integer(const std::string &number)//конструктор на основе string, первый знак "+" или "-" отбрасывается, 
		//хранится положительное число с признаком negativ = true для отрицательных чисел
	{
		int i_start = 0;
		negativ = false;
		if (static_cast<int>(number[0]) == PLUS)
		{
			i_start = 1;
		}
		else if(static_cast<int>(number[0]) == MINUS)
		{
			i_start = 1;
			negativ = true;
		}
		for (int i = i_start; i < number.length(); ++i)//проверка на число введенного в конструктор значения
		{			
			if(!(static_cast<int>(number[i]) >= ZERO && static_cast<int>(number[i]) <= NINE))
			{
				std::string message = "Введенное значение \"" + number + "\" не является целым числом!";
				throw NotInt(message.c_str());
			}
		}
		_number = number.substr(i_start, number.length() - i_start);//запись числа в поле _number без знака			
	};

	big_integer(const big_integer &other)//конструктор копирования
	{
		_number = other._number;
		negativ = other.negativ;
	};

	big_integer(big_integer &&other) noexcept//конструктор перемещения
	{
		_number = std::move(other._number);
		negativ = other.negativ;
		other.negativ = false;
	};

	big_integer &operator=(const big_integer &other)//оператор присваивания копированием
	{
		if (&other != this)
		{
			_number = other._number;
			negativ = other.negativ;
		}
		return *this;
	};	

	big_integer &operator=(big_integer &&other) noexcept//оператор присваивания перемещением
	{
		if (&other != this)
		{
			_number = std::move(other._number);
			negativ = other.negativ;
			other.negativ = false;
		}
		return *this;
	};

	friend big_integer operator+(const big_integer &left, const big_integer &right);
	friend big_integer operator+(const big_integer &left, const int right);
	friend big_integer operator+(const int right, const big_integer &left);

	friend big_integer operator-(const big_integer &left, const big_integer &right);
	friend big_integer operator-(const big_integer &left, const int right);
	friend big_integer operator-(const int right, const big_integer &left);

	friend bool operator>(const big_integer &left, const big_integer &right);
	friend bool operator<(const big_integer &left, const big_integer &right);
	friend bool operator>=(const big_integer &left, const big_integer &right);
	friend bool operator<=(const big_integer &left, const big_integer &right);
	friend bool operator==(const big_integer &left, const big_integer &right);
	friend bool operator!=(const big_integer &left, const big_integer &right);

	friend std::ostream &operator<<(std::ostream &stream, const big_integer &other);	

	big_integer abs() const//модуль числа
	{
		big_integer rezult = *this;
		rezult.negativ = false;
		return rezult;
	}

	operator std::string()
	{
		return _number;
	};

private:
	std::string _number;
	bool negativ;
};

std::ostream &operator<<(std::ostream &stream, const big_integer &other)
{
	if (other.negativ)
	{
		stream << "-" + other._number;
	}
	else
	{
		stream << other._number;
	}	
	return stream;
};

big_integer operator+(const big_integer &left, const big_integer &right)
{
	big_integer result;
	int shift = 0;
	int sum = 0;
	
	if (left.negativ && !right.negativ)
	{
		result = right - left.abs();
	}
	else if (!left.negativ && right.negativ)
	{
		result = left - right.abs();
	}
	else
	{
		if (left.negativ && right.negativ)
		{
			result.negativ = true;
		}
		else
		{
			result.negativ = false;
		}
				
		for (int i_left = left._number.length() - 1, i_right = right._number.length() - 1; i_left >= 0 || i_right >= 0; --i_left, --i_right)
		{
			if (i_left < 0)
			{
				char a = right._number[i_right];
				sum = atoi(&a) + shift;
			}
			else if (i_right < 0)
			{
				char a = left._number[i_left];
				sum = atoi(&a) + shift;
			}
			else
			{
				char a = left._number[i_left];
				char b = right._number[i_right];
				sum = atoi(&a) + atoi(&b) + shift;				
			}
			shift = sum / 10;
			result._number = std::to_string(sum % 10) + result._number;
		}
		if (shift > 0)
		{
			result._number = std::to_string(shift) + result._number;
		}
	}
	return result;
};

big_integer operator+(const big_integer &left, const int right)
{
	big_integer tmp_int(std::to_string(right));

	return left + tmp_int;
};

big_integer operator+(const int left, const big_integer &right)
{
	return right + left;
};

big_integer operator-(const big_integer &left, const big_integer &right)
{
	big_integer result;
	int shift = 0;
	int dif = 0;
	if (left.negativ && right.negativ)
	{
		result = right.abs() - left.abs();
	}
	else if (!left.negativ && right.negativ)
	{
		result = left + right.abs();
	}
	else if (left.negativ && !right.negativ)
	{
		result = left.abs() + right.abs();
		result.negativ = true;
	}
	else
	{
		if (left >= right)
		{
			result.negativ = false;
			for (int i_left = left._number.length() - 1, i_right = right._number.length() - 1; i_left >= 0 || i_right >= 0; --i_left, --i_right)
			{
				if (i_right < 0)
				{
					char a = left._number[i_left];
					dif = atoi(&a) - shift;
					shift = 0;
				}
				else
				{
					char a = left._number[i_left];
					char b = right._number[i_right];
					if ((atoi(&a) - shift) >= atoi(&b))
					{
						dif = atoi(&a) - shift - atoi(&b);
						shift = 0;
					}
					else
					{
						dif = atoi(&a) + 10 - shift - atoi(&b);
						shift = 1;
					}
				}
				result._number = std::to_string(dif) + result._number;
			}
		}
		else
		{
			result = right - left;
			result.negativ = true;
		}
		
		for (int i = 0; i < result._number.length(); ++i)//удаление незначащих нулей
		{
			if (result._number[i] == '0' && result._number.length() != 1)
			{
				result._number.erase(0, 1);
				--i;
			}
			else
			{
				break;
			}
		}
	}
	return result;
};

big_integer operator-(const big_integer &left, const int right)
{
	big_integer tmp_int(std::to_string(right));

	return left - tmp_int;
};

big_integer operator-(const int left, const big_integer &right)
{
	big_integer tmp_int(std::to_string(left));

	return tmp_int - right;
};

bool operator>(const big_integer &left, const big_integer &right)
{
	std::string s_left = left._number;
	std::string s_right = right._number;
	if (s_left.length() > s_right.length())//выравнивание для лексикографического сравнения
	{
		s_right.insert(0, s_left.length() - s_right.length(), '0');
	}
	else if (left._number.length() < right._number.length())//выравнивание для лексикографического сравнения
	{
		s_left.insert(0, s_right.length() - s_left.length(), '0');
	}
	
	if (!left.negativ && !right.negativ)
	{
		return s_left > s_right;
	}
	else if (left.negativ && right.negativ)
	{
		return s_left < s_right;
	}
	else if (left.negativ && !right.negativ)
	{
		return false;
	}
	else
	{
		return true;
	}
};

bool operator<=(const big_integer &left, const big_integer &right)
{
	return !(left > right);
};

bool operator<(const big_integer &left, const big_integer &right)
{
	std::string s_left = left._number;
	std::string s_right = right._number;
	if (s_left.length() > s_right.length())//выравнивание для лексикографического сравнения
	{
		s_right.insert(0, s_left.length() - s_right.length(), '0');
	}
	else if (left._number.length() < right._number.length())//выравнивание для лексикографического сравнения
	{
		s_left.insert(0, s_right.length() - s_left.length(), '0');
	}

	if (!left.negativ && !right.negativ)
	{
		return s_left < s_right;
	}
	else if (left.negativ && right.negativ)
	{
		return s_left > s_right;
	}
	else if (left.negativ && !right.negativ)
	{
		return true;
	}
	else
	{
		return false;
	}
};

bool operator>=(const big_integer &left, const big_integer &right)
{
	return !(left < right);
};

bool operator==(const big_integer &left, const big_integer &right)
{
	if ((!left.negativ && !right.negativ) || (left.negativ && right.negativ))
	{
		return left._number == right._number;
	}
	else
	{
		return false;
	}
};

bool operator!=(const big_integer &left, const big_integer &right)
{
	return !(left == right);
};

int main()
{
	setlocale(LC_ALL, "RU");
	big_integer number1, number2, number3, number4, number5;
	try
	{
		number1 = big_integer("114575");
		number2 = big_integer("78524");
		number3 = big_integer("-78524");
		number4 = big_integer("-20");
		number5 = big_integer("20g");
	}
	catch (const NotInt &er)
	{
		std::cout << er.what() << std::endl;
	}
	
	std::cout << "number1 = " << number1 << std::endl;
	std::cout << "number2 = " << number2 << std::endl;
	std::cout << "number3 = " << number3 << std::endl;
	std::cout << "number4 = " << number4 << std::endl;
	
	std::cout << "\n";
	std::cout << "number1 + number2 = " << number1 + number2 << std::endl; // (+)+(+) 193099
	std::cout << "number1 + number3 = " << number1 + number3 << std::endl; // (+)+(-) 36051
	std::cout << "number3 + number1 = " << number3 + number1 << std::endl; // (-)+(+) 36051
	std::cout << "number3 + number4 = " << number3 + number4 << std::endl; // (-)+(-) -78544
	std::cout << "4 + number2 = " << 4 + number2 << std::endl;			   // (int)+(+) 78528
	std::cout << "number2 + 4 = " << number2 + 4 << std::endl;			   // (+)+(int) 78528

	std::cout << "\n";
	std::cout << "number1 - number2 = " << number1 - number2 << std::endl; // (+)-(+) 36051
	std::cout << "number1 - number3 = " << number1 - number3 << std::endl; // (+)-(-) 193099
	std::cout << "number3 - number1 = " << number3 - number1 << std::endl; // (-)-(+) -193099
	std::cout << "number3 - number4 = " << number3 - number4 << std::endl; // (-)-(-) -78504
	std::cout << "4 - number2 = " << 4 - number2 << std::endl;			   // (int)-(+) -78520
	std::cout << "number2 - 4 = " << number2 - 4 << std::endl;			   // (+)-(int) 78520

	std::cout << "\n";
	std::cout << "number2 + number3 = " << number2 + number3 << std::endl; // 0
	std::cout << "number2 - number2 = " << number2 - number2 << std::endl; // 0
}