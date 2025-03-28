
/*  call_stack

    ���� �ý��ۿ����� ������ �޸𸮿� ���������, �� ���������� `int` �迭�� �̿��Ͽ� �޸𸮸� �����մϴ�.
    ������ SFP�� Return Address�� ���� ���� �޸� �ּҰ� ��������, �̹� ���������� -1�� ��ü�մϴ�.

    int call_stack[]      : ���� ������(`int ��`) �Ǵ� `-1` (��Ÿ������ ���п�)�� �����ϴ� int �迭
    char stack_info[][]   : call_stack[]�� ���� ��ġ(index)�� ���� ������ �����ϴ� ���ڿ� �迭

    ==========================call_stack ���� ��Ģ==========================
    �Ű� ���� / ���� ������ push�� ���   : int �� �״��
    Saved Frame Pointer �� push�� ���  : call_stack������ index
    ��ȯ �ּҰ��� push�� ���       : -1
    =======================================================================


    ==========================stack_info ���� ��Ģ==========================
    �Ű� ���� / ���� ������ push�� ���        : ������ ���� ����
    Saved Frame Pointer �� push�� ���  : � �Լ��� SFP����
    ��ȯ �ּҰ��� push�� ���                 : "Return Address"
    ========================================================================
*/
#include <stdio.h>
#define STACK_SIZE 50 // �ִ� ���� ũ��

int     call_stack[STACK_SIZE];         // Call Stack�� �����ϴ� �迭
char    stack_info[STACK_SIZE][20];     // Call Stack ��ҿ� ���� ������ �����ϴ� �迭

/*  SP (Stack Pointer), FP (Frame Pointer)

    SP�� ���� ������ �ֻ�� �ε����� ����ŵ�ϴ�.
    ������ ������� �� SP = -1, �ϳ��� ���̸� `call_stack[0]` -> SP = 0, `call_stack[1]` -> SP = 1, ...

    FP�� ���� �Լ��� ���� ������ �������Դϴ�.
    ���� ���� �Լ� ���� �������� sfp�� ����ŵ�ϴ�.
*/
int SP = -1;
int FP = -1;

int func1(int arg1, int arg2, int arg3);
int func2(int arg1, int arg2);
int func3(int arg1);

/*
    ���� call_stack ��ü�� ����մϴ�.
    �ش� �Լ��� ��� ������� �������� ���� �ϼ����� ���� �����Դϴ�.
*/
void print_stack()
{
    if (SP == -1)
    {
        printf("Stack is empty.\n");
        return;
    }

    printf("====== Current Call Stack ======\n");

    for (int i = SP; i >= 0; i--)
    {
        if (call_stack[i] != -1)
            printf("%d : %s = %d", i, stack_info[i], call_stack[i]);
        else
            printf("%d : %s", i, stack_info[i]);

        if (i == SP)
            printf("    <=== [esp]\n");
        else if (i == FP)
            printf("    <=== [ebp]\n");
        else
            printf("\n");
    }
    printf("================================\n\n");
}

void push(char* s, int a) {
    SP++;
    call_stack[SP] = a;
    strcpy(stack_info[SP], s);
}

void pop() {
    SP--;
}

void local_var_push(int count, int arr[], char str[][10]) {
	SP += count;
	for (int i = 0; i < count; i++) {
		call_stack[SP - count +i +1] = arr[i];
		strcpy(stack_info[SP - count + i + 1], str[i]);
	}
}

void local_var_pop(int count) {
	SP -= count;
}

//func ���δ� �����Ӱ� �߰��ص� ��������, �Ʒ��� ������ �ٲ����� ������
int func1(int arg1, int arg2, int arg3)
{
    // ���� ����
    int var_1 = 100;
    int local_var_count = 1;

    //���� ���� �迭(���� �Ҵ�)
	int* var_arr = (int*)malloc(sizeof(int) * local_var_count);
	var_arr[0] = var_1;
	char (*var_name_arr)[10] = (char (*)[10])malloc(sizeof(char) * 10 * local_var_count);
	strcpy(var_name_arr[0], "var_1");

    // func1�� ���� ������ ���� (�Լ� ���ѷα� + push)
    push("arg3", arg3);
    push("arg2", arg2);
    push("arg1", arg1);
    push("Return Adress", -1);
    push("func1 SFP", FP);
    FP = SP;
	local_var_push(local_var_count, var_arr, var_name_arr);

    // ���� stack ���
    print_stack();

    // func2 ȣ��
    int return_count = func2(11, 13);

    // func2�� ���� ������ ���� (�Լ� ���ʷα� + pop)
	local_var_pop(return_count);
    FP = call_stack[FP];
    pop();
    pop();
    pop();
    pop();

    // ���� stack ���
    print_stack();

	//���� �Ҵ� �޸� ����
	free(var_arr);
	free(var_name_arr);

	// ���� ���� ���� ����
	return local_var_count;
}


int func2(int arg1, int arg2)
{
    // ���� ����
    int var_2 = 200;
	int local_var_count = 1;

    //���� ���� �迭(���� �Ҵ�)
	int* var_arr = (int*)malloc(sizeof(int) * local_var_count);
	var_arr[0] = var_2;
    char (*var_name_arr)[10] = (char (*)[10])malloc(sizeof(char) * 10 * local_var_count);
	strcpy(var_name_arr[0], "var_2");

    // func2�� ���� ������ ���� (�Լ� ���ѷα� + push)
    push("arg2", arg2);
    push("arg1", arg1);
    push("Return Adress", -1);
    push("func2 SFP", FP);
    FP = SP;
    local_var_push(local_var_count, var_arr, var_name_arr);

    // ���� stack ���
    print_stack();

    // func3 ȣ��
    int return_count = func3(77);

    // func3�� ���� ������ ���� (�Լ� ���ʷα� + pop)
	local_var_pop(return_count);
    FP = call_stack[FP];
    pop();
    pop();
    pop();

    // ���� stack ���
    print_stack();

	//���� �Ҵ� �޸� ����
	free(var_arr);
	free(var_name_arr);

	// ���� ���� ���� ����
	return local_var_count;
}


int func3(int arg1)
{
    // ���� ����
    int var_3 = 300;
    int var_4 = 400;
	int local_var_count = 2;

	//���� ���� �迭(���� �Ҵ�)
	int* var_arr = (int*)malloc(sizeof(int) * local_var_count);
	var_arr[0] = var_3;
	var_arr[1] = var_4;
    char (*var_name_arr)[10] = (char (*)[10])malloc(sizeof(char) * 10 * local_var_count);
	strcpy(var_name_arr[0], "var_3");
	strcpy(var_name_arr[1], "var_4");

    // func3�� ���� ������ ���� (�Լ� ���ѷα� + push)
    push("arg1", arg1);
    push("Return Adress", -1);
    push("func3 SFP", FP);
    FP = SP;
    local_var_push(local_var_count, var_arr, var_name_arr);

    // ���� stack ���
    print_stack();

    //���� �Ҵ� �޸� ����
	free(var_arr);
	free(var_name_arr);

    // ���� ���� ���� ����
	return local_var_count;
}


//main �Լ��� ���õ� stack frame�� �������� �ʾƵ� �˴ϴ�.
int main()
{
    int return_count = func1(1, 2, 3);
    // func1�� ���� ������ ���� (�Լ� ���ʷα� + pop)
	local_var_pop(return_count);
    FP = -1;
    pop();
    pop();
    pop();
    pop();
    pop();

    // ���� stack ���
    print_stack();

    // ��
    return 0;
}