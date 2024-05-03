int exponentiation(int x, int y)
{
    int result = 1;
    for(int i = 0;i < y; ++i)
        result *= x;

    return result;
}

int main()
{
    int sym_x, sym_y;
    klee_make_symbolic(&sym_x, sizeof(sym_x), "sym_x");
    klee_make_symbolic(&sym_y, sizeof(sym_y), "sym_y");
    return exponentiation(sym_x, sym_y);
}

