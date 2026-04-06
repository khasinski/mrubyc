# Frequent shallow method calls
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

c = Counter.new
i = 0
while i < 100000
  c.increment
  i += 1
end
c.value
