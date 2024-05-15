#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include <stdlib.h>

void encrypt(char *plaintext, char *key, char *ciphertext)
{
    int textLen = strlen(plaintext);
    int keyLen = strlen(key);

    for (int i = 0, j = 0; i < textLen; i++)
    {
        if (isalpha(plaintext[i]))
        {
            char offset = isupper(plaintext[i]) ? 'A' : 'a';
            ciphertext[i] = (plaintext[i] - offset + toupper(key[j % keyLen]) - 'A') % 26 + offset;
            j++;
        }
        else
        {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[textLen] = '\0';
}

void decrypt(char *ciphertext, char *key, char *plaintext)
{
    int textLen = strlen(ciphertext);
    int keyLen = strlen(key);

    for (int i = 0, j = 0; i < textLen; i++)
    {
        if (isalpha(ciphertext[i]))
        {
            char offset = isupper(ciphertext[i]) ? 'A' : 'a';
            plaintext[i] = (ciphertext[i] - offset - (toupper(key[j % keyLen]) - 'A') + 26) % 26 + offset;
            j++;
        }
        else
        {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[textLen] = '\0';
}

void handle_encrypt()
{
    char *plaintext;
    char *key;
    char ciphertext[256];
    plaintext = readline("请输入需要加密的文本:");
    key = readline("请输入key，该key可用于解密:");
    encrypt(plaintext, key, ciphertext);
    printf("加密后的内容：%s\n", ciphertext);
}

void handle_decrypt()
{
    char *ciphertext;
    char *key;
    char plaintext[256];
    ciphertext = readline("请输入需要解密的密文:");
    key = readline("请输入解密用的Key:");
    decrypt(ciphertext, key, plaintext);
    printf("解密后的内容:%s\n", plaintext);
}

void menu()
{
    printf("===================\n");
    printf("\t维吉尼亚加密算法\t\n");
    printf("请选择模式：\n");
    printf("1. 加密\n");
    printf("2. 解密\n");
    printf("0. 退出\n");
    printf("===================\n");
}

int main()
{
    int command;
    while (1)
    {
        menu();
        scanf("%d", &command);
        switch (command)
        {
        case 1:
            handle_encrypt();
            break;
        case 2:
            handle_decrypt();
            break;
        case 0:
            exit(0);
        default:
            perror("未知参数");
            exit(-1);
        }
    }
    return 0;
}
