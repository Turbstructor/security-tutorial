#include<stdio.h>
#include<time.h>

const int target = 1048576;
const int mega = 1e6;
const int giga = 1e9;

int performance_test(int loop_count, void (*target_func)(int)){
    clock_t start = clock();
    target_func(loop_count);
    double microsecs = ((double)clock() - start);

    fprintf(stdout, "(%-10d loops) - %9.1f msec\n", loop_count, microsecs / 1000.0f);
}

void use_mul(int cnt)
{
    int n, i;
    
    for(n = 1; n <= cnt; n++)
    {
        for(i = 1; i < target; i *= 2);
    }
}

void use_mul_alt(int cnt)
{
    int n, i;

    for(n = 1; n <= cnt; n++)
    {
        for(i = 1; i < target; i *= target);
    }
}

void use_shl(int cnt)
{
    int n, i;

    for(n = 1; n <= cnt; n++)
    {
        for(i = 1; i < target; i <<= 1);
    }
}

void use_shl_alt(int cnt)
{
    int n, i;

    for(n = 1; n <= cnt; n++)
    {
        for(i = 1; i < target; i <<= 20);
    }
}

void use_div(int cnt)
{
    int n, i;

    for(n = 1; n <= cnt; n++)
    {
        for(i = target; i > 0; i /= 2);
    }
}

void use_div_alt(int cnt)
{
    int n, i;

    for(n = 1; n <= cnt; n++)
    {
        for(i = target; i > 0; i /= target);
    }
}

void use_shr(int cnt)
{
    int n, i;

    for(n = 1; n <= cnt; n++)
    {
        for(i = target; i > 0; i >>= 1);
    }
}

void use_shr_alt(int cnt)
{
    int n, i;

    for(n = 1; n <= cnt; n++)
    {
        for(i = target; i > 0; i >>= 20);
    }
}

void use_mod(int cnt)
{
    int n, i;

    for(n = 1; n <= cnt; n++)
    {
        i = 3;
        i %= 2;
    }
}

void use_bwand(int cnt)
{
    int n, i;

    for(n = 1; n <= cnt; n++)
    {
        i = 3;
        i &= 1;
    }
}


int main()
{
    clock_t start;
    double us;

    fprintf(stdout, "starting 20 \"i * 2\"s...\n");
    performance_test(mega, use_mul);
    performance_test(giga, use_mul);

    fprintf(stdout, "\n");
    
    fprintf(stdout, "starting 20 \"i << 1\"s...\n");
    performance_test(mega, use_shl);
    performance_test(giga, use_shl);
    
    fprintf(stdout, "\n");
    fprintf(stdout, "================================================================================\n");
    

    fprintf(stdout, "starting \"i * 1048576\"s...\n");
    performance_test(mega, use_mul_alt);
    performance_test(giga, use_mul_alt);
    
    fprintf(stdout, "\n");
    
    fprintf(stdout, "starting \"i << 20\"s...\n");
    performance_test(mega, use_shl_alt);
    performance_test(giga, use_shl_alt);
    
    fprintf(stdout, "\n");
    fprintf(stdout, "================================================================================\n");


    fprintf(stdout, "starting 20 \"i / 2\"s...\n");
    performance_test(mega, use_div);
    performance_test(giga, use_div);

    fprintf(stdout, "\n");

    fprintf(stdout, "starting 20 \"i >> 1\"s...\n");
    performance_test(mega, use_shr);
    performance_test(giga, use_shr);

    fprintf(stdout, "\n");
    fprintf(stdout, "================================================================================\n");


    fprintf(stdout, "starting \"i / 1048576\"s...\n");
    performance_test(mega, use_div_alt);
    performance_test(giga, use_div_alt);

    fprintf(stdout, "\n");

    fprintf(stdout, "Starting \"i >> 20\"s...\n");
    performance_test(mega, use_shr_alt);
    performance_test(giga, use_shr_alt);

    fprintf(stdout, "\n");
    fprintf(stdout, "================================================================================\n");


    fprintf(stdout, "Starting \"i %% 2\"s...\n");
    performance_test(mega, use_mod);
    performance_test(giga, use_mod);

    fprintf(stdout, "\n");

    fprintf(stdout, "Starting \"i & 1\"s...\n");
    performance_test(mega, use_bwand);
    performance_test(giga, use_bwand);

    return 0;
}
