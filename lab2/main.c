#include "stdio.h"

int main()
{
    int array[10] = {-13, 12, -14, 76, -19, 22, 19, 18, 7};
    int A = 12, B = 18, S = 0;
    int i = 0;
    while (array[i] != A) i++;
    i++;
    for (; i < 10; i++)
    {
        if (array[i] == B) break;
        if (array[i] > 0) S+= array[i];
        if (S > 127)
        {
            printf("Error: overflow");
            return 1;
        }
    }
    printf("%d\n", S);
    return 0;
}
