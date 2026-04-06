# Deep recursive calls to stress callinfo push/pop
def fib(n)
  if n < 2
    n
  else
    fib(n - 1) + fib(n - 2)
  end
end

i = 0
while i < 10
  fib(20)
  i += 1
end
