# Benchmark: method calls with arguments
# Tests callinfo with various argument counts

def add2(a, b)
  a + b
end

def add3(a, b, c)
  a + b + c
end

def identity(x)
  x
end

i = 0
while i < 100000
  identity(i)
  add2(i, i)
  add3(i, i, i)
  i += 1
end
