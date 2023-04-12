#include <iostream>
#include <vector>
#include <stack>
#include <list>
#include <algorithm>
#include <string>
#include <iterator>

struct Data{

    std::vector<char> input;
    std::vector<double> onp;
    std::stack<double> to_onp;
    std::stack<double> to_result;
    const double plus = -5;
    const double minus = -3;
    const double mult = -6;
    const double divi = -1;
    const double l_br = -8;
    const double r_br = -7;
    
};

void string_to_vector(Data &data, std::string temp);
void make_spaces(Data &data);
bool is_digit_char(Data data, int pos);
bool is_double(Data data, int pos);
bool is_operator(Data data, int pos);
double vector_to_double(Data data, int &pos);
double operator_to_double(Data &data, int &pos);
void stack_not_empty(Data &data, double temp);
void change_to_ONP(Data &data);
double operations(double a, double b, double oper, Data data);
double result(Data &data);


int main()
{
    Data data;
    std::string temp;
    
    std::getline(std::cin, temp);
    string_to_vector(data, temp);

    make_spaces(data);
    change_to_ONP(data);
    std::cout << result(data) << " <- Result" << std::endl;

}

void string_to_vector(Data &data, std::string temp)
{
    data.input.assign(temp.begin(), temp.end());
    data.input.erase(remove(data.input.begin(), data.input.end(), ' '), data.input.end());
}

void make_spaces(Data &data)
{
    int elem = 0;
    for(int i = 0; i < data.input.size(); i++)
    {

        if(data.input[elem] == '+' || data.input[elem] == '-' || data.input[elem] == '*' || data.input[elem] == '/'
        || data.input[elem] == '(' || data.input[elem] == ')')
        {
            data.input.insert(data.input.begin() + elem, 32);
            data.input.insert(data.input.begin() + elem + 2, 32);
            elem += 3; 
            continue;
        }
        elem++;
    }
}

bool is_digit_char(Data data, int pos)
{
    if(data.input[pos] >= '0' && data.input[pos] <= '9')
        return true;
    else
        return false;
}

bool is_double(Data data, int pos)
{
    if(data.onp[pos] >= 0)
        return true;
    else
        return false;
}

bool is_operator(Data data, int pos)
{
    if(data.input[pos] == '+' || data.input[pos] == '-' || data.input[pos] == '*' || data.input[pos] == '/')
        return true;
    else 
        return false;
}

double vector_to_double(Data data, int &pos)
{
    double num = 0;
    while(pos < data.input.size() && is_digit_char(data, pos))
    {
        num = num * 10 + data.input[pos] - '0';
        pos++;
    }
    pos--;
    return num;
}

double operator_to_double(Data &data, int &pos)
{
    return data.input[pos] - '0';
}

void stack_not_empty(Data &data, double temp)
{
    if(!data.to_onp.empty())
    {
        if(temp == data.minus)
        {
            if(data.to_onp.top() != data.l_br)
            {
                data.onp.insert(data.onp.end(), data.to_onp.top());
                data.to_onp.pop();
                stack_not_empty(data, temp);
            }
            data.to_onp.push(temp);
        }
        else if(temp == data.divi)
        {
            data.to_onp.push(temp);
        }
        else if(temp == data.plus)
        {
            if(data.to_onp.top() != data.l_br)
            {
                data.onp.insert(data.onp.end(), data.to_onp.top());
                data.to_onp.pop();
                stack_not_empty(data, temp);
            }
            data.to_onp.push(temp);
        }
        else if(temp == data.mult)
        {
            data.to_onp.push(temp);
        }
    }
}

void change_to_ONP(Data &data)
{
    int i = 0;
    for(int pos = 0; pos < data.input.size() + 1; pos++)
    {
        if(is_digit_char(data, pos))
        {
            data.onp.insert(data.onp.end(), vector_to_double(data, pos));
            i++;
        }
        if(is_operator(data, pos))
        {
            double temp = operator_to_double(data, pos);
            if(!data.to_onp.empty())
            {
                stack_not_empty(data, temp);
            }
            else
            {
                data.to_onp.push(temp);
            }
        }
        if(data.input[pos] == '(')
        {
            double temp = operator_to_double(data, pos);
            data.to_onp.push(temp);
        }
        if(data.input[pos] == ')')
        {
           while(data.to_onp.top() != data.l_br)
            {
                data.onp.insert(data.onp.end(), data.to_onp.top());
                data.to_onp.pop();
            }
            data.to_onp.pop();
        }
    }
    while(!data.to_onp.empty())
    {
        data.onp.insert(data.onp.end(), data.to_onp.top());
        data.to_onp.pop();
    }


}

double operations(double a, double b, double oper, Data data)
{
    if(oper == data.plus)
    {
        return a + b;
    }
    if(oper == data.minus)
    {
        return a - b;
    }
    if(oper == data.mult)
    {
        return a * b;
    }
    if(oper == data.divi)
    {
        return a / b;
    }
    return 0;
}

double result(Data &data)
{
    double a, b, oper;
    for( int i = 0; i < data.onp.size(); i++)
    {
        if(is_double(data, i))
        {
            data.to_result.push(data.onp[i]);
        }
        else
        {
            if(data.onp[i] < 0)
            {
                a = data.to_result.top();
                data.to_result.pop();
                b = data.to_result.top();
                data.to_result.pop();
                data.to_result.push(operations(b, a, data.onp[i], data));
            }
        }
    }
    return data.to_result.top();
}