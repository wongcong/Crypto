#include<stdio.h>
#include<math.h>
#include<string.h>

#define maxsize 1024

typedef struct {
	int key;
	int N;
} key;

void PrintPrime();
int CheckPrime(int);
int ExtendedEuclid(int, int, int *);
void Crypt_Int(int, int *, key);
int CharToInt(char *, int *);
int IntToHexChar(int *, char *, int);
int HexCharToInt(char *, int *, int);
int IntToChar(int *, char *, int);

int main() {
	key Public_Key, Secret_Key;
	int prime_P, prime_Q, Phi_N, Plain_Int[maxsize] = { 0 }, Cipher_Int[maxsize] = { 0 };
	int i, flag, length;
	char Plain[maxsize] = { 0 }, Cipher[maxsize] = { 0 };
	PrintPrime();
	printf("Please choose p & q from these prime.\n");
	scanf("%d%d", &prime_P, &prime_Q);
	while (1) {
		if (!CheckPrime(prime_P) || !CheckPrime(prime_Q)) {
			printf("Error data!\n");
			printf("Please choose p & q from these prime.\n");
			scanf("%d%d", &prime_P, &prime_Q);
		}
		else break;
	}

	Public_Key.N = prime_P * prime_Q;
	Secret_Key.N = Public_Key.N;
	Phi_N = (prime_P - 1) * (prime_Q - 1);

	printf("Please choose Public_Key.key.(1 < Public_Key.key < Phi_N)\n");
	scanf("%d", &Public_Key.key);
	while (1) {
		if (!ExtendedEuclid(Phi_N, Public_Key.key, &Secret_Key.key))
		{
			printf("Error data!\n");
			printf("Please keep Public_Key.key with Phi_N is a pair of coprime.\n");
			printf("You can choose a number form prime table.\n");
			scanf("%d", &Public_Key.key);
		}
		else break;
	}

	printf("OK!\n");
	printf("Your Public Key is {%d, %d}\n", Public_Key.key, Public_Key.N);
	printf("Your Secret Key is {%d, %d}\n", Secret_Key.key, Public_Key.N);

	printf("Plaese choose your service.\n1.Encrypt\n2.Decrypt\n");
	scanf("%d", &flag);
	while (flag != 1 && flag != 2) {
		printf("Error input, please input 1 or 2.\n");
		scanf("%d", flag);
	}

	if (flag == 1) {
		printf("Please input your Plain.\n");
		scanf("%s", Plain);
		length = CharToInt(Plain, Plain_Int, strlen(Plain));
		for (i = 0; i < length; i++) {
			Crypt_Int(Plain_Int[i], &Cipher_Int[i], Public_Key);
		}
		length = IntToHexChar(Cipher_Int, Cipher, length);
		for (i = 0; i < length; i++)
			printf("%c", Cipher[i]);
	}
	else {
		printf("Please input your Cipher.\n");
		scanf("%s", Cipher);
		length = HexCharToInt(Cipher, Cipher_Int, strlen(Cipher));
		for (i = 0; i < length; i++) {
			Crypt_Int(Cipher_Int[i], &Plain_Int[i], Secret_Key);
		}
		length = IntToChar(Plain_Int, Plain, length);
		for (i = 0; i < length; i++)
			printf("%c", Plain[i]);
	}

	putchar('\n');
	system("Pause");

	return 0;
}

void PrintPrime() {
	int i, j, count;
	count = 1;
	for (i = 101; i < 999; i++) {
		if (CheckPrime(i)) { 
			printf("%d\t", i); 
			if (count++ % 10 == 0)putchar('\n'); 
		}
	}
	putchar('\n');
}

int CheckPrime(int p) {
	int temp, i;
	temp = (int)sqrt((double)p);
	for (i = 2; i <= temp; i++) {
		if (p % i)continue;
		else break;
	}
	if (i > temp)return 1;
	return 0;
}

//求d模f的逆
int ExtendedEuclid(int f, int d, int *result) {
    int x1, x2, x3, y1, y2, y3, t1, t2, t3, q;
    x1 = y2 = 1;
    x2 = y1 = 0;
    x3 = (f >= d) ? f : d;
    y3 = (f >= d) ? d : f;
    while (1) {
        if (y3 == 0) {
            *result = x3; /* 两个数不互素则result为两个数的最大公约数，此时返回值为零 */
            return 0;
        }
        if (y3 == 1) {
			if (y2 < 0)y2 += f;
            *result = y2; /* 两个数互素则resutl为其乘法逆元，此时返回值为1 */
            return 1;
        }
        q = x3 / y3;
        t1 = x1 - q*y1;
        t2 = x2 - q*y2;
        t3 = x3 - q*y3;
        x1 = y1;
        x2 = y2;
        x3 = y3;
        y1 = t1;
        y2 = t2;
        y3 = t3;
    }
}

void Crypt_Int(int Data_In, int *Data_Out, key k) 
{
	int i;
	int mul = Data_In % k.N;
	Data_In = mul;
	for (i = 1; i < k.key; i++) {
		mul = (mul * Data_In) % k.N;
	}
	
	*Data_Out = mul;
}

int CharToInt(char Char_In[], int Int_Out[], int Length_Char)
{
	int i;
	for (i = 0; i < Length_Char; i++)
		Int_Out[i] = Char_In[i];
	return Length_Char;
}

int IntToHexChar(int Int_In[], char Char_Out[], int Length_Int)
{
	int i, j;
	for (i = 0; i < Length_Int; i++) {
		for (j = 0; j < 8; j++) {
			Char_Out[8 * i + j] = Int_In[i] >> (7 - j) * 4 & 0x0000000f;
			if (Char_Out[8 * i + j] > 9)Char_Out[8 * i + j] += 55;
			else Char_Out[8 * i + j] += 48;
		}
	}
	return Length_Int * 8;
}

int HexCharToInt(char Char_In[], int In_Out[], int Length_Char) {
	int i, j, temp;
	for (i = 0; i < Length_Char / 8; i++) {
		temp = 0;
		for (j = 0; j < 8; j++) {
			temp <<= 4;
			if (Char_In[i * 8 + j] >= 'A')Char_In[i * 8 + j] -= 55;
			else Char_In[i * 8 + j] -= 48;
			temp += Char_In[i * 8 + j];
		}
		In_Out[i] = temp;
	}
	return Length_Char / 8;
}

int IntToChar(int Int_In[], char Char_Out[], int Length_Int)
{
	int i;
	for (i = 0; i < Length_Int; i++) {
		Char_Out[i] = Int_In[i] & 0x000000ff;
	}
	return Length_Int;
}