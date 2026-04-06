# Method call benchmark
# Tests various method dispatch patterns

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

# Instance method calls
c = Counter.new
i = 0
while i < 5000
  c.increment
  i += 1
end

# String method calls
s = "hello"
2000.times do
  s.length
  s.size
end

# Integer method calls
total = 0
j = 0
while j < 5000
  total += j.to_s.length
  j += 1
end
