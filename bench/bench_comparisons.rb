# Comparison operations benchmark
# Tests integer comparisons and branching

count = 0
i = 0
while i < 10000
  if i > 5000
    count += 1
  end
  if i < 3000
    count += 1
  end
  if i == 7777
    count += 1
  end
  if i != 0
    count += 1
  end
  if i >= 9000
    count += 1
  end
  if i <= 1000
    count += 1
  end
  i += 1
end
