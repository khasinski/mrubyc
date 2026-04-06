# Benchmark: array operations with method calls
# Tests register cleanup with object references

def sum_array(arr)
  total = 0
  j = 0
  while j < arr.length
    total += arr[j]
    j += 1
  end
  total
end

i = 0
while i < 20000
  a = [1, 2, 3, 4, 5]
  sum_array(a)
  i += 1
end
