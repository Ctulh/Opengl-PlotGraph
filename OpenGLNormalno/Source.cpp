#include<GL/glut.h>
#include <graphics.h>
#include <dos.h>
#include<iostream>
#include <string>
#include <vector>
#include<cmath>
#include <fstream>




struct symbols
{
	char element;
	bool is_it_opration;
	int priority;
	float number;
};

std::string cords;
int level_brackets_priority = 4;

bool allright = true;

char opperations[5] = { '+','-','/','*','^' };
int sizee = sizeof(opperations) / sizeof(char);

bool check_brackets(std::vector<symbols> vect)
{
	int counter = 0;
	for (int i = 0; i < vect.size(); i++)
	{
		if (vect[i].element == ')')
		{
			counter--;
			if (counter < 0)
				return false;
		}
		else if (vect[i].element == '(')
			counter++;
	}
	if (counter == 0)
		return true;
	return false;
}


symbols settings(char ch)
{
	int i;
	symbols temp;

	temp.is_it_opration = false;
	temp.element = ch;
	temp.priority = 0;

	for (i = 0; i < sizee; i++)
	{
		if (temp.element == opperations[i]) {
			temp.is_it_opration = true;
			if (i <= 1)
				temp.priority = 1;
			else if (i <= 4)
				temp.priority = 2;
		}
	}
	if (!temp.is_it_opration)
		temp.number = ((int)temp.element) - 48;
	return temp;
}


void amount_brakets(std::vector<symbols> vect, int& total, int& max)
{
	total = 0;
	int count = 0;
	max = 0;
	for (int i = 0; i < vect.size(); i++)
	{
		if (vect[i].element == '(') {
			total++;
			count++;
			if (count > max)
				max = count;
		}
		else if (vect[i].element == ')')
		{
			count--;
		}
	}
}

void set_brackets_priority(std::vector<symbols>& vect)
{
	int total;
	int max;

	int counter = 0;

	for (auto it = vect.begin(); it != vect.end(); it++)
	{
		if ((*it).element == '(')
			counter++;
		else if ((*it).element == ')')
			counter--;
		(*it).priority += level_brackets_priority * counter;
	}
	std::vector<symbols>::iterator iter;

	amount_brakets(vect, total, max);
	while (total > 0)
	{

		for (iter = vect.begin(); iter != vect.end(); iter++)
		{
			if ((*iter).element == '(')
			{
				break;
			}
		}
		vect.erase(iter);
		for (iter = vect.begin(); iter != vect.end(); iter++)
		{
			if ((*iter).element == ')')
			{
				break;
			}
		}
		vect.erase(iter);
		amount_brakets(vect, total, max);
	}
}

int max_priority(std::vector<symbols> vect)
{
	int max = 0;
	for (std::vector<symbols>::iterator it = vect.begin(); it != vect.end(); it++)
	{
		if ((*it).priority > max && (*it).is_it_opration)
			max = (*it).priority;
	}
	return max;
}

void vect_operation(std::vector<symbols>& vect, int i)
{
	try {
		switch (vect[i].element)
		{
		case '+':
			vect[i - 1].number = vect[i - 1].number + vect[i + 1].number;
			break;
		case '-':
			vect[i - 1].number = vect[i - 1].number - vect[i + 1].number;
			break;
		case'*':
			vect[i - 1].number = vect[i - 1].number * vect[i + 1].number;
			break;
		case '/':
			vect[i - 1].number = vect[i - 1].number / vect[i + 1].number;
			break;
		case 's':
			vect[i + 1].number = sin(vect[i + 1].number);
			break;
		case 'c':
			vect[i + 1].number = cos(vect[i + 1].number);
			break;
		case '^':
			vect[i - 1].number = pow(vect[i - 1].number, vect[i].number);
			break;
		}
		int j = 0;
		std::vector<symbols>::iterator it;

		for (it = vect.begin(); it != vect.end(); ++it)
		{

			if (j == i)
				break;
			j++;
		}
		if ((*it).element == 's' || (*it).element == 'c' || (*it).element == '^')
			vect.erase(it);
		else
			vect.erase(it, it + 2);
	}
	catch (...)
	{
		std::cout << "Something went wrong. please send a report by mail 'pilnik2016@gmail.com' ";
	}
}


