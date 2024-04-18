int factorial(int n)
{
    if(n == 1)
        return 1;
    else    
        return n*factorial(n-1);
}

int main()
{
    int sym_n;
    klee_make_symbolic(&sym_n, sizeof(sym_n), "sym_n");
    return factorial(sym_n);
}
