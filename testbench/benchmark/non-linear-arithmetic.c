int conditional(int x)
{
  if(x == 0)
    x = 2;

  int y = (x * x) + (x << 3) - 7;

  if (y ^ (y / x) == 1024)
    return 1; 

  return 0;
}

int non_linear_arithmetic(int x)
{
  if(conditional(x))
    return 1;
  else  
    return 0;
}

int main()
{
  int sym_x;
  klee_make_symbolic(&sym_x, sizeof(sym_x), "sym_x");
  return non_linear_arithmetic(sym_x);
}