bool is_double_in_vector(std::vector<symbols> vect)
{
	for (std::vector<symbols>::iterator it = vect.begin(); it != vect.end(); it++)
	{
		if ((*it).element == '.')
			return true;
	}
	return false;
}

void check_double(std::vector<symbols>& vect)
{
	for (std::vector<symbols>::iterator it = vect.begin(); it != vect.end(); it++)
	{
		if ((*it).element == '.')
			(*(it - 1)).number = (*(it - 1)).number + (*(it + 1)).number / 10;
	}

	while (is_double_in_vector(vect)) {

		std::vector<symbols>::iterator it;
		for (it = vect.begin(); it != vect.end(); it++)
		{
			if ((*it).element == '.')
				break;
		}
		vect.erase(it, it + 2);
	}
}


void check_sctg(std::vector<symbols>& vect)
{
	std::vector<symbols>::iterator it;
	for (it = vect.begin(); it != vect.end(); it++)
	{
		if (((*it).element == 's' && (*(it + 1)).element == 'i'))
		{
			(*it).element = 's';
			(*it).is_it_opration = true;
			vect.erase(it + 1, it + 3);
			(*it).priority = (*(it + 1)).priority;

			break;
		}
		else if ((*it).element == 'c' && (*(it + 1)).element == 'o')
		{
			(*it).element = 'c';
			(*it).is_it_opration = true;
			vect.erase(it + 1, it + 3);
			(*it).priority = (*(it + 1)).priority;
		}
	}
}

void check_power(std::vector<symbols>& vect)
{
	for (std::vector<symbols>::iterator it = vect.begin(); it != vect.end(); it++)
	{
		if ((*it).is_it_opration && (*it).element == '^')
		{
			(*it).number = (*(it + 1)).number;
			vect.erase(next(it));
		}
	}
}

void check_negative(std::vector<symbols>& vect)
{
	if (vect[0].is_it_opration && vect[0].element == '-')
	{
		vect[1].number = -vect[1].number;
		vect.erase(vect.begin());
	}
	if (vect.size() > 2)
	{
		for (int i = 2; i < vect.size(); i++)
		{
			if (vect[i - 2].is_it_opration && vect[i - 1].is_it_opration && vect[i - 1].element == '-')
			{
				vect[i].number = -vect[i].number;
				int j = 0;
				std::vector<symbols>::iterator it;
				for (it = vect.begin(); j != i; it++)
					j++;
				vect.erase(prev(it));
			}
		}
	}
}



POINTFLOAT arr[1000];
int cnt = 1;



void check_numbers(std::vector<symbols>& vect)
{
	
		std::vector<symbols>::iterator it;
		if (vect.size() > 2) {
			for (it = vect.begin(); it != (it - 1); it++)
			{
				if ((!(*it).is_it_opration) && (!(*next(it)).is_it_opration))
				{
					(*it).number = (*it).number * 10 + (*next(it)).number;
					vect.erase(next(it));
					it = vect.begin();
				}
			}
		}
}


void calculate(std::vector<symbols> vect)
{
		check_double(vect);//function check non-integers;

		set_brackets_priority(vect);//Function set priority for equations in bbrackets 
		check_negative(vect);//Function check negative numbers in vector "vect"
		check_numbers(vect);//Function check digits that take more then 1 char in string and make numbers  
		check_power(vect);//Function checks the degree in the equation 

		int i = 0;
		for (std::vector<symbols>::const_iterator it = vect.begin(); it != vect.end(); it++)
		{
			check_sctg(vect);
			if ((*it).is_it_opration && (*it).priority == max_priority(vect))
			{
				vect_operation(vect, i);
				it = vect.begin();
				i = 0;
			}
			if (vect.size() == 1)
				break;
			i++;

		}

		arr[cnt].y = vect[0].number;	
}

bool contain_x(std::vector<symbols> vect)
{
	for (int i = 0; i < vect.size(); i++)
	{
		if (vect[i].element == 'x')
			return true;
	}
	return false;
}

