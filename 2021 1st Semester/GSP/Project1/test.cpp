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
    printf("ù��° ������ �Է�: ");
    scanf_s("%d", &firstNumber);
    printf("�ι�° ������ �Է�: ");
    scanf_s("%d", &secondNumber);

    //���� ����!!
    scanf_s("%c", &ch, 1);
    printf("�����ȣ�� �Է�(+,-,*,/,%): ");
    scanf_s("%c", &ch, 1);

    int result = calc(firstNumber, secondNumber, ch);

    printf("%d\n", result);
    return 0;
}