# Benchmark: comparisons and conditionals
# Method calls via operators that go through callinfo

i = 0
while i < 200000
  if i > 100
    x = i < 200000
  else
    x = i == 0
  end
  i += 1
end