void for_undefind(std::string str)
{
	std::vector <symbols> vect;

	for (int i = 0; i < str.length(); i++)
	{
		vect.push_back(settings(str[i]));
	}
	if (check_brackets(vect)) {
		std::vector<symbols>::iterator it;
		for (it = vect.begin(); it != vect.end(); it++)
		{

			if ((*it).element == 'x')
				break;
		}
		if (contain_x(vect)) {
			for (int i = -10; i <= 10; i++)
			{
				for (float j = i; j <= i + 1; j += 0.1)
				{
					arr[cnt].x = j;
					(*it).number = j;//error if cant find undefind 'x'
					calculate(vect);
					cnt++;
				}
			}
		}
		else
		{
			std::cout << "Sorry. Cant find undefind 'x'";
			allright = false;
		}
	}
	else
	{
		std::cout << "Incorrect sequence of brackets. Try again...";
		allright = false;
	}


}

void lines(float alfa)
{
	static float d = 5;
	glPushMatrix();
	glRotatef(alfa,0,0,1);
	glBegin(GL_LINES);
		glVertex2f(-500, 0);
		glVertex2f(500, 0);
		glVertex2f(500, 0);
		glVertex2f(500-d, 0+d);
		glVertex2f(500, 0);
		glVertex2f(500-d, 0-d);
	glEnd();
	glPopMatrix();
}


void szhim()
{
	float sx = 2.0 / (arr[cnt - 1].x - arr[0].x);
	glPushMatrix();
	glScalef(sx, 0.5, 1);
}

void owninit()
{
	glBegin(GL_LINE_STRIP);
	for (int i = 1; i < cnt; i++)
		glVertex2f(arr[i].x, arr[i].y);
	glEnd();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	szhim();
	glLineWidth(2);
	glColor3f(1, 1, 1);
	lines(0);//horizontal line
	glColor3f(1, 1, 1);
	lines(90);//vertical line
	glColor3f(0, 1, 1);
	owninit();
	glFlush();
}

void myinit() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(5.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 499.0, 0.0, 499.0);
}

void spaces(int n)
{
	for (int i = 0; i < n; i++)
		std::cout << " ";
}

void show(std::string str)
{
	int temp = (28 - str.length()) / 2;
	if ((temp * 2) + str.length() == 28)
	{
		spaces(temp);
		std::cout << str;
		spaces(temp);
	}
	else if ((temp * 2) + str.length() == 29)
	{
		spaces(temp - 1);
		std::cout << str;
		spaces(temp);
	}
	else if ((temp * 2) + str.length() == 27)
	{
		spaces(temp + 1);
		std::cout << str;
		spaces(temp);
	}
}


void main_show()
{
	for (int i = 0; i <= 10; i++)
	{

		if (i % 10 == 0)
		{
			std::cout << "+";
			for (int j = 0; j < 28; j++)
			{
				std::cout << "-";
			}
			std::cout << "+" << std::endl;
		}
		else
		{
			std::cout << "|";
			if (i == 1)
				show("Working:");
			else if (i == 3)
				show("Sin, Cos");
			else if (i == 4)
				show("+, -, *, /");
			else if (i == 5)
				show("Exponentiation '^' ");
			else
			{
				spaces(28);
			}
			std::cout << "|";
			if (i == 3)
			{
				spaces(10);
				std::cout<<"Examples: sinx, sin(x)"<<std::endl;
			}
			else if (i == 4)
			{
				spaces(10);
				std::cout << "Examples: x+2, 1-x, x*2" << std::endl;
			}
			else if (i == 5)
			{
				spaces(10);
				std::cout << "Examples: x^2" << std::endl;
			}
			else
			{
				std::cout << std::endl;
			}
		}
	}
	std::cout << "!Your equation must contain unknown x. If you find an error, please send a report to the pilnik2016@gmail.com" << std::endl;
	std::cout << "Enter your equation: ";
}

void main(int argc, char** argv) {
	main_show();
	std::string str;
	std::cin >> str;
	for_undefind(str);
	if (allright) {
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
		glutInitWindowSize(500, 500);
		glutInitWindowPosition(0, 0);
		glutCreateWindow("Points");
		glutDisplayFunc(display);
		myinit();
		glutMainLoop();
	}
}