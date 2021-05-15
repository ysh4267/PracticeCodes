#include <iostream>

int calc(int first, int second, char ch) {

    switch (ch)
    {
    case '+':
        return first + second;
        break;
    case '-':
        return first - second;
        break;
    case '*':
        return first * second;
        break;
    case '/':
        return first / second;
        break;
    case '%':
        return first % second;
        break;
    default:
        break;
    }

}

int main(void)
{
    int firstNumber = 0, secondNumber = 0;
    char ch = 0;//NULL
    printf("첫번째 정수를 입력: ");
    scanf_s("%d", &firstNumber);
    printf("두번째 정수를 입력: ");
    scanf_s("%d", &secondNumber);

    //여기 질문!!
    scanf_s("%c", &ch, 1);
    printf("연산기호를 입력(+,-,*,/,%): ");
    scanf_s("%c", &ch, 1);

    int result = calc(firstNumber, secondNumber, ch);

    printf("%d\n", result);
    return 0;
}