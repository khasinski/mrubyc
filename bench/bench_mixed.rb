# Benchmark: mixed workload
# Combines method calls, string ops, and arithmetic

class Counter
  def initialize
    @count = 0
  end

  def increment
    @count += 1
  end

  def value
    @count
  end
end

def fib(n)
  if n < 2
    n
  else
    fib(n - 1) + fib(n - 2)
  end
end

c = Counter.new
i = 0
while i < 50000
  c.increment
  if i % 1000 == 0
    fib(10)
  end
  i += 1
end
