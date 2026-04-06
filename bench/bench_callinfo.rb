# Call info / stack frame benchmark
# Tests method call overhead

def add(a, b)
  a + b
end

def fib(n)
  if n < 2
    n
  else
    fib(n - 1) + fib(n - 2)
  end
end

# Many simple method calls
sum = 0
i = 0
while i < 5000
  sum = add(sum, i)
  i += 1
end

# Recursive calls
10.times do
  fib(15)
end